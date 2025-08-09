/*
		Project:		GAKLIB
		Module:			LogFilkeTest,h
		Description:	Tesst logging and profiling
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

#include <gak/thread.h>

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

#include <iostream>
#include <gak/unitTest.h>

namespace gak
{

struct ThreadProfiler : public gak::Thread
{
	static const size_t s_loop_count;
	size_t m_count;

	gakLogging::LogLevel m_logLevel;


	void ProfileThreadTest2();
	void ProfileThreadTest();

	ThreadProfiler(gakLogging::LogLevel ll) : Thread(false), m_count(0), m_logLevel(ll)
	{}

	void ExecuteThread()
	{
		doEnterFunctionEx(gakLogging::llInfo, "ThreadProfiler::ExecuteThread");
		ProfileThreadTest();
	}
};

class LogfileTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "LogfileTest";
	}

	static const size_t s_loop_count;
	size_t count;

	void ProfileTestNone2();
	void ProfileTestNone();
	void ProfileTestDetail2();
	void ProfileTestDetail();
	void ProfileTestInfo2();
	void ProfileTestInfo();
	void ProfileTest();

	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::PerformTest");
		TestScope scope( "PerformTest" );

		doLogPosition();
		doLogPositionEx(gakLogging::llInfo);

		bool value=true;
		doLogValue(value );
		doLogValueEx(gakLogging::llInfo, value );

		doLogMessage( "Hello world" );
		doLogMessageEx( gakLogging::llInfo, "Hello world" );

#ifdef __WINDOWS__
		doLogError( GetLastError() );
#endif

		ProfileTest();
	}
};


#ifdef NDEBUG
const size_t LogfileTest::s_loop_count = 10000000;
const size_t ThreadProfiler::s_loop_count = 10000000;
#else
const size_t LogfileTest::s_loop_count = 100000;
const size_t ThreadProfiler::s_loop_count = 100000;
#endif


void ThreadProfiler::ProfileThreadTest2()
{
	doEnterFunctionEx(m_logLevel, "ThreadProfiler::ProfileThreadTest2");
	m_count++;
}

void ThreadProfiler::ProfileThreadTest()
{
	doEnterFunctionEx(gakLogging::llInfo, "ThreadProfiler::ProfileThreadTest");
	for( int i=0; i<s_loop_count; ++i )
	{
		ProfileThreadTest2();
	}
}

void LogfileTest::ProfileTestNone2()
{
	count++;
}

void LogfileTest::ProfileTestNone()
{
	doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTestNone");
	for( int i=0; i<s_loop_count; ++i )
	{
		ProfileTestNone2();
	}
}

void LogfileTest::ProfileTestDetail2()
{
	doEnterFunctionEx(gakLogging::llDetail, "LogfileTest::ProfileTestDetail2");
	count++;
}

void LogfileTest::ProfileTestDetail()
{
	doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTestDetail");
	for( int i=0; i<s_loop_count; ++i )
	{
		ProfileTestDetail2();
	}
}

void LogfileTest::ProfileTestInfo2()
{
	doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTestInfo2");
	count++;
}

void LogfileTest::ProfileTestInfo()
{
	doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTestInfo");
	for( int i=0; i<s_loop_count; ++i )
	{
		ProfileTestInfo2();
	}
}

void LogfileTest::ProfileTest()
{
	doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTest");

	{
		doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTest::SingleThread");
		count = 0;
		ProfileTestInfo();
		ProfileTestDetail();
		ProfileTestNone();
		UT_ASSERT_EQUAL( count, 3*s_loop_count);
	}

	{
		doEnterFunctionEx(gakLogging::llInfo, "LogfileTest::ProfileTest::MultiThread");

		ThreadProfiler	detailThread( gakLogging::llDetail );
		ThreadProfiler	infoThread( gakLogging::llInfo );

		infoThread.StartThread("infoThread");
		detailThread.StartThread("detailThread");

		detailThread.join();
		infoThread.join();

		UT_ASSERT_EQUAL( detailThread.m_count, ThreadProfiler::s_loop_count);
		UT_ASSERT_EQUAL( infoThread.m_count, ThreadProfiler::s_loop_count);
	}
}

static LogfileTest	myLogfileTest;

}	//namespace gak

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

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

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

