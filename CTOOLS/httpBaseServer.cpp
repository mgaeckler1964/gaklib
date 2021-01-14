/*
		Project:		GAKLIB
		Module:			httpBaseServer.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/httpBaseServer.h>
#include <gak/t_string.h>
#include <gak/numericString.h>
#include <gak/logfile.h>

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

void HTTPserverBase::sendResponse(  void  )
{
	if( !response.getStatusCode() )
	{
		response.setStatusCode( 500 );
	}

	std::ostream	out( this );
	response.flushData( out );
	out.flush();
}

void HTTPserverBase::sendResponse( const ArrayOfData &body  )
{
	if( !response.getStatusCode() )
	{
		response.setStatusCode( 500 );
	}

	std::ostream	out( this );
	response.flushData( out, body );
	out.flush();
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int HTTPserverBase::underflow( void )
{
	if( headerReading || !request.contentLength || bytesStreamed < requestSize )
	{
		int returnChar = ServerProcessorBase::underflow();

		if( returnChar != EOF )
		{
			bytesStreamed += egptr() - eback();
		}

		return returnChar;
	}
	return EOF;
}

int HTTPserverBase::handleGetRequest( const STRING & )
{
	return 501;
}

int HTTPserverBase::handlePutRequest( void )
{
	return 501;
}

int HTTPserverBase::handlePostRequest( const STRING &, ArrayOfData & )
{
	return 501;
}

int HTTPserverBase::handlePostRequest( const STRING &url )
{
	ArrayOfData &body = request.body;

	body.setChunkSize( getBufferSize() );
	body[request.contentLength] = 0;
	size_t	i=0;
	int		c;

	while( (c=getNextByte()) >= 0 )
	{
		body[i++] = (unsigned char)c;
	}
	body[i] = 0;
	request.contentLength = i;

	return handlePostRequest( url, body );
}

int HTTPserverBase::handleHeadRequest( void )
{
	return 501;
}

void HTTPserverBase::runServerThread( void )
{
	doEnterFunction("HTTPserverBase::runServerThread");

	T_STRING	headerLine;
	CI_STRING	fieldName;
	STRING		fieldValue;
	size_t		colonPos;
	int			status;

	headerReading = true;
	bytesStreamed =	m_bytesRead = 0;

	// first line must be the method + URL
	headerLine = getNextLine();
	request.method = headerLine.getFirstToken( " " );
	request.url = headerLine.getNextToken();
	request.contentLength = 0;
	do
	{
		headerLine = getNextLine();
		if( !headerLine.isEmpty() )
		{
			colonPos = headerLine.searchChar( ':' );
			if( colonPos != headerLine.no_index )
			{
				fieldName = headerLine.leftString( colonPos );
				headerLine += colonPos +1;
				fieldValue = headerLine.stripBlanks();
				if( fieldName == "Content-Length" )
				{
					requestSize = fieldValue.getValueE<size_t>();
					request.contentLength = requestSize;
				}
				else
					request.headerLines[fieldName] = fieldValue;
			}
			else
			{
				request.headerLines[headerLine] = STRING(headerLine);
			}
		}
		else
			break;

	} while( !m_socketError && !headerLine.isEmpty() );

	headerReading = false;
	requestSize += m_bytesRead;

	if( request.method == "get" )
		status = handleGetRequest( request.url );
	else if( request.method == "head" )
		status = handleHeadRequest();
	else if( request.method == "post" )
		status = handlePostRequest( request.url );
	else if( request.method == "put" )
		status = handlePutRequest();
	else
		status = 500;

	if( status )
	{
		// response not yet sent
		response.setStatusCode( status );

		sendResponse();
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void HTTPserverBase::setLocation( const STRING &location )
{
	STRING		newLocation;

	if( strncmp( location, "http://", sizeof( "http://" ) - 1 )
	&&  strncmp( location, "https://", sizeof( "https://" ) - 1 ) )
	{
		newLocation = "http://";
		STRING host = request.headerLines["host"];
		if( !host.isEmpty() )
			newLocation += host;
		else
		{
			/* should try to find the host name later */
			newLocation += "localhost";

			unsigned short port = getPort();
			if( port != 80 )
			{
				newLocation += ':';
				newLocation += formatNumber( port );
			}
		}

#if 0
		if( location[0U] != '/' )
		{
			if( (env = getenv("SCRIPT_NAME")) != NULL )
			{
				newLocation += env;
			}
		}
#endif
	}

	newLocation += location;

	response.setLocation( newLocation );
}
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

