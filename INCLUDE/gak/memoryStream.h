/*
		Project:		GAKLIB
		Module:			memoryStream.h
		Description:	stream buffer to read/write from/to a vector compatible
						container
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

#ifndef GAK_MEMORY_STREAM_H
#define GAK_MEMORY_STREAM_H

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

const size_t MemBufferSize	= 16;
const size_t MemPutbackSize	= 8;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <class ContainerT, class charT = typename ContainerT::value_type, class traits = std::char_traits<charT> >
class BasicMemoryStreamBuf : public std::basic_streambuf<charT, traits>
{
	public:
	typedef typename traits::int_type	int_type;

	private:
	charT		buffer[MemBufferSize];
	ContainerT	&theDataBuffer;

	size_t	readPosition;			// the read position in ContainerT for the next underflow
	size_t	bufferStartPosition;	// the read position in ContainerT of the last underflow

	private:
	// do nothing, just forbid copying
	BasicMemoryStreamBuf( const BasicMemoryStreamBuf &aStream );
	BasicMemoryStreamBuf &operator = ( const BasicMemoryStreamBuf & );

	size_t getCurrentPosition( void ) const
	{
		return bufferStartPosition + (this->gptr()-buffer) - MemPutbackSize;
	}
	protected:
	// reading
	virtual int_type underflow( void );
	virtual std::streamsize showmanyc( void )
	{
		return theDataBuffer.size() - getCurrentPosition();
	}
	virtual int_type pbackfail( int_type c )
	{
		charT	*ptr = this->gptr();
		if( ptr > buffer )
		{
			--ptr;
			*ptr = charT(c);
			setg( ptr, ptr, this->egptr() );

			return c;
		}

		return traits::eof();
	}

	// writing
	virtual int_type overflow( int_type c )
	{
		sync();

		if( c != traits::eof() )
		{
			sputc( charT(c) );
			this->gbump(1);       // pptr and gptr must be the same
		}

		return c;
	}

	// seeking
	std::streampos seekoff( std::streamoff offset, seekdir direction, openmode mode )
	{
		if( mode & std::ios::in )
		{
			if( direction == std::ios::cur )
				readPosition = getCurrentPosition() + size_t(offset);
			else if( direction == std::ios::beg )
				readPosition = size_t(offset);
			else
				readPosition = theDataBuffer.size() + size_t(offset);
		}

		sync();

		return readPosition;
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
	BasicMemoryStreamBuf( ContainerT &aString ) : theDataBuffer( aString )
	{
		bufferStartPosition = readPosition = 0;
		this->setp( buffer+MemPutbackSize, buffer+MemBufferSize );
		this->setg( buffer+MemPutbackSize, buffer+MemBufferSize, buffer+MemBufferSize );
	}
	~BasicMemoryStreamBuf()
	{
	}
	virtual int sync( void );

	ContainerT &getContainer( void )
	{
		return theDataBuffer;
	}
};

template <class ContainerT, class charT = typename ContainerT::value_type, class traits = std::char_traits<charT> >
class BasicIMemoryStream : public std::basic_istream<charT, traits>
{
	public:
	typedef BasicMemoryStreamBuf<ContainerT, charT, traits>	StreamBuf;

	private:
	StreamBuf	*theStream;

	private:
	// do nothing, just forbid copying
	BasicIMemoryStream( const BasicIMemoryStream & );
	BasicIMemoryStream &operator = ( const BasicIMemoryStream & );

	public:
	BasicIMemoryStream( const ContainerT &aString ) : std::basic_istream<charT, traits>( new StreamBuf( const_cast<ContainerT&>(aString) ) )
	{
		theStream = static_cast<StreamBuf *>( this->rdbuf() );
	}
	~BasicIMemoryStream()
	{
		theStream->sync();
		delete theStream;
		this->init( NULL );
	}
	ContainerT &getContainer( void )
	{
		return theStream->getContainer();
	}
	void close( void )
	{
		getContainer().close();
	}
};

template <class ContainerT, class charT = typename ContainerT::value_type, class traits = std::char_traits<charT> >
class BasicOMemoryStream : public std::basic_ostream<charT, traits>
{
	public:
	typedef BasicMemoryStreamBuf<ContainerT, charT, traits>	StreamBuf;

	private:
	StreamBuf	*theStream;

	private:
	// do nothing, just forbid copying
	BasicOMemoryStream( const BasicOMemoryStream & );
	BasicOMemoryStream &operator = ( const BasicOMemoryStream & );

	public:
	BasicOMemoryStream( ContainerT &aString ) : std::basic_ostream<charT, traits>( new StreamBuf( aString ) )
	{
		theStream = static_cast<StreamBuf *>( this->rdbuf() );
	}
	~BasicOMemoryStream()
	{
		theStream->sync();
		delete theStream;
		this->init( NULL );
	}
	ContainerT &getContainer( void )
	{
		return theStream->getContainer();
	}
	void close( void )
	{
		this->flush();
		getContainer().close();
	}
};

template <class ContainerT, class charT = typename ContainerT::value_type, class traits = std::char_traits<charT> >
class BasicIOMemoryStream : public std::basic_iostream<charT, traits>
{
	public:
	typedef BasicMemoryStreamBuf<ContainerT, charT, traits>	StreamBuf;

	private:
	StreamBuf	*theStream;

	private:
	// do nothing, just forbid copying
	BasicIOMemoryStream( const BasicIOMemoryStream & );
	BasicIOMemoryStream &operator = ( const BasicIOMemoryStream & );

	public:
	BasicIOMemoryStream( ContainerT &aString ) : std::basic_iostream<charT, traits>( new StreamBuf( aString ) )
	{
		theStream = static_cast<StreamBuf *>( this->rdbuf() );
	}
	~BasicIOMemoryStream()
	{
		theStream->sync();
		delete theStream;
		this->init( NULL );
	}
	ContainerT &getContainer( void )
	{
		return theStream->getContainer();
	}
	void close( void )
	{
		this->flush();
		getContainer().close();
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
   
template <class ContainerT, class charT, class traits>
typename BasicMemoryStreamBuf<ContainerT, charT, traits>::int_type BasicMemoryStreamBuf<ContainerT, charT, traits>::underflow( void )
{
	charT *ptr;
	if( readPosition >= theDataBuffer.size() )
/*@*/	return traits::eof();

	bufferStartPosition = readPosition;
	for( ptr=buffer+MemPutbackSize;
		readPosition<theDataBuffer.size() && ptr < buffer+MemBufferSize;
		readPosition++
	)
	{
		*ptr = theDataBuffer[readPosition];
		if( readPosition<theDataBuffer.size() )
			ptr++;
		else
			break;
	}

	if( ptr == buffer+MemPutbackSize )
/*@*/	return traits::eof();

	setg( buffer+MemPutbackSize, buffer+MemPutbackSize, ptr );

	return traits::to_int_type( buffer[MemPutbackSize] );
}

template <class ContainerT, class charT, class traits>
int BasicMemoryStreamBuf<ContainerT, charT, traits>::sync( void )
{
	ptrdiff_t		count;

	count = this->pptr() - this->pbase();
	if( count > 0 )
	{
		int		i;
		charT	*ptr;
		for( i=0, ptr = this->pbase(); i<count; i++, ptr++ )
			theDataBuffer.push_back( *ptr );
	}

	setp( buffer+MemPutbackSize, buffer+MemBufferSize );

	// set input pointers so that next read causes an underflow
	setg( buffer+MemPutbackSize, buffer+MemBufferSize, buffer+MemBufferSize );

	return 0;
}

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