/*
		Project:		GAKLIB
		Module:			datetime.cpp
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

#include <gak/datetime.h>
#include <gak/ci_string.h>
#include <gak/t_string.h>
#include <gak/fmtNumber.h>
#include <gak/numericString.h>
#include <gak/logfile.h>
#include <gak/geometry.h>

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

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static const char *monthNames[] =
{
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

static const char *dayNames[] =
{
	"Sun",
	"Mon",
	"Tue",
	"Wen",
	"Thu",
	"Fri",
	"Sat",
};

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

void DateTime::initTimeZone()
{
	static bool first = true;
	if( first )
	{
		tzset();
		first = false;
	}
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

STRING DateTime::getInetTimeStr() const
{
	doEnterFunctionEx(gakLogging::llDetail, "DateTime::getInetTimeStr");

	STRING	inetTime = formatNumber( getDay(), 2 );

	inetTime += ' ';

	int month = getMonth();
	inetTime += monthNames[month-1];
	inetTime += ' ';

	inetTime += formatNumber( getYear(), 4 );
	inetTime += ' ';
	inetTime += formatNumber( getHour(), 2 );
	inetTime += ':';
	inetTime += formatNumber( getMinute(), 2 );
	inetTime += ':';
	inetTime += formatNumber( getSecond(), 2 );

	return inetTime;
}

void DateTime::calcTime( long tzOffset )
{
	short	hourOffset = (short)(tzOffset / 3600);
	short	minuteOffset = (short)((tzOffset % 3600) / 60);

	short	hour = getHour();
	short	minute = getMinute();
	short	second = getSecond();

	minute -= minuteOffset;
	while( minute >= 60 )
	{
		minute -= (short)60;
		hour += (short)1;
	}
	while( minute < 0 )
	{
		minute += (short)60;
		hour -= (short)1;
	}
	hour -= hourOffset;
	while( hour < 0 )
	{
		hour += (short)24;
		decrement( 1 );
	}
	while( hour >= 24 )
	{
		hour -= (short)24;
		increment( 1 );
	}

	setTime( (unsigned char)hour, (unsigned char)minute, (unsigned char)second );
}

long DateTime::calcTzOffset() const
{
	long	tzOffset = -getTimezone();
	bool	isDst = hasDst(getHour());

	if( isDst )
		tzOffset += 3600;
#if 0
	if( isDst != localDaylight )
	{
		if( isDst )
			tzOffset += 3600;
		else
			tzOffset -= 3600;
	}
#endif

	return tzOffset;
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

void DateTime::setDefaultTime( const STRING &defaultTime )
{
	try
	{
		T_STRING		dateTimeStr = defaultTime;
		T_STRING		dateStr = dateTimeStr.getFirstToken( " " );
		T_STRING		timeStr = dateTimeStr.getNextToken();

		STRING			dayStr = dateStr.getFirstToken(".:/");
		STRING			monthStr = dateStr.getNextToken();
		STRING			yearStr = dateStr.getNextToken();
		STRING			hourStr = timeStr.getFirstToken(".:/");
		STRING			minuteStr = timeStr.getNextToken();
		STRING			secondStr = timeStr.getNextToken();

		unsigned char	day = dayStr.getValueE<unsigned char>();
		unsigned		month = monthStr.getValueE<unsigned>();
		unsigned short	year = yearStr.getValueE<unsigned short>();
		unsigned char	hour = hourStr.getValueE<unsigned char>();
		unsigned char	minute = minuteStr.getValueE<unsigned char>();
		unsigned char	second = secondStr.getValueE<unsigned char>();

		setDate( day, Month(month), year );
		setTime( hour, minute, second );
	}

	catch( std::exception &e )
	{
		throw IllegalInternetTimestamp( defaultTime ).addErrorText( e );
	}
}

void DateTime::setInetTime( const STRING &inetTime )
{
	try
	{
		T_STRING	dateTimeStr = inetTime;

		size_t	commaPos = dateTimeStr.searchChar( ',' );
		if( commaPos != dateTimeStr.no_index )
			dateTimeStr += commaPos + 1;
		dateTimeStr.stripBlanks();

		STRING dayStr = dateTimeStr.getFirstToken( " " );
		dateTimeStr += dayStr.strlen();
		dateTimeStr.stripBlanks();

		CI_STRING monthStr = dateTimeStr.getFirstToken( " " );
		dateTimeStr += monthStr.strlen();
		dateTimeStr.stripBlanks();

		STRING yearStr = dateTimeStr.getFirstToken( " " );
		dateTimeStr += yearStr.strlen();
		dateTimeStr.stripBlanks();

		STRING timeStr = dateTimeStr.getFirstToken( " " );
		dateTimeStr += timeStr.strlen();
		dateTimeStr.stripBlanks();

		STRING	tzOffsetStr = dateTimeStr;

		unsigned char day = dayStr.getValueE<unsigned char>();
		unsigned char month;
		for( month = 0; month < 12; month++ )
		{
			if( monthStr == monthNames[month] )
			{
				break;
			}
		}
		month++;

		unsigned short year = yearStr.getValueE<unsigned short>();

		unsigned int hour, minute, second;
		sscanf( timeStr, "%u:%u:%u", &hour, &minute, &second );

		if( year < 50 )
		{
			year += (unsigned short)2000;
		}
		else if( year < 100 )
		{
			year += (unsigned short)1900;
		}

		setDate( day, Month(month), year );
		setTime(
			(unsigned char)hour,
			(unsigned char)minute,
			(unsigned char)second
		);

		const char	*tzname;
		long		tzOffset = getValue<long>( tzOffsetStr, &tzname);
		tzOffset = tzOffset / 100 * 3600 + (tzOffset % 100) * 60;
		calcTime( tzOffset );
		m_tzOffset = tzOffset;
	}
	catch( ... )
	{
		setDefaultTime(inetTime);	// try the default time
		// throw IllegalInternetTimestamp( inetTime ).addErrorText( e );
	}
}

STRING DateTime::getMailTime()  const
{
	doEnterFunctionEx(gakLogging::llDetail, "DateTime::getMailTime");

	STRING mailTime = dayNames[weekDay()];

	mailTime += ' ';
	mailTime += monthNames[getMonth()-1];
	mailTime += ' ';
	mailTime += formatNumber( getDay(), 2 );
	mailTime += ' ';
	mailTime += formatNumber( getHour(), 2 );
	mailTime += ':';
	mailTime += formatNumber( getMinute(), 2 );
	mailTime += ':';
	mailTime += formatNumber( getSecond(), 2 );
	mailTime += ' ';
	mailTime += formatNumber( getYear(), 4 );
	mailTime += " +0000";

	return mailTime;
}

STRING DateTime::getInetTime( long tzOffset )  const
{
	doEnterFunctionEx(gakLogging::llDetail, "DateTime::getInetTime");

	STRING	inetTime;
	char	buffer[16];

	if( tzOffset )
	{
		DateTime	tmp = *this;
		tmp.calcTime( -tzOffset );
		inetTime = tmp.getInetTimeStr();
	}
	else
	{
		inetTime = getInetTimeStr();
	}

	tzOffset = tzOffset / 3600 * 100 + (tzOffset % 3600) / 60;
	sprintf( buffer, "%04ld", math::abs(tzOffset) );

	inetTime += ' ';
	inetTime += tzOffset >= 0 ? '+' : '-';

	inetTime += buffer;

	return inetTime;
}

inline std::time_t j2ts(double j)
{
    return std::time_t((j - 2440587.5) * 86400 +0.5);
}

void DateTime::sunriseEquation(double lon, double lat, DateTime *sunrise, DateTime *sunset) const
{
	const double elevation = 0;
	// Julian day (days since 1.1.2000)
	long J_day = since() - Date(1, 2000 ).since();

	// Mean Solar Time
	double	mst = J_day + 0.0009 - lon / 360.0;

	// Solar mean anomaly
	double M_degrees = fmod(357.5291 + 0.98560028 * mst, 360);
	double M_radians = math::degree2radians(M_degrees);

	// Equation of center
	double C_degrees = 1.9148 * sin(M_radians) + 0.02 * sin(2 * M_radians) + 0.0003 * sin(3 * M_radians);

	// Ecliptic longitude
	double L_degrees = fmod(M_degrees + C_degrees + 180.0 + 102.9372, 360);
	double L_radians = math::degree2radians(L_degrees);

	// Solar transit time
	double J_transit = (2451545.0 + mst + 0.0053 * sin(M_radians) - 0.0069 * sin(2 * L_radians));

	// hour angle
	double sin_d = sin(L_radians) * sin(math::degree2radians(23.4397));
	double cos_d = cos(asin(sin_d));
	double latRad = math::degree2radians(lat);
	double some_cos = (sin(math::degree2radians(-0.833 - 2.076 * sqrt(elevation) / 60.0)) - sin(latRad) * sin_d) / (cos(latRad) * cos_d);
	double ha_radians = acos(some_cos);
	double ha_degrees = math::radians2degree(ha_radians);  // 0...180

	// sunrise and sunset
	double j_rise = J_transit - ha_degrees / 360;
	double j_set = J_transit + ha_degrees / 360;

#if 0
	std::cout << "Julian day " << J_day << std::endl;
	std::cout << "Mean solar time " << mst << std::endl;
	std::cout << "Solar mean anomaly " << M_radians << ' ' << M_degrees << '°' << std::endl;
	std::cout << "Equation of center " << C_degrees << '°' << std::endl;
	std::cout << "Ecliptic longitude " << L_radians << ' ' << L_degrees << '°' << std::endl;
	std::cout << "Solar transit time " << J_transit << std::endl;
	std::cout << "Hour angle " << ha_radians << ' ' << ha_degrees << '°' << std::endl;
#endif

	// convert to DateTime
	*sunrise = DateTime( j2ts(j_rise) );
	*sunset = DateTime( j2ts(j_set) );
}

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

