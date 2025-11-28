/*
		Project:		GAKLIB
		Module:			stopWatch.h
		Description:	A stop watch for user and cpu time
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

#ifndef GAK_STOP_WATCH_H
#define GAK_STOP_WATCH_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <ctime>
#include <gak/fmtNumber.h>

#if defined( __MACH__ ) || defined( __unix__ )
#	include <sys/time.h>
#endif

#include <gak/string.h>

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

/**
	@brief Class storing a period of time in milliseconds
	@tparam BaseT Scalar type storing a tick
*/
template <typename BaseT=std::clock_t>
class MilliSeconds
{
	public:
	static const BaseT	MILLIS_PER_SECOND = 1000;
	static const BaseT	SECONDS_PER_MINUTE = 60;
	static const BaseT	MINUTES_PER_HOUR = 60;
	static const BaseT	HOURS_PER_DAY = 24;
	static const BaseT	DAYS_PER_WEEK = 7;

	static const BaseT	MILLIS_PER_MINUTE = MILLIS_PER_SECOND * SECONDS_PER_MINUTE;
	static const BaseT	MILLIS_PER_HOUR = MILLIS_PER_MINUTE * MINUTES_PER_HOUR;
	static const BaseT	MILLIS_PER_DAY = MILLIS_PER_HOUR * HOURS_PER_DAY;
	static const BaseT	MILLIS_PER_WEEK = MILLIS_PER_DAY * DAYS_PER_WEEK;

	private:
	BaseT				m_milliSeconds;

	public:
	/// copy construktor
	MilliSeconds( const MilliSeconds<BaseT> &source ) : m_milliSeconds(source.m_milliSeconds)
	{
	}

	/// constructs a time value from a scalar type in milliseconds
	explicit MilliSeconds( BaseT milliSeconds=BaseT() ) : m_milliSeconds(milliSeconds)
	{
	}
	/// assigns a time value from a scalar type in milliseconds
	const MilliSeconds &operator = ( BaseT milliSeconds )
	{
		m_milliSeconds = milliSeconds;
		return *this;
	}

	/// copy assignment
	const MilliSeconds &operator = ( const MilliSeconds<BaseT> &source )
	{
		m_milliSeconds = source.m_milliSeconds;
		return *this;
	}

	/// returns the total number of milliseconds
	BaseT get( void ) const
	{
		return m_milliSeconds;
	}
	/// returns the total number of milliseconds
	BaseT asMillis() const
	{
		return m_milliSeconds;
	}
	/// returns the total number of seconds
	BaseT asSeconds() const
	{
		return get() / MILLIS_PER_SECOND;
	}
	/// returns the total number of minutes
	BaseT asMinutes() const
	{
		return get() / MILLIS_PER_MINUTE;
	}
	/// returns the total number of hours
	BaseT asHours() const
	{
		return get() / MILLIS_PER_HOUR;
	}
	/// returns the total number of days
	BaseT asDays() const
	{
		return get() / MILLIS_PER_DAY;
	}

	/// returns the total number of weeks
	BaseT asWeeks() const
	{
		return get() / MILLIS_PER_WEEK;
	}

	/// returns the number of milli seconds that are less than a seconds
	BaseT millisPart( void ) const
	{
		return get() % MILLIS_PER_SECOND;
	}
	/// returns the number of seconds that are less than a minute
	BaseT secondsPart( void ) const
	{
		return asSeconds() % SECONDS_PER_MINUTE;
	}
	/// returns the number of minutes that are less than a hour
	BaseT minutesPart( void ) const
	{
		return asMinutes() % MINUTES_PER_HOUR;
	}
	/// returns the number of hours that are less than a day
	BaseT hoursPart( void ) const
	{
		return asHours() % HOURS_PER_DAY;
	}
	/// returns the number of days that are less than a week
	BaseT daysPart( void ) const
	{
		return asDays() % DAYS_PER_WEEK;
	}
	/// returns the number of weeks
	BaseT weeksPart( void ) const
	{
		return asWeeks();
	}

	/**
		returns the string representation of the ellapsed time
		@param part bool if true seconds are truncated from the string
	*/
	STRING toString( bool part=false ) const
	{
#ifdef __BORLANDC__
#	pragma warn -inl
#endif
		return formatNumber( part ? millisPart() : get() ) + "ms";
#ifdef __BORLANDC__
#	pragma warn +inl
#endif
	}

	/// + operator
	MilliSeconds<BaseT> operator + ( const MilliSeconds<BaseT> &oper ) const
	{
		return MilliSeconds<BaseT>( get() + oper.get() );
	}
};

