/*
		Project:		GAKLIB
		Module:			MathTest.h
		Description:	Some math functions and classes
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#include <gak/math.h>

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

class MathTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "MathTest";
	}

	template <typename GeoT>
	void GeoTest()
	{
		GeoT	point1( 15, 48 );
		GeoT	point2( 10, 48 );

		double dist1 = math::distance( point1, point2 );		// the template
		double dist2 = math::getDistance( point1, point2 );		// my original
		UT_ASSERT_EQUAL( dist1, dist2 );
		UT_ASSERT_EQUAL_FLT( dist1, 372023.20, 0.1 );

		GeoT	point3( 10.5, 48 ),
				point4( 5, 48 );
		gak::Array<GeoT>	testData;
		testData.push_back(point1);
		testData.push_back(point2);
		testData.push_back(point3);
		testData.push_back(point4);

		math::Mean< GeoT > mean(testData.cbegin(), testData.cend());

		UT_ASSERT_EQUAL(mean.getMean().longitude, 10.125 );
		UT_ASSERT_EQUAL(mean.getMean().latitude, 48 );
	}

	void AllGeoTests()
	{
		math::GeoPosition<float>	point1( 15, 48 ),
									point2( 10, 48 );

		math::GpsPosition<float>	point3( 15, 48 ),
									point4( 10, 48 );

		double dist1 = math::distance( point1, point2 );
		double dist2 = math::distance( point3, point4 );
		UT_ASSERT_EQUAL(dist1, dist2);

		{
			TestScope scope( "GeoPosition<float>" );
			GeoTest< math::GeoPosition<float> >();
		}
		{
			TestScope scope( "GpsPosition<float>" );
			GeoTest< math::GpsPosition<float> >();
		}
	}
	void getExponentTest()
	{
		UT_ASSERT_EQUAL( 2, math::getExponent( 999.9999 ) );
		UT_ASSERT_EQUAL( 2, math::getExponent( 100.0 ) );
		UT_ASSERT_EQUAL( 2, math::getExponent( -100.0 ) );
		UT_ASSERT_EQUAL( 1, math::getExponent( 99.99999 ) );
		UT_ASSERT_EQUAL( 1, math::getExponent( 10.0 ) );
		UT_ASSERT_EQUAL( 1, math::getExponent( -10.0 ) );
		UT_ASSERT_EQUAL( 0, math::getExponent( 9.999999 ) );
		UT_ASSERT_EQUAL( 0, math::getExponent( 1.0 ) );
		UT_ASSERT_EQUAL( 0, math::getExponent( -1.0 ) );
		UT_ASSERT_EQUAL( 0, math::getExponent( .0 ) );
		UT_ASSERT_EQUAL( -1, math::getExponent( 0.9999999 ) );
		UT_ASSERT_EQUAL( -1, math::getExponent( 0.1 ) );
		UT_ASSERT_EQUAL( -1, math::getExponent( -0.1 ) );
		UT_ASSERT_EQUAL( -2, math::getExponent( 0.09999999 ) );
		UT_ASSERT_EQUAL( -2, math::getExponent( 0.01 ) );
		UT_ASSERT_EQUAL( -2, math::getExponent( -0.01 ) );
	}
	void normalizeTest()
	{
		int exponent;
		double value;

		value = math::normalize( 999.9999, &exponent );
		UT_ASSERT_EQUAL( 9.999999, value );
		UT_ASSERT_EQUAL( 2, exponent );
		value = math::normalize( 99.99999, &exponent );
		UT_ASSERT_EQUAL( 9.999999, value );
		UT_ASSERT_EQUAL( 1, exponent );
		value = math::normalize( 9.999999, &exponent );
		UT_ASSERT_EQUAL( 9.999999, value );
		UT_ASSERT_EQUAL( 0, exponent );
		value = math::normalize( .9999999, &exponent );
		UT_ASSERT_EQUAL( 9.999999, value );
		UT_ASSERT_EQUAL( -1, exponent );
		value = math::normalize( .09999999, &exponent );
		UT_ASSERT_EQUAL( 9.999999, value );
		UT_ASSERT_EQUAL( -2, exponent );

		value = math::normalize( -999.9999, &exponent );
		UT_ASSERT_EQUAL( -9.999999, value );
		UT_ASSERT_EQUAL( 2, exponent );
		value = math::normalize( -99.99999, &exponent );
		UT_ASSERT_EQUAL( -9.999999, value );
		UT_ASSERT_EQUAL( 1, exponent );
		value = math::normalize( -9.999999, &exponent );
		UT_ASSERT_EQUAL( -9.999999, value );
		UT_ASSERT_EQUAL( 0, exponent );
		value = math::normalize( -0.9999999, &exponent );
		UT_ASSERT_EQUAL( -9.999999, value );
		UT_ASSERT_EQUAL( -1, exponent );
		value = math::normalize( -0.09999999, &exponent );
		UT_ASSERT_EQUAL( -9.999999, value );
		UT_ASSERT_EQUAL( -2, exponent );
	}
	void minMaxMeanTest()
	{
		math::MinMax<int>	minMax;

		UT_ASSERT_LESS( 0, minMax.getMin() );
		UT_ASSERT_GREATER( 0, minMax.getMax() );

		minMax.test( 5 );

		UT_ASSERT_EQUAL( 5, minMax.getMin() );
		UT_ASSERT_EQUAL( 5, minMax.getMax() );

		minMax.test( 10 );
		minMax.test( 3 );

		UT_ASSERT_EQUAL( 3, minMax.getMin() );
		UT_ASSERT_EQUAL( 10, minMax.getMax() );

		math::MinMax<double>	minMaxD;

		minMaxD.test( -4 );
		minMaxD.test( -8 );
		UT_ASSERT_EQUAL( -8, minMaxD.getMin() );
		UT_ASSERT_EQUAL( -4, minMaxD.getMax() );

		math::Mean<double>	mean;
		mean.add( 8.0 );
		UT_ASSERT_EQUAL( 8.0, mean.getMean() );
		mean.add( 8.0 );
		UT_ASSERT_EQUAL( 8.0, mean.getMean() );
		mean.add( 0.0 );
		mean.add( 0.0 );
		UT_ASSERT_EQUAL( 4.0, mean.getMean() );
		UT_ASSERT_EQUAL( 4, mean.getCount() );

		{
			int tmp[] = { 3,16,8 };
			Array<int>	datas(tmp);

			math::MinMax<int>	minMax( datas.cbegin(), datas.cend() );
			math::Mean<int>		mean( datas.cbegin(), datas.cend() );

			UT_ASSERT_EQUAL(minMax.getMin(), 3);
			UT_ASSERT_EQUAL(minMax.getMax(), 16);
			UT_ASSERT_EQUAL(minMax.getRange(), 13);
			UT_ASSERT_EQUAL(minMax.getMidRange(), 6);

			UT_ASSERT_EQUAL(mean.getCount(), 3);
			UT_ASSERT_EQUAL(mean.getMean(), 9);
		}
	}
	void scalarProductTest()
	{
		double tmp1[] = { 1,2,0,3,666 };
		double tmp2[] = { 0,2,33,4 };
		Array<double> vec1( tmp1 );
		Array<double> vec2( tmp2 );
		double result = math::scalarProduct( vec1, vec2 );
		UT_ASSERT_EQUAL(result, 16);
	}
	void vectorSumTest()
	{
		int tmp1[] = { 1,2,666 };
		int tmp2[] = { 0,2 };
		Array<int> vec1( tmp1 );
		Array<int> vec2( tmp2 );
		Array<int> result1 = math::vectorSum( vec1, vec2 );
		UT_ASSERT_EQUAL(result1[0], 1);
		UT_ASSERT_EQUAL(result1[1], 4);
		UT_ASSERT_EQUAL(result1[2], 666);
		Array<int> result2 = math::vectorSum( vec2, vec1 );
		UT_ASSERT_EQUAL(result1[0], result2[0]);
		UT_ASSERT_EQUAL(result1[1], result2[1]);
		UT_ASSERT_EQUAL(result1[2], result2[2]);
	}

	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "MathTest::PerformTest");
		TestScope scope( "PerformTest" );

		getExponentTest();
		normalizeTest();
		minMaxMeanTest();
		scalarProductTest();
		vectorSumTest();

		AllGeoTests();
	}
};

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

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static MathTest	myMathTest;

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

