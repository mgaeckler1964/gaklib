/*
		Project:		GAKLIB
		Module:			GpsTest.h
		Description:	
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

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

#include <gak/gps.h>

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
namespace math
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

class GpsTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "GpsTest";
	}
	virtual void PerformTest( void )
	{
		PositionTest();
		RectangleTest();
		TileTest();
	}
	void PositionTest( void )
	{
		GpsPosition<double>	start;

		start.latitude = ConvertDegree( 48, 46.686 );
		start.longitude = ConvertDegree( 11, 26.778 );
		start.height = 0;

		GpsPosition<double>	end;

		end.latitude = ConvertDegree( 48, 46.487 );
		end.longitude = ConvertDegree( 11, 26.222 );
		end.height = 0;

		double	distance = getDistance( start, end );
		std::cout << distance << std::endl;
		UT_ASSERT_EQUAL( distance, 772.70455160827623 );

		double angle = 12.2525;
		int degree;
		unsigned minutes;
		double seconds;

		ConvertDegree( angle, &degree, &minutes, &seconds );
		UT_ASSERT_EQUAL( degree, 12 );
		UT_ASSERT_EQUAL( minutes, 15U );
		UT_ASSERT_LESS( abs(seconds-9.0), 1e-11 );

		ConvertDegree( -angle, &degree, &minutes, &seconds );
		UT_ASSERT_EQUAL( degree, -12 );
		UT_ASSERT_EQUAL( minutes, 15U );
		UT_ASSERT_LESS( abs(seconds-9.0), 1e-11 );

		{
			GeoPosition<double> start( 10, 48 );
			GeoPosition<double> end( 10, 50 );
			double bearing = getBearing( start, end );
			UT_ASSERT_EQUAL( 0.0, bearing );

			end.latitude = 40;
			bearing = getBearing( start, end );
			UT_ASSERT_EQUAL( M_PI, bearing );

			start.latitude = 0;
			end.latitude = 0;
			end.longitude = 12;
			bearing = getBearing( start, end );
			UT_ASSERT_EQUAL( M_PI/2, bearing );
			bearing = getBearingDegree( start, end );
			UT_ASSERT_EQUAL( 90.0, bearing );

			end.longitude = 8;
			bearing = getBearing( start, end );
			UT_ASSERT_EQUAL( -M_PI/2, bearing );

			bearing = getBearingDegree( start, end );
			UT_ASSERT_EQUAL( 270.0, bearing );
		}
	}
	void RectangleTest()
	{
		GeoPosition<double>	left( 10, 2 );
		GeoPosition<double>	right( 11, 22 );
		GeoPosition<double>	sum = left + right;
		UT_ASSERT_EQUAL( sum.longitude, 21.0 );
		UT_ASSERT_EQUAL( sum.latitude, 24.0 );
		left += right;
		UT_ASSERT_EQUAL( left.longitude, 21.0 );
		UT_ASSERT_EQUAL( left.latitude, 24.0 );

		Rectangle< GeoPosition<double> >	rect( left, right );
		moveRectangle( &rect, left );
		UT_ASSERT_EQUAL( rect.topLeft.longitude, 42.0 );
		UT_ASSERT_EQUAL( rect.topLeft.latitude, 48.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.longitude, 32.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.latitude, 46.0 );

		moveRectangle( &rect, GeoPosition<double>( 0, 45 ) );
		UT_ASSERT_EQUAL( rect.topLeft.longitude, 42.0 );
		UT_ASSERT_EQUAL( rect.topLeft.latitude, 90.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.longitude, 32.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.latitude, 88.0 );

		moveRectangle( &rect, GeoPosition<double>( 0, -180 ) );
		UT_ASSERT_EQUAL( rect.topLeft.longitude, 42.0 );
		UT_ASSERT_EQUAL( rect.topLeft.latitude, -88.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.longitude, 32.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.latitude, -90.0 );
	}
	void TileTest()
	{
		tileid_t tileId = GeoPosition<double>::getTileID(-179.9, -89.9);
		UT_ASSERT_EQUAL( tileId, tileid_t(0) );

		TileTest(0, 0);					// atlantic ocean
		TileTest(-180, 0);				// pacific ocean
		TileTest(-180, -90);			// south pole

		TileTest(179.99, 90);			// north pole

		TileTest(14.33657, 48.24250);	// where I'm living
		TileTest(11.56187, 48.13097);	// where I was born
	}
	void TileTest( double longitude, double latitude )
	{
		tileid_t tileId = GeoPosition<double>::getTileID(longitude, latitude);
		GeoPosition<double> lowerLeft;
		GeoPosition<double> upperRight;
		
		GeoPosition<double>::getTile(tileId, lowerLeft, upperRight);
		UT_ASSERT_LESSEQ( lowerLeft.longitude, longitude );
		UT_ASSERT_LESSEQ( lowerLeft.latitude, latitude );
		UT_ASSERT_GREATEREQ( upperRight.longitude, longitude );
		UT_ASSERT_GREATEREQ( upperRight.latitude, latitude );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static GpsTest myGpsTest;

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

}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

