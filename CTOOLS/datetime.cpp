/*
		Project:		GAKLIB
		Module:			datetime.cpp
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

bool DateTime::s_localDaylight = false;

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

void DateTime::initTimeZone( void )
{
	static bool first = true;
	if( first )
	{
		tzset();
		std::time_t	timer = time(NULL);
		std::tm		*tblock = localtime( &timer );
		s_localDaylight = tblock->tm_isdst > 0 ? true : false;

		first = false;
	}
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

STRING DateTime::getInetTimeStr( void ) const
{
	doEnterFunction("DateTime::getInetTimeStr");

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

long DateTime::calcTzOffset( void ) const
{
	long	tzOffset = -getTimezone();
	bool	isDst = hasDst();

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
	catch( std::exception &e )
	{
		throw IllegalInternetTimestamp( inetTime ).addErrorText( e );
	}
}

STRING DateTime::getInetTime( long tzOffset )  const
{
	doEnterFunction("DateTime::getInetTime");

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

