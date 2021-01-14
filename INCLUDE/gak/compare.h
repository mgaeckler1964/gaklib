/*
		Project:		GAKLIB
		Module:			compare.h
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

#ifndef GAK_COMPARE_ARRAY_H
#define GAK_COMPARE_ARRAY_H


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/types.h>
#include <gak/ansiChar.h>

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

/**
	@brief compares two items according to a sort criteria

	the type of the first element must provide a member function compare 
	that does the real compare. If this is not possible, you must specialize
	this template

	@param [in] elem1 first item
	@param [in] elem2 second item
	@param [in] sortType the sort criteria
	@return the compare result
*/
template <class OBJ, class KEY, class SortType>
inline
int compare( const OBJ &elem1, const KEY &elem2, SortType sortType )
{
	return elem1.compare( elem2, sortType );
}

/**
	@brief compares two scalar items

	@param [in] elem1 first item
	@param [in] elem2 second item
	@return \li -1 if elem1 < elem2
			\li 0 if elem1 == elem2
			\li 1 if elem1 > elem2
*/
template <class OBJ>
int scalarCompare( OBJ elem1, OBJ elem2 )
{
	if( elem1 < elem2 )
		return -1;
	if( elem1 > elem2 )
		return 1;
	return 0;
} 

/**
	@brief compares two items

	the type of the first element must provide a member function compare 
	that does the real compare. If this is not possible, you must specialize
	this template

	@param [in] elem the item that must perform the compare
	@param [in] key the key value to be compared with elem
	@return the compare result
	@tparam OBJ a type that can compare an item with a key value
	@tparam KEY a key type that can be compared by OBJ
*/
template <typename OBJ, typename KEY>
inline int compare( const OBJ &elem, const KEY &key )
{
	return elem.compare( key );
}

