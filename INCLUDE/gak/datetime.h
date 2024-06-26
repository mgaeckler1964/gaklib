/*
		Project:		GAKLIB
		Module:			datetime.h
		Description:	Date and time handling class
		Author:			Martin G�ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Germany, Munich ``AS IS''
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
#ifndef GAK_DATE_TIME_H
#define GAK_DATE_TIME_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>

#ifdef _Windows
#	ifndef STRICT
#		define STRICT 1
#	endif
#include <windows.h>
#endif

#ifdef __GNU__
#include <sys/time.h>
#endif

#include <gak/date.h>
#include <gak/time.h>
#include <gak/string.h>
#include <gak/exception.h>

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

// spring 1970 (utc): 21. 3. 00:56
static const time_t SPRING1970 = 6828960;
// summer 1970 (utc): 21. 6. 19:42
static const time_t SUMMER1970 = 14845320;
// autum  1970 (utc): 23. 9. 10:58
static const time_t AUTUMN1970 = 22935480;
// winter 1970 (utc): 22.12. 06:35
static const time_t WINTER1970 = 30695700;

// spring 2022 (utc): 20. 3. 15:33
static const time_t SPRING2022 = 1647790380;
// summer 2022 (utc): 21. 6. 09:13
static const time_t SUMMER2022 = 1655802780;
// autum  2022 (utc): 23. 9. 01:03XXXXXXXXXXXXX
static const time_t AUTUMN2022 = 1663894980;
// winter 2022 (utc): 21.12. 21:48
static const time_t WINTER2022 = 1671659280;

static const time_t SPRINGYEAR = (SPRING2022-SPRING1970)/52;
static const time_t SUMMERYEAR = (SUMMER2022-SUMMER1970)/52;
static const time_t AUTUMNYEAR = (AUTUMN2022-AUTUMN1970)/52;
static const time_t WINTERYEAR = (WINTER2022-WINTER1970)/52;

static const time_t SEASONYEAR = (SPRING2022-SPRING1970+SUMMER2022-SUMMER1970+AUTUMN2022-AUTUMN1970+WINTER2022-WINTER1970)/(52*4);
static const time_t MEANYEAR = (365*24+6)*3600;

static const time_t SPRINGDURATION70 = SUMMER1970-SPRING1970;
static const time_t SUMMERDURATION70 = AUTUMN1970-SUMMER1970;
static const time_t AUTUMNDURATION70 = WINTER1970-AUTUMN1970;

static const time_t SPRINGDURATION22 = SUMMER2022-SPRING2022;
static const time_t SUMMERDURATION22 = AUTUMN2022-SUMMER2022;
static const time_t AUTUMNDURATION22 = WINTER2022-AUTUMN2022;

// Lunation	new moon (utc)		full moon (utc) 
//  587		04. 06. 1970 02:21	19. 06. 1970 12:27
//			13314060			14646420
static const int LUNATION_1 = 587;
static const time_t NEW_MOON_1 = 13314060;
static const time_t FULL_MOON_1 = 14646420;
static const time_t HALF_MOON_1 = FULL_MOON_1 - NEW_MOON_1;

// 1256		05. 07. 2024 22:57	21. 07. 2024 10:17
//			1720220220			1721557020		
static const int LUNATION_2 = 1256;
static const time_t NEW_MOON_2 = 1720224660;		// TODO: check difference? with the orig time from a moon calendar I get diffs
static const time_t FULL_MOON_2 = 1721557020;
static const time_t HALF_MOON_2 = FULL_MOON_2 - NEW_MOON_2;

static const int LUNATION_COUNT = LUNATION_2-LUNATION_1;
static const time_t NEW_MOON_TIME = NEW_MOON_2 - NEW_MOON_1;
static const time_t FULL_MOON_TIME = FULL_MOON_2 - FULL_MOON_1;

static const time_t NEW_MOON_PHASE = NEW_MOON_TIME/LUNATION_COUNT;
static const time_t FULL_MOON_PHASE = FULL_MOON_TIME/LUNATION_COUNT;
static const time_t AVG_MOON_PHASE = (FULL_MOON_TIME+NEW_MOON_TIME)/(2*LUNATION_COUNT);

static const time_t MOON_PHASE = ((((29*24)+12)*60)+44)*60+3;


// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

template <time_t SEASON70START, time_t SEAONYEAR>
time_t getLastSeasonStart( time_t utctime )
{
	const time_t sesondsSinceStart = utctime - SEASON70START;
	const time_t yearsSinceStart = sesondsSinceStart/SEAONYEAR;

	return SEASON70START + SEAONYEAR * yearsSinceStart;
}

template <time_t SEASON70START, time_t SEAONYEAR>
time_t getNextSeasonStart( time_t utctime )
{
	const time_t sesondsSinceStart = utctime - SEASON70START;
	const time_t yearsSinceStart = sesondsSinceStart/SEAONYEAR;

	return SEASON70START + SEAONYEAR * (yearsSinceStart+1);
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class IllegalInternetTimestamp : public LibraryException
{
	public:
	IllegalInternetTimestamp( const STRING &inetStr ) 
	: LibraryException( "Illegal Internet Timestamp: " + inetStr )
	{
	}
};

class DateTime : public Date, public Time
{
	private:
//	static int		localTimezone;
	static bool		s_localDaylight;

	/*
		when using the time zone offset
		the time part containts the UTC-time
	*/
	long	m_tzOffset;	// original time zome offset (incl. DST)

	STRING	getInetTimeStr( void ) const;
	void	calcTime( long tzOffset );
	long	calcTzOffset( void ) const;

	void makeEntry( time_t	timer )
	{
		setDate( timer );
		setTime( timer );

		m_tzOffset = calcTzOffset();
	}
	void makeEntry( time_t	timer, long tzOffset )
	{
		setDate( timer );
		setTime( timer );

		m_tzOffset = tzOffset;
	}
	void makeNow( void )
	{
		time_t timer;

		timer = time(NULL);

		makeEntry( timer );
	}


	static void initTimeZone( void );

	public:
	/*
		constructors alway use UTC time
	*/
	DateTime(
		unsigned char day, Month month, unsigned short year,
		unsigned char hour, unsigned char minute, unsigned char second,
		long tzOffset = 0
	) : Date(day, month, year ), Time(hour, minute, second )
	{
		initTimeZone();
		m_tzOffset = tzOffset;
	}
	DateTime(
		const Date &newDate, const Time &newTime,
		long tzOffset = 0
	) : Date( newDate ), Time( newTime )
	{
		initTimeZone();
		m_tzOffset = tzOffset;
	}
	DateTime( const STRING &inetTime ) : Date( true ), Time( true )
	{
		initTimeZone();
		setInetTime( inetTime );
	}
	DateTime() : Date( true ), Time( true )
	{
		initTimeZone();
		makeNow();
	}
	DateTime( time_t timer ) : Date( true ), Time( true )
	{
		initTimeZone();
		makeEntry( timer );
	}
	DateTime( time_t timer, long tzOffset ) : Date( true ), Time( true )
	{
		initTimeZone();
		makeEntry( timer, tzOffset );
	}

#if defined( __BORLANDC__ )
	DateTime( TDateTime source ) : Date( source ), Time( source )
	{
		initTimeZone();
		m_tzOffset = calcTzOffset();
		calcTime( m_tzOffset );
	}
	operator TDateTime ( void ) const
	{
		return Date::operator TDateTime() + Time::operator TDateTime();
	}
#endif
#ifdef _Windows
	DateTime( const FILETIME &utcTime ) : Date( true ), Time( true )
	{
		initTimeZone();
		setFileTime( utcTime );
	}
	DateTime( const SYSTEMTIME &sysTime ) : Date( true ), Time( true )
	{
		initTimeZone();
		setSystemTime( sysTime );
	}
	void setFileTime( const FILETIME &utcTime )
	{
		SYSTEMTIME	sysTime;

		FileTimeToSystemTime( &utcTime, &sysTime );

		setSystemTime( sysTime );
	}
	void setSystemTime( const SYSTEMTIME &sysTime )
	{
		setDate( (unsigned char)sysTime.wDay, Month(sysTime.wMonth), sysTime.wYear );
		setTime( (unsigned char)sysTime.wHour, (unsigned char)sysTime.wMinute, (unsigned char)sysTime.wSecond );
		m_tzOffset = calcTzOffset();
	}
	void getSystemTime( SYSTEMTIME *sysTime )
	{
		sysTime->wDay = getDay();
		sysTime->wMonth = static_cast<unsigned short>(getMonth());
		sysTime->wYear = getYear();
		sysTime->wHour = getHour();
		sysTime->wMinute = getMinute();
		sysTime->wSecond = getSecond();
		sysTime->wMilliseconds = 0;
	}
	void getFileTime( FILETIME *utcTime )
	{
		SYSTEMTIME	sysTime;
		getSystemTime( &sysTime );

		SystemTimeToFileTime( &sysTime, utcTime );
	}
#endif

	void toBinaryStream( std::ostream &stream ) const
	{
		Date::toBinaryStream( stream );
		Time::toBinaryStream( stream );
	}
	void fromBinaryStream( std::istream &stream )
	{
		Date::fromBinaryStream( stream );
		Time::fromBinaryStream( stream );
	}
	static DateTime now( void )
	{
		DateTime thisStamp;

		return thisStamp;
	}

	DateTime lastSpring() const
	{
		return DateTime( getLastSeasonStart<SPRING1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime lastSummer() const
	{
		return DateTime( getLastSeasonStart<SUMMER1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime lastAutumn() const
	{
		return DateTime( getLastSeasonStart<AUTUMN1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime lastWinter() const
	{
		return DateTime( getLastSeasonStart<WINTER1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime lastNewMoon() const
	{
		return DateTime( getLastSeasonStart<NEW_MOON_1,AVG_MOON_PHASE>(getUtcUnixSeconds()), 0 );
	}
	DateTime lastFullMoon() const
	{
		return DateTime( getLastSeasonStart<FULL_MOON_1,AVG_MOON_PHASE>(getUtcUnixSeconds()), 0 );
	}

	DateTime nextSpring() const
	{
		return DateTime( getNextSeasonStart<SPRING1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime nextSummer() const
	{
		return DateTime( getNextSeasonStart<SUMMER1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime nextAutumn() const
	{
		return DateTime( getNextSeasonStart<AUTUMN1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime nextWinter() const
	{
		return DateTime( getNextSeasonStart<WINTER1970,SEASONYEAR>(getUtcUnixSeconds()), 0 );
	}
	DateTime nextNewMoon() const
	{
		return DateTime( getNextSeasonStart<NEW_MOON_1,AVG_MOON_PHASE>(getUtcUnixSeconds()), 0 );
	}
	DateTime nextFullMoon() const
	{
		return DateTime( getNextSeasonStart<FULL_MOON_1,AVG_MOON_PHASE>(getUtcUnixSeconds()), 0 );
	}

	Season getSeason() const
	{
		Month month = getMonth();
		switch( month )
		{
		case JANUARY:
		case FEBRUARY:
			return S_WINTER;
		case APRIL:
		case MAY:
			return S_SPRING;
		case JULY:
		case AUGUST:
			return S_SUMMER;
		case OCTOBER:
		case NOVEMBER:
			return S_AUTUMN;

		case MARCH:
			{
				DateTime springStart = lastSpring();
				return springStart.getYear() == getYear() ? S_SPRING : S_WINTER;
			}

		case JUNE:
			{
				DateTime summerStart = lastSummer();
				return summerStart.getYear() == getYear() ? S_SUMMER : S_SPRING;
			}
		case SEPTEMBER:
			{
				DateTime autumnStart = lastAutumn();
				return autumnStart.getYear() == getYear() ? S_AUTUMN : S_SUMMER;
			}
		case DECEMBER:
			{
				DateTime winterStart = lastWinter();
				return winterStart.getYear() == getYear() ? S_WINTER : S_AUTUMN;
			}
		case UNDEFINED_MONTH:
		default:
			return S_UNKNOWN;
		}
	}
	int compare( const DateTime &dateTime ) const
	{
		unsigned long thisLong = Date::getLong();
		unsigned long otherLong = dateTime.Date::getLong();

		if( thisLong == otherLong )
		{
			thisLong = Time::getLong();
			otherLong = dateTime.Time::getLong();
		}
		return (int)(thisLong - otherLong);
	}
	bool operator < ( const DateTime &date ) const
	{
		return compare( date ) < 0;
	}
	bool operator <= ( const DateTime &date ) const
	{
		return compare( date ) <= 0;
	}
	bool operator == ( const DateTime &date ) const
	{
		return compare( date ) == 0;
	}
	int operator != ( const DateTime &date ) const
	{
		return compare( date );
	}
	bool operator >= ( const DateTime &date ) const
	{
		return compare( date ) >= 0;
	}
	bool operator > ( const DateTime &date ) const
	{
		return compare( date ) > 0;
	}

	const DateTime &operator = ( time_t timeVal )
	{
		makeEntry( timeVal );
		return *this;
	}
	time_t getUtcUnixSeconds( void ) const
	{
		return Date::getUtcUnixSeconds() + getDaySeconds();
	}
	long getTZoffset( void ) const
	{
		return m_tzOffset;
	}
	static long getTimezone()
	{
#if defined( __BORLANDC__ )
		return std::_timezone;
#elif defined( __MACH__ ) && !defined( __LP64__ )
		return 0;
#else
		return timezone;
#endif
	}

	static const char *getTimezoneName()
	{
#if defined( __BORLANDC__ )
		return std::_tzname[std::_daylight ? 1 : 0];
#elif defined( __MACH__ ) && !defined( __LP64__ )
		return "GMT";
#else
		return tzname[daylight ? 1 : 0];
#endif
	}

	STRING getInetTime( long tzOffset )  const;
	STRING getOriginalTime( void ) const
	{
		return getInetTime( m_tzOffset );
	}
	STRING getLocalTime( void ) const
	{
		return getInetTime( -getTimezone() - (s_localDaylight ? -3600 : 0));
	}
	STRING getUTCTime( void ) const
	{
		return getInetTime( 0 );
	}
	DateTime calcOriginalTime( void ) const
	{
		DateTime tmp = *this;
		tmp.calcTime( -m_tzOffset );

		return tmp;
	}
	DateTime calcLocalTime( void ) const
	{
		DateTime tmp = *this;
		tmp.calcTime( getTimezone()  - (s_localDaylight ? 3600 : 0 ) );

		return tmp;
	}
	void setInetTime( const STRING &inetTime );
};

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

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

inline std::ostream &operator << ( std::ostream &stream, const DateTime &dateTime )
{
	stream << dateTime.getOriginalTime();

	return stream;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
