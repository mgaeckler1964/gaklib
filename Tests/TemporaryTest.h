/*
		Project:		GAKLIB
		Module:			TemporaryTest.h
		Description:	Usefull to add sam small hacking tests with short live time
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
#include <gak/memory>

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

template <typename FunctorT>
static void xConsoleOut( const FunctorT &functor )
{
static Critical s_consoleCheck;

	CriticalScope	scope( s_consoleCheck );
	functor();
}

static Critical s_consoleCheck;
#define ConsoleOut( functor )	\
{ \
	CriticalScope	scope( s_consoleCheck );	\
	functor;	\
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class TemporaryTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return DISABLED_TEST_PREFIX "TemporaryTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "TemporaryTest::PerformTest");
		TestScope scope( "PerformTest" );

		UT_EXPECT_TRUE( false );
		UT_EXPECT_FALSE( true );
		UT_EXPECT_NOT_NULL( nullptr );
		UT_EXPECT_NULL( GetClassName() );
		UT_EXPECT_RANGE( int(0), int(-8), int(10) );
		UT_EXPECT_EQUAL( 50, 621 );
		UT_EXPECT_EQUAL( double(50.5), double(621.5) );
		UT_EXPECT_NOT_EQUAL( 50, 50 );
		UT_EXPECT_LESS(50,50);
		UT_EXPECT_LESSEQ(51,50);
		UT_EXPECT_GREATER(50,50);
		UT_EXPECT_GREATEREQ(49,50);
		UT_EXPECT_EQUAL_FLT( double(50.5), double(621.5), 1.0 );
		UT_ASSERT_EQUAL_FLT( double(50.5), double(621.5), 1.0 );

		// my old C++ builder and the old Gnu C++ do not know unique_ptr
		std::unique_ptr<STRING> myUnique( new STRING );
		std::auto_ptr<STRING> myAuto( new STRING );
		std::unique_ptr<STRING[]> myUnique2( new STRING[4] );
		myUnique2.reset();
		myUnique.reset();

		UT_EXPECT_LESSEQ( sizeof(myUnique), sizeof(myAuto) );
		std::cout << sizeof(myUnique) << ' ' << sizeof(myAuto) << std::endl;

		const size_t count = 100000;
		StopWatch sw1(true);
		for( size_t i=0; i<count; ++i )
		{
			ConsoleOut( {  std::cout << i << '\r' << std::flush; } );
		}
		sw1.stop();
		std::cout << std::endl << "Macro " << sw1.get< Seconds<> >().toString() << std::endl;

#if __cplusplus > 1
		StopWatch sw2(true);
		for( size_t i=0; i<count; ++i )
		{
			xConsoleOut( [=] {  std::cout << i << '\r' << std::flush; } );
		}
		sw2.stop();
		std::cout << std::endl << "Lambda " << sw2.get< Seconds<> >().toString() << std::endl;

		UT_EXPECT_LESS( sw2.getMillis(), sw1.getMillis() );
#endif
	}
	virtual bool canThreadTest()
	{
		return true;
	}
	virtual UnitTest *duplicate()
	{
		return new TemporaryTest( false );
	}
	public:
	TemporaryTest( bool isStatic=true ) : UnitTest( isStatic ) {}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static TemporaryTest myTemporaryTest;

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

