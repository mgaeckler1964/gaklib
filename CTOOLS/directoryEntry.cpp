/*
		Project:		GAKLIB
		Module:			directoryEntry.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/directoryEntry.h>
#include <gak/directory.h>
#include <gak/wideString.h>
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

int DirectoryEntry::compare ( const DirectoryEntry &oper, SortType theSort ) const
{
	int compareResult = 0;

	if( theSort == SORT_SIZE )
	{
		if( fileSize < oper.fileSize )
			compareResult = -1;
		else if( fileSize > oper.fileSize )
			compareResult = 1;
	}
	else if( theSort == SORT_CREATION_DATE )
	{
		if( creationDate < oper.creationDate )
			compareResult = -1;
		else if( creationDate > oper.creationDate )
			compareResult = 1;
	}
	else if( theSort == SORT_MODIFIED_DATE )
	{
		if( modifiedDate < oper.modifiedDate )
			compareResult = -1;
		else if( modifiedDate > oper.modifiedDate )
			compareResult = 1;
	}
	else if( theSort == SORT_ACCESS_DATE )
	{
		if( accessDate < oper.accessDate )
			compareResult = -1;
		else if( accessDate > oper.accessDate )
			compareResult = 1;
	}
	else if( theSort == SORT_TYPE )
	{
		if( this->directory && !oper.directory )
			compareResult = -1;
		else if( !this->directory && oper.directory )
			compareResult = 1;
	}
	if( !compareResult )
		compareResult = fileName.compare( oper.fileName );

	return compareResult;
}

void DirectoryEntry::findFile()
{
#if defined( __WINDOWS__ )
	HANDLE						hFile;

	if( fileName.getCharSet() == STR_UTF8 )
	{
		hFile = CreateFileW(
			uSTRING().decodeUTF8( fileName ),
			GENERIC_READ,
			0,
			NULL, OPEN_EXISTING, 0, NULL
		);
	}
	else
	{
		hFile = CreateFileA(
			fileName, GENERIC_READ,
			0,
			NULL, OPEN_EXISTING, 0, NULL
		);
	}

	if( hFile != INVALID_HANDLE_VALUE )
	{
		BY_HANDLE_FILE_INFORMATION	fileInfo;

		if( GetFileInformationByHandle( hFile, &fileInfo ) )
		{
			fileSize = (uint64(fileInfo.nFileSizeHigh) << uint64(32)) | uint64(fileInfo.nFileSizeLow);
			creationDate.setFileTime( fileInfo.ftCreationTime );
			modifiedDate.setFileTime( fileInfo.ftLastWriteTime );
			accessDate.setFileTime( fileInfo.ftLastAccessTime );
			directory = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			hidden = fileInfo.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM);
			readOnly = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
			needBackup = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE; 
			reparsePoint = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT; 
			fileID.deviceID = fileInfo.dwVolumeSerialNumber;
			fileID.fileIndex = (uint64(fileInfo.nFileIndexHigh) << uint64(32)) | uint64(fileInfo.nFileIndexLow);
			numLinks = fileInfo.nNumberOfLinks;
		}
		else
			throw StatReadError( fileName );

		CloseHandle(hFile);
	}
	else
	{
   /* TODO 1 -ogak : Better solution? */
		DirectoryList	tmp;

		tmp.findFiles( fileName );
		if( tmp.size() == 1 )
			*this = *tmp.cbegin();
		else
			throw OpenReadError( fileName ).addNTerror();
	}
//		throw OpenReadError( fileName ).addNTerror();
#elif defined( __unix__ ) || defined( __MACH__ )
	struct stat	buf;

	strStatE( fileName, &buf );

	fileSize = buf.st_size;
	creationDate = buf.st_ctime;
	modifiedDate = buf.st_mtime;
	accessDate = buf.st_atime;
	directory = S_ISDIR( buf.st_mode );
	hidden = fileName[0U] == '.';
	readOnly = !(buf.st_mode & S_IWUSR);
	fileID.deviceID = buf.st_dev;
	fileID.fileIndex = buf.st_ino;
	numLinks = buf.st_nlink;
#else
#	error "Unknown OS"
#endif
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

