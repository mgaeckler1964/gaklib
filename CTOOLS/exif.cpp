/*
		Project:		GAKLIB
		Module:			EXIF.CPP
		Description:	Reading exif data from an image file
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/exif.h>
#include <gak/array.h>
#include <gak/ci_string.h>
#include <gak/fmtNumber.h>
#include <gak/wideString.h>
#include <gak/types.h>
#include <gak/logfile.h>

#if defined( __BORLANDC__ )
#	include <stdio.h>
#	include <vcl/sysutils.hpp>
#endif
// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a1
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const int EXIF_BYTE		= 1;
static const int EXIF_STRING	= 2;
static const int EXIF_SHORT		= 3;
static const int EXIF_LONG		= 4;
static const int EXIF_RATIONAL	= 5;
static const int EXIF_UNDEFINED	= 7;
static const int EXIF_SLONG		= 9;
static const int EXIF_SRATIONAL	= 10;

static const std::size_t	NOT_FOUND = std::numeric_limits<std::size_t>::max();

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct tiffHeader
{
	uint16	byteOrder, fix42;
	uint32	offset;
};

struct tagStructure
{
	uint16	tag, type;
	uint32	count, offset;
};

template <>
struct is_binary<tagStructure>
{
	static const bool value = true;
};
template <>
struct is_binary<tiffHeader>
{
	static const bool value = true;
};

struct IFDstructure
{
	uint16					count;
	PODarray<tagStructure>	tags;
	uint32					nextOffset;
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

static void readExif( FILE *fp, size_t tiffPos, size_t offset,	bool bigEndian, ImageMetaData *exif );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static size_t searchSegment( FILE *fp, size_t startPos, unsigned char segType )
{
	int		c, s1, s2, segSize;
	size_t	segmentPos = startPos;
	bool	found = false;

	fseek( fp, long(startPos), SEEK_SET );

	while( !feof( fp ) )
	{
		c = fgetc( fp );

		while( !feof( fp )&& c == 0xFF )
		{
			c = fgetc( fp );
			if( c == segType )
			{
				found = true;
				break;
			}

			segmentPos++;
		}

		if( found )
			break;

		segmentPos++;


		if( c != 0xD8 && c != 0xD9 && c != 0xDA && c )
		{
			s1 = fgetc( fp );
			s2 = fgetc( fp );
			if( s1 < 0 || s2 < 0 )
/*v*/			break;

			segSize = s1*256 + s2;

			segmentPos += segSize;
			fseek( fp, segSize-2, SEEK_CUR );
		}
	}

	return found ? segmentPos : NOT_FOUND;
}

static size_t searchSOI( FILE *fp )
{
	int		c;
	size_t	SOIpos = 0;
	bool	found = false;

	fseek( fp, 0, SEEK_SET );

	while( !feof( fp ) )
	{
		c = fgetc( fp );

		while( !feof( fp )&& c == 0xFF )
		{
			c = fgetc( fp );
			if( c == 0xD8 )
			{
				found = true;
				break;
			}

			SOIpos++;
		}

		if( found )
			break;

		SOIpos++;
	}

	return found ? SOIpos : NOT_FOUND;
}

static bool readTiffHeader( FILE *fp, struct tiffHeader *tiff )
{
	bool found = false;

	fread( tiff, sizeof( struct tiffHeader ), 1, fp );
	if( tiff->byteOrder == 0x4D4D )
	{
		tiff->fix42 = intimot( tiff->fix42 );
		tiff->offset = intlmot( tiff->offset );
	}
	if( (tiff->byteOrder == 0x4D4D || tiff->byteOrder == 0x4949) && tiff->fix42 == 42 )
		found = true;

	return found;
}

static size_t searchTiffHeader( FILE *fp, size_t exifPos, struct tiffHeader *tiff )
{
	char	ExifMagic[6];
	size_t	tiffPos = exifPos;
	int		c, s1, s2, exifSize;
	bool	found = false;

	fseek( fp, long(exifPos), SEEK_SET );

	// skip the marker APP1
	while( !feof( fp ) )
	{
		c = fgetc( fp );
		tiffPos++;

		while( !feof( fp )&& c == 0xFF )
		{
			c = fgetc( fp );
			tiffPos++;
			if( c == 0xE1 )
			{
				found = true;
				break;
			}
		}

		if( found )
			break;
	}

	if( found )
	{
		found = false;

		s1 = fgetc( fp );
		s2 = fgetc( fp );
		fread( ExifMagic, 1, 6, fp );
		tiffPos += 8;

		exifSize = s1*256 + s2;
		if( !strcmpi( ExifMagic, "Exif" ) && exifSize )
		{
			found = readTiffHeader( fp, tiff );
		}
	}

	return found ? tiffPos : NOT_FOUND;
}

