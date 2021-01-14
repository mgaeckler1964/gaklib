/*
		Project:		GAKLIB
		Module:			stopWatch.h
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

#ifndef GAK_STOP_WATCH_H
#define GAK_STOP_WATCH_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <ctime>

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
	explicit MilliSeconds( BaseT milliSeconds=BaseT() ) : m_milliSeconds(milliSeconds)
	{
	}
	const MilliSeconds &operator = ( BaseT milliSeconds )
	{
		m_milliSeconds = milliSeconds;
		return *this;
	}

	MilliSeconds( const MilliSeconds<BaseT> &source ) : m_milliSeconds(source.m_milliSeconds)
	{
	}
	const MilliSeconds &operator = ( const MilliSeconds<BaseT> &source )
	{
		m_milliSeconds = source.m_milliSeconds;
		return *this;
	}

	BaseT get( void ) const
	{
		return m_milliSeconds;
	}
	BaseT asMillis() const
	{
		return m_milliSeconds;
	}
	BaseT asSeconds() const
	{
		return get() / MILLIS_PER_SECOND;
	}
	BaseT asMinutes() const
	{
		return get() / MILLIS_PER_MINUTE;
	}
	BaseT asHours() const
	{
		return get() / MILLIS_PER_HOUR;
	}
	BaseT asDays() const
	{
		return get() / MILLIS_PER_DAY;
	}
	BaseT asWeeks() const
	{
		return get() / MILLIS_PER_WEEK;
	}

	BaseT millisPart( void ) const
	{
		return get() % MILLIS_PER_SECOND;
	}
	BaseT secondsPart( void ) const
	{
		return asSeconds() % SECONDS_PER_MINUTE;
	}
	BaseT minutesPart( void ) const
	{
		return asMinutes() % MINUTES_PER_HOUR;
	}
	BaseT hoursPart( void ) const
	{
		return asHours() % HOURS_PER_DAY;
	}
	BaseT daysPart( void ) const
	{
		return asDays() % DAYS_PER_WEEK;
	}
	BaseT weeksPart( void ) const
	{
		return asWeeks();
	}

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

	MilliSeconds<BaseT> operator + ( const MilliSeconds<BaseT> &oper ) const
	{
		return MilliSeconds<BaseT>( get() + oper.get() );
	}
};

template <typename BaseT=std::clock_t>
class Seconds : public MilliSeconds<BaseT>
{
	public:
	Seconds( const MilliSeconds<BaseT> &src ) : MilliSeconds<BaseT>(src)
	{
	}
	explicit Seconds( BaseT seconds=BaseT() ) : MilliSeconds<BaseT>(seconds*this->MILLIS_PER_SECOND)
	{
	}
	const Seconds &operator = ( BaseT milliSeconds )
	{
		MilliSeconds<BaseT>::operator = ( milliSeconds*this->MILLIS_PER_SECOND );
		return *this;
	}
	BaseT get( void ) const
	{
		return this->asSeconds();
	}
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

template <typename BaseT=std::clock_t>
class Minutes : public Seconds<BaseT>
{
	public:
	Minutes( const MilliSeconds<BaseT> &src ) : Seconds<BaseT>(src)
	{
	}
	explicit Minutes( BaseT minutes=BaseT() ) : Seconds<BaseT>(minutes*this->SECONDS_PER_MINUTE)
	{
	}
	const Minutes &operator = ( BaseT minutes )
	{
		Seconds<BaseT>::operator = ( minutes*this->SECONDS_PER_MINUTE );
		return *this;
	}
	BaseT get( void ) const
	{
		return this->asMinutes();
	}
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

template <typename BaseT=std::clock_t>
class Hours : public Minutes<BaseT>
{
	public:
	Hours( const MilliSeconds<BaseT> &src ) : Minutes<BaseT>(src)
	{
	}
	explicit Hours( BaseT hours=BaseT() ) : Minutes<BaseT>(hours*this->MINUTES_PER_HOUR)
	{
	}
	const Hours &operator = ( BaseT hours )
	{
		Minutes<BaseT>::operator = ( hours*this->MINUTES_PER_HOUR );
		return *this;
	}
	BaseT get( void ) const
	{
		return this->asHours();
	}
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

template <typename BaseT=std::clock_t>
class Days : public Hours<BaseT>
{
	public:
	Days( const MilliSeconds<BaseT> &src ) : Hours<BaseT>(src)
	{
	}
	explicit Days( BaseT days=BaseT() ) : Hours<BaseT>(days*this->HOURS_PER_DAY)
	{
	}
	const Days &operator = ( BaseT days )
	{
		Hours<BaseT>::operator = ( days*this->HOURS_PER_DAY );
		return *this;
	}
	BaseT get( void ) const
	{
		return this->asDays();
	}
	STRING toString( bool part=false ) const
	{
		return formatNumber( part ? this->daysPart() : get() ) + "d " + Hours<BaseT>::toString( true );
	}
};

template <typename BaseT=std::clock_t>
class Weeks : public Days<BaseT>
{
	public:
	Weeks( const MilliSeconds<BaseT> &src ) : Days<BaseT>(src)
	{
	}
	explicit Weeks( BaseT weeks=BaseT() ) : Days<BaseT>(weeks*this->DAYS_PER_WEEK)
	{
	}
	const Weeks &operator = ( BaseT weeks )
	{
		Days<BaseT>::operator = ( weeks*this->DAYS_PER_WEEK );
		return *this;
	}
	BaseT get( void ) const
	{
		return this->asWeeks();
	}
	STRING toString() const
	{
		return formatNumber( get() ) + "w " + Days<BaseT>::toString( true );
	}
};

struct TimeConverter
{
	static std::clock_t getMillis( std::clock_t clock )
	{
#if CLOCKS_PER_SEC != 1000
		{
			clock = clock * 1000 / CLOCKS_PER_SEC;
		}
#endif
		return clock;
	}
};

struct UserTimeClock : public TimeConverter
{
	/// return execution (user) time in ms
	static std::clock_t clock( void )
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

struct CpuTimeClock : public TimeConverter
{
	/// return execution (user) time in ms
	static std::clock_t clock( void )
	{
		return getMillis( std::clock() );
	}
};

template<typename ClockT>
class BasicStopWatch
{
	std::clock_t	m_startTime, m_endTime;
	bool			m_isRunning;

	public:
	BasicStopWatch( bool start=false )
	{
		m_startTime = start ? ClockT::clock() : 0;
		m_endTime = 0;
		m_isRunning = start;
	}

	void start()
	{
		m_startTime = ClockT::clock();
		m_isRunning = true;
	}
	void stop()
	{
		if( m_isRunning )
		{
			m_endTime = ClockT::clock();
			m_isRunning = false;
		}
	}
	template <typename TimerT>
	TimerT get( void ) const
	{
		return TimerT( MilliSeconds<std::clock_t>( getMillis() ) );
	}
	std::clock_t getMillis() const
	{
		return (m_isRunning ? ClockT::clock() : m_endTime) - m_startTime;
	}
};

typedef BasicStopWatch<UserTimeClock>	StopWatch;
typedef BasicStopWatch<CpuTimeClock>	CpuStopWatch;

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
