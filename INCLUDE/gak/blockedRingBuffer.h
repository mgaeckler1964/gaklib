/*
		Project:		GAKLIB
		Module:			blockedRingBuffer.h
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

#ifndef GAK_BLOCKED_RING_BUFFER_H
#define GAK_BLOCKED_RING_BUFFER_H


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/ringBuffer.h>
#include <gak/conditional.h>

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
	A BlockedRingBuffer is a RingBuffer that is blocking when reading
	from an empty buffer or writing to a full buffer.

	@tparam OBJ the item type that is stored in this BlockedRingBuffer
*/
template <typename OBJ>
class BlockedRingBuffer : private RingBuffer<OBJ>
{
	public:
	typedef typename RingBuffer<OBJ>::value_type	value_type;

	private:
	Conditional	m_conditional;

	void waitForSpace( void )
	{
		while( this->isFull() )
		{
			m_conditional.wait();
		}
	}
	void waitForData( void )
	{
		while( this->isEmpty() )
		{
			m_conditional.wait();
		}
	}

	protected:
	/// waits for the next notification
	void wait( void )
	{
		m_conditional.wait();
	}
	/// notifies a thread that is waiting on this BlockedRingBuffer
	void notify( void )
	{
		m_conditional.notify();
	}
	/// @copydoc RingBuffer::isFull
	bool isFull( void ) const
	{
		return RingBuffer<OBJ>::isFull();
	}
	/// @copydoc RingBuffer::isEmpty
	bool isEmpty( void ) const
	{
		return RingBuffer<OBJ>::isEmpty();
	}

	public:
	/**
		@brief creates a new BlockedRingBuffer
		@param [in] size the size of the buffer
	*/
	BlockedRingBuffer( size_t size ) : RingBuffer<OBJ>( size )
	{
	}

	/**
		@brief adds a new item to the BlockedRingBuffer

		this function is waiting if the space is exhausted

		@param [in] newData the item to add
		@return true on success (not full)
		@see RingBuffer::push
	*/
	bool push( const OBJ &newData )
	{
		waitForSpace();

		bool result = RingBuffer<OBJ>::push( newData );

		if( result )
		{
			notify();
		}

		return result;
	}

	/**
		@brief fetches the oldest item from the RingBuffer 
		
		this function is waiting if the buffer is empty

		@param [out] newData the next item in the Queue
		@return true if function was successful
		@see RingBuffer::pop
	*/
	bool pop( OBJ *newData )
	{
		waitForData();

		bool result = RingBuffer<OBJ>::pop( newData );

		if( result )
		{
			notify();
		}

		return result;
	}

	/// @copydoc push
	void push_back( const OBJ &newData )
	{
		push( newData );
	}
#if 0
	OBJ operator [] ( size_t /* index */ )
	{
		OBJ	result;
		pop( &result );
		return result;
	}
#endif
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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_BLOCKED_RING_BUFFER_H