static void readIFD( FILE *fp, size_t offset, bool bigEndian, struct IFDstructure *ifd )
{
	doEnterFunction("readIFD( FILE *fp, size_t offset, bool bigEndian, struct IFDstructure *ifd )");

	ifd->count = 0;
	ifd->nextOffset = 0;
	if( !fseek( fp, long(offset), SEEK_SET ) )
	{
		if( fread( &ifd->count, sizeof( ifd->count ), 1, fp ) == 1 )
		{
			if( bigEndian )
				ifd->count = intimot( ifd->count );

			ifd->tags.readFromFile( fp, ifd->count );
			if( bigEndian )
			{
				for( size_t i=0; i<ifd->count; i++ )
				{
					struct tagStructure &tag = ifd->tags[i];
					tag.tag = intimot( tag.tag );
					tag.type = intimot( tag.type );
					tag.count = intlmot( tag.count );
					if( tag.type == EXIF_SHORT && tag.count == 1 )
						tag.offset = intimot( (uint16)tag.offset );
					else
						tag.offset = intlmot( tag.offset );
				}
			}
			fread( &ifd->nextOffset, sizeof( ifd->nextOffset ), 1, fp );
			if( bigEndian )
				ifd->nextOffset = intlmot( ifd->nextOffset );
		}

	}
}

static STRING readString( FILE *fp, size_t tiffPos, size_t offset, size_t count )
{
	STRING	value;

	if( offset )
	{
		if( count <= 4 )
		{
			char tmpBuff[5];

			while( !(offset & 0xFF000000) )
				offset <<= 8;

			*((uint32*)tmpBuff) = uint32(offset);
			tmpBuff[4] = 0;
			value = tmpBuff;
		}
		else
		{
			offset += tiffPos;

			fseek( fp, long(offset), SEEK_SET );
			value.setMinSize( count );
			for( size_t i=0; !feof( fp ) && i<count; i++ )
				value += (char)fgetc( fp );
		}
	}

	return value.stripBlanks();
}

static void readRational( FILE *fp, size_t offset, bool bigEndian, size_t count, MetaRational *data )
{
	fseek( fp, long(offset), SEEK_SET );
	fread( data, sizeof( *data ), count, fp );

	while( count )
	{
		if( bigEndian )
		{
			data->numerator = (int32)intlmot((unsigned long)data->numerator);
			data->denominator = (int32)intlmot((unsigned long)data->denominator);
		}
		data->reduce();
		data++;
		count--;
	}
}

