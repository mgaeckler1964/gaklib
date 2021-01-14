/*
		Project:		GAKLIB
		Module:			httpBaseServer.h
		Description:	Base classes used for an HTTP server
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

#ifndef HTTP_BASE_SERVER_H
#define HTTP_BASE_SERVER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/socketServer.h>
#include <gak/httpResponse.h>
#include <gak/fieldSet.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{
namespace net
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

/**
	@brief base for an HTTP server

	you must overload one or more of the following functions:

	\li virtual int handleGetRequest( const STRING &url );
	\li virtual int handlePutRequest( void );
	\li virtual int handlePostRequest( const STRING &url );
	\li virtual int handlePostRequest( const STRING &url, ArrayOfData &body );
	\li virtual int handleHeadRequest( void );

	The default implementations return an HTTP status of 501 to the client

	Then create a server with your derived class and start the server

	@code
		SocketServer<TestWebServer>	myServer;
		myServer.startServer( 6666, 100 );
	@endcode

	@see SocketServer
*/
class HTTPserverBase : public ServerProcessorBase
{
	private:
	bool	headerReading;
	size_t	bytesStreamed, requestSize;

	protected:
	/// the client request sent to the server
	struct HTTPserverRequest
	{
		/// the request method
		CI_STRING		method;
		/// the request URL
		STRING			url;
		/// the length of the post data
		size_t			contentLength;

		/// the header fields
		FieldSet		headerLines;

		/// the post data
		ArrayOfData		body;
	};

	/// the client request sent to the server
	HTTPserverRequest	request;
	/// the server response sent to the client
	HTTPserverResponse	response;

	/// creates a new HTTP Server used for the listener thread
	HTTPserverBase() : ServerProcessorBase()
	{
		headerReading = false;
		bytesStreamed = requestSize = 0;
	}
	/// creates a new HTTP Server used for the worker thread
	private:
	virtual int underflow( void );
	virtual void runServerThread( void );

	virtual int handleGetRequest( const STRING &url );
	virtual int handlePutRequest( void );
	virtual int handlePostRequest( const STRING &url );
	virtual int handlePostRequest( const STRING &url, ArrayOfData &body );
	virtual int handleHeadRequest( void );

	protected:
	/// sends the response to the client
	void sendResponse( void );
	/**
		@brief sends the response to the client
		@param [in] body the post data that will be send to the client
	*/
	void sendResponse( const ArrayOfData &body );

	/// @copydoc HTTPserverResponse::setStatusCode
	void setStatusCode( int statusCode )
	{
		response.setStatusCode( statusCode );
	}
	/// @copydoc HTTPserverResponse::setContentType
	void setContentType( const STRING &contentType )
	{
		response.setContentType( contentType );
	}
	/// @copydoc HTTPserverResponse::setLastModified
	void setLastModified( const DateTime &lastModified )
	{
		response.setLastModified( lastModified );
	}
	/// @copydoc HTTPserverResponse::setError
	void setError( const STRING &error )
	{
		response.setError( error );
	}
	/// @copydoc HTTPserverResponse::setLocation
	void setLocation( const STRING &location );

#if defined( __BORLANDC__ )
	void setLastModified( TDateTime lastModified )
	{
		response.setLastModified( lastModified );
	}
#endif
};

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

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
