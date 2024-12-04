/*
		Project:		GAKLIB
		Module:			sortedArray.h
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

#ifndef GAK_SORTED_ARRAY_H
#define GAK_SORTED_ARRAY_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstdlib>
#include <cstddef>
#include <cassert>
#include <iterator>

//#include <cstdio>

#include <gak/arrayBase.h>
#include <gak/ci_string.h>
#include <gak/compare.h>

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
	@brief template class for a sorted array

	a sorted array is allways sorted. If you're adding elements with 
	insertElementAt or createElement or if you're changing elements, the 
	sort state is not guaranteed.

	@tparam OBJ the type of the items to store
	@tparam Comparator the class that can compare two items
	@tparam ALLOCATOR the class that allocates the memory
	@see Allocator, PODallocator, FixedComparator, DynamicComparator, CompareFunction
*/

template <
	typename OBJ, 
	typename Comparator=FixedComparator<OBJ>, 
	typename ALLOCATOR=Allocator<OBJ> 
>
class SortedArray : public ArrayBase<OBJ, ALLOCATOR>
{
	typedef ArrayBase<OBJ, ALLOCATOR>	Super;

	private:
	Comparator		m_comparator;

	public:
	/**
		@brief creates a new empty sorted array
		@param [in] comparator the comparator
	*/
	SortedArray( const Comparator &comparator = Comparator() )
	: m_comparator( comparator )
	{
	}
	/**
		@brief creates a new empty sorted array
		@param [in] func a compare function
		@param [in] crit the sort criteria
		@see CompareFunction
	*/
	template<class CompareFunctionPtr, class SortType>
	SortedArray( CompareFunctionPtr func, SortType crit )
	: m_comparator( CompareFunction<OBJ, SortType>( func, crit )  )
	{
	}

	/*
	-------------------------------------------------------------------------
		Memory
	-------------------------------------------------------------------------
	*/
	/// @name Memory
	///@{

