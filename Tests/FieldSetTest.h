/*
		Project:		GAKLIB
		Module:			FieldSetTest.h
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

#include <gak/fieldSet.h>
#include <gak/stringStream.h>

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

class FieldSetTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "FieldSetTest";
	}
	template <typename T>
	void testDynamicIO( const T &value )
	{
		ArrayOfData	buffer;
		DynamicVar	dynamic( value );
		oBinaryStream	ostream( buffer );
		dynamic.toBinaryStream( ostream );
		ostream.flush();
		
		DynamicVar dest;
		iBinaryStream	istream( buffer );
		dest.fromBinaryStream( istream );

		UT_ASSERT_EQUAL( dynamic, dest );
	}
	void testDynamic()
	{
		DynamicVar	piv = 3.14;
		double		pid = piv;
		STRING		pis = piv;

		UT_ASSERT_EQUAL( pis, STRING("3.14") );
		UT_ASSERT_EQUAL( pid, 3.14 );

	}
	virtual void PerformTest( void )
	{
		testDynamic();
		FieldSet	fSet;

		fSet["integer"] = 2;
		fSet["string"] = "Hello World";
		fSet["float"] = 3.14;
		fSet["date"] = gak::Date();
		fSet["time"] = gak::Time();

		UT_ASSERT_EQUAL( fSet["integer"].getType(), DynamicVar::DV_INTEGER );
		UT_ASSERT_EQUAL( fSet["string"].getType(), DynamicVar::DV_VARCHAR );
		UT_ASSERT_EQUAL( fSet["float"].getType(), DynamicVar::DV_FLOAT );
		UT_ASSERT_EQUAL( fSet["date"].getType(), DynamicVar::DV_DATE );
		UT_ASSERT_EQUAL( fSet["time"].getType(), DynamicVar::DV_TIME );
		STRING	hello = fSet["string"];
		UT_ASSERT_EQUAL( hello, STRING("Hello World") );
		STRING	pi = fSet["float"];
		UT_ASSERT_EQUAL( pi, STRING("3.14") );

		double result = fSet["integer"] + fSet["float"];
		UT_ASSERT_EQUAL( result, 5.14 );
		result = fSet["integer"] * fSet["float"];
		UT_ASSERT_EQUAL( result, 6.28 );
		result = fSet["integer"] - fSet["float"];
		UT_ASSERT_EQUAL( result, -1.14 );
		result = fSet["float"] / fSet["integer"];
		UT_ASSERT_EQUAL( result, 1.57 );

		double oper1 = 0.1, oper2 = 0.2, oper3 = long(oper1) && long(oper2);
		std::cout << oper3;

		{
			TestScope	scope( "STRING" );
			testDynamicIO( "Hello world" );
		}
		{
			TestScope	scope( "float" );
			testDynamicIO( 3.14 );
		}
		{
			TestScope	scope( "int" );
			testDynamicIO( -3 );
		}
		{
			TestScope	scope( "Date" );
			testDynamicIO( gak::Date() );
		}
		{
			TestScope	scope( "Time" );
			testDynamicIO( gak::Time() );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FieldSetTest myFieldSetTest;

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

