/*
		Project:		GAKLIB
		Module:			GraphTest.h
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

#include <iostream>
#include <gak/unitTest.h>

#include <gak/graph.h>

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

class GraphTest : public UnitTest
{
	public:
	virtual const char *GetClassName() const
	{
		return "GraphTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "GraphTest::PerformTest");
		TestScope scope( "PerformTest" );

		//typedef Graph<double, double, GraphTree, size_t, size_t>	SimpleGraph;
		typedef Graph<double, double, GraphMap, size_t, size_t>	SimpleGraph;

		SimpleGraph		theGraph;

		theGraph.addNode( 0, 3.149 );
		theGraph.addNode( 1, 2.181 );
		theGraph.addNode( 2, 10 );

		theGraph.addLink( 3, 3.14, 1, 0 );
		theGraph.addLink( 4, 2.18, 0, 1 );

		theGraph.addLink( 5, 3.141, 1, 2 );
		theGraph.addLink( 6, 2.182, 2, 1 );

		theGraph.addLink( 7, 55, 0, 2 );
		theGraph.addLink( 8, 56, 2, 0 );

		SimpleGraph		theGraph2;
		theGraph2.merge(theGraph);

		UT_ASSERT_EQUAL( size_t(6), theGraph.getNumLinks() );
		UT_ASSERT_EQUAL( size_t(3), theGraph.getNumNodes() );
		UT_ASSERT_EQUAL( size_t(6), theGraph2.getNumLinks() );
		UT_ASSERT_EQUAL( size_t(3), theGraph2.getNumNodes() );

		UT_ASSERT_EQUAL( theGraph.getLink( 4 ), 2.18 );
		UT_ASSERT_EQUAL( theGraph.getLink( 5 ), 3.141 );
		UT_ASSERT_EQUAL( theGraph.getLink( 7 ), 55. );
		UT_ASSERT_EQUAL( theGraph.getLinkStart( 7 ), size_t(0) );
		UT_ASSERT_EQUAL( theGraph.getLinkEnd( 7 ), size_t(2) );

		UT_ASSERT_EQUAL( theGraph.getNode( 2 ), 10. );
		const SimpleGraph::link_key_types	&outgoing = theGraph.getOutgoing( 2 );
		UT_ASSERT_EQUAL( size_t(2), outgoing.size() );
		UT_ASSERT_TRUE( outgoing.hasElement( 8 ) );
		UT_ASSERT_TRUE( outgoing.hasElement( 6 ) );

#if HAVE_INCOMING
		theGraph.removeNode( 0 );
		theGraph.removeNode( 1 );
		theGraph.removeNode( 2 );

		UT_ASSERT_EQUAL( size_t(0), theGraph.getNumLinks() );
		UT_ASSERT_EQUAL( size_t(0), theGraph.getNumNodes() );
#endif

	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static GraphTest	myGraphTest;

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

