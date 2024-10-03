/*
		Project:		GAKLIB
		Module:			List.H
		Description:	Doubly linked list class
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

#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdlib.h>
#include <cassert>

#include <gak/gaklib.h>
#include <gak/container.h>
#include <gak/compare.h>
#include <gak/exception.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/// @cond
namespace internal
{

template<typename PointerT>
class BaseListCursor
{
	protected:
	size_t		m_entryNum;
	PointerT	m_current;

	// we need to cache next and prev, in case of removment of current
	PointerT	m_next, m_prev;

	public:
	BaseListCursor()
	{
		invalidate();
	}
	void invalidate( void )
	{
		m_entryNum = Container::no_index;
		m_current = NULL;
	}
	bool isValid( void ) const
	{
		return m_entryNum != Container::no_index && m_current;
	}
	int compare( const BaseListCursor<PointerT> &other ) const
	{
		if( m_current && other.m_current )
		{
			return gak::compare( m_entryNum, other.m_entryNum );
		}
		else if( m_current )
		{
			return 1;
		}
		else if( other.m_current )
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	PointerT setPosition( PointerT newObj, size_t newEntryNum );

	size_t getPos( void ) const
	{
		return m_entryNum;
	}

	PointerT getCurrent( void ) const
	{
		return m_current;
	}
};

template<typename PointerT>
class IncrListCursor : public BaseListCursor<PointerT>
{
	public:
	PointerT getNext( void )
	{
		return this->setPosition( this->m_next, this->m_entryNum+1 );
	}
	PointerT getPrev( void )
	{
		return this->setPosition( this->m_prev, this->m_entryNum-1 );
	}
	const IncrListCursor<PointerT> &operator ++()				// pre inkrement
	{
		this->getNext();
		return *this;
	}
	const IncrListCursor<PointerT> operator ++( int )			// post inkrement
	{
		IncrListCursor<PointerT>	temp = *this;
		this->getNext();
		return temp;
	}
	const IncrListCursor<PointerT> &operator --()				// pre dekrement
	{
		this->getPrev();
		return *this;
	}
	IncrListCursor<PointerT> operator --( int )					// post dekrement
	{
		IncrListCursor<PointerT>	temp = *this;
		this->getPrev();
		return temp;
	}
};

template<typename PointerT>
class DecrListCursor : public BaseListCursor<PointerT>
{
	public:
	PointerT getNext( void )
	{
		return this->setPosition( this->m_prev, this->m_entryNum-1 );
	}
	PointerT getPrev( void )
	{
		return this->setPosition( this->m_next, this->m_entryNum+1 );
	}
	const DecrListCursor<PointerT> &operator ++()				// pre inkrement
	{
		this->getNext();
		return *this;
	}
	const DecrListCursor<PointerT> operator ++( int )			// post inkrement
	{
		DecrListCursor<PointerT>	temp = *this;
		this->getNext();
		return temp;
	}
	const DecrListCursor<PointerT> &operator --()				// pre dekrement
	{
		this->getPrev();
		return *this;
	}
	DecrListCursor<PointerT> operator --( int )					// post dekrement
	{
		DecrListCursor<PointerT>	temp = *this;
		this->getPrev();
		return temp;
	}
};

}	// namespace internal
/// @endcond

/// @brief Base class for all elements of a ListContainer
class ListEntry
{
	friend class ListContainer;

	private:
	ListEntry	*m_next,
				*m_prev;

	void appendObject( ListEntry *newObj );
	void insertObject( ListEntry *newObj );
	void remove( void );
	void putFirstObject( ListEntry * );
	void putLastObject( ListEntry *);

	protected:
	ListEntry()
	{
		m_next =
		m_prev = NULL;
	};
	virtual ~ListEntry();
	/**
		@brief allway returns true indicating that all items are visible

		overwrite this member if you need the filter capability of the ListContainer

		@see ListContainer::getFilteredEntry, ListContainer::getNumVisibleEntries
	*/
	virtual bool isVisible( void ) const;

	public:
	/// @brief returns the next entry in the ListContainer
	ListEntry *getNext( void ) const
	{
		return m_next;
	}
	/// @brief returns the previous entry in the ListContainer
	ListEntry *getPrev( void ) const
	{
		return m_prev;
	}
};

