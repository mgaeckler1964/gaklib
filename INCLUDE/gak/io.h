/*
		Project:		GAKLIB
		Module:			io.h
		Description:	Low Level I/O routines
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

#ifndef GAK_IO_H
#define GAK_IO_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifdef _Windows
#include <io.h>
#endif
#include <fcntl.h>

#include <gak/string.h>
#include <gak/strFiles.h>

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

enum OpenModes
{
	OM_READ, OM_WRITE, OM_READWRITE
};

enum CreateModes
{
	CM_READ, CM_WRITE, CM_READWRITE
};

enum SeekModes
{
	SeekSet = SEEK_SET,
	SeekCur = SEEK_CUR,
	SeekEnd = SEEK_END	
};

typedef long off_t;

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

/*
	Here are the basic IO routines. They sound and work like
	old TOS Gemdos routines with the same name
	TOS was the first operating system
*/
inline int Fcreate( const STRING &fName, CreateModes mode )
{
	int sysmode;
#ifdef _Windows
	if( mode == CM_READ )
		sysmode = S_IREAD;
	else if( mode == CM_WRITE )
		sysmode = S_IWRITE;
	else if( mode == CM_READWRITE )
		sysmode = S_IREAD|S_IWRITE;
#else
	sysmode = 0;
	if( mode == CM_READ )
		sysmode |= 0444;
	else if( mode == CM_WRITE )
		sysmode |= 0222;
	else if( mode == CM_READWRITE )
		sysmode |= 0666;
#endif

	return strCreat( fName, sysmode );
}

inline int Fopen( const STRING &fName, OpenModes mode )
{
	int sysMode;
	if( mode == OM_READ )
		sysMode = O_RDONLY;
	else if( mode == OM_WRITE )
		sysMode = O_WRONLY;
	else if( mode == OM_READWRITE )
		sysMode = O_RDWR;
#ifdef _Windows
	sysMode |= O_BINARY;
#endif

	return strOpen( fName, sysMode );
}

#ifdef _Windows
size_t Fread( int handle, size_t count, void *buf );
#else
inline size_t Fread( int handle, size_t count, void *buf )
{
	return read( handle, buf, count );
}
#endif

#ifdef _Windows
size_t Fwrite( int handle, size_t count, void *buf );
#else
inline size_t Fwrite( int handle, size_t count, void *buf )
{
	return write( handle, buf, count );
}
#endif


inline off_t Fseek( off_t newPos, int handle, SeekModes mode )
{
	return lseek( handle, newPos, int(mode) );
}

inline void Fclose( int handle )
{
	close( handle );
}

/******** RFILE functions ************************************************/

/*
	these functions are used to read a file line by line.
	unlike standard functions, it can handle different line endings
	'\n', '\r', '\r\n' and \n\r'
*/

const int buffSize=10240;

class  RFILE
{
	bool		eof;
	int     	handle;
	size_t		nextRead, firstClear;
	RLINE_ENDS	lineEnd;
	char    	buffer[buffSize];

	int getCharacter( void  );
	int getNextCharacter( void );

	RFILE( const RFILE &src );
	const RFILE &operator = ( const RFILE &src );

	public:
	RFILE()
	{
		eof = true;
		handle = -1;
	}
	~RFILE()
	{
		close();
	}
	void open( const STRING &fileName );
	STRING gets( void );
	bool isEOF( void ) const
	{
		return eof;
	}
	off_t getpos( void ) const
	{
		return Fseek( 0L, handle, SeekCur )
				- off_t(firstClear)
				+ off_t(nextRead);
	}
	void setpos( off_t pos )
	{
		eof = false;
		Fseek( pos, handle, SeekSet );
		nextRead = 0;
		firstClear = 0;
	}

	void close( void )
	{
		if( handle >= 0 )
		{
			Fclose( handle );
			handle = -1;
		}
	}
};

/******* File compare ***********************************************/

/*
	this is actually the only function, which uses the prev declared
	I/O routines
*/
const STRING &diff( const STRING &file1, const STRING &file2 );

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

}	// namepsace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
