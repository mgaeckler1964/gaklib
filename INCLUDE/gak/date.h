/*
		Project:		GAKLIB
		Module:			Date.H
		Description:	Date handlig class
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

#ifndef GAK_DATE_H
#define GAK_DATE_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstdlib>
#include <ctime>

#include <iostream>

#if defined( __BORLANDC__ )
#include <system.hpp>
#include <SysUtils.hpp>
#endif

#include <gak/fmtNumber.h>
#include <gak/string.h>
#include <gak/iostream.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc

#	pragma warn -inl
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class DynamicVar;

class IllegalDate : public LibraryException
{
	public:
	IllegalDate( unsigned char day, unsigned char month, unsigned short year );
};

struct JulianTrait
{
	static const unsigned WEEKDAY_OFFSET = 4;

	static bool isLeapYear( unsigned short year )
	{
		return year % 4 == 0;
	}
	static unsigned getNumDays( unsigned short year )
	{
		return isLeapYear( year ) ? 366 : 365;
	}

	/**
		@return epoch days for 1.1.year
	*/
	static unsigned long days( unsigned short year )
	{
		unsigned long numDays = year * 365;

		if( year )
		{
			// add one for each leap year
			numDays += (year-1)/4 + 1;
		}
		return numDays;
	}
	static unsigned short years( unsigned long days, unsigned long *remainingResult )
	{
		if( days >= 366 )
		{
			days -= 366;
			unsigned short	numYears = static_cast<unsigned short>(1 + days / 365);
			long			remaining = days % 365;

			// substract one for each leap year
			remaining -= numYears/4;

			// correct
			while( remaining < 0 )
			{
				numYears--;
				remaining += getNumDays( numYears );
			}

			*remainingResult = remaining;
			return numYears;
		}
		else
		{
			*remainingResult = days;
			return 0;
		}
	}
};

struct GregorianTrait
{
	static const unsigned WEEKDAY_OFFSET = 6;

	/**
		@return The number of days since 1.1.1
	*/
	static bool isLeapYear( unsigned short year )
	{
		bool leap;

		if( year % 4 )
			leap = false;
		else if( year % 400 == 0 )
			leap = true;
		else if( year % 100 == 0 )
			leap = false;
		else
			leap = true;

		return leap;
	}
	static unsigned getNumDays( unsigned short year )
	{
		return isLeapYear( year ) ? 366 : 365;
	}

	/**
		@return epoch days for 1.1.year
	*/
	static unsigned long days( unsigned short year )
	{
		unsigned long numDays = year * 365;

		if( year )
		{
			--year;
			// add one for each leap year
			numDays += year/4 + 1;
			numDays -= year/100;
			numDays += year/400;
		}
		return numDays;
	}
	static unsigned short years( unsigned long days, unsigned long *remainingResult )
	{
		if( days >= 366 )
		{
			days -= 366;
			unsigned short	numYears = static_cast<unsigned short>(1 + days / 365);
			long			remaining = days % 365;

			// substract one for each leap year
			remaining -= numYears/4;
			remaining += numYears/100;
			remaining -= numYears/400;

			// correct
			while( remaining < 0 )
			{
				numYears--;
				remaining += getNumDays( numYears );
			}

			*remainingResult = remaining;
			return numYears;
		}
		else
		{
			*remainingResult = days;
			return 0;
		}
	}
};

template <typename DateTraitT>
class BasicDate
{
	public:
	enum WeekDay
	{
		SUNDAY, MONDAY, TUESDAY, WENDSDAY, THURSDAY, FRIDAY, SATURDAY, UNDEFINED_DAY
	};

	enum Month
	{
		UNDEFINED_MONTH,
		JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE,
		JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
	};
	enum Season
	{
		S_UNKNOWN, S_SPRING, S_SUMMER, S_AUTUMN, S_WINTER
	};

	private:
	unsigned char	m_month, m_day;
	unsigned short	m_year;

	unsigned long sinceUnix( void ) const
	{
		BasicDate<GregorianTrait> unixStart( 1, JANUARY, 1970 );

		return since() - unixStart.since();
	}

	private:
	static unsigned char daysInMonth( Month month, unsigned short year );
	static unsigned short dayOfYear( unsigned char day, Month month, unsigned short year );
	static const char *weekdays[];

    private:
    void initCurrentDate( void )
    {
	   std::time_t timer;

	   timer = time(NULL);

	   setDate( std::localtime(&timer) );
    }

	protected:

	unsigned long getLong( void ) const
	{
		return (unsigned long)((m_year << 16U) | (m_month << 8U) | m_day);
	}

	void setDate( const BasicDate<DateTraitT> &date )
	{
		m_day = date.m_day;
		m_month = date.m_month;
		m_year = date.m_year;
	}

