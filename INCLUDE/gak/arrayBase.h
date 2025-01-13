/*
		Project:		GAKLIB
		Module:			arrayBase.H
		Description:
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2023 Martin Gäckler

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

#ifndef GAK_ARRAY_BASE_H
#define GAK_ARRAY_BASE_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstdlib>
#include <cstddef>
#include <cassert>

#include <cstdio>

#include <gak/compare.h>
#include <gak/iostream.h>
#include <gak/stdlib.h>
#include <gak/string.h>
#include <gak/container.h>
#include <gak/exception.h>
#include <gak/math.h>
#include <gak/type_traits.h>
#include <gak/logfile.h>

// -------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -x
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc

#	pragma warn -8092		//  ‘type’ argument ‘specifier’ passed to 'function' is not an iterator: ‘type’ iterator required
#	pragma warn -ccc		// condition is allways true/false
#	pragma warn -rch		// code never reached
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

template<typename OBJ>
inline void moveAssign( OBJ &target, OBJ &source )
{
	target = source;
}

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief standard allocator for arrays with C++ objects
	@tparam OBJ the item type to allocate
	@see ArrayBase
*/
template <class OBJ> 
class Allocator
{
	public:
	/// a handle for a memory block
	typedef OBJ *MemHandle;
	/// allocates a new block of data
	static OBJ *alloc( size_t count )
	{
		return new OBJ[count];
	}
	/// frees a new block of data
	static void release( OBJ *data )
	{
		delete [] data;
	}
	/// returns the address of the block
	static const OBJ *getData( const MemHandle &data )
	{
		return data;
	}
	/// returns the address of the block
	static OBJ *getData( MemHandle &data )
	{
		return data;
	}
};

/**
	@brief standard allocator for arrays with C objects (uses calloc instead of new)
	@tparam OBJ the item type to allocate
	@see ArrayBase
*/
template <class OBJ>
class PODallocator
{
	public:
	/// @copydoc Allocator::MemHandle
	typedef OBJ *MemHandle;

	/// @copydoc Allocator::alloc
	static OBJ *alloc( size_t count )
	{
		return static_cast<OBJ*>( calloc( count, sizeof( OBJ ) ) );
	}
	/// @copydoc Allocator::release
	static void release( OBJ *data )
	{
		free( data );
	}
	/// @copydoc Allocator::getData
	static const OBJ *getData( const MemHandle &data )
	{
		return data;
	}
	/// @copydoc Allocator::getData
	static OBJ *getData( MemHandle &data )
	{
		return data;
	}
};

/// @cond
namespace internal
{

template <class OBJ> 
struct Reference
{
	typedef	OBJ			value_type;
	typedef	OBJ			*pointer;
	typedef	OBJ			&reference;
};

template <class OBJ> 
struct ConstReference
{
	typedef	OBJ			value_type;
	typedef	const OBJ	*pointer;
	typedef	const OBJ	&reference;
};

template <class ReferenceT> 
class ReverseIterator : public std::iterator<std::random_access_iterator_tag, typename ReferenceT::value_type>
{
	public:
	typedef typename ReferenceT::value_type		value_type;
	typedef typename ReferenceT::pointer		pointer;
	typedef typename ReferenceT::reference		reference;

	typedef typename Container::size_type		size_type;
	typedef typename Container::difference_type	difference_type;

	private:
	pointer		theData;

	public:
	ReverseIterator( pointer startValue ) : theData( startValue )
	{
	}
	ReverseIterator( const ReverseIterator &src ) : theData( src.theData )
	{
	}
	const ReverseIterator & operator = ( const ReverseIterator &src )
	{
		theData = src.theData;
		return *this;
	}