/// @brief double linked list containing elements of type ListEntry 
class ListContainer : public Container
{
	public:
	typedef internal::IncrListCursor<ListEntry*>		ListCursor;
	typedef internal::IncrListCursor<const ListEntry*>	ConstListCursor;
	typedef internal::DecrListCursor<ListEntry*>		ListReverseCursor;
	typedef internal::DecrListCursor<const ListEntry*>	ConstListReverseCursor;

	private:
	bool			m_shutdown;
	ListEntry		*m_first,
					*m_last;

	protected:
	/// @brief returns the first item of the list
	ListEntry *getFirstItem( void ) const
	{
		return m_first;
	}
	/// @brief returns the last item of the list
	ListEntry *getLastItem( void ) const
	{
		return m_last;
	}
	/// @brief sets a new first item overwriting the old one
	void setFirst( ListEntry *newFirst )
	{
		m_first = newFirst;
	}
	/// @brief sets a new last item overwriting the old one
	void setLast( ListEntry *newLast )
	{
		m_last = newLast;
	}

	void forget()
	{
		Container::clear();
		m_first =
		m_last  = NULL;

		m_shutdown = false;
	}

	private:
	// forbid copying
	ListContainer( const ListContainer& );
	ListContainer & operator = ( const ListContainer& );

	public:
	ListContainer()
	{
		m_first		=
		m_last		= NULL;
		m_shutdown	= false;
	};
	~ListContainer()
	{
		m_shutdown = true;
		clear();
	}

	void moveFrom( ListContainer &source )
	{
		if( &source != this )
		{
			clear();
			this->m_first = source.m_first;
			this->m_last = source.m_last;
			Container::moveFrom( source );
			source.forget();
		}
	}

	/**
		@brief returns the entry at a given position
		@param [in] num the index of the element to search
		@param [in,out] buff the addres of a ListCursor used to search for the item
		@return the element NULL if not found
	*/
	ListEntry *getEntry( std::size_t num, ListCursor *buff ) const;
	/**
		@brief returns the visible entry at a given position

		the position counts only those entries, whose member function isVisible returns true

		@param [in] num the index of the element to search
		@param [in,out] buff the addres of a ListCursor used to search for the item
		@return the element NULL if not found
		@see ListEntry::isVisible
	*/
	ListEntry *getFilteredEntry( std::size_t num, ListCursor *buff );

	/**
		@brief inserts a new item to the list
		if the item is an element of a list the behaviour is undefined
		@param [in] point a ListCursor pointing to the item where to insert the new item
		@param [in] newObj the address of the new item
		@param [in] after if true the new item is inserted after the given point
		@tparam CursorT one of the ListContainer's cursor types
	*/
	template <typename CursorT>
	void insertObject  ( CursorT *point, ListEntry *newObj, bool after = true );

	/**
		@brief removes an item from the list
		if the item is not an element of this list the behaviour is undefined
		@param [in] obj the address of the item to remove
	*/
	void removeObject  ( ListEntry *obj );

	/**
		@brief inserts a new item as first element in the list
		if the item is an element of a list the behaviour is undefined
		@param [in] obj the address of the new item
	*/
	void putFirstObject( ListEntry *obj );
	/**
		@brief inserts a new item as last element in the list
		if the item is an element of a list the behaviour is undefined
		@param [in] obj the address of the new item
	*/
	void putLastObject ( ListEntry *obj );

	/**
		@brief returns of visible items in this list
		@see ListEntry::isVisible
	*/
	long getNumVisibleEntries( void ) const;

	/*
		these members are implemented in all container types
		List, ARRAY some of them in ListContainer
	*/
	/// @brief deletes all items in this list
	void clear( void );

	/**
		@brief inserts a new item as last element in the list
		if the item is an element of a list the behaviour is undefined
		@param [in] obj the address of the new item
	*/
	void addElement  ( ListEntry *obj );

