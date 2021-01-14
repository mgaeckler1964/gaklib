/*
		Project:		GAKLIB
		Module:			CppTest.h
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

#include <gak/cppParser.h>
#include <gak/cppPreprocessor.h>

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

class CppTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "CppTest";
	}
	void doTest( const STRING &source, const STRING &expectedResult )
	{
		gak::iSTRINGstream	istream( source );
		CPPparser		myFile( &istream );
		CPreprocessor	myProcessor(CPreprocessor::omText);
		myProcessor.addMacro( "__cplusplus", "1" );
#ifdef _Windows
		myProcessor.addMacro( "_Windows", "1" );
#endif
#ifdef __MACH__ 
		myProcessor.addMacro( "__MACH__ ", "1" );
#endif
#ifdef __unix__  
		myProcessor.addMacro( "__unix__  ", "1" );
#endif

		STRING	result;
		{
			gak::oSTRINGstream	ostream(result);
			myProcessor.precompile( &myFile, ostream );
		}

		result.stripBlanks();
		UT_ASSERT_EQUAL( result, expectedResult );
	}
	virtual void PerformTest( void )
	{
		{
			TestScope scope( "#if 0" );
			doTest(
				"#if 0\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if 1" );
			doTest(
				"#if 1\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if <var> is false" );
			doTest(
				"#define __HELLO__ 0\n"
				"#if __HELLO__ \n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if <var> is true" );
			doTest(
				"#define __HELLO__ 1\n"
				"#if __HELLO__ \n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#ifndef is true" );
			doTest(
				"#ifndef __HELLO__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if !defined is true" );
			doTest(
				"#if !defined( __HELLO__ )\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#ifdef is false" );
			doTest(
				"#ifdef __HELLO__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined is false" );
			doTest(
				"#if defined( __HELLO__ )\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#ifndef is false" );
			doTest(
				"#define __HELLO__\n"
				"#ifndef __HELLO__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if !defined is false" );
			doTest(
				"#define __HELLO__\n"
				"#if !defined( __HELLO__ )\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#ifdef is true" );
			doTest(
				"#define __HELLO__\n"
				"#ifdef __HELLO__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined is true" );
			doTest(
				"#define __HELLO__\n"
				"#if defined __HELLO__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined is false" );
			doTest(
				"#if defined __HELLO__\n"
					"bad\n"
				"#elif defined __WORLD__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#elif defined is true" );
			doTest(
				"#define __WORLD__ 0\n"
				"#if defined __HELLO__\n"
					"bad\n"
				"#elif defined __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#elif defined is false" );
			doTest(
				"#define __HELLO__ 0\n"
				"#define __WORLD__ 0\n"
				"#if defined __HELLO__\n"
					"good\n"
				"#elif defined __WORLD__\n"
					"bad\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined( __HELLO__ ) && defined( __WORLD__ ) is true" );
			doTest(
				"#define __HELLO__ 0\n"
				"#define __WORLD__ 0\n"
				"#if defined( __HELLO__ ) && defined( __WORLD__ )\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined( __HELLO__ ) && !defined( __WORLD__ ) is true" );
			doTest(
				"#define __HELLO__ 0\n"
				"#if defined( __HELLO__ ) && !defined( __WORLD__ )\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined( __HELLO__ ) || defined( xxx ) is true" );
			doTest(
				"#define __HELLO__ 0\n"
				"#if defined( __HELLO__ ) || defined( __WORLD__ )\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if defined( xxx ) || defined( __WORLD__ ) is true" );
			doTest(
				"#define __WORLD__ 0\n"
				"#if defined( __HELLO__ ) || defined( __WORLD__ )\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ > __WORLD__ is true" );
			doTest(
				"#define __HELLO__ 0x10\n"
				"#define __WORLD__ 10\n"
				"#if __HELLO__ > __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ > __WORLD__ is false" );
			doTest(
				"#define __HELLO__ 10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ > __WORLD__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ >= __WORLD__ true >" );
			doTest(
				"#define __HELLO__ 0x10\n"
				"#define __WORLD__ 10\n"
				"#if __HELLO__ >= __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ >= __WORLD__ true ==" );
			doTest(
				"#define __HELLO__ 0x10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ >= __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ >= __WORLD__ false" );
			doTest(
				"#define __HELLO__ 10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ >= __WORLD__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ == __WORLD__ true" );
			doTest(
				"#define __HELLO__ 16\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ == __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ == __WORLD__ false" );
			doTest(
				"#define __HELLO__ 10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ == __WORLD__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ <= __WORLD__ false" );
			doTest(
				"#define __HELLO__ 0x10\n"
				"#define __WORLD__ 10\n"
				"#if __HELLO__ <= __WORLD__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ <= __WORLD__ true ==" );
			doTest(
				"#define __HELLO__ 0x10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ <= __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ <= __WORLD__ true <" );
			doTest(
				"#define __HELLO__ 10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ <= __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ < __WORLD__ false" );
			doTest(
				"#define __HELLO__ 0x10\n"
				"#define __WORLD__ 10\n"
				"#if __HELLO__ < __WORLD__\n"
					"bad\n"
				"#else\n"
					"good\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "#if __HELLO__ < __WORLD__ true" );
			doTest(
				"#define __HELLO__ 10\n"
				"#define __WORLD__ 0x10\n"
				"#if __HELLO__ < __WORLD__\n"
					"good\n"
				"#else\n"
					"bad\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "nested #if" );
			doTest(
				"#define __HELLO__ 10\n"
				"#if !defined( __HELLO__ )\n"
					"bad\n"
					"#if !defined( __HELLO__ )\n"
						"bad\n"
					"#elif defined( __WORLD__ )\n"
						"bad\n"
					"#else\n"
						"bad\n"
					"#endif\n"
				"#elif defined( __WORLD__ )\n"
						"bad\n"
					"#if !defined( __HELLO__ )\n"
						"bad\n"
					"#elif defined( __WORLD__ )\n"
						"bad\n"
					"#else\n"
						"bad\n"
					"#endif\n"
				"#else\n"
					"#if !defined( __HELLO__ )\n"
						"bad\n"
					"#elif defined( __WORLD__ )\n"
						"bad\n"
					"#else\n"
						"good\n"
					"#endif\n"
				"#endif\n",
				"good"
			);
		}
		{
			TestScope scope( "function macro" );
			doTest(
				"/*\n"
				"	comments must not appear umlauts äöüßÄÖÜ must not be a problem here\n"
				"*/\n"
				"// comments must not appear\n"
				"#define __HELLO__	5		// comments must not appear\n"
				"#define __WORLD__	6		// comments must not appear\n"
				"#define max(a,b) (a>b?a:b)	// comments must not appear\n"
				"max(__HELLO__,__WORLD__);	// comments must not appear\n"
				"int test( int max );\n",
				"(5 >6 ?5 :6 );\n"
				"int test (int max );"
			);
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static CppTest myCppTest;

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