static void readEXIF( FILE *fp, size_t tiffPos, bool bigEndian, struct IFDstructure *ifd, ImageMetaData *exif )
{
	doEnterFunction("readEXIF( FILE *fp, size_t tiffPos, bool bigEndian, struct IFDstructure *ifd, struct exifData *exif )");

	for( size_t i=0; i<ifd->count; i++ )
	{
		const struct tagStructure &tag = ifd->tags[i];

		switch( tag.tag )
		{
			/*
				TIFF specific data
				==================
			*/
			// Tags relating to image data structure
			case 0x103:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->tiffData.Compression = (int16)tag.offset;
				break;
			case 0x112:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->tiffData.Orientation = (int16)tag.offset;
				break;
			case 0x213:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->tiffData.YCbCrPositioning = (int16)tag.offset;
				break;
			case 0x11A:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->tiffData.xResolution );
				break;
			case 0x11B:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->tiffData.yResolution );
				break;
			case 0x128:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->tiffData.ResolutionUnit = (int16)tag.offset;
				break;

			// Tags relating to recording offset
			case 0x201:
			case 0x202:
				// ignore thumbnail
				break;

			// Tags relating to image data characteristics
			case 0x214:
				if( tag.type == EXIF_RATIONAL && tag.count == 6 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 6, exif->tiffData.ReferenceBlackWhite );
				break;

			// Other tags
			case 0x0132:
				if( tag.type == EXIF_STRING )
					exif->tiffData.DateTime = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x010E:
				if( tag.type == EXIF_STRING )
					exif->tiffData.Description = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x010F:
				if( tag.type == EXIF_STRING )
					exif->tiffData.Manufacturer = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x0110:
				if( tag.type == EXIF_STRING )
					exif->tiffData.Model = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x0131:
				if( tag.type == EXIF_STRING )
					exif->tiffData.Software = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x013B:
				if( tag.type == EXIF_STRING )
					exif->tiffData.Artist = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x8298:
				if( tag.type == EXIF_STRING )
					exif->tiffData.Copyright = readString( fp, tiffPos, tag.offset, tag.count );
				break;

			/*
				EXIF specific data
				==================
			*/
			// tags relating to version
			case 0x9000:
				if( tag.type == EXIF_UNDEFINED && tag.count == 4 )
					*((uint32*)exif->exifData.ExifVersion) = tag.offset;
				break;
			case 0xA000:
				if( tag.type == EXIF_UNDEFINED && tag.count == 4 )
					*((uint32*)exif->exifData.FlashpixVersion) = tag.offset;
				break;
			// tags relating to image data characteristics
			case 0xA001:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.ColorSpace = (int16)tag.offset;
				break;

			// tags relating to image configuration
			case 0x9101:
				if( tag.type == EXIF_UNDEFINED && tag.count == 4 )
					*((uint32*)exif->exifData.ComponentsConfiguration) = tag.offset;
				break;
			case 0xA002:
				if( (tag.type == EXIF_SHORT || tag.type == EXIF_LONG) && tag.count == 1 )
					exif->exifData.PixelXDimension = (int32)tag.offset;
				break;
			case 0xA003:
				if( (tag.type == EXIF_SHORT || tag.type == EXIF_LONG) && tag.count == 1 )
					exif->exifData.PixelYDimension = (int32)tag.offset;
				break;


			// tags relating to user information
			case 0x927C:
				if( tag.type == EXIF_UNDEFINED )
				{
					fseek( fp, long(tiffPos + tag.offset), SEEK_SET );
					exif->exifData.MakersNote.readFromFile( fp, tag.count );
				}
				break;
			case 0x9286:
				if( tag.type == EXIF_UNDEFINED && tag.count >= 8 )
				{
					fseek( fp, long(tiffPos + tag.offset), SEEK_SET );
					CI_STRING	encoding;
					encoding.readFromFile( fp, 8 );
					if( encoding == "UNICODE" )
					{
						uSTRING	unicodeVersion;

						unicodeVersion.readFromFile( fp, (tag.count - 8)/2 );
						exif->exifData.UserComment = unicodeVersion.toString();

					}
					else // if( encoding.isEmpty() || encoding == "ASCII" || encoding == "JIS" )
					{
						exif->exifData.UserComment = readString( fp, tiffPos, tag.offset + 8, tag.count - 8 );
					}
				}
				else if( tag.type == EXIF_STRING )
					exif->exifData.UserComment = readString( fp, tiffPos, tag.offset, tag.count );
				break;

			// tags relating to related file information

			// tags relating to date and time
			case 0x9003:
				if( tag.type == EXIF_STRING )
					exif->exifData.DateTimeOriginal = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x9004:
				if( tag.type == EXIF_STRING )
					exif->exifData.DateTimeDigitized = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x9290:
				if( tag.type == EXIF_STRING )
					exif->exifData.SubsecTime = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x9291:
				if( tag.type == EXIF_STRING )
					exif->exifData.SubsecTimeOriginal = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0x9292:
				if( tag.type == EXIF_STRING )
					exif->exifData.SubsecTimeDigitzed = readString( fp, tiffPos, tag.offset, tag.count );
				break;

			// tags relating to picture taking conditions
			case 0x829A:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.ExposureTime );
				break;
			case 0x829D:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.FNumber );
				break;

			// Other tags
			case 0xA434:
				if( tag.type == EXIF_STRING )
					exif->exifData.LensModel = readString( fp, tiffPos, tag.offset, tag.count );
				break;
			case 0xA432:
				if( tag.type == EXIF_RATIONAL && tag.count == 4 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 4, exif->exifData.LensSpecification );
				break;
			case 0x8822:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.ExposureProgram = (int16)tag.offset;
				break;
			case 0x8827:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.PhotographicSensitivity = (int16)tag.offset;
				break;
			case 0x8830:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.SensitivityType = (int16)tag.offset;
				break;
			case 0x8833:
				if( tag.type == EXIF_LONG && tag.count == 1 )
					exif->exifData.ISOSpeed = (int32)tag.offset;
				break;
			case 0x9201:
				if( tag.type == EXIF_SRATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.ShutterSpeed );
				break;
			case 0x9202:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.Aperture );
				break;
			case 0x9204:
				if( tag.type == EXIF_SRATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.ExposureBiasValue );
				break;
			case 0x9205:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.MaxAperture );
				break;
			case 0x9206:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.SubjectDistance );
				break;
			case 0x9207:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.MeteringMode = (int16)tag.offset;
				break;
			case 0x9208:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.LightSource = (int16)tag.offset;
				break;
			case 0x9209:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.Flash = (int16)tag.offset;
				break;
			case 0x920A:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.FocalLength );
				break;
			case 0xA215:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.ExposureIndex );
				break;
			case 0xA217:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.SensingMethod = (int16)tag.offset;
				break;
			case 0xA300:
				if( tag.type == EXIF_UNDEFINED && tag.count == 1 )
					exif->exifData.FileSource = (char)tag.offset;
				break;

			case 0xA301:
				if( tag.type == EXIF_UNDEFINED && tag.count == 1 )
					exif->exifData.SceneType = (char)tag.offset;
				break;
			case 0xA302:
				if( tag.type == EXIF_UNDEFINED )
				{
					fseek( fp, long(tiffPos + tag.offset), SEEK_SET );
					exif->exifData.CFAPattern.readFromFile( fp, tag.count );
				}
				break;
			case 0xA401:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.CustomRendered = (int16)tag.offset;
				break;
			case 0xA402:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.ExposureMode = (int16)tag.offset;
				break;
			case 0xA403:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.WhiteBalance = (int16)tag.offset;
				break;
			case 0xA404:
				if( tag.type == EXIF_RATIONAL && tag.count == 1 )
					readRational( fp, tiffPos+tag.offset, bigEndian, 1, &exif->exifData.DigitalZoomRatio );
				break;
			case 0xA405:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.FocalLengthIn35mmFilm = (int16)tag.offset;
				break;
			case 0xA406:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.SceneCaptureType = (int16)tag.offset;
				break;
			case 0xA407:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.GainControl = (int16)tag.offset;
				break;
			case 0xA408:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.Contrast = (int16)tag.offset;
				break;
			case 0xA409:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.Saturation = (int16)tag.offset;
				break;
			case 0xA40A:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.Sharpness = (int16)tag.offset;
				break;
			case 0xA40C:
				if( tag.type == EXIF_SHORT && tag.count == 1 )
					exif->exifData.SubjectDistanceRange = (int16)tag.offset;
				break;


			/*
				another chunk of data
			*/
			case 0x8769: //exif specific
			case 0x8825: // GPS
				if( tag.type == EXIF_LONG && tag.count == 1 )
				{
					readExif( fp, tiffPos, tag.offset, bigEndian, exif );
				}
				break;

			/*
				not yet supported tags
			*/

			default:
				break;
		}
	}
}

