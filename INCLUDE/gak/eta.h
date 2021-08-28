/*
		Project:		GAKLIB
		Module:			eta.h
		Description:	estimated time of arival calculator
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			http://www.gaeckler.at/

		Copyright:		(c) 1988-2020 Martin Gäckler

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

#ifndef GAKLIB_ETA_H
#define GAKLIB_ETA_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <ctime>
#include <gak/array.h>

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

struct StdClockProvider
{
	typedef std::clock_t	ClockTicks;

	static ClockTicks	ticks()
	{
		return std::clock();
	}
};

template <class COUNTER_T=std::size_t, class ClockProvider_T=StdClockProvider>
class Eta
{
public:
	typedef typename ClockProvider_T::ClockTicks ClockTicks;
private:
	struct ValueTimePairs
	{
		COUNTER_T	counter;
		ClockTicks	ticks;
	};
	Array<ValueTimePairs>	counters;

	public:
	void addValue(COUNTER_T value)
	{
		if( counters.size() )
		{
			COUNTER_T lastValue = counters[counters.size()-1].counter;
			if( lastValue < value)
			{
				counters.clear();
			}
			else if( lastValue == value )
			{
				return;
			}
		}
		ValueTimePairs	&newValue = counters[counters.size()?1:0];
		newValue.counter = value;
		newValue.ticks = ClockProvider_T::ticks();
	}
	bool isValid() const
	{
		return counters.size() >= 2;
	}
	ClockTicks getETA() const
	{
		if( isValid() )
		{
			const ValueTimePairs	&first = counters[0];
			const ValueTimePairs	&last = counters[counters.size()-1];
			ClockTicks				elapsed = last.ticks - first.ticks;
			COUNTER_T				reached = first.counter - last.counter;
			if(elapsed&&reached)
			{
				ClockTicks	remain = ClockTicks((elapsed*last.counter)/reached);

				return remain;
			}
		}
		return std::numeric_limits<ClockTicks>::min();
	}
};

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
