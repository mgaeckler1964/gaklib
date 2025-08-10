/*
		Project:		GAKLIB
		Module:			IndexerTest.h
		Description:	unit test for indexer
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

#include <iostream>
#include <gak/unitTest.h>

#include <gak/indexer.h>

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

class IndexerTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "IndexerTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "IndexerTest::PerformTest");
		TestScope scope( "PerformTest" );

		const char		*searchWord1 = "the";
		const char		*searchWord2 = "of";
		const char		*searchWord3 = "brown";
		const char		*searchWord4 = "namespace";
		const char		*searchWord5 = "className";
		const char		*stopWord = "BROWN";

		SortedArray<CI_STRING>	stopWords;
		stopWords.addElement( stopWord );
		const STRING	testText1 = "the quick brown fox jumps over the lazy dog äöüß+ÄÖÜß=äöüßÄÖÜß";
		const STRING	testText2 = "the best of the world of all brown universes namespace::className";
		StringIndex		positions = indexString( testText1, stopWords );

		UT_ASSERT_EQUAL( std::size_t(19), positions.size() );
		UT_ASSERT_EQUAL( std::size_t(2), positions[searchWord1].size());

		Index<STRING>	globalIndex;
		typedef Index<STRING>::SearchResult	SearchResult;

		globalIndex.mergeIndexPositions( "testText1", positions );
		UT_ASSERT_EQUAL( 
			std::size_t(1), 
			globalIndex[searchWord1].size()
		);
		UT_ASSERT_FALSE( 
			globalIndex.hasElement(searchWord2)
		);
		UT_ASSERT_FALSE(
			globalIndex.hasElement(searchWord3)
		);
		globalIndex.mergeIndexPositions( 
			"testText2", indexString( testText2, stopWords )
		);
		UT_ASSERT_EQUAL( 
			std::size_t(2), 
			globalIndex[searchWord1].size()
		);
		UT_ASSERT_EQUAL( 
			std::size_t(1), 
			globalIndex[searchWord2].size()
		);
		UT_ASSERT_EQUAL( 
			STRING("testText2"), 
			globalIndex[searchWord2].getKeys()[0]
		);
		UT_ASSERT_EQUAL( 
			std::size_t(2), 
			globalIndex[searchWord2]["testText2"].size()
		);

		UT_ASSERT_EQUAL( 
			std::size_t(1), 
			globalIndex[searchWord4].size()
		);
		UT_ASSERT_EQUAL( 
			std::size_t(1), 
			globalIndex[searchWord5].size()
		);

		const Position &of0 = globalIndex[searchWord2]["testText2"][0];
		const Position &of1 = globalIndex[searchWord2]["testText2"][1];
		UT_ASSERT_EQUAL( STRING(searchWord2), testText2.subString( of0.m_start, 2 ) );
		UT_ASSERT_EQUAL( STRING(searchWord2), testText2.subString( of1.m_start, 2 ) );
		UT_ASSERT_FALSE(
			globalIndex.hasElement(searchWord3)
		);

		SearchResult	sources = globalIndex.findWords( searchWord1, true, true, false );
		UT_ASSERT_EQUAL( std::size_t(2), sources.size() );

		sources = globalIndex.findWords( "the -quick", true, true, false );
		UT_ASSERT_EQUAL( std::size_t(1), sources.size() );
		UT_ASSERT_EQUAL( STRING("testText2"), sources.cbegin()->getKey() );

		sources = globalIndex.findWords( "the +quick", true, true, false );
		UT_ASSERT_EQUAL( std::size_t(1), sources.size() );
		UT_ASSERT_EQUAL( STRING("testText1"), sources.cbegin()->getKey() );
		UT_ASSERT_EQUAL( std::size_t(0), sources.cbegin()->getValue()[0].m_start );
		UT_ASSERT_EQUAL( std::size_t(4), sources.cbegin()->getValue()[1].m_start );

		sources = globalIndex.findWords( "fox all", true, true, false );
		UT_ASSERT_EQUAL( std::size_t(2), sources.size() );

		sources = globalIndex.findWords( "fox all", true, true, true );
		UT_ASSERT_EQUAL( std::size_t(2), sources.size() );

		StatistikData	stats = globalIndex.getStatistik();
		UT_ASSERT_EQUAL( std::size_t(32), stats.size() );

		globalIndex.mergeIndexPositions( "Gäckler", indexString( STRING("Gäckler"), stopWords ) );
		sources = globalIndex.findWords( "gakler", true, true, true );
		UT_ASSERT_EQUAL( std::size_t(1), sources.size() );
		Index<STRING>::RelevantHits	relevant = globalIndex.getRelevantHits( "gakler", true, true, true );
		UT_ASSERT_EQUAL( std::size_t(1), relevant.size() );

		sources = globalIndex.findWords( "gakle?", true, true, true );
		UT_ASSERT_EQUAL( std::size_t(1), sources.size() );
		sources = globalIndex.findWords( "kakle?", true, true, true );
		UT_ASSERT_EQUAL( std::size_t(1), sources.size() );
		sources = globalIndex.findWords( "kakle?", false, true, true );
		UT_ASSERT_EQUAL( std::size_t(0), sources.size() );

		writeToBinaryFile("indexer.tmp", globalIndex, 123, 1, owmOverwrite );
		readFromBinaryFile("indexer.tmp", &globalIndex, 123, 1, false );
		strRemoveE("indexer.tmp");
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static IndexerTest	myIndexerTest;

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

