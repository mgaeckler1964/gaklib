/*
		Project:		GAKLIB
		Module:			strFiles.h
		Description:	check for UTF-8 characters in file names prior some
						standard calls.
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2023 Martin Gäckler

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

#ifndef GAK_STR_FILES_H
#define GAK_STR_FILES_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable: 4986 4820 4668 )
#endif

#include <sys/stat.h>

#if defined( _MSC_VER )
#include <sys/utime.h>
#else
#include <utime.h>
#endif

#ifndef _Windows
#include <fcntl.h>
#endif

#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#include <gak/string.h>

#include <gak/exception.h>

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

#ifdef _MSC_VER
#define S_ISDIR( mode ) (mode & S_IFDIR)
#endif

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

extern "C" 
{
	int utime( const char *filename, struct utimbuf *times );
	int _wutime( const wchar_t *filename, struct utimbuf *times );
}

#ifdef _Windows
FILE *strFopen( const STRING &filename, const STRING &mode );

int strCreat( const STRING &filename, int mode );
int strOpen( const STRING &filename, int flags );

int strStat( const STRING &filename, struct stat *statbuff );
int strAccess( const STRING &filename, int amode );

int strUtime( const STRING &filename, struct utimbuf *times );
int strRemove( const STRING &filename );
int strRmdir( const STRING &filename );
int strRename( const STRING &oldname, const STRING &newname );
#else
inline int strCreat( const STRING &fName, mode_t mode )
{
	return creat( fName.convertToCharset( STR_UTF8 ), mode );
}
inline int strOpen( const STRING &fName, int flags )
{
	return open( fName.convertToCharset( STR_UTF8 ), flags );
}

inline int strStat( const STRING &filename, struct stat *statbuff )
{
	return stat( filename.convertToCharset( STR_UTF8 ), statbuff );
}

inline int strAccess( const STRING &filename, int amode )
{
	return access( filename.convertToCharset( STR_UTF8 ), amode );
}

inline int strUtime( const STRING &path, struct utimbuf *times )
{
	return utime( path.convertToCharset( STR_UTF8 ), times );
}
inline FILE *strFopen( const STRING &path, const STRING &mode )
{
	return fopen( path.convertToCharset( STR_UTF8 ), mode );
}
inline int strRemove( const STRING &filename )
{
	return remove( filename.convertToCharset( STR_UTF8 ) );
}
inline int strRmdir( const STRING &filename )
{
	return rmdir( filename.convertToCharset( STR_UTF8 ) );
}
inline int strRename( const STRING &oldname, const STRING &newname )
{
	return rename(
		oldname.convertToCharset( STR_UTF8 ),
		newname.convertToCharset( STR_UTF8 )
	);
}
#endif

inline void strRemoveE( const STRING &filename )
{
	if( strRemove( filename ) )
	{
		throw RemoveError(filename);
	}
}
inline void strRmdirE( const STRING &filename )
{
	if( strRmdir( filename ) )
	{
		throw RemoveError(filename);
	}
}

inline void strRenameE( const STRING &oldname, const STRING &newname )
{
	if( strRename( oldname, newname ) )
	{
		throw RenameError(oldname, newname);
	}
}

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
