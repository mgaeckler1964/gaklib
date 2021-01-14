/*
		Project:		GAKLIB
		Module:			ContainerTest.h
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/map.h>
#include <gak/sortedArray.h>
#include <gak/set.h>
#include <gak/list.h>

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

#	pragma warn -inl
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

class btreeItem
{
	STRING	text;

	public:
	typedef bool	SortType;

	btreeItem( const STRING &newText=NULL_STRING ) : text( newText ) {}
	int compare( const btreeItem &oper, SortType = false) const
	{
		return text.compare( oper.text );
	}
	const STRING &getText() const
	{
		return text;
	}
};

std::ostream & operator << ( std::ostream  &out, const btreeItem &item )
{
	out << item.getText();
	return out;
}

class mapItem
{
	public:
	typedef int key_type;

	private:
	int		key;
	STRING	value;

	public:
	mapItem()
	: key( randomNumber( 32000 ) ), value( formatNumber( randomNumber( 32000 ) ) )
	{
	}
	mapItem( int nKey, const STRING &nString )
	: key( nKey ), value( nString )
	{
	}
	int getKey( void ) const
	{
		return key;
	}
	void setKey( int nKey )
	{
		key = nKey;
	}
	const STRING &getText() const
	{
		return value;
	}
	int compare( const mapItem &oper ) const
	{
		int result = gak::compare( key, oper.key );
		if( !result )
		{
			result = value.compare( oper.value );
		}
		return result;
	}
};

std::ostream & operator << ( std::ostream  &out, const mapItem &item )
{
	out << item.getKey() << '.' << item.getText();
	return out;
}

class ContainerTest : public gak::UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "ContainerTest";
	}
	virtual void PerformTest( void )
	{
		Array<STRING>									arrayString;
		Btree<STRING>									sBtreeString;
		Btree<btreeItem, DynamicComparator<btreeItem> >	btreeString;
		SortedArray<STRING>								sortedString;
		gak::Set<STRING>								setString;
		UnorderedSet<STRING>							setXString;
		List<STRING>									listString;

		{
			TestScope scope("Array<STRING>");
			testContainer( arrayString, makeString, true );
		}
		{
			TestScope scope("Btree<STRING>");
			testContainer( sBtreeString,makeString, true  );
		}
		{
			TestScope scope("Btree<btreeItem, DynamicComparator<btreeItem> >");
			testContainer( btreeString, makeString, true );
		}
		{
			TestScope scope("SortedArray<STRING>");
			testContainer( sortedString, makeString, false );
		}
		{
			TestScope scope("Set<STRING>");
			testContainer( setString, makeString, false );
		}
		{
			TestScope scope("UnorderedSet<STRING>");
			testContainer( setXString, makeString, false );
		}
		{
			TestScope scope("List<STRING>");
			testContainer( listString, makeString, true );
		}

		gak::Set<STRING>		copySetString = setString;
		setString.addElements( copySetString );
		UT_ASSERT_EQUAL( copySetString.size(), setString.size() );
		UT_ASSERT_EQUAL( copySetString, setString );

		SortedArray<STRING>		copySortedString = sortedString;
		sortedString.addElements( copySortedString );
		for( size_t i=1; i< sortedString.size(); ++i )
		{
			UT_ASSERT_LESSEQ( sortedString[i-1], sortedString[i] );
		}
		UnorderedPairMap<int,STRING>	unorderedPairMap;
		PairMap<int,STRING>				orderedPairMap;
		TreeMap<int,STRING>				treeMap;

		testPairMap( treeMap, makeInt, makeString );
		testPairMap( unorderedPairMap, makeInt, makeString );
		testPairMap( orderedPairMap, makeInt, makeString );

		UnorderedMap<mapItem>			theUnMap;
		Map<mapItem>					theMap;

		testMap( theUnMap, makeMapItem );
		testMap( theMap, makeMapItem );

		FixedHeapArray<int> myHeapArray( 123 );
		UT_ASSERT_EQUAL( myHeapArray.size(), size_t(123) );

		FixedArray<int, 666> myLocalArray;
		UT_ASSERT_EQUAL( myLocalArray.size(), size_t(666) );

		{
			Array< gak::Set<int> >	setArray( 2 );
			setArray.insertElement(0);
			setArray.insertElement(0);
			setArray.removeElementAt(0);
			setArray.removeElementsAt(0, 2);

			gak::Set<int>	*tmp = setArray + 1;
			doLogValue( tmp->size() );

			UnorderedMap<KeyValuePair<CI_STRING, int> >	theMap;
			theMap["hello"] = KeyValuePair<CI_STRING, int>("", 1);
			theMap["world"] = KeyValuePair<CI_STRING, int>("", 2);

			int i=1;
			int what = theMap.getElementAt(i).getValue();
			doLogValue( what );


			Array< Queue<int> >		theSuperArray( 2 );
			theSuperArray.createElement();
			theSuperArray.createElements( 5 );
			for( size_t i=0; i< theSuperArray.size(); ++i )
			{
				Queue<int>	&queue = theSuperArray[i];
				for( int j=0; j<5; ++j )
				{
					queue.push( i*j );
				}
			}
		}
		{
			TestScope scope("Array<int>");
			testSetOperations< Array<int> >();
		}
		{
			TestScope scope("SortedArray<int>");
			testSetOperations< SortedArray<int> >();
		}
		{
			TestScope scope("Set<int>");
			testSetOperations< gak::Set<int> >();
		}
		{
			TestScope scope("Btree<int>");
			testSetOperations< Btree<int> >();
		}
		{
			TestScope scope("Map< KeyValuePair<int,int> >");
			testSetOperations< Map< KeyValuePair<int,int> > >();
		}
		{
			TestScope scope("UnorderedMap< KeyValuePair<int,int> >");
			testSetOperations< UnorderedMap< KeyValuePair<int,int> > >();
		}
		{
			TestScope scope("PairMap< int,int >");
			testSetOperations< PairMap< int,int > >();
		}
		{
			TestScope scope("TreeMap<int, int>");
			testSetOperations< TreeMap<int, int> >();
		}
		{
			testIntSetOperations< Array<int> >();
			testIntSetOperations< SortedArray<int> >();
			testIntSetOperations< gak::Set<int> >();
			testIntSetOperations< Btree<int> >();

			testFindElement< Array<int> >();
			testFindElement< SortedArray<int> >();
			testFindElement< gak::Set<int> >();
		}
		{
			TestScope scope("SortedArray<int>");
			testGetRange< SortedArray<int> >();
		}
		{
			TestScope scope("Btree<int>");
			testGetRange< Btree<int> >();
		}
	}

	template <typename ContainerT>
	void testGetRange()
	{
		ContainerT	container;

		container.addElement( 1 );
		container.addElement( 3 );
		container.addElement( 5 );
		container.addElement( 5 );
		container.addElement( 5 );
		container.addElement( 7 );
		container.addElement( 7 );
		container.addElement( 9 );

		ConstIterable<typename ContainerT::const_iterator>	range = container.getRange( 5, 8 );
		typename ContainerT::const_iterator	it = range.cbegin();
		UT_ASSERT_EQUAL( 5, *it ); ++it;
		UT_ASSERT_EQUAL( 5, *it ); ++it;
		UT_ASSERT_EQUAL( 5, *it ); ++it;
		UT_ASSERT_EQUAL( 7, *it ); ++it;
		UT_ASSERT_EQUAL( 7, *it ); ++it;
		UT_ASSERT_EQUAL( it, range.cend() );;

		range = container.getRange( 7, 11 );
		it = range.cbegin();
		UT_ASSERT_EQUAL( 7, *it ); ++it;
		UT_ASSERT_EQUAL( 7, *it ); ++it;
		UT_ASSERT_EQUAL( 9, *it ); ++it;
		UT_ASSERT_EQUAL( it, range.cend() );;

		range = container.getRange( 0, 5 );
		it = range.cbegin();
		UT_ASSERT_EQUAL( 1, *it ); ++it;
		UT_ASSERT_EQUAL( 3, *it ); ++it;
		UT_ASSERT_EQUAL( it, range.cend() );;

		range = container.getRange( -9, -1 );
		UT_ASSERT_EQUAL( range.cbegin(), range.cend() );;

		range = container.getRange( 10, 11 );
		UT_ASSERT_EQUAL( range.cbegin(), range.cend() );;
	}
	template <typename ContainerT>
	void testSetOperations()
	{
		ContainerT	array1, array2;
#ifdef __BORLANDC__
		array1.addElement( ContainerT::value_type() );
		array2.addElement( ContainerT::value_type() );
#else
		array1.addElement( typename ContainerT::value_type() );
		array2.addElement( typename ContainerT::value_type() );
#endif
		ContainerT	result1 = unionSet( array1, array2 );
		ContainerT	result2 = intersect( array1, array2 );
		ContainerT	result3 = substract( array1, array2 );

		UT_ASSERT_EQUAL( std::size_t(1), result1.size() );
		UT_ASSERT_EQUAL( std::size_t(1), result2.size() );
		UT_ASSERT_EQUAL( std::size_t(0), result3.size() );
		UT_ASSERT_EQUAL( result1, result2 );
	}

	template <typename ContainerT>
	void testIntSetOperations()
	{
		ContainerT	array1, array2;

		array1.addElement( 1 );
		array1.addElement( 2 );
		array1.addElement( 3 );

		array2.addElement( 4 );
		array2.addElement( 3 );
		array2.addElement( 2 );

		ContainerT	result1 = unionSet( array1, array2 );
		ContainerT	result2 = intersect( array1, array2 );
		ContainerT	result3 = substract( array1, array2 );

		UT_ASSERT_EQUAL( std::size_t(4), result1.size() );
		UT_ASSERT_EQUAL( std::size_t(2), result2.size() );
		UT_ASSERT_EQUAL( std::size_t(1), result3.size() );
	}

	template <typename ContainerT>
	void testFindElement()
	{
		ContainerT	container;
#ifdef __BORLANDC__
		container.addElement( ContainerT::value_type() );
		UT_ASSERT_EQUAL( std::size_t(0), container.findElement( ContainerT::value_type() ) );
#else
		container.addElement( typename ContainerT::value_type() );
		UT_ASSERT_EQUAL( std::size_t(0), container.findElement( typename ContainerT::value_type() ) );
#endif
	}

	static STRING makeString( int idx )
	{
		return formatNumber( makeInt( idx ) );
	}
	static int makeInt( int idx )
	{
		return (randomNumber( 1024*1024 ) << 8) | (idx & 0xFF);
	}
	static mapItem makeMapItem( int idx )
	{
		return mapItem( makeInt( idx ), makeString( idx ) );
	}

	template<class ContainerT, class FunctionT>
	void fillContainer( ContainerT &theContainer, FunctionT theProducer )
	{
		size_t	current = theContainer.size();
		for( size_t i=0; i<10; i++ )
		{
			theContainer += theProducer( int(i) );
			theContainer.push_back( theProducer( int(i) ) );
			theContainer.push_front( theProducer( int(i) ) );
		}
		UT_ASSERT_EQUAL( theContainer.size(), current + size_t(30) );
	}
	template <class ContainerT, class KeyGenT, class ValueGenT >
	void fillKeyValueMap( ContainerT &theContainer, KeyGenT keyGen, ValueGenT valueGen )
	{
		size_t	current = theContainer.size();
		for( size_t i=0; i<10; ++i )
		{
			theContainer.setValue( keyGen(int(i)), valueGen(int(i)) );
			theContainer[keyGen(int(i))] = valueGen(int(i));
		}
		UT_ASSERT_EQUAL( theContainer.size(), current + size_t(20) );
	}
	template <class ContainerT, class ValueGenT>
	void fillKeyObjectMap( ContainerT &theContainer, ValueGenT valueGen )
	{
		size_t	current = theContainer.size();
		for( size_t i=0; i<10; ++i )
		{
			typename ContainerT::value_type newVal = valueGen( int(i) );
			typename ContainerT::key_type key = newVal.getKey();
			theContainer[key] = newVal;
		}
		UT_ASSERT_EQUAL( theContainer.size(), current + size_t(10) );
	}

	template <class ContainerT>
	void scanWithIterator( ContainerT &theContainer )
	{
		Stack<typename ContainerT::value_type>	items;

		// walk through with iterator
		size_t i=0;
		for(
			typename ContainerT::iterator	it = theContainer.begin(), endIT = theContainer.end();
			it != endIT; 
			++it, ++i 
		)
		{
			items.push( *it );
		}
		UT_ASSERT_EQUAL( i, theContainer.size() );

		// walk through with reverse iterator
		i=0;
		for( 
			typename ContainerT::reverse_iterator	it = theContainer.rbegin(), endIT = theContainer.rend();
			it != endIT; 
			++it, ++i 
		)
		{
			UT_ASSERT_EQUAL( *it, items.pop() );
		}
		UT_ASSERT_EQUAL( i, theContainer.size() );
	}

	template <class ContainerT>
	void scanWithConstIterator( const ContainerT &theContainer )
	{
		Stack<typename ContainerT::value_type>	items;

		// walk through with iterator
		size_t i=0;
		for(
			typename ContainerT::const_iterator	it = theContainer.cbegin(), endIT = theContainer.cend();
			it != endIT; 
			++it, ++i 
		)
		{
			items.push( *it );
		}
		UT_ASSERT_EQUAL( i, theContainer.size() );

		// walk through with reverse iterator
		i=0;
		for( 
			typename ContainerT::const_reverse_iterator	it = theContainer.crbegin(), endIT = theContainer.crend();
			it != endIT; 
			++it, ++i 
		)
		{
			UT_ASSERT_EQUAL( *it, items.pop() );
		}
		UT_ASSERT_EQUAL( i, theContainer.size() );
	}

	template<class ContainerT>
	void testConstMapAccess( const ContainerT &theContainer )
	{
		Array<typename ContainerT::key_type>	keys = theContainer.getKeys();
		UT_ASSERT_EQUAL( theContainer.size(), keys.size() );

		for(
			typename Array<typename ContainerT::key_type>::iterator it = keys.begin(), endIT = keys.end();
			it != endIT;
			++it
		)
		{
			if( !theContainer.hasElement( *it ) )
				std::cout << "Data not found\n";

			UT_ASSERT_TRUE( theContainer.hasElement( *it ) );
			typename ContainerT::value_type	value = theContainer[*it];
			typename ContainerT::key_type	key = theContainer.findElement( value );
			UT_ASSERT_EQUAL( *it, key );
		}
	}
	template<class ContainerT>
	void testMapAccess( ContainerT &theContainer )
	{
		Array<typename ContainerT::key_type>	keys = theContainer.getKeys();
		UT_ASSERT_EQUAL( theContainer.size(), keys.size() );

		for(
			typename Array<typename ContainerT::key_type>::iterator it = keys.begin(), endIT = keys.end();
			it != endIT;
			++it
		)
		{
			UT_ASSERT_TRUE( theContainer.hasElement( *it ) );
			typename ContainerT::value_type	value = theContainer[*it];
			typename ContainerT::key_type	key = theContainer.findElement( value );
			UT_ASSERT_EQUAL( *it, key );

			theContainer.removeElementByKey( key );
		}
		UT_ASSERT_EQUAL( theContainer.size(), size_t(0) );
	}

	template<class ContainerT, class FunctionT>
	void testMap( ContainerT &theContainer, FunctionT theProducer )
	{
		fillContainer( theContainer, theProducer );
		fillKeyObjectMap( theContainer, theProducer );
		scanWithConstIterator( theContainer );

		testConstMapAccess( theContainer );
		testMapAccess( theContainer );
	}

	template <class ContainerT>
	void testConstPairMapAccess( const ContainerT &theContainer )
	{
		Array<typename ContainerT::key_type>	keys = theContainer.getKeys();
		UT_ASSERT_EQUAL( theContainer.size(), keys.size() );

		for(
			typename Array<typename ContainerT::key_type>::iterator it = keys.begin(), endIT = keys.end();
			it != endIT;
			++it
		)
		{
			UT_ASSERT_TRUE( theContainer.hasElement( *it ) );
			typename ContainerT::mapped_type	value = theContainer[*it];
			typename ContainerT::key_type		key = theContainer.findValue( value );
#ifndef NDEBUG
			if( *it != key )
			{
				std::cout << *it << ',' << key << ',' << value << ',' << theContainer[key] << '\n';
				theContainer[*it];
				theContainer.findValue( value );
			}
#endif
			UT_ASSERT_EQUAL( *it, key );
		}
	}

	template <class ContainerT>
	void testPairMapAccess( ContainerT &theContainer )
	{
		Array<typename ContainerT::key_type>	keys = theContainer.getKeys();
		UT_ASSERT_EQUAL( theContainer.size(), keys.size() );

		for(
			typename Array<typename ContainerT::key_type>::iterator it = keys.begin(), endIT = keys.end();
			it != endIT;
			++it
		)
		{
			UT_ASSERT_TRUE( theContainer.hasElement( *it ) );
			typename ContainerT::mapped_type	value = theContainer[*it];
			typename ContainerT::key_type		key = theContainer.findValue( value );
			UT_ASSERT_EQUAL( *it, key );

			theContainer.removeElementByKey( key );
		}
		UT_ASSERT_EQUAL( theContainer.size(), size_t(0) );
	}

	template <class ContainerT, class KeyGenT, class ValueGenT>
	void testPairMap( ContainerT &theContainer, KeyGenT keyGen, ValueGenT valueGen )
	{
		fillKeyValueMap( theContainer, keyGen, valueGen );

		scanWithConstIterator( theContainer );
		testConstPairMapAccess( theContainer );
		testPairMapAccess( theContainer );
	}

	template<class ContainerT, class FunctionT>
	void testContainer( ContainerT &theContainer, FunctionT theProducer, bool doClear )
	{
		fillContainer( theContainer, theProducer );

		scanWithIterator( theContainer );
		scanWithConstIterator( theContainer );

		// empty
		if( doClear )
		{
			while( theContainer.size() )
			{
				theContainer.erase( theContainer.begin() );
			}
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static ContainerTest myContainerTest;

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

#	pragma warn +inl
#endif

