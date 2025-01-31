/*
		Project:		GAKLIB
		Module:			btree.h
		Description:	binary tree, binary map
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
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

#ifndef GAK_SIMPLE_BTREE_H
#define GAK_SIMPLE_BTREE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <assert.h>
#include <iterator>

#include <gak/container.h>
#include <gak/compare.h>
#include <gak/iostream.h>
#include <gak/keyValuePair.h>

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
	@brief a balanced binary tree for fast inserting and searching
	@tparam OBJ the type of items to store in this Container
	@tparam Comparator the type that can compare two items
	@tparam FACTOR int value that controls the balancing
	@tparam OFFSET int value that controls the balancing
	@todo better documentation
	@see FixedComparator, DynamicComparator, CompareFunction, TreeMap
*/
template <class OBJ, class Comparator=FixedComparator<OBJ>, int FACTOR=10, int OFFSET=5>
class Btree : public Container
{
	public:
	typedef Btree<OBJ, Comparator, FACTOR, OFFSET> SelfT;

	/// @todo check bug in gcc
#ifndef __GNUC__
	template < class KeyT, class ValueT, class CompT >
	friend class TreeMap;

	private:
#else
	protected:
#endif
	struct Node
	{
		size_t	m_count;
		Node	*m_parent, *m_prev, *m_next;
		OBJ		m_data;

		bool isPrevBalanced( size_t prevCount, size_t nextCount )
		{
			if( FACTOR && OFFSET && prevCount * FACTOR + OFFSET < nextCount )
			{
				return false;
			}
			return true;
		}
		bool isNextBalanced( size_t nextCount, size_t prevCount )
		{
			return isPrevBalanced( nextCount, prevCount );
		}
		size_t testCount( size_t level )
		{
			level++;
			size_t	prevCount = m_prev ? m_prev->testCount( level ) : 0;
			size_t	nextCount = m_next ? m_next->testCount( level ) : 0;

			assert( prevCount + nextCount + 1 == m_count );

			return m_count;
		}
		bool testBalance( size_t level )
		{
			level++;
			size_t	prevCount = m_prev ? m_prev->m_count : 0;
			size_t	nextCount = m_next ? m_next->m_count : 0;

			if( m_prev && !m_prev->testBalance( level ) )
			{
				return false;
			}
			if( m_next && !m_next->testBalance( level ) )
			{
				return false;
			}

			assert( isPrevBalanced( prevCount, nextCount ) );
			assert( isNextBalanced( nextCount, prevCount ) );

			return true;
		}
		size_t testDepth( size_t level )
		{
			level++;
			size_t	prevDepth = m_prev ? m_prev->testDepth( level ) : 0;
			size_t	nextDepth = m_next ? m_next->testDepth( level ) : 0;

			if( prevDepth > level )
			{
				level = prevDepth;
			}
			if( nextDepth > level )
			{
				level = nextDepth;
			}
			assert( level < 100000 );

			return level;
		}
		bool testPointer( size_t level )
		{
			level++;
			if( m_prev )
			{
				assert( m_prev->m_parent == this );
				assert( m_prev->testPointer( level ) );
			}
			if( m_next )
			{
				assert( m_next->m_parent == this );
				assert( m_next->testPointer( level ) );
			}

			return true;
		}
		Node()
		{
			m_count = 1;
			m_parent = m_prev = m_next = NULL;
		}
		Node( const OBJ &data ) : m_data( data )
		{
			m_count = 1;
			m_parent = m_prev = m_next = NULL;
		}
		~Node()
		{
			if( m_prev )
			{
				delete m_prev;
			}
			if( m_next )
			{
				delete m_next;
			}
		}

		Node *getFirst() const;
		Node *getNext() const;
		Node *getLast() const;
		Node *getPrev() const;

		template <class FunctionT>
		void checkBalance( Node **root, const FunctionT &compare  );

		template <class FunctionT>
		void addNode2Child( Node *newNode, Node **root, const FunctionT &compare );

		template <class FunctionT>
		void addNode( Node *newNode, Node **root, const FunctionT &compare );

