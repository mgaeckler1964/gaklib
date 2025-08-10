/*
		Project:		GAKLIB
		Module:			FractionTest.h
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

class FractionTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "FractionTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "FractionTest::PerformTest");
		TestScope scope( "PerformTest" );

		math::MixedFraction n1( 2, 1, 2 );
		math::MixedFraction n2( 5, 1, 4 );

		math::MixedFraction n3 = n1 - n2;
		UT_ASSERT_EQUAL( n3.integer, -2L );
		UT_ASSERT_EQUAL( n3.fraction.numerator, -3L );
		UT_ASSERT_EQUAL( n3.fraction.denominator, 4UL );
		UT_ASSERT_EQUAL( double(n3), -2.75 );

		math::MixedFraction n4( 0, -1, 2 );
		math::MixedFraction n5 = n3 * n4;
		UT_ASSERT_EQUAL( n5.integer, 1L );
		UT_ASSERT_EQUAL( n5.fraction.numerator, 3L );
		UT_ASSERT_EQUAL( n5.fraction.denominator, 8UL );

		math::MixedFraction n6 = n1 + n2;
		UT_ASSERT_EQUAL( double(n6), 7.75 );

		math::MixedFraction n7 = n1 / n2;
		math::MixedFraction n8(0,10,21);
		UT_ASSERT_EQUAL( n7, n8 );

		math::MixedFraction n9 = n2 / n1;
		math::MixedFraction n10(2,1,10);
		UT_ASSERT_EQUAL( n9, n10 );

		math::MixedFraction	n11 = n2 * n1.reziprokCopy();
		UT_ASSERT_EQUAL( n11, n10 );

		math::MixedFraction	n12 = n10.reziprokCopy();
		math::MixedFraction	n13 = n12.reziprokCopy();
		UT_ASSERT_EQUAL( n13, n10 );

		math::MixedFraction	n14 = math::MixedFraction( 5 );
		UT_ASSERT_EQUAL( n14.integer, 5L );
		UT_ASSERT_EQUAL( n14.fraction.numerator, 0L );
		UT_ASSERT_EQUAL( n14.fraction.denominator, 1UL );

		math::MixedFraction	n15 = math::MixedFraction( 1, 10 );
		UT_ASSERT_EQUAL( n15.integer, 0L );
		UT_ASSERT_EQUAL( n15.fraction.numerator, 1L );
		UT_ASSERT_EQUAL( n15.fraction.denominator, 10UL );

		math::Fraction	f3 = n3;
		UT_ASSERT_EQUAL( f3.numerator, -11L );
		UT_ASSERT_EQUAL( f3.denominator, 4UL );

		math::MixedFraction	n33 = f3;

		UT_ASSERT_EQUAL( n33, n3 );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FractionTest myFractionTest;

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

