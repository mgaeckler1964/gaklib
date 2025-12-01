/*
		Project:		GAKLIB
		Module:			BtreeTest.h
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>
#include <iostream>
#include <gak/unitTest.h>

#include <gak/directory.h>
#include <gak/map.h>

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

struct NodeTemplate
{
	size_t		m_count;
	int			m_parent, m_prev, m_next;
	int			m_data;
};

NodeTemplate	template1[] =
{
//  { c, pa, pr, ne, da },
	{ 5, -1, -1,  1, 1 },
	{ 4,  0, -1,  2, 2 },
	{ 3,  1, -1,  3, 3 },
	{ 2,  2, -1,  4, 4 },
	{ 1,  3, -1, -1, 5 },
};

NodeTemplate	template2[] =
{
//  { c, pa, pr, ne, da },
	{ 10, -1,  6,  1, 100 },			// 0
	{  8,  0, -1,  2, 200 },			// 1
	{  7,  1,  5,  3, 330 },			// 2
	{  5,  2, -1,  4, 400 },			// 3
	{  4,  3, -1,  7, 500 },			// 4
	{  1,  2, -1, -1, 250 },			// 5
	{  1,  0, -1, -1,  50 },			// 6
	{  3,  4, -1,  8, 600 },			// 7
	{  2,  7, -1,  9, 700 },			// 8
	{  1,  8, -1, -1, 710 },			// 9
};

NodeTemplate	simpleTemp[] =
{
//  { c, pa, pr, ne, da },
	{ 5, -1,  1,  2, 1  },
	{ 1,  0, -1, -1, 0  },
	{ 3,  0,  4,  3, 3  },
	{ 1,  2, -1, -1, 4  },
	{ 1,  2, -1, -1, 2  },
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class DeleteCounter
{
	int		m_value;
	size_t	*m_counter;

	public:
	DeleteCounter(int value, size_t *counter) : m_value(value), m_counter(counter) {}
	~DeleteCounter()
	{
		(*m_counter)++;
	}
	int compare( const DeleteCounter &other ) const
	{
		return gak::compare( m_value, other.m_value );
	}
};
class BtreeTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "BtreeTest";
	}

	typedef Btree<int>::Node Node;
	Node *prepareNodes(NodeTemplate *tmpl, size_t numTempl, Array<Node*> &testNodes )
	{
		for( size_t i=0; i<numTempl; ++i )
		{
			Node *newNode = new Node(int(tmpl[i].m_data));
			testNodes.addElement( newNode );
		}
		Node *root=nullptr;
		for( size_t i=0; i<numTempl; ++i )
		{
			Node *testNode = testNodes[i];
			int val = tmpl[i].m_parent;
			testNode->m_parent = (val>=0) ? testNodes[val] : nullptr;
			if( val < 0 )
				root = testNode;

			val = tmpl[i].m_next;
			testNode->m_next = (val>= 0) ? testNodes[val] : nullptr;

			val = tmpl[i].m_prev;
			testNode->m_prev = (val>= 0) ? testNodes[val] : nullptr;

			testNode->m_count = tmpl[i].m_count;
		}

		return root;
	}

	template <typename ContainerT>
	void rebalanceTest(ContainerT &container, bool expectBadInput)
	{
		size_t	depth1, depth2;
		UT_ASSERT_TRUE( container.test(&depth1, false) );

		if( expectBadInput )
		{
			UT_ASSERT_NOT_EQUAL(container.isBalanced(), btBalanced );
		}
		container.rebalance();
		UT_ASSERT_EQUAL(container.isBalanced(), btBalanced );
		UT_ASSERT_TRUE( container.test(&depth2, true) );

		if( expectBadInput )
		{
			UT_ASSERT_GREATER(depth1, depth2 );
		}
		else
		{
			UT_ASSERT_GREATEREQ(depth1, depth2 );
		}
	}

	void simpleTest()
	{
		doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::simpleTest" );
		TestScope scope( "simpleTest" );

		Btree<int> container;
		Array<Node*> testNodes;

		Node *root = prepareNodes( template2, arraySize(template2), testNodes );
		container.setRoot(root);
		rebalanceTest(container, true);

//		std::auto_ptr<Node> root(oldRoot);
//		root->testPointer(true);
//		root.reset( root.release()->rebalance(0) );
//		assert( root->m_parent == nullptr );
//		root->testPointer(true);
//		oldRoot->testPointer(true);
	}

	void deleteTest()
	{
		size_t counter=0;

		{
			Btree<DeleteCounter>	btree;

			UT_ASSERT_EQUAL( counter, 0 );

			btree.addElement( DeleteCounter( 1, &counter ) );
			btree.addElement( DeleteCounter( 2, &counter ) );
			btree.addElement( DeleteCounter( 3, &counter ) );
			btree.addElement( DeleteCounter( 10, &counter ) );
			btree.addElement( DeleteCounter( 7, &counter ) );
			btree.addElement( DeleteCounter( 6, &counter ) );
			btree.addElement( DeleteCounter( 8, &counter ) );
			btree.addElement( DeleteCounter( 9, &counter ) );
			btree.addElement( DeleteCounter( 5, &counter ) );
			btree.addElement( DeleteCounter( 4, &counter ) );

			UT_ASSERT_EQUAL( counter, 10 );	// temporary items also count
		}
		UT_ASSERT_EQUAL( counter, 20 );
	}

	template <typename ContainerT>
	void loopTest(ContainerT &container)
	{
		doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::loopTest" );
		{
			typename ContainerT::iterator it = container.begin();
			typename ContainerT::iterator end = container.end();
			int	last = *it;
			++it;

			while( it != end )
			{
				doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while1" );
				UT_ASSERT_LESSEQ( last, *it );
				last = *it;
				++it;
			} 
		}
		{
			typename ContainerT::const_iterator it = container.cbegin();
			typename ContainerT::const_iterator end = container.cend();
			int	last = *it;
			++it;

			while( it != end )
			{
				doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while2" );
				UT_ASSERT_LESSEQ( last, *it );
				last = *it;
				++it;
			} 
		}
		{
			typename ContainerT::reverse_iterator it = container.rbegin();
			typename ContainerT::reverse_iterator end = container.rend();
			int last = *it;
			++it;

			while( it != end )
			{
				doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while3" );
				UT_ASSERT_GREATEREQ( last, *it );
				last = *it;
				++it;
			} 
		}
		{
			typename ContainerT::const_reverse_iterator it = container.crbegin();
			typename ContainerT::const_reverse_iterator end = container.crend();
			int last = *it;
			++it;

			while( it != end )
			{
				doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while4" );
				UT_ASSERT_GREATEREQ( last, *it );
				last = *it;
				++it;
			} 
		}
	}

	template <int FACTOR, int OFFSET>
	void ContainerTest(const size_t numItems, const bool orderedAdd, const bool doLoopTest)
	{
		doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::ContainerTest" );
		typedef Btree<int,FixedComparator<int>, FACTOR, OFFSET>	MyBtree;
		MyBtree container;

		size_t depth;

		for( size_t i=0; i<numItems; i++ )
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::add" );
			if( orderedAdd )
			{
				container.addElement( int(i) );
			}
			else
			{
				container += randomNumber( std::numeric_limits<int>::max() );
			}
			size_t depth;
			container.test(&depth, true);
		}
		UT_ASSERT_TRUE( container.test(&depth) );

		if( doLoopTest )
		{
			loopTest(container);
		}

		rebalanceTest(container, !doLoopTest);

		if( doLoopTest )
		{
			while( container.size() )
			{
				doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while4" );
				typename MyBtree::const_reverse_iterator it = container.crbegin();
				typename MyBtree::const_reverse_iterator end = container.crend();
				size_t	fwdCount = randomNumber( int(container.size()) );

				for( size_t idx = 0; idx < fwdCount && it != end; ++it, ++idx ) 
					;
				if( it != end )
					container.removeElement( *it );
			}
		}
	}

	void CopyBtreeTest()
	{
#ifdef NDEBUG
		const size_t numItems=10240;
#else
		const size_t numItems=1024;
#endif
		const size_t numTries=16;

		Btree<STRING>	myTree;

		for( size_t i=0; i<numItems; ++i )
		{
			for( size_t j=0; j<numTries; ++j )
			{
				STRING newElement = STRING("TMP") + formatNumber( randomNumber(numItems * numItems), 10) + "GAK";
				if( !myTree.findElement(newElement) )
				{
					myTree.addElement( newElement );
					break;
				}
			}
		}
		Btree<STRING>	myTreeCopy = myTree;
		UT_ASSERT_EQUAL( myTree.size(),myTreeCopy.size() );

		for( 
			Btree<STRING>::const_iterator it1 = myTree.cbegin(), it2 = myTreeCopy.cbegin(), endIT = myTree.cend();
			it1 != endIT;
			++it1, ++it2
		)
		{
			UT_ASSERT_EQUAL(*it1, *it2);
		}

		Btree<STRING>	myTreeCopy2 = myTree;

		myTree.clear();
		UT_ASSERT_EQUAL( myTree.size(), 0 );
		UT_ASSERT_EQUAL( myTreeCopy.size(), myTreeCopy2.size() );

		for( 
			Btree<STRING>::const_iterator it1 = myTreeCopy.cbegin(), it2 = myTreeCopy2.cbegin(), endIT = myTreeCopy.cend();
			it1 != endIT;
			++it1, ++it2
		)
		{
			UT_ASSERT_EQUAL(*it1, *it2);
		}


	}

	virtual void PerformTest()
	{
		doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::PerformTest" );

		TestScope scope( "PerformTest" );
		simpleTest();
		deleteTest();
		CopyBtreeTest();
#if defined( NDEBUG ) && !defined(__GNUC__)
		const size_t numItems = 32000;
#else
		const size_t numItems = 4000;
#endif
		const bool orderedAdd = false;

		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::PerformTest::<10,5>" );
			TestScope scope( "<10,5>" );
			ContainerTest<10,5>(numItems,orderedAdd, true);
		}
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::PerformTest::<2,5>" );
			TestScope scope( "<2,5>" );
			ContainerTest<2,5>(numItems,orderedAdd,true);
		}
		{
//			TestScope scope( "<10,5> no loop" );
//			ContainerTest<2,5>(64000,true,false);
		}

		typedef Btree<DirectoryEntry, DynamicComparator<DirectoryEntry> >	BtreeList;
		BtreeList		dirBtree;
		DirectoryList	dirList;

		dirList.findFiles( "*.*" );

		for( 
			DirectoryList::const_iterator it = dirList.cbegin(), endIT = dirList.cend();
			it != endIT;
			++it
		)
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::for5" );
			const DirectoryEntry	&listEntry = *it;
			dirBtree.addElement( listEntry );
			size_t depth;
			UT_ASSERT_TRUE( dirBtree.test(&depth) );
		}

		UT_ASSERT_EQUAL( dirBtree.size(), dirList.size() );
		size_t i=0;
		for( 
			BtreeList::iterator it = dirBtree.begin(), endit = dirBtree.end();
			it != endit;
			++it, ++i
		)
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::for6" );
			UT_ASSERT_TRUE( dirBtree.hasElement( *it ) );
		}
		UT_ASSERT_EQUAL( dirBtree.size(), i );

		dirBtree.resort( DirectoryEntry::SORT_SIZE );
		uint64	lastSize = 0;
		i=0;
		for( 
			BtreeList::iterator it = dirBtree.begin(), endit = dirBtree.end();
			it != endit;
			++it, ++i
		)
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::for7" );
			UT_ASSERT_LESSEQ( lastSize, it->fileSize );
			lastSize = it->fileSize;
		}
		UT_ASSERT_EQUAL( dirBtree.size(), i );

		while( dirBtree.size() )
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while8" );
			BtreeList::const_reverse_iterator it = dirBtree.crbegin();
			BtreeList::const_reverse_iterator end = dirBtree.crend();
			size_t	fwdCount = randomNumber( int(dirBtree.size()) );

			for( size_t idx = 0; idx < fwdCount && it != end; ++it, ++idx ) 
				;
			if( it != end )
				dirBtree.removeElement( *it );
			size_t depth;
			UT_ASSERT_TRUE( dirBtree.test( &depth, false ) );
		}

		TreeMap<int,STRING>						map;
		typedef TreeMap<int,STRING>::value_type	MyPair;

		for( int i=0; i<30000; i++ )
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::for9" );
			map += MyPair( i, formatNumber( randomNumber( std::numeric_limits<int>::max() ) ) );
		}

		STRING	&value1 = map[5000];
		UT_ASSERT_FALSE( value1.isEmpty() );
		map[5000] = "Manche mögens heiss";
		STRING	&value2 = map[5000];
		UT_ASSERT_EQUAL( value2, STRING("Manche mögens heiss") );

		STRING	value = map[-20];
		UT_ASSERT_TRUE( value.isEmpty() );
		value = map[60000];
		UT_ASSERT_TRUE( value.isEmpty() );
	}

	virtual bool canStressTest()
	{
		return true;
	}
	virtual void StressTest( size_t factor )
	{
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::PerformTest::<10,5>" );
			TestScope scope( "<10,5>" );
			ContainerTest<10,5>(factor*10240,true, true);
		}

		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::PerformTest::<2,5>" );
			TestScope scope( "<2,5>" );
			ContainerTest<2,5>(factor*1024,false,true);
		}
	}
	virtual bool canThreadTest()
	{
		return true;
	}
	virtual UnitTest *duplicate()
	{
		return new BtreeTest( false );
	}
	public:
	BtreeTest( bool isStatic=true ) : UnitTest( isStatic ) {}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static BtreeTest myBtreeTest;

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

