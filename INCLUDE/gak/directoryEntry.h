/*
		Project:		GAKLIB
		Module:			directoryEntry.h
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
#ifndef GAK_DIRECTORY_ENTRY_H
#define GAK_DIRECTORY_ENTRY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if defined( _Windows )
#	ifndef STRICT
#		define STRICT	1
#	endif
#	include <windows.h>
#elif defined( __unix__ ) || defined( __MACH__ )
#	include <sys/stat.h>
#else
#	error "Unknown operating system"
#endif

#include <gak/gaklib.h>
#include <gak/datetime.h>
#include <gak/operators.h>
#include <gak/compare.h>
#include <gak/iostream.h>
#include <gak/fileID.h>

#ifdef __WINDOWS__
#include <gak/ci_string.h>
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

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __WINDOWS__
	typedef CI_STRING 	F_STRING;
#else
	typedef STRING 		F_STRING;
#endif

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct DirectoryEntry
{
	F_STRING 	fileName;

	uint64		fileSize;
	DateTime	creationDate, modifiedDate, accessDate;
	bool		directory;
#if defined( __WINDOWS__ )
	bool		needBackup;
	bool		reparsePoint;
#endif
	bool		hidden;
	bool		readOnly;
	FileID		fileID;
	size_t		numLinks;

	enum SortType
	{
		SORT_NAME, SORT_SIZE, SORT_CREATION_DATE, SORT_MODIFIED_DATE, SORT_ACCESS_DATE, SORT_TYPE
	};

	private:
	void findFile();

	public:
	DirectoryEntry()
	{
	}

	explicit DirectoryEntry( const STRING &fName ) : fileName( fName )
	{
		findFile();
	}

	explicit DirectoryEntry( 
		const STRING &fName, uint64 fileSize,
		DateTime creationDate, DateTime modifiedDate, DateTime accessDate,
		bool directory, bool hidden, bool readOnly
#if defined( __WINDOWS__ )
			, bool needBackup, bool reparsePoint
#endif
	) : fileName( fName ), creationDate(creationDate), modifiedDate(modifiedDate), accessDate(accessDate)
	{
		this->fileSize = fileSize;
		this->directory = directory;
		this->hidden = hidden;
		this->readOnly = readOnly;
#if defined( __WINDOWS__ )
		this->needBackup = needBackup;
		this->reparsePoint = reparsePoint;
#endif
		fileID.deviceID = 0;
		fileID.fileIndex = 0;
		numLinks = 0;
	}
	void findFile( const STRING &fileName )
	{
		this->fileName = fileName;
		findFile();
	}
	void setName( const STRING &fileName )
	{
		this->fileName = fileName;
	}

	int compare ( const DirectoryEntry &oper, SortType theSort=SORT_NAME ) const;
	int compare( const STRING &fileName )
	{
		return this->fileName.compare( fileName );
	}
	void toFmtStream( std::ostream &out ) const
	{
		out << '(' << fileName << ' '
			<< fileSize << ' '
			<< creationDate << ' '
			<< modifiedDate << ' '
			<< accessDate << ' '
			<< directory << ' '
			<< hidden << ' '
			<< readOnly << ' '
#if defined( __WINDOWS__ )
			<< needBackup << ' '
			<< reparsePoint << ' '
#endif
			<< fileID << ' '
			<< numLinks << ')';
	}
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, fileName );
		gak::toBinaryStream( stream, fileSize );
		gak::toBinaryStream( stream, creationDate );
		gak::toBinaryStream( stream, modifiedDate );
		gak::toBinaryStream( stream, accessDate );
		gak::toBinaryStream( stream, directory );
		gak::toBinaryStream( stream, hidden );
		gak::toBinaryStream( stream, readOnly );
#if defined( __WINDOWS__ )
		gak::toBinaryStream( stream, needBackup );
		gak::toBinaryStream( stream, reparsePoint );
#endif
		gak::toBinaryStream( stream, fileID );
		gak::toBinaryStream( stream, numLinks );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &fileName );
		gak::fromBinaryStream( stream, &fileSize );
		gak::fromBinaryStream( stream, &creationDate );
		gak::fromBinaryStream( stream, &modifiedDate );
		gak::fromBinaryStream( stream, &accessDate );
		gak::fromBinaryStream( stream, &directory );
		gak::fromBinaryStream( stream, &hidden );
		gak::fromBinaryStream( stream, &readOnly );
#if defined( __WINDOWS__ )
		gak::fromBinaryStream( stream, &needBackup );
		gak::fromBinaryStream( stream, &reparsePoint );
#endif
		gak::fromBinaryStream( stream, &fileID );
		gak::fromBinaryStream( stream, &numLinks );
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

inline std::ostream &operator << ( std::ostream &theStream, const DirectoryEntry &theSource )
{
	theSource.toFmtStream( theStream );
	return theStream;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
