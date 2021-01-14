/*
		Project:		
		Module:			
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

class DateTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "DateTest";
	}
	void testJulianSince( unsigned char day, JulianDate::Month month, unsigned short year )
	{
		gak::JulianDate		theSourceDate(day, month, year );
		gak::JulianDate		theTarget( theSourceDate.since() );
		UT_ASSERT_EQUAL( theSourceDate, theTarget );
	}
	void testGregorianSince( unsigned char day, Date::Month month, unsigned short year )
	{
		gak::Date		theSourceDate(day, month, year );
		gak::Date		theTarget( theSourceDate.since() );
		UT_ASSERT_EQUAL( theSourceDate, theTarget );
	}

	virtual void PerformTest( void )
	{
		gak::Date	Jan1( 1, Date::JANUARY, 2016 );
		gak::Date	Dec1( 1, Date::DECEMBER, 2015 );
		gak::Date	Dec31( 31, Date::DECEMBER, 2015 );

		double	numWorkdays = getNumWorkDays( Dec1, Jan1 );
		UT_ASSERT_EQUAL( numWorkdays, 21.0 );

		numWorkdays = getNumWorkDays( Dec1, Dec31 );
		UT_ASSERT_EQUAL( numWorkdays, 20.5 );

		gak::Date	Mar1( 1, Date::MARCH, 2016 );
		gak::Date	Apr1( 1, Date::APRIL, 2016 );

		numWorkdays = getNumWorkDays( Mar1, Apr1 );
		UT_ASSERT_EQUAL( numWorkdays, 21.0 );

		long numDays = Jan1 - Dec1;
		UT_ASSERT_EQUAL( numDays, 31L );
		Dec1++;
		numDays = Jan1 - Dec1;
		UT_ASSERT_EQUAL( numDays, 30L );
		UT_ASSERT_GREATER( Jan1, Dec1 );

		Date::WeekDay	wd = Jan1.weekDay();
		UT_ASSERT_EQUAL( wd, Date::FRIDAY );

		{
			gak::JulianDate	theDate(4, JulianDate::OCTOBER, 1582 );
			JulianDate::WeekDay	wd = theDate.weekDay();
			UT_ASSERT_EQUAL( wd, JulianDate::THURSDAY );
		}
		{
			gak::Date		theDate(15, Date::OCTOBER, 1582 );
			Date::WeekDay	wd = theDate.weekDay();
			UT_ASSERT_EQUAL( wd, Date::FRIDAY );
		}
		{
			gak::JulianDate	theJulDate(5, JulianDate::OCTOBER, 1582 );
			gak::Date		theGregDate(15, Date::OCTOBER, 1582 );

			UT_ASSERT_EQUAL( int(theGregDate.weekDay()), int(theJulDate.weekDay()) );
			UT_ASSERT_EQUAL( theGregDate, julian2GregorianDate( theJulDate ) );
			UT_ASSERT_EQUAL( theJulDate, gregorian2JulianDate( theGregDate ) );
		}
		{
			gak::JulianDate	theJulDate(19, JulianDate::FEBRUARY, 1700 );
			gak::Date		theGregDate(1, Date::MARCH, 1700 );

			UT_ASSERT_EQUAL( int(theGregDate.weekDay()), int(theJulDate.weekDay()) );
			UT_ASSERT_EQUAL( theGregDate, julian2GregorianDate( theJulDate ) );
			UT_ASSERT_EQUAL( theJulDate, gregorian2JulianDate( theGregDate ) );
		}
		{
			gak::JulianDate	theJulDate(1, JulianDate::FEBRUARY, 1918 );
			gak::Date		theGregDate(14, Date::FEBRUARY, 1918 );

			UT_ASSERT_EQUAL( int(theGregDate.weekDay()), int(theJulDate.weekDay()) );
			UT_ASSERT_EQUAL( theGregDate, julian2GregorianDate( theJulDate ) );
			UT_ASSERT_EQUAL( theJulDate, gregorian2JulianDate( theGregDate ) );
		}

		testJulianSince( 1, JulianDate::JANUARY, 0 );
		testJulianSince( 31, JulianDate::DECEMBER, 0 );
		testJulianSince( 1, JulianDate::JANUARY, 1 );
		testJulianSince( 1, JulianDate::JANUARY, 1900 );
		testJulianSince( 29, JulianDate::FEBRUARY, 1900 );
		testJulianSince( 1, JulianDate::MARCH, 1900 );
		testJulianSince( 31, JulianDate::DECEMBER, 1900 );
		testJulianSince( 11, JulianDate::JUNE, 1964 );

		testGregorianSince( 1, Date::JANUARY, 0 );
		testGregorianSince( 31, Date::DECEMBER, 0 );
		testGregorianSince( 1, Date::JANUARY, 1 );
		testGregorianSince( 1, Date::JANUARY, 1900 );
		testGregorianSince( 28, Date::FEBRUARY, 1900 );
		testGregorianSince( 1, Date::MARCH, 1900 );
		testGregorianSince( 31, Date::DECEMBER, 1900 );
		testGregorianSince( 11, Date::JUNE, 1964 );

		{
			gak::Date	theDate = gak::Date::getEastern( 2018 );

			UT_ASSERT_EQUAL( 1, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::APRIL, theDate.getMonth() );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Ostersonntag") );
			UT_ASSERT_EQUAL( 0., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate = gak::Date::getEastern( 2000 );

			UT_ASSERT_EQUAL( 23, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::APRIL, theDate.getMonth() );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Ostersonntag") );
			UT_ASSERT_EQUAL( 0., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate = gak::Date::getEastern( 1900 );

			UT_ASSERT_EQUAL( 15, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::APRIL, theDate.getMonth() );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Ostersonntag") );
			UT_ASSERT_EQUAL( 0., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate = gak::Date::getMothersDay( 1900 );

			UT_ASSERT_EQUAL( 13, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MAY, theDate.getMonth() );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Muttertag") );
			UT_ASSERT_EQUAL( 0., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate = gak::Date::getMothersDay( 2018 );

			UT_ASSERT_EQUAL( 13, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MAY, theDate.getMonth() );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Muttertag") );
			UT_ASSERT_EQUAL( 0., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate(6, Date::JANUARY, 2018 );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Hl. Drei Könige") );
			UT_ASSERT_EQUAL( 0., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate(24, Date::DECEMBER, 2018 );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("Hl. Abend") );
			UT_ASSERT_EQUAL( 0.5, theDate.isWorkDay() );
		}
		{
			gak::Date	theDate(11, Date::JUNE, 1964 );
			UT_ASSERT_EQUAL( theDate.holiday(), static_cast<const char*>("") );
			UT_ASSERT_EQUAL( 1., theDate.isWorkDay() );
		}
		{
			gak::Date	theDate(11, Date::JUNE, 1964 );
			UT_ASSERT_EQUAL( Date::THURSDAY, theDate.weekDay() );
		}
		{
			gak::Date	theDate(11, Date::JUNE, 1964 );
			UT_ASSERT_EQUAL( (unsigned short)163, theDate.dayOfYear() );
		}
		{
			gak::Date	theDate(11, Date::JUNE, 2000 );
			UT_ASSERT_EQUAL( (unsigned short)163, theDate.dayOfYear() );
		}
		{
			gak::Date	theDate(11, Date::JUNE, 1900 );
			UT_ASSERT_EQUAL( (unsigned short)162, theDate.dayOfYear() );
		}
		{
			gak::Date	theDate( 28, Date::FEBRUARY, 1900 );
			theDate.increment( 1 );
			UT_ASSERT_EQUAL( 1, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MARCH, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate.decrement( 1 );
			UT_ASSERT_EQUAL( 28, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::FEBRUARY, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			++theDate;
			UT_ASSERT_EQUAL( 1, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MARCH, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			--theDate;
			UT_ASSERT_EQUAL( 28, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::FEBRUARY, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate++;
			UT_ASSERT_EQUAL( 1, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MARCH, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate--;
			UT_ASSERT_EQUAL( 28, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::FEBRUARY, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate += 1;
			UT_ASSERT_EQUAL( 1, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MARCH, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate -= 1;
			UT_ASSERT_EQUAL( 28, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::FEBRUARY, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate = theDate + 1;
			UT_ASSERT_EQUAL( 1, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::MARCH, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
			theDate = theDate - 1;
			UT_ASSERT_EQUAL( 28, int(theDate.getDay()) );
			UT_ASSERT_EQUAL( Date::FEBRUARY, theDate.getMonth() );
			UT_ASSERT_EQUAL( 1900, int(theDate.getYear()) );
		}
	}
};


// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static DateTest		myDateTest;

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

