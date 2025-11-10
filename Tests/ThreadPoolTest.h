/*
		Project:		GAKLIB
		Module:			ThreadPoolTest.h
		Description:	
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/threadPool.h>

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

class Functor
{
	int		m_id;
	size_t	*m_callCount;

	public:
	Functor( int id=-1, size_t	*callCount=nullptr ) : m_id(id), m_callCount(callCount)
	{}

	void operator () () const
	{
		static Locker	locker;
		::Sleep( 1000 );

		LockGuard	guard( locker );
		if( guard )
		{
			assert( m_callCount );
			std::cout << "Here is " << ++(*m_callCount) << '/' << m_id << std::endl;
		}
	}
};

struct MainData
{
	Critical	critical;

	int sum;
	int waiting;

	MainData() : sum(0), waiting(0) {}
};

template <>
struct ProcessorType<int>
{
	typedef int object_type;

	void process( int i, void *threadPool, void *imainData )
	{
		MainData * mainData = (MainData *)imainData;
		CriticalScope scope(mainData->critical);

		mainData->sum += i;
		mainData->waiting += ((ThreadPool<int> *)threadPool)->size();
	}
};

class ThreadPoolTest : public UnitTest
{
	size_t	callCount;

	virtual const char *GetClassName() const
	{
		return "ThreadPoolTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "ThreadPoolTest::PerformTest");
		TestScope scope( "PerformTest" );

		callCount = 0;
		const size_t	count(20);
		{
			ThreadPool<Functor>	pool( 5, "ThreadPoolTest1", nullptr );

			UT_ASSERT_EQUAL( pool.getCurrentState(), psIdle );
			pool.start();
			UT_ASSERT_EQUAL( pool.getCurrentState(), psWaiting );
			for( size_t i=0; i<count; ++i )
			{
				pool.process( Functor( int(i), &callCount ) );
			}
			UT_ASSERT_EQUAL( pool.getCurrentState(), psProssesing );
			pool.flush();
			UT_ASSERT_EQUAL( pool.getCurrentState(), psWaiting );
			UT_ASSERT_EQUAL( count, pool.total() );
		}
		UT_ASSERT_EQUAL( count, callCount );
		{
			const size_t	count(10);
			ThreadPool<Functor>	pool( 5, "ThreadPoolTest2", nullptr );

			callCount = 0;
			pool.start();
			Sleep( 2000 );
			for( size_t i=0; i<count; ++i )
			{
				Sleep( 2000 );
				pool.process( Functor( int(i), &callCount ) );
			}
			Sleep( 2000 );
			pool.flush();
			UT_ASSERT_EQUAL( count, callCount );
			pool.shutdown();

			callCount = 0;
			pool.start();
			for( size_t i=0; i<count; ++i )
			{
				pool.process( Functor( int(i), &callCount ) );
			}
			pool.flush();
			pool.shutdown();
			UT_ASSERT_EQUAL( count, callCount );

			UT_ASSERT_EQUAL( pool.getCurrentState(), psIdle );

			UT_ASSERT_EXCEPTION(
				pool.process( Functor( int(0), &callCount ) ),
				ThreadPoolError
			);
		}
		{
			MainData	mainData;
			ThreadPool<int>	pool(3, "MyIntPool", &mainData );

			pool.start();
			UT_ASSERT_EQUAL( mainData.sum, 0 );
			pool.process( 5 );
			pool.flush();
			pool.shutdown();
			UT_ASSERT_EQUAL( mainData.sum, 5 );
			pool.start();
			pool.process( 5 );
			pool.process( 10 );
			pool.flush();
			pool.shutdown();
			UT_ASSERT_EQUAL( mainData.sum, 20 );

			UT_ASSERT_GREATER( mainData.waiting, 0 );
		}
		{
			MainData	mainData;
			ThreadPool<int>	pool(0, "MyIntPool", &mainData );

			pool.start();
			UT_ASSERT_EQUAL( mainData.sum, 0 );
			pool.process( 5 );
			pool.flush();
			pool.shutdown();
			UT_ASSERT_EQUAL( mainData.sum, 5 );
			pool.start();
			pool.process( 5 );
			pool.process( 10 );
			pool.flush();
			pool.shutdown();
			UT_ASSERT_EQUAL( mainData.sum, 20 );

			UT_ASSERT_GREATER( mainData.waiting, 0 );
		}
	}
	virtual bool canThreadTest()
	{
		return true;
	}
	virtual UnitTest *duplicate()
	{
		return new ThreadPoolTest( false );
	}
	public:
	ThreadPoolTest( bool isStatic=true ) : UnitTest( isStatic ) {}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static ThreadPoolTest myThreadPoolTest;

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

