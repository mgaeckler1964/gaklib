/*
		Project:		GAKLIB
		Module:			filename.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/directory.h>

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

namespace gak
{

	F_STRING fsplit( const F_STRING &complete, F_STRING *path, F_STRING *file )
	{
		ssize_t	slashPos = complete.searchRChar( DIRECTORY_DELIMITER );
		if( slashPos != STRING::no_index )
		{
			if( path )
			{
				*path = complete.leftString( slashPos +1 );
			}
			if( file )
			{
				*file = complete.subString( slashPos + 1 );
			}
		}
		else
		{
			if( path )
				*path = NULL_STRING;
			if( file )
				*file = complete;
		}
	
		ssize_t	dotPos = complete.searchRChar( '.' );
	
		return (dotPos > slashPos) ? complete.subString( dotPos+1 ) : NULL_STRING;
	}

	F_STRING fullPath( const F_STRING &fileName )
	{
		F_STRING curPath = getcwd() + DIRECTORY_DELIMITER_STRING ".";

		return makeFullPath( curPath, fileName );
	}

}	// namespace gak


/*
	old stuff no longer supported
*/
extern "C" {

/*
	scan a filespec parameter
*/
#if 0
char *scanFileArg( char *fileSpec, const char *fileArg )
{
	char c, *cp;

	strcpy( fileSpec, fileArg );
	for(	cp=fileSpec+strlen( fileSpec );
			(c = *cp) != DIRECTORY_DELIMITER && c != ':' && cp >= fileSpec;
			cp-- )
	;

	cp++;
	if( !*cp && (c == DIRECTORY_DELIMITER || cp == fileSpec+2) )
	{
		*(cp+0) = '*';
		*(cp+1) = '.';
		*(cp+2) = '*';
		*(cp+3) = 0;
	}

	return cp;
}
#endif

/*
	generate full path specification
*/
#if 0
void fullpath( char *fullspec, const char *halfspec )
{
	int path, i, j;

	path = 0;
	if( *halfspec == '/' )
	{
		*fullspec++ = '/';
		halfspec++;
	}
	fullspec[1] = ':';
	if( halfspec[1] == ':' )
	{
		fullspec[0] = char(ansiToUpper( halfspec[0] ));
		path = 2;
	}
	else
		fullspec[0] = (char)(Dgetdrv() + 'A');
	if( halfspec[path] == '\\' )
		strcpy( fullspec+2, &(halfspec[path]) );
	else
	{
		Dgetpath( fullspec+2, fullspec[0] - 'A' + 1);
		strcat( fullspec, "\\" );
		strcat( fullspec, &(halfspec[path]) );
	}
	i = 0;
	do
	{
		if( fullspec[i] == '.' && fullspec[i-1] == '\\')
		{
			if( fullspec[i+1] == '.' )
			{
				j = i - 1;
				while( j > 0 )
					if( fullspec[--j] == '\\' ) break;
				if( j >= 0 )
				{
					strcpy( fullspec + j, fullspec + i + 2);
					i = j;
				}
			}
		}
	} while( fullspec[i++] );
#ifndef _Windows
	strupr( fullspec );
#endif
}
#endif

#ifdef __TOS__
/*
	transform GEM-Dialog-filename to DOS-filename
*/
void compactName( const char *fName, char *buffer )
{
	int i, j;

	for( i = 0; fName[i] != ' '; i++ )
		buffer[i] = fName[i];
	buffer[i++] = '.';
	
	for( j = 9; fName[j] != ' ' && fName[j]; i++, j++ )
		buffer[i] = fName[j];
	if( j == 9 )
		buffer[i - 1] = 0;
	else
		buffer[i] = 0;
}

/*
	transform DOS-filename to GEM-Dialog-filename
*/
void expandName( const char *fName, char *buffer )
{
	char	c;
	int	i, j;

	for( i = 0; (c = fName[i]) != 0 && c != '.'; i++ )
		buffer[i] = c;
	j = i +1;

	/* fill blanks */
	for( ; i < 8 + 1; i++ )
		buffer[i] = ' ';

	/* copy extension */
	if( c )
		for( ; (c = fName[j]) != 0; i++, j++ )
			buffer[i] = c;

	/* fill blanks */
	for( ; i < 12; i++ )
		buffer[i] = ' ';
	buffer[i] = 0;
}

#endif

}	// extern "C"

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

