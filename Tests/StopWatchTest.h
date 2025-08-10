/*
		Project:		GAKLIB
		Module:			StopWatchTest.h
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

class StopWatchTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "StopWatchTest";
	}
	virtual void PerformTest( void )
	{
		doEnterFunctionEx(gakLogging::llInfo, "StopWatchTest::PerformTest");
		TestScope scope( "PerformTest" );
		{
			const int		sleepTime = 900;
			const int		allowdOffset = 6;
			const clock_t	minTime = sleepTime - allowdOffset;
			const clock_t	maxTime = sleepTime + allowdOffset;

			StopWatch		sw( true );
			CpuStopWatch	cpuSW;
			cpuSW.start();
			Sleep( sleepTime );
			clock_t			millisSime = sw.getMillis();
			Hours<clock_t>	hoursTime = sw.get< Hours<std::clock_t> >();
			sw.stop();
			cpuSW.stop();

			UT_ASSERT_LESSEQ( cpuSW.getMillis(), millisSime );
			UT_ASSERT_RANGE( minTime, millisSime, maxTime );
			UT_ASSERT_EQUAL( sw.get<Weeks<> >().get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( sw.get<Days<> >().get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( sw.get<Hours<> >().get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( sw.get<Minutes<> >().get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( sw.get<Seconds<> >().get(), std::clock_t(0L) );
			UT_ASSERT_RANGE( minTime, sw.get<MilliSeconds<> >().get(), maxTime );

			UT_ASSERT_RANGE( minTime, hoursTime.asMillis(), maxTime );
		}
		{
			Weeks<>	time(MilliSeconds<>(450));
			UT_ASSERT_EQUAL( time.get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( time.Days<>::get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( time.Hours<>::get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(0L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(450L) );
			UT_ASSERT_EQUAL( time.toString(), STRING("0w 0d 0h 0m 0s 450ms") );
		}
		{
			Weeks<>	time(1);
			UT_ASSERT_EQUAL( time.get(), std::clock_t(1L) );
			UT_ASSERT_EQUAL( time.Days<>::get(), std::clock_t(7L) );
			UT_ASSERT_EQUAL( time.Hours<>::get(), std::clock_t(7L*24L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(7L*24L*60L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(7L*24L*60L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(7L*24L*60L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("1w 0d 0h 0m 0s 0ms") );

			time = 2;
			UT_ASSERT_EQUAL( time.get(), std::clock_t(2L) );
			UT_ASSERT_EQUAL( time.Days<>::get(), std::clock_t(2L*7L) );
			UT_ASSERT_EQUAL( time.Hours<>::get(), std::clock_t(2L*7L*24L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(2L*7L*24L*60L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(2L*7L*24L*60L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(2L*7L*24L*60L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("2w 0d 0h 0m 0s 0ms") );
		}
		{
			Days<>	time(1);
			UT_ASSERT_EQUAL( time.get(), std::clock_t(1L) );
			UT_ASSERT_EQUAL( time.Hours<>::get(), std::clock_t(24L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(24L*60L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(24L*60L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(24L*60L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("1d 0h 0m 0s 0ms") );

			time = 3;
			UT_ASSERT_EQUAL( time.get(), std::clock_t(3L) );
			UT_ASSERT_EQUAL( time.Hours<>::get(), std::clock_t(3L*24L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(3L*24L*60L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(3L*24L*60L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(3L*24L*60L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("3d 0h 0m 0s 0ms") );
		}
		{
			Hours<>	time(1);
			UT_ASSERT_EQUAL( time.get(), std::clock_t(1L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(60L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(60L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(60L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("1h 0m 0s 0ms") );

			time = 4;
			UT_ASSERT_EQUAL( time.get(), std::clock_t(4L) );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), std::clock_t(4L*60L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(4L*60L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(4L*60L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("4h 0m 0s 0ms") );
		}
		{
			Minutes<>	time(1);
			UT_ASSERT_EQUAL( time.get(), std::clock_t(1L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("1m 0s 0ms") );

			time = 5;
			UT_ASSERT_EQUAL( time.get(), std::clock_t(5L) );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), std::clock_t(5L*60L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(5L*60L*1000) );
			UT_ASSERT_EQUAL( time.toString(), STRING("5m 0s 0ms") );
		}
		{
			Seconds<>	time(1);
			UT_ASSERT_EQUAL( time.get(), std::clock_t(1L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(1000L) );
			UT_ASSERT_EQUAL( time.toString(), STRING("1s 0ms") );

			time = 6;
			UT_ASSERT_EQUAL( time.get(), std::clock_t(6L) );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), std::clock_t(6L*1000L) );
			UT_ASSERT_EQUAL( time.toString(), STRING("6s 0ms") );
		}
		{
			Weeks<>	time = Weeks<>(1) + Days<>(1) + Hours<>(1) + Minutes<>(1) + Seconds<>(1) + MilliSeconds<>(1);
			const clock_t	expectedWeeks	= 1;
			const clock_t	expectedDays	= expectedWeeks * 7 + 1;
			const clock_t	expectedHours	= expectedDays * 24 + 1;
			const clock_t	expectedMinutes	= expectedHours * 60 + 1;
			const clock_t	expectedSeconds	= expectedMinutes * 60 + 1;
			const clock_t	expectedMillis	= expectedSeconds * 1000 + 1;

			UT_ASSERT_EQUAL( time.get(), expectedWeeks );
			UT_ASSERT_EQUAL( time.Days<>::get(), expectedDays );
			UT_ASSERT_EQUAL( time.Hours<>::get(), expectedHours );
			UT_ASSERT_EQUAL( time.Minutes<>::get(), expectedMinutes );
			UT_ASSERT_EQUAL( time.Seconds<>::get(), expectedSeconds );
			UT_ASSERT_EQUAL( time.MilliSeconds<>::get(), expectedMillis );
			UT_ASSERT_EQUAL( time.toString(), STRING("1w 1d 1h 1m 1s 1ms") );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static StopWatchTest myStopWatchTest;

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