	/**
		@brief returns the entry at a given position
		@param [in] pos the index of the element to search
		@return the element NULL if not found
	*/
	ListEntry *getElementAt( size_t pos )
	{
		ListCursor	tmp;

		return getEntry( pos, &tmp );
	}
	/**
		@brief returns the entry at a given position
		@param [in] pos the index of the element to search
		@return the element NULL if not found
	*/
	const ListEntry *getElementAt( size_t pos ) const
	{
		ListCursor	tmp;

		return getEntry( pos, &tmp );
	}

	/**
		@brief returns the first entry 
		@param [out] buff a new ListCursor
		@return the element NULL if not found
	*/
	ListEntry *getFirst( ListCursor *buff )
	{
		buff->setPosition( m_first, 0 );
		return m_first;
	}
	/**
		@brief returns the last entry 
		@param [out] buff a new ListCursor
		@return the element NULL if not found
	*/
	ListEntry *getLast( ListCursor *buff )
	{
		buff->setPosition( m_last, size()-1 );

		return m_last;
	}

	/**
		@brief returns the first entry 
		@param [out] buff a new ConstListCursor
		@return the element NULL if not found
	*/
	const ListEntry *getFirst( ConstListCursor *buff ) const
	{
		buff->setPosition( m_first, 0 );
		return m_first;
	}
	/**
		@brief returns the last entry 
		@param [out] buff a new ConstListCursor
		@return the element NULL if not found
	*/
	const ListEntry *getLast( ConstListCursor *buff ) const
	{
		buff->setPosition( m_last, size()-1 );

		return m_last;
	}

	/**
		@brief returns the first entry 
		@param [out] buff a new ListReverseCursor
		@return the element NULL if not found
	*/
	ListEntry *getFirst( ListReverseCursor *buff )
	{
		buff->setPosition( m_last, size()-1 );
		return m_last;
	}
	/**
		@brief returns the last entry 
		@param [out] buff a new ListReverseCursor
		@return the element NULL if not found
	*/
	ListEntry *getLast( ListReverseCursor *buff )
	{
		buff->setPosition( m_first, 0 );

		return m_first;
	}

	/**
		@brief returns the first entry 
		@param [out] buff a new ConstListReverseCursor
		@return the element NULL if not found
	*/
	const ListEntry *getFirst( ConstListReverseCursor *buff ) const
	{
		buff->setPosition( m_last, size()-1 );
		return m_last;
	}

	/**
		@brief returns the last entry 
		@param [out] buff a new ConstListReverseCursor
		@return the element NULL if not found
	*/
	const ListEntry *getLast( ConstListReverseCursor *buff ) const
	{
		buff->setPosition( m_first, 0 );

		return m_first;
	}


	/**
		@brief returns the entry at a given position
		@param [in] pos the index of the element to search
		@return the element NULL if not found
	*/
	ListEntry *operator[] ( size_t pos )
	{
		return getElementAt( pos );
	}
	/**
		@brief returns the entry at a given position
		@param [in] pos the index of the element to search
		@return the element NULL if not found
	*/
	const ListEntry *operator[] ( size_t pos ) const
	{
		return getElementAt( pos );
	}
};

typedef ListContainer::ListCursor				ListCursor;
typedef ListContainer::ConstListCursor			ConstListCursor;
typedef ListContainer::ListReverseCursor		ListReverseCursor;
typedef ListContainer::ConstListReverseCursor	ConstListReverseCursor;

/**
	@brief double linked list of elements of one type

	unlike ListContainer you can add only elements of one type

	@tparam OBJ the type of the elements
*/
template <class OBJ> 
class List : public ListContainer
{
	typedef ListContainer	Super;

	protected:
	/// @cond
	class ListItem : public ListEntry
	{
		OBJ	object;

		public:
		ListItem()  {};
		ListItem( const OBJ &item ) : object( item ) {};

		OBJ &getItem( void )
		{
			return object;
		}
		const OBJ &getItem( void ) const 
		{
			return object;
		}

		operator OBJ & ()
		{
			return object;
		}
		operator const OBJ & () const
		{
			return object;
		}
	};
	/// @endcond

	/*
		these members are implemented in all container types
		List, ARRAY some of them in ListContainer
	*/
	private:
	template <typename RefT, typename PointerT, typename WrapperT, typename BaseCursorT>
	class BaseCursor
	{
		public:
		//typedef	BaseCursor<RefT, PointerT, WrapperT, BaseCursorT>	SelfT;

