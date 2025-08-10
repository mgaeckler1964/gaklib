/*
		Project:		GAKLIB
		Module:			FmtNumberTest.h
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

#include <gak/fmtNumber.h>

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

class FmtNumberTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "FmtNumberTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "FmtNumberTest::PerformTest");
		TestScope scope( "PerformTest" );

		UT_ASSERT_EQUAL( STRING("999.999990000000025"), formatNumber( 999.999990000000025 ) );
		UT_ASSERT_EQUAL( STRING("99.999999000000003"), formatNumber( 99.999999000000003 ) );
		UT_ASSERT_EQUAL( STRING("9.999999900000001"), formatNumber( 9.999999900000001 ) );
		UT_ASSERT_EQUAL( STRING("0.9999999"), formatNumber( 0.9999999 ) );
		UT_ASSERT_EQUAL( STRING("0.09999999"), formatNumber( 0.09999999 ) );
		UT_ASSERT_EQUAL( STRING("0.009999999"), formatNumber( 0.009999999 ) );

		UT_ASSERT_EQUAL( STRING("-999.999990000000025"), formatNumber( -999.999990000000025 ) );
		UT_ASSERT_EQUAL( STRING("-0.009999999"), formatNumber( -0.009999999 ) );

		UT_ASSERT_EQUAL( STRING("  0.05"), formatNumber( 0.05, 6 ) );
		UT_ASSERT_EQUAL( STRING(" -0.05"), formatNumber( -0.05, 6 ) );
		UT_ASSERT_EQUAL( STRING("-0.005"), formatNumber( -0.005, 6 ) );
		UT_ASSERT_EQUAL( STRING("-0.0005"), formatNumber( -0.0005, 6 ) );

		UT_ASSERT_EQUAL( STRING("9999"), formatNumber( 9999 ) );
		UT_ASSERT_EQUAL( STRING("-9999"), formatNumber( -9999 ) );
		UT_ASSERT_EQUAL( STRING("009999"), formatNumber( 9999, 6 ) );
		UT_ASSERT_EQUAL( STRING("-09999"), formatNumber( -9999, 6 ) );
		UT_ASSERT_EQUAL( STRING("  9999"), formatNumber( 9999, 6, ' ' ) );
		UT_ASSERT_EQUAL( STRING(" -9999"), formatNumber( -9999, 6, ' ' ) );

		UT_ASSERT_EQUAL( STRING("   9 999"), formatNumber( 9999, 8, ' ', ' ' ) );
		UT_ASSERT_EQUAL( STRING("  -9 999"), formatNumber( -9999, 8, ' ', ' ' ) );

		UT_ASSERT_EQUAL( STRING( "9 999"), formatNumber(  9999, 3, ' ', ' ' ) );
		UT_ASSERT_EQUAL( STRING("-9 999"), formatNumber( -9999, 3, ' ', ' ' ) );

		UT_ASSERT_EQUAL( STRING("999.999990000000025"), formatFloat( 999.999990000000025 ) );
		UT_ASSERT_EQUAL( STRING("99.999999000000003"), formatFloat( 99.999999000000003 ) );
		UT_ASSERT_EQUAL( STRING("9.999999900000001"), formatFloat( 9.999999900000001 ) );
		UT_ASSERT_EQUAL( STRING("0.9999999"), formatFloat( 0.9999999 ) );
		UT_ASSERT_EQUAL( STRING("0.09999999"), formatFloat( 0.09999999 ) );
		UT_ASSERT_EQUAL( STRING("0.009999999"), formatFloat( 0.009999999 ) );

		UT_ASSERT_EQUAL( STRING("-999.999990000000025"), formatFloat( -999.999990000000025 ) );
		UT_ASSERT_EQUAL( STRING("-0.009999999"), formatFloat( -0.009999999 ) );

		UT_ASSERT_EQUAL( STRING("   99.99"), formatFloat( 99.99, 8, 2 ) );
		UT_ASSERT_EQUAL( STRING("  -99.99"), formatFloat( -99.99, 8, 2 ) );

		UT_ASSERT_EQUAL( STRING("   100.0"), formatFloat( 99.99, 8, 1 ) );
		UT_ASSERT_EQUAL( STRING("  -100.0"), formatFloat( -99.99, 8, 1 ) );

		UT_ASSERT_EQUAL( STRING("     100"), formatFloat( 99.99, 8, 0 ) );
		UT_ASSERT_EQUAL( STRING("    -100"), formatFloat( -99.99, 8, 0 ) );

		UT_ASSERT_EQUAL( STRING("   0.005"), formatFloat(  0.005, 8, 3 ) );
		UT_ASSERT_EQUAL( STRING("  -0.005"), formatFloat( -0.005, 8, 3 ) );
		UT_ASSERT_EQUAL( STRING("   0.00500"), formatFloat(  0.005, 10, 5 ) );
		UT_ASSERT_EQUAL( STRING("  -0.00500"), formatFloat( -0.005, 10, 5 ) );

		UT_ASSERT_EQUAL( STRING("    9 999.9900"), formatFloat( 9999.99, 14, 4, ' ' ) );
		UT_ASSERT_EQUAL( STRING("   -9 999.9900"), formatFloat( -9999.99, 14, 4, ' ' ) );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FmtNumberTest myFmtNumberTest;

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

