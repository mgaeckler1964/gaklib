/*
		Project:		GAKLIB
		Module:			httpResponse.cpp
		Description:	HTTP response for CGI applications or HTTP server
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

#include <gak/httpResponse.h>
#include <gak/http.h>

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

struct HTTP_STATUS_CODES
{
	int statusCode;
	const char	*statusText;
} httpStatusCodes[] =
{
	{ 200, "OK" },
	{ 301, "Moved Permanently" },
	{ 302, "Found" },
	{ 304, "Not Modified" },
	{ 401, "Unauthorized" },
	{ 404, "Object Not Found" },
	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ -1, NULL },
};

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

void HTTPcgiResponse::sendStatusCode( std::ostream &out ) const
{
	out << "Status: " << m_statusCode << ' ' << m_statusText << m_endln;
}

void HTTPserverResponse::sendStatusCode( std::ostream &out ) const
{
	out << "HTTP/1.1 " << m_statusCode << ' ' << m_statusText << m_endln;
}

   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING HTTPcgiResponse::getLocation( const STRING &location ) const
{
	STRING		newLocation;
	const char	*env;

	if( strncmp( location, "http://", sizeof( "http://" ) - 1 )
	&&  strncmp( location, "https://", sizeof( "https://" ) - 1 ) )
	{
		char *https = getenv( "HTTPS" );
		bool sslHttp = https && !strcmpi( https, "on" );
		const char *defPort = sslHttp ? "443" : "80"; 

		newLocation = sslHttp ? "https://" : "http://";
		if( (env = getenv("SERVER_NAME")) != NULL )
			newLocation += env;
		else
			newLocation += "localhost";

		if( (env = getenv("SERVER_PORT")) != NULL && strcmp( env, defPort ) )
		{
			newLocation += ':';
			newLocation += env;
		}

		if( location[0U] != '/' )
		{
			if( (env = getenv("SCRIPT_NAME")) != NULL )
			{
				newLocation += env;
			}
		}
	}
	newLocation += location;

	return newLocation;
}

void HTTPserverResponse::flushData( std::ostream &out, const ArrayOfData &body )
{
	if( m_statusCode )
	{
		if( m_statusText.isEmpty() )
		{
			std::size_t	statusIndex = 0;

			for( std::size_t i=0; httpStatusCodes[i].statusCode > 0; i++ )
			{
				if( httpStatusCodes[i].statusCode == m_statusCode )
				{
					statusIndex = i;
					break;
				}
				else if( httpStatusCodes[i].statusCode == 500 )
					statusIndex = i;
			}

			m_statusText = httpStatusCodes[statusIndex].statusText;
		}
		sendStatusCode( out );
	}

	out << "Connection: close" << m_endln;
	out << "Server: GAKLIB Web Server Toolkit (" __DATE__ "-" __TIME__ ")" << m_endln;
	out << "Date: " << DateTime().getLocalTime() << m_endln;

	if( !m_error.isEmpty() )
	{
		html::HTML		theErrorHtml( m_error );
		html::HTML_TEXT	*theText = new html::HTML_TEXT( m_error );
		theText->setSize( 7 );
		theErrorHtml.getBody()->addObject( theText );
		STRING value = theErrorHtml.generateDoc();

		out << "Pragma: no-cache" << m_endln
			<< "Cache-Control: no-cache" << m_endln
			<< "Content-Type: text/html" << m_endln
			<< m_endln
			<< value;
	}
	else
	{
		for( 
			FieldSet::const_iterator it = m_cookies.cbegin(),
				endIT = m_cookies.cend();
			it != endIT;
			++it
		)
		{
			const Named_Field	&cookie = *it;
			STRING value = webEscape(STRING(cookie.getValue()));
			out << "Set-Cookie: " << cookie.getKey() << '=' << value << m_endln;
		}

		STRING	cacheControl;

		if( m_noCacheFlag )
		{
			cacheControl = "no-cache";
			out << "Pragma: no-cache" << m_endln;
		}
		if( m_maxAge >= 0 )
		{
			if( !cacheControl.isEmpty() )
			{
				cacheControl += "; ";
			}
			cacheControl += "max-age=";

			cacheControl += formatNumber( m_maxAge );
		}
		if( !cacheControl.isEmpty() )
		{
			out << "Cache-Control: " << cacheControl << m_endln;
		}

		if( !m_expires.isEmpty() )
		{
			out << "Expires: " << m_expires << m_endln;
		}

		if( !m_location.isEmpty() )
		{
			out << "Location: " << m_location << m_endln
				<< "Content-Type: text/plain" << m_endln
				<< "Content-Length: "
				<< (m_location.strlen()+strlen(m_endln)) << m_endln
				<< m_endln
				<< m_location << m_endln;
		}
		else
		{
			if( !m_lastModified.isEmpty() )
			{
				out << "Last-Modified: " << m_lastModified << m_endln;
			}
			if( !m_eTag.isEmpty() )
			{
				out << "ETag: " << m_eTag << m_endln;
			}

			out << "Content-Type: "
				<< (m_contentType.isEmpty()
					? "text/plain" : static_cast<const char *>(m_contentType))
			;
			if( !m_charset.isEmpty() )
			{
				out << "; charset=\"" << m_charset << '\"';
			}
			out << m_endln;

			if( body.size() )
			{
				out << "Content-Length: " << body.size() << m_endln
					<< m_endln;

				out.write( body.getDataBuffer(), body.size() );
			}
		}
	}
	out.flush();
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

