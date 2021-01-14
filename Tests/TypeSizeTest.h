/*
		Project:		GAKLIB
		Module:			TypeSizeTest.h
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

#include <gak/type_traits.h>
#include <gak/array.h>

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

class TypeSizeTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "TypeSizeTest";
	}
	virtual void PerformTest( void )
	{
		UT_ASSERT_EQUAL( sizeof( uint8 ), size_t(1) );
		UT_ASSERT_EQUAL( sizeof( int8 ), size_t(1) );

		UT_ASSERT_EQUAL( sizeof( uint16 ), size_t(2) );
		UT_ASSERT_EQUAL( sizeof( int16 ), size_t(2) );

		UT_ASSERT_EQUAL( sizeof( uint32 ), size_t(4) );
		UT_ASSERT_EQUAL( sizeof( int32 ), size_t(4) );

		UT_ASSERT_EQUAL( sizeof( uint64 ), size_t(8) );
		UT_ASSERT_EQUAL( sizeof( int64 ), size_t(8) );

		UT_ASSERT_EQUAL( sizeof( ssize_t ), sizeof( std::size_t ) );

		UT_ASSERT_TRUE( is_float<double>::value );
		UT_ASSERT_TRUE( is_float<float>::value );
		UT_ASSERT_FALSE( is_float<int>::value );
		UT_ASSERT_FALSE( is_float<bool>::value );
		UT_ASSERT_FALSE( is_float< Array<int> >::value );

		UT_ASSERT_TRUE( is_float<const float>::value );
		UT_ASSERT_TRUE( is_float<const double>::value );
		UT_ASSERT_FALSE( is_float<const int>::value );
		UT_ASSERT_FALSE( is_float<const bool>::value );
		UT_ASSERT_FALSE( is_float< const Array<int> >::value );

		UT_ASSERT_FALSE( is_integral<double>::value );
		UT_ASSERT_FALSE( is_integral<float>::value );
		UT_ASSERT_TRUE( is_integral<int>::value );
		UT_ASSERT_TRUE( is_integral<bool>::value );
		UT_ASSERT_FALSE( is_integral< Array<int> >::value );

		UT_ASSERT_FALSE( is_integral<const double>::value );
		UT_ASSERT_FALSE( is_integral<const float>::value );
		UT_ASSERT_TRUE( is_integral<const int>::value );
		UT_ASSERT_TRUE( is_integral<const bool>::value );
		UT_ASSERT_FALSE( is_integral< const Array<int> >::value );

		UT_ASSERT_TRUE( is_arithmetic<double>::value );
		UT_ASSERT_TRUE( is_arithmetic<float>::value );
		UT_ASSERT_TRUE( is_arithmetic<int>::value );
		UT_ASSERT_TRUE( is_arithmetic<bool>::value );
		UT_ASSERT_FALSE( is_arithmetic< Array<int> >::value );

		UT_ASSERT_TRUE( is_arithmetic<const double>::value );
		UT_ASSERT_TRUE( is_arithmetic<const float>::value );
		UT_ASSERT_TRUE( is_arithmetic<const int>::value );
		UT_ASSERT_TRUE( is_arithmetic<const bool>::value );
		UT_ASSERT_FALSE( is_arithmetic< const Array<int> >::value );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static TypeSizeTest myTypeSizeTest;

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

