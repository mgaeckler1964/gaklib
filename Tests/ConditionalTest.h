/*
		Project:		GAKLIB
		Module:			ConditionalTest.h
		Description:	Test conditionals
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

#include <gak/shared.h>
#include <gak/thread.h>
#include <gak/conditional.h>

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

static Conditional theConditional;
static int notifyCount = 0;

static const int firstSleep = 2;
static const int secondSleep = 3;
static const int expectedTime = firstSleep + secondSleep + 2;

class ControllerThread : public Thread
{
	virtual void ExecuteThread()
	{
		doEnterFunctionEx( gakLogging::llInfo, "ControllerThread::ExecuteThread" );
		theConditional.notify();
		notifyCount = 1;
		Sleep( firstSleep * 1000 );
		notifyCount = 2;
		theConditional.notify();
		Sleep( secondSleep * 1000 );
		notifyCount = 3;
		theConditional.notify();
	}
};

class WorkerThread : public Thread
{
	const char *GetClassName( void ) const
	{
		return "ConditionalTest";
	}
	virtual void ExecuteThread()
	{
		doEnterFunctionEx( gakLogging::llInfo, "WorkerThread::ExecuteThread" );
		Sleep( 1000 );
		clock_t	start = clock();
		theConditional.wait();
		clock_t	end = clock();
		UT_ASSERT_EQUAL( 1, notifyCount );
		UT_ASSERT_LESS( int(end-start), 10 );

		theConditional.wait();
		UT_ASSERT_EQUAL( 2, notifyCount );

		{
			time_t	start = time( NULL );
			theConditional.wait();
			time_t	end = time( NULL );
			UT_ASSERT_EQUAL( 3, notifyCount );
			UT_ASSERT_GREATER( int(end-start), 1 );
		}

		notifyCount = -1;
	}
};

class ConditionalTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "ConditionalTest";
	}
	virtual void PerformTest( void )
	{
		doEnterFunctionEx( gakLogging::llInfo, "ConditionalTest::PerformTest" );
		TestScope scope( "PerformTest" );

		SharedObjectPointer<ControllerThread>	controller = new ControllerThread();
		SharedObjectPointer<WorkerThread>		worker = new WorkerThread();

		controller->StartThread();
		worker->StartThread();
		for( size_t i=0; i<(expectedTime * 10) && (controller->isRunning || worker->isRunning); ++i )
		{
			//std::cout << i << '\r' << std::flush;
			Sleep( 100 );
		}
		if( controller->isRunning )
		{
			controller->StopThread( true );
			std::cout << "\nControler is running 8-(" << std::endl;
		}
		if( worker->isRunning )
		{
			worker->StopThread( true );
			std::cout << "\nWorker is running 8-(" << std::endl;
		}
		UT_ASSERT_EQUAL( -1, notifyCount );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static ConditionalTest myConditional;

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

