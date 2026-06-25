/*
		Project:		GAKLIB
		Module:			waveFile.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/string.h>
#include <gak/wavefile.h>
#include <gak/exception.h>
#include <gak/iostream.h>
#include <gak/fmtNumber.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
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

void WaveFile::updateSize()
{
	size_t newSize = m_theHeader.riffDataHeader.size + m_bytesPerSample * m_numSamples;
	if( newSize > std::numeric_limits<uint32>::max() )
		throw IntegerOverflowError("size_t value");

	m_theHeader.riffDataHeader.size = uint32(newSize);
	m_stream.seekp( 0, std::ios_base::beg );
	binaryToBinaryStream( m_stream, m_theHeader );

	m_stream.seekp( sizeof(FormatHeader), std::ios_base::cur );

	m_theData.size = uint32(m_bytesPerSample * m_numSamples);
	binaryToBinaryStream( m_stream, m_theData );
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void WaveFile::createWaveFile( const STRING &fileName, uint32 sampleRate, uint16 numChannels, uint16 bitsPerSample )
{
	m_stream.open( fileName, std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios::binary );
	m_writing = true;

	m_theHeader.riffDataHeader.magic = riffMagic;
	m_theHeader.riffDataHeader.size = sizeof( RiffHeader ) + sizeof( FormatHeader ) - 8;
	m_theHeader.typeMagic = waveMagic;

	binaryToBinaryStream( m_stream, m_theHeader );

	m_theFormat.formatDataHeader.magic = fmtMagic;
	m_theFormat.formatDataHeader.size = sizeof( AudioFormat );
	m_theFormat.format.format = pcmFORMAT;
	m_theFormat.format.numChannels = numChannels;
	m_theFormat.format.sampleRate = sampleRate;
	setBitsPerSample( bitsPerSample );

	m_theFormat.format.blockAllign = m_theFormat.format.numChannels * ((m_theFormat.format.bitsPerSample+7)/8);
	m_theFormat.format.avgBytesPerSec = m_theFormat.format.sampleRate * m_theFormat.format.blockAllign;

	binaryToBinaryStream( m_stream, m_theFormat );

	m_theData.magic = dataMagic;
	m_theData.size = 0;
	binaryToBinaryStream( m_stream, m_theData );
}

void WaveFile::openWaveFile( const STRING &fileName )
{
	m_stream.open( fileName, std::ios_base::in | std::ios::binary );

	binaryFromBinaryStream( m_stream, &m_theHeader );
	if( m_theHeader.riffDataHeader.magic != riffMagic )
	{
		throw BadHeaderError( STRING("riffMagic: ") + fileName );
	}
	if( m_theHeader.typeMagic != waveMagic )
	{
		throw BadHeaderError( STRING("waveMagic: ") + fileName );
	}

	while( m_stream )
	{
		binaryFromBinaryStream( m_stream, &m_theFormat.formatDataHeader );
		if( m_theFormat.formatDataHeader.magic != fmtMagic )
		{
			m_stream.seekp( m_theFormat.formatDataHeader.size, std::ios_base::cur );
		}
		else if( m_theFormat.formatDataHeader.size == sizeof( m_theFormat.format ) )
		{
			binaryFromBinaryStream( m_stream, &m_theFormat.format );
			if( m_theFormat.format.format != pcmFORMAT )
			{
				throw BadHeaderError( STRING("This is not in PCM format: ") + fileName );
			}
			if( m_theFormat.format.bitsPerSample != 16 && m_theFormat.format.bitsPerSample != 24 )
			{
				throw BadHeaderError( STRING("Unsupported sample size: ") + formatNumber(m_theFormat.format.bitsPerSample) + ' ' + fileName );
			}
			break;
		}
		else
		{
			throw BadHeaderError( STRING("format size: ") + fileName );
		}
	}

	if( m_theFormat.formatDataHeader.magic != fmtMagic )
	{
		throw BadHeaderError( STRING("fmtMagic: ") + fileName );
	}

	while( m_stream )
	{
		binaryFromBinaryStream( m_stream, &m_theData );
		if( m_theData.magic != dataMagic )
		{
			m_stream.seekp( m_theData.size, std::ios_base::cur );
		}
		else
		{
			break;
		}
	}

	if( m_theData.magic != dataMagic )
	{
		throw BadHeaderError( STRING("dataMagic: ") + fileName );
	}

	setBytesPerSample( m_theFormat.format.bitsPerSample );
}

STRING WaveFile::getFormatedTimeCode( size_t sampleIdx ) const
{
	double		seconds = getTimeCode( sampleIdx );
	unsigned	hour = unsigned(seconds)/3600;
	STRING		result = formatNumber( hour, 2 ) + ':';
	seconds -= hour*3600;

	unsigned	minute = unsigned(seconds)/60;
	result += formatNumber( minute, 2 ) + ':';
	seconds -= minute*60;
	result += formatNumber( seconds );

	return result;
}

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

