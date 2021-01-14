/*
		Project:		GAKLIB
		Module:			streamRingBuffer.h
		Description:	
		Author:			Martin G�ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Germany, Munich ``AS IS''
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

#ifndef GAK_STREAM_RING_BUFFER_H
#define GAK_STREAM_RING_BUFFER_H


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/blockedRingBuffer.h>
#include <gak/memoryStream.h>

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

template <class OBJ>
class StreamRingBuffer : private BlockedRingBuffer<OBJ>
{
	public:
	typedef typename BlockedRingBuffer<OBJ>::value_type	value_type;

	private:
	bool	m_closed;
	size_t	m_numData;

	public:
	StreamRingBuffer( size_t size ) : BlockedRingBuffer<OBJ>( size ), m_numData( 0 ), m_closed( false )
	{
	}
	bool waitForSpace( void )
	{
		while( !m_closed && this->isFull() )
		{
			this->wait();
		}

		return !m_closed;
	}
	bool waitForData( void )
	{
		while( !m_closed && this->isEmpty() )
		{
			this->wait();
		}

		return !this->isEmpty();
	}

	bool push( const OBJ &newData )
	{
		if( waitForSpace() )
		{
			bool result = BlockedRingBuffer<OBJ>::push( newData );
			m_numData++;

			return result;
		}
		else
			return false;
	}

	bool pop( OBJ *newData )
	{
		if( waitForData() )
		{
			bool result = BlockedRingBuffer<OBJ>::pop( newData );

			return result;
		}
		else
			return false;
	}
	void close( void )
	{
		m_closed = true;
		this->notify();
	}

	size_t size( void ) const
	{
		return m_closed ? m_numData : m_numData+1;
	}

	void push_back( const OBJ &newData )
	{
		push( newData );
	}

	OBJ operator [] ( size_t /* index */ )
	{
		OBJ	result;
		pop( &result );
		return result;
	}
};

template <class OBJ>
class BasicIRingStream : public BasicIMemoryStream< StreamRingBuffer<OBJ> >
{
	public:
	BasicIRingStream( StreamRingBuffer<OBJ> &buffer ) : BasicIMemoryStream< StreamRingBuffer<OBJ> >( buffer )
	{
	}
};

template <class OBJ>
class BasicORingStream : public BasicOMemoryStream< StreamRingBuffer<OBJ> >
{
	public:
	BasicORingStream( StreamRingBuffer<OBJ> &buffer ) : BasicOMemoryStream< StreamRingBuffer<OBJ> >( buffer )
	{
	}
};

typedef BasicIRingStream<char>	iRingStream;
typedef BasicORingStream<char>	oRingStream;

// typedef BasicIMemoryStream< StreamRingBuffer<char> >	iRingStream;
// typedef BasicOMemoryStream< StreamRingBuffer<char> >	oRingStream;

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

#endif	// GAK_STREAM_RING_BUFFER_H
