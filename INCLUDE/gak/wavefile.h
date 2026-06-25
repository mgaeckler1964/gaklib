/*
		Project:		GAKLIB
		Module:			wavefile.h
		Description:	Read and write wave files
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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
	RiffHeader		m_theHeader;
	FormatHeader	m_theFormat;
	DataHeader		m_theData;

	std::fstream	m_stream;
	bool			m_writing;
	size_t			m_bytesPerSample, m_numSamples;
	int				m_shift;

	void updateSize();

	public:
	WaveFile() : m_numSamples(0), m_writing(false)
	{
	}
	~WaveFile()
	{
		close();
	}

	void setBytesPerSample( uint16 bitsPerSample )
	{
		m_shift = 32-bitsPerSample;
		m_bytesPerSample = bitsPerSample / std::numeric_limits<unsigned char>::digits;
		if( bitsPerSample % std::numeric_limits<unsigned char>::digits )
		{
			++m_bytesPerSample;
		}
	}
	void setBitsPerSample( uint16 bitsPerSample )
	{
		m_theFormat.format.bitsPerSample = bitsPerSample;
		setBytesPerSample( bitsPerSample );
	}
	void createWaveFile( const STRING &fileName, uint32 sampleRate, uint16 numChannels, uint16 bitsPerSample );
	void openWaveFile( const STRING &fileName );

	void writeSample( int32 sample )
	{
		m_stream.write( reinterpret_cast<const char *>(&sample), m_bytesPerSample );
		++m_numSamples;
	}
	int32 readSample()
	{
		int32 sample=0;
		m_stream.read( reinterpret_cast<char *>(&sample), m_bytesPerSample );
		sample <<= m_shift;
		sample >>= m_shift;

		return sample;
	}

	void close()
	{
		if( m_writing && m_stream.is_open() )
		{
			updateSize();
			m_stream.close();
		}
	}

	int32 getPosClip() const
	{
		return std::numeric_limits<int32>::max() >> m_shift;
	}
	int32 getNegClip() const
	{
		return std::numeric_limits<int32>::min() >> m_shift;
	}

	size_t getNumSamples()
	{
		return m_theData.size / m_bytesPerSample;
	}
	uint16 getNumChannels()
	{
		return m_theFormat.format.numChannels;
	}
	size_t getNumSamplesPerChannel()
	{
		return getNumSamples() / getNumChannels();
	}

	double getTimeCode( size_t	sampleIdx ) const
	{
		return double(sampleIdx)/double(m_theFormat.format.sampleRate);
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