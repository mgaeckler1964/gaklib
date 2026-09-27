/*
		Project:		GAKLIB
		Module:			OptionalTest.h
		Description:	Optional values like std::Optional
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

#include <gak/optional.h>

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

enum UsedConstructor
{
	ucEmpty, ucOneParam, ucTwoParam, ucThreeParam
};

struct MyValue
{
	UsedConstructor	constUsed;
	int m_dummy1;
	double m_dummy2;
	const char *m_dummy3;
	MyValue() : constUsed(ucEmpty) {}
	MyValue( int dummy1 ) : constUsed(ucOneParam), m_dummy1(dummy1) {}
	MyValue( int dummy1, double dummy2 ) : constUsed(ucTwoParam), m_dummy1(dummy1), m_dummy2(dummy2) {}
	MyValue( int dummy1, double dummy2, const char *dummy3 ) : constUsed(ucThreeParam), m_dummy1(dummy1), m_dummy2(dummy2), m_dummy3(dummy3) {}
};

class OptionalTest : public UnitTest
{
	const char *GetClassName() const override
	{
		return "OptionalTest";
	}
	void PerformTest() override
	{
		doEnterFunctionEx(gakLogging::llInfo, "OptionalTest::PerformTest");
		TestScope scope( "PerformTest" );

		{
			Optional<int>	optInt;

			UT_EXPECT_FALSE( optInt );
			UT_EXPECT_TRUE( !optInt );
			UT_EXPECT_FALSE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_EXPECT_EXCEPTION( optInt.get(), OptionalError );

			optInt = 0;

			UT_EXPECT_TRUE( optInt );
			UT_EXPECT_FALSE( !optInt );
			UT_EXPECT_TRUE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.get(), 0 );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			const Optional<int>	optInt;

			UT_EXPECT_FALSE( optInt );
			UT_EXPECT_TRUE( !optInt );
			UT_EXPECT_FALSE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_EXPECT_EXCEPTION( optInt.get(), OptionalError );
		}
		{
			Optional<int>	optInt(0);

			UT_EXPECT_TRUE( optInt );
			UT_EXPECT_FALSE( !optInt );
			UT_EXPECT_TRUE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.get(), 0 );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			const Optional<int>	optInt(0);

			UT_EXPECT_TRUE( optInt );
			UT_EXPECT_FALSE( !optInt );
			UT_EXPECT_TRUE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.get(), 0 );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			Optional<int>	optSrc(0);
			Optional<int>	optInt(optSrc);

			UT_EXPECT_TRUE( optInt );
			UT_EXPECT_FALSE( !optInt );
			UT_EXPECT_TRUE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.get(), 0 );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			Optional<int>	optInt;

			UT_EXPECT_FALSE( optInt );
			UT_EXPECT_TRUE( !optInt );
			UT_EXPECT_FALSE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_EXPECT_EXCEPTION( optInt.get(), OptionalError );

			optInt = Optional<int>::of( 0 );

			UT_EXPECT_TRUE( optInt );
			UT_EXPECT_FALSE( !optInt );
			UT_EXPECT_TRUE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.get(), 0 );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 0 );

			optInt = Optional<int>();

			UT_EXPECT_FALSE( optInt );
			UT_EXPECT_TRUE( !optInt );
			UT_EXPECT_FALSE( optInt.isPresent() );
			UT_EXPECT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_EXPECT_EXCEPTION( optInt.get(), OptionalError );
		}
		{
			{
				Optional<MyValue>	optVal;

				optVal.create();
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucEmpty );

				optVal.create( 666 );
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucOneParam );
				UT_ASSERT_EQUAL( optVal.get().m_dummy1, 666 );

				optVal.create();
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucEmpty );
			}
			{
				Optional<MyValue>	optVal;

				optVal.create( 666 );
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucOneParam );
				UT_ASSERT_EQUAL( optVal.get().m_dummy1, 666 );
			}

			{
				Optional<MyValue>	optVal;

				optVal.create( 666, 999.0 );
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucTwoParam );
				UT_ASSERT_EQUAL( optVal.get().m_dummy1, 666 );
				UT_ASSERT_EQUAL( optVal.get().m_dummy2, 999.0 );

				optVal.create();
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucEmpty );

				optVal.create( 666, 999.0 );
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucTwoParam );
				UT_ASSERT_EQUAL( optVal.get().m_dummy1, 666 );
				UT_ASSERT_EQUAL( optVal.get().m_dummy2, 999.0 );
			}
			{
				Optional<MyValue>	optVal;

				optVal.create( 666, 999.0, (const char *)"dummy" );
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucThreeParam );
				UT_ASSERT_EQUAL( optVal.get().m_dummy1, 666 );
				UT_ASSERT_EQUAL( optVal.get().m_dummy2, 999.0 );
				UT_ASSERT_EQUAL( optVal.get().m_dummy3, (const char *)"dummy" );

				optVal.create();
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucEmpty );

				optVal.create( 666, 999.0, (const char *)"dummy" );
				UT_EXPECT_TRUE( optVal );
				UT_ASSERT_EQUAL( optVal.get().constUsed, ucThreeParam );
				UT_ASSERT_EQUAL( optVal.get().m_dummy1, 666 );
				UT_ASSERT_EQUAL( optVal.get().m_dummy2, 999.0 );
				UT_ASSERT_EQUAL( optVal.get().m_dummy3, (const char *)"dummy" );
			}
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static OptionalTest	myOptionalTest;

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

