/*
		Project:		GAKLIB
		Module:			eta.h
		Description:	estimated time of arival calculator
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef GAKLIB_ETA_H
#define GAKLIB_ETA_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iomanip>
#include <ctime>
#include <gak/ringBuffer.h>
#include <gak/stopWatch.h>

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

static const size_t MINDATA_COUNT = 16;
static const size_t MAXDATA_COUNT = 512;
static const size_t MINDATA_TIME = 16*1000;		// 16 seconds

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
	@brief estimate time of arrival

	Estimates the time til a process finishes. This is calculated by watching a counter til its value becomes 0. 

	@tparam COUNTER_T The type of the counter, default is std::size_t
	@tparam ClockProvider_T The type of a clock provider, default is UserTimeClock

	@see UserTimeClock
*/
template <class COUNTER_T=std::size_t, class ClockProvider_T=UserTimeClock>
class Eta
{
public:
	/// @brief the type of clock ticks
	typedef typename ClockProvider_T::ClockTicks ClockTicks;
private:
	struct ValueTimePair
	{
		COUNTER_T	m_counter;
		ClockTicks	m_ticks;
		ValueTimePair( const COUNTER_T &counter=0, const ClockTicks &ticks=0 ) : m_counter(counter), m_ticks(ticks) {}
		ClockTicks since() const
		{
			return ClockProvider_T::clock() - m_ticks;
		}
	};
	RingBuffer<ValueTimePair>	m_counters;
	ClockTicks					m_lastRemain;
	ValueTimePair				m_last;

	COUNTER_T					m_maxCount;
	COUNTER_T					m_useCount;
	char						m_markChar;

	public:
	/// @brief the constructor
	Eta() : m_counters(MAXDATA_COUNT), m_maxCount(0), m_useCount(0), m_markChar(' ')
	{
		m_lastRemain = 0;
	}

	/**
		@brief watch the counter value
		@param [in] value The current value of the counter
	*/
	void addValue(COUNTER_T value)
	{
		m_markChar = ' ';
		if( value > m_maxCount )
		{
			m_maxCount = value;
			m_useCount = (value >> 3);
		}
		if( m_counters.size() )
		{
			COUNTER_T lastValue = m_counters.newest().get().m_counter;
			if( lastValue < value)
			{
				m_counters.clear();
			}
			else if( lastValue == value )
			{
				return;
			}
			if( m_counters.isFull() )
			{
				return;
				//m_counters.pop();
			}
		}
		ValueTimePair	newValue(value,ClockProvider_T::clock());
		m_counters.push(newValue);
	}
	/// @return true if the time of arrival can be estimated 
	bool isValid() const
	{
		doEnterFunctionEx( gakLogging::llDetail, "Eta<>::isValid" );
		doLogValueEx( gakLogging::llDetail, m_counters.size() );
		return m_counters.size() >= 2;
	}

	private:
	ClockTicks getETAfromLast() const
	{
		doEnterFunctionEx( gakLogging::llDetail, "Eta<>::getETAfromLast" );
		ClockTicks sinceLast = m_last.since();
		ClockTicks remain = (m_lastRemain > sinceLast) ? (m_lastRemain - sinceLast) : 0;
		doLogValueEx( gakLogging::llDetail, m_lastRemain );
		doLogValueEx( gakLogging::llDetail, sinceLast );
		doLogValueEx( gakLogging::llDetail, remain );
		return remain;
	}

	ValueTimePair getFirst()
	{
		doEnterFunctionEx( gakLogging::llDetail, "Eta<>::getFirst" );
		ValueTimePair	first = m_counters.oldest().get();

		if( m_useCount )
		{

			COUNTER_T		processed = first.m_counter - m_last.m_counter;
			ClockTicks		elapsedTime = m_last.m_ticks - first.m_ticks;
			size_t			inSize = m_counters.size();

			while( processed > m_useCount && m_counters.size() > MINDATA_COUNT && elapsedTime > MINDATA_TIME )
			{
				m_counters.pop();
				first = m_counters.oldest().get();
				processed = first.m_counter - m_last.m_counter;
				elapsedTime = m_last.m_ticks - first.m_ticks;
			}
			if( inSize != m_counters.size() )
			{
				doLogValueEx( gakLogging::llDetail, m_counters.size() );
				doLogValueEx( gakLogging::llDetail, (inSize - m_counters.size()) );
			}
		}
		return first;
	}

