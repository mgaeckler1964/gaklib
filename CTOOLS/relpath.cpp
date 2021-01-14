/*
		Project:		GAKLIB
		Module:			relpath.cpp
		Description:	Generate relative or absolute paths
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

#include <gak/directory.h>
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

F_STRING makeRelPath( const STRING &basePathStr, const STRING &destinationStr )
{
	F_STRING	relPath;
	const char	*destination = destinationStr;
	const char	*basePath = basePathStr;

#if defined( __MSDOS__ ) || defined( _Windows )
	if( ansiToUpper( basePath[0] ) != ansiToUpper( destination[0] ) )
	{
		relPath = destinationStr;
	}
	else
	{
		basePath += 3;
		destination += 3;
#else
		basePath++;
		destination++;
#endif
		const char	*dirPos;
		size_t		dirLen;

		// remove leading directories, that are identical
		while( *basePath )
		{

			dirPos = strchr( basePath, DIRECTORY_DELIMITER );
			if( dirPos )
			{
				dirLen = dirPos - basePath +1;
#if defined( __MSDOS__ ) || defined( _Windows )
				if( !strncmpi( basePath, destination, dirLen ) )
#else
				if( !strncmp( basePath, destination, dirLen ) )
#endif
				{
					basePath += dirLen;
					destination += dirLen;
				}
				else
				{
/*v*/				break;
				}
			}
			else
			{
/*v*/			break;
			}
		}

		// add "../" for each direcory still in basePath
		while( *basePath )
		{
			dirPos = strchr( basePath, DIRECTORY_DELIMITER );
			if( dirPos )
			{
				relPath += "..";
				relPath += DIRECTORY_DELIMITER;
				basePath = dirPos + 1;
			}
			else
			{
/*v*/			break;
			}
		}

		relPath += destination;

#if defined( __MSDOS__ ) || defined( _Windows )
	}
#endif

	return relPath;
}

F_STRING makeFullPath( const STRING &basePath, const STRING &destinationStr )
{
	doEnterFunction("char *makeFullPath( const char *basePath, const char *destination )");

	const char	*destination = destinationStr;
	size_t		dirPos;
	F_STRING	fullPath;


#if defined( __MSDOS__ ) || defined( _Windows )
	if( basePath.isEmpty() || destination[1] == ':' )
#else
	if( basePath.isEmpty() || destination[0] == DIRECTORY_DELIMITER )
#endif
	{
		fullPath = destinationStr;
	}
	else
	{
		fullPath = basePath;

		// remove last filename
		dirPos = fullPath.searchRChar( DIRECTORY_DELIMITER );
		if( dirPos != STRING::no_index )
		{
			fullPath.cut( dirPos );
		}
		else
		{
			fullPath = NULL_STRING;
		}

		// go directory up
		while( !strncmp( destination, ".." DIRECTORY_DELIMITER_STRING, 3 ) )
		{
			dirPos = fullPath.searchRChar( DIRECTORY_DELIMITER );
			if( dirPos != STRING::no_index )
			{
				fullPath.cut( dirPos );
			}
			destination += 3;
		}

		// add remaining path
		if( *destination )
		{
			if( !fullPath.endsWith(DIRECTORY_DELIMITER) )
			{
				fullPath += DIRECTORY_DELIMITER;
			}
			fullPath += destination;
		}
	}
	return fullPath;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