	/*
		--------------------------------------------------------------------------------------
		default constructors
		--------------------------------------------------------------------------------------
	*/
	protected:
	BasicDate( bool noDefault )
	{
		if( !noDefault )
		{
			initCurrentDate();
		}
	}
	public:
	BasicDate()
	{
		initCurrentDate();
	}

	/*
		--------------------------------------------------------------------------------------
		from/to components
		--------------------------------------------------------------------------------------
	*/
	public:
	BasicDate( unsigned char day, Month month, unsigned short year )
	{
		setDate( day, month, year );
	}
	void setDate( unsigned char day, Month month, unsigned short year )
	{
		if( month >= JANUARY && month <= DECEMBER && day <= daysInMonth( month, year ) )
		{
			m_day   = day;
			m_month = static_cast<unsigned char>(month);
			m_year  = year;
		}
		else
		{
			m_day   = 1;
			m_month = 1;
			m_year  = 0;

			throw IllegalDate(day, static_cast<unsigned char>(month), year);
		}
	}
	void getDate( unsigned char *day, Month *month, unsigned short *year )	const
	{
		*day   = m_day;
		*month = Month(m_month);
		*year  = m_year;
	}

	unsigned char getDay( void ) const
	{
		return m_day;
	}
	Month getMonth( void ) const
	{
		return Month(m_month);
	}
	unsigned short getYear( void ) const
	{
		return m_year;
	}


	BasicDate( unsigned short dayOfYear, unsigned short year )
	{
		setDate( dayOfYear, year );
	}
	void setDate( unsigned short dayOfYear, unsigned short year )
	{
		m_day = 1;
		m_month = JANUARY;
		m_year = year;

		*this += (dayOfYear -1);
	}
	unsigned short dayOfYear( void ) const
	{
		return dayOfYear( m_day, Month(m_month), m_year );
	}

	/*
		--------------------------------------------------------------------------------------
		from/to epoch (1.1.0) in days
		--------------------------------------------------------------------------------------
	*/
	BasicDate( unsigned long days )
	{
		setDate( days );
	}
	unsigned long since( void ) const
	{
		return DateTraitT::days( m_year ) + dayOfYear()-1;
	}
	void setDate( unsigned long days )
	{
		unsigned long remaining;
		unsigned short year = DateTraitT::years( days, &remaining );
		setDate( static_cast<unsigned short>(remaining+1), year );
	}

#if defined( __BORLANDC__ )
	/*
		--------------------------------------------------------------------------------------
		from/to TDateTime
		--------------------------------------------------------------------------------------
	*/
	BasicDate( TDateTime source )
	{
		unsigned short day, month, year;
		source.DecodeDate( &year, &month, &day );
		setDate( (unsigned char)day, Month(month), year );
	}
	operator TDateTime ( void ) const
	{
		return ::EncodeDate( m_year, m_month, m_day );
	}
#endif
	/*
		--------------------------------------------------------------------------------------
		from/to std::time_t or struct tm
		--------------------------------------------------------------------------------------
	*/
	public:
	std::time_t getUtcUnixSeconds( void ) const
	{
		return sinceUnix()*24*3600;
	}
	void setDate( std::time_t time );

	void setDate( std::tm *tblock )
	{
		setDate( 
			(unsigned char)tblock->tm_mday, 
			Month(tblock->tm_mon+1), 
			(unsigned short)(tblock->tm_year + 1900) 
		);
	}





	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, m_day );
		gak::toBinaryStream( stream, m_month );
		gak::toBinaryStream( stream, m_year );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &m_day );
		gak::fromBinaryStream( stream, &m_month );
		gak::fromBinaryStream( stream, &m_year );
	}

	bool isValid( void ) const;

	unsigned weekOfYear( void ) const;
	static WeekDay weekDay( unsigned long since )
	{
		return WeekDay((since+DateTraitT::WEEKDAY_OFFSET) % 7);
	}

	WeekDay weekDay( void ) const
	{
		return weekDay( since() );
	}
	const char *weekDayName( void ) const
	{
		return weekdays[std::size_t(weekDay())];
	}
	unsigned char daysInMonth( void ) const
	{
		return daysInMonth( Month(m_month), m_year );
	}

	bool isLeapYear( void ) const
	{
		return isLeapYear( m_year );
	}

	void increment( unsigned days );
	BasicDate operator + ( unsigned days ) const
	{
		BasicDate result = *this;

		result += days;

		return result;
	}
	BasicDate operator += ( unsigned days )
	{
		increment( days );
		return *this;
	}
	const BasicDate &operator ++()				// pre inkrement
	{
		increment( 1 );
		return *this;
	}
	BasicDate operator ++( int )					// post inkrement
	{
		BasicDate	tmp( *this );
		increment( 1 );
		return tmp;
	}


	void decrement( unsigned days );
	BasicDate operator - ( unsigned days ) const
	{
		BasicDate result = *this;

		result -= days;

		return result;
	}
	BasicDate operator -= ( unsigned days )
	{
		decrement( days );
		return *this;
	}
	const BasicDate &operator --()				// pre dekrement
	{
		decrement( 1 );
		return *this;
	}
	BasicDate operator --( int )					// post dekrement
	{
		BasicDate	temp = *this;

		decrement( 1 );
		return temp;
	}

	int compare( const BasicDate &date ) const
	{
		unsigned long thisDate = getLong();
		unsigned long otherDate = date.getLong();

		return int(thisDate - otherDate);
	}
