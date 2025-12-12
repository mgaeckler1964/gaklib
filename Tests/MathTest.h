/*
		Project:		GAKLIB
		Module:			MathTest.h
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
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "MathTest::PerformTest");
		TestScope scope( "PerformTest" );

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

