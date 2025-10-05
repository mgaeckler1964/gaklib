/*
		Project:		GAKLIB
		Module:			StringTest.h
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

#include <gak/ci_string.h>
#include <gak/t_string.h>

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

class StringTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "StringTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "StringTest::PerformTest");
		TestScope scope( "PerformTest" );

		UT_ASSERT_TRUE( checkData() );

		ConstructorTest();
		ModifyingTests();
		SizeTests();
		ConversionTests();
		ComparingTests();
		CiComparingTests();
		OtherTests();

		FileTests();
	}
	void ComparingTests()
	{
		STRING	testLower = "hello world" LOWER_UMLAUT;
		STRING	testUpper = "HELLO WORLD" UPPER_UMLAUT;

		// isNull, isEmpty and operator ! are tested during constructor test

		UT_ASSERT_EQUAL( 0, testLower.compare( "hello world" LOWER_UMLAUT ) );
		UT_ASSERT_GREATER( 0, testLower.compare( "zz top" ) );
		UT_ASSERT_LESS( 0, testLower.compare( "abba" ) );

		UT_ASSERT_EQUAL( 0, testLower.compare( STRING("hello world" LOWER_UMLAUT) ) );
		UT_ASSERT_GREATER( 0, testLower.compare( STRING("zz top") ) );
		UT_ASSERT_LESS( 0, testLower.compare( STRING("abba") ) );

		UT_ASSERT_EQUAL( 0, testLower.compareI( "HELLO WORLD" UPPER_UMLAUT) );
		UT_ASSERT_GREATER( 0, testUpper.compareI( "zz zop" ) );
		UT_ASSERT_LESS( 0, testLower.compareI( "ABBA" ) );

		UT_ASSERT_EQUAL( 0, testLower.compareI( testUpper ) );
		UT_ASSERT_GREATER( 0, testUpper.compareI( STRING("zz zop") ) );
		UT_ASSERT_LESS( 0, testLower.compareI( STRING("ABBA") ) );

		UT_ASSERT_TRUE( testLower != "abba" );
		UT_ASSERT_FALSE( testLower != "hello world" LOWER_UMLAUT);
		UT_ASSERT_TRUE( testLower != STRING("abba") );
		UT_ASSERT_FALSE( testLower != STRING("hello world" LOWER_UMLAUT) );

		UT_ASSERT_FALSE( testLower == "abba" );
		UT_ASSERT_TRUE( testLower == "hello world" LOWER_UMLAUT);
		UT_ASSERT_FALSE( testLower == STRING("abba") );
		UT_ASSERT_TRUE( testLower == STRING("hello world" LOWER_UMLAUT) );

		UT_ASSERT_TRUE( testLower <= "hello world" LOWER_UMLAUT );
		UT_ASSERT_TRUE( testLower >= "hello world" LOWER_UMLAUT );
		UT_ASSERT_FALSE( testLower < "hello world" LOWER_UMLAUT );
		UT_ASSERT_FALSE( testLower > "hello world" LOWER_UMLAUT );

		UT_ASSERT_TRUE( testLower <= "zz top" );
		UT_ASSERT_FALSE( testLower >= "zz top" );
		UT_ASSERT_TRUE( testLower < "zz top" );
		UT_ASSERT_FALSE( testLower > "zz top" );

		UT_ASSERT_FALSE( testLower <= "abba" );
		UT_ASSERT_TRUE( testLower >= "abba" );
		UT_ASSERT_FALSE( testLower < "abba" );
		UT_ASSERT_TRUE( testLower > "abba" );

		UT_ASSERT_TRUE( testLower <= STRING("hello world" LOWER_UMLAUT ) );
		UT_ASSERT_TRUE( testLower >= STRING("hello world" LOWER_UMLAUT ) );
		UT_ASSERT_FALSE( testLower < STRING("hello world" LOWER_UMLAUT ) );
		UT_ASSERT_FALSE( testLower > STRING("hello world" LOWER_UMLAUT ) );

		UT_ASSERT_TRUE( testLower <= STRING("zz top") );
		UT_ASSERT_FALSE( testLower >= STRING("zz top") );
		UT_ASSERT_TRUE( testLower < STRING("zz top") );
		UT_ASSERT_FALSE( testLower > STRING("zz top") );

		UT_ASSERT_FALSE( testLower <= STRING("abba") );
		UT_ASSERT_TRUE( testLower >= STRING("abba") );
		UT_ASSERT_FALSE( testLower < STRING("abba") );
		UT_ASSERT_TRUE( testLower > STRING("abba") );

		UT_ASSERT_LESS( STRING("äöü"), STRING("bcd") );
		UT_ASSERT_GREATER( STRING("äöü"), STRING("abc") );
		UT_ASSERT_LESS( STRING("Äöü"), STRING("abc") );

		UT_ASSERT_LESS( STRING( "abc" ), STRING( "abcd" ) );
	}
	void CiComparingTests()
	{
		CI_STRING	testLower = "hello world" LOWER_UMLAUT;
		CI_STRING	testUpper = "HELLO WORLD" UPPER_UMLAUT;

		// isNull, isEmpty and operator ! are tested during constructor test

		UT_ASSERT_EQUAL( 0, testLower.compare( "hello world" LOWER_UMLAUT ) );
		UT_ASSERT_GREATER( 0, testLower.compare( "zz top" ) );
		UT_ASSERT_LESS( 0, testLower.compare( "abba" ) );

		UT_ASSERT_EQUAL( 0, testLower.compare( "HELLO WORLD" UPPER_UMLAUT ) );
		UT_ASSERT_GREATER( 0, testLower.compare( "ZZ TOP" ) );
		UT_ASSERT_LESS( 0, testLower.compare( "ABBA" ) );

		UT_ASSERT_EQUAL( 0, testLower.compare( STRING("hello world" LOWER_UMLAUT) ) );
		UT_ASSERT_GREATER( 0, testLower.compare( STRING("zz top") ) );
		UT_ASSERT_GREATER( 0, testLower.compare( STRING("zz top") ) );

		UT_ASSERT_EQUAL( 0, testLower.compare( STRING("HELLO WORLD" UPPER_UMLAUT) ) );
		UT_ASSERT_GREATER( 0, testLower.compare( STRING("ZZ TOP") ) );
		UT_ASSERT_GREATER( 0, testLower.compare( STRING("ZZ TOP") ) );

		UT_ASSERT_TRUE( testLower != "abba" );
		UT_ASSERT_FALSE( testLower != "hello world" LOWER_UMLAUT);
		UT_ASSERT_TRUE( testLower != STRING("abba") );
		UT_ASSERT_FALSE( testLower != STRING("hello world" LOWER_UMLAUT) );

		UT_ASSERT_TRUE( testLower != "ABBA" );
		UT_ASSERT_FALSE( testLower != "HELLO WORLD" UPPER_UMLAUT);
		UT_ASSERT_TRUE( testLower != STRING("ABBA") );
		UT_ASSERT_FALSE( testLower != STRING("HELLO WORLD" UPPER_UMLAUT) );

		UT_ASSERT_FALSE( testLower == "abba" );
		UT_ASSERT_TRUE( testLower == "hello world" LOWER_UMLAUT);
		UT_ASSERT_FALSE( testLower == STRING("abba") );
		UT_ASSERT_TRUE( testLower == STRING("hello world" LOWER_UMLAUT) );

		UT_ASSERT_FALSE( testLower == "ABBA" );
		UT_ASSERT_TRUE( testLower == "HELLO WORLD" LOWER_UMLAUT);
		UT_ASSERT_FALSE( testLower == STRING("ABBA") );
		UT_ASSERT_TRUE( testLower == STRING("HELLO WORLD" LOWER_UMLAUT) );

		UT_ASSERT_TRUE( testLower <= "hello world" LOWER_UMLAUT );
		UT_ASSERT_TRUE( testLower >= "hello world" LOWER_UMLAUT );
		UT_ASSERT_FALSE( testLower < "hello world" LOWER_UMLAUT );
		UT_ASSERT_FALSE( testLower > "hello world" LOWER_UMLAUT );

		UT_ASSERT_TRUE( testLower <= "HELLO WORLD" LOWER_UMLAUT );
		UT_ASSERT_TRUE( testLower >= "HELLO WORLD" LOWER_UMLAUT );
		UT_ASSERT_FALSE( testLower < "HELLO WORLD" LOWER_UMLAUT );
		UT_ASSERT_FALSE( testLower > "HELLO WORLD" LOWER_UMLAUT );

		UT_ASSERT_TRUE( testLower <= "zz top" );
		UT_ASSERT_FALSE( testLower >= "zz top" );
		UT_ASSERT_TRUE( testLower < "zz top" );
		UT_ASSERT_FALSE( testLower > "zz top" );

		UT_ASSERT_TRUE( testLower <= "ZZ TOP" );
		UT_ASSERT_FALSE( testLower >= "ZZ TOP" );
		UT_ASSERT_TRUE( testLower < "ZZ TOP" );
		UT_ASSERT_FALSE( testLower > "ZZ TOP" );

		UT_ASSERT_FALSE( testLower <= "abba" );
		UT_ASSERT_TRUE( testLower >= "abba" );
		UT_ASSERT_FALSE( testLower < "abba" );
		UT_ASSERT_TRUE( testLower > "abba" );

		UT_ASSERT_FALSE( testLower <= "ABBA" );
		UT_ASSERT_TRUE( testLower >= "ABBA" );
		UT_ASSERT_FALSE( testLower < "ABBA" );
		UT_ASSERT_TRUE( testLower > "ABBA" );

		UT_ASSERT_TRUE( testLower <= STRING("hello world" LOWER_UMLAUT ) );
		UT_ASSERT_TRUE( testLower >= STRING("hello world" LOWER_UMLAUT ) );
		UT_ASSERT_FALSE( testLower < STRING("hello world" LOWER_UMLAUT ) );
		UT_ASSERT_FALSE( testLower > STRING("hello world" LOWER_UMLAUT ) );

		UT_ASSERT_TRUE( testLower <= STRING("HELLO WORLD" LOWER_UMLAUT ) );
		UT_ASSERT_TRUE( testLower >= STRING("HELLO WORLD" LOWER_UMLAUT ) );
		UT_ASSERT_FALSE( testLower < STRING("HELLO WORLD" LOWER_UMLAUT ) );
		UT_ASSERT_FALSE( testLower > STRING("HELLO WORLD" LOWER_UMLAUT ) );

		UT_ASSERT_TRUE( testLower <= STRING("zz top") );
		UT_ASSERT_FALSE( testLower >= STRING("zz top") );
		UT_ASSERT_TRUE( testLower < STRING("zz top") );
		UT_ASSERT_FALSE( testLower > STRING("zz top") );

		UT_ASSERT_TRUE( testLower <= STRING("ZZ TOP") );
		UT_ASSERT_FALSE( testLower >= STRING("ZZ TOP") );
		UT_ASSERT_TRUE( testLower < STRING("ZZ TOP") );
		UT_ASSERT_FALSE( testLower > STRING("ZZ TOP") );

		UT_ASSERT_FALSE( testLower <= STRING("abba") );
		UT_ASSERT_TRUE( testLower >= STRING("abba") );
		UT_ASSERT_FALSE( testLower < STRING("abba") );
		UT_ASSERT_TRUE( testLower > STRING("abba") );

		UT_ASSERT_FALSE( testLower <= STRING("ABBA") );
		UT_ASSERT_TRUE( testLower >= STRING("ABBA") );
		UT_ASSERT_FALSE( testLower < STRING("ABBA") );
		UT_ASSERT_TRUE( testLower > STRING("ABBA") );

		UT_ASSERT_LESS( CI_STRING("äöü"), CI_STRING("BCD") );
		UT_ASSERT_GREATER( CI_STRING("äöü"), CI_STRING("ABC") );
		UT_ASSERT_GREATER( CI_STRING("Äöü"), CI_STRING("ABC") );
	}
	void ConversionTests()
	{
		{
			STRING	test = "Hello world";
			const char	*ccp1 = test;
			const char	*ccp2 = test;

			UT_ASSERT_EQUAL( static_cast<const void*>(ccp1), static_cast<const void*>(ccp2) );

			char	*cp1 = test;
			char	*cp2 = test;

			UT_ASSERT_NOT_EQUAL( static_cast<void*>(cp1), static_cast<void*>(cp2) );
			free( cp1 );
			free( cp2 );
		}
		{
			STRING		text = "4711";
			unsigned	val = text.getValueE<unsigned>();
			UT_ASSERT_EQUAL( val, 4711U );
		}
		{
			STRING		text = "-4711";
			int			val = text.getValueE<int>();
			UT_ASSERT_EQUAL( val, -4711 );
		}
		{
			STRING		text = "32767";
			short		val = text.getValueE<short>();
			UT_ASSERT_EQUAL( val, short(32767) );
		}
		{
			STRING		text = "32767";
			int16		val = text.getValueE<int16>();
			UT_ASSERT_EQUAL( val, int16(32767) );
		}
		{
			STRING		text = "32768";
			UT_ASSERT_EXCEPTION(text.getValueE<int16>(), IntegerOverflowError);
		}
		{
			STRING		text = "-32768";
			int16		val = text.getValueE<int16>();
			UT_ASSERT_EQUAL( val, int16(-32768) );
		}
		{
			STRING		text = "-32769";
			UT_ASSERT_EXCEPTION(text.getValueE<int16>(), IntegerUnderflowError);
		}

		{
			STRING		text = "65535";
			uint16		val = text.getValueE<uint16>();
			UT_ASSERT_EQUAL( val, uint16(65535) );
		}
		{
			STRING		text = "65536";
			UT_ASSERT_EXCEPTION(text.getValueE<uint16>(), IntegerOverflowError);
		}
		{
			STRING		text = "-1";
			UT_ASSERT_EXCEPTION(text.getValueE<uint16>(), BadNumericFormatError);
		}
		{
			STRING		text = "1234567891234567890A";
			UT_ASSERT_EXCEPTION(text.getValueE<uint16>(), BadNumericFormatError);
		}
		{
			STRING		text = "4594812546";
			UT_ASSERT_EQUAL(4594812546, text.getValueE<int64>());
		}

		{
			STRING		text = "true";
			UT_ASSERT_TRUE(text.getValueN<bool>());
		}
		{
			STRING		text = "false";
			UT_ASSERT_FALSE(text.getValueN<bool>());
		}
		{
			STRING		text = "3.14";
			UT_ASSERT_EQUAL(3.14, text.getValueN<double>());
		}
		{
			STRING		text = "3,14";
			UT_ASSERT_EQUAL(3.14, text.getValueN<double>());
		}
		{
			STRING		text = "1,798e308";
			UT_ASSERT_EXCEPTION( text.getValueE<double>(), FloatOverflowError );
		}
		{
			STRING		text = "1e" + formatNumber(std::numeric_limits<double>::max_exponent10+1);
			UT_ASSERT_EXCEPTION( text.getValueE<double>(), ExponentOverflowError );
		}
		{
			STRING		text = "1e" + formatNumber(std::numeric_limits<float>::max_exponent10+1);
			UT_ASSERT_EXCEPTION( text.getValueE<float>(), ExponentOverflowError );
		}
		{
			STRING		text = "1e" + formatNumber(std::numeric_limits<double>::min_exponent10-1);
			UT_ASSERT_EXCEPTION( text.getValueE<double>(), ExponentUnderflowError );
		}
		{
			STRING		text = "1e" + formatNumber(std::numeric_limits<float>::min_exponent10-1);
			UT_ASSERT_EXCEPTION( text.getValueE<float>(), ExponentUnderflowError );
		}
	}
	void SizeTests()
	{
		STRING	test;
		UT_ASSERT_EQUAL( size_t(0), test.strlen() );
		test = "";
		UT_ASSERT_EQUAL( size_t(0), test.strlen() );
		test = "1234567890";
		UT_ASSERT_EQUAL( size_t(10), test.strlen() );
	}
	void ConstructorTest()
	{
		{
			STRING	defaultString;

			UT_ASSERT_EQUAL( size_t(0), defaultString.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(0), defaultString.strlen() );
			UT_ASSERT_TRUE( defaultString.isNullPtr() );
			UT_ASSERT_TRUE( defaultString.isEmpty() );
			UT_ASSERT_TRUE( !defaultString );
			UT_ASSERT_EQUAL( STRING(""), defaultString );
		}
		{
			const char *testText = "Hello World"; 
			STRING	testString1 = testText;

			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( strlen(testText), testString1.strlen() );
			UT_ASSERT_FALSE( testString1.isNullPtr() );
			UT_ASSERT_FALSE( testString1.isEmpty() );
			UT_ASSERT_FALSE( !testString1 );
			UT_ASSERT_EQUAL( 0, ansiCompare( testText, testString1 ) );

			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText), testString2 );

			testString2 = testText;
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText), testString2 );

			testString1 = "";
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_FALSE( testString1.isNullPtr() );
			UT_ASSERT_TRUE( testString1.isEmpty() );
			UT_ASSERT_TRUE( !testString1 );

		}
		{
			const char	*testText1 = "H"; 
			const char	testChar1 = 'H';
			const char	*testText2 = "B"; 
			const char	testChar2 = 'B';
			const char	*testText3 = "AAAAA"; 
			const char	testChar3 = 'A';
			STRING	testString1 = STRING(testChar1);

			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( strlen(testText1), testString1.strlen() );
			UT_ASSERT_FALSE( testString1.isNullPtr() );
			UT_ASSERT_FALSE( testString1.isEmpty() );
			UT_ASSERT_FALSE( !testString1 );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );

			testString1 = testChar2;
			UT_ASSERT_EQUAL( strlen(testText2), testString1.strlen() );
			UT_ASSERT_EQUAL( STRING(testText2), testString1 );

			STRING	testString2( testChar3, 5 );
			UT_ASSERT_EQUAL( STRING(testText3), testString2 );
		}
		{
			const char	*testText = "Hello World"; 
			DynamicVar	testDynamic = testText;
			STRING	testString1 = testDynamic;

			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( strlen(testText), testString1.strlen() );
			UT_ASSERT_FALSE( testString1.isNullPtr() );
			UT_ASSERT_FALSE( testString1.isEmpty() );
			UT_ASSERT_FALSE( !testString1 );
			UT_ASSERT_EQUAL( 0, ansiCompare( testText, testString1 ) );

			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(3), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(3), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText), testString2 );

			testString2 = testDynamic;
			UT_ASSERT_EQUAL( size_t(3), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(3), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText), testString2 );

			testDynamic = testText;
			testString2 = testDynamic;
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText), testString2 );

		}
		{
			STRING	test1 = "Hello world";
			UT_ASSERT_EQUAL( size_t(1), test1.getUsageCount() );
			{
				STRING	test2 = test1;
				UT_ASSERT_EQUAL( size_t(2), test1.getUsageCount() );
				UT_ASSERT_EQUAL( size_t(2), test2.getUsageCount() );
			}
			UT_ASSERT_EQUAL( size_t(1), test1.getUsageCount() );
		}
	}
	void ModifyingTests()
	{
		{
			const char *testText1 = "012345ABC6789";
			const char *testText2 = "012345Hello world6789";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.replaceText( 6, 3, "Hello world" );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "012345ABC6789";
			const char *testText2 = "ABC6789";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2 += std::size_t(6);
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = " \t\n\rABC   \t\n\r";
			const char *testText2 = "ABC";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.stripBlanks();
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = ">>>>>ABC>>>>>";
			const char *testText2 = "ABC>>>>>";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.stripLeftChar( '>' );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = ">>>>>ABC>>>>>";
			const char *testText2 = ">>>>>ABC";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.stripRightChar( '>' );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = ">>>>>ABC>>>>>";
			const char *testText2 = "ABC";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.stripChar( '>' );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "1234567890";
			const char *testText2 = "12345";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.cut( 5 );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "1234567890";
			const char *testText2 = "1234590";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.delStr( 5, 3 );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "1234567890";
			const char *testText2 = "123457890";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.delChar( 5 );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "1234590";
			const char *testText2 = "AAA1234567890";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.insStr( 0, "AAA" ).insStr( 8, "678" );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "1234590";
			const char *testText2 = "BBBA1CCC2346590";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.insChar( 0, 'A' ).insChar( 5, '6' ).insChar( 0, 'B', 3 ).insChar( 5, 'C', 3 );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "the quick brown fox jums over the lazy dog" LOWER_UMLAUT;
			const char *testText2 = "THE QUICK BROWN FOX JUMS OVER THE LAZY DOG" UPPER_UMLAUT;

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.upperCase();
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "THE QUICK BROWN FOX JUMS OVER THE LAZY DOG" UPPER_UMLAUT;
			const char *testText2 = "the quick brown fox jums over the lazy dog" LOWER_UMLAUT;

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.lowerCase();
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "-----THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG-----";
			const char *testText2 = "=====THE+QUICK+BROWN+FOX+JUMPS+OVER+THE+LAZY+DOG=====";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.replaceChar( '-', '=' ).replaceChar( ' ', '+' );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
		}
		{
			const char *testText1 = "\nTHE\nQUICK\nBROWN\nFOX\nJUMPS\nOVER\nTHE\nLAZY\nDOG\n";
			const char *testText2 = "\rTHE\rQUICK\rBROWN\rFOX\rJUMPS\rOVER\rTHE\rLAZY\rDOG\r";
			const char *testText3 = "\r\nTHE\r\nQUICK\r\nBROWN\r\nFOX\r\nJUMPS\r\nOVER\r\nTHE\r\nLAZY\r\nDOG\r\n";
			const char *testText4 = "\n\rTHE\n\rQUICK\n\rBROWN\n\rFOX\n\rJUMPS\n\rOVER\n\rTHE\n\rLAZY\n\rDOG\n\r";

			STRING	testString1 = testText1;
			STRING	testString2 = testString1;
			UT_ASSERT_EQUAL( size_t(2), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(2), testString2.getUsageCount() );
			testString2.replaceLineEnds( RL_END_CR );
			UT_ASSERT_EQUAL( size_t(1), testString1.getUsageCount() );
			UT_ASSERT_EQUAL( size_t(1), testString2.getUsageCount() );
			UT_ASSERT_EQUAL( STRING(testText1), testString1 );
			UT_ASSERT_EQUAL( STRING(testText2), testString2 );
			testString2.replaceLineEnds( RL_END_CRLF );
			UT_ASSERT_EQUAL( STRING(testText3), testString2 );
			testString2.replaceLineEnds( RL_END_LFCR );
			UT_ASSERT_EQUAL( STRING(testText4), testString2 );
			testString2.replaceLineEnds( RL_END_LF );
			UT_ASSERT_EQUAL( STRING(testText1), testString2 );
		}
		{
			STRING		testText = "Hello World";
			STRING		result = testText.padCopy( 10, STR_P_LEFT );
			UT_ASSERT_EQUAL( result, STRING("...o World") );
			result = testText.padCopy( 10, STR_P_RIGHT );
			UT_ASSERT_EQUAL( result, STRING("Hello W...") );

			result = testText.padCopy( 15, STR_P_LEFT );
			UT_ASSERT_EQUAL( result, STRING("    Hello World") );
			result = testText.padCopy( 15, STR_P_RIGHT );
			UT_ASSERT_EQUAL( result, STRING("Hello World    ") );

			result = testText.padCopy( 2, STR_P_LEFT );
			UT_ASSERT_EQUAL( result, STRING("..") );
			result = testText.padCopy( 2, STR_P_RIGHT );
			UT_ASSERT_EQUAL( result, STRING("..") );

			result = testText.padCopy( 0, STR_P_LEFT );
			UT_ASSERT_TRUE( result.isEmpty() );
			result = testText.padCopy( 0, STR_P_RIGHT );
			UT_ASSERT_TRUE( result.isEmpty() );
		}
	}
	void OtherTests()
	{
		{
			STRING		myTest = "Hello World";
			CI_STRING	myCiString = myTest;
			T_STRING	myTstring;

			myTstring = myTest;

			UT_ASSERT_TRUE( myTest == "Hello World" );
			UT_ASSERT_TRUE( myCiString == myTest );
			UT_ASSERT_TRUE( myTstring == myTest );

			myTest.upperCase();
			UT_ASSERT_TRUE( myCiString == myTest );
			UT_ASSERT_TRUE( myTest != myCiString );

			myTstring = "hello@world";
			STRING	part1 = myTstring.getFirstToken("@");
			STRING	part2 = myTstring.getNextToken();
			bool	next = myTstring.hasNextToken();
			UT_ASSERT_FALSE( next );
			UT_ASSERT_EQUAL( part1, STRING("hello") );
			UT_ASSERT_EQUAL( part2, STRING("world") );
		}

		{
			STRING	text = "meier.inf";
			STRING pattern = "meier*.*";

			UT_ASSERT_TRUE( text.match( pattern ) );
			UT_ASSERT_TRUE( patternMatch( "meier.inf", "meier*.*" ) );
			UT_ASSERT_FALSE( patternMatch( "meier.inf", "*.doc" ) );

			UT_ASSERT_TRUE( patternMatch( "meier.inf", "*.inf" ) );
			UT_ASSERT_TRUE( patternMatch( "meier.doc.inf.mager", "*.inf.*" ) );
			UT_ASSERT_TRUE( patternMatch( "meier.doc.inf.", "*.inf.*" ) );
		}
		{
			STRING	normal = "\xFF\x80\x7F\x01";
			STRING	escaped = "%FF%80\x7F%01";

			STRING result = net::webEscape( normal );
			UT_ASSERT_EQUAL( result, escaped );

			result = net::webUnEscape( escaped );
			UT_ASSERT_EQUAL( result, normal );
		}

		{
			const char *end;
			int result = getValue<int>( "13X", 10, &end );
			UT_ASSERT_EQUAL( result, 13 );
			UT_ASSERT_EQUAL( *end, 'X' );
		}
		{
			const char *end;
			int result = getValue<int>( "-13Y", 10, &end );
			UT_ASSERT_EQUAL( result, -13 );
			UT_ASSERT_EQUAL( *end, 'Y' );
		}
		{
			const char *end;
			unsigned result = getValue<unsigned>( "-13", 10, &end );
			UT_ASSERT_EQUAL( result, 0U );
			UT_ASSERT_EQUAL( *end, '-' );
		}
		{
			const char *end;
			double result = getValue<double>( "-13.56X", &end );
			UT_ASSERT_EQUAL( result, -13.56 );
			UT_ASSERT_EQUAL( *end, 'X' );
		}
		{
			const char *end;
			double result = getValue<double>( "-13.56E1\xFF", &end );
			UT_ASSERT_EQUAL( result, -135.6 );
			UT_ASSERT_EQUAL( *end, '\xFF' );
		}
		{
			STRING	test = STRING('a');
			UT_ASSERT_EQUAL( test[0U], 'a' );
			UT_ASSERT_EQUAL( std::size_t(1), test.strlen() );

			test.insChar( 0, 'b', 5 );
			UT_ASSERT_EQUAL( test, STRING("bbbbba") );
			test.addCharStr( 'c', 5 );
			UT_ASSERT_EQUAL( test, STRING("bbbbbaccccc") );
			test.insChar( 5, 'd', 5 );
			UT_ASSERT_EQUAL( test, STRING("bbbbbdddddaccccc") );
		}
	}

	void FileTests()
	{
		STRING myUtf8;
		myUtf8.readFromFile( "test_data" DIRECTORY_DELIMITER_STRING "utf8bom.txt" );
		UT_ASSERT_EQUAL( myUtf8.getCharSet(), STR_UTF8 );
	}

};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static StringTest myStringTest;

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

