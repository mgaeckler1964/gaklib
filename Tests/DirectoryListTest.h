/*
		Project:		GAKLIB
		Module:			DirectoryListTest.h
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

#include <gak/directory.h>

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

class DirectoryListTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "DirectoryListTest";
	}
	virtual void PerformTest( void )
	{
		DirectoryList	list;
		size_t			i=0;

#define SEARCH_PATTERN	"*.cpp"
#if USE_LOCAL_ONLY
	#define SEARCH_PATH "CTOOLS" DIRECTORY_DELIMITER_STRING
#elif defined( _Windows )
	#define SEARCH_PATH "L:\\Source\\GAKLIB\\CTOOLS\\"
#else
	#define SEARCH_PATH "/Source/GAKLIB/CTOOLS/"
#endif
		list.findFiles( SEARCH_PATH SEARCH_PATTERN );
		
		// the number of C++ Source files ind CTOOLS here:
		UT_ASSERT_EQUAL( list.size(), size_t(81) );
		for( 
			DirectoryList::const_iterator it = list.cbegin(), endIT = list.cend();
			it != endIT;
			++it
		)
		{
			const DirectoryEntry		&listEntry = *it;
			F_STRING					fName = F_STRING(SEARCH_PATH)+listEntry.fileName;
			DirectoryEntry				theEntry( fName );
			UT_ASSERT_EQUAL( theEntry.fileName, fName );
			UT_ASSERT_EQUAL( listEntry.modifiedDate, theEntry.modifiedDate );
// perhaps this test changes the accessdate 8-(
//			UT_ASSERT_EQUAL( listEntry.accessDate, theEntry.accessDate );
		}

		list.findFiles( __FILE__ );
		UT_ASSERT_EQUAL( list.size(), size_t(1) );

#if defined( NDEBUG ) && !defined( __MACH__ )
#	if defined( _Windows )
#		define TREE_PATH	"C:\\CRESD"
#	else
#		define TREE_PATH	"/home"
#	endif
		std::cout << "loading" << std::endl;
		clock_t startTime = clock();
		list.clear();
		list.resort( DirectoryEntry::SORT_SIZE );
		list.dirtree( TREE_PATH );
		std::cout << clock() - startTime << " ms" << std::endl;

		std::cout << "copying" << std::endl;
		startTime = clock();
		typedef SortedArray<DirectoryEntry, DynamicComparator<DirectoryEntry> >	ArrayTree;
		ArrayTree arrayTree;
		arrayTree.resort( DirectoryEntry::SORT_SIZE );

		std::copy( list.begin(), list.end(), std::back_inserter( arrayTree ) );
		std::cout << clock() - startTime << " ms" << std::endl;

		std::cout << "Testing Btree" << std::endl;
		uint64	lastSize = 0;
		i=0;
		for( 
			DirectoryList::iterator it = list.begin(), endit = list.end();
			it != endit;
			++it, ++i
		)
		{
			UT_ASSERT_LESSEQ( lastSize, it->fileSize );
			lastSize = it->fileSize;
		}
		UT_ASSERT_TRUE( list.test() );
		UT_ASSERT_EQUAL( list.size(), i );

		std::cout << "Testing Array" << std::endl;
		lastSize = 0;
		i=0;
		for( 
			ArrayTree::const_iterator it = arrayTree.cbegin(), endit = arrayTree.cend();
			it != endit;
			++it, ++i
		)
		{
			UT_ASSERT_LESSEQ( lastSize, it->fileSize );
			lastSize = it->fileSize;
		}
		UT_ASSERT_EQUAL( arrayTree.size(), i );

		std::cout << "resort tree" << std::endl;
		startTime = clock();
		list.resort( DirectoryEntry::SORT_NAME );
		std::cout << clock() - startTime << " ms" << std::endl;

		std::cout << "resort array" << std::endl;
		startTime = clock();
		arrayTree.resort( DirectoryEntry::SORT_NAME );
		std::cout << clock() - startTime << " ms" << std::endl;
#endif
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static DirectoryListTest myDLtest;

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