		template <class FunctionT>
		Node *unlinkSelf( const FunctionT &compare );

		template <class FunctionT>
		void unlinkChild( Node *node, const FunctionT &compare );

		template <class KEY, class FunctionT>
		Node *findNode( const KEY &key, const FunctionT &compare  ) const
		{
			int compareResult = compare( this->m_data, key );
			if( compareResult > 0 )			// i'm greater
			{
				return m_prev ? m_prev->findNode( key, compare ) : NULL;
			}
			else if( compareResult < 0 )	// i'm smaller
			{
				return m_next ? m_next->findNode( key, compare ) : NULL;
			}
			else
			{
				return const_cast<Node *>(this);
			}
		}
		template <class KEY, class FunctionT>
		Node *findMinNode( const KEY &key, const FunctionT &compare  ) const;
		template <class KEY, class FunctionT>
		Node *findMaxNode( const KEY &key, const FunctionT &compare  ) const;
	};

	Node			*m_root;
	Comparator		m_comparator;


	public:
	/// creates an empty tree
	Btree( const Comparator &comparator = Comparator() ) : m_comparator( comparator )
	{
		m_root = NULL;
	}

	/// copy constructor
	Btree( const Btree &src )
	{
		m_root = NULL;
		m_comparator = src.m_comparator;
		std::copy(src.cbegin(), src.cend(), std::back_inserter( *this ));
	}
	/// copy assignment
	const Btree &operator = ( const Btree &src )
	{
		clear();
		m_comparator = src.m_comparator;
		std::copy(src.cbegin(), src.cend(), std::back_inserter( *this ));

		return *this;
	}
	/// destroys the Container and all its items
	~Btree()
	{
		if( m_root )
		{
			delete m_root;
		}
	}

	/*
	-------------------------------------------------------------------------
		Memory
	-------------------------------------------------------------------------
	*/
	/// @name Memory
	///@{
#ifndef __GNUC__
	private:
#else
	protected:
#endif
	void forget( void )
	{
		m_root = NULL;
		Container::clear();
	}
	public:
	/// deletes all items in this container
	void clear( void )
	{
		if( m_root )
		{
			delete m_root;
			forget();
		}
		Container::clear();
	}
	/**
		@brief moves all items to the target array
		this container will be empty after return
		@param [out] target the target array
	*/
	void moveFrom( Btree<OBJ, Comparator, FACTOR, OFFSET> &source )
	{
		if( this != &source )
		{
			this->clear();
			this->m_root = source.m_root;
			Container::moveFrom( source );
			source.forget();
		}
	}
	/// returns the size of an item
	size_t getElementSize( void ) const
	{
		return sizeof( OBJ );
	}

	/**
		@brief copies all items from source to the array
		@param [in] target the target array
		@return this
		@todo implement
	*/
	SelfT &merge( const SelfT &src )
	{
		throw NotImplementedException();
		return *this;
	}

	/*
	-------------------------------------------------------------------------
		Adding existing data
	-------------------------------------------------------------------------
	*/
	/// @name Adding existing data
	///@{
	public:
	/**
		@brief adds a new element to the buffer
		@param [in] data the new item
		@return the new copied item
	*/
	OBJ &addElement( const OBJ &data )
	{
		Node	*newNode = new Node( data );
		if( m_root )
		{
			m_root->addNode( newNode, &m_root, m_comparator );
		}
		else
		{
			m_root = newNode;
		}
		incNumElements();

		return newNode->m_data;
	}

