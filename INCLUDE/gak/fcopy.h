/*
		Project:		GAKLIB
		Module:			fcopy.h
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

#ifndef FCOPY_H
#define FCOPY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

/*
#include <gak/directory.h>
#include <gak/strFiles.h>
#include <gak/exception.h>
*/
#include <gak/directoryEntry.h>
#include <gak/stdlib.h>
#include <gak/logfile.h>

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

struct FcopyWatcher
{
	bool operator ()( int, std::size_t )
	{
		return false;
	}
};

template <typename FcopyWatcherT>
void fcopy( const STRING &source, const STRING &destination, FcopyWatcherT &watcher )
{
	doEnterFunction("fcopy");

	const std::size_t	memory = 4UL*1024UL*1024UL;
	uint64				numProcessed = 0, 
						fileLen = 0;
#if defined( __BORLANDC__ ) && defined(  __MT__ )
	static void *buffer = NULL;
#endif


#if defined( __BORLANDC__ ) && defined(  __MT__ )
	if( !buffer )
	{
		if( (buffer=malloc(memory)) == NULL  )
/*@*/		throw AllocError();
	}
#else
	Buffer<void>	buffer( malloc(memory) );
	if( !buffer  )
/*@*/	throw AllocError();
#endif

	DirectoryEntry	theEntry(source);

	fileLen = theEntry.fileSize;

	bool canceled = false;
	STDfile handle_source( source, "rb" );
	if( handle_source )
	{
		STDfile handle_dest( destination, "wb" );
		if( handle_dest )
		{
			std::size_t	bytesRead;
			bool		error = false;
			do
			{
				bytesRead = fread( buffer, 1, memory, handle_source );
				if( fwrite( buffer, 1, bytesRead, handle_dest ) < bytesRead )
				{
					error = true;
					break;
				}

				numProcessed += bytesRead;
				if( watcher( unsigned(double(numProcessed)/double(fileLen) * 1000. + .5), bytesRead ) )
				{
					canceled = true;
/*v*/				break;
				}
			} while( bytesRead == memory );

			handle_dest.close();

			if( error || canceled )
			{
				int errCode = errno;
				strRemove(destination);
/*@*/			throw WriteError( destination ).addCerror( errCode );
			}
		}
		else
		{
/*@*/		throw OpenWriteError( destination ).addCerror();
		}
		handle_source.close();

		struct utimbuf	timeBuff;
		DirectoryEntry	stat( source );

		timeBuff.actime = stat.accessDate.getUtcUnixSeconds();
		timeBuff.modtime = stat.modifiedDate.getUtcUnixSeconds();

		if( timeBuff.actime < timeBuff.modtime )
		{
			timeBuff.actime = timeBuff.modtime;
		}
		if( strUtime( destination, &timeBuff ) )
		{
/*@*/		throw DateError( destination ).addCerror();
		}
	}
	else
	{
/*@*/	throw OpenReadError( source ).addCerror();
	}
}

inline void fcopy( const STRING &source, const STRING &destination )
{
	FcopyWatcher tmp;
	fcopy( source, destination, tmp );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// FCOPY_H