	operator pointer () const
	{
		return theData;
	}
	pointer operator -> () const
	{
		return theData;
	}
	reference operator * () const
	{
		return *theData;
	}
	const ReverseIterator &operator ++()				// pre inkrement
	{
		--theData;
		return *this;
	}
	ReverseIterator operator ++( int )					// post inkrement
	{
		ReverseIterator		tmp( *this );
		theData--;
		return tmp;
	}
	const ReverseIterator &operator += ( difference_type offset )	// inkrement
	{
		theData -= offset;
		return *this;
	}
	ReverseIterator operator + ( difference_type offset )			// inkrement
	{
		return ReverseIterator<ReferenceT>( this->theData - offset );
	}
	const ReverseIterator &operator --()				// pre dekrement
	{
		++theData;
		return *this;
	}
	ReverseIterator operator --( int )					// post dekrement
	{
		ReverseIterator		temp = *this;
		theData++;
		return temp;
	}
	const ReverseIterator &operator -= ( difference_type offset )	// dekrement
	{
		theData += offset;
		return *this;
	}
	ReverseIterator operator - ( difference_type offset ) const		// dekrement
	{
		return ReverseIterator<ReferenceT>( this->theData + offset );
	}
	difference_type operator - ( const ReverseIterator &oper ) const
	{
		return oper.theData - this->theData;
	}

	reference operator [] ( size_type index ) const
	{
		return theData[index];
	}

	bool operator == (const ReverseIterator &oper) const
	{
		return oper.theData == this->theData;
	}
	bool operator != (const ReverseIterator &oper) const
	{
		return oper.theData != this->theData;
	}

	bool operator <  (const ReverseIterator &oper) const
	{
		return oper.theData < this->theData;
	}
	bool operator <= (const ReverseIterator &oper) const
	{
		return oper.theData <= this->theData;
	}
	bool operator >= (const ReverseIterator &oper) const
	{
		return oper.theData >= this->theData;
	}
	bool operator >  (const ReverseIterator &oper) const
	{
		return oper.theData > this->theData;
	}

};

}	// namespace internal

/// @endcond

/**
	@brief protected base for array type containers

	an array is a container that stores all items in one large memory block
	the class type OBJ must have a default constructor and an accessible assignment 
	operator

	@tparam OBJ the type of the items to store
	@tparam ALLOCATOR the class that allocates the memory
	@see Allocator, PODallocator
*/
template <typename OBJ, typename ALLOCATOR=Allocator<OBJ> >
class ArrayBase : public Container
{
	public:
	typedef typename ALLOCATOR::MemHandle	MemHandle;
	typedef ArrayBase<OBJ,ALLOCATOR> SelfT;

	private:
	MemHandle	m_data;

	std::size_t	m_capacity,
				m_chunkSize;

	protected:
	/// creates an empty array
	ArrayBase()
	{
		forget();
	}
	/**
		@brief creates a new array
		@param [in] initialSize the number of items to create immediately
	*/
	ArrayBase( size_t initialSize )
	{
		forget();
		createElements( initialSize );
	}
	/**
		@brief copy contructor
		@param [in] source the source to copy from
	*/
	ArrayBase( const ArrayBase<OBJ, ALLOCATOR> &source ) : m_data(0)
	{
		m_capacity = 0;
		m_chunkSize = source.m_chunkSize;
		addElements( source );
	}
	/**
		@brief assignment operator
		@param [in] source the source to copy from
	*/
	const ArrayBase<OBJ, ALLOCATOR> &operator = ( const ArrayBase<OBJ, ALLOCATOR> &source )
	{
		if( &source != this )
		{
			clear();
			m_chunkSize = source.m_chunkSize;
			addElements( source );
		}

		return *this;
	}
	~ArrayBase()
	{
		if( m_data )
		{
			ALLOCATOR::release( m_data );
		}
	}


	/*
	-------------------------------------------------------------------------
		Memory
	-------------------------------------------------------------------------
	*/
	/// @name Memory
	///@{
	protected:
	/// @todo realy protected
	/// forget any data without freeing it @see move
	void forget( void )
	{
		m_data = NULL;
		m_capacity = 0;
		m_chunkSize = 32;
		Container::clear();
	}

	public:
	/// returns the MemHandle to the allocated memory block
	MemHandle getDataHandle( void ) const
	{
		return m_data;
	}
	/// sets the chunk (block) size for allocation
	void setChunkSize( size_t chunkSize )
	{
		m_chunkSize = chunkSize;
	}

	/**
		@brief reallocates to a minimum size may delete existing objects
		@param [in] newCapacity new min size of the block
		@param [in] exact set to true, if there is no extra capacity required

	*/
	void setCapacity( size_t newCapacity, bool exact );
	/**
		@brief reallocate to a minimum size does not delete existing objects
		@param [in] newSize new min size of the block
	*/
	void setMinSize( size_t newSize )
	{
		if( newSize > m_capacity )
		{
			setCapacity( newSize, false );
		}
	}
	/**
		@brief reallocate to a size deletes or creates objects
		@param [in] newSize new size of the block
	*/
	void setSize( size_t newSize )
	{
		setCapacity( newSize, true );
		setNumElements( newSize );
	}
	/**
		@brief Moves all items from the source to this array
		The source will be empty after return.
		@param [in|out] source The source array
	*/
	void moveFrom( ArrayBase<OBJ, ALLOCATOR> &source )
	{
		if( &source != this )
		{
			this->clear();
			this->m_data = source.m_data;
			this->m_chunkSize = source.m_chunkSize;
			this->m_capacity = source.m_capacity;

			Container::moveFrom( source );
			source.forget();
		}
	}

	/// removes all items in the buffer and releases all allocated memory
	void clear( void )
	{
		if( m_data )
		{
			ALLOCATOR::release( m_data );
			m_data = NULL;

			m_capacity = 0;
			Container::clear();
		}
	}
	/// returns the size of an item
	size_t getElementSize( void ) const
	{
		return sizeof( OBJ );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Creating new data
	-------------------------------------------------------------------------
	*/
	/// @name Creating new data
	///@{
	protected:
	///	returns a new created element in the buffer
	OBJ &createElement( void )
	{
		return getOrCreateElementAt( size() );
	}
	/**
		@brief creates some elements in this buffer
		@param [in] numElements the number of elements to create
		@return the address if the first created element
	*/
	OBJ *createElements( size_t numElements )
	{
		size_t	oldNumElements = size();
		if( numElements )
		{
			getOrCreateElementAt( oldNumElements + numElements-1 );
		}

		return getDataBuffer() + oldNumElements;
	}

	/**
		@brief returns an item at a position

		if there is no item at the position it will be created

		@param [in] pos the position
		@return the element (created)
	*/
	OBJ &getOrCreateElementAt( size_t pos );
	/**
		@brief inserts a new item at a position

		@param [in] pos the position
		@return the element created
	*/
	OBJ &insertElement( size_t pos )
	{
		return insertElement( OBJ(), pos );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Adding existing data
	-------------------------------------------------------------------------
	*/
	/// @name Adding existing data
	///@{
	protected:
	/**
		@brief insert a element at a position
		@param [in] elem the item to insert
		@param [in] pos the position
		@return the inserted item
	*/
	OBJ &insertElement( const OBJ &elem, size_t pos );

	public:
	/**
		@brief adds a new element to the buffer
		@param [in] item the new item
		@return the new copied item
	*/
	OBJ &addElement( const OBJ &item )
	{
		OBJ	&newData = createElement();

		newData = item;

		return newData;
	}
	/**
		@brief adds a new element to the buffer
		@param [in] item the new item
		@return the buffer itself
	*/
	ArrayBase<OBJ, ALLOCATOR> &operator += ( const OBJ &item )
	{
		addElement( item );

		return *this;
	}
	/**
		@brief add items to the buffer
		@param [in] elem the address of the first item
		@param [in] size the number of items to add
		@return the buffer itself
	*/
	SelfT &addElements( const OBJ *elem, size_t size );

	/**
		@brief add items to the buffer
		@param [in] source the source container
		@return the buffer itself
	*/
	template <typename ArrayT>
	SelfT &addElements( const ArrayT &source )
	{
		return addElements( source.getDataBuffer(), source.size() );
	}

	/**
		@brief add items to the buffer
		@param [in] source the source container
		@return the buffer itself
	*/
	SelfT &merge( const SelfT &source )
	{
		return addElements( source.getDataBuffer(), source.size() );
	}


	///@}

	/*
	-------------------------------------------------------------------------
		Get Data
	-------------------------------------------------------------------------
	*/
	/// @name Get Data
	///@{
	protected:
	/**
		@brief returns a mutable element
		@param [in] pos the position
		@return the element
		@exception IndexError in case of a range error 
	*/
	OBJ &getMutableElementAt( size_t pos )
	{
		if( pos >= size() )
		{
			throw IndexError();
		}

		return getDataBuffer()[pos];
	}
	/**
		@brief returns an imutable element
		@param [in] pos the position
		@return the element
		@exception IndexError in case of a range error 
	*/
	const OBJ& getConstElementAt( size_t pos ) const
	{
		if( pos >= size() )
		{
			throw IndexError();
		}

		return getDataBuffer()[pos];
	}

	public:
	/// returns the address of the buffer
	const OBJ *getDataBuffer( void ) const
	{
		return ALLOCATOR::getData( m_data );
	}
	/// returns the address of the buffer
	OBJ *getDataBuffer( void )
	{
		return ALLOCATOR::getData( m_data );
	}
	/**
		@brief returns the address of an item
		@param [in] pos the position
		@return the element
		@exception IndexError in case of a range error 
	*/
	const OBJ *operator + ( size_t pos ) const
	{
		if( pos >= this->size() )
		{
			throw IndexError();
		}

		return this->getDataBuffer() + pos;
	}
	/**
		@brief returns the address of an item resizes if neccessary
		@param [in] pos the position
		@return the element
	*/
	OBJ *operator + ( size_t pos )
	{
		if( pos >= this->size() )
		{
			this->setSize( pos );
		}
		return this->getDataBuffer() + pos;
	}

	///	@copydoc getConstElementAt
	const OBJ& operator [] (size_t pos) const
	{
		return getConstElementAt( pos );
	}
	///	@copydoc getOrCreateElementAt
	OBJ& operator [] (size_t pos)
	{
		return getOrCreateElementAt( pos );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Modify Data
	-------------------------------------------------------------------------
	*/
	/// @name Modify Data
	///@{
	protected:
	/**
		@brief moves an item
		@param [in] from the current position of the item to move
		@param [in] to the new position
	*/
	void moveElement( size_t from, size_t to );
	/**
		@brief moves an item
		@param [in] searchFor the item to search for
		@param [in] newPosition the new position
	*/
	void moveElement( const OBJ &searchFor, size_t newPosition )
	{
		size_t oldPosition = findElement( searchFor );
		if( oldPosition != no_index && oldPosition != newPosition )
		{
			moveElement( oldPosition, newPosition );
		}
	}

	public:
	/**
		@brief removes an item
		@param [in] pos the position of the item to remove
	*/
	void removeElementAt( size_t pos );
	/**
		@brief removes multiple items
		@param [in] pos the position of the items to remove
		@param [in] count the number of items to remove
	*/
	void removeElementsAt( size_t pos, size_t count );
	/**
		@brief removes an item
		@param [in] elem the item to search for
	*/
	void removeElementVal( const OBJ &elem  )
	{
		size_t	pos = findElement( elem );
		if( pos != no_index )
		{
			removeElementAt( pos );
		}
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Sorting
	-------------------------------------------------------------------------
	*/
	/// @name Sorting
	///@{
	private:
	void swap( size_t i1, size_t i2 )
	{
		assert( i1 != i2 );

		OBJ	*data = getDataBuffer();

		OBJ	tmp = data[i1];
		data[i1] = data[i2];
		data[i2] = tmp;
	}

	template<class Comparator>
	void internalSort(
		size_t				left,
		size_t				right,
		const Comparator	&fcmp
	);

	public:
	/**
		@brief call standard quicksort
		@param [in] fcmp the compare function for quick sort
	*/
	void qsort( int (*fcmp)(const void *, const void *) )
	{
		std::qsort( getDataBuffer(), size(), sizeof( OBJ ), fcmp );
	}

	/**
		@brief return the index of the first item, that is not in sort order
		@param [in] fcmp the compare function must accept two items to compare
		@return the index found, 0 if the buffer is sorted
	*/
	template<class Comparator>
	size_t needSort( Comparator fcmp ) const;

	/**
		@brief sorts the buffer
		@param [in] fcmp the compare function must accept two items to compare
	*/
	template<class Comparator>
	void sort( const Comparator &fcmp )
	{
		doEnterFunctionEx(gakLogging::llDetail, "ArrayBase::sort");
		internalSort( 0, size()-1, fcmp );
	}

	/**
		@brief sorts the buffer
		@param [in] func the compare function must accept two items to compare and a sort criteria
		@param [in] crit the sort criteria
		@see CompareFunction
		@todo better documentation
	*/
	template<class SortFunction, class SortType>
	void sort( SortFunction func, SortType crit )
	{
		CompareFunction<OBJ, SortType> fcmp( func, crit );
		sort( fcmp );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Searching
	-------------------------------------------------------------------------
	*/
	/// @name Searching
	///@{
	public:
	/**
		@brief search for an item
		@param [in] searchFor the item to search
		@return the index of the first item matching the search. no_index if not found
	*/
	size_t findElement( const OBJ &searchFor ) const;

	/**
		@brief test the existence of an item
		@param [in] searchFor the item to search
		@return true if the item was found
	*/
	bool hasElement( const OBJ &searchFor ) const
	{
		return findElement( searchFor ) != no_index;
	}

	/**
		@brief performs a function for each element
		@param [in] func address of a function that accepts an item
	*/
	template <class F> 
	void forEach( const F &func );

	/**
		@brief performs a function for each element
		@param [in] func address of a function that accepts an item
	*/
	template <class F> 
	void forEach( const F &func ) const;

#if !defined( __BORLANDC__ ) || __BORLANDC__ > 0x551
	/**
		@brief performs a function for each element
		@tparam F class with a static member function perform that accepts an item
	*/
	template <class F> 
	void forEach( void )
	{
		OBJ	*dataPtr = getDataBuffer();
		for( size_t i=0; i<size(); i++ )
		{
			F::perform( *dataPtr++ );
		}
	}
	/**
		@brief performs a function for each element
		@tparam F class with a static member function perform that accepts an item
	*/
	template <class F> 
	void forEach( void ) const
	{
		const OBJ	*dataPtr = getDataBuffer();
		for( size_t i=0; i<size(); i++ )
		{
			F::perform( *dataPtr++ );
		}
	}
#endif
	///@}
	/*
	-------------------------------------------------------------------------
		compare
	-------------------------------------------------------------------------
	*/
	public:
	/**
		@brief compares this buffer with another

		the class type OBJ must be a primitive type of have a member function compare

		@param [in] other the other container
		@return -1 if this is less than the other,\n 0 if they are equal,\n 1 otherwise
	*/
	int compare( const ArrayBase<OBJ, ALLOCATOR> &other ) const
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
		@brief reads binary data from a standard C file
		@param [in] fp the file handle to read from
		@param [in] count the number of items to read
		@exception ReadError in case of an I/O error
		@warning use this function with POD items, only
	*/
	void readFromFile( FILE *fp, size_t count )
	{
		assert( is_binary<OBJ>::value );

		setSize( count );
		size_t numElements = fread( getDataBuffer(), sizeof( OBJ ), count, fp );
		if( numElements != count )
		{
			throw ReadError();
		}
	}

	/**
		@brief writes binary data to a standard C file
		@param [in] fp the file handle to write to
		@exception WriteError in case of an I/O error
		@warning use this function with POD items, only
	*/
	void writeToFile( FILE *fp ) const
	{
		assert( is_binary<OBJ>::value );

		size_t numElements = size();
		if( fwrite( getDataBuffer(), sizeof( OBJ ), numElements, fp ) != numElements )
		{
			throw WriteError();
		}
	}

	/**
		@brief writes binary data to a stream
		@param [in] stream the stream to write to
		@exception WriteError in case of an I/O error
	*/
	void toBinaryStream( std::ostream &stream ) const
	{
		if( is_binary<OBJ>::value )
		{
			varArrayToBinaryStream( stream, *this );
		}
		else
		{
			containerToBinaryStream( stream, *this );
		}
	}
	/**
		@brief reads binary data from a stream
		@param [in] stream the stream to read from
		@exception ReadError in case of an I/O error
	*/
	void fromBinaryStream( std::istream &stream )
	{
		if( is_binary<OBJ>::value )
		{
			varArrayFromBinaryStream( stream, *this );
		}
		else
		{
			uint64 numElements;

			gak::fromBinaryStream( stream, &numElements );
			if( numElements > std::numeric_limits<std::size_t>::max() )
			{
				throw AllocError();
			}
			if( numElements )
			{
				setSize( std::size_t(numElements) );

				containerDataFromBinaryStream( stream, *this );
			}
		}
	}

	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	public:
	/// @name Some standard typedefs:
	///@{
	typedef OBJ			value_type;
	typedef OBJ			&reference;
	typedef const OBJ	&const_reference;
	typedef OBJ			*pointer;
	typedef const OBJ	*const_pointer;

	typedef OBJ			*iterator;
	typedef const OBJ	*const_iterator;

	typedef internal::ReverseIterator< internal::Reference< OBJ > >			
						reverse_iterator;
	typedef internal::ReverseIterator< internal::ConstReference< OBJ > >	
						const_reverse_iterator;
	///@}

	/// @name Some standard member functions:
	///@{
	const_iterator cbegin() const
	{
		return getDataBuffer();
	}
	const_iterator cend() const
	{
		return getDataBuffer() + this->size();
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator( getDataBuffer() + this->size() - 1 );
	}
	const_reverse_iterator crend() const
	{
		return const_reverse_iterator( getDataBuffer() - 1 );
	}

	iterator begin()
	{
		return getDataBuffer();
	}
	iterator end()
	{
		return getDataBuffer() + this->size();
	}
	reverse_iterator rbegin()
	{
		return reverse_iterator( getDataBuffer() + this->size() - 1 );
	}
	reverse_iterator rend()
	{
		return reverse_iterator( getDataBuffer() - 1 );
	}

	void push_back( const OBJ &newData )
	{
		addElement( newData );
	}
	void push_front( const OBJ &newData )
	{
		insertElement( newData, 0 );
	}

	iterator erase( const iterator &it )
	{
		size_t	index = it - getDataBuffer();
		removeElementAt( index );
		return getDataBuffer() + index;
	}
	///@}
};

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

template <class OBJ, class ALLOCATOR>
void ArrayBase<OBJ, ALLOCATOR>::moveElement( size_t from, size_t to )
{
	if( from >= size() )
	{
/*@*/	return;
	}

	if( to >= size() )
	{
		to = size()-1;
	}

	OBJ	*data = getDataBuffer();
	OBJ elem = data[from];

	if( from < to )
	{
		for( size_t i=from; i<to; i++ )
		{
			moveAssign( data[i], data[i+1] );
		}
	}
	else
	{
		for( size_t i=from; i>to; i-- )
		{
			moveAssign( data[i], data[i-1] );
		}
	}
	data[to] = elem;
}

template <class OBJ, class ALLOCATOR>
OBJ &ArrayBase<OBJ, ALLOCATOR>::insertElement( const OBJ &elem, size_t pos )
{
	setMinSize( math::max( pos, size()+1 ) );

	size_t	numElements = incNumElements();
	OBJ		*data = getDataBuffer();

	if( numElements > 1 )
	{
		size_t i=numElements-2;
		if( i>=pos )
		{
#ifdef _MSC_VER
#pragma warning ( suppress: 4127 )
#endif
			while( true )
			{
				moveAssign( data[i+1], data[i] );
				if( i == pos )
				{
					break;
				}
				i--;
			}
		}
	}

	data[pos] = elem;
	return data[pos];
}

template <class OBJ, class ALLOCATOR>
OBJ &ArrayBase<OBJ, ALLOCATOR>::getOrCreateElementAt(size_t pos)
{
	setMinSize( pos+1 );

	if( pos < m_capacity )
	{
		if( pos>=size() )
		{
			setNumElements( pos+1 );
		}

		return getDataBuffer()[pos];
	}
	else
	{
		/// @todo IndexError?
		throw IndexError();
	}
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <class OBJ, class ALLOCATOR>
	template <class F>
void ArrayBase<OBJ, ALLOCATOR>::forEach( const F &func )
{
	OBJ	*dataPtr = getDataBuffer();
	for( size_t i=0; i<size(); i++ )
	{
		func( *dataPtr++ );
	}
}

template <class OBJ, class ALLOCATOR>
	template <class F>
void ArrayBase<OBJ, ALLOCATOR>::forEach( const F &func ) const
{
	const OBJ	*dataPtr = getDataBuffer();
	for( size_t i=0; i<size(); i++ )
	{
		func( *dataPtr++ );
	}
}

template <class OBJ, class ALLOCATOR>
	template<class Comparator>
void ArrayBase<OBJ, ALLOCATOR>::internalSort(
	size_t				left,
	size_t				right,
	const Comparator	&fcmp
)
{
	OBJ	*data = getDataBuffer();

	size_t	numElemens = right-left+1;

	if( numElemens <= 1 )	// nothing to do?
		return;
	else if( numElemens == 2 )
	{
		// simple sort, only two elements: compare them and swap if necessary
		if( fcmp( data[left], data[right] ) > 0 )
		{
			swap( left, right );
		}
	}
	else
	{
		// divide the array in two parts
		size_t	pivot = (size_t) ((left+right)/2);
		size_t	i=left;
		size_t	j=right;

		do
		{
			// search first entry bigger than pivot from start
			while( i<pivot && fcmp( data[i], data[pivot] ) <= 0 )
				i++;

			// search first entry smaller than pivot from end
			while( j>pivot && fcmp( data[j], data[pivot] ) >= 0 )
				j--;

			if( i < pivot && pivot < j )
			{
				// if both are on the wrong side, swap them
				swap( i++, j-- );
			}
			else if( i < pivot )
			{
				// if the left one is bigger than the pivot, swap it with pivot
				// and restart
				swap( i, pivot );
//				i = left;
				j = right;
			}
			else if( j > pivot )
			{
				// if the right one is small than the pivot, swap it with the
				// pivot an restart
				swap( j, pivot );
				i = left;
//				j = right;
			}
			else
				break;
#ifdef _MSC_VER
#pragma warning ( suppress: 4127 )
#endif
		} while( true );

		// sort the first half if available
		if (left < pivot-1)
			internalSort( left, pivot-1, fcmp );

		// sort the second half if available
		if (pivot+1 < right)
			internalSort( pivot+1, right, fcmp );
	}
}

template <class OBJ, class ALLOCATOR>
	template<class Comparator>
size_t ArrayBase<OBJ, ALLOCATOR>::needSort( Comparator fcmp ) const
{
	size_t		i = 0, badIndex = 0;
	const OBJ	*prev = this->getDataBuffer();
	const OBJ	*next = prev+1;
	const OBJ	*last = prev+this->size();

	while( next<last )
	{
		i++;
		if( fcmp( *prev++, *next++ ) > 0 )
		{
			badIndex = i;
/*v*/		break;
		}
	}

	return badIndex;
}

template <class OBJ, class ALLOCATOR>
void ArrayBase<OBJ, ALLOCATOR>::removeElementAt( size_t pos )
{
	if( pos < size() )			// is pos within range ?
	{
		OBJ		*data = getDataBuffer();

		size_t	i;
		for( i=pos; i<size()-1; i++ )
		{
			moveAssign( data[i], data[i+1] );
		}
		if( !is_binary<OBJ>::value )
		{
			data[i] = OBJ();
		}

		decNumElements();
	}
}

template <class OBJ, class ALLOCATOR>
void ArrayBase<OBJ, ALLOCATOR>::removeElementsAt( size_t pos, size_t count )
{
	OBJ *data = getDataBuffer();

	size_t	numElements = size();
	if( pos < numElements )			// is pos within range ?
	{
		size_t	i, j;

		for( i=pos, j=pos+count; j<numElements; i++, j++ )
		{
			moveAssign( data[i], data[j] );
		}
		if( !is_binary<OBJ>::value )
		{
			for( ; i<numElements; i++ )
			{
				data[i] = OBJ();
			}
		}
		if( pos + count >= numElements )
		{
			setNumElements( pos );
		}
		else
		{
			setNumElements( numElements - count );
		}
	}
}

template <class OBJ, class ALLOCATOR>
void ArrayBase<OBJ, ALLOCATOR>::setCapacity( size_t newCapacity, bool exact )
{
	if( !exact )
	{
		size_t		chunkSize = newCapacity/8;

		if( chunkSize < m_chunkSize )
		{
			chunkSize = m_chunkSize;
		}
		if( chunkSize < 32 )
		{
			chunkSize = 32;
		}

		newCapacity = ((newCapacity / chunkSize) +1) * chunkSize;
	}

	if( newCapacity != m_capacity )
	{
		typename ALLOCATOR::MemHandle	newData = ALLOCATOR::alloc( newCapacity );

		if( newData )
		{
			if( m_data )
			{
				size_t	newNumElements = math::min( newCapacity, size() );
				for( size_t i=0; i<newNumElements; i++ )
				{
					moveAssign( newData[i], m_data[i] );
				}

				ALLOCATOR::release( m_data );
				setNumElements( newNumElements );
			}
			m_data = newData;
			m_capacity = newCapacity;
		}
		else
		{
			throw AllocError();
		}
	}
}

template <class OBJ, class ALLOCATOR>
typename ArrayBase<OBJ, ALLOCATOR>::SelfT &ArrayBase<OBJ, ALLOCATOR>::addElements( const OBJ *elem, size_t size )
{
	OBJ	*newElements = createElements( size );

	for( size_t i=0; i<size; i++ )
	{
		*newElements++ = *elem++;
	}

	return *this;
}

template <class OBJ, class ALLOCATOR>
size_t ArrayBase<OBJ, ALLOCATOR>::findElement( const OBJ &searchFor ) const
{
	size_t		i = this->size()-1;
	const OBJ	*ptr = this->getDataBuffer()+ i;
	for(; i!=no_index; --i, --ptr )
	{
		if( *ptr == searchFor )
		{
			break;
		}
	}

	return i;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template<typename OBJ, typename ALLOCATOR>
inline void moveAssign( ArrayBase<OBJ, ALLOCATOR> &target, ArrayBase<OBJ, ALLOCATOR> &source )
{
	target.moveFrom( source );
}

template <class OBJ, class ALLOCATOR>
inline
std::ostream &operator << ( std::ostream &stream, const ArrayBase<OBJ, ALLOCATOR> &array )
{
	printContainer( stream, array );

	return stream;
}

#if 0
template <class OBJ> std::istream &operator >> ( std::istream &source, ArrayBase<OBJ, ALLOCATOR> &array )
{
	size_t	numElements;

	source.read( (char *)&numElements, sizeof( numElements ) );
	array.resize( numElements );
	for( size_t i=0; i<numElements; i++ )
		source >> array[i];

	return source;
}
#endif

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -x.
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.

#   pragma warn +ccc
#	pragma warn +rch
#endif

#endif	// GAK_ARRAY_BASE_H
