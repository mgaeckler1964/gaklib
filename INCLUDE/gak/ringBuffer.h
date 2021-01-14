/*
		Project:		GAKLIB
		Module:			ringBuffer.h
		Description:	a queue for fast access with two threads
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

#ifndef GAK_RING_BUFFER_H
#define GAK_RING_BUFFER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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
	@brief A RingBuffer is a FIFO buffer like a Queue

	Unlike a Queue it has a fixed capacity. If there is one Thread writing 
	to this buffer and one Thread reading, there is no need lock the buffer
	since reading and writing do not change the same members.

	@tparam OBJ the item type that is stored in this RingBuffer
*/
template <class OBJ> 
class RingBuffer
{
	public:
	typedef OBJ	value_type;

	private:
	OBJ		*m_data;
	OBJ		*m_nextRead, *m_nextWrite, *m_end;
	bool	m_full;

	// no copy
	RingBuffer( const RingBuffer& src );
	const RingBuffer & operator = ( const RingBuffer& src );

	public:
	/**
		@brief creates a new RingBuffer
		@param [in] size the size of the buffer
	*/
	RingBuffer( size_t size )
	{
		m_nextRead = m_nextWrite = m_data = new OBJ[size];
		m_end = m_data+size;
		m_full = false;
	}
	~RingBuffer()
	{
		delete[] m_data;
	}
	/// returns true, if the buffer is full
	bool isFull( void ) const
	{
		return m_full;
	}
	/**
		@brief adds a new item to the buffer
		@param [in] newData the item to add
		@return true on success (not full)
	*/
	bool push( const OBJ &newData )
	{
		if( isFull() )
		{
			return false;
		}

		*m_nextWrite++ = newData;
		if( m_nextWrite >= m_end )
		{
			m_nextWrite = m_data;
		}

		if( m_nextRead == m_nextWrite )
		{
			m_full = true;
		}

		return true;
	}
	/// returns true, if the buffer is empty
	bool isEmpty( void ) const
	{
		return !m_full && m_nextRead == m_nextWrite;
	}
	/**
		@brief fetches the oldest item from the RingBuffer
		@param [out] result the next item in the Queue
		@return true on success (not empty)
		@see RingBuffer::pop
	*/
	bool pop( OBJ *result )
	{
		if( isEmpty() )
		{
			return false;
		}

		*result = *m_nextRead++;
		if( m_nextRead >= m_end )
		{
			m_nextRead = m_data;
		}

		m_full = false;
		return true;
	}
	/// removes all items in this buffer
	void clear()
	{
		m_nextRead = m_nextWrite = m_data;
		m_full = false;
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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif