/*
		Project:		GAKLIB
		Module:			unitTest.cpp
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

#ifdef __BORLANDC__
#	pragma warn -inl
#	pragma warn -csu
#	include <stdexcep.h>
#	pragma warn +inl
#	pragma warn +csu
#else
#	include <exception>
#endif

#include <memory>
#include <iomanip>
#include <sstream>

#include <gak/stopWatch.h>
#include <gak/gaklib.h>
#include <gak/unitTest.h>
#include <gak/set.h>
#include <gak/threadPool.h>
#include <gak/soap.h>

#include <gak/locker.h>
#include <gak/logfile.h>
#include <gak/exception.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -x
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

static std::size_t			TEST_NAME_WIDTH = 30;
static const std::size_t	NUM_POOL_THREADS = 8;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef ThreadPool< UnitTest*>	TestPool;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <>
class ProcessorType<UnitTest*>
{
	public:
	typedef UnitTest*	object_type;
	/**
		@brief processes one item
		@param [in] objectToProcess the item to process
	*/
	static void process( const object_type &objectToProcess, void *, void * )
	{
		objectToProcess->PerformThreadTest();
	}
};

class StreamCatcher
{
	std::ostringstream	ostr, oerrStr;
    std::streambuf		*coutbuf, *cerrbuf;
	bool m_caught;

	public:
	StreamCatcher( bool catchStream )
	{
		if( catchStream )
		{
			coutbuf = std::cout.rdbuf();
			cerrbuf = std::cerr.rdbuf();

			std::cout.rdbuf(ostr.rdbuf());
			std::cerr.rdbuf(oerrStr.rdbuf());
		}
		m_caught = catchStream;
	}
	void release()
	{
		if( m_caught )
		{
			std::cout.flush();
			std::cerr.flush();
			ostr.flush();
			oerrStr.flush();
			std::cout.rdbuf(coutbuf);
			std::cerr.rdbuf(cerrbuf);
			m_caught = false;
		}
	}
	void printResult() const
	{
		std::cout	<< ostr.str() << '\n'
					<< oerrStr.str() << std::endl;
	}