static void readExif( FILE *fp, size_t tiffPos, size_t offset,	bool bigEndian, ImageMetaData *exif )
{
	doEnterFunction("readEXIF( FILE *fp, size_t tiffPos, size_t offset,	bool bigEndian, struct exifData *exif )");

	struct IFDstructure		ifd;

	while( offset && !feof( fp ) )
	{
		readIFD( fp, tiffPos + offset, bigEndian, &ifd );
		readEXIF( fp, tiffPos, bigEndian, &ifd, exif );
		offset = ifd.nextOffset;
	}
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

const char *ExifMetaData::getMeteringMode( int16 MeteringMode )
{
	const char *meteringModeStr;

	if( MeteringMode >= 0 )
	{
		static const char *metering[] = { "Unknown", "Average", "Center-weighted average", "Spot", "Multi-Spot", "Multi-Segment", "Partial" };
		if( MeteringMode < sizeof(metering)/sizeof(metering[0]) )
			meteringModeStr = metering[MeteringMode];
		else
			meteringModeStr = "Other";
	}
	else
		meteringModeStr = "";

	return meteringModeStr;
}

const char *ExifMetaData::getExposureProgram( int16 ExposureProgram )
{
	const char *exposureProgramStr;

	if( ExposureProgram >= 0 )
	{
		static const char *exposure[] = { "Not defined", "Manual", "Normal program", "Aperture priority", "Shutter priority", "Creative program", "Action program", "Portrait mode", "Landscape mode" };
		if( ExposureProgram < sizeof(exposure)/sizeof(exposure[0]) )
			exposureProgramStr = exposure[ExposureProgram];
		else
			exposureProgramStr = "Other";
	}
	else
		exposureProgramStr = "";

	return exposureProgramStr;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING ExifMetaData::getLens( void ) const
{
	STRING lens = this->LensModel;

	if( lens.isEmpty() || lens.beginsWithI( "Unknown" ) )
	{
		long minFocus = LensSpecification[0].getAsLong();
		long maxFocus = LensSpecification[1].getAsLong();
		double minAperture = LensSpecification[2].getAsDouble();
		double maxAperture = LensSpecification[3].getAsDouble();

		if( minFocus > 0 )
		{
			lens = "";
			lens += formatNumber( (unsigned long)minFocus );
			if( maxFocus > minFocus )
			{
				lens += '-';
				lens += formatNumber( (unsigned long)maxFocus );
			}
			lens += "mm f/";
			lens += formatFloat( minAperture, 0, 1 );
			if( maxAperture > minAperture )
			{
				lens += '-';
				lens += formatFloat( maxAperture, 0, 1 );
			}
		}
	}
	return lens;
}

const char *ExifMetaData::getStrobeReturnStr( void ) const
{
	static const char *returnStr[] = {
		"No strobe return detection function",
		"reserved",
		"Strobe return light not detected",
		"Strobe return light detected"
	};

	return returnStr[ getStrobeReturn() ];
}

const char *ExifMetaData::getFlashModeStr( void ) const
{
	static const char *flashStr[] = {
		"unknown",
		"Compulsory flash firing",
		"Compulsory flash suppression",
		"Auto mode"
	};

	return flashStr[ getFlashMode() ];
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

bool readImageMetaData( FILE *fp, ImageMetaData *metaData )
{
	doEnterFunction("readImageMetaData( FILE *fp, ImageMetaData *metaData )");

	size_t				SOIpos, exifPos;
	struct tiffHeader	tiff;

	bool 				found;
	bool 				bigEndian = false;
	size_t				tiffPos = 0;

	found = readTiffHeader( fp, &tiff );

	/*
		check for a JPEG file
	*/
	if( !found )
	{
		SOIpos = searchSOI( fp );
		if( SOIpos != NOT_FOUND )
		{
			exifPos = searchSegment( fp, SOIpos, 0xE1 );
			if( exifPos != NOT_FOUND )
			{
				tiffPos = searchTiffHeader( fp, exifPos, &tiff );
				if( tiffPos != NOT_FOUND )
					found = true;
			}
		}
	}

	/*
		did we found the TIFF header?
	*/
	if( found )
	{
		if( tiff.byteOrder == 0x4D4D )
			bigEndian = true;
		readExif( fp, tiffPos, tiff.offset, bigEndian, metaData );
		found = true;
	}
	return found;
}

bool readImageMetaData( const STRING &fileName, ImageMetaData *metaData )
{
	doEnterFunction("readImageMetaData( const STRING &fileName, ImageMetaData *metaData )");

	bool 	found = false;
	STDfile fp( fileName, "rb" );
	if( fp )
		found = readImageMetaData( fp, metaData );

	return found;
}

#if defined( __BORLANDC__ )
TDateTime parseExifTimestamp( const STRING &timestamp )
{
	int year, month, day, hour, minute, seconds;

	sscanf( timestamp, "%d:%d:%d %d:%d:%d", &year, &month, &day, &hour, &minute, &seconds );

	return EncodeDate( (Word)year, (Word)month, (Word)day ) + EncodeTime( (Word)hour, (Word)minute, (Word)seconds, 0 );
}
#endif

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

