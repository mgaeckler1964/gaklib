/*
		Project:		GAKLIB
		Module:			wavefile.h
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

#ifndef WAVE_FILE_H
#define WAVE_FILE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <fstream>

#include <gak/types.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a2
#	ifdef __WIN32__
#		pragma option -pc
#	else
#		pragma option -po
#	endif
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

/* for little endian only */
static const uint32	riffMagic = 0x46464952;	// 'RIFF'
static const uint32 waveMagic = 0x45564157;	// 'WAVE'
static const uint32 fmtMagic = 0x20746D66;	// 'fmt '
static const uint32 dataMagic = 0x61746164;	// 'data'

static const uint32 pcmFORMAT = 1;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

#undef max
#undef min

class STRING;

class WaveFile
{
	public:
#ifdef _MSC_VER
#	pragma pack(push, 2)
#endif
	struct DataHeader
	{
		uint32	magic,
				size;
	};

	struct RiffHeader
	{
		DataHeader	riffDataHeader;

		uint32		typeMagic;
	};

	struct AudioFormat
	{
		uint16	format, numChannels;
		uint32	sampleRate,
				avgBytesPerSec;

		uint16	blockAllign,
				bitsPerSample;
	};

	struct FormatHeader
	{
		DataHeader	formatDataHeader;
		AudioFormat	format;
	};

#ifdef _MSC_VER
#	pragma pack( pop )
#endif

	private:
	RiffHeader		theHeader;
	FormatHeader	theFormat;
	DataHeader		theData;

	std::fstream	stream;
	bool			writing;
	size_t			bytesPerSample, numSamples;
	int				shift;

	void updateSize();

	public:
	WaveFile() : numSamples(0), writing(false)
	{
	}
	~WaveFile()
	{
		close();
	}

	void setBytesPerSample( uint16 bitsPerSample )
	{
		shift = 32-bitsPerSample;
		bytesPerSample = bitsPerSample / std::numeric_limits<unsigned char>::digits;
		if( bitsPerSample % std::numeric_limits<unsigned char>::digits )
		{
			++bytesPerSample;
		}
	}
	void setBitsPerSample( uint16 bitsPerSample )
	{
		theFormat.format.bitsPerSample = bitsPerSample;
		setBytesPerSample( bitsPerSample );
	}
	void createWaveFile( const STRING &fileName, uint32 sampleRate, uint16 numChannels, uint16 bitsPerSample );
	void openWaveFile( const STRING &fileName );

	void writeSample( int32 sample )
	{
		stream.write( reinterpret_cast<const char *>(&sample), bytesPerSample );
		++numSamples;
	}
	int32 readSample()
	{
		int32 sample=0;
		stream.read( reinterpret_cast<char *>(&sample), bytesPerSample );
		sample <<= shift;
		sample >>= shift;

		return sample;
	}

	void close()
	{
		if( writing && stream.is_open() )
		{
			updateSize();
			stream.close();
		}
	}

	int32 getPosClip() const
	{
		return std::numeric_limits<int32>::max() >> shift;
	}
	int32 getNegClip() const
	{
		return std::numeric_limits<int32>::min() >> shift;
	}

	size_t getNumSamples()
	{
		return theData.size / bytesPerSample;
	}
	uint16 getNumChannels()
	{
		return theFormat.format.numChannels;
	}
	size_t getNumSamplesPerChannel()
	{
		return getNumSamples() / getNumChannels();
	}

	double getTimeCode( size_t	sampleIdx ) const
	{
		return double(sampleIdx)/double(theFormat.format.sampleRate);
	}
	STRING getFormatedTimeCode( size_t sampleIdx ) const;
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
#	pragma option -a.
#	pragma option -p.
#endif

#endif