		private:
		BaseCursorT		m_cursor;

		public:
		bool isValid() const
		{
			return m_cursor.isValid();
		}
		const BaseCursor &operator ++()				// pre inkrement
		{
			++m_cursor;
			return *this;
		}
		const BaseCursor operator ++( int )			// post inkrement
		{
			BaseCursor	temp = *this;
			++m_cursor;
			return temp;
		}
		const BaseCursor &operator --()				// pre dekrement
		{
			--m_cursor;
			return *this;
		}
		BaseCursor operator --( int )					// post dekrement
		{
			BaseCursor	temp = *this;
			--m_cursor;
			return temp;
		}
		int compare(const BaseCursor &other) const
		{
			return m_cursor.compare( other.m_cursor );
		}
		RefT getCurrent( void ) const
		{
			return static_cast<WrapperT>(m_cursor.getCurrent())->getItem();
		}
		RefT getNext( void )
		{
			return static_cast<WrapperT>(m_cursor.getNext())->getItem();
		}
		RefT getPrev( void )
		{
			return static_cast<WrapperT>(m_cursor.getPrev())->getItem();
		}

		RefT operator * ( void ) const
		{
			return getCurrent();
		}
		PointerT operator -> ( void ) const
		{
			return &getCurrent();
		}

		BaseCursorT *getBaseCursor()
		{
			return &m_cursor;
		}
		const BaseCursorT *getBaseCursor() const
		{
			return &m_cursor;
		}
	};

	private:
	// forbid copying
	List( const List& );
	List & operator = ( const List& );

	public:
	List()
	{
	}

	void moveFrom( List<OBJ> &source )
	{
		Super::moveFrom( source );
	}

