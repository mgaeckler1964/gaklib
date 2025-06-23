/*
		Project:		GAKLIB
		Module:			dirlist.cpp
		Description:	directory (tree) scanner
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifdef _Windows
#	ifndef STRICT
#		define STRICT 1
#	endif
#include <windows.h>
#endif

#if defined( __MACH__ ) || defined( __unix__ )
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#include <gak/gaklib.h>
#include <gak/directory.h>
#include <gak/wideString.h>
#include <gak/strFiles.h>
#include <gak/exception.h>
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

void DirectoryList::findFiles( const STRING &path )
{
	doEnterFunctionEx(gakLogging::llDetail, "DirectoryList::findFiles");

#if defined( _Windows )
	WIN32_FIND_DATAW	findDataW;
	STRING				file;
	uSTRING				_tmpBuff;

	unsigned long ntErrCode;
	HANDLE fs = FindFirstFileW( uSTRING(path), &findDataW );
	if( fs != INVALID_HANDLE_VALUE )
	{
		clear();

		do
		{
			_tmpBuff = findDataW.cFileName;

			addElement( 
				DirectoryEntry( 
					_tmpBuff.toString(), (uint64(findDataW.nFileSizeHigh) << uint64(32)) | uint64(findDataW.nFileSizeLow),
					DateTime(findDataW.ftCreationTime), DateTime(findDataW.ftLastWriteTime), DateTime(findDataW.ftLastAccessTime), 
					findDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, 
					findDataW.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM),
					findDataW.dwFileAttributes & FILE_ATTRIBUTE_READONLY
				)
			);

		} while( FindNextFileW( fs, &findDataW ) );
		FindClose( fs );
	}
	else if( (ntErrCode = GetLastError()) != ERROR_FILE_NOT_FOUND && ntErrCode != ERROR_PATH_NOT_FOUND )
	{
		throw OpenReadError( path ).addNTerror( ntErrCode );
	}

#elif defined( __MACH__ ) || defined( __unix__ )

	size_t			slashPos = path.searchRChar( DIRECTORY_DELIMITER );
	STRING			myPath = slashPos != -1
		? path.leftString( slashPos )
		: STRING(".")
	;
	STRING			pattern = path + (size_t)(slashPos +1);
	struct dirent	*file;
	DIR				*dir = opendir( myPath );
	STRING			name;
	STR_CHARSET     charSet;

	if( pattern == "*.*" )
		pattern = "*";
	if( dir )
	{
		clear();
		while( (file = readdir( dir )) != NULL )
		{
			name = file->d_name;
			if( name.match( pattern ) )
			{
				DirectoryEntry		newElement;

				charSet = name.testCharSet();
				if( charSet == STR_ANSI )
					charSet = STR_UTF8;
				name.setCharSet( charSet );

				newElement.findFile( myPath + DIRECTORY_DELIMITER + name );
				newElement.fileName = name;
				newElement.directory = (file->d_type == DT_DIR);
				newElement.hidden = name[0U] == '.';

				addElement( newElement );
			}
		}
		closedir( dir );
	}
	else
		throw OpenReadError( path ).addCerror();
#else
#	error "Unkown OS"
#endif
}

void DirectoryList::dirlist( const STRING &path, const STRING &filePattern )
{
	doEnterFunctionEx(gakLogging::llDetail, "DirectoryList::dirlist");
	STRING			dir = path;

	if( !dir.endsWith( DIRECTORY_DELIMITER ) )
		dir += DIRECTORY_DELIMITER_STRING ALL_FILES_PATTERN;
	else
		dir += ALL_FILES_PATTERN;

	findFiles( dir );

	if( !filePattern.isEmpty() )
	{
		iterator	it = begin();

		while( it != end() )
		{
			const DirectoryEntry &theElement = *it;
			if( !theElement.directory )
			{
				if( !theElement.fileName.match( filePattern ) )
					it = erase( it );
				else
					++it;
			}
			else
				++it;
		}
	}
}

void DirectoryList::dirtree( DirectoryList *root, const STRING &path, const STRING &filePattern )
{
	STRING			name;
	DirectoryList	theFiles, subtree;

	theFiles.dirlist( path, filePattern );

	for( 
		const_iterator it = theFiles.cbegin(), endIT = theFiles.cend();
		it != endIT;
		++it
	)
	{
		const DirectoryEntry	&fileFound = *it;
		name = fileFound.fileName;
		if( name != "." && name != ".." )
		{
			name = path;
			if( !path.endsWith( DIRECTORY_DELIMITER ) )
				name += DIRECTORY_DELIMITER;
			name += fileFound.fileName;

			DirectoryEntry	newElement = fileFound;
			newElement.fileName = name;
			root->addElement( newElement );

			if( fileFound.directory )
			{
				try
				{
					subtree.dirtree( root, name, filePattern );
				}
				catch( ... )
				{
				}
			}
		}
	}
}

void DirectoryList::checkAccess( AccessType accessType )
{
	iterator it=begin();

	while( it != end() )
	{
		const STRING	&fName = it->fileName;
		if( strAccess( fName, int(accessType) ) )
			it = erase( it );
		else
			++it;
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#if defined( _Windows )
FStype fileName( STRING *fileName )
{
	FStype	fileType = fsNOT_EXISTING;
	STRING	nameFound;

	bool		useWide = (fileName->getCharSet() == STR_UTF8);
	if( useWide )
	{
		WIN32_FIND_DATAW	findData;

		HANDLE				fs = FindFirstFileW( uSTRING(*fileName), &findData );

		if( fs != INVALID_HANDLE_VALUE )
		{
			nameFound = uSTRING( findData.cFileName ).toString();

			if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				fileType = fsDIRECTORY;
			else
				fileType = fsFILE;
			FindClose( fs );
		}
	}
	else
	{
		WIN32_FIND_DATA		findData;
		HANDLE				fs = FindFirstFile( *fileName, &findData );

		if( fs != INVALID_HANDLE_VALUE )
		{
			nameFound = findData.cFileName;

			if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				fileType = fsDIRECTORY;
			else
				fileType = fsFILE;
			FindClose( fs );
		}
	}

	*fileName = makeFullPath( *fileName, nameFound );
	return fileType;
}
#endif

#if defined( _Windows )
FStype fileType( const wchar_t *fileName )
{
	FStype				fType = fsNOT_EXISTING;
	WIN32_FIND_DATAW	findData;

	HANDLE	fs = FindFirstFileW( fileName, &findData );

	if( fs != INVALID_HANDLE_VALUE )
	{
		if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			fType = fsDIRECTORY;
		else
			fType = fsFILE;
		FindClose( fs );
	}

	return fType;
}
#endif

FStype fileType( const STRING &fileName )
{
	doEnterFunctionEx(gakLogging::llDetail, "FStype fileType( const F_STRING &fileName )");

	FStype	fType = fsNOT_EXISTING;

#if defined( _Windows )
	bool	useWide = (fileName.getCharSet() == STR_UTF8);
	if( useWide )
	{
		fType = fileType( uSTRING(fileName) );
	}
	else
	{
		WIN32_FIND_DATA		findData;
		HANDLE				fs = FindFirstFile( fileName, &findData );

		if( fs != INVALID_HANDLE_VALUE )
		{
			if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				fType = fsDIRECTORY;
			else
				fType = fsFILE;
			FindClose( fs );
		}
	}
#elif defined( __MACH__ ) || defined( __unix__ )
	struct stat	buf;

	if( !stat( fileName, &buf ) )
	{
		if( S_ISDIR( buf.st_mode ) )
			fType = fsDIRECTORY;
		else
			fType = fsFILE;
	}
#else
#	error "Unkown OS"
#endif

	return fType;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