/**
	@brief Class storing a period of time in seconds
	@tparam BaseT Scalar type storing a tick
*/
template <typename BaseT=std::clock_t>
class Seconds : public MilliSeconds<BaseT>
{
	public:
	/// copy constructor
	Seconds( const MilliSeconds<BaseT> &src ) : MilliSeconds<BaseT>(src)
	{
	}
	/// constructs a time value from a scalar type in seconds
	explicit Seconds( BaseT seconds=BaseT() ) : MilliSeconds<BaseT>(seconds*Seconds<BaseT>::MILLIS_PER_SECOND)
	{
	}
	/// assigns a scalar type value as number of seconds to the ellapsed time
	const Seconds &operator = ( BaseT milliSeconds )
	{
		MilliSeconds<BaseT>::operator = ( milliSeconds*this->MILLIS_PER_SECOND );
		return *this;
	}
	/// returns the ellapse time in seconds
	BaseT get( void ) const
	{
		return this->asSeconds();
	}
	/**
		returns the string representation of the ellapsed time
		@param part bool if true minutes are truncated from the string
	*/
	STRING toString( bool part=false ) const
	{
#ifdef __BORLANDC__
#	pragma warn -inl
#endif
		return formatNumber( part ? this->secondsPart() : get() ) + "s " + MilliSeconds<BaseT>::toString( true );
#ifdef __BORLANDC__
#	pragma warn +inl
#endif
	}
};

/**
	@brief Class storing a period of time in minutes
	@tparam BaseT Scalar type storing a tick
*/
template <typename BaseT=std::clock_t>
class Minutes : public Seconds<BaseT>
{
	public:
	/// copy constructor
	Minutes( const MilliSeconds<BaseT> &src ) : Seconds<BaseT>(src)
	{
	}
	/// constructs from scalar type value as number of minutes to the ellapsed time
	explicit Minutes( BaseT minutes=BaseT() ) : Seconds<BaseT>(minutes*Minutes<BaseT>::SECONDS_PER_MINUTE)
	{
	}
	/// assigns a scalar type value as number of minutes to the ellapsed time
	const Minutes &operator = ( BaseT minutes )
	{
		Seconds<BaseT>::operator = ( minutes*this->SECONDS_PER_MINUTE );
		return *this;
	}
	/// returns the ellapse time in minutes
	BaseT get( void ) const
	{
		return this->asMinutes();
	}
	/**
		returns the string representation of the ellapsed time
		@param part bool if true hours are truncated from the string
	*/
	STRING toString( bool part=false ) const
	{
#ifdef __BORLANDC__
#	pragma warn -inl
#endif
		return formatNumber( part ? this->minutesPart() : get() ) + "m " + Seconds<BaseT>::toString( true );
#ifdef __BORLANDC__
#	pragma warn +inl
#endif
	}
};

/**
	@brief Class storing a period of time in hour
	@tparam BaseT Scalar type storing a tick
*/
template <typename BaseT=std::clock_t>
class Hours : public Minutes<BaseT>
{
	public:
	/// copy constructor
	Hours( const MilliSeconds<BaseT> &src ) : Minutes<BaseT>(src)
	{
	}
	/// constructs a time value from a scalar type in hours
	explicit Hours( BaseT hours=BaseT() ) : Minutes<BaseT>(hours*Hours<BaseT>::MINUTES_PER_HOUR)
	{
	}
	/// assigns a scalar type value as number of hours to the ellapsed time
	const Hours &operator = ( BaseT hours )
	{
		Minutes<BaseT>::operator = ( hours*this->MINUTES_PER_HOUR );
		return *this;
	}
	/// returns the ellapse time in hours
	BaseT get( void ) const
	{
		return this->asHours();
	}
	/**
		returns the string representation of the ellapsed time
		@param part bool if true days are truncated from the string
	*/
	STRING toString( bool part=false ) const
	{
#ifdef __BORLANDC__
#	pragma warn -inl
#endif
		return formatNumber( part ? this->hoursPart() : get() ) + "h " + Minutes<BaseT>::toString( true );
#ifdef __BORLANDC__
#	pragma warn +inl
#endif
	}
};

/**
	@brief Class storing a period of time in days
	@tparam BaseT Scalar type storing a tick
*/
template <typename BaseT=std::clock_t>
class Days : public Hours<BaseT>
{
	public:
	/// copy constructor
	Days( const MilliSeconds<BaseT> &src ) : Hours<BaseT>(src)
	{
	}
	/// constructs from scalar type value as number of days to the ellapsed time
	explicit Days( BaseT days=BaseT() ) : Hours<BaseT>(days*Days<BaseT>::HOURS_PER_DAY)
	{
	}
	/// assigns a scalar type value as number of days to the ellapsed time
	const Days &operator = ( BaseT days )
	{
		Hours<BaseT>::operator = ( days*this->HOURS_PER_DAY );
		return *this;
	}
	/// returns the scalar representing the ellapsed time in days
	BaseT get( void ) const
	{
		return this->asDays();
	}
	/**
		returns the string representation of the ellapsed time
		@param part bool if true weeks are truncated from the string
	*/
	STRING toString( bool part=false ) const
	{
		return formatNumber( part ? this->daysPart() : get() ) + "d " + Hours<BaseT>::toString( true );
	}
};

