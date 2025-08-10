/*
		Project:		GAKLIB
		Module:			StreamsTest.h
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

#include <gak/streams.h>

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

struct RandomChar
{
	static const size_t	NUM_ITEMS = 10;

	int		numCreated;

	RandomChar() : numCreated(0)
	{
	}
	bool eof() const
	{
		return numCreated >= NUM_ITEMS;
	}
	char getNext()
	{
		++numCreated;
		return 'a' + randomNumber(26);
	}
};

const size_t	RandomChar::NUM_ITEMS;

static std::size_t numOddCalls = 0;
static std::size_t numOddSucces = 0;
static std::size_t numEvenCalls = 0;
static std::size_t numEvenSucces = 0;

static void resetCounters()
{
	numOddCalls = 0;
	numOddSucces = 0;
	numEvenCalls = 0;
	numEvenSucces = 0;
};

class StreamsTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "StreamsTest";
	}

	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "StreamsTest::PerformTest");
		TestScope scope( "PerformTest" );

		FileTest();

		ContainerTest();
		UnionTest();
		MapTest();
		FilterTest();
		FunctionSourceTest();
		FlatmapSourceTest();
		AllTests();
	}
	void FileTest()
	{
		static const STRING FILE_NAME = "fileStream.sink";
		{
			doLogValue( "Creating ContainerStream1" );
			streams::Stream<STRING>	strStream1 = streams::makeContainerStream( myStrings1 );
			strStream1->setName("strStream1");

			streams::Sink<
				STRING, 
				streams::FileSinkFunction< 
					STRING 
				> 
			>	strSink2 = strStream1->addFileSink(
				 FILE_NAME
			);
			strSink2->setName("strSink2");

			strSink2->start();
			strSink2->wait();
		}
		{
			doLogValue( "Creating FileStream1" );
			streams::Stream<STRING> strStream1 = streams::makeFileStream<STRING>( FILE_NAME );
			strStream1->setName("strStream1");

			Array<STRING>	resultArray;
			streams::Sink<
				STRING, 
				streams::ContainerSinkFunction< 
					std::back_insert_iterator< 
						Array<STRING> 
					>, 
					STRING 
				> 
			>	strSink2 = strStream1->addContainerSink(
				std::back_inserter( resultArray ) 
			);
			strSink2->setName("strSink2");

			strSink2->start();
			strSink2->wait();

			UT_ASSERT_EQUAL( resultArray.size(), array1Size );
			UT_ASSERT_EQUAL( resultArray, myStrings1 );
		}
		strRemoveE( FILE_NAME );
	}
	void ContainerTest()
	{
		doLogValue( "Creating ContainerStream1" );
		streams::Stream<STRING>	strStream1 = streams::makeContainerStream( myStrings1 );
		strStream1->setName("strStream1");

		doLogValue( "Creating strSink2" );
		Array<STRING>	resultArray;
		streams::Sink<
			STRING, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<STRING> 
				>, 
				STRING 
			> 
		>	strSink2 = strStream1->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink2->start();
		strSink2->wait();

		UT_ASSERT_EQUAL( strStream1->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( resultArray.size(), array1Size );
	}
	void UnionTest()
	{
		doLogValue( "Creating ContainerStream1" );
		streams::Stream<STRING>	strStream1 = streams::makeContainerStream( myStrings1 );
		strStream1->setName("strStream1");

		doLogValue( "Creating ContainerStream2" );
		streams::Stream<STRING>	strStream2 = streams::makeContainerStream( myStrings2 );
		strStream2->setName("strStream2");

		doLogValue( "Creating unionStream1" );
		streams::Stream<STRING>	unionStream1 = strStream1->unionStream( strStream2 );
		unionStream1->setName("unionStream1");

		doLogValue( "Creating strSink2" );
		Array<STRING>	resultArray;
		streams::Sink<
			STRING, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<STRING> 
				>, 
				STRING 
			> 
		>	strSink2 = unionStream1->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink2->start();
		strSink2->wait();

		UT_ASSERT_EQUAL( strStream1->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( strStream2->getNumItems(), array2Size );
		UT_ASSERT_EQUAL( strStream2->getNumItemsProcessed(), array2Size );
		UT_ASSERT_EQUAL( strStream2->getNumItemsDispatched(), array2Size );
		UT_ASSERT_EQUAL( strStream2->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( unionStream1->getNumItems(), arrayXSize );
		UT_ASSERT_EQUAL( unionStream1->getNumItemsProcessed(), arrayXSize );
		UT_ASSERT_EQUAL( unionStream1->getNumItemsDispatched(), arrayXSize );
		UT_ASSERT_EQUAL( unionStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( resultArray.size(), arrayXSize );
	}
	void MapTest()
	{
		doLogValue( "Creating ContainerStream1" );
		streams::Stream<STRING>	strStream1 = streams::makeContainerStream( myStrings1 );
		strStream1->setName("strStream1");

		doLogValue( "Creating firstLetterStream" );
		streams::Stream<char>	firstLetterStream = strStream1->map<char>( static_cast<char (*)(const STRING &)>(firstLetter) );
		firstLetterStream->setName("firstLetterStream");


		doLogValue( "Creating strSink2" );
		Array<char>	resultArray;
		streams::Sink<
			char, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<char> 
				>, 
				char 
			> 
		>	strSink2 = firstLetterStream->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink2->start();
		strSink2->wait();

		UT_ASSERT_EQUAL( strStream1->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( firstLetterStream->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( resultArray.size(), array1Size );
	}
	void FilterTest()
	{
		doLogValue( "Creating ContainerStream1" );
		streams::Stream<STRING>	strStream1 = streams::makeContainerStream( myStrings1 );
		strStream1->setName("strStream1");

		doLogValue( "Creating firstLetterStream" );
		streams::Stream<char>	firstLetterStream = strStream1->map<char>( static_cast<char (*)(const STRING &)>(firstLetter) );
		firstLetterStream->setName("firstLetterStream");

		doLogValue( "Creating evenStream" );
		streams::Stream<char>	evenStream = firstLetterStream->filter( static_cast<bool (*)( char )>(filterEven) );
		evenStream->setName("evenStream");

		doLogValue( "Creating strSink2" );
		Array<char>	resultArray;
		streams::Sink<
			char, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<char> 
				>, 
				char
			> 
		>	strSink2 = evenStream->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink2->start();
		strSink2->wait();

		UT_ASSERT_EQUAL( strStream1->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( firstLetterStream->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsWaiting(), size_t(0) );

		size_t evenSize = evenStream->getNumItems();

		UT_ASSERT_EQUAL( evenSize, numEvenSucces );

		UT_ASSERT_EQUAL( array1Size, numEvenCalls );

		UT_ASSERT_EQUAL( evenStream->getNumItemsProcessed(), evenSize );
		UT_ASSERT_EQUAL( evenStream->getNumItemsDispatched(), evenSize );
		UT_ASSERT_EQUAL( evenStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( resultArray.size(), evenSize );

		resetCounters();
	}
	void FunctionSourceTest()
	{
		doLogValue( "Creating randomStream" );
		streams::Stream<char>	randomStream = streams::makeFunctionStream<char>( RandomChar() );
		randomStream->setName("randomStream");

		doLogValue( "Creating strSink2" );
		Array<char>	resultArray;
		streams::Sink<
			char, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<char> 
				>, 
				char
			> 
		>	strSink2 = randomStream->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink2->start();
		strSink2->wait();

		UT_ASSERT_EQUAL( randomStream->getNumItems(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsProcessed(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsDispatched(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( resultArray.size(), RandomChar::NUM_ITEMS );
	}
	void FlatmapSourceTest()
	{
		doLogValue( "Creating randomStream" );
		streams::Stream<char>	randomStream = streams::makeFunctionStream<char>( RandomChar() );
		randomStream->setName("randomStream");

		doLogValue( "Creating randomFlatMapped3Stream" );
		streams::Stream<STRING>	randomFlatMapped3Stream = randomStream->flatMap<STRING>( static_cast<void (*)( char, streams::StreamBuffer<STRING> &)>(flatmap3) );
		randomFlatMapped3Stream->setName("randomFlatMapped3Stream");

		doLogValue( "Creating strSink2" );
		Array<STRING>	resultArray;
		streams::Sink<
			STRING, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<STRING> 
				>, 
				STRING
			> 
		>	strSink2 = randomFlatMapped3Stream->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink2->start();
		strSink2->wait();

		UT_ASSERT_EQUAL( randomStream->getNumItems(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsProcessed(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsDispatched(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( randomFlatMapped3Stream->getNumItems(), RandomChar::NUM_ITEMS*3 );
		UT_ASSERT_EQUAL( randomFlatMapped3Stream->getNumItemsProcessed(), RandomChar::NUM_ITEMS*3 );
		UT_ASSERT_EQUAL( randomFlatMapped3Stream->getNumItemsDispatched(), RandomChar::NUM_ITEMS*3 );
		UT_ASSERT_EQUAL( randomFlatMapped3Stream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( resultArray.size(), RandomChar::NUM_ITEMS*3 );
	}
	void AllTests()
	{
		doLogValue( "Creating ContainerStream1" );
		streams::Stream<STRING>	strStream1 = streams::makeContainerStream( myStrings1 );
		strStream1->setName("strStream1");

		doLogValue( "Creating ContainerStream2" );
		streams::Stream<STRING>	strStream2 = streams::makeContainerStream( myStrings2 );
		strStream2->setName("strStream2");

		doLogValue( "Creating unionStream1" );
		streams::Stream<STRING>	unionStream1 = strStream1->unionStream( strStream2 );
		unionStream1->setName("unionStream1");

		doLogValue( "Creating firstLetterStream" );
		streams::Stream<char>	firstLetterStream = unionStream1->map<char>( static_cast<char (*)(const STRING &)>(firstLetter) );
		firstLetterStream->setName("firstLetterStream");

		doLogValue( "Creating evenStream" );
		streams::Stream<char>	evenStream = firstLetterStream->filter( static_cast<bool (*)( char )>(filterEven) );
		evenStream->setName("evenStream");

		doLogValue( "Creating oddStream" );
		streams::Stream<char>	oddStream = firstLetterStream->filter( static_cast<bool (*)( char )>(filterOdd) );
		oddStream->setName("oddStream");

		doLogValue( "Creating evenFlatMapped3Stream" );
		streams::Stream<STRING>	evenFlatMapped3Stream = evenStream->flatMap<STRING>( static_cast<void (*)( char, streams::StreamBuffer<STRING> &)>(flatmap3) );
		evenFlatMapped3Stream->setName("evenFlatMapped3Stream");

		doLogValue( "Creating oddFlatMapped1Stream" );
		streams::Stream<STRING>	oddFlatMapped1Stream = oddStream->flatMap<STRING>( static_cast<void (*)( char, streams::StreamBuffer<STRING> &)>(flatmap1) );
		oddFlatMapped1Stream->setName("oddFlatMapped1Stream");

		doLogValue( "Creating unionStream2" );
		streams::Stream<STRING>	unionStream2 = evenFlatMapped3Stream->unionStream( oddFlatMapped1Stream ) ;
		unionStream2->setName("unionStream2");

		doLogValue( "Creating randomStream" );
		streams::Stream<char>	randomStream = streams::makeFunctionStream<char>( RandomChar() );
		randomStream->setName("randomStream");

		doLogValue( "Creating randomFlatMapped1Stream" );
		streams::Stream<STRING>	randomFlatMapped1Stream = randomStream->flatMap<STRING>( static_cast<void (*)( char, streams::StreamBuffer<STRING> &)>(flatmap1) );
		randomFlatMapped1Stream->setName("randomFlatMapped1Stream");

		doLogValue( "Creating unionStream3" );
		streams::Stream<STRING>	unionStream3 = unionStream2->unionStream( randomFlatMapped1Stream ) ;
		unionStream3->setName("unionStream3");

		doLogValue( "Creating strSink1" );
		streams::Stream<STRING>::SimpleSink	strSink1 = unionStream3->addConsoleSink();
		strSink1->setName("strSink1");

		doLogValue( "Creating strSink2" );
		Array<STRING>	resultArray;
		streams::Sink<
			STRING, 
			streams::ContainerSinkFunction< 
				std::back_insert_iterator< 
					Array<STRING> 
				>, 
				STRING 
			> 
		>	strSink2 = unionStream3->addContainerSink(
			std::back_inserter( resultArray ) 
		);
		strSink2->setName("strSink2");

		strSink1->start();
		strSink2->start();

		strSink1->wait();
		strSink2->wait();

		UT_ASSERT_EQUAL( strStream1->getNumItems(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsProcessed(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsDispatched(), array1Size );
		UT_ASSERT_EQUAL( strStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( strStream2->getNumItems(), array2Size );
		UT_ASSERT_EQUAL( strStream2->getNumItemsProcessed(), array2Size );
		UT_ASSERT_EQUAL( strStream2->getNumItemsDispatched(), array2Size );
		UT_ASSERT_EQUAL( strStream2->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( unionStream1->getNumItems(), arrayXSize );
		UT_ASSERT_EQUAL( unionStream1->getNumItemsProcessed(), arrayXSize );
		UT_ASSERT_EQUAL( unionStream1->getNumItemsDispatched(), arrayXSize );
		UT_ASSERT_EQUAL( unionStream1->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( firstLetterStream->getNumItems(), arrayXSize );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsProcessed(), arrayXSize );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsDispatched(), arrayXSize );
		UT_ASSERT_EQUAL( firstLetterStream->getNumItemsWaiting(), size_t(0) );

		size_t evenSize = evenStream->getNumItems();
		size_t oddSize = oddStream->getNumItems();

		UT_ASSERT_EQUAL( evenSize, numEvenSucces );
		UT_ASSERT_EQUAL( oddSize, numOddSucces );

		UT_ASSERT_EQUAL( arrayXSize, numEvenCalls );
		UT_ASSERT_EQUAL( arrayXSize, numOddCalls );

		UT_ASSERT_EQUAL( evenStream->getNumItemsProcessed(), evenSize );
		UT_ASSERT_EQUAL( evenStream->getNumItemsDispatched(), evenSize );
		UT_ASSERT_EQUAL( evenStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( oddStream->getNumItemsProcessed(), oddSize );
		UT_ASSERT_EQUAL( oddStream->getNumItemsDispatched(), oddSize );
		UT_ASSERT_EQUAL( oddStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( evenSize + oddSize, arrayXSize );

		size_t evenFlatmapSize = evenFlatMapped3Stream->getNumItems();
		UT_ASSERT_EQUAL( evenFlatMapped3Stream->getNumItemsProcessed(), evenFlatmapSize );
		UT_ASSERT_EQUAL( evenFlatMapped3Stream->getNumItemsDispatched(), evenFlatmapSize );
		UT_ASSERT_EQUAL( evenFlatMapped3Stream->getNumItemsWaiting(), size_t(0) );
		UT_ASSERT_EQUAL( evenFlatmapSize, evenSize*3 );

		size_t oddFlatmapSize = oddFlatMapped1Stream->getNumItems();
		UT_ASSERT_EQUAL( oddFlatMapped1Stream->getNumItemsProcessed(), oddFlatmapSize );
		UT_ASSERT_EQUAL( oddFlatMapped1Stream->getNumItemsDispatched(), oddFlatmapSize );
		UT_ASSERT_EQUAL( oddFlatMapped1Stream->getNumItemsWaiting(), size_t(0) );
		UT_ASSERT_EQUAL( oddFlatmapSize, oddSize );

		UT_ASSERT_EQUAL( unionStream2->getNumItems(), evenFlatmapSize+oddFlatmapSize );
		UT_ASSERT_EQUAL( unionStream2->getNumItemsProcessed(), evenFlatmapSize+oddFlatmapSize );
		UT_ASSERT_EQUAL( unionStream2->getNumItemsDispatched(), evenFlatmapSize+oddFlatmapSize );
		UT_ASSERT_EQUAL( unionStream2->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( randomStream->getNumItems(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsProcessed(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsDispatched(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomStream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( randomFlatMapped1Stream->getNumItems(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomFlatMapped1Stream->getNumItemsProcessed(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomFlatMapped1Stream->getNumItemsDispatched(), RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( randomFlatMapped1Stream->getNumItemsWaiting(), size_t(0) );

		UT_ASSERT_EQUAL( unionStream3->getNumItems(), evenFlatmapSize+oddFlatmapSize+RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( unionStream3->getNumItemsProcessed(), evenFlatmapSize+oddFlatmapSize+RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( unionStream3->getNumItemsDispatched(), evenFlatmapSize+oddFlatmapSize+RandomChar::NUM_ITEMS );
		UT_ASSERT_EQUAL( unionStream3->getNumItemsWaiting(), size_t(0) );
		
		UT_ASSERT_EQUAL( resultArray.size(), evenFlatmapSize+oddFlatmapSize+RandomChar::NUM_ITEMS );
	}

	Array<STRING>	myStrings1;
	Array<STRING>	myStrings2;

	size_t	array1Size;
	size_t	array2Size;
	size_t	arrayXSize;

	public:
	StreamsTest()
	{
		myStrings1.createElement() = "Hello";
		myStrings1.createElement() = "world";

		myStrings2.createElement() = "the";
		myStrings2.createElement() = "quick";
		myStrings2.createElement() = "brown";
		myStrings2.createElement() = "fox";
		myStrings2.createElement() = "jumps";
		myStrings2.createElement() = "over";
		myStrings2.createElement() = "the";
		myStrings2.createElement() = "lazy";
		myStrings2.createElement() = "dog";

		array1Size = myStrings1.size();
		array2Size = myStrings2.size();
		arrayXSize = array1Size + array2Size;
	}

	static char firstLetter( const STRING &val )
	{
		doEnterFunction("firstLetter");
		doLogValue( val );
		doLogValue( val[0U] );
		return val[0U];
	}
	static bool filterEven( char c )
	{
		doEnterFunction("filterEven");
		++numEvenCalls;
		doLogValue( c );
		doLogValue( int(c) );
		bool result = c % 2 == 0; 
		if( result )
		{
			++numEvenSucces;
		}
		return result; 
	}

	static bool filterOdd( char c )
	{
		doEnterFunction("filterOdd");
		++numOddCalls;
		doLogValue( c );
		doLogValue( int(c) );
		bool result = c % 2 != 0; 
		if( result )
		{
			++numOddSucces;
		}
		return result; 
	}

	static void flatmap1( char c, streams::StreamBuffer<STRING> &out )
	{
		doEnterFunction("flatmap1");
		doLogValue( c );
		STRING	result3;
		switch( c )
		{
			case 'H': result3 = "Hello";	break;
			case 'w': result3 = "world";	break;
			case 't': result3 = "the";		break;
			case 'q': result3 = "quick";	break;
			case 'b': result3 = "brown";	break;
			case 'f': result3 = "fox";		break;
			case 'j': result3 = "jumps";	break;
			case 'o': result3 = "over";		break;
			case 'l': result3 = "lazy";		break;
			case 'd': result3 = "dog";		break;
			default:  result3 = "Unkown ";	result3 += c; break;
		}
		doLogValue( result3 );
		out->push( result3 );
	}

	static void flatmap3( char c, streams::StreamBuffer<STRING> &out )
	{
		doEnterFunction("flatmap3");
		doLogValue( c );

		STRING	result1 = STRING(c);
		STRING	result2 = formatNumber( c );

		doLogValue( result1 );
		doLogValue( result2 );

		out->push( result1 );
		out->push( result2 );
		flatmap1( c, out );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static StreamsTest	myStreamsTest;

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

