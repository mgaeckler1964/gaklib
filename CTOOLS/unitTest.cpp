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

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
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

#include <iomanip>
#include <sstream>

#include <gak/stopWatch.h>
#include <gak/gaklib.h>
#include <gak/unitTest.h>
#include <gak/set.h>

#include <gak/soap.h>

#include <gak/locker.h>
#include <gak/logfile.h>

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

#define REDIRECT_COUT 1

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static std::size_t TEST_NAME_WIDTH = 30;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

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

void UnitTest::PerformTest( UnitTest *theTest )
{
	std::size_t			errorCount = s_errorCount;
	STRING				exceptionType, errorText;
#if REDIRECT_COUT
	std::ostringstream	ostr, oerrStr;
    std::streambuf		*coutbuf = std::cout.rdbuf();
    std::streambuf		*cerrbuf = std::cerr.rdbuf();

    std::cout.rdbuf(ostr.rdbuf());
    std::cerr.rdbuf(oerrStr.rdbuf());
#endif
	StopWatch	stopWatch( true );
	try
	{
		std::stringstream	out;
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
#if REDIRECT_COUT
	std::cout.flush();
	std::cerr.flush();
	ostr.flush();
	oerrStr.flush();
    std::cout.rdbuf(coutbuf);
    std::cerr.rdbuf(cerrbuf);
#endif
	theTest->m_ellapsedTime = stopWatch.get< Hours<std::clock_t> >();
	theTest->m_tested = true;
	if( !exceptionType.isEmpty() || !errorText.isEmpty() )
	{
		AddResult(
			theTest->GetClassName(), theTest->GetClassName(), 0, theTest->GetClassName(),
			exceptionType + ": " + errorText, false
		);

	}

	if( errorCount == s_errorCount )
	{
		std::cout << " OK" << std::endl;
	}
	else
	{
		std::cout << " FAILED\n";
#if REDIRECT_COUT
		std::cout	<< ostr.str() << '\n'
					<< oerrStr.str() << std::endl;
#endif
	}
}

void UnitTest::PerformTests( const char *argv[] )
{
	doEnterFunctionEx(gakLogging::llInfo, "UnitTest::PerformTests");

	SortedArray<const char*>	testsToPerform;

	for( ++argv; *argv; ++argv )
	{
		testsToPerform.addElement( *argv );
	}

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
			PerformTest( theTest );

			if( testFilter )
			{
				testsToPerform.removeElementAt( testIndex );
			}
		}
	}
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
	std::cout << numDisabledTests << " disabled test(s)" << std::endl;
}

void UnitTest::PrintResult( void )
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

	if( !errorCount )
		std::cout << "\nCongratulations!!!!\n";
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


