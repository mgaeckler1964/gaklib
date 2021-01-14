/*
		Project:		GAKLIB
		Module:			OptionalTest.h
		Description:	
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
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

class OptionalTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "OptionalTest";
	}
	virtual void PerformTest( void )
	{
		{
			Optional<int>	optInt;

			UT_ASSERT_FALSE( optInt );
			UT_ASSERT_TRUE( !optInt );
			UT_ASSERT_FALSE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_ASSERT_EXCEPTION( optInt.get(), OptionalError );

			optInt = 0;

			UT_ASSERT_TRUE( optInt );
			UT_ASSERT_FALSE( !optInt );
			UT_ASSERT_TRUE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.get(), 0 );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			const Optional<int>	optInt;

			UT_ASSERT_FALSE( optInt );
			UT_ASSERT_TRUE( !optInt );
			UT_ASSERT_FALSE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_ASSERT_EXCEPTION( optInt.get(), OptionalError );
		}
		{
			Optional<int>	optInt(0);

			UT_ASSERT_TRUE( optInt );
			UT_ASSERT_FALSE( !optInt );
			UT_ASSERT_TRUE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.get(), 0 );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			const Optional<int>	optInt(0);

			UT_ASSERT_TRUE( optInt );
			UT_ASSERT_FALSE( !optInt );
			UT_ASSERT_TRUE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.get(), 0 );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			Optional<int>	optSrc(0);
			Optional<int>	optInt(optSrc);

			UT_ASSERT_TRUE( optInt );
			UT_ASSERT_FALSE( !optInt );
			UT_ASSERT_TRUE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.get(), 0 );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 0 );
		}
		{
			Optional<int>	optInt;

			UT_ASSERT_FALSE( optInt );
			UT_ASSERT_TRUE( !optInt );
			UT_ASSERT_FALSE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_ASSERT_EXCEPTION( optInt.get(), OptionalError );

			optInt = Optional<int>::of( 0 );

			UT_ASSERT_TRUE( optInt );
			UT_ASSERT_FALSE( !optInt );
			UT_ASSERT_TRUE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.get(), 0 );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 0 );

			optInt = Optional<int>();

			UT_ASSERT_FALSE( optInt );
			UT_ASSERT_TRUE( !optInt );
			UT_ASSERT_FALSE( optInt.isPresent() );
			UT_ASSERT_EQUAL( optInt.orElse( 1 ), 1 );
			UT_ASSERT_EXCEPTION( optInt.get(), OptionalError );
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

