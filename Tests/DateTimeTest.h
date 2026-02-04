/*
		Project:		GAKLIB
		Module:			DateTimeTest.h
		Description:	Date and time handling class
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

class DateTimeTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "DateTimeTest";
	}
	void TestSummerWinterSunriseEquation()
	{
		DateTime	now;
		{
#if !NO_SEASON
			DateTime nextWinter = now.nextWinter();
#else
			DateTime nextWinter = DateTime(21,DateTime::DECEMBER,2026, 0,0,0);
#endif
			DateTime sunrise, sunset;
			nextWinter.sunriseEquation( 15, 48, &sunrise, &sunset );
			gak::uint64 winterDay = sunset.getUtcUnixSeconds() - sunrise.getUtcUnixSeconds();
			std::cout << "Sunrise " << sunrise << ' ' << sunrise.getUtcUnixSeconds() << '\n';
			std::cout << "Sunset  " << sunset << ' ' << sunset.getUtcUnixSeconds() << '\n';
			std::cout << "day length " << winterDay << '\n'; 

			DateTime nextWinterBefore = nextWinter; nextWinterBefore.decrement(2);
			DateTime sunriseBefore, sunsetBefore;
			nextWinterBefore.sunriseEquation( 15, 48, &sunriseBefore, &sunsetBefore );
			gak::uint64 winterDayBefore = sunsetBefore.getUtcUnixSeconds() - sunriseBefore.getUtcUnixSeconds();
			std::cout << "SunriseBefore " << sunriseBefore << ' ' << sunriseBefore.getUtcUnixSeconds() << '\n';
			std::cout << "SunsetBefore  " << sunsetBefore << ' ' << sunsetBefore.getUtcUnixSeconds() << '\n';
			std::cout << "day length " << winterDayBefore << '\n'; 

			DateTime nextWinterAfter = nextWinter; nextWinterAfter.increment(2);
			DateTime sunriseAfter, sunsetAfter;
			nextWinterAfter.sunriseEquation( 15, 48, &sunriseAfter, &sunsetAfter );
			gak::uint64 winterDayAfter = sunsetAfter.getUtcUnixSeconds() - sunriseAfter.getUtcUnixSeconds();
			std::cout << "SunriseAfter " << sunriseAfter << ' ' << sunriseAfter.getUtcUnixSeconds() << '\n';
			std::cout << "SunsetAfter  " << sunsetAfter << ' ' << sunsetAfter.getUtcUnixSeconds() << '\n';
			std::cout << "day length " << winterDayAfter << '\n';

			UT_ASSERT_LESS( winterDay, winterDayBefore );
			UT_ASSERT_LESS( winterDay, winterDayAfter );
		}
		{
#if !NO_SEASON
			DateTime nextSummer = now.nextSummer();
#else
			DateTime nextSummer = DateTime(21,DateTime::JUNE,2026, 0,0,0);
#endif
			DateTime sunrise, sunset;
			nextSummer.sunriseEquation( 15, 48, &sunrise, &sunset );
			gak::uint64 summerDay = sunset.getUtcUnixSeconds() - sunrise.getUtcUnixSeconds();
			std::cout << "Sunrise" << sunrise << ' ' << sunrise.getUtcUnixSeconds() << '\n'; 
			std::cout << "Sunset" << sunset << ' ' << sunset.getUtcUnixSeconds() << '\n'; 
			std::cout << "day length " << summerDay << '\n'; 

			DateTime nextSummerBefore = nextSummer; nextSummerBefore.decrement(2);
			DateTime sunriseBefore, sunsetBefore;
			nextSummerBefore.sunriseEquation( 15, 48, &sunriseBefore, &sunsetBefore );
			gak::uint64 summerDayBefore = sunsetBefore.getUtcUnixSeconds() - sunriseBefore.getUtcUnixSeconds();
			std::cout << "SunriseBefore " << sunriseBefore << ' ' << sunriseBefore.getUtcUnixSeconds() << '\n';
			std::cout << "SunsetBefore  " << sunsetBefore << ' ' << sunsetBefore.getUtcUnixSeconds() << '\n';
			std::cout << "day length " << summerDayBefore << '\n'; 

			DateTime nextSummerAfter = nextSummer; nextSummerAfter.increment(2);
			DateTime sunriseAfter, sunsetAfter;
			nextSummerAfter.sunriseEquation( 15, 48, &sunriseAfter, &sunsetAfter );
			gak::uint64 summerDayAfter = sunsetAfter.getUtcUnixSeconds() - sunriseAfter.getUtcUnixSeconds();
			std::cout << "SunriseAfter " << sunriseAfter << ' ' << sunriseAfter.getUtcUnixSeconds() << '\n';
			std::cout << "SunsetAfter  " << sunsetAfter << ' ' << sunsetAfter.getUtcUnixSeconds() << '\n';
			std::cout << "day length " << summerDayAfter << '\n'; 

			UT_ASSERT_GREATER( summerDay, summerDayBefore );
			UT_ASSERT_GREATER( summerDay, summerDayAfter );
		}
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "DateTimeTest::PerformTest");
		TestScope scope( "PerformTest" );

		DateTime	now;

#if !NO_SEASON
		std::cout << "\nSPRINGYEAR" << SPRINGYEAR << ' ' << SPRINGYEAR - MEANYEAR << ' ' << SPRINGYEAR - SEASONYEAR << ' ' << SPRINGDURATION70 << ' ' << SPRINGDURATION22 << ' ' << SPRINGDURATION22-SPRINGDURATION70 << "\n"
				<< "SUMMERYEAR" << SUMMERYEAR << ' ' << SUMMERYEAR - MEANYEAR << ' ' << SUMMERYEAR - SEASONYEAR << ' ' << SUMMERDURATION70 << ' ' << SUMMERDURATION22 << ' ' << SUMMERDURATION22-SUMMERDURATION70 << "\n"
				<< "AUTUMNYEAR" << AUTUMNYEAR << ' ' << AUTUMNYEAR - MEANYEAR << ' ' << AUTUMNYEAR - SEASONYEAR << ' ' << AUTUMNDURATION70 << ' ' << AUTUMNDURATION22 << ' ' << AUTUMNDURATION22-AUTUMNDURATION70 << "\n"
				<< "WINTERYEAR" << WINTERYEAR << ' ' << WINTERYEAR - MEANYEAR << ' ' << WINTERYEAR - SEASONYEAR << "\n";

		DateTime lastSpring = now.lastSpring();
		DateTime nextSpring = now.nextSpring();

		std::cout << now << ' ' << now.getUtcUnixSeconds() << '\n';
		std::cout << lastSpring << '\n';
		std::cout << nextSpring << '\n';
		DateTime::Season curSeason = now.getSeason();
		const char *SeasonStrings[] =
		{
			"S_UNKNOWN", "S_SPRING", "S_SUMMER", "S_AUTUMN", "S_WINTER"
		};
		std::cout << SeasonStrings[curSeason] << '\n';

		std::cout << "Vollmond vs Neumond (total) " << FULL_MOON_TIME << ' ' << NEW_MOON_TIME << ' ' << (FULL_MOON_TIME - NEW_MOON_TIME) << '\n';
		std::cout << "Phasenvergleich\n";
		std::cout << "Vollmond vs Neumond" << FULL_MOON_PHASE << ' ' << NEW_MOON_PHASE << ' ' << (FULL_MOON_PHASE - NEW_MOON_PHASE) << '\n';
		std::cout << "Mond vs Neumond " << MOON_PHASE << ' ' << NEW_MOON_PHASE << ' ' << (MOON_PHASE - NEW_MOON_PHASE) << '\n';
		std::cout << "Mond vs Vollmond " << MOON_PHASE << ' ' << FULL_MOON_PHASE << ' ' << (MOON_PHASE - FULL_MOON_PHASE) << '\n';
		std::cout << "AVG Mond vs Neumond " << AVG_MOON_PHASE << ' ' << NEW_MOON_PHASE << ' ' << (AVG_MOON_PHASE - NEW_MOON_PHASE) << '\n';
		std::cout << "AVG Mond vs Vollmond " << AVG_MOON_PHASE << ' ' << FULL_MOON_PHASE << ' ' << (AVG_MOON_PHASE - FULL_MOON_PHASE) << '\n';
		std::cout << "1970 vs 2024 " << HALF_MOON_1 << ' ' << HALF_MOON_2 << ' ' << (HALF_MOON_1 - HALF_MOON_2) << '\n';


		DateTime nextFullMoon = now.nextFullMoon();
		DateTime nextNewMoon = now.nextNewMoon();
		std::cout << "Vollmond: " << nextFullMoon << " Neumond: " << nextNewMoon << '\n';
#endif

		UT_ASSERT_EQUAL( now.getTZoffset(), 3600L );
		DateTime	epochBegin( time_t( 0 ) );
		UT_ASSERT_EQUAL( epochBegin.getYear(), (unsigned short)1970 );
		UT_ASSERT_EQUAL( epochBegin.getMonth(), Date::JANUARY );
		UT_ASSERT_EQUAL( int(epochBegin.getDay()), 1 );
		UT_ASSERT_EQUAL( int(epochBegin.getHour()), 0 );
		UT_ASSERT_EQUAL( int(epochBegin.getMinute()), 0 );
		UT_ASSERT_EQUAL( int(epochBegin.getSecond()), 0 );

		const time_t daySeconds = 24 * 3600;
		const time_t yearSeconds = 365 * daySeconds;
		const time_t leapYearSeconds = yearSeconds + daySeconds;

		{
			DateTime	theDate( time_t( yearSeconds*2 + leapYearSeconds + yearSeconds ) );
			UT_ASSERT_EQUAL( theDate.getYear(), (unsigned short)1974 );
			UT_ASSERT_EQUAL( theDate.getMonth(), Date::JANUARY );
			UT_ASSERT_EQUAL( int(theDate.getDay()), 1 );
			UT_ASSERT_EQUAL( int(theDate.getHour()), 0 );
			UT_ASSERT_EQUAL( int(theDate.getMinute()), 0 );
			UT_ASSERT_EQUAL( int(theDate.getSecond()), 0 );
		}

		{
			DateTime	theDate( time_t( yearSeconds*2 + 31*daySeconds + 28*daySeconds + 14*3600 + 30 * 60 + 25 ) );
			UT_ASSERT_EQUAL( theDate.getYear(), (unsigned short)1972 );
			UT_ASSERT_EQUAL( theDate.getMonth(), Date::FEBRUARY );
			UT_ASSERT_EQUAL( int(theDate.getDay()), 29 );
			UT_ASSERT_EQUAL( int(theDate.getHour()), 14 );
			UT_ASSERT_EQUAL( int(theDate.getMinute()), 30 );
			UT_ASSERT_EQUAL( int(theDate.getSecond()), 25 );
		}

		{
			DateTime	theDate( 14, Date::NOVEMBER, 2023, 22, 13, 20 );
			std::cout << theDate << ' ' << theDate.getUtcUnixSeconds() << '\n'; 
			UT_ASSERT_EQUAL( theDate.getUtcUnixSeconds(), 1700000000 );
		}
		//UT_ASSERT_FALSE(true);
		{
		    double latitude = 48.269655;
			double longitude = 14.311495;
			DateTime	theDate( 25, Date::DECEMBER, 2026, 0, 0, 0 );
			DateTime	sunrise, sunset;

			theDate.sunriseEquation( longitude, latitude, &sunrise, &sunset );
			std::cout << sunrise << ' ' << sunset << std::endl;
		}
		TestSummerWinterSunriseEquation();
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static DateTimeTest	myDateTimeTest;

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

