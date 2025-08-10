/*
		Project:		GAKLIB
		Module:			RingBufferTest.h
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

#include <gak/ringBuffer.h>
#include <gak/unitTest.h>

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

class RingBufferTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "RingBufferTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "RingBufferTest::PerformTest");
		TestScope scope( "PerformTest" );

		RingBuffer<int>		buffer(5);

		{
			Optional<int>	first = buffer.oldest();
			Optional<int>	last = buffer.newest();
			UT_ASSERT_FALSE( first.isPresent() );
			UT_ASSERT_FALSE( last.isPresent() );
		}
		UT_ASSERT_TRUE( buffer.isEmpty() );
		UT_ASSERT_FALSE( buffer.isFull() );
		UT_ASSERT_EQUAL( buffer.size(), 0UL );

		buffer.push( 1 );
		UT_ASSERT_FALSE( buffer.isEmpty() );
		UT_ASSERT_FALSE( buffer.isFull() );
		UT_ASSERT_EQUAL( buffer.size(), 1UL );
		{
			Optional<int>	first = buffer.oldest();
			Optional<int>	last = buffer.newest();
			UT_ASSERT_TRUE( first.isPresent() );
			UT_ASSERT_TRUE( last.isPresent() );
			UT_ASSERT_EQUAL( first.get(), 1 );
			UT_ASSERT_EQUAL( last.get(), 1 );
		}

		buffer.push( 2 );
		buffer.push( 3 );
		buffer.push( 4 );
		UT_ASSERT_FALSE( buffer.isEmpty() );
		UT_ASSERT_FALSE( buffer.isFull() );
		UT_ASSERT_EQUAL( buffer.size(), 4UL );
		{
			Optional<int>	first = buffer.oldest();
			Optional<int>	last = buffer.newest();
			UT_ASSERT_TRUE( first.isPresent() );
			UT_ASSERT_TRUE( last.isPresent() );
			UT_ASSERT_EQUAL( first.get(), 1 );
			UT_ASSERT_EQUAL( last.get(), 4 );
		}

		UT_ASSERT_TRUE( buffer.push( 5 ) );
		UT_ASSERT_FALSE( buffer.push( 5 ) );
		UT_ASSERT_FALSE( buffer.isEmpty() );
		UT_ASSERT_TRUE( buffer.isFull() );
		UT_ASSERT_EQUAL( buffer.size(), 5UL );

		int cur;
		UT_ASSERT_TRUE( buffer.pop( &cur ) );
		UT_ASSERT_EQUAL( cur, 1 );
		UT_ASSERT_EQUAL( buffer.size(), 4UL );
		UT_ASSERT_TRUE( buffer.pop( &cur ) );
		UT_ASSERT_EQUAL( cur, 2 );
		UT_ASSERT_EQUAL( buffer.size(), 3UL );
		UT_ASSERT_TRUE( buffer.pop( &cur ) );
		UT_ASSERT_EQUAL( cur, 3 );
		UT_ASSERT_EQUAL( buffer.size(), 2UL );
		UT_ASSERT_TRUE( buffer.pop( &cur ) );
		UT_ASSERT_EQUAL( cur, 4 );
		UT_ASSERT_EQUAL( buffer.size(), 1UL );
		UT_ASSERT_TRUE( buffer.pop( &cur ) );
		UT_ASSERT_EQUAL( cur, 5 );
		UT_ASSERT_EQUAL( buffer.size(), 0UL );
		UT_ASSERT_FALSE( buffer.pop( &cur ) );

		buffer.push( 1 );
		buffer.push( 2 );
		buffer.push( 3 );
		UT_ASSERT_EQUAL( buffer.size(), 3UL );

		for( int i=0; i<9; ++i )
		{
			int dummy;

			buffer.push( i );
			buffer.pop( &dummy );
			UT_ASSERT_EQUAL( buffer.size(), 3UL );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static RingBufferTest myRingBufferTest;

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

