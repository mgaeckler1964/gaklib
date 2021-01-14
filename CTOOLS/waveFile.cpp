/*
		Project:		GAKLIB
		Module:			waveFile.cpp
		Description:	Read and write wave files
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

void WaveFile::updateSize(  )
{
	theHeader.riffDataHeader.size += bytesPerSample * numSamples;
	stream.seekp( 0, std::ios_base::beg );
	binaryToBinaryStream( stream, theHeader );

	stream.seekp( sizeof(FormatHeader), std::ios_base::cur );

	theData.size = bytesPerSample * numSamples;
	binaryToBinaryStream( stream, theData );
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
	stream.open( fileName, std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios::binary );
	writing = true;

	theHeader.riffDataHeader.magic = riffMagic;
	theHeader.riffDataHeader.size = sizeof( RiffHeader ) + sizeof( FormatHeader ) - 8;
	theHeader.typeMagic = waveMagic;

	binaryToBinaryStream( stream, theHeader );

	theFormat.formatDataHeader.magic = fmtMagic;
	theFormat.formatDataHeader.size = sizeof( AudioFormat );
	theFormat.format.format = pcmFORMAT;
	theFormat.format.numChannels = numChannels;
	theFormat.format.sampleRate = sampleRate;
	setBitsPerSample( bitsPerSample );

	theFormat.format.blockAllign = theFormat.format.numChannels * ((theFormat.format.bitsPerSample+7)/8);
	theFormat.format.avgBytesPerSec = theFormat.format.sampleRate * theFormat.format.blockAllign;

	binaryToBinaryStream( stream, theFormat );

	theData.magic = dataMagic;
	theData.size = 0;
	binaryToBinaryStream( stream, theData );
}

void WaveFile::openWaveFile( const STRING &fileName )
{
	stream.open( fileName, std::ios_base::in | std::ios::binary );

	binaryFromBinaryStream( stream, &theHeader );
	if( theHeader.riffDataHeader.magic != riffMagic )
	{
		throw BadHeaderError( STRING("riffMagic: ") + fileName );
	}
	if( theHeader.typeMagic != waveMagic )
	{
		throw BadHeaderError( STRING("waveMagic: ") + fileName );
	}

	while( stream )
	{
		binaryFromBinaryStream( stream, &theFormat.formatDataHeader );
		if( theFormat.formatDataHeader.magic != fmtMagic )
		{
			stream.seekp( theFormat.formatDataHeader.size, std::ios_base::cur );
		}
		else if( theFormat.formatDataHeader.size == sizeof( theFormat.format ) )
		{
			binaryFromBinaryStream( stream, &theFormat.format );
			if( theFormat.format.format != pcmFORMAT )
			{
				throw BadHeaderError( STRING("This is not in PCM format: ") + fileName );
			}
			if( theFormat.format.bitsPerSample != 16 && theFormat.format.bitsPerSample != 24 )
			{
				throw BadHeaderError( STRING("Unsupported sample size: ") + formatNumber(theFormat.format.bitsPerSample) + ' ' + fileName );
			}
			break;
		}
		else
		{
			throw BadHeaderError( STRING("format size: ") + fileName );
		}
	}

	if( theFormat.formatDataHeader.magic != fmtMagic )
	{
		throw BadHeaderError( STRING("fmtMagic: ") + fileName );
	}

	while( stream )
	{
		binaryFromBinaryStream( stream, &theData );
		if( theData.magic != dataMagic )
		{
			stream.seekp( theData.size, std::ios_base::cur );
		}
		else
		{
			break;
		}
	}

	if( theData.magic != dataMagic )
	{
		throw BadHeaderError( STRING("dataMagic: ") + fileName );
	}

	setBytesPerSample( theFormat.format.bitsPerSample );
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

