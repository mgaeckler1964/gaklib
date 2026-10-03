/*
		Project:		GAKLIB
		Module:			StringBufferTest.h
		Description:	A small buffer on stack used to construct strings
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

#include <gak/StringBuffer.h>

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

class StringBufferTest : public UnitTest
{
	const char *GetClassName() const override
	{
		return "StringBufferTest";
	}
	void PerformTest() override
	{
		doEnterFunctionEx(gakLogging::llInfo, "StringBufferTest::PerformTest");
		TestScope scope( "PerformTest" );

		StringBuffer<5>	tmpBuffer;
		const char *cd = "cd";

		UT_EXPECT_EQUAL(tmpBuffer.size(), 0);
		tmpBuffer += "ab";
		UT_EXPECT_EQUAL(tmpBuffer.size(), 2);
		tmpBuffer.addCP(cd);
		UT_EXPECT_EQUAL(tmpBuffer.size(), 4);
		tmpBuffer += 'e';
		UT_EXPECT_EQUAL(tmpBuffer.size(), 5);
		UT_EXPECT_EQUAL(tmpBuffer.c_str(), (const char *)"abcde");

		UT_EXPECT_EXCEPTION( tmpBuffer += 'f', IndexError );
		UT_EXPECT_EXCEPTION( tmpBuffer += "f", IndexError );
		UT_EXPECT_EXCEPTION( tmpBuffer.addCP(cd), IndexError );

		tmpBuffer.clear();
		UT_EXPECT_EQUAL(*tmpBuffer.c_str(), 0);

		tmpBuffer.addNumber( 1005, 5, '0', ' ' );
		UT_EXPECT_EQUAL(tmpBuffer.c_str(), (const char *)"1 005");

		StringBuffer<128>	largeBuffer;
		UT_EXPECT_EQUAL( formatFloatFast( &largeBuffer, 666.125 ), (const char *)"666.125" );

		UT_EXPECT_EXCEPTION( formatNumberFast( &tmpBuffer, -12345 ), IndexError );
		UT_EXPECT_EXCEPTION( formatFloatFast( &tmpBuffer, 666, 6 ), IndexError );
		UT_EXPECT_EXCEPTION( formatFloatFast( &tmpBuffer, 666, 0, 3), IndexError );
		UT_EXPECT_EXCEPTION( formatFloatFast( &tmpBuffer, 666, 0, 6), IndexError );

		UT_EXPECT_EQUAL(tmpBuffer.clear().insDigit( '0', 0, 5 ).c_str(), (const char *)"00000");
		UT_EXPECT_EXCEPTION(tmpBuffer.clear().insDigit( '0', 0, 6 ).c_str(), IndexError);	// too large
		UT_EXPECT_EXCEPTION(tmpBuffer.clear().insDigit( '0', 3, 1 ).c_str(), IndexError);	// beyond end

		STRING tmp = StringBuffer<128>().addCP("Hello").addFloat( 666666.125, 12, 3, '.', ',' ).c_str();
		UT_EXPECT_EQUAL(tmp, "Hello 666.666,125");

		tmp = StringBuffer<128>().addCP("Hello").addFloat( -666666.125, 12, 3, '.', ',' ).c_str();
		UT_EXPECT_EQUAL(tmp, "Hello-666.666,125");
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static StringBufferTest myStringBufferTest;

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
