/*
		Project:		GAKLIB
		Module:			eta.h
		Description:	estimated time of arival calculator
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			http://www.gaeckler.at/

		Copyright:		(c) 1988-2023 Martin Gäckler

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
	struct ValueTimePairs
	{
		COUNTER_T	m_counter;
		ClockTicks	m_ticks;
		ValueTimePairs( const COUNTER_T &counter=0, const ClockTicks &ticks=0 ) : m_counter(counter), m_ticks(ticks) {}
	};
	RingBuffer<ValueTimePairs>	m_counters;

	public:
	/// @brief the constructor
	Eta() : m_counters(1024) {}

	/**
		@brief watch the counter value
		@param [in] value The current value of the counter
	*/
	void addValue(COUNTER_T value)
	{
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
		}
		ValueTimePairs	newValue(value,ClockProvider_T::clock());
		m_counters.push(newValue);
	}
	/// @return true if the time of arrival can be estimated 
	bool isValid() const
	{
		return m_counters.size() >= 2;
	}
	/// @return The number of estimated ticks until the counter reaches 0.
	ClockTicks getETA() const
	{
		if( isValid() )
		{
			ValueTimePairs	first = m_counters.oldest().get();
			ValueTimePairs	last = m_counters.newest().get();
			ClockTicks		elapsed = last.m_ticks - first.m_ticks;
			COUNTER_T		reached = first.m_counter - last.m_counter;
			if(elapsed&&reached)
			{
				ClockTicks	remain = ClockTicks((elapsed*last.m_counter)/reached);

				return remain;
			}
		}
		return std::numeric_limits<ClockTicks>::min();
	}
};

/// @brief the output operator for an ETA. If the ETA is not yet valid it prints nothing
template <class COUNTER_T, class ClockProvider_T>
std::ostream & operator << ( std::ostream &out, const Eta<COUNTER_T, ClockProvider_T> &eta )
{
	if( eta.isValid() )
	{
		typename ClockProvider_T::ClockTicks	ticks = eta.getETA();

#if 1
		out << std::setw(2) << std::setfill('0') << ticks / (24*60*60*CLOCKS_PER_SEC) << 'd';
		ticks %= (24*60*60*CLOCKS_PER_SEC);

		out << std::setw(2) << std::setfill('0') << ticks / (60*60*CLOCKS_PER_SEC) << 'h';
		ticks %= (60*60*CLOCKS_PER_SEC);

		out << std::setw(2) << std::setfill('0') << ticks / (60*CLOCKS_PER_SEC) << 'm';
		ticks %= (60*CLOCKS_PER_SEC);

		out << std::setw(2) << std::setfill('0') << ticks / (CLOCKS_PER_SEC);
#else
		out << ticks << 't';
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
