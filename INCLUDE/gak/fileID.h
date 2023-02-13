/*
		Project:		GAKLIB
		Module:			fileID.h
		Description:	
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

#ifndef GAK_FILE_ID_H
#define GAK_FILE_ID_H

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
#endif

#include <gak/types.h>
#include <gak/iostream.h>
#include <gak/compare.h>

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

/**
	@brief A FileID stores a unique file identifier 

	Each file on a hard disk has it's own uinique identifier, Whith this identifier
	you can easily identify 2 or more had links that point to the save file
*/
struct FileID
{
	#if defined( _Windows )
		typedef	DWORD	DeviceID;
		typedef uint64	FileIndex;
	#elif defined( __unix__ ) || defined( __MACH__ )
		typedef	dev_t	DeviceID;
		typedef ino_t	FileIndex;
	#endif

	/// the unique id for each hard drive
	DeviceID	deviceID;
	/// the unique id for each file on a hard disk
	FileIndex	fileIndex;

	/// compare two objects of type FileID
	int compare( const FileID &oper ) const
	{
		int result = gak::compare( oper.deviceID, this->deviceID );
		if( !result )
			result = gak::compare( oper.fileIndex, this->fileIndex );

		return result;
	}
	/**
		@brief	print formated a file id object to a stream
		@param [in] out the text mode output stream
	*/
	void toFmtStream( std::ostream &out ) const
	{
		out << '(' << deviceID << '/'
			<< fileIndex << ')';
	}
	/**
		@brief	print binary a file id object to a stream
		@param [in] out the binary output stream
	*/
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, deviceID );
		gak::toBinaryStream( stream, fileIndex );
	}
	/**
		@brief	read a file id object from a stream
		@param [in] stream the binary input stream
	*/
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &deviceID );
		gak::fromBinaryStream( stream, &fileIndex );
	}
	/// returns true, if object is set (valid)
	operator bool () const
	{
		return deviceID || fileIndex;
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

/**
	@brief determin the file id
	@param [in] fname the filename
	@return the file id
*/
FileID	getFileID( const STRING &fname );

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

/// stream operator to print the file id formated to a text stream
inline std::ostream &operator << ( std::ostream &theStream, const FileID &theSource )
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
