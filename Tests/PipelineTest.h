/*
		Project:		GAKLIB
		Module:			PipelineTest.h
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

#include <gak/pipeline.h>

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

const char pipeTestText[] = "the quick brown fox jumps over the lazy dog";

class PipelineTest : public UnitTest
{
	class Producer : public ProducerThread
	{
		const char *GetClassName() const
		{
			return "PipelineTest::Producer";
		}
		public:
		Producer( oRingStream &cout ) : ProducerThread( cout )
		{
		}
		virtual void ExecuteThread( oRingStream &cout )
		{
			doEnterFunctionEx(gakLogging::llInfo, "Producer::ExecuteThread");
			Sleep( 5000 );
			for( const char *cp = pipeTestText; *cp; ++cp )
			{
				std::cout << getThreadID() << " Producer: " << *cp << std::endl;
				cout << *cp;
				Sleep( 1000 );
			}
		}
	};
	class Filter : public FilterThread
	{
		const char *GetClassName() const
		{
			return "PipelineTest::Filter";
		}
		public:
		Filter( iRingStream &cin, oRingStream &cout ) : FilterThread( cin, cout )
		{
		}
		virtual void ExecuteThread( iRingStream &cin, oRingStream &cout )
		{
			doEnterFunctionEx(gakLogging::llInfo, "Filter::ExecuteThread");
			cin.unsetf( std::ios_base::skipws );
			size_t	numData = 0;
			const char *cp = pipeTestText;
			while( true )
			{
				char c = 0;
				cin >> c;
				if( !cin || !c )
					break;
				UT_ASSERT_EQUAL( c, *cp );
				std::cout << getThreadID() << " Filter: " << char(ansiToUpper( c )) << std::endl;
				cout << char( ansiToUpper( c ) );
				if( c != *cp )
					break;
				cp++;
				numData++;
				Sleep( 1000 );
			}
			UT_ASSERT_EQUAL( strlen( pipeTestText ), numData );
		}
	};

	class Consumer : public ConsumerThread
	{
		const char *GetClassName() const
		{
			return "PipelineTest::Consumer";
		}
		public:
		Consumer( iRingStream &cin ) : ConsumerThread( cin )
		{
		}
		virtual void ExecuteThread( iRingStream &cin )
		{
			doEnterFunctionEx(gakLogging::llInfo, "Consumer::ExecuteThread");
			cin.unsetf( std::ios_base::skipws );
			size_t	numData = 0;
			const char *cp = pipeTestText;
			while( true )
			{
				char c = 0;
				cin >> c;
				if( !cin || !c )
					break;
				UT_ASSERT_EQUAL( c, char(ansiToUpper( *cp )) );
				std::cout << getThreadID() << " Consumer: " << char(ansiToUpper( c )) << std::endl;
				if( c != ansiToUpper( *cp ) )
					break;
				cp++;
				numData++;
				Sleep( 1000 );
			}
			UT_ASSERT_EQUAL( strlen( pipeTestText ), numData );
		}
	};

	virtual const char *GetClassName() const
	{
		return "PipelineTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "PipelineTest::PerformTest");
		TestScope scope( "PerformTest" );

		Pipe							buffer1(5);
		Pipe							buffer2(5);
		SharedObjectPointer<Producer>	producer = new Producer( buffer1.getOutStream() );
		SharedObjectPointer<Filter>		filter = new Filter( buffer1.getInStream(), buffer2.getOutStream() );
		SharedObjectPointer<Consumer>	consumer = new Consumer( buffer2.getInStream() );

		consumer->StartThread();
		filter->StartThread();
		producer->StartThread();

		consumer->join();
		filter->join();
		producer->join();
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static PipelineTest myPipelineTest;

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