/// @copydoc scalarCompare
template<>
inline int compare( const bool &elem1, const bool &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const char &elem1, const char &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const signed char &elem1, const signed char &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const unsigned char &elem1, const unsigned char &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const signed short &elem1, const signed short &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const unsigned short &elem1, const unsigned short &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const signed int &elem1, const signed int &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const unsigned int &elem1, const unsigned int &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const signed long &elem1, const signed long &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const unsigned long &elem1, const unsigned long &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const int64 &elem1, const int64 &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const uint64 &elem1, const uint64 &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const float &elem1, const float &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const double &elem1, const double &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @copydoc scalarCompare
template<>
inline int compare( const long double &elem1, const long double &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/// @brief helper type to compare pointers
typedef void *vPtr;

/// @copydoc scalarCompare
template<>
inline int compare( const vPtr &elem1, const vPtr &elem2 )
{
	return scalarCompare( elem1, elem2 );
}

/**
	@brief compares two C-style strings using ansiCompare
	@param [in] elem1 the first C-style string
	@param [in] elem2 the second C-style string
	@return the result of ansiCompare
*/
inline int compare( const char *elem1, const char *elem2 )
{
	return ansiCompare( elem1, elem2 );
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief a class that compares two items
	@tparam OBJ the type of the items to compare
	@see SortedArray, Btree
*/
template <class OBJ>
class FixedComparator
{
	public:
	/// @copydoc gak::compare( const OBJ &elem, const KEY &elem2 )
	/// @see gak::compare( const OBJ &elem, const KEY &elem2 )
	static int compare( const OBJ &elem, const OBJ &key )
	{
		return gak::compare( elem, key );
	}

	/// @copydoc gak::compare( const OBJ &elem, const KEY &elem2 )
	/// @see gak::compare( const OBJ &elem, const KEY &elem2 )
	int operator() ( const OBJ &elem, const OBJ &key ) const
	{
		return gak::compare( elem, key );
	}
};

/**
	@brief a class that compares two items acording to a sort criteria

	With this class you can sort a Container by a variable field.

	@tparam OBJ the type of the items to compare
	@tparam SortTypeT the type of the sort criteria
	@see SortedArray, Btree
*/
template <class OBJ, class SortTypeT= typename OBJ::SortType >
class DynamicComparator
{
	public:
	/// the type of the sort criteria
	typedef SortTypeT SortType;

	protected:
	/// the value of the sort criteria
	SortType	sortCriteria;

	public:
	/**
		@brief creates a new comparator
		@param [in] crit the sort criteria
	*/
	DynamicComparator( const SortType &crit = SortType() ) 
	: sortCriteria( crit ) 
	{
	}
	/**
		@brief assigns a sort criteria
		@param [in] crit the sort criteria
		@return the comparator
	*/
	const DynamicComparator &operator = ( const SortType &crit )
	{
		sortCriteria = crit;
		return *this;
	}

	/**
		@brief compares two items according to a sort criteria

		the sort criteria used is the default value of the sort type

		@param [in] elem1 first item
		@param [in] elem2 second item
		@return the compare result
		@see gak::compare( const OBJ &elem1, const KEY &elem2, SortType sortType )
	*/
	int compare( const OBJ &elem1, const OBJ &elem2 ) const
	{
		return gak::compare( elem1, elem2, sortCriteria );
	}
	/**
		@brief compares two items according to the sort criteria
		@param [in] elem1 first item
		@param [in] elem2 second item
		@return the compare result
		@see gak::compare( const OBJ &elem1, const KEY &elem2, SortType sortType )
	*/
	int operator() ( const OBJ &elem1, const OBJ &elem2 ) const
	{
		return gak::compare( elem1, elem2, sortCriteria );
	}
};

/**
	@brief a class that compares the keys of two items
	@tparam KEY the type of the item keys to compare
	@tparam OBJ the type of the items to compare must provide a member getKey
	@see Map, TreeMap
*/
template <class KEY, class OBJ>
class KeyComparator
{
	public:
	/**
		@brief compares the keys of two items
		@param [in] elem1 first item
		@param [in] elem2 second item
		@return the compare result
		@see gak::compare( const OBJ &elem, const KEY &elem2 )
	*/
	static int compare( const OBJ &elem1, const OBJ &elem2 )
	{
		return gak::compare( elem1.getKey(), elem2.getKey() );
	}
	/**
		@brief compares the keys of one items with another key
		@param [in] elem the item
		@param [in] key the other key
		@return the compare result
		@see gak::compare( const OBJ &elem, const KEY &elem2 )
	*/
	static int compareWithKey( const OBJ &elem, const KEY &key )
	{
		return gak::compare( elem.getKey(), key );
	}
	/// @copydoc compare
	int operator() ( const OBJ &elem1, const OBJ &elem2 ) const
	{
		return gak::compare( elem1.getKey(), elem2.getKey() );
	}
};

/**
	@brief class that stores the addess of a compare function
	@tparam OBJ the type of the items to compare
	@tparam SortTypeT the type of the sort criteria
	@see ArrayBase, SortedArray
*/
template <class OBJ, class SortTypeT=typename OBJ::SortType >
class CompareFunction
{
	public:
	/// the type of the sort criteria
	typedef SortTypeT SortType;
	/// the type of the function pointer
	typedef int (*CompareFunctionPtr)( const OBJ &elem1, const OBJ &elem2, SortType sortCriteria);

	protected:
	/// the sort criteria
	SortType			sortCriteria;
	/// the address of the compare function
	CompareFunctionPtr	compareFunction;

	public:
	/**
		@brief creates a new comparator
		@param [in] func the function pointer
		@param [in] crit the sort criteria
	*/
	CompareFunction( CompareFunctionPtr func, SortType crit )
	: compareFunction( func ), sortCriteria( crit )
	{
	}
	/**
		@brief assigns a new sort criteria
		@param [in] crit the sort criteria
	*/
	const CompareFunction &operator = ( const SortType &crit )
	{
		sortCriteria = crit;
		return *this;
	}

	/**
		@brief compares two items according to a sort criteria

		the sort criteria used is the default value of the sort type

		@param [in] elem1 first item
		@param [in] elem2 second item
		@return the compare result
	*/
	int compare( const OBJ &elem1, const OBJ &elem2 ) const
	{
		return compareFunction( elem1, elem2, SortType() );
	}
	/**
		@brief compares two items according to the sort criteria
		@param [in] elem1 first item
		@param [in] elem2 second item
		@return the compare result
		@see gak::compare( const OBJ &elem1, const KEY &elem2, SortType sortType )
	*/
	int operator() ( const OBJ &elem1, const OBJ &elem2 ) const
	{
		return compareFunction( elem1, elem2, sortCriteria );
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

/**
	@brief compares two containers

	the two containers are compared like c standard function strcmp
	both containers must store´compatible item types

	@param [in] left the first container
	@param [in] right the second container
	@return the compare result
	@see gak::compare( const OBJ &elem1, const KEY &elem2 )
*/
template <typename ContainerT>
int compareContainer( const ContainerT &left, const ContainerT &right )
{
	int compareResult = 0;
	typename ContainerT::const_iterator		leftIT = left.cbegin(), leftEndIT = left.cend(), 
											rightIT = right.cbegin(), rightEndIT = right.cend();
	for(
		;
		!compareResult && leftIT != leftEndIT && rightIT != rightEndIT;
		++leftIT, ++rightIT
	)
	{
		compareResult = compare( *leftIT, *rightIT );
	}

	if( !compareResult )
	{
		if( leftIT == leftEndIT && rightIT != rightEndIT )
		{
			compareResult = -1;
		}
		else if( leftIT != leftEndIT && rightIT == rightEndIT )
		{
			compareResult = +1;
		}
	}
	return compareResult;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_COMPARE_ARRAY_H
