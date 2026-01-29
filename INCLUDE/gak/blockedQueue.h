/*
		Project:		GAKLIB
		Module:			blockedQueue.h
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#ifndef GAK_BLOCKED_QUEUE_H
#define GAK_BLOCKED_QUEUE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/exception.h>
#include <gak/lockQueue.h>
#include <gak/conditional.h>
#include <gak/stopWatch.h>

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
	@brief a BlockedQueue is a Queue that has a blocking @ref pop member.

	this Queue is waiting for an item, if you try to @ref pop the next item
	and the Queue is empty

	@tparam OBJ the item types that is stored in this Queue
*/
template <typename OBJ>
class BlockedQueue : private LockQueue<OBJ>
{
	Conditional	m_conditional;
	Locker		m_lock;

	public:
	/**
		@brief adds a new item to the Queue
		@param [in] item the new item
	*/
	void push( const OBJ &item )
	{
		LockQueue<OBJ>::push( item );
		m_conditional.notify();
	}
	/**
		@brief fetches the oldest item from the Queue waiting if it is empty
		@param [in] timeout max. time in milliseconds to wait for an exclusive access
		@return the next item in the Queue
		@throws TimeoutException if exclusive access was not granted withing timeout milliseconds
	*/
	OBJ pop( unsigned long timeout=Locker::WAIT_FOREVER );
	/**
		@brief deletes all items in this Queue
		@param [in] timeout max. time in milliseconds to wait for an exclusive access
		@throws TimeoutException if exclusive access was not granted withing timeout milliseconds
	*/
	void clear( unsigned long timeout=Locker::WAIT_FOREVER )
	{
		LockGuard	lock( m_lock, timeout );
		if( lock )
		{
			LockQueue<OBJ>::clear();
		}
		else
		{
			throw TimeoutException();
		}
	}

	/// returns the number of elements in this Queue
	std::size_t size( void ) const
	{
		return LockQueue<OBJ>::size();
	}
	/// @return the number of elements ever pushed in this Queue
	std::size_t total( void ) const
	{
		return LockQueue<OBJ>::total();
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

template <typename OBJ>
OBJ BlockedQueue<OBJ>::pop( unsigned long timeout )
{
	StopWatch	sw(	true );

	LockGuard	lock( m_lock, timeout );
	if( lock )
	{
		while( !size() && static_cast<unsigned long>(sw.getMillis()) < timeout  )
		{
			m_conditional.wait( timeout );
		}
		if( size() )
		{
			return LockQueue<OBJ>::pop();
		}
	}

	throw TimeoutException();
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

#endif	// GAK_BLOCKED_QUEUE_H
