/*
		Project:		GAKLIB
		Module:			TEST.CPP
		Description:	The Test
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

#define DO_UNIT_TEST	1
#define DO_PREPROCESSOR	0
#define USE_LOCAL_ONLY	0

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if DO_PREPROCESSOR
#	include <gak/cppParser.h>
#	include <gak/cppPreprocessor.h>
#endif

#if DO_UNIT_TEST

#if defined( __BORLANDC__ ) && __BORLANDC__ >= 0x550
#	include <aclapi.h>
#endif

#include <gak/arrayFile.h>
#include <gak/numericString.h>
#include <gak/unitTest.h>
#include <gak/directory.h>

#ifndef __BORLANDC__
#	include "Tests/StreamsTest.h"
#endif

#include "Tests/HostResolverTest.h"
#include "Tests/GraphTest.h"
#include "Tests/GeoGraphTest.h"
#include "Tests/RoutingTest.h"

#include "Tests/GeometryTest.h"
#include "Tests/IndexerTest.h"
#include "Tests/QuantitiesTest.h"
#include "Tests/FmtNumberTest.h"
#include "Tests/MathTest.h"
#include "Tests/SharedTest.h"
#include "Tests/StopWatchTest.h"
#include "Tests/PriorityQueueTest.h"
#include "Tests/OptionalTest.h"
#include "Tests/ConditionalTest.h"
#include "Tests/ThreadPoolTest.h"
#include "Tests/FieldSetTest.h"
#include "Tests/ContainerTest.h"
#include "Tests/CmdlineTest.h"
#include "Tests/CppTest.h"
#include "Tests/TypeSizeTest.h"
#include "Tests/GpsTest.h"
#include "Tests/FileIDTest.h"
#include "Tests/RFileTest.h"
#include "Tests/MapTest.h"
#include "Tests/DirectoryTest.h"
#include "Tests/MatrixTest.h"
#include "Tests/DateTest.h"
#include "Tests/DateTimeTest.h"
#include "Tests/BtreeTest.h"
#include "Tests/HttpTest.h"
#include "Tests/StringStreamTest.h"
#include "Tests/UnicodeTest.h"
#include "Tests/PathTest.h"
#include "Tests/FractionTest.h"
#include "Tests/PrimeTest.h"
#include "Tests/FcopyTest.h"
#include "Tests/BitFieldTest.h"
#include "Tests/StringTest.h"
#include "Tests/ArrayListTest.h"
#include "Tests/XmlTest.h"
#include "Tests/DirectoryListTest.h"
#include "Tests/QueueStackTest.h"
#include "Tests/SleeperTest.h"
#include "Tests/AlgorithmTest.h"
#include "Tests/PipelineTest.h"
#include "Tests/BlockedRingBufferTest.h"
#include "Tests/EvaluatorTest.h"
#include "Tests/PtrArrayTest.h"
#include "Tests/IostreamTest.h"


#if !USE_LOCAL_ONLY
#	include "Tests/SoapClientTest.h"
#	include "Tests/ExifTest.h"
#endif

#ifndef __WIN64__
	// curently no support for 64-bit openSSL
#	include "Tests/CryptoTest.h"
#endif



#ifdef NDEBUG
#	include "Tests/PerformanceTest.h"
#endif

#endif	// DO_UNIT_TEST

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

#	pragma warn -inl
#	pragma warn -pch
#	pragma warn -csu
#endif

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

#if DO_UNIT_TEST
/*
#include <iostream>
#include <gak/unitTest.h>

namespace gak
{

class TemplateTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "TemplateTest";
	}
	virtual void PerformTest( void )
	{
	}
};

static TemplateTest	myTemplateTest;

}	//namespace gak

*/
#endif	// DO_UNIT_TEST

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

#if DO_UNIT_TEST

int main( int, const char *argv[] )
{
	gak::F_STRING	sourcePath;

	gak::fsplit( gak::fullPath( __FILE__ ), &sourcePath );
	if( !sourcePath.isEmpty() )
	{
		std::cout << "Changing directory to " << sourcePath << std::endl;
		if( setcwd( sourcePath ) )
		{
			perror( sourcePath );
		}
	}
	else
	{
		std::cout << "Don't know Source Path, using " << gak::getcwd() << std::endl;
	}

	gak::UnitTest::PerformTests( argv );
	gak::UnitTest::PrintResult();

#ifdef _Windows
	getchar();
#endif
	return EXIT_SUCCESS;
}

#elif DO_PREPROCESSOR
//=========================================================================

int main( void )
{
	CPPparser		myFile( "C:\\CRESD\\SOURCE\\GAKLIB\\cppTest.txt" );
	//CPPparser		myFile( __FILE__ );
	CPreprocessor	myProcessor(CPreprocessor::omText);

//	myProcessor.addIncludePath( "E:\\VisualStudion2010\\VC\\INCLUDE\\" );
	myProcessor.addIncludePath( "C:\\CRESD\\SOURCE\\" );
	myProcessor.addIncludePath( "C:\\CRESD\\SOURCE\\GAKLIB\\INCLUDE\\" );
	myProcessor.addMacro( "__cplusplus", "1" );
#ifdef _Windows
	myProcessor.addMacro( "_Windows", "1" );
#endif
#ifdef __MACH__ 
	myProcessor.addMacro( "__MACH__ ", "1" );
#endif
#ifdef __unix__  
	myProcessor.addMacro( "__unix__  ", "1" );
#endif

	myProcessor.precompile( &myFile, std::cout );
	STRING error = myFile.getErrors();
	if( !error.isEmpty() )
		std::cerr << error << std::endl;
	const Includes &includeFiles = myProcessor.getIncludeFiles();
	for(
		Includes::const_iterator it = includeFiles.cbegin(), endIT = includeFiles.cend();
		it != endIT;
		++it 
	)
	{
		std::cout << it->getKey() << std::endl;
		const Set<F_STRING>	&sourceFiles = it->getValue();
		for(
			Set<F_STRING>::const_iterator it = sourceFiles.cbegin(), endIT = sourceFiles.cend();
			it != endIT;
			++it 
		)
		{
			std::cout << '\t' << *it << std::endl;
		}
	}
}
#else
#include <iostream>

class TestClass
{
	const char	*m_text;
	int			m_number;
	
	protected:
	const char *getText() const
	{
		return m_text;
	}
	public:
	TestClass( const char *text, int number ) : m_text( text ), m_number(number)
	{
	}
	operator const char * () const
	{
		return m_text;
	}
	int getNumber() const
	{
		return m_number;
	}
};

class DerivedClass : private TestClass
{
	public:
	DerivedClass( const char *text, int number ) : TestClass( text, number )
	{
	}
	operator const TestClass * () const
	{
		return this;
	}
	int getNumber() const
	{
		return TestClass::getNumber();
	}

};

int main( void )
{
	TestClass		test( "Hello", 666 );
	
	if( test && test.getNumber() )
	{
		std::cout << test.getNumber() << std::endl;
	}

	DerivedClass	derived( "Hello", 123 );

	if( derived && derived.getNumber() )
	{
		std::cout << derived.getNumber() << std::endl;
	}
	return 0;
}
#endif	// DO_PREPROCESSOR

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