#if 0
	bool operator < ( const BasicDate &date ) const
	{
		return compare( date ) < 0;
	}
	bool operator <= ( const BasicDate &date ) const
	{
		return compare( date ) <= 0;
	}
	bool operator == ( const BasicDate &date ) const
	{
		return compare( date ) == 0;
	}
	bool operator != ( const BasicDate &date ) const
	{
		return compare( date ) != 0;
	}
	bool operator >= ( const BasicDate &date ) const
	{
		return compare( date ) >= 0;
	}
	bool operator > ( const BasicDate &date ) const
	{
		return compare( date ) > 0;
	}
#endif

	long operator - ( const BasicDate &date ) const
	{
		return since() - date.since();
	}

	bool hasDst( void ) const;

	STRING toString( void ) const;
};

class Date : public BasicDate<GregorianTrait>
{
	public:
	using BasicDate<GregorianTrait>::setDate;

	private:
	static void loadHolidays( const char *fileName = NULL );
	static unsigned short getEasternDayOfYear( unsigned short year );
	static unsigned short getMothersDayOfYear( unsigned short year );

	/*
		--------------------------------------------------------------------------------------
		default constructors
		--------------------------------------------------------------------------------------
	*/
	protected:
	Date( bool noDefault ) : BasicDate<GregorianTrait>( noDefault )
	{
	}
	public:
	Date() : BasicDate<GregorianTrait>()
	{
	}
	/*
		--------------------------------------------------------------------------------------
		from/to components
		--------------------------------------------------------------------------------------
	*/
	public:
	Date( unsigned char day, Month month, unsigned short year ) : BasicDate<GregorianTrait>( day, month, year )
	{
	}
	Date( unsigned short dayOfYear, unsigned short year ) : BasicDate<GregorianTrait>( dayOfYear, year )
	{
	}

	/*
		--------------------------------------------------------------------------------------
		from/to epoch (1.1.0) in days
		--------------------------------------------------------------------------------------
	*/
	Date( unsigned long days ) : BasicDate<GregorianTrait>( days )
	{
	}

	/*
		--------------------------------------------------------------------------------------
		from/to dynamic var
		--------------------------------------------------------------------------------------
	*/
	private:
	void setDate( const DynamicVar &source );
	public:
	Date( const DynamicVar &source ) : BasicDate<GregorianTrait>( false )
	{
		setDate( source );
	}
	const Date &operator = ( const DynamicVar &source )
	{
		setDate( source );
		return *this;
	}

#if defined( __BORLANDC__ )
	/*
		--------------------------------------------------------------------------------------
		from/to TDateTime
		--------------------------------------------------------------------------------------
	*/
	Date( TDateTime source ) : BasicDate<GregorianTrait>( source )
	{
	}
#endif

	private:
	std::size_t findHoliday( void ) const;

	public:
	const Date &operator = ( const BasicDate<GregorianTrait> &source )
	{
		setDate( source );
		return *this;
	}

	static Date getEastern( unsigned short year )
	{
		return Date( getEasternDayOfYear( year ), year );
	}
	static Date getMothersDay( unsigned short year )
	{
		return Date( getMothersDayOfYear( year ), year );
	}

	const char *holiday( void ) const;
	double isWorkDay( unsigned long since=0 ) const;
};

typedef BasicDate<JulianTrait>		JulianDate;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

template<typename DateTraitT>
const char *BasicDate<DateTraitT>::weekdays[] =
{
	"Sonntag",
	"Montag",
	"Dienstag",
	"Mittwoch",
	"Donnerstag",
	"Freitag",
	"Samstag",
	""
};

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

template<typename DateTraitT>
unsigned char BasicDate<DateTraitT>::daysInMonth( Month month, unsigned short year )
{
static unsigned	char days[]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if( month < JANUARY || month > DECEMBER )
	{
		return 0;
	}
	if( month == FEBRUARY )
	{
		return (unsigned char)(DateTraitT::isLeapYear( year ) ? 29 : 28);
	}

	return days[month];
}