	/**
		@copydoc ArrayBase::moveFrom
	*/
	void moveFrom( SortedArray<OBJ, Comparator, ALLOCATOR> &source )
	{
		Super::moveFrom( source );
		this->m_comparator = source.m_comparator;
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
		@copydoc ArrayBase::addElement

		the array remains sorted
	*/
	const OBJ &addElement( const OBJ &item );
	/**
		@copydoc ArrayBase::operator+=

		the array remains sorted
	*/
	SortedArray<OBJ, Comparator> &operator += ( const OBJ &item )
	{
		addElement( item );

		return *this;
	}
	/**
		@copydoc ArrayBase::addElements( const OBJ *elem, size_t size )

		the array remains sorted
	*/
	void addElements( const OBJ *elem, size_t size );

	/**
		@copydoc ArrayBase::addElements( const ArrayT &source )

		the array remains sorted
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
	public:
	/// @copydoc ArrayBase::getDataBuffer
	const OBJ *getDataBuffer( void ) const
	{
		return Super::getDataBuffer();
	}
	/// @copydoc ArrayBase::operator+
	const OBJ *operator + ( size_t pos ) const
	{
		return Super::operator + ( pos );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Modify Data
	-------------------------------------------------------------------------
	*/
	/// @name Modify Data
	///@{
	public:
	/// @copydoc ArrayBase::removeElementVal
	void removeElement( const OBJ &elem )
	{
		size_t	pos = findElement( elem );
		if( pos != this->no_index )
		{
			this->removeElementAt( pos );
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
	public:
	/**
		@brief applies a new comparator and resorts the array
		@param [in] comparator a comparator
	*/
	void resort( const Comparator &comparator )
	{
		m_comparator = comparator;
		resort();
	}
	/**
		@brief applies a new sort criteria and resorts the array
		@param [in] sortCrit sort criteria
	*/
	template <class SortTypeT>
	void resort( const SortTypeT &sortCrit )
	{
		m_comparator = sortCrit;
		resort();
	}
	/// sorts this array again
	void resort( void )
	{
		this->sort( m_comparator );
	}
	/// returns the index of the first item not sorted. 0 if array is sorted
	size_t needSort( void ) const
	{
		return this->ArrayBase<OBJ>::needSort( m_comparator );
	}
	/// return the current comparator
	const Comparator &getComparator( void ) const
	{
		return m_comparator;
	}
	///@}


	/*
	-------------------------------------------------------------------------
		Searching
	-------------------------------------------------------------------------
	*/
	typedef typename Super::const_iterator		const_iterator;
	/// @name Searching
	///@{
	private:
	size_t findPos( const OBJ &elem, int *oCompareResult ) const;
	size_t findMin( const OBJ &elem ) const;

	public:
	size_t findInsertPos( const OBJ &elem ) const;

	/// copydoc ArrayBase::findElement
	size_t findElement( const OBJ &searchFor ) const;
	/// copydoc ArrayBase::hasElement
	bool	hasElement( const OBJ &searchFor ) const
	{
		return findElement( searchFor ) != this->no_index;
	}
	ConstIterable<const_iterator> getRange( const OBJ &min, const OBJ &max ) const
	{
		doEnterFunction("SortedArray::getRange");
		assert( min <= max );
		return ConstIterable<const_iterator>(
			const_iterator( getDataBuffer()+findMin( min ) ),
			const_iterator( getDataBuffer()+findMin( max ) )
		);
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
	typedef typename Super::iterator			iterator;
	///@}

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
	iterator erase( const iterator &it )
	{
		size_t	index = it - getDataBuffer();
		this->removeElementAt( index );
		return Super::getDataBuffer() + index;
	}
	///@}
};

template <
	typename OBJ, 
	typename Comparator, 
	typename ALLOCATOR
>
inline void moveAssign( 
	SortedArray<OBJ, Comparator, ALLOCATOR> &target, SortedArray<OBJ, Comparator, ALLOCATOR> &source 
)
{
	target.moveFrom( source );
}

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

template <class OBJ, class Comparator, class ALLOCATOR> 
inline
const OBJ &SortedArray<OBJ, Comparator, ALLOCATOR>::addElement( const OBJ &elem )
{
	size_t pos = findInsertPos( elem );

	insertElement( elem, pos );

	return this->getDataBuffer()[pos];
}

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

template <class OBJ, class Comparator, class ALLOCATOR> 
size_t SortedArray<OBJ, Comparator, ALLOCATOR>::findElement( const OBJ &elem ) const
{
	int		compareResult;
	size_t	min, max, pos;

	min = 0;
	max = this->size();

	while( min < max )
	{
		pos = math::medium(min,max);
		compareResult = m_comparator( this->getDataBuffer()[pos], elem );
		if( !compareResult )
		{
/***/		return pos;
		}
		if( compareResult < 0 )
		{
			min = pos+1;
		}
		else
		{
			max = pos;
		}
	}
	return this->no_index;
}

template <class OBJ, class Comparator, class ALLOCATOR> 
void SortedArray<OBJ, Comparator, ALLOCATOR>::addElements( const OBJ *elem, size_t size )
{
	for( size_t i=0; i<size; i++ )
	{
		addElement( *elem++ );
	}
}

template <class OBJ, class Comparator, class ALLOCATOR> 
size_t SortedArray<OBJ, Comparator, ALLOCATOR>::findPos( const OBJ &elem, int *oCompareResult ) const
{
	std::size_t	min = 0,
				max = this->size();
	int			compareResult = 0;

	if( max >= 2 )
	{
		compareResult = m_comparator( this->getDataBuffer()[0], elem );
		if( compareResult >= 0 )	// elem less than first
		{
			max = 0;
		}
		else
		{
			compareResult = m_comparator( this->getDataBuffer()[max-1], elem );
			if( compareResult <= 0 )	// elem greater than last
			{
				min = max;
			}
		}
	}

	std::size_t	pos = math::medium(min,max);
	while( min < max )
	{
		compareResult = m_comparator( this->getDataBuffer()[pos], elem );
		if( !compareResult )
		{
/*v*/		break;
		}
		if( compareResult < 0 )
		{
			min = pos+1;
		}
		else
		{
			max = pos;
		}

		pos = math::medium(min,max);
	}

	*oCompareResult = compareResult;

	return pos;
}

template <class OBJ, class Comparator, class ALLOCATOR> 
size_t SortedArray<OBJ, Comparator, ALLOCATOR>::findInsertPos( const OBJ &elem ) const
{
	int		compareResult;
	size_t	pos = findPos( elem, &compareResult );

	while( !compareResult && pos < this->size() )
	{
		pos++;
		if( pos < this->size() )
		{
			compareResult = m_comparator( this->getDataBuffer()[pos], elem );
		}
	}

	return pos;
}

template <class OBJ, class Comparator, class ALLOCATOR> 
size_t SortedArray<OBJ, Comparator, ALLOCATOR>::findMin( const OBJ &elem ) const
{
	int compareResult;
	size_t pos = findPos( elem, &compareResult );
	if( !compareResult && pos > 0 && this->size() )
	{
		do
		{
			--pos;
			if( pos == this->no_index )
			{
				break;
			}
			compareResult = m_comparator( this->getDataBuffer()[pos], elem );
		} while( !compareResult );
		pos++;
	}
	return pos;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template <class OBJ, class Comparator, class ALLOCATOR> 
inline SortedArray<OBJ, Comparator, ALLOCATOR> intersect( 
	const SortedArray<OBJ, Comparator, ALLOCATOR> &first, 
	const SortedArray<OBJ, Comparator, ALLOCATOR> &second 
)
{
	SortedArray<OBJ, Comparator, ALLOCATOR>	result;

	intersectSorted( first, second, std::back_inserter( result ) );

	return result;
}

template <class OBJ, class Comparator, class ALLOCATOR> 
inline SortedArray<OBJ, Comparator, ALLOCATOR> substract( 
	const SortedArray<OBJ, Comparator, ALLOCATOR> &first, 
	const SortedArray<OBJ, Comparator, ALLOCATOR> &second 
)
{
	return substractSorted( first, second );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

#endif
