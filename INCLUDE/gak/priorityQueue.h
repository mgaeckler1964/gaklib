/*
		Project:		GAKLIB
		Module:			priorityQueue.h
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

#ifndef GAK_PRIORITY_QUEUE_H
#define GAK_PRIORITY_QUEUE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/compare.h>
#include <gak/list.h>
#include <gak/queue.h>
#include <gak/map.h>
#include <gak/shared.h>

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

template<typename OBJ, typename PriorityT>
struct PriorityExtractor
{
	typedef PriorityT	priority_type;

	static priority_type getPriority( const OBJ &item )
	{
		return item.getPriority();
	}
};

template<typename OBJ, typename PriorityT>
struct PointerPriorityExtractor
{
	typedef PriorityT	priority_type;

	static priority_type getPriority( const OBJ *item )
	{
		return item->getPriority();
	}
};

template<typename OBJ, typename PriorityT = typename OBJ::priority_type, typename PriorityExtractorT = PriorityExtractor<OBJ, PriorityT> >
class PriorityQueue : private List<OBJ>
{
	typedef PriorityT						priority_type;
	typedef typename List<OBJ>::ListItem	ListItem;

	public:
	/**
		@brief adds a new item to the Queue
		@param [in] item the new item
	*/
	void push( const OBJ &item );

	/**
		@brief fetches the oldest item from the Queue

		if the Queue is empty a QueueEmptyError exception is thrown

		@return the next item in the Queue
	*/
	OBJ pop( void );
	/// deletes all items in this Queue
	void clear( void )
	{
		List<OBJ>::clear();
	}
	/// @return the number of elements in this Queue
	size_t size( void ) const
	{
		return List<OBJ>::size();
	}
};

template<typename OBJ, typename PriorityT = typename OBJ::priority_type, typename PriorityExtractorT = PriorityExtractor<OBJ, PriorityT> >
class PriorityQueue2 : public Container
{
	typedef SharedPointer< Queue<OBJ> >			QueuePtr;
	typedef PriorityT							priority_type;
	typedef TreeMap<priority_type, QueuePtr>	Queues;

	Queues										m_queue;

	public:
	/**
		@brief adds a new item to the Queue
		@param [in] item the new item
	*/
	void push( const OBJ &item )
	{
		QueuePtr	&theQueue = m_queue[PriorityExtractorT::getPriority( item )];
		if( !theQueue )
		{
			theQueue = QueuePtr::makeShared();
		}
		theQueue->push( item );
		incNumElements();
	}

	/**
		@brief fetches the oldest item from the Queue

		if the Queue is empty a QueueEmptyError exception is thrown

		@return the next item in the Queue
	*/
	OBJ pop( void );
	/// deletes all items in this Queue
	void clear( void )
	{
		m_queue.clear();
		clear();
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

template<typename OBJ, typename PriorityT, typename PriorityExtractorT>
void PriorityQueue<OBJ, PriorityT, PriorityExtractorT>::push( const OBJ &item )
{
	typename List<OBJ>::iterator	it, endIT;
	for(
		it = this->begin(), endIT = this->end();
		it != endIT;
		++it
	)
	{
		if( 
			gak::compare( 
				PriorityExtractorT::getPriority( item ), 
				PriorityExtractorT::getPriority( *it ) 
			) > 0 
		)
		{
			break;
		}
	}
	this->insertObject( it.getBaseCursor(), new ListItem( item ), false );
}

template<typename OBJ, typename PriorityT, typename PriorityExtractorT>
OBJ PriorityQueue<OBJ, PriorityT, PriorityExtractorT>::pop( void )
{
	ListItem *first = static_cast<ListItem *>(
		this->getFirstItem()
	);
	if( first )
	{
		OBJ item = static_cast<OBJ>(*first);
		this->removeObject( first );

		delete first;
		return item;
	}

	throw QueueEmptyError();
}

template<typename OBJ, typename PriorityT, typename PriorityExtractorT>
OBJ PriorityQueue2<OBJ, PriorityT, PriorityExtractorT>::pop( void )
{
	if( size() > 0 )
	{
		typename Queues::value_type	&priorityQueue = *m_queue.rbegin();
		QueuePtr			&theQueue = priorityQueue.getValue();
		OBJ					item = theQueue->pop();
		if( theQueue->size() == 0 )
		{
			m_queue.removeElementByKey( priorityQueue.getKey() );
		}
		this->decNumElements();
		return item;
	}

	throw QueueEmptyError();
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

#endif	// GAK_PRIORITY_QUEUE_H