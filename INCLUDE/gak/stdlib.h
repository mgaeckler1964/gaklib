/*
		Project:		GAKLIB
		Module:			stdlib.h
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

#ifndef GAK_STDLIB_H
#define GAK_STDLIB_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstdio>
#include <cstdlib>

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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/// simple C++ wrapper for std::FILE *
class STDfile
{
	FILE	*fp;

	STDfile( const STDfile &src );
	const STDfile & operator = ( const STDfile &src );

	public:
	/**
		@brief Open a file

		@param [in] fileName the file to open
		@param [in] mode the mode string passed to std::fopen
	*/
	STDfile( const STRING &fileName, const STRING &mode )
	{
		this->fp = strFopen( fileName, mode );
	}
	/**
		@brief Assigns an open FILE handle
		@param [in] pointer to the std::FILE handle
	*/
	STDfile( FILE *fp )
	{
		this->fp = fp;
	}
	/**
		@brief Assigns an open FILE handle
		@param [in] pointer to the std::FILE handle
		@return this
	*/
	const STDfile &operator = ( FILE *fp )
	{
		close();
		this->fp = fp;

		return *this;
	}
	/// Destrurctor, closes the file
	~STDfile()
	{
		if( fp )
			fclose( fp );
	}
	/// Returns the FILE handle
	operator FILE * ( void )
	{
		return fp;
	}
	/// Returns the FILE handle
	FILE *operator -> ( void )
	{
		return fp;
	}

	/// Closes the file
	void close( void )
	{
		if( fp )
		{
			fclose( fp );
			fp = NULL;
		}
	}
};

/**
	@brief C++ Wrapper for mallo/free
	@tparam TYPE type of the pointer to manage
*/
template <class TYPE>
class Buffer
{
	TYPE	*buff;

	// do not copy
	Buffer( const Buffer &src );
	const Buffer & operator = ( const Buffer &src );

	public:
	/**
		@brief Constructs a pointer object from an allocated memory.

		The buffer should be allocated with std::malloc or std::calloc

		@param [in] buffer the address of the memory buffer
	*/
	Buffer( void *buff )
	{
		this->buff = static_cast<TYPE*>(buff);
	}
	/// Destructor, frees the memory block
	~Buffer()
	{
		if( buff )
			::free( buff );
	}
	/// Returns true if pointer is valid
	operator bool ( void )
	{
		return buff != NULL;
	}
	/// Returns a pointer to TYPE
	operator TYPE * ( void )
	{
		return buff;
	}
	/// Returns a pointer to TYPE
	TYPE *operator -> ( void )
	{
		return buff;
	}
	/**
		@brief Returns a pointer to an index element
		@param [in] offset the index of the element
	*/
	TYPE *operator + ( int offset )
	{
		return buff + offset;
	}
	/**
		@brief Returns a const pointer to an index element
		@param [in] offset the index of the element
	*/
	const TYPE *operator + ( int offset ) const
	{
		return buff + offset;
	}
	/// Frees the memory block
	void free( void )
	{
		if( buff )
		{
			::free( buff );
			buff = NULL;
		}
	}
	/// resizes the buffer
	void resize(size_t newSize)
	{
		buff = static_cast<TYPE*>(realloc(buff, newSize));
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

} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif //  GAK_STDLIB_H