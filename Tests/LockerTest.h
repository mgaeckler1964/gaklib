/*
		Project:		GAKLIB
		Module:			LockerTest.h
		Description:	Test the locker
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

#include <gak/locker.h>

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

static const unsigned long TIMEOUT = 1000;
struct TestThread : public Thread
{
	bool		m_ok;
	bool		m_unlockAfter;
	Locker		&m_locker;
	StopWatch	m_sw;

	TestThread(Locker &locker, bool unlockAfter) : Thread(false), m_locker(locker), m_ok(false), m_unlockAfter(unlockAfter)
	{}

	void ExecuteThread()
	{
		AutoStopWatch sw(m_sw);

		m_ok = m_locker.lock(TIMEOUT);
		if( m_ok && m_unlockAfter )
		{
			m_locker.unlock();
		}
	}
};

class LockerTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "LockerTest";
	}
	virtual void PerformTest( void )
	{
		doEnterFunctionEx(gakLogging::llInfo, "LockerTest::PerformTest");
		TestScope scope( "PerformTest" );

		Locker	locker;
		
		UT_ASSERT_EQUAL( locker.getLockCount(), 0 );

		locker.lock();
		UT_ASSERT_EQUAL( locker.getLockCount(), 1 );

		locker.lock();
		UT_ASSERT_EQUAL( locker.getLockCount(), 2 );

		locker.unlock();
		UT_ASSERT_EQUAL( locker.getLockCount(), 1 );

		locker.unlock();
		UT_ASSERT_EQUAL( locker.getLockCount(), 0 );

		{
			TestScope scope( "concurrent test with unlock" );

			locker.lock();
			TestThread	thread( locker, true );
			thread.StartThread();
			Sleep(TIMEOUT/2);
			locker.unlock();
			thread.join();
			UT_ASSERT_TRUE( thread.m_ok );
			UT_ASSERT_GREATER( thread.m_sw.getMillis(), clock_t(TIMEOUT)/2 );
			UT_ASSERT_LESS( thread.m_sw.getMillis(), clock_t(TIMEOUT) );
			UT_ASSERT_EQUAL( locker.getLockCount(), 0 );
		}

		{
			TestScope scope( "concurrent test with timeout" );

			LockGuard lg( locker );
			UT_ASSERT_EQUAL( locker.getLockCount(), 1 );

			TestThread	thread( locker, false );
			StopWatch sw(true);
			thread.StartThread();
			thread.join();
			UT_ASSERT_FALSE( thread.m_ok );
			UT_ASSERT_GREATEREQ( sw.getMillis(), clock_t(TIMEOUT) );
		}
		UT_ASSERT_EQUAL( locker.getLockCount(), 0 );
		{
			TestScope scope( "concurrent test with already free" );

			TestThread	thread( locker, false );
			StopWatch sw(true);
			thread.StartThread();
			thread.join();
			UT_ASSERT_TRUE( thread.m_ok );
			UT_ASSERT_LESS( sw.getMillis(), clock_t(TIMEOUT) );
		}
		{
			TestScope scope( "concurrent test with lock forever" );

			StopWatch sw(true);
			bool ok = locker.lock(TIMEOUT);
			UT_ASSERT_FALSE( ok );
			UT_ASSERT_GREATEREQ( sw.getMillis(), clock_t(TIMEOUT) );
			UT_ASSERT_EQUAL( locker.getLockCount(), 1 );
			locker.unlock();
			UT_ASSERT_EQUAL( locker.getLockCount(), 1 );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static LockerTest myLockerTest;

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

