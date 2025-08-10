/*
		Project:		GAKLIB
		Module:			SleeperTest.h
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

class SLEEPER : public Thread
{
	int numSec;

	public:
	SLEEPER( int numSec ) : Thread( false )
	{
		this->numSec = numSec;
	}
	virtual void ExecuteThread( void );
};

class ConditionalThread : public Thread
{
	int numSec;
	Conditional	theConditional;

	public:
	ConditionalThread( int numSec ) : Thread( false )
	{
		this->numSec = numSec;
	}
	virtual void ExecuteThread( void );
	void notify( void )
	{
		theConditional.notify();
	}
};

class SleeperTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "SleeperTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "SleeperTest::PerformTest");
		TestScope scope( "PerformTest" );
		{
			unsigned long startTime = UserTimeClock::clock();

			SLEEPER		myThread(10);
			myThread.StartThread();
			myThread.join();

			unsigned long elapsedTime = UserTimeClock::clock() - startTime;

			UT_ASSERT_RANGE( 9800UL, elapsedTime, 10200UL );
		}

		{
			unsigned long startTime = UserTimeClock::clock();

			ConditionalThread	myThread( 10 );

			myThread.StartThread();
			myThread.join();

			unsigned long elapsedTime = UserTimeClock::clock() - startTime;

			UT_ASSERT_RANGE( 9800UL, elapsedTime, 10200UL );
		}
		{
			unsigned long startTime = UserTimeClock::clock();

			ConditionalThread	myThread( 10 );

			myThread.StartThread();
			Sleep( 5000 );
			myThread.notify();
			myThread.join();

			unsigned long elapsedTime = UserTimeClock::clock() - startTime;

			UT_ASSERT_RANGE( 4800UL, elapsedTime, 5200UL );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static SleeperTest mySleeperTest;

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
   
void SLEEPER::ExecuteThread()
{
	doEnterFunctionEx(gakLogging::llInfo, "SLEEPER::ExecuteThread");
	unsigned long startTime = UserTimeClock::clock();

	for( int i=0; i<numSec; i++ )
	{
		Sleep( 1000 );
		std::cout	<< (void*)Locker::GetCurrentThreadID() 
					<< ' ' 
					<< i 
					<< (UserTimeClock::clock()-startTime) 
					<< '\n';
	}
}

void ConditionalThread::ExecuteThread( void )
{
	doEnterFunctionEx(gakLogging::llInfo, "ConditionalThread::ExecuteThread");
	theConditional.wait( numSec * 1000 );
}


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

