/*
		Project:		GAKLIB
		Module:			BitFieldTest.h
		Description:	A bitfield with up to 32 bits
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

#include <gak/bitfield.h>

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

class BitFieldTest : public UnitTest
{
	const char *GetClassName() const override
	{
		return "BitFieldTest";
	}
	typedef enum
	{
		TST_BIT1, TST_BIT2, TST_BIT3, TST_BIT4,
		TST_BIT5, TST_BIT6, TST_BIT7, TST_BIT8,
	} TST_BITS;

	void PerformTest() override
	{
		doEnterFunctionEx(gakLogging::llInfo, "BitFieldTest::PerformTest");
		TestScope scope( "PerformTest" );

		Bitfield x, x2, x3;

		// setting
		UT_EXPECT_FALSE( x.test( TST_BIT5 ) );
		UT_EXPECT_FALSE( x.test( TST_BIT8 ) );
		x.set( TST_BIT5 );
		UT_EXPECT_TRUE( x.test( TST_BIT5 ) );
		UT_EXPECT_FALSE( x.test( TST_BIT8 ) );
		x <<= TST_BIT8;
		UT_EXPECT_TRUE( x.test( TST_BIT5 ) );
		UT_EXPECT_TRUE( x.test( TST_BIT8 ) );

		// setting
		x.clear( TST_BIT5 );
		UT_EXPECT_FALSE( x.test( TST_BIT5 ) );
		UT_EXPECT_TRUE( x.test( TST_BIT8 ) );
		x >>= TST_BIT8;
		UT_EXPECT_FALSE( x.test( TST_BIT5 ) );
		UT_EXPECT_FALSE( x.test( TST_BIT8 ) );

		// OR
		x.set( TST_BIT5 );
		x2.set( TST_BIT8 );
		x3 = x | x2;
		std::cout << "\nOR\n" << x << '\n' << x2 << '\n' << x3 << std::endl;
		UT_EXPECT_TRUE( x3.test( TST_BIT5 ) );
		UT_EXPECT_TRUE( x3.test( TST_BIT8 ) );

		// AND
		x.set( TST_BIT8 );
		x3 = x & x2;
		std::cout << "\nAND\n" << x << '\n' << x2 << '\n' << x3 << std::endl;
		UT_EXPECT_FALSE( x3.test( TST_BIT5 ) );
		UT_EXPECT_TRUE( x3.test( TST_BIT8 ) );

		// XOR
		x.set( TST_BIT8 );
		x2.set( TST_BIT1 );
		x3 = x ^ x2;
		std::cout << "\nXOR\n" << x << '\n' << x2 << '\n' << x3 << std::endl;
		UT_EXPECT_TRUE( x3.test( TST_BIT5 ) );
		UT_EXPECT_FALSE( x3.test( TST_BIT8 ) );
		UT_EXPECT_TRUE( x3.test( TST_BIT1 ) );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static BitFieldTest myBitFieldTest;

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

