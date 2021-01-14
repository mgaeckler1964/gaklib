/*
		Project:		GAKLIB
		Module:			datetime.h
		Description:	Date and time handling class
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

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

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
	void makeNow( void )
	{
		time_t timer;

		timer = time(NULL);

		makeEntry( timer );
	}


	static void initTimeZone( void );

	public:
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
	DateTime( time_t timer) : Date( true ), Time( true )
	{
		initTimeZone();
		makeEntry( timer );
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
	time_t getUnixSeconds( void ) const
	{
		return Date::getUnixSeconds() + getDaySeconds();
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
