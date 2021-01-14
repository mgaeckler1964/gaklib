/*
		Project:		GAKLIB
		Module:			nullStream.h
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

#ifndef GAK_NULL_STREAM_H
#define GAK_NULL_STREAM_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>

#include <gak/iostream.h>

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

template <class charT = char, class traits = std::char_traits<charT> >
class BasicNullStreamBuf : public std::basic_streambuf<charT, traits>
{
	public:
	typedef typename traits::int_type	int_type;

	private:
	// do nothing, just forbid copying
	BasicNullStreamBuf( const BasicNullStreamBuf &aStream );
	BasicNullStreamBuf &operator = ( const BasicNullStreamBuf & );

	size_t getCurrentPosition( void ) const
	{
		return 0;
	}
	protected:
	// reading
	virtual int_type underflow( void )
	{
		return traits::eof();
	}
	virtual std::streamsize showmanyc( void )
	{
		return 0;
	}
	virtual int_type pbackfail( int_type c )
	{
		return traits::eof();
	}

	// writing
	virtual int_type overflow( int_type c )
	{
		return c;
	}

	// seeking
	std::streampos seekoff( std::streamoff offset, seekdir direction, openmode mode )
	{
		return 0;
	}

	std::streampos seekpos( std::streampos pos, openmode mode )
	{
		return seekoff( pos, std::ios::beg, mode );
	}

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x551	// C++ Builder 5
	std::streampos seekoff( std::streamoff offset, int direction, openmode mode )
	{
		return seekoff( offset, seekdir(direction), mode );
	}
#endif
	public:
	BasicNullStreamBuf()
	{
	}
	~BasicNullStreamBuf()
	{
	}
	virtual int sync( void )
	{
		return 0;
	}
};

template <class charT = char, class traits = std::char_traits<charT> >
class BasicINullStream : public std::basic_istream<charT, traits>
{
	public:
	typedef BasicNullStreamBuf<charT, traits>	StreamBuf;

	private:
	StreamBuf	*theStream;

	private:
	// do nothing, just forbid copying
	BasicINullStream( const BasicINullStream & );
	BasicINullStream &operator = ( const BasicINullStream & );

	public:
	BasicINullStream() : std::basic_istream<charT, traits>( new StreamBuf() )
	{
		theStream = static_cast<StreamBuf *>( this->rdbuf() );
	}
	~BasicINullStream()
	{
		delete theStream;
		this->init( NULL );
	}
};

typedef BasicINullStream<>	iNullStream;

template <class charT = char, class traits = std::char_traits<charT> >
class BasicONullStream : public std::basic_ostream<charT, traits>
{
	public:
	typedef BasicNullStreamBuf<charT, traits>	StreamBuf;

	private:
	StreamBuf	*theStream;

	private:
	// do nothing, just forbid copying
	BasicONullStream( const BasicONullStream & );
	BasicONullStream &operator = ( const BasicONullStream & );

	public:
	BasicONullStream() : std::basic_ostream<charT, traits>( new StreamBuf() )
	{
		theStream = static_cast<StreamBuf *>( this->rdbuf() );
	}
	~BasicONullStream()
	{
		delete theStream;
		this->init( NULL );
	}
};

typedef BasicONullStream<>	oNullStream;


template <class charT = char, class traits = std::char_traits<charT> >
class BasicIONullStream : public std::basic_iostream<charT, traits>
{
	public:
	typedef BasicNullStreamBuf<charT, traits>	StreamBuf;

	private:
	StreamBuf	*theStream;

	private:
	// do nothing, just forbid copying
	BasicIONullStream( const BasicIONullStream & );
	BasicIONullStream &operator = ( const BasicIONullStream & );

	public:
	BasicIONullStream() : std::basic_iostream<charT, traits>( new StreamBuf() )
	{
		theStream = static_cast<StreamBuf *>( this->rdbuf() );
	}
	~BasicIONullStream()
	{
		delete theStream;
		this->init( NULL );
	}
};

typedef BasicIONullStream<>	ioNullStream;

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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif