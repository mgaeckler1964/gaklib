/*
		Project:		GAKLIB
		Module:			set.h
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

#ifndef GAK_SET_H
#define GAK_SET_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/sortedArray.h>

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
	@brief An Array without duplicate items.

	Adding duplicate items is ignored

	@tparam OBJ the type of the items to store
	@tparam ALLOCATOR the class that allocates the memory
	@see Allocator, PODallocator, Set
*/
template <typename OBJ, typename ALLOCATOR=Allocator<OBJ> >
class UnorderedSet : public ArrayBase<OBJ, ALLOCATOR>
{
	public:
	/*
	-------------------------------------------------------------------------
		Adding existing data
	-------------------------------------------------------------------------
	*/
	/// @name Adding existing data
	///@{

	/**
		@copydoc ArrayBase::addElement

		a duplicate item is ignored the function will return the existing item
	*/
	OBJ &addElement( const OBJ &item )
	{
		size_t	pos = findElement( item );
		if( pos == this->no_index )
		{
			return ArrayBase<OBJ, ALLOCATOR>::addElement( item );
		}
		else
		{
			return this->getDataBuffer()[pos];
		}
	}
	/**
		@copydoc ArrayBase::operator+=

		a duplicate item is ignored
	*/
	UnorderedSet<OBJ, ALLOCATOR> &operator += ( const OBJ &item )
	{
		addElement( item );

		return *this;
	}

	/**
		@copydoc ArrayBase::addElements( const OBJ *elem, size_t size )

		duplicate items are ignored
	*/
	void addElements( const OBJ *elem, size_t size );
	/**
		@copydoc ArrayBase::addElements( const ArrayT &source )

		duplicate items are ignored
	*/
	template <typename ArrayT>
	void addElements( const ArrayT &source )
	{
		addElements( source.getDataBuffer(), source.size() );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Get Data
	-------------------------------------------------------------------------
	*/
	/// @name Get Data
	///@{

	///@copydoc ArrayBase::operator[]
	const OBJ &operator [] ( size_t pos ) const
	{
		return ArrayBase<OBJ>::operator []( pos );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Modify Data
	-------------------------------------------------------------------------
	*/

	/// @name Modify Data
	///@{

	/// @copydoc ArrayBase::removeElementVal
	void removeElement( const OBJ &elem )
	{
		return ArrayBase<OBJ>::removeElementVal( elem );
	}

	/// @copydoc ArrayBase::moveElement( size_t from, size_t to );
	void moveElement( size_t from, size_t to )
	{
		ArrayBase<OBJ>::moveElement( from, to );
	}
	/// @copydoc ArrayBase::moveElement( const OBJ &searchFor, size_t newPosition )
	void moveElement( const OBJ &searchFor, size_t newPosition )
	{
		ArrayBase<OBJ>::moveElement( searchFor, newPosition );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	/// @name Some standard typedefs:
	///@{
	typedef typename ArrayBase<OBJ>::const_iterator			iterator;
	typedef typename ArrayBase<OBJ>::const_reverse_iterator	reverse_iterator;
	///@}

	/// @name Some standard member functions:
	///@{
	iterator begin()
	{
		return ArrayBase<OBJ>::cbegin();
	}
	iterator end()
	{
		return ArrayBase<OBJ>::cend();
	}
	reverse_iterator rbegin()
	{
		return ArrayBase<OBJ>::crbegin();
	}
	reverse_iterator rend()
	{
		return ArrayBase<OBJ>::crend();
	}
	iterator erase( const iterator &it )
	{
		size_t	index = it - this->getDataBuffer();
		this->removeElementAt( index );
		return this->getDataBuffer() + index;
	}
	///@}
};

/**
	@brief A SortedArray without duplicate items.

	Adding duplicate items is ignored

	@tparam OBJ the type of the items to store
	@tparam ALLOCATOR the class that allocates the memory
	@tparam Comparator the class that can compare two items
	@see Allocator, PODallocator, FixedComparator, CompareFunction, UnorderedSet
*/
template <
	typename OBJ, 
	typename Comparator=FixedComparator<OBJ>, 
	typename ALLOCATOR=Allocator<OBJ> 
>
class Set : public SortedArray<OBJ, Comparator, ALLOCATOR>
{
	typedef SortedArray<OBJ, Comparator, ALLOCATOR>	Super;

	/*
	-------------------------------------------------------------------------
		Memory
	-------------------------------------------------------------------------
	*/
	/// @name Memory
	///@{
	public:
	/**
		@copydoc SortedArray::moveFrom
	*/
	void moveFrom( Set<OBJ, Comparator, ALLOCATOR> &source )
	{
		Super::moveFrom( source );
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
		@copydoc SortedArray::addElement

		a duplicate item is ignored the function will return the existing item
	*/
	const OBJ &addElement( const OBJ &item )
	{
		size_t	pos = findElement( item );
		if( pos == this->no_index )
		{
			return SortedArray<OBJ, Comparator, ALLOCATOR>::addElement( item );
		}
		else
		{
			return (*this)[pos];
		}
	}
	/**
		@copydoc SortedArray::operator+=

		a duplicate item is ignored
	*/
	Set<OBJ, Comparator, ALLOCATOR> &operator += ( const OBJ &item )
	{
		addElement( item );

		return *this;
	}
	/**
		@copydoc SortedArray::addElements( const OBJ *elem, size_t size )

		duplicate items are ignored
	*/
	void addElements( const OBJ *elem, size_t size );
	/**
		@copydoc SortedArray::addElements( const ArrayT &source )

		duplicate items are ignored
	*/
	template <typename ArrayT>
	void addElements( const ArrayT &source )
	{
		addElements( source.getDataBuffer(), source.size() );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	/// @name Some standard member functions:
	///@{
	void push_back( const OBJ &newData )
	{
		addElement( newData );
	}
	void push_front( const OBJ &newData )
	{
		addElement( newData );
	}
	///@}
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

template <typename OBJ, typename ALLOCATOR>
void UnorderedSet<OBJ, ALLOCATOR>::addElements( const OBJ *elem, size_t size )
{
	for( size_t i=0; i<size; i++ )
	{
		addElement( *elem++ );
	}
}

template <typename OBJ, typename Comparator, typename ALLOCATOR>
void Set<OBJ, Comparator, ALLOCATOR>::addElements( const OBJ *elem, size_t size )
{
	for( size_t i=0; i<size; i++ )
	{
		addElement( *elem++ );
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template <class OBJ, class Comparator, class ALLOCATOR> 
inline Set<OBJ, Comparator, ALLOCATOR> intersect( 
	const Set<OBJ, Comparator, ALLOCATOR> &first, 
	const Set<OBJ, Comparator, ALLOCATOR> &second 
)
{
	Set<OBJ, Comparator, ALLOCATOR>	result;

	intersectSorted( first, second, std::back_inserter( result ) );

	return result;
}

template <class OBJ, class Comparator, class ALLOCATOR> 
inline Set<OBJ, Comparator, ALLOCATOR> substract(
	const Set<OBJ, Comparator, ALLOCATOR> &first, 
	const Set<OBJ, Comparator, ALLOCATOR> &second 
)
{
	return substractSorted( first, second );
}

template <typename OBJ, typename Comparator, typename ALLOCATOR>
inline void moveAssign( 
	Set<OBJ, Comparator, ALLOCATOR> &target, 
	Set<OBJ, Comparator, ALLOCATOR> &source 
)
{
	target.moveFrom( source );
}

template <typename OBJ, typename ALLOCATOR>
inline void moveAssign(
	UnorderedSet<OBJ, ALLOCATOR> &target,
	UnorderedSet<OBJ, ALLOCATOR> &source 
)
{
	target.moveFrom( source );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_SET_H
