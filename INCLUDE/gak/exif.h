/*
		Project:		GAKLIB
		Module:			exif.h
		Description:
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

#ifndef GAK_EXIF_H
#define GAK_EXIF_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/string.h>
#include <gak/array.h>
#include <gak/math.h>

#if defined( __BORLANDC__  )
#	include <system.hpp>
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

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class MetaRational
{
	public:
	int32 numerator, denominator;

	MetaRational() : numerator(-1), denominator(0) {};
	double getAsDouble( void ) const
	{
		return denominator
			? (double)numerator/(double)denominator
			: -99999999
		;
	}
	int32 getAsLong( void ) const
	{
		return int32(getAsDouble() + 0.5);
	}
	void reduce( void )
	{
		if( math::abs( numerator ) < 10000000L
		&&  denominator && math::abs( denominator ) < 10000000L )
		{
			math::Fraction fraction( numerator, static_cast<unsigned long>(denominator) );
			fraction.reduce();
			numerator = int32(fraction.numerator);
			denominator = int32(fraction.denominator);
		}
	}
};

class TiffMetaData
{
	public:
	STRING			DateTime, Description, Manufacturer, Model, Software,
					Artist, Copyright;

	MetaRational	xResolution, yResolution, ReferenceBlackWhite[6];

	int16			Orientation, ResolutionUnit, YCbCrPositioning,
					Compression;

	TiffMetaData()
	{
		Orientation = ResolutionUnit = YCbCrPositioning = Compression = -1;
	}
};

class ExifMetaData
{
	static const char *metering[];

	public:
	STRING			DateTimeOriginal, DateTimeDigitized, SubsecTime,
					SubsecTimeOriginal, SubsecTimeDigitzed, UserComment,
					LensModel;

	MetaRational	ExposureTime, FNumber, ShutterSpeed, Aperture,
					ExposureBiasValue, MaxAperture, SubjectDistance,
					FocalLength, ExposureIndex, DigitalZoomRatio,
					LensSpecification[4];

	ArrayOfData		MakersNote, CFAPattern;

	char			FileSource, SceneType;

	int16			ColorSpace, SubjectDistanceRange, Contrast, Saturation,
					Sharpness, FocalLengthIn35mmFilm, GainControl,
					ExposureProgram, PhotographicSensitivity, SensitivityType,
					MeteringMode, LightSource, Flash, SensingMethod,
					CustomRendered, ExposureMode, WhiteBalance,
					SceneCaptureType;

	int32			ISOSpeed, PixelXDimension, PixelYDimension;
	char			ExifVersion[5], FlashpixVersion[5],
					ComponentsConfiguration[4];


	ExifMetaData()
	{
		FileSource = SceneType = -1;

		*((int32*)ExifVersion) = 0;
		ExifVersion[4] = 0;
		*((int32*)FlashpixVersion) = 0;
		FlashpixVersion[4] = 0;
		*((int32*)ComponentsConfiguration) = 0;

		SubjectDistanceRange = ColorSpace = ExposureProgram = SensitivityType =
		MeteringMode = LightSource = Flash = ExposureMode = WhiteBalance =
		PhotographicSensitivity =	SensingMethod = CustomRendered =
		FocalLengthIn35mmFilm = SceneCaptureType = GainControl = Contrast =
		Saturation = Sharpness = -1;

		ISOSpeed = PixelXDimension = PixelYDimension = -1;
	}

	STRING getLens( void ) const;
	int32 getISO( void ) const
	{
		if( ISOSpeed > 0 )
			return ISOSpeed;
		else
			return PhotographicSensitivity;
	}

	bool getFlashFired( void ) const
	{
		return Flash & 1;
	}
	int16 getStrobeReturn( void ) const
	{
		return (int16)((Flash >> 1) & 3);
	}
	const char *getStrobeReturnStr( void ) const;
	int16 getFlashMode( void ) const
	{
		return (int16)((Flash >> 3) & 3);
	}
	const char *getFlashModeStr( void ) const;
	bool getFlashFunction( void ) const
	{
		return (Flash >> 5) & 1;
	}
	bool getRedEyeMode( void ) const
	{
		return (Flash >> 6) & 1;
	}

	static const char *getMeteringMode( int16 MeteringMode );
	const char *getMeteringMode( void ) const
	{
		return getMeteringMode( MeteringMode );
	}

	static const char *getExposureProgram( int16 ExposureProgram );
	const char *getExposureProgram( void ) const
	{
		return getExposureProgram( ExposureProgram );
	}
};

class ImageMetaData
{
	public:
	TiffMetaData			tiffData;
	ExifMetaData			exifData;

	STRING getLens( void ) const
	{
		return exifData.getLens();
	}

	int32 getISO( void ) const
	{
		return exifData.getISO();
	}
	bool getFlashFired( void ) const
	{
		return exifData.getFlashFired();
	}
	int16 getStrobeReturn( void ) const
	{
		return exifData.getStrobeReturn();
	}
	const char *getStrobeReturnStr( void ) const
	{
		return exifData.getStrobeReturnStr();
	}

	int16 getFlashMode( void ) const
	{
		return exifData.getFlashMode();
	}
	const char *getFlashModeStr( void ) const
	{
		return exifData.getFlashModeStr();
	}
	bool getFlashFunction( void ) const
	{
		return exifData.getFlashFunction();
	}
	bool getRedEyeMode( void ) const
	{
		return exifData.getRedEyeMode();
	}
	const char *getMeteringMode( void ) const
	{
		return exifData.getMeteringMode();
	}
	const char *getExposureProgram( void ) const
	{
		return exifData.getExposureProgram();
	}
};

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

bool readImageMetaData( const STRING &fileName, ImageMetaData *exif );
bool readImageMetaData( FILE *fp, ImageMetaData *exif );
#if defined( __BORLANDC__ )
TDateTime parseExifTimestamp( const STRING &timestamp );
#endif	

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
