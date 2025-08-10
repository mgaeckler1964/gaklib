/*
		Project:		GAKLIB
		Module:			GeoGraphTest.h
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

#include <gak/geoGraph.h>

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

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class GeoGraphTest : public UnitTest
{
	MvvGraph	theMVV;

	public:
	virtual const char *GetClassName() const
	{
		return "GeoGraphTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "GeoGraphTest::PerformTest");
		TestScope scope( "PerformTest" );

		math::Rectangle< math::GeoPosition<double> > bbox = theMVV.getBoundingBox();
		UT_ASSERT_EQUAL( theMVV.minLon, bbox.topLeft.longitude );
		UT_ASSERT_EQUAL( theMVV.maxLat, bbox.topLeft.latitude );
		UT_ASSERT_EQUAL( theMVV.maxLon, bbox.bottomRight.longitude );
		UT_ASSERT_EQUAL( theMVV.minLat, bbox.bottomRight.latitude );

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

		theMVV.moveNode( SBAHN_LAYER, UBAHN_LAYER, Marienplatz_S );
		theSmallMVV.clear();
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
		UT_ASSERT_TRUE( theSmallMVV.hasElement( Marienplatz_S ) );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static GeoGraphTest	myGeoGraphTest;

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