	/*
	-------------------------------------------------------------------------
		Creating new data
	-------------------------------------------------------------------------
	*/
	/// @name Creating new data
	///@{
	public:
	/// @brief creates and returns the reference of new item
	OBJ &createElement( void )
	{
		ListItem	*lItem = new ListItem;
		ListContainer::addElement( lItem );

		return lItem->getItem();
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Adding existing data
	-------------------------------------------------------------------------
	*/
	/// @name Adding existing data
	///@{
	public:
	/**
		@brief copies the passed element to the end of the list
		@param [in] item the new item
		@return the addres of the new item
	*/
	OBJ *addElement( const OBJ &item )
	{
		ListItem	*lItem = new ListItem(item);
		ListContainer::addElement( lItem );
		return &lItem->getItem();
	}
	/**
		@brief copies the passed element to the end of the list
		@param [in] item the new item
		@return the reference of the list itself
	*/
	const List<OBJ> &operator += ( const OBJ &item )
	{
		addElement( item );

		return *this;
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Get Data
	-------------------------------------------------------------------------
	*/
	/// @name Get Data
	///@{
	public:
	/// @brief a forward cursor used to retriev a modifiable element
	typedef BaseCursor<OBJ &, OBJ *, ListItem *, ListCursor>								Cursor;
	/// @brief a forward cursor used to retriev a const element
	typedef BaseCursor<const OBJ &, const OBJ *, const ListItem *, ConstListCursor>			ConstCursor;
	/// @brief a reverse cursor used to retriev a modifiable element
	typedef BaseCursor<OBJ &, OBJ *, ListItem *, ListReverseCursor>							ReverseCursor;
	/// @brief a reverse cursor used to retriev a const element
	typedef BaseCursor<const OBJ &, const OBJ *, const ListItem *, ConstListReverseCursor>	ConstReverseCursor;

	/**
		@brief retrieves the element at a given position
		@param [in] pos the index position of the element
		@return a reference of the element
		@throws IndexError if index is out of bounds
	*/
	OBJ &getElementAt( size_t pos )
	{
		ListItem	*lItem = static_cast<ListItem *>(
			ListContainer::getElementAt( pos )
		);

		if( !lItem )
		{
			throw IndexError();
		}

		return lItem->getItem();
	}
	/**
		@brief retrieves the element at a given position
		@param [in] pos the index position of the element
		@return a reference of the element
		@throws IndexError if index is out of bounds
	*/
	const OBJ &getElementAt( size_t pos ) const
	{
		ListItem	*lItem = static_cast<ListItem *>(
			ListContainer::getElementAt( pos )
		);

		if( !lItem )
		{
			throw IndexError();
		}

		return lItem->getItem();
	}
	/**
		@brief retrieves the element at a given position
		@param [in] pos the index position of the element
		@return a reference of the element
		@throws IndexError if index is out of bounds
	*/
	OBJ &operator[ ] (size_t pos)
	{
		return getElementAt( pos );
	}
	/**
		@brief retrieves the element at a given position
		@param [in] pos the index position of the element
		@return a reference of the element
		@throws IndexError if index is out of bounds
	*/
	const OBJ &operator[ ] (size_t pos) const 
	{
		return getElementAt( pos );
	}
	/**
		@brief retrieves the element at a given position
		@param [in] cursor the Cursor pointing to the element
		@return a reference of the element
	*/
	OBJ &operator [] ( const Cursor &cursor )
	{
		return cursor.getCurrent();
	}
	/**
		@brief retrieves the element at a given position
		@param [in] cursor the Cursor pointing to the element
		@return a reference of the element
	*/
	const OBJ &operator [] ( const Cursor &cursor ) const
	{
		return cursor.getCurrent();
	}

	/// @brief returns a Cursor pointing to the first element
	Cursor getFirstCursor( void )
	{
		Cursor newCursor;
		ListContainer::getFirst( newCursor.getBaseCursor() );
		return newCursor;
	}
	/// @brief returns a Cursor pointing to the last element
	Cursor getLastCursor( void )
	{
		Cursor newCursor;
		ListContainer::getLast( newCursor.getBaseCursor() );
		return newCursor;
	}

	/// @brief returns a ConstCursor pointing to the first element
	ConstCursor getFirstCursor( void ) const
	{
		ConstCursor newCursor;
		ListContainer::getFirst( newCursor.getBaseCursor() );
		return newCursor;
	}
	/// @brief returns a ConstCursor pointing to the last element
	ConstCursor getLastCursor( void ) const
	{
		ConstCursor newCursor;
		ListContainer::getLast( newCursor.getBaseCursor() );
		return newCursor;
	}

	/// @brief returns a ReverseCursor pointing to the first element
	ReverseCursor getFirstReverseCursor( void )
	{
		ReverseCursor newCursor;
		ListContainer::getFirst( newCursor.getBaseCursor() );
		return newCursor;
	}
	/// @brief returns a ReverseCursor pointing to the last element
	ReverseCursor getLastReverseCursor( void )
	{
		ReverseCursor newCursor;
		ListContainer::getLast( newCursor.getBaseCursor() );
		return newCursor;
	}

	/// @brief returns a ConstReverseCursor pointing to the first element
	ConstReverseCursor getFirstReverseCursor( void ) const
	{
		ConstReverseCursor newCursor;
		ListContainer::getFirst( newCursor.getBaseCursor() );
		return newCursor;
	}
	/// @brief returns a ConstReverseCursor pointing to the last element
	ConstReverseCursor getLastReverseCursor( void ) const
	{
		ConstReverseCursor newCursor;
		ListContainer::getLast( newCursor.getBaseCursor() );
		return newCursor;
	}

	/**
		@brief retrievs the first element and the coresponding Cursor
		@param [out] newCursor the Cursor pointing to the first element
		@return the reference to the first element
		@throws IndexError if the is no element
	*/
	OBJ &getFirst( Cursor *newCursor )
	{
		ListItem *lItem = static_cast<ListItem *>(
			ListContainer::getFirst( newCursor->getBaseCursor() )
		);

		if( !lItem )
		{
			throw IndexError();
		}

		return lItem->getItem();
	}
	/**
		@brief retrievs the last element and the coresponding Cursor
		@param [out] newCursor the Cursor pointing to the last element
		@return the reference to the last element
		@throws IndexError if the is no element
	*/
	OBJ &getLast( Cursor *newCursor )
	{
		ListItem *lItem = static_cast<ListItem *>(
			ListContainer::getLast( newCursor->getBaseCursor() )
		);

		if( !lItem )
		{
			throw IndexError();
		}

		return lItem->getItem();
	}

	/**
		@brief retrievs the first element and the coresponding Cursor
		@param [out] newCursor the ConstCursor pointing to the first element
		@return the reference to the first element
		@throws IndexError if the is no element
	*/
	const OBJ &getFirst( ConstCursor *newCursor ) const
	{
		ListItem *lItem = static_cast<ListItem *>(
			ListContainer::getFirst( newCursor->getBaseCursor() )
		);

		if( !lItem )
		{
			throw IndexError();
		}

		return lItem->getItem();
	}
	/**
		@brief retrievs the last element and the coresponding Cursor
		@param [out] newCursor the ConstCursor pointing to the last element
		@return the reference to the last element
		@throws IndexError if the is no element
	*/
	const OBJ &getLast( ConstCursor *newCursor ) const
	{
		ListItem *lItem = static_cast<ListItem *>(
			ListContainer::getLast( newCursor->getBaseCursor() )
		);

		if( !lItem )
		{
			throw IndexError();
		}

		return lItem->getItem();
	}
	///@}
	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	public:
	/// @name Some standard typedefs:
	///@{
	typedef OBJ					value_type;
	typedef OBJ					&reference;
	typedef const OBJ			&const_reference;
	typedef OBJ					*pointer;
	typedef const OBJ			*const_pointer;

	typedef Cursor				iterator;
	typedef ConstCursor			const_iterator;
	typedef ReverseCursor		reverse_iterator;
	typedef ConstReverseCursor	const_reverse_iterator;
	///@}

	/// @name Some standard member functions:
	///@{
	iterator begin( void )
	{
		return getFirstCursor();
	}
	iterator end( void )
	{
		return iterator();
	}
	const_iterator cbegin( void ) const
	{
		return getFirstCursor();
	}
	const_iterator cend( void ) const
	{
		return const_iterator();
	}
	reverse_iterator rbegin( void )
	{
		return getFirstReverseCursor();
	}
	reverse_iterator rend( void )
	{
		return reverse_iterator();
	}
	const_reverse_iterator crbegin( void ) const
	{
		return getFirstReverseCursor();
	}
	const_reverse_iterator crend( void ) const
	{
		return const_reverse_iterator();
	}

	iterator erase( iterator it )
	{
		ListEntry *entry = it.getBaseCursor()->getCurrent();
		++it;
		this->removeObject( entry );

		return it;
	}
	void push_back( const OBJ &item )
	{
		addElement( item );
	}
	void push_front( const OBJ &item )
	{
		insertObject( begin().getBaseCursor(), new ListItem( item ), false );
	}
	///@}
};

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

/// @cond
namespace internal
{

template <typename PointerT>
inline PointerT BaseListCursor<PointerT>::setPosition( PointerT newObj, size_t newEntryNum )
{
	m_current = newObj;
	if( newObj )
	{
		m_next = newObj->getNext();
		m_prev = newObj->getPrev();
	}
	else
	{
		m_next = m_prev = NULL;
	}

	m_entryNum = newEntryNum;

	return m_current;
}

}	// namespace internal
/// @endcond

inline void ListEntry::insertObject( ListEntry *newObj )
{
	assert( !newObj->m_next );
	assert( !newObj->m_prev );

	if( m_prev )
	{
		m_prev->m_next = newObj;
		newObj->m_prev = m_prev;
	}
	m_prev = newObj;
	newObj->m_next = this;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename CursorT>
void ListContainer::insertObject( CursorT *cursor, ListEntry *newObj, bool after )
{
	if( m_shutdown )
	{
/***/	return;
	}

	if( !cursor->isValid() )
	{
		addElement( newObj );
		cursor->setPosition( newObj, size()-1 );
	}
	else
	{
		if( after )
		{
			cursor->getCurrent()->appendObject( newObj );
			if( cursor->getCurrent() == m_last )
			{
				m_last = newObj;
			}
		}
		else
		{
			cursor->getCurrent()->insertObject( newObj );
			cursor->setPosition( cursor->getCurrent(), cursor->getPos()+1 );
			if( cursor->getCurrent() == m_first )
			{
				m_first = newObj;
			}
		}
		incNumElements();
	}
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -p.
#	pragma option -a.
#endif

#endif
