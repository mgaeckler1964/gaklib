/*
		Project:		GAKLIB
		Module:			RoutingTest.h
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

#include <gak/stopWatch.h>
#include <gak/geoGraph.h>
#include <gak/routing.h>

#include "mvv.h"

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

template <>
double getLinkCost( const Connection &link, CostType costType )
{
	return costType == ctBY_DISTANCE ? link.length : link.time;
}
template <>
double getEstimatedCost( const Station &from, const Station &to, CostType costType )
{
	double distance = getDistance(from.position, to.position);

	return costType == ctBY_DISTANCE ? distance : distance / (120./3.6);
}


template <>
double getLinkCost( const double &link, bool costType )
{
	if( costType )
		return link;
	else
		return 1;
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class RoutingTest : public UnitTest
{
	MvvGraph	theMVV;

	public:
	virtual const char *GetClassName() const
	{
		return "RoutingTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "RoutingTest::PerformTest");
		TestScope scope( "PerformTest" );

		typedef Graph<double, double, GraphTree, size_t, size_t>	SimpleGraph;
		typedef RecursiveRouting<SimpleGraph>						SimpleRouting;
		typedef AstarRouting<SimpleGraph>							SimpleARouting;

		SimpleGraph		theGraph;
		SimpleRouting	simpleRouting( theGraph );
		SimpleARouting	simpleARouting( theGraph );

		theGraph.addNode( 0, 3.149 );
		theGraph.addNode( 1, 2.181 );
		theGraph.addNode( 2, 10 );

		theGraph.addLink( 3, 3.14, 1, 0 );
		theGraph.addLink( 4, 2.18, 0, 1 );

		theGraph.addLink( 5, 3.141, 1, 2 );
		theGraph.addLink( 6, 2.182, 2, 1 );

		theGraph.addLink( 7, 55, 0, 2 );
		theGraph.addLink( 8, 56, 2, 0 );

		UT_ASSERT_EQUAL( size_t(6), theGraph.getNumLinks() );
		UT_ASSERT_EQUAL( size_t(3), theGraph.getNumNodes() );

		Array<size_t>	route1 = simpleRouting.route( 0, 2, false ).get().links;
		UT_ASSERT_EQUAL( route1.size(), size_t( 1 ) );
		UT_ASSERT_EQUAL( route1[0], size_t( 7 ) );

		Array<size_t>	route2 = simpleRouting.route( 0, 2, true ).get().links;
		UT_ASSERT_EQUAL( route2.size(), size_t( 2 ) );
		UT_ASSERT_EQUAL( route2[0], size_t( 4 ) );
		UT_ASSERT_EQUAL( route2[1], size_t( 5 ) );

		{
			TestScope scope("OEZ_U1, Mangfallplatz, ctBY_DISTANCE");
			UT_ASSERT_TRUE( routeMVV( OEZ_U1, Mangfallplatz, ctBY_DISTANCE ) );
			UT_ASSERT_TRUE( routeMVV( Geltendorf, Ebersberg, ctBY_DISTANCE ) );
			UT_ASSERT_TRUE( routeMVV( Geltendorf, Ebersberg, ctBY_SPEED ) );
		}
		{
			TestScope scope("Geltendorf, OEZ_U3, ctBY_DISTANCE");
			UT_ASSERT_TRUE( routeMVV( Geltendorf, OEZ_U3, ctBY_DISTANCE ) );
			UT_ASSERT_TRUE( routeMVV( Geltendorf, OEZ_U3, ctBY_SPEED ) );
		}
		{
			TestScope scope("Ostbahnhof_S, LaimerPlatz, ctBY_SPEED");
			UT_ASSERT_TRUE( routeMVV( Ostbahnhof_S, LaimerPlatz, ctBY_SPEED ) );
		}
		{
			TestScope scope("Ostbahnhof_S, LaimerPlatz, ctBY_DISTANCE");
			UT_ASSERT_TRUE( routeMVV( Ostbahnhof_S, LaimerPlatz, ctBY_DISTANCE ) );
		}
		{
			TestScope scope("IngolstadtNord, Ostbahnhof_S, ctBY_DISTANCE");
			UT_ASSERT_FALSE( routeMVV( IngolstadtNord, Ostbahnhof_S, ctBY_DISTANCE ) );
		}
		{
			TestScope scope("Ostbahnhof_S, IngolstadtNord, ctBY_DISTANCE");
			UT_ASSERT_FALSE( routeMVV( Ostbahnhof_S, IngolstadtNord, ctBY_DISTANCE ) );
		}
		{
			TestScope scope("Ostbahnhof_S, Ostbahnhof_S, ctBY_DISTANCE");
			UT_ASSERT_TRUE( routeMVV( Ostbahnhof_S, Ostbahnhof_S, ctBY_DISTANCE ) );
		}

		Array<Bahnhoefe>	theSmallMVV;
		theMVV.getRegion( 
			UBAHN_LAYER, 
			math::Rectangle< math::GeoPosition<double> >(
				math::GeoPosition<double>(math::ConvertDegree( 11, 34.528 ),
				math::ConvertDegree( 48, 8.236 )),
				math::GeoPosition<double>(math::ConvertDegree( 11, 36.289 ),
				math::ConvertDegree( 48, 7.648 ))
			), 
			&theSmallMVV 
		);
		UT_ASSERT_TRUE( theSmallMVV.hasElement( Marienplatz_U ) );
		UT_ASSERT_TRUE( theSmallMVV.hasElement( Ostbahnhof_U ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( LaimerPlatz ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( Stachus_U ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( Trudering_U ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( MessestadtOst ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( Odeonsplatz_U3 ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( SendlingerTor_U3 ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( Kolumbusplatz ) );
		
		UT_ASSERT_FALSE( theSmallMVV.hasElement( Marienplatz_S ) );
		UT_ASSERT_FALSE( theSmallMVV.hasElement( Ostbahnhof_S ) );
	}

	bool routeMVV( Bahnhoefe start, Bahnhoefe ende, CostType pt )
	{
		typedef RecursiveRouting<MvvGraph>	MvvRouting;
		typedef AstarRouting<MvvGraph>		MvvARouting;

		MvvRouting	mvvRouting( theMVV );
		MvvARouting	mvvARouting( theMVV );

		std::cout << "From " << theMVV.getNode( start ).name << " to " << theMVV.getNode( ende ).name << std::endl;
		StopWatch	routeTime( true );
		Optional<MvvRouting::RouteResult>	route = mvvRouting.route( start, ende, pt );
		routeTime.stop();

		StopWatch	starTime( true );
		Optional<MvvARouting::RouteResult>	starRoute = mvvARouting.route( start, ende, pt );
		starTime.stop();
		std::cout << routeTime.get< Seconds<clock_t> >().toString() << " vs. " << starTime.get< Seconds<clock_t> >().toString() << std::endl;
		UT_ASSERT_EQUAL( route.isPresent(), starRoute.isPresent() );

		if( route.isPresent() && starRoute.isPresent() )
		{
			UT_ASSERT_EQUAL( route.get().cost, starRoute.get().cost );

			showRoute( route.get().links );
			showRoute( starRoute.get().links );
		}

		return route.isPresent() || starRoute.isPresent();
	}

	void showRoute( const Array<long> &route )
	{
		Array<Bahnhoefe> stations = theMVV.getNodes( route );
		for(
			Array<Bahnhoefe>::const_iterator it = stations.cbegin(), endIT = stations.cend();
			it != endIT;
			++it
		)
		{
			std::cout << theMVV.getNode( *it ).name << std::endl;
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static RoutingTest	myRoutingTest;

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