	ClockTicks adjustRemainFromPast( ClockTicks remain, ClockTicks expectedMinDiff, ClockTicks expectedMaxDiff )
	{
		doEnterFunctionEx( gakLogging::llDetail, "Eta<>::adjustRemainFromPast" );

		bool updateUsedCount = true;
		char newMarker = ' ';

		doLogValueEx( gakLogging::llDetail, m_lastRemain );
		doLogValueEx( gakLogging::llDetail, expectedMinDiff );
		doLogValueEx( gakLogging::llDetail, expectedMaxDiff );
		doLogValueEx( gakLogging::llDetail, m_useCount );

		if( m_lastRemain > 0 && (expectedMinDiff || expectedMaxDiff) )
		{
			if( m_lastRemain < remain )		// expectation is worse than before
			{
				if( m_useCount < m_maxCount )
				{
					m_useCount <<=  1;		// we need more data
				}
				updateUsedCount = false;
				newMarker = '-';
			}
			else
			{
				ClockTicks actDiff = gak::math::abs(m_lastRemain-remain);
				if( actDiff < expectedMinDiff || actDiff > expectedMaxDiff)		// outside the accepted time range
				{
					if( m_useCount > 8 )
					{
						m_useCount >>= 1;		// we need less data to handle performance changes
					}
					updateUsedCount = false;
					doLogMessageEx( gakLogging::llDetail,  "bad diff data" );
					newMarker = '?';
				}
			}
		}
		if( updateUsedCount && m_useCount < m_maxCount )
		{
			m_useCount +=  (m_maxCount >> 3);		// use more data to become more stable
		}

		m_markChar = newMarker;
		m_lastRemain = remain;
		doLogValueEx( gakLogging::llDetail, m_useCount );
		doLogValueEx( gakLogging::llDetail, m_lastRemain );
		return remain - m_last.since();
	}

	public:
	/// @return The number of estimated ticks until the counter reaches 0.
	ClockTicks getETA(ClockTicks expectedMinDiff=0, ClockTicks expectedMaxDiff=0)
	{
		doEnterFunctionEx( gakLogging::llDetail, "Eta<>::getETA" );
		if( isValid() )
		{
			ValueTimePair	last = m_counters.newest().get();
			if( last.m_counter == m_last.m_counter && last.m_ticks == m_last.m_ticks )
			{
				return getETAfromLast();
			}
			m_last = last;
			ValueTimePair	first = getFirst();

			COUNTER_T		processed = first.m_counter - last.m_counter;
			ClockTicks		elapsedTime = last.m_ticks - first.m_ticks;

			doLogValueEx( gakLogging::llDetail, processed );
			doLogValueEx( gakLogging::llDetail, elapsedTime );
			if(elapsedTime&&processed)
			{
				ClockTicks	remain = ClockTicks((elapsedTime*m_last.m_counter)/processed);
				return adjustRemainFromPast( remain, expectedMinDiff, expectedMaxDiff );
			}
		}
		doLogMessageEx( gakLogging::llDetail, "few diff data" );
		m_markChar = '*';
		return m_lastRemain;
	}

	char markChar() const
	{
		return m_markChar;
	}
};

/// @brief the output operator for an ETA. If the ETA is not yet valid it prints nothing
template <class COUNTER_T, class ClockProvider_T>
std::ostream & operator << ( std::ostream &out, Eta<COUNTER_T, ClockProvider_T> &eta )
{
	doEnterFunctionEx(gakLogging::llDetail, "operator << ( std::ostream &out, Eta<COUNTER_T, ClockProvider_T> &eta )"  );
	if( eta.isValid() )
	{
		typename ClockProvider_T::ClockTicks	ticks = eta.getETA();

#if 1
		doLogValueEx( gakLogging::llDetail, ticks );
		if( ticks > 0 )
		{
			out << std::setw(2) << std::setfill('0') << ticks / (24*60*60*CLOCKS_PER_SEC) << 'd';
			ticks %= (24*60*60*CLOCKS_PER_SEC);

			out << std::setw(2) << std::setfill('0') << ticks / (60*60*CLOCKS_PER_SEC) << 'h';
			ticks %= (60*60*CLOCKS_PER_SEC);

			out << std::setw(2) << std::setfill('0') << ticks / (60*CLOCKS_PER_SEC) << 'm';
			ticks %= (60*CLOCKS_PER_SEC);

			out << std::setw(2) << std::setfill('0') << ticks / (CLOCKS_PER_SEC);

			out << eta.markChar();
		}
		else
			out << ticks << 't' << eta.markChar();
#else
#endif
	}
	return out;
}

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

#endif