	~StreamCatcher()
	{
		release();
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

const char TESTED_FILENAME[] = ".tested";

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

Array<StressResult>	UnitTest::s_theStressResults;
Array<TestResult>	UnitTest::s_theTestResults;
Stack<const char *>	UnitTest::s_scopes;
Locker				UnitTest::s_testLocker;
std::size_t			UnitTest::s_errorCount = 0;


// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static void addToTested( const STRING &testName )
{
	std::ofstream out(TESTED_FILENAME, std::ios_base::app);
	out << testName << '\n';
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

Array<UnitTest*> &UnitTest::getTheTestItems()
{
	static Array<UnitTest*>	theTestItems;

	return theTestItems;
}

void UnitTest::PerformTests( const char *argv[] )
{
	std::cout << "catching signals " << std::endl;
	SignalException::catchSignals();

	TestMode tm = tmTest;
	bool		catchCout = true,
				excludeTests = false,
				checkTested = false;
	ArrayOfStrings	alreadyTested;

	SortedArray<const char*>	testsToPerform;
	for( ++argv; *argv; ++argv )
	{
		if( !strcmpi( *argv, "-stress" ) )
			tm = tmStress;
		else if( !strcmpi( *argv, "-mt" ) )
			tm = tmThread;
		else if( !strcmpi( *argv, "-showIO" ) )
			catchCout = false;
		else if( !strcmpi( *argv, "-exclude" ) )
			excludeTests = true;
		else if( !strcmpi( *argv, "-ct" ) )
			checkTested = true;
		else
			testsToPerform.addElement( *argv );
	}

	if( checkTested )
		alreadyTested.readFromFile(TESTED_FILENAME);

	if( excludeTests || (checkTested && alreadyTested.size()) )
	{
		const Array<UnitTest*>	&theTestItems = getTheTestItems();
		SortedArray<const char*>	excludes;
		excludes.moveFrom( testsToPerform );

		for( 
			Array<UnitTest*>::const_iterator it = theTestItems.cbegin(), endIT = theTestItems.cend();
			it != endIT;
			++it
		)
		{
			UnitTest	*theTest = *it;
			if( !theTest->isDisabled() )
			{
				size_t		testIndex = excludeTests ? excludes.findElement( theTest->GetClassName() ) : excludes.no_index;

				if( testIndex == excludes.no_index && alreadyTested.size() )
					testIndex = alreadyTested.findElement( theTest->GetClassName() );
				if( testIndex == excludes.no_index )
				{
					testsToPerform.addElement( theTest->GetClassName() );
				}
			}
		}
	}
	if( tm == tmTest )
		PerformTests( testsToPerform, catchCout, checkTested );
	else if( tm == tmStress )
		StressTests( testsToPerform, checkTested );
	else
		ThreadTest( testsToPerform, checkTested );
}

void UnitTest::ShowNotFound( const SortedArray<const char*> &testsToPerform )
{
	for( 
		SortedArray<const char*>::const_iterator it = testsToPerform.cbegin(), endIT = testsToPerform.cend();
		it != endIT;
		++it
	)
	{
		std::cout << "Test " << *it << " not found!" << std::endl;
		AddResult(
			*it, *it, 0, *it,
			"Test not found!", false
		);
	}
}

/*
	---------------------------------------------------------------------------
		Unit Tests
	---------------------------------------------------------------------------
*/
bool UnitTest::PerformTest( UnitTest *theTest, bool catchCout )
{
	std::size_t			errorCount = s_errorCount;
	STRING				exceptionType, errorText;
	StreamCatcher		catcher( catchCout );
	StopWatch			stopWatch( true );
	try
	{
		theTest->PerformTest();
	}
	catch( net::SoapRequest::SoapException &myException )
	{
		exceptionType = "net::SoapRequest::SoapException &myException";
		errorText =  myException.faultCode +':'+ myException.faultActor +':'+ myException.faultString +':'+ myException.faultDetail;
	}
	catch( LibraryException &e )
	{
		exceptionType = "gak::LibraryException";
		errorText = e.what();
	}
	catch( std::exception &e )
	{
		exceptionType = "std::exception";
		errorText = e.what();
	}
	catch( ... )
	{
		exceptionType = "Unknown Exception";
		errorText = "Unknown";
	}
	catcher.release();

	theTest->m_ellapsedTime = stopWatch.get< Hours<std::clock_t> >();
	theTest->m_tested = true;
	if( !exceptionType.isEmpty() || !errorText.isEmpty() )
	{
		AddResult(
			theTest->GetClassName(), theTest->GetClassName(), 0, theTest->GetClassName(),
			exceptionType + ": " + errorText, false
		);

	}

	if( catchCout )
	{
		if( errorCount == s_errorCount )
		{
			std::cout << " OK" << std::endl;
		}
		else
		{
			std::cout << " FAILED\n";
			catcher.printResult();
		}
	}

	return errorCount < s_errorCount;
}

void UnitTest::PerformTests( SortedArray<const char*> &testsToPerform, bool catchCout, bool checkTested )
{
	doEnterFunctionEx(gakLogging::llInfo, "UnitTest::PerformTests");

	std::size_t				i=1;
	std::size_t				testFilter = testsToPerform.size();
	const Array<UnitTest*>	&theTestItems = getTheTestItems();
	std::size_t				numElements = testFilter ? testFilter : theTestItems.size();
	std::size_t				width = math::getExponent( double(numElements) )+1;
	std::size_t				numDisabledTests = 0;
	for( 
		Array<UnitTest*>::const_iterator it = theTestItems.cbegin(), endIT = theTestItems.cend();
		it != endIT;
		++it
	)
	{
		UnitTest	*theTest = *it;
		size_t		testIndex = testsToPerform.findElement( theTest->GetClassName() );

		if( !testFilter && theTest->isDisabled() )
		{
			numDisabledTests++;
			continue;
		}
		if( !testFilter || testIndex != testsToPerform.no_index )
		{
			std::cout << "Performing " << std::setw(width) << (i++) << '/' << numElements << ' ' 
					  << STRING(theTest->GetClassName()).pad(TEST_NAME_WIDTH, STR_P_RIGHT ) << std::flush;
			bool errFlag = PerformTest( theTest, catchCout );
			if( !errFlag && checkTested )
				addToTested(theTest->GetClassName());

			if( testFilter )
			{
				testsToPerform.removeElementAt( testIndex );
			}
		}
	}
	ShowNotFound(testsToPerform);
	std::cout << numDisabledTests << " disabled test(s)" << std::endl;
}

void UnitTest::PrintResult()
{
	LockGuard	lock( s_testLocker );

	Set<STRING>	failedClasses;
	size_t		numTestedClasses = 0;

	std::cout << "\n\nThe Result:\n===========\n";

	const Array<UnitTest*>	&theTestItems = getTheTestItems();
	std::cout << "\nExecution Time(s):\n";
	for( 
		Array<UnitTest*>::const_iterator it = theTestItems.cbegin(), endIT = theTestItems.cend();
		it != endIT;
		++it
	)
	{

		const UnitTest	*theTest = *it;
		if( theTest->m_tested )
		{
			std::cout << STRING(theTest->GetClassName()).pad(TEST_NAME_WIDTH, STR_P_RIGHT ) 
					  << ' ' << theTest->m_ellapsedTime.toString() << std::endl;
			numTestedClasses++;
		}
	}

	std::cout << "\nError(s):\n";
	std::size_t	errorCount = 0;
	for( 
		Array<TestResult>::const_iterator it = s_theTestResults.cbegin(), endIT = s_theTestResults.cend();
		it != endIT;
		++it
	)
	{
		const TestResult	&theResult = *it;
		if( !theResult.m_success )
		{
			errorCount++;
			std::cout << theResult << '\n';
			failedClasses += STRING(theResult.m_className);
		}
	}
	if( !errorCount )
	{
		std::cout << "None 8-)" << std::endl;
	}
	if( failedClasses.size() )
	{
		std::cout << "\nFailed Class(es):\n";
		for( 
			Set<STRING>::const_iterator it = failedClasses.cbegin(), endIT = failedClasses.cend();
			it != endIT;
			++it
		)
			std::cout << *it << '\n';
	}

	std::cout	<< "\nSummary:\n" 
				<< theTestItems.size() << " Test Class(es) " << numTestedClasses << " Tested Class(es) " << (theTestItems.size()-numTestedClasses) << " skipped class(es)\n"
				<< s_theTestResults.size() << " Test Result(s)\n"
				<< failedClasses.size() << " Failed Class(es)\n"
				<< errorCount << " error(s)\n"
	;

	for( 
		Array<StressResult>::const_iterator it = s_theStressResults.cbegin(), endIT = s_theStressResults.cend(); 
		it != endIT;
		++it )
	{
		std::cout << it->testName << '\t' << it->goodCount << ' ' << it->goodTime << '\t' << it->badCount << ' ' << it->badTime << std::endl;
	}
	if( !errorCount )
		std::cout << "\nCongratulations!!!!\n";
}

/*
	---------------------------------------------------------------------------
		Stress Tests
	---------------------------------------------------------------------------
*/

bool UnitTest::StressTest( UnitTest *theTest )
{
	size_t	count = 1;
	clock_t time, lastTime;
	size_t errorCount = s_errorCount;

	StopWatch	stopWatch( true );
	while( true )
	{
		theTest->StressTest( count );
		stopWatch.stop();
		time = stopWatch.getMillis();
		if( count == 1 )
		{
			lastTime = time;
		}
		std::cout << ' ' << count << ' ' << (time/1000.0) << 's' << std::endl;

		if( time > 3*lastTime && count > 7 )
			break;

		count *= 2;
		stopWatch.start();
	}

	if( count > 1 )
	{
		LockGuard	lock( s_testLocker );
		StressResult &sr = s_theStressResults.createElement();
		sr.testName = theTest->GetClassName();
		sr.goodCount = count/2;
		sr.badCount = count;
		sr.goodTime = lastTime;
		sr.badTime = time;
	}
	
	return errorCount < s_errorCount;
}

void UnitTest::StressTests( SortedArray<const char*> &testsToPerform, bool checkTested )
{
	doEnterFunctionEx(gakLogging::llInfo, "UnitTest::StressTests");

	std::size_t				i=1;
	std::size_t				testFilter = testsToPerform.size();
	const Array<UnitTest*>	&theTestItems = getTheTestItems();
	std::size_t				numElements = testFilter ? testFilter : theTestItems.size();
	std::size_t				width = math::getExponent( double(numElements) )+1;
	std::size_t				numDisabledTests = 0;
	for( 
		Array<UnitTest*>::const_iterator it = theTestItems.cbegin(), endIT = theTestItems.cend();
		it != endIT;
		++it
	)
	{
		UnitTest	*theTest = *it;
		size_t		testIndex = testsToPerform.findElement( theTest->GetClassName() );

		if( !testFilter && theTest->isDisabled() )
		{
			numDisabledTests++;
			continue;
		}
		if( theTest->canStressTest() && (!testFilter || testIndex != testsToPerform.no_index ))
		{
			std::cout << "Stressing " << std::setw(width) << (i++) << '/' << numElements << ' ' 
					  << STRING(theTest->GetClassName()).pad(TEST_NAME_WIDTH, STR_P_RIGHT ) << std::flush;
			bool errFlag = StressTest( theTest );
			if( !errFlag && checkTested )
				addToTested(theTest->GetClassName());

			if( testFilter )
			{
				testsToPerform.removeElementAt( testIndex );
			}
		}
	}
	ShowNotFound(testsToPerform);
	std::cout << numDisabledTests << " disabled test(s)" << std::endl;
}

/*
	---------------------------------------------------------------------------
		MultiThread Tests
	---------------------------------------------------------------------------
*/

void UnitTest::PerformThreadTest()
{
	try
	{
		std::auto_ptr<UnitTest> myDup( duplicate() );
		assert( myDup.get() );
		myDup->PerformTest();
	}
	catch( ... )
	{
		AddResult(
			GetClassName(), GetClassName(), 0, GetClassName(),
			"Exception within PerformThreadTest", false
		);
	}
}

bool UnitTest::ThreadTest( UnitTest *theTest, void *pool )
{
	size_t errorCount = s_errorCount;
	StreamCatcher	catcher( true );
	TestPool *muliThreadTestPool = static_cast<TestPool*>(pool);
	for( int i=0; i<NUM_POOL_THREADS*2; ++i )
	{
		muliThreadTestPool->process( theTest );
	}
	PerformTest(theTest, false);
	muliThreadTestPool->flush();

	catcher.release();
	if( errorCount == s_errorCount )
	{
		std::cout << " OK" << std::endl;
	}
	else
	{
		std::cout << " FAILED\n";
		catcher.printResult();
	}
	return errorCount < s_errorCount;
}

void UnitTest::ThreadTest( SortedArray<const char*> &testsToPerform, bool checkTested )
{
	doEnterFunctionEx(gakLogging::llInfo, "UnitTest::ThreadTest");

	std::size_t				i=1;
	std::size_t				testFilter = testsToPerform.size();
	const Array<UnitTest*>	&theTestItems = getTheTestItems();
	std::size_t				numElements = testFilter ? testFilter : theTestItems.size();
	std::size_t				width = math::getExponent( double(numElements) )+1;
	std::size_t				numDisabledTests = 0;

	TestPool				muliThreadTestPool(NUM_POOL_THREADS, "UnitTestThreads", nullptr);

	muliThreadTestPool.start();

	for( 
		Array<UnitTest*>::const_iterator it = theTestItems.cbegin(), endIT = theTestItems.cend();
		it != endIT;
		++it
	)
	{
		UnitTest	*theTest = *it;
		size_t		testIndex = testsToPerform.findElement( theTest->GetClassName() );

		if( !testFilter && theTest->isDisabled() )
		{
			numDisabledTests++;
			continue;
		}
		if( theTest->canThreadTest() && (!testFilter || testIndex != testsToPerform.no_index ))
		{
			std::cout << "Threading " << std::setw(width) << (i++) << '/' << numElements << ' ' 
					  << STRING(theTest->GetClassName()).pad(TEST_NAME_WIDTH, STR_P_RIGHT ) << std::flush;;
			bool errFlag = ThreadTest( theTest, &muliThreadTestPool );
			if( !errFlag && checkTested )
				addToTested(theTest->GetClassName());

			if( testFilter )
			{
				testsToPerform.removeElementAt( testIndex );
			}
		}
	}
	muliThreadTestPool.flush();
	muliThreadTestPool.shutdown();

	ShowNotFound(testsToPerform);
	std::cout << numDisabledTests << " disabled test(s)" << std::endl;
}

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
//#	pragma option -x.
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif


