/*
		Project:		GAKLIB
		Module:			GeometryTest.h
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

#include <gak/geometry.h>

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

class GeometryTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "GeometryTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "GeometryTest::PerformTest");
		TestScope scope( "PerformTest" );
		PointTest();
		RectangleTest();
	}
	void PointTest()
	{
		Point<float>	floatPoint( 5, 10 );
		Point<double>	doublePoint( floatPoint );

		UT_ASSERT_EQUAL( floatPoint.x, float(doublePoint.x) );
		UT_ASSERT_EQUAL( floatPoint.y, float(doublePoint.y) );

		floatPoint.x = 10;
		floatPoint.y = 5;
		doublePoint = floatPoint;

		UT_ASSERT_EQUAL( floatPoint.x, float(doublePoint.x) );
		UT_ASSERT_EQUAL( floatPoint.y, float(doublePoint.y) );

		floatPoint.x = 10;
		floatPoint.y = 0;
		PolarPoint<double>	pPoint = floatPoint;

		UT_ASSERT_EQUAL( pPoint.radius, 10.0 );
		UT_ASSERT_EQUAL( pPoint.angle, 0.0 );

		doublePoint = pPoint;
		UT_ASSERT_EQUAL( doublePoint.x, 10.0 );
		UT_ASSERT_EQUAL( doublePoint.y, 0.0 );

		floatPoint.x = -10;
		floatPoint.y = 0;
		pPoint = floatPoint;

		UT_ASSERT_EQUAL( pPoint.radius, 10.0 );
		UT_ASSERT_LESS( abs( pPoint.angle - M_PI ), 1e-7 );

		doublePoint = pPoint;
		UT_ASSERT_LESS( abs(doublePoint.x  +10.0), 1e-7 );
		UT_ASSERT_LESS( abs(doublePoint.y), 1e-6 );

		floatPoint.x = 0;
		floatPoint.y = 10;
		pPoint = floatPoint;

		UT_ASSERT_EQUAL( pPoint.radius, 10.0 );
		UT_ASSERT_LESS( abs( pPoint.angle - M_PI/2 ), 1e-7 );

		doublePoint = pPoint;
		UT_ASSERT_LESS( abs(doublePoint.x), 1e-6 );
		UT_ASSERT_EQUAL( doublePoint.y, 10.0 );

		floatPoint.x = 0;
		floatPoint.y = -10;
		pPoint = floatPoint;

		UT_ASSERT_EQUAL( pPoint.radius, 10.0 );
		UT_ASSERT_LESS( abs( pPoint.angle + M_PI/2 ), 1e-7 );

		doublePoint = pPoint;
		UT_ASSERT_LESS( abs(doublePoint.x), 1e-6 );
		UT_ASSERT_EQUAL( doublePoint.y, -10.0 );

		for( double x=-5; x<=+5; x += 1 )
		{
			for( double y=-5; y<=+5; y += 1 )
			{
				Point<double>		kp1( x, y );
				PolarPoint<double>	pp( kp1 );
				Point<double>		kp2( pp );
				UT_ASSERT_EQUAL( kp1.x, kp2.x );
				UT_ASSERT_EQUAL( kp1.y, kp2.y );
			}
		}

		for( double angle=-M_PI; angle<=+M_PI; angle+= M_PI/5 )
		{
			for( double radius=1; radius<=2; ++radius )
			{
				PolarPoint<double>	pp1( radius, angle );
				Point<double>		kp( pp1 );
				PolarPoint<double>	pp2( kp );
				UT_ASSERT_EQUAL( pp1.radius, pp2.radius );
				UT_ASSERT_EQUAL( pp1.angle, pp2.angle );
			}
		}
	}
	void RectangleTest()
	{
		Point<double>	left( 10, 2 );
		Point<double>	right( 11, 22 );
		Point<double>	sum = left + right;
		UT_ASSERT_EQUAL( sum.x, 21.0 );
		UT_ASSERT_EQUAL( sum.y, 24.0 );
		left += right;
		UT_ASSERT_EQUAL( left.x, 21.0 );
		UT_ASSERT_EQUAL( left.y, 24.0 );

		Rectangle< Point<double> >	rect( left, right );
		moveRectangle( &rect, left );
		UT_ASSERT_EQUAL( rect.topLeft.x, 42.0 );
		UT_ASSERT_EQUAL( rect.topLeft.y, 48.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.x, 32.0 );
		UT_ASSERT_EQUAL( rect.bottomRight.y, 46.0 );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static GeometryTest	myGeometryTest;

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

