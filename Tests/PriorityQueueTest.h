/*
		Project:		GAKLIB
		Module:			PriorityQueueTest.h
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/priorityQueue.h>

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

class PriorityQueueTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "PriorityQueueTest";
	}

	virtual void PerformTest( void )
	{
		{
			TestScope scope("PriorityQueue");
			simpleTest<PriorityQueue<Priority> >();
		}
		{
			TestScope scope("PriorityQueue2");
			simpleTest<PriorityQueue2<Priority> >();
		}
#ifdef NDEBUG
		Hours<>	speed = speedTest<PriorityQueue<Priority> >();
		Hours<>	speed2 = speedTest<PriorityQueue2<Priority> >();

		std::cout << speed.toString() << " vs. " << speed2.toString() << std::endl;
#endif
	}

	template <typename PriorityQueueT>
	void simpleTest()
	{
		PriorityQueueT	myQueue;

		myQueue.push( Priority( 1.0, 0 ) );
		myQueue.push( Priority( 1.0, 1 ) );
		
		UT_ASSERT_EQUAL( myQueue.size(), size_t(2) );
		Priority	val = myQueue.pop();
		UT_ASSERT_EQUAL( val.m_value, 0 );
		val = myQueue.pop();
		UT_ASSERT_EQUAL( val.m_value, 1 );
		UT_ASSERT_EQUAL( myQueue.size(), size_t(0) );

		myQueue.push( Priority( 1.0, 0 ) );
		myQueue.push( Priority( 1.0, 1 ) );
		myQueue.push( Priority( 2.0, 2 ) );
		myQueue.push( Priority( 2.0, 3 ) );

		UT_ASSERT_EQUAL( myQueue.size(), size_t(4) );

		val = myQueue.pop();
		UT_ASSERT_EQUAL( val.m_value, 2 );
		val = myQueue.pop();
		UT_ASSERT_EQUAL( val.m_value, 3 );
		val = myQueue.pop();
		UT_ASSERT_EQUAL( val.m_value, 0 );
		val = myQueue.pop();
		UT_ASSERT_EQUAL( val.m_value, 1 );

		UT_ASSERT_EQUAL( myQueue.size(), size_t(0) );

		UT_ASSERT_EXCEPTION( myQueue.pop(), QueueEmptyError );
	}
	template <typename PriorityQueueT>
	Hours<> speedTest()
	{
		const int			loopCount = 10;
		const double		prioCount = 2000;

		StopWatch			sw( true );
		PriorityQueueT		myQueue;
		int					i=0;

		for( int l=0; l<loopCount; ++l )
		{
			for( double priority = 1.0; priority <= prioCount; ++priority )
			{
				myQueue.push( Priority( priority, ++i ) );
			}
			myQueue.pop();
			for( double priority = prioCount*2; priority >= prioCount; --priority )
			{
				myQueue.push( Priority( priority, ++i ) );
			}
			myQueue.pop();
			for( double priority1 = 1.0, priority2 = prioCount*2; priority1 <= priority2; ++priority1, --priority2 )
			{
				myQueue.push( Priority( priority1, ++i ) );
				myQueue.push( Priority( priority2, ++i ) );
			}
		}

		while( myQueue.size() )
		{
			myQueue.pop();
		}

		std::cout << i << " items generated\n";

		return sw.get< Hours<> >();
	}
	struct Priority
	{
		typedef double priority_type;

		double	m_priority;
		int		m_value;

		Priority( double priority, int value ) : m_priority(priority), m_value(value)
		{
		}

		double getPriority( ) const
		{
			return m_priority;
		}
	};
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static PriorityQueueTest	myPriorityQueueTest;

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

