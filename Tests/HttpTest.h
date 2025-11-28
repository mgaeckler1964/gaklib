/*
		Project:		GAKLIB
		Module:			HttpTest.h
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

#include <gak/http.h>
#include <gak/httpBaseServer.h>

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

ArrayOfData	serverResult;

class TestWebServer : public HTTPserverBase
{
	private:
	virtual int handleGetRequest( const STRING &url );
};

class HttpTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "HttpTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "HttpTest::PerformTest");
		TestScope scope( "PerformTest" );

		LockGuard		lock( s_testLocker );
		SocketServer<TestWebServer>	myServer;

		HTTPrequest	myClient;
		STRING	   	url = "https://localhost:6666/" __FILE__;

		/*
			start server and client
		*/
		myServer.startServer( 6666, 1 );
		Sleep( 1000 );
		STRING	socketError = myServer.getSocketError();
		UT_ASSERT_EQUAL( socketError, EMPTY_STRING );
		if( socketError.isEmpty() )
		{
			myClient.Get( url );
			myServer.stopServer( false );

			/*
				get client results
			*/
			size_t size = myClient.getHttpResponse().getContentLength();
			const char *cp = myClient.getHttpResponse().getBody();

			/*
				compare with server data
			*/
			UT_ASSERT_EQUAL( size, serverResult.size() );
			int status = myClient.getHttpResponse().getStatusCode();
			UT_ASSERT_EQUAL( status, 200 );

			for( size_t i=0; i<serverResult.size(); i++ )
			{
				char c1 = cp[i];
				char c2 = serverResult[i];
				UT_ASSERT_EQUAL( c1, c2 );
				if( c1 != c2 )
					break;
			}
			if( size > serverResult.size() )
			{
				STRING	extra = cp + serverResult.size();
				UT_ASSERT_EQUAL( extra, EMPTY_STRING );
			}
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static HttpTest myHttpTest;

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

int TestWebServer::handleGetRequest( const STRING &url )
{
	doEnterFunctionEx(gakLogging::llInfo, "TestWebServer::handleGetRequest");

	try
	{
		STRING			file = url+size_t(1);		// ignore directory delimiter
		DirectoryEntry	theFile( file );

		readFromFile( &serverResult, file );

		response.setStatusCode( 200 );
		sendResponse( serverResult );

		return 0;
	}
	catch( ... )
	{
		return 400;
	}
}

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