	/**
		@brief adds a new element to the buffer
		@param [in] data the new item
		@return the buffer itself
	*/
	const Btree<OBJ, Comparator, FACTOR, OFFSET> &operator += ( const OBJ &data )
	{
		addElement( data );
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
	/// returns the first element in the tree
	const OBJ *getFirst( void ) const
	{
		return m_root ? &m_root->getFirst()->m_data : NULL;
	}
	/// returns the last element in the tree
	const OBJ *getLast( void ) const
	{
		return m_root ? &m_root->getLast()->m_data : NULL;
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Modify Data
	-------------------------------------------------------------------------
	*/
	/// @name Modify Data
	///@{
#ifndef __GNUC__
	private:
#else
	protected:
#endif
	void removeElement( Node *data );

	public:
	/// searches for an item and removes the item if found
	void removeElement( const OBJ &data );
	///@}

	/*
	-------------------------------------------------------------------------
		Sorting
	-------------------------------------------------------------------------
	*/
	/// @name Sorting
	///@{
	private:
	void resort( void );

	public:
	/**
		@brief changes the sort order of the tree
		@param [in] comparator a new comparator 
		@see FixedComparator, DynamicComparator
	*/
	template <typename ComparatorT>
	void resort( const ComparatorT &comparator )
	{
		m_comparator = comparator;
		resort();
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Searching
	-------------------------------------------------------------------------
	*/
	/// @name Searching
	///@{
	private:
	Node *findNode( const OBJ &data ) const
	{
		return m_root ? m_root->findNode( data, m_comparator ) : NULL;
	}
	Node *findMinNode( const OBJ &data ) const
	{
		return m_root ? m_root->findMinNode( data, m_comparator ) : NULL;
	}

	public:
	/// return true if a given value is available
	bool hasElement( const OBJ &data ) const
	{
		return findNode( data ) != NULL;
	}
	/**
		@brief searches for a value
		@param [in] data the value to search for
		@return the address of the element found NULL if not found
	*/
	const OBJ *findElement( const OBJ &data ) const
	{
		const Node *node = findNode( data );
		return  node ? &node->m_data : NULL;
	}
	class const_iterator;

	ConstIterable<const_iterator> getRange( const OBJ &min, const OBJ &max ) const
	{
		assert( min <= max );
		return ConstIterable<const_iterator>(
			const_iterator( findMinNode( min ) ), const_iterator( findMinNode( max ) )
		);
	}
	///@}

	/*
	-------------------------------------------------------------------------
		compare
	-------------------------------------------------------------------------
	*/
	public:
	/**
		@brief compares this buffer with another
		@param [in] other the other container
		@return the compare result
		@see SortedArray, gak::compareContainer
	*/
	int compare( const Btree<OBJ, Comparator, FACTOR, OFFSET> &other ) const
	{
		return compareContainer( *this, other );
	}


	/*
	-------------------------------------------------------------------------
		File I/O
	-------------------------------------------------------------------------
	*/
	/// @name File I/O
	///@{
	public:
	/**
		@brief writes binary data to a stream
		@param [in] stream the stream to write to
		@exception WriteError in case of an I/O error
	*/
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::containerToBinaryStream( stream, *this );
	}
	/**
		@brief reads binary data from a stream
		@param [in] stream the stream to read from
		@exception ReadError in case of an I/O error
	*/
	void fromBinaryStream( std::istream &stream );
	///@}

	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	public:
	/// @name Some standard typedefs:
	///@{
	typedef OBJ			value_type;
	typedef const OBJ	&reference;
	typedef const OBJ	&const_reference;
	typedef const OBJ	*pointer;
	typedef const OBJ	*const_pointer;
	///@}

	private:
	class iterator_base : public std::iterator<std::bidirectional_iterator_tag, value_type>
	{
		protected:
		Node *value;


		public:
		Node *getNode( void ) const
		{
			return const_cast<Node *>(value);
		}

		iterator_base( Node *start )
		{
			value = start;
		}
		iterator_base( const iterator_base &start )
		{
			value = start.value;
		}
		const iterator_base & operator = ( const iterator_base &src )
		{
			value = src.value;
			return *this;
		}

		int compare( const iterator_base &oper ) const
		{
			return int(oper.value - this->value);
		}
	};

	public:
	/// @name Some standard classes:
	///@{
	class iterator : public iterator_base
	{
		public:

		iterator( Node *start ) : iterator_base( start )
		{
		}
		iterator( const iterator &start )  : iterator_base( start )
		{
		}

		// can change data
		operator OBJ * ()
		{
			return &this->value->m_data;
		}
		OBJ *operator -> ()
		{
			return &this->value->m_data;
		}
		OBJ & operator * ()
		{
			return this->value->m_data;
		}

		// from front to back
		const iterator &operator ++()				// pre inkrement
		{
			this->value = this->value->getNext();
			return *this;
		}
		iterator operator ++( int )					// post inkrement
		{
			iterator		temp( *this );
			this->value = this->value->getNext();
			return temp;
		}
		const iterator &operator --()				// pre dekrement
		{
			this->value = this->value->getPrev();
			return *this;
		}
		iterator operator --( int )					// post dekrement
		{
			iterator	temp = *this;
			this->value = this->value->getPrev();
			return temp;
		}
	};
	class reverse_iterator : public iterator_base
	{
		public:

		reverse_iterator( Node *start ) : iterator_base( start )
		{
		}
		reverse_iterator( const reverse_iterator &start )  : iterator_base( start )
		{
		}

		// can change data
		operator OBJ * ()
		{
			return &this->value->data;
		}
		OBJ *operator -> ()
		{
			return &this->value->m_data;
		}
		OBJ & operator * ()
		{
			return this->value->m_data;
		}

		// from back to front
		const reverse_iterator &operator ++()				// pre inkrement
		{
			this->value = this->value->getPrev();
			return *this;
		}
		reverse_iterator operator ++( int )					// post inkrement
		{
			iterator		temp( *this );
			this->value = this->value->getPrev();
			return temp;
		}
		const reverse_iterator &operator --()				// pre dekrement
		{
			this->value = this->value->getNext();
			return *this;
		}
		reverse_iterator operator --( int )					// post dekrement
		{
			iterator	temp = *this;
			this->value = this->value->getNext();
			return temp;
		}
	};

	class const_iterator : public iterator_base
	{
		public:

		const_iterator( Node *start ) : iterator_base( start )
		{
		}
		const_iterator( const const_iterator &start )  : iterator_base( start )
		{
		}

		// cannot change data
		operator const OBJ * () const
		{
			return &this->value->m_data;
		}
		const OBJ *operator -> () const
		{
			return &this->value->m_data;
		}
		const OBJ & operator * () const
		{
			return this->value->m_data;
		}

		// from front to back
		const const_iterator &operator ++()				// pre inkrement
		{
			this->value = this->value->getNext();
			return *this;
		}
		const_iterator operator ++( int )					// post inkrement
		{
			const_iterator		temp( *this );
			this->value = this->value->getNext();
			return temp;
		}
		const const_iterator &operator --()					// pre dekrement
		{
			this->value = this->value->getPrev();
			return *this;
		}
		const_iterator operator --( int )					// post dekrement
		{
			const_iterator	temp = *this;
			this->value = this->value->getPrev();
			return temp;
		}
	};

	class const_reverse_iterator : public iterator_base
	{
		public:

		const_reverse_iterator(  Node *start ) : iterator_base( start )
		{
		}
		const_reverse_iterator( const const_reverse_iterator &start )  : iterator_base( start )
		{
		}
		
		// cannot change data
		operator const OBJ * () const
		{
			return &this->value->m_data;
		}
		const OBJ *operator -> () const
		{
			return &this->value->m_data;
		}
		const OBJ & operator * () const
		{
			return this->value->m_data;
		}

		// from back to front
		const const_reverse_iterator &operator ++()				// pre inkrement
		{
			this->value = this->value->getPrev();
			return *this;
		}
		const_reverse_iterator operator ++( int )					// post inkrement
		{
			const_reverse_iterator		temp( *this );
			this->value = this->value->getPrev();
			return temp;
		}
		const const_reverse_iterator &operator --()				// pre dekrement
		{
			this->value = this->value->getNext();
			return *this;
		}
		const_reverse_iterator operator --( int )					// post dekrement
		{
			const_reverse_iterator	temp = *this;
			this->value = this->value->getNext();
			return temp;
		}
	};
	///@}

	/// @name Some standard member functions:
	///@{
	iterator begin()
	{
		return iterator( m_root ? m_root->getFirst() : NULL );
	}
	iterator end()
	{
		return iterator( NULL );
	}
	const_iterator cbegin() const
	{
		return const_iterator( m_root ? m_root->getFirst() : NULL );
	}
	const_iterator cend() const
	{
		return const_iterator( NULL );
	}
	reverse_iterator rbegin() const
	{
		return reverse_iterator( m_root ? m_root->getLast() : NULL );
	}
	reverse_iterator rend() const
	{
		return reverse_iterator( NULL );
	}
	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator( m_root ? m_root->getLast() : NULL );
	}
	const_reverse_iterator crend() const
	{
		return const_reverse_iterator( NULL );
	}

	void push_back( const OBJ &newData )
	{
		addElement( newData );
	}
	void push_front( const OBJ &newData )
	{
		addElement( newData );
	}
	iterator erase( const iterator &_it );
	///@}

	/// used for internal testing
	size_t test( bool noBalance = false )
	{
		assert( (m_root ? m_root->testCount( 0 ) : 0) == size() );

		if( FACTOR && OFFSET && !noBalance && m_root )
		{
			assert( m_root->testBalance( 0 ) );
		}

		assert( m_root ? m_root->testPointer( 0 ) : true );
		size_t depth = m_root ? m_root->testDepth( 0 ) : 0;
		assert( depth < 10000 );

		return depth;
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

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::removeElement( Node *node )
{
	assert( node );

	if( node == m_root )
	{
		m_root = node->unlinkSelf( m_comparator );
	}
	else
	{
		node->m_parent->unlinkChild( node, m_comparator );
	}

	assert( node->m_parent == NULL );
	assert( node->m_prev == NULL );
	assert( node->m_next == NULL );
	delete node;
	decNumElements();
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::getFirst() const
{
	Node *current = const_cast<Node*>(this);
	while( current->m_prev )
	{
		current = current->m_prev;
	}

	return current;
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::getNext() const
{
	if( m_next )
	{
		return m_next->getFirst();
	}
	const Node *current = this;
	while( current->m_parent && current->m_parent->m_next == current )
	{
		current = current->m_parent;
	}

	return current->m_parent;
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::getLast() const
{
	Node *current = const_cast<Node*>(this);
	while( current->m_next )
	{
		current = current->m_next;
	}

	return current;
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::getPrev() const
{
	if( m_prev )
	{
		return m_prev->getLast();
	}
	const Node *current = this;
	while( current->m_parent && current->m_parent->m_prev == current )
	{
		current = current->m_parent;
	}

	return current->m_parent;
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class FunctionT>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::unlinkSelf( const FunctionT &compare )
{
	Node	*newRoot;
	if( m_prev && m_next )
	{
		if( m_prev->m_count < m_next->m_count )
		{
			m_prev->m_parent = NULL;
			m_next->addNode( m_prev, &m_next, compare );
			newRoot = m_next;
		}
		else
		{
			m_next->m_parent = NULL;
			m_prev->addNode( m_next, &m_prev, compare );
			newRoot = m_prev;
		}
		newRoot->m_parent = NULL;
		m_next = m_prev = NULL;
	}
	else if( m_prev )
	{
		m_prev->m_parent = NULL;
		newRoot = m_prev;
		m_prev = NULL;
	}
	else if( m_next )
	{
		m_next->m_parent = NULL;
		newRoot = m_next;
		m_next = NULL;
	}
	else
	{
		newRoot = NULL;
	}
	m_parent = NULL;
	m_count = 1;

	return newRoot;
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class FunctionT>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::unlinkChild( Node *node, const FunctionT &compare )
{
	assert( node == m_prev || node == m_next );

	if( node == m_prev )
	{
		m_prev = node->unlinkSelf( compare );
		if( m_prev )
		{
			m_prev->m_parent = this;
		}
	}
	else
	{
		m_next = node->unlinkSelf( compare );
		if( m_next )
		{
			m_next->m_parent = this;
		}
	}
	Node *parent = this;
	while( parent )
	{
		parent->m_count--;
		parent = parent->m_parent;
	}
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class FunctionT>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::checkBalance( Node **root, const FunctionT &compare  )
{
	size_t prevCount = m_prev ? m_prev->m_count : 0;
	size_t nextCount = m_next ? m_next->m_count : 0;
	assert( !m_parent || &m_parent->m_prev == root || &m_parent->m_next == root );

	if( !isPrevBalanced( prevCount, nextCount ) )
	{
		// add previous tree to next tree
		if( m_prev )
		{
			m_prev->m_parent = NULL;
			m_next->addNode( m_prev, &m_next, compare  );
			assert( m_next->m_count == m_count-1 );
			m_prev = NULL;
		}

		// move next to root
		*root = m_next;
		m_next->m_parent = m_parent;

		// remove me from tree
		m_next = NULL;
		m_parent = NULL;
		m_count = 1;

		// add myself to the new root
		(*root)->addNode( this, root, compare  );
	}
	else if( !isNextBalanced( nextCount, prevCount ) )
	{
		// add next tree to previous tree
		if( m_next )
		{
			m_next->m_parent = NULL;
			m_prev->addNode( m_next, &m_prev, compare );
			assert( m_prev->m_count == m_count-1 );
			m_next = NULL;
		}

		// move prev to root
		*root = m_prev;
		m_prev->m_parent = m_parent;

		// remove me from tree
		m_prev = NULL;
		m_parent = NULL;
		m_count = 1;

		// add myself to the new root
		(*root)->addNode( this, root, compare  );
	}

	// assert( testCount( 0 ) );
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class FunctionT>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::addNode2Child( Node *newNode, Node **root, const FunctionT &compare )
{
	assert( root == &m_prev || root == &m_next );

	if( *root )
	{
		//assert( (*root)->testCount( 0 ) );
		(*root)->addNode( newNode, root, compare );
		//assert( (*root)->testCount( 0 ) );

	}
	else
	{
		*root = newNode;
		newNode->m_parent = this;
	}
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class KEY, class FunctionT>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::findMinNode( 
	const KEY &key, const FunctionT &compare  
) const
{
	Node *result;

	int compareResult = compare( this->m_data, key );
	if( compareResult > 0 )			// i'm greater
	{
		if( m_prev )
		{
			result = m_prev->findMinNode( key, compare );
			if( !result )
			{
				result = const_cast<Node *>(this);
			}
		}
		else
		{
			result = const_cast<Node *>(this);
		}
	}
	else if( compareResult < 0 )	// i'm smaller
	{
		result = m_next ? m_next->findMinNode( key, compare ) : NULL;
	}
	else
	{
		result = const_cast<Node *>(this);
		while( true )
		{
			Node *prev = result->getPrev();
			if( !prev || compare( prev->m_data, key ) )
			{
				break;
			}
			result = prev;
		} 
	}
	return result; 
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class KEY, class FunctionT>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::Node *Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::findMaxNode( 
	const KEY &key, const FunctionT &compare  
) const
{
	Node *result;

	int compareResult = compare( this->m_data, key );
	if( compareResult > 0 )			// i'm greater
	{
		result = m_prev ? m_prev->findMaxNode( key, compare ) : NULL;
	}
	else if( compareResult < 0 )	// i'm smaller
	{
		if( m_next )
		{
			result = m_next->findMaxNode( key, compare );
			if( !result )
			{
				result = const_cast<Node *>(this);
			}
		}
		else
		{
			result = const_cast<Node *>(this);
		}
	}
	else
	{
		result = const_cast<Node *>(this);
		while( true )
		{
			Node *next = result->getNext();
			if( !next || compare( next->m_data, key ) )
			{
				break;
			}
			result = next;
		} 
	}
	return result; 
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
	template <class FunctionT>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::Node::addNode( Node *newNode, Node **root, const FunctionT &compare )
{
	assert( newNode );
	assert( !newNode->m_parent );
	//assert( newNode->testCount( 0 ) );

	m_count += newNode->m_count;

	int compareResult = compare( newNode->m_data, m_data );
	if( compareResult < 0 )
	{
		addNode2Child( newNode, &m_prev, compare  );
	}
	else if( compareResult > 0 )
	{
		addNode2Child( newNode, &m_next, compare  );
	}
	else
	{
		size_t prevCount = m_prev ? m_prev->m_count : 0;
		size_t nextCount = m_next ? m_next->m_count : 0;
		if( nextCount < prevCount )
		{
			addNode2Child( newNode, &m_next, compare  );
		}
		else
		{
			addNode2Child( newNode, &m_prev, compare  );
		}
	}

#if defined( __BORLANDC__ )
#	pragma warn -8008
#endif
	if( FACTOR && OFFSET )
		checkBalance( root, compare  );
#if defined( __BORLANDC__ )
#	pragma warn .8008
#endif
	//assert( testCount( 0 ) );
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::removeElement( const OBJ &data )
{
	Node *node = findNode( data );
	if( node )
	{
		removeElement( node );
	}
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
typename Btree<OBJ,Comparator, FACTOR, OFFSET>::iterator Btree<OBJ,Comparator, FACTOR, OFFSET>::erase( const iterator &_it )
{
	iterator it = _it;
	Node *node = it.getNode();
	if( node )
	{
		++it;
		removeElement( node );
	}

	return it;
}

#undef max

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
void Btree<OBJ,Comparator, FACTOR, OFFSET>::fromBinaryStream( std::istream &stream )
{
	uint64		numElements;

	clear();
	gak::fromBinaryStream( stream, &numElements );
	if( numElements )
	{
		if( numElements > std::numeric_limits<std::size_t>::max() )
		{
			throw AllocError();
		}
		for( size_t i=0; i<numElements; ++i )
		{
			OBJ	newData;
			gak::fromBinaryStream( stream, &newData );
			addElement( newData );
		}
	}
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
void Btree<OBJ, Comparator, FACTOR, OFFSET>::resort( void )
{
	if( this->size() < 2 )
	{
		return;
	}

	Node	*current = this->m_root->getFirst();
	Node	*next = current->getNext();

	if( current == this->m_root )
	{
		this->m_root = current->unlinkSelf( m_comparator );
	}
	else
	{
		current->m_parent->unlinkChild( current, m_comparator );
	}

	Node	*newRoot = current;
	size_t	tmp = 0;
	current = next;
	while( current )
	{
		tmp++;

		assert( tmp < this->size() );

		next = current->getNext();

		if( current == this->m_root )
		{
			this->m_root = current->unlinkSelf( m_comparator );
		}
		else
		{
			current->m_parent->unlinkChild( current, m_comparator );
		}

		newRoot->addNode( current, &newRoot, m_comparator );
		current = next;
	}

	this->m_root = newRoot;
}



// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
inline Btree<OBJ, Comparator, FACTOR, OFFSET> intersect(
	const Btree<OBJ, Comparator, FACTOR, OFFSET> &first,
	const Btree<OBJ, Comparator, FACTOR, OFFSET> &second
)
{
	Btree<OBJ, Comparator, FACTOR, OFFSET>	result;

	intersectSorted( first, second, std::back_inserter( result ) );

	return result;
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
inline Btree<OBJ, Comparator, FACTOR, OFFSET> substract(
	const Btree<OBJ, Comparator, FACTOR, OFFSET> &first, 
	const Btree<OBJ, Comparator, FACTOR, OFFSET> &second 
)
{
	return substractSorted( first, second );
}

template <class OBJ, class Comparator, int FACTOR, int OFFSET>
inline void moveAssign(
	Btree<OBJ, Comparator, FACTOR, OFFSET> &target,
	Btree<OBJ, Comparator, FACTOR, OFFSET> &source )
{
	target.moveFrom( source );
}



/// operator to print a Btree to a text stream
template <class OBJ, class Comparator, int FACTOR, int OFFSET>
inline
std::ostream &operator << (std::ostream &stream, const Btree<OBJ,Comparator, FACTOR, OFFSET> &tree )
{
	printContainer( stream, tree );

	return stream;
}


}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_SIMPLE_BTREE_H
