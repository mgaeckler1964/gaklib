/*
		Project:		GAKLIB
		Module:			Date.CPP
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdio.h>
#include <string.h>

#include <gak/date.h>
#include <gak/array.h>
#include <gak/fmtNumber.h>
#include <gak/bitfield.h>
#include <gak/dynamic.h>
#include <gak/operators.h>

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


struct HOLIDAY
{
	short			mode;	// 0: relative to fix,
							// 1: relative to eastern,
							// 2: relative mother's day
	unsigned char	day;
	Date::Month		month;
	short			offset;
	Date::WeekDay	weekDay;
	double			workDay;
	char			name[64];
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static Bitfield			workDays;
static Array<HOLIDAY>	holidays;

static HOLIDAY defaultHolidays[] =
{
	{ 0,  1, Date::JANUARY,           0, Date::UNDEFINED_DAY,   0,	"Neujahr" },
	{ 0,  6, Date::JANUARY,           0, Date::UNDEFINED_DAY,   0,	"Hl. Drei Könige" },
	{ 0,  1, Date::APRIL,            -7, Date::SUNDAY,	        1,	"Begin Sommerzeit" },
	{ 0,  1, Date::MAY,               0, Date::UNDEFINED_DAY,   0,	"Maifeiertag" },
	{ 0, 15, Date::AUGUST,            0, Date::UNDEFINED_DAY,   0,	"Maria Himmelfahrt" },
	{ 0,  3, Date::OCTOBER,           0, Date::UNDEFINED_DAY,   0,	"Tag der Einheit" },
	{ 0,  1, Date::NOVEMBER,         -7, Date::SUNDAY,	        1,	"Ende Sommerzeit" },
	{ 0,  1, Date::NOVEMBER,          0, Date::UNDEFINED_DAY,   0,	"Allerheiligen" },
	{ 0,  6, Date::DECEMBER,          0, Date::UNDEFINED_DAY,   1,	"Nikolaus" },
	{ 0, 24, Date::DECEMBER,          0, Date::UNDEFINED_DAY, 0.5,	"Hl. Abend" },
	{ 0, 25, Date::DECEMBER,          0, Date::UNDEFINED_DAY,   0,	"1. Weihnachtstag" },
	{ 0, 25, Date::DECEMBER,         -7, Date::SUNDAY,          0,	"4. Advent" },
	{ 0, 25, Date::DECEMBER,        -14, Date::SUNDAY,          0,	"3. Advent" },
	{ 0, 25, Date::DECEMBER,        -21, Date::SUNDAY,          0,	"2. Advent" },
	{ 0, 25, Date::DECEMBER,        -28, Date::SUNDAY,          0,	"1. Advent" },
	{ 0, 25, Date::DECEMBER,        -35, Date::SUNDAY,          0,	"Totensonntag" },
	{ 0, 25, Date::DECEMBER,        -39, Date::SUNDAY,          0,	"Buß- und Bettag" },
	{ 0, 25, Date::DECEMBER,        -42, Date::SUNDAY,          0,	"Volkstrauertag" },
	{ 0, 26, Date::DECEMBER,          0, Date::UNDEFINED_DAY,   0,	"2. Weihnachtstag" },
	{ 0, 31, Date::DECEMBER,          0, Date::UNDEFINED_DAY, 0.5,	"Silvester" },
	{ 1,  0, Date::UNDEFINED_MONTH, -48, Date::UNDEFINED_DAY,   1,	"Rosenmontag" },
	{ 1,  0, Date::UNDEFINED_MONTH, -47, Date::UNDEFINED_DAY,   1,	"Faschingsdienstag" },
	{ 1,  0, Date::UNDEFINED_MONTH, -46, Date::UNDEFINED_DAY,   1,	"Aschermittwoch" },
	{ 1,  0, Date::UNDEFINED_MONTH,  -3, Date::UNDEFINED_DAY,   1,	"Gründonnerstag" },
	{ 1,  0, Date::UNDEFINED_MONTH,  -2, Date::UNDEFINED_DAY,   0,	"Karfreitag" },
	{ 1,  0, Date::UNDEFINED_MONTH,   0, Date::UNDEFINED_DAY,   0,	"Ostersonntag" },
	{ 1,  0, Date::UNDEFINED_MONTH,   1, Date::UNDEFINED_DAY,   0,	"Ostermontag" },
	{ 1,  0, Date::UNDEFINED_MONTH,  39, Date::UNDEFINED_DAY,   0,	"Christi Himmelfahrt" },
	{ 1,  0, Date::UNDEFINED_MONTH,  49, Date::UNDEFINED_DAY,   0,	"Pfingstsonntag" },
	{ 1,  0, Date::UNDEFINED_MONTH,  50, Date::UNDEFINED_DAY,   0,	"Pfingstmontag" },
	{ 1,  0, Date::UNDEFINED_MONTH,  60, Date::UNDEFINED_DAY,   0,	"Fronleichnam" },
	{ 2,  0, Date::UNDEFINED_MONTH,   0, Date::UNDEFINED_DAY,   0,	"Muttertag" }
};

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

IllegalDate::IllegalDate( unsigned char day, unsigned char month, unsigned short year ) 
: LibraryException( "Illegal Date: " + formatNumber(day) + '.' + formatNumber(month) + '.' + formatNumber(year) )
{
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

void Date::loadHolidays( const char *fileName )
{
	int		c;

	if( holidays.size() )	// do nothing if loaded allready
	{
		return;
	}

	if( !fileName || !*fileName )
	{
		fileName = "xholidays.dat";
	}

	STDfile fp( fileName, "r" );
	if( fp )
	{
		int		mode, day, month, offset, weekDay;
		double	workDay;
		HOLIDAY	holiday;

		while( !feof( fp ) && !ferror( fp ) )
		{
			fscanf(
				fp, "%d,%d,%d,%d,%d,%lf,",
				&mode,
				&day,
				&month,
				&offset,
				&weekDay,
				&workDay
			);
			holiday.mode = short(mode);
			holiday.day = (unsigned char)day;
			holiday.month = Month(month);
			holiday.offset = short(offset);
			holiday.weekDay = WeekDay(weekDay);
			holiday.workDay = workDay;

			char *cp = holiday.name;
			while( (c=fgetc( fp )) != EOF )
			{
				if( c < ' ' )
				{	// any control character should terminate the string
					break;
				}

				*cp++ = (char)c;
			}
			*cp = '\0';

			holidays.createElement() = holiday;
		}
	}
	else
	{
		STDfile fp( fileName, "w" );
		if( fp )
		{
			for( size_t i=0; i<arraySize(defaultHolidays); i++ )
			{
				fprintf(
					fp, "%d,%d,%d,%d,%d,%f,%s\n",
					(int)defaultHolidays[i].mode,
					(int)defaultHolidays[i].day,
					(int)defaultHolidays[i].month,
					(int)defaultHolidays[i].offset,
					(int)defaultHolidays[i].weekDay,
					defaultHolidays[i].workDay,
					defaultHolidays[i].name 
				);
			}
		}
		for( size_t i=0; i<arraySize(defaultHolidays); i++ )
		{
			holidays.createElement() = defaultHolidays[i];
		}
	}
}

unsigned short Date::getEasternDayOfYear( unsigned short year )
{
	static unsigned short lastValue = 0, lastYear = 0;
	if( lastYear != year  )
	{
		int		gz, jhd, ksj, korr, so, epakte, n;

		gz = (year%19)+1;
		jhd = year/100+1;
		ksj = (3*jhd)/4-12;
		korr = (8*jhd+5)/25-5;
		so = (5*year)/4-ksj-10;
		epakte = (11*gz+20+korr-ksj) % 30;
		if ( (epakte == 25 && gz > 11) || epakte == 24 )
			epakte++;
		n = 44-epakte;
		if ( n < 21 )
			n = n + 30;
		n = n + 7 - (so+n) % 7;
		n += (GregorianTrait::isLeapYear( year ) ? 1 : 0)+ 59;

		lastYear = year;
		lastValue = (unsigned short)n;
	}
	return lastValue;
}

unsigned short Date::getMothersDayOfYear( unsigned short year )
{
	static unsigned short lastValue = 0, lastYear = 0;
	if( lastYear != year  )
	{
		unsigned short	tnum;
		Date			tmpDay( 30, APRIL, year );

		tnum = tmpDay.dayOfYear();
		tnum = static_cast<unsigned short>(tnum - tmpDay.weekDay() + 14);

		if ( tnum == getEasternDayOfYear(year)+49 )
		{
			tnum -= static_cast<unsigned short>(7);
		}

		lastYear = year;
		lastValue = (unsigned short)tnum;
	}
	return lastValue;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void Date::setDate( const DynamicVar &source )
{
	*this = source.getDate();
}

size_t Date::findHoliday( void ) const
{
	std::size_t i;
	Date		mothersDay = getMothersDay( getYear() );
	Date		eastern = getEastern( getYear() );
	Date		holiday( false );
	WeekDay		weekDay;

	if( !holidays.size() )
	{
		loadHolidays();
	}

	for( i=0; i<holidays.size(); i++ )
	{
		if( holidays[i].mode == 2 )				// relative to mother's day
		{
			holiday = mothersDay;
		}
		else if( holidays[i].mode == 1 )		// relative to eastern
		{
			holiday = eastern;
		}
		else if( holidays[i].mode == 0 )
		{
			holiday.setDate( holidays[i].day, holidays[i].month, getYear() );
		}

		if( holidays[i].weekDay != UNDEFINED_DAY )
		{
			// find next special week day
			weekDay = holiday.weekDay();
			if( weekDay < holidays[i].weekDay )
			{
				holiday += holidays[i].weekDay - weekDay;
			}
			else if( weekDay > holidays[i].weekDay )
			{
				holiday += 7- (weekDay-holidays[i].weekDay);
			}
		}

		// respect the offset
		if( holidays[i].offset > 0 )
		{
			holiday += unsigned(holidays[i].offset);
		}
		else if( holidays[i].offset < 0 )
		{
			holiday -= unsigned(-holidays[i].offset);
		}

		// compare with my time
		if( holiday.getDay() == getDay() && holiday.getMonth() == getMonth() )
		{
			// we found a holiday
			break;
		}
	}

	return i;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

double Date::isWorkDay( unsigned long sinceEpoch ) const
{
	if( !workDays )
	{
		workDays = workDays << MONDAY << TUESDAY << WENDSDAY << THURSDAY << FRIDAY;
	}
	if( !sinceEpoch )
	{
		sinceEpoch = since();
	}
	WeekDay	dayOfWeek = weekDay( sinceEpoch );

	double	workDay = workDays.test( dayOfWeek ) ? 1 : 0;

	if( workDay )
	{
		size_t	holiday = findHoliday();
		if( holiday < holidays.size() )
		{
			workDay = holidays[holiday].workDay;
		}
	}

	return workDay;
}

const char *Date::holiday( void ) const
{
	std::size_t	i=findHoliday();

	// return result if found
	if( i < holidays.size() )
	{
		return holidays[i].name;
	}
	else
	{
		return "";
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

double getNumWorkDays( const Date &start, const Date &end )
{
	double	numWorkdays = 0;
	unsigned long	since = start.since();
	for( Date day = start; day < end; ++day )
	{
		numWorkdays += day.isWorkDay( since++ );
	}
	return numWorkdays;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