/**
	@brief Class storing a period of time in weeks
	@tparam BaseT Scalar type storing a tick
*/
template <typename BaseT=std::clock_t>
class Weeks : public Days<BaseT>
{
	public:
	/// copy constructor
	Weeks( const MilliSeconds<BaseT> &src ) : Days<BaseT>(src)
	{
	}
	/// constructs from scalar type value as number of weeks to the ellapsed time
	explicit Weeks( BaseT weeks=BaseT() ) : Days<BaseT>(weeks*Weeks<BaseT>::DAYS_PER_WEEK)
	{
	}
	/// assigns a scalar type value as number of weeks to the ellapsed time
	const Weeks &operator = ( BaseT weeks )
	{
		Days<BaseT>::operator = ( weeks*this->DAYS_PER_WEEK );
		return *this;
	}
	/// returns the scalar representing the ellapsed time in week
	BaseT get( void ) const
	{
		return this->asWeeks();
	}
	/// returns the string representation of the ellapsed time
	STRING toString() const
	{
		return formatNumber( get() ) + "w " + Days<BaseT>::toString( true );
	}
};

/// helper class used to convert clock ticks as returned by std::clock in miliseconds
struct TimeConverter
{
	/// converts the clock ticks in milliseconds ticks
	static std::clock_t getMillis( std::clock_t clock )
	{
		if( CLOCKS_PER_SEC != 1000 )
		{
			clock = clock * 1000 / CLOCKS_PER_SEC;
		}

		return clock;
	}
};

/**
	@brief stop watch to meassure the execution time with a user clock
*/
struct UserTimeClock : private TimeConverter
{
	typedef std::clock_t	ClockTicks;

	/// return execution (user) time in ms
	static ClockTicks clock( void )
	{
#ifdef _Windows
		return getMillis( std::clock() );
#else
		static time_t	s_startTime = std::time( NULL );

		std::time_t	now = std::time( NULL );
		now -= s_startTime;

		struct timeval tv;
		struct timezone	tz;
		gettimeofday( &tv, &tz );

		clock_t	result = now * 1000 + tv.tv_usec / 1000;
		return result;
#endif
	}
};

/**
	@brief stop watch to meassure the execution time with a cpu clock

	@note Windows uses a user clock instead
*/
struct CpuTimeClock : private TimeConverter
{
	typedef std::clock_t	ClockTicks;

	/// return execution (cpu) time in ms
	static ClockTicks clock( void )
	{
		return getMillis( std::clock() );
	}
};

/**
	@brief StopWatch template used to meassure execution times
	@tparam ClockT stop watch use to meassure the execution time
	@see CpuTimeClock, UserTimeClock
*/
template<typename ClockT>
class BasicStopWatch
{
	std::clock_t	m_startTime, m_endTime, m_offset;
	bool			m_isRunning;

	public:
	/**
		@brief constructor to create a stop watch
		@param start bool immediately start the stop watch
	*/
	BasicStopWatch( bool start=false )
	{
		m_startTime = start ? ClockT::clock() : 0;
		m_offset = m_endTime = 0;
		m_isRunning = start;
	}

	/// starts or resets the stop watch
	void start()
	{
		m_startTime = ClockT::clock();
		m_isRunning = true;
	}
	/// stops the stop watch if its currenty running
	void stop()
	{
		if( m_isRunning )
		{
			m_endTime = ClockT::clock();
			m_isRunning = false;
		}
		m_startTime -= m_offset;
		m_offset = 0;
	}

	/// pause the stop watch if its currenty running
	void pause()
	{
		if( m_isRunning )
		{
			stop();
			m_offset = getMillis();
		}
	}

	/**
		@brief returns the execution time of the stop watch
		@tparam TimerT The type of the clock ticks
		@see MilliSeconds, Seconds, Minutes, Hours, Days, Weeks
	*/
	template <typename TimerT>
	TimerT get( void ) const
	{
		return TimerT( MilliSeconds<std::clock_t>( getMillis() ) );
	}
	/// returns the execution time in milliseconds
	std::clock_t getMillis() const
	{
		return m_offset + (m_isRunning ? ClockT::clock() : m_endTime) - m_startTime;
	}
};

/**
	@brief AutoStopWatch template used to meassure execution times
	@tparam ClockT stop watch use to meassure the execution time
	@see CpuTimeClock, UserTimeClock
*/
template <typename ClockT>
class BasicAutoStopWatch
{
	BasicStopWatch<ClockT> &m_sw;
public:

	BasicAutoStopWatch(BasicStopWatch<ClockT> &sw) : m_sw(sw) 
	{
		sw.start();
	}
	~BasicAutoStopWatch() 
	{
		m_sw.stop();
	}
};

/// StopWatch uses a user time clock to meassure the execution time. @see BasicStopWatch
typedef BasicStopWatch<UserTimeClock>	StopWatch;
/// CpuStopWatch uses a cpu time clock to meassure the execution time. @see BasicStopWatch
typedef BasicStopWatch<CpuTimeClock>	CpuStopWatch;

/// AutoStopWatch uses a user time clock to meassure the execution time. @see BasicAutoStopWatch
typedef BasicAutoStopWatch<UserTimeClock>	AutoStopWatch;
/// CpuAutoStopWatch uses a cpu time clock to meassure the execution time. @see BasicAutoStopWatch
typedef BasicAutoStopWatch<CpuTimeClock>	CpuAutoStopWatch;

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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_STOP_WATCH_H
