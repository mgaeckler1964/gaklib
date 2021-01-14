/*
		Project:		GAKLIB
		Module:			time.h
		Description:	time handlig class
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
#ifndef GAK_TIME_H
#define GAK_TIME_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <ctime>

#include <iostream>

#if defined( __BORLANDC__ )
#include <system.hpp> 
#include <SysUtils.hpp> 
#endif

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

class DynamicVar;

class Time
{
	unsigned char	hour, minute, second;

	private:
	void setCurrentTime( void )
	{
		std::time_t	timer;
		std::tm		*tblock;

		timer = time(NULL);

		tblock = std::localtime(&timer);
		setTime( tblock );
	}

	protected:
	unsigned long getLong( void ) const
	{
		return (unsigned long)((hour << 16U) | (minute << 8U) | second);
	}

	Time( bool noDefault )
	{
		if( !noDefault )
		{
			setCurrentTime();
		}
	}
	public:
	Time()
	{
		setCurrentTime();
	}
	Time( unsigned char hour, unsigned char minute, unsigned char second )
	{
		setTime( hour, minute, second );
	}
	explicit Time( unsigned long miliSecond )
	{
		setTime( miliSecond );
	}
	/*
		from dynamic var
	*/
	private:
	void setTime( const DynamicVar &source );
	public:
	Time( const DynamicVar &source )
	{
		setTime( source );
	}
	const Time &operator = ( const DynamicVar &source )
	{
		setTime( source );
		return *this;
	}
#if defined( __BORLANDC__ )
	/*
		from TDateTime
	*/
	Time( TDateTime source )
	{
		unsigned short hour, minutes, secconds, milliSec;
		source.DecodeTime( &hour, &minutes, &secconds, &milliSec );
		setTime(
			(unsigned char)hour, (unsigned char)minutes, (unsigned char)secconds
		);
	}
	operator TDateTime ( void )
	{
		return ::EncodeTime( hour, minute, second, 0 );
	}
#endif
	void setTime( unsigned char hour, unsigned char minute, unsigned char second )
	{
		this->hour = hour;
		this->minute = minute;
		this->second = second;
	}
	void setTime( std::tm *tblock )
	{
	   setTime(
		(unsigned char)tblock->tm_hour,
		(unsigned char)tblock->tm_min,
		(unsigned char)tblock->tm_sec
	   );
	}
	void setTime( unsigned long miliSeconds )
	{
		miliSeconds += 500;
		miliSeconds /= 1000;
		second = (unsigned char)(miliSeconds % 60);
		miliSeconds /= 60;
		minute = (unsigned char)(miliSeconds % 60);
		hour = (unsigned char)(miliSeconds / 60);
	}
	void setTime( std::time_t timer )
	{
		timer = timer % (24*3600);

		second = (unsigned char)(timer % 60);
		timer /= 60;
		minute = (unsigned char)(timer % 60);
		hour = (unsigned char)(timer / 60);
	}
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, hour );
		gak::toBinaryStream( stream, minute );
		gak::toBinaryStream( stream, second );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &hour );
		gak::fromBinaryStream( stream, &minute );
		gak::fromBinaryStream( stream, &second );
	}
	STRING toString( void ) const;
	int compare( const Time &time ) const
	{
		unsigned long	thisTime = getLong();
		unsigned long	otherTime = time.getLong();

		return (int)(thisTime - otherTime);
	}
	bool operator < ( const Time &time ) const
	{
		return compare( time ) < 0;
	}
	bool operator <= ( const Time &time ) const
	{
		return compare( time ) <= 0;
	}
	bool operator == ( const Time &time ) const
	{
		return compare( time ) == 0;
	}
	bool operator != ( const Time &time ) const
	{
		return compare( time ) != 0;
	}
	bool operator >= ( const Time &time ) const
	{
		return compare( time ) >= 0;
	}
	bool operator > ( const Time &time ) const
	{
		return compare( time ) > 0;
	}

	unsigned char getHour( void ) const
	{
		return hour;
	}
	unsigned char getMinute( void ) const
	{
		return minute;
	}
	unsigned char getSecond( void ) const
	{
		return second;
	}
	time_t getDaySeconds( void ) const
	{
		return hour*3600+minute*60+second;
	}
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

inline std::ostream &operator << ( std::ostream &stream, const Time &time )
{
	stream << time.toString();

	return stream;
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