template<typename DateTraitT>
unsigned short BasicDate<DateTraitT>::dayOfYear( unsigned char day, Month month, unsigned short year )
{
	static unsigned	short days[]={0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

	if( month < JANUARY || month > DECEMBER )
	{
		return 0;
	}

	unsigned short numDays = days[month];

	numDays += day;

	if( month > FEBRUARY && DateTraitT::isLeapYear( year ) )
	{
		numDays++;
	}

	return numDays;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template<typename DateTraitT>
unsigned BasicDate<DateTraitT>::weekOfYear( void ) const
{
	unsigned theWeek;

	BasicDate<DateTraitT>	firstDayOfYear( 1, JANUARY, m_year );
	WeekDay					theFirstWeekDay = firstDayOfYear.weekDay();
	unsigned short			theDayOfYear = 	dayOfYear();


	if( theFirstWeekDay <= WENDSDAY )
	{
		theDayOfYear += static_cast<unsigned short>(theFirstWeekDay);
	}
	else
	{
		theDayOfYear -= static_cast<unsigned short>(7-theFirstWeekDay);
	}

	theWeek = unsigned(theDayOfYear/7 + 1);
	if( theWeek <= 0 )
	{
		theWeek += 52;
	}

	return theWeek;	
}

template<typename DateTraitT>
void BasicDate<DateTraitT>::setDate( std::time_t time )
{
	unsigned short	tmpYear;

	int	numDays = int(time / (24*3600));
	if( numDays > 2*365+366 )	// > 1972
	{
		tmpYear = 1973;
		numDays -= 2*365+366;
	}
	else if( numDays > 2*365 )
	{
		tmpYear = 1972;
		numDays -= 2*365;
	}
	else
	{
		tmpYear = 1970;
	}
	int numYears = (numDays * 100) / 36525;
	tmpYear += (unsigned short)numYears;
	unsigned short dayOfYear = (unsigned short)(
		numDays - long(double(numYears) * 365.25 +0.01)
	);
	dayOfYear++;
	setDate( dayOfYear, tmpYear );
}

template<typename DateTraitT>
bool BasicDate<DateTraitT>::isValid( void ) const
{
	bool valid;

	if( m_month >= JANUARY && m_month <= DECEMBER
	&&  m_day > 0U && m_day <= daysInMonth() )
		valid = true;
	else
		valid = false;

	return valid;
}

template<typename DateTraitT>
void BasicDate<DateTraitT>::increment( unsigned days )
{
	unsigned monthDays;

	days += m_day;
	while( days > (monthDays = unsigned(daysInMonth())) )
	{
		days -= monthDays;
		m_month++;
		if( m_month > DECEMBER )
		{
			m_month = JANUARY;
			m_year++;
		}
	}
	m_day = static_cast<unsigned char>(days);
}

template<typename DateTraitT>
void BasicDate<DateTraitT>::decrement( unsigned days )
{
	days = m_day - days;
	while( int(days) < 1 )
	{
		m_month--;
		if( !m_month )
		{
			m_year--;
			m_month = DECEMBER;
		}
		days += daysInMonth();
	}
	m_day = static_cast<unsigned char>(days);
}

template<typename DateTraitT>
bool BasicDate<DateTraitT>::hasDst( void ) const
{
	if( m_month < MARCH || m_month > OCTOBER )
	{
		return false;
	}

	if( m_month > MARCH && m_month < OCTOBER )
	{
		return true;
	}

	if( m_month == MARCH && m_day <= 31-7 )
	{
		return false;
	}

	if( m_month == OCTOBER && m_day <= 31-7 )
	{
		return true;
	}

	WeekDay wd = weekDay();

	bool afterLastSunday = ( m_day-wd > 24 );

	return m_month == MARCH ? afterLastSunday : !afterLastSunday;
}

template<typename DateTraitT>
STRING BasicDate<DateTraitT>::toString( void ) const
{
	STRING	result = formatNumber( m_day, 2 );

	result += '.';
	result += formatNumber( m_month, 2 );
	result += '.';
	result += formatNumber( m_year, 4 );

	return result;
}

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

double getNumWorkDays( const Date &start, const Date &end );

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template<typename DateTraitT>
inline std::ostream &operator << ( std::ostream &stream, const BasicDate<DateTraitT> &date )
{
	stream << date.toString();

	return stream;
}

#if 0
inline std::ostream &operator << ( std::ostream &stream, const JulianDate &date )
{
	stream << date.toString();

	return stream;
}
#endif

inline Date julian2GregorianDate( const JulianDate &julianDate )
{
	return Date( julianDate.since() - 2 );
}

inline JulianDate gregorian2JulianDate( const Date &gregorianDate )
{
	return JulianDate( gregorianDate.since() + 2 );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.

#	pragma warn +inl
#endif

#endif

