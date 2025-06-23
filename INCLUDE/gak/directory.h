/*
		Project:		GAKLIB
		Module:			directory.h
		Description:
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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
#ifndef GAK_DIRECTORY_H
#define GAK_DIRECTORY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifdef __cplusplus
#include <gak/btree.h>
#include <gak/directoryEntry.h>
#include <gak/array.h>
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
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef _Windows
#define ALL_FILES_PATTERN	"*.*"
#else
#define ALL_FILES_PATTERN	"*"
#endif

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum FStype { fsNOT_EXISTING, fsFILE, fsDIRECTORY };

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct DlinkOptions
{
	STRING	filePattern;
	bool	overwrite,
			noEmpty;
};

class DirectoryList : public Btree< DirectoryEntry, DynamicComparator<DirectoryEntry> >
{
	void dirtree( DirectoryList *root, const STRING &path, const STRING &filePattern = NULL_STRING );
	public:
	enum AccessType
	{
		WRITE = 02, READ = 04, READ_WRITE = 06
	};

	void findFiles( const STRING &path );
	void dirlist( const STRING &path, const STRING &filePattern = NULL_STRING );
	void dirtree( const STRING &path, const STRING &filePattern = NULL_STRING )
	{
		clear();
		dirtree( this, path, filePattern );
	}
	void checkAccess( AccessType accessType );

	using Btree<DirectoryEntry, DynamicComparator<DirectoryEntry> >::findElement;
	const DirectoryEntry *findElement( const STRING &fileName ) const
	{
		DirectoryEntry tmp;
		tmp.fileName = fileName;

		return findElement( tmp );
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

/* from flink.cpp */
void flink( const STRING &source, const STRING &destination );

/* from fProtect.cpp */
void fprotect( const STRING &file );
void funprotect( const STRING &file );

/* from dircopy.cpp */
void dcopy( const STRING &source, const STRING &destination );

/* from dirlink.cpp */
size_t dlink( const STRING &source, const STRING &destination, const DlinkOptions *options=NULL );


/* from makepath.cpp */
void makePath( const STRING &path );
void makeDirectory( const STRING &dirName );

/* from setcwd.cpp */
int setcwd( const STRING &newPath );

/* from getcwd.cpp */
F_STRING getcwd( void );

/* from relpath.cpp */
F_STRING makeRelPath( const STRING &basePath, const STRING &destination );
F_STRING makeFullPath( const STRING &basePath, const STRING &destination );

/* from dirlist.cpp */
FStype fileType( const wchar_t *fileName );
FStype fileType( const STRING &fileName );

#ifdef _Windows
FStype fileName( STRING *fileName );
#else
inline FStype fileName( F_STRING *fileName )
{
	return fileType( *fileName );
}
#endif

/* from filename.cpp */
#ifdef __BORLANDC__
#	pragma warn -inl
#endif
F_STRING fullPath( const F_STRING &fileName );

// split a file path to path with delimiter and name return extension without dot
F_STRING fsplit( const F_STRING &complette, F_STRING *path=NULL, F_STRING *file=NULL );
inline F_STRING getExtension( const F_STRING &complete )
{
	return fsplit( complete, NULL, NULL );
}
inline F_STRING removeExtension( const F_STRING &complete )
{
	ssize_t	slashPos = complete.searchRChar( DIRECTORY_DELIMITER );
	ssize_t	dotPos = complete.searchRChar( '.' );
	
	return (dotPos > slashPos) ? complete.leftString( dotPos ) : complete;
}

#ifdef __BORLANDC__
#	pragma warn +inl
#endif

/* from directory.cpp */

F_STRING getTempPath( void );
F_STRING getPersonalHome( void );
#if defined( _Windows )
F_STRING getPersonalDocs( void );
#elif defined( __MACH__ ) || defined( __ELF__ )
#	define getPersonalDocs() getPersonalHome()
#endif

#if defined( _Windows )
F_STRING getPersonalConfig( void );
#elif defined( __MACH__ )
#	define getPersonalConfig() (getPersonalHome() + DIRECTORY_DELIMITER_STRING "Library")
#elif defined( __ELF__ )
#	define getPersonalConfig() getPersonalHome()
#endif

void getUSBdrives( ArrayOfStrings *result );

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

inline bool exists( const F_STRING &fileName )
{
	return fileType(fileName) != fsNOT_EXISTING;
}
inline bool isFile( const F_STRING &fileName )
{
	return fileType(fileName) == fsFILE;
}
inline bool isDirectory( const F_STRING &fileName )
{
	return fileType(fileName) == fsDIRECTORY;
}

inline int setModTime( const F_STRING &fileName, time_t timeStamp )
{
	struct utimbuf times;
	times.actime = times.modtime = timeStamp;
	return strUtime( fileName, &times );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
