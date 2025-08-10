/*
		Project:		GAKLIB
		Module:			BlockedRingBufferTest.h
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

#include <gak/thread.h>
#include <gak/blockedRingBuffer.h>

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

const char testText[] = "the quick brown fox jumps over the lazy dog";

class BlockedRingBufferTest : public UnitTest
{
	class Producer : public Thread
	{
		const char *GetClassName() const
		{
			return "BlockedRingBufferTest::Producer";
		}
		BlockedRingBuffer<char>	&buffer;
		public:
		Producer( BlockedRingBuffer<char> &buffer ) : buffer(buffer)
		{
		}
		virtual void ExecuteThread()
		{
			doEnterFunctionEx(gakLogging::llInfo, "BlockedRingBufferTest::Producer::ExecuteThread");
			Sleep( 2000 );
			for( const char *cp = testText; *cp; ++cp )
			{
				UT_ASSERT_TRUE( buffer.push( *cp ) );
			}
		}
	};
	class Consumer : public Thread
	{
		const char *GetClassName() const
		{
			return "BlockedRingBufferTest::Consumer";
		}
		BlockedRingBuffer<char>	&buffer;
		public:
		Consumer( BlockedRingBuffer<char> &buffer ) : buffer(buffer)
		{
		}
		virtual void ExecuteThread()
		{
			doEnterFunctionEx(gakLogging::llInfo, "BlockedRingBufferTest::Consumer::ExecuteThread");
			for( const char *cp = testText; *cp; ++cp )
			{
				char c;
				UT_ASSERT_TRUE( buffer.pop( &c ) );
				UT_ASSERT_EQUAL( c, *cp );
				std::cout << c << std::flush;
				Sleep( 100 );
			}
			std::cout << std::endl;
		}
	};

	virtual const char *GetClassName() const
	{
		return "BlockedRingBufferTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "BlockedRingBufferTest::PerformTest");
		TestScope scope( "PerformTest" );

		BlockedRingBuffer<char>		buffer(5);

		SharedObjectPointer<Consumer> consumer = new Consumer( buffer );
		SharedObjectPointer<Producer> producer = new Producer( buffer );

		consumer->StartThread();
		producer->StartThread();

		producer->join();
		consumer->join();
	}
};


// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static BlockedRingBufferTest myBlockedRingBufferTest;

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

