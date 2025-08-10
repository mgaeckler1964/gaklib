/*
		Project:		GAKLIB
		Module:			SoapClientTest.h
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

#define TEST_IMPORTER	1
#define TEST_CLIENT		1

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/wsdlImporter.h>

#if TEST_CLIENT
// the C# Soap Server
#include "Service1.cpp"

// the PHP Soap Server
#include "myService.cpp"
#endif

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

#	pragma warn -inl
#endif

namespace gak
{
using namespace net;

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

class SoapClientTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return DISABLED_TEST_PREFIX "SoapClientTest";	// currently disabled since the C# webservice is not running
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "SoapClientTest::PerformTest");
		TestScope scope( "PerformTest" );

#if TEST_IMPORTER
		{
			TestScope	scope( "the C# Soap Server" );
			importWSDL("http://saturn/WebService/Service1.asmx?WSDL");
		}

		{
			TestScope	scope( "the PHP Soap Server" );
			importWSDL("http://judith/WebService/service.php?wsdl", "root", "password" );
		}
#endif

#if TEST_CLIENT
		TestCsharpService();
		TestPHPService();
#endif
	}

#if TEST_IMPORTER
	void importWSDL(
		const STRING &wsdl,
		const STRING &userName=STRING((const char*)NULL),
		const STRING &password=STRING((const char*)NULL)
	)
	{
		WSDLimporter	theRequest( wsdl, userName, password );
		theRequest.writeCppClass( "Tests" DIRECTORY_DELIMITER_STRING );
	}
#endif

#if TEST_CLIENT
	void TestCsharpService()
	{
		STRING		result;
		Service1	myCsharpService;

		try
		{
			result = myCsharpService.HelloWorld();
			UT_ASSERT_EQUAL( result, STRING("Hello World") );

			result = myCsharpService.StrCat( "Martin", "Gaeckler" );
			UT_ASSERT_EQUAL( result, STRING("Martin Gaeckler") );

			Service1::TestClass_t	param;
			param.name = "Martin Gaeckler";
			param.id = 1;

			result = myCsharpService.extractName( param );
			UT_ASSERT_EQUAL( result, STRING("Martin Gaeckler") );
		}
		catch( ... )
		{
			result = myCsharpService.getXmlRequest();
			std::cout << result << '\n';
			result = myCsharpService.getHttpResponse().getStatusText();
			std::cout << result << '\n';
			result = myCsharpService.getHttpResponse().getBody();
			std::cout << result << '\n';
//			throw;
		}
	}
	void TestPHPService()
	{
		STRING		result;
		myService	myPHPservice;

		bool	gotException = false;
		if( myPHPservice.getNeedCredentials() )
			myPHPservice.setCredentials( "root", "password" );
		try
		{
			result = myPHPservice.get_message( "" );
		}
		catch( SoapRequest::SoapException &myException )
		{
			gotException = true;
			std::cout << myException.faultCode << myException.faultActor << myException.faultString << myException.faultDetail;
			result = myPHPservice.getBody();
			std::cout << result << '\n';
		}
		catch( ... )
		{
			result = myPHPservice.getBody();
			std::cout << result << '\n';
		}
		UT_ASSERT_TRUE( gotException );

		result = myPHPservice.get_message( "Martin" );
		UT_ASSERT_EQUAL( result, STRING("Welcome Martin.") );
		std::cout << result << '\n';

/*
		result = myPHPservice.getXmlRequest();
		cout << result << '\n';
		result = myPHPservice.getHttpResponse().getBody();
		cout << result << '\n';
*/

		myService::MyRecord_t myStruct;
		myStruct.ID = 1;
		myStruct.YourName = "Martin";
		xml::Element	*xmlResult = myPHPservice.HelloComplexWorld( myStruct );
//		result = myPHPservice.getXmlRequest();
//		cout << result << '\n';
//		result = myPHPservice.getHttpResponse().getBody();
//		cout << result << '\n';
		UT_ASSERT_NOT_NULL( xmlResult );
		if( xmlResult )
		{
			Array<myService::MyRecord_t>	resultTable;
			myPHPservice.fillMyRecord( resultTable, xmlResult );
			UT_ASSERT_EQUAL( resultTable.size(), size_t(3) );

			for( size_t i=0; i<resultTable.size(); i++ )
			{
				const myService::MyRecord_t	&newItem = resultTable[i];

				std::cout << newItem.ID << ' ' << newItem.YourName << '\n';

				UT_ASSERT_EQUAL( int(newItem.ID), int(i+2) );
				UT_ASSERT_EQUAL( newItem.YourName, STRING("Hello Martin") );
			}
//			result = xmlResult->getValue( XML_MODE );
//			cout << result << '\n';
		}
	}
#endif
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static SoapClientTest mySoapClientTest;

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

#	pragma warn +inl
#endif

