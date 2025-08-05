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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class BtreeTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "BtreeTest";
	}
	virtual void PerformTest( void )
	{
		doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform" );
		Btree<int>	container;

		for( size_t i=0; i<30000; i++ )
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::add" );
			container += randomNumber( std::numeric_limits<int>::max() );
//			container.addElement( int(i) );
		}

		{
			Btree<int>::iterator it = container.begin();
			Btree<int>::iterator end = container.end();
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
			Btree<int>::const_iterator it = container.cbegin();
			Btree<int>::const_iterator end = container.cend();
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
			Btree<int>::reverse_iterator it = container.rbegin();
			Btree<int>::reverse_iterator end = container.rend();
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
			Btree<int>::const_reverse_iterator it = container.crbegin();
			Btree<int>::const_reverse_iterator end = container.crend();
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
		while( container.size() )
		{
			doEnterFunctionEx( gakLogging::llInfo, "BtreeTest::Perform::while4" );
			Btree<int>::const_reverse_iterator it = container.crbegin();
			Btree<int>::const_reverse_iterator end = container.crend();
			size_t	fwdCount = randomNumber( int(container.size()) );

			for( size_t idx = 0; idx < fwdCount && it != end; ++it, ++idx ) 
				;
			if( it != end )
				container.removeElement( *it );
			if( !(container.size() % 1000 ) )
				std::cout << "Depth: " << container.test(true) << std::endl;
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
			dirBtree.test();
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
			dirBtree.test( true );
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

