/*
		Project:		GAKLIB
		Module:			httpResponse.h
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
#ifndef GAK_HTTP_RESPONSE_H
#define GAK_HTTP_RESPONSE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <time.h>
#include <iostream>

#include <cstdarg>

#include <gak/string.h>
#include <gak/fieldSet.h>
#include <gak/html.h>
#include <gak/socketbuf.h>
#include <gak/datetime.h>

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

/// describes the possible authorization schemes
enum HTTPauthScheme
{
	/// no authorization is required
	HTTP_NO_AUTH, 
	/// basic authorization required
	HTTP_BASIC_AUTH
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief base class for all HTTP response data sent by a server or received by a client

	This class contains common data from all HTTP responses
*/
class HTTPresponse
{
	/// @cond
	protected:

	FieldSet			m_cookies;
	STRING				m_error;
	STRING				m_location;

	bool				m_noCacheFlag;
	long				m_maxAge;
	STRING				m_expires;

	int					m_statusCode;

	STRING				m_statusText,
						m_contentType,
						m_lastModified,
						m_eTag,
						m_charset;

	const char			*m_endln;
	ArrayOfData			m_body;

	HTTPauthScheme		m_authScheme;
	STRING				m_realm;

	protected:
	HTTPresponse()
	{
		init();
	}
	void init( void )
	{
		m_noCacheFlag = false;
		m_statusCode = 0;
		m_endln = "\r\n";
		m_maxAge = -1;

		m_location = "";
		m_contentType = "";
		m_charset = "";
		m_expires = "";
		m_lastModified = "";
		m_eTag = "";
		m_body.clear();
		m_authScheme = HTTP_NO_AUTH;
	}
	/// @endcond
	public:
	/// sets the flag that the client should not cache the response
	void setNoCache( void )
	{
		m_noCacheFlag = true;
	}
	/// clears the flag that the client should not cache the response
	void clrNoCache( void )
	{
		m_noCacheFlag = false;
	}
	/// returns true if the client must not cache the response
	bool getNoCacheFlag( void ) const
	{
		return m_noCacheFlag;
	}
	/// sets the max age of this response
	void setMaxAge( long maxAge )
	{
		m_maxAge = maxAge;
	}
	/// returns the max age of this document
	long getMaxAge( void ) const
	{
		return m_maxAge;
	}
	/**
		@brief sets an error text of the response

		If the status code was not yet set it will be set to 500

		@param [in] error the error text
	*/
	void setError( const STRING &error )
	{
		if( !m_statusCode )
		{
			setStatusCode( 500 );
		}
		m_error = error;
	}

	/// returns true if there was an error set
	bool isError( void ) const
	{
		return m_error.isEmpty();
	}
	/**
		@brief adds or updates a cookie to/in the response
		@param [in] name the name of the cookie
		@param [in] value the value of the cookie
	*/
	void setCookie( const char *name, const char *value )
	{
		m_cookies.addField( name, value );
	}
	/// returns the set of all cookies
	FieldSet &getCookies( void )
	{
		return m_cookies;
	}
	/// returns the set of all cookies
	const FieldSet &getCookies( void ) const
	{
		return m_cookies;
	}

	/**
		@brief sets the authorization scheme for the recent request
		@param [in] authScheme the scheme see HTTPauthScheme
		@param [in] realm the realm string for this request
	*/
	void setAuthScheme( HTTPauthScheme authScheme, const STRING &realm )
	{
		m_authScheme = authScheme;
		m_realm = realm;
	}

	/**
		@brief sets the value of the expiration header field
		@param [in] expiration the value of the expiration header field
	*/
	void setExpiration( const STRING &expiration )
	{
		m_expires = expiration;
	}

	/// returns the value of the expiration header field
	const STRING &getExpiration( void ) const
	{
		return m_expires;
	}
	/**
		@brief sets the content type of the response
		@param [in] contentType the content type
	*/
	void setContentType( const STRING &contentType )
	{
		m_contentType = contentType;
	}
	/// returns the content type of this response
	const STRING &getContentType( void ) const
	{
		return m_contentType;
	}
	/**
		@brief sets the character set for this response
		@param [in] charset the character set
	*/
	void setCharset( const STRING &charset )
	{
		m_charset = charset;
	}
	/// returns the character set of this response
	const STRING &getCharset( void ) const
	{
		return m_charset;
	}

	/**
		@brief sets the last modified date of the response
		@param [in] lastModified the last modified date of the response
	*/
	void setLastModified( const STRING &lastModified )
	{
		m_lastModified = lastModified;
	}
	/**
		@brief sets the last modified date of the response
		@param [in] lastModified the last modified date of the response
	*/
	void setLastModified( const DateTime &lastModified )
	{
		setLastModified( lastModified.getLocalTime() );
	}
#if defined( __BORLANDC__ )
	void setLastModified( TDateTime lastModified )
	{
		setLastModified( DateTime( lastModified ) );
	}
#endif
	/// returns the data this response was last modified
	const STRING &getLastModified( void ) const
	{
		return m_lastModified;
	}
	/**
		@brief sets an ETag for this response
		@param [in] eTag the eTag
	*/
	void setETag( const STRING &eTag )
	{
		m_eTag = eTag;
	}
	/// returns the eTag of this response
	const STRING &getETag( void ) const
	{
		return m_eTag;
	}

	/**
		@brief sets the status code of the response
		@param [in] statusCode the status code
		@todo make this enum
	*/
	void setStatusCode( int statusCode )
	{
		m_statusCode = statusCode;
	}
	/// returns the status code of this response
	int getStatusCode( void ) const
	{
		return m_statusCode;
	}
	/**
		@brief sets the status text of the response
		@param [in] statusText the text that follows the status code
	*/
	void setStatusText( const STRING &statusText )
	{
		m_statusText = statusText;
	}
	/// returns the status text of this response
	const STRING &getStatusText( void ) const
	{
		return m_statusText;
	}

	/**
		@brief sets a redirect URL of the response
		@param [in] location the URL
	*/
	void setLocation( const STRING &location )
	{
		if( !m_statusCode )
		{
			setStatusCode( 301 );
		}
		m_location = location;
	}
	/// returns the redirection URL of this response
	const STRING &getLocation( void ) const
	{
		return m_location;
	}
	/// returns the response body for this response. Use this for text content, only
	const char *getBody( void ) const
	{
		return m_body.getDataBuffer();
	}

	/// returns the response body for this response. Use this for binary content
	const ArrayOfData &getBodyArray( void ) const
	{
		return m_body;
	}
	/// returns the response body for this response. Use this for binary content
	ArrayOfData &getBodyArray( void )
	{
		return m_body;
	}
	/// returns the number of bytes received for this response
	size_t getBodySize( void ) const
	{
		return m_body.size();
	}
};
/**
	@brief the response for all HTTP servers
	@see HTTPserverBase
*/
class HTTPserverResponse : public HTTPresponse
{
	private:
	virtual void sendStatusCode( std::ostream &out ) const;

	public:
	/**
		@brief sends all header fields and the body to the client
		@param [in] out the stream where to send the data
		@param [in] body the response body
	*/
	void flushData( std::ostream &out, const ArrayOfData &body );
	/**
		@brief sends all header fields and the body to the client
		@param [in] out the stream where to send the data
	*/
	void flushData( std::ostream &out )
	{
		flushData( out, m_body );
	}

	/**
		@brief adds a new line plus new line character to the resonse body
		@param [in] s the new line
	*/
	void puts( const char *s )
	{
		m_body += s;
		m_body += '\n';
	}
	/**
		@brief adds new text to the resonse body
		@param [in] fmt the printf format string
	*/
	void printf( const char *fmt, ... )
	{
		va_list	args;
		char	buffer[10240];

		va_start( args, fmt );
		vsprintf( buffer, fmt, args );
		va_end( args );
	}
	/**
		@brief sets the content body to the HTML text

		this function also sets the content type and the noCacheFlag

		@param [in] html the html element containig the data
	*/
	void setHtmlCode( html::HTML *html )
	{
		m_contentType = "text/html";

		if( m_noCacheFlag )
		{
			html->setNoCache();
		}

		m_body = html->generateDoc();
	}
	/**
		@brief copies binary data to the response body
		@param [in] body the binary data
	*/
	void setBody( const ArrayOfData &body )
	{
		m_body = body;
	}

};

/**
	@brief this class is used by CGI applications to build a server response
	@see CGIrequest
*/
class HTTPcgiResponse : public HTTPserverResponse
{
	using HTTPserverResponse::flushData;

	virtual void sendStatusCode( std::ostream &out ) const;

	STRING getLocation( const STRING &relativePath ) const;

	public:

	/**
		@brief sends binary data to the client via std::cout
		@param [in] body the response body
	*/
	void flushData( const ArrayOfData &body )
	{
		m_endln = "\n";
		flushData( std::cout, body );
		m_endln = "\r\n";
	}
	/// sends the response to te client via std::cout
	void flushData( void )
	{
		m_endln = "\n";
		flushData( std::cout, m_body );
		m_endln = "\r\n";
	}
	/**
		@brief creates a redirect response
		@param [in] location the redirect URL for the client may be ralative to the last request
	*/
	void setLocation( const STRING &location )
	{
		HTTPserverResponse::setLocation( getLocation( location ) );
	}
};

/**
	@brief this class stores the response received from a server
	@see HTTPrequest
*/
class HTTPclientResponse : public HTTPresponse
{
	public:
	/// this type is used for the map storing this response
	typedef CI_STRING key_type;

	private:
	int			m_fetchCount;
	size_t		m_contentLength,
				m_responseSize;

	clock_t		m_totalTime,
				m_connectTime, m_sendTime, m_receiveTime, m_parseTime;
	STRING		m_url,
				m_htmlParserErrors,
				m_header,
				m_referer;

	long		m_loadTime;

	void parseCacheControl( const STRING &cacheControl );

	public:
	/// creates an empty response object
	HTTPclientResponse()
	{
		m_totalTime = m_connectTime = m_sendTime = m_receiveTime = m_parseTime = 0;
		m_fetchCount = 0;
		m_contentLength = 0;

		m_loadTime = 0;
	}
	/// used by the map storing this response
	void setKey( const char *key )
	{
		m_url = key;
	}
	/// used by the map storing this response
	const char *getKey( void ) const
	{
		return m_url;
	}
	/// returns the URL that was used to load this response
	const STRING &getUrl( void ) const
	{
		return m_url;
	}
	/// initialies this response object
	void init( void )
	{
		m_contentLength = 0;

		m_htmlParserErrors = "";
		m_header = "";
		HTTPresponse::init();
	}
	/// updates the time passed to establish the connection
	void setConnectTime( clock_t connectTime )
	{
		m_connectTime = connectTime;
	}
	/// returns the time passed to establish the connection
	clock_t getConnectTime( void ) const
	{
		return m_connectTime;
	}
	/// updates the time passed to send the request to the server
	void setSendTime( clock_t sendTime )
	{
		m_sendTime = sendTime;
	}
	/// returns the time passed to send the request to the server
	clock_t getSendTime( void ) const
	{
		return m_sendTime;
	}
	/// updates the time passed to receive data from the server
	void setReceiveTime( clock_t receiveTime )
	{
		m_receiveTime = receiveTime;
	}
	/// returns the time passed to receive data from the server
	clock_t getReceiveTime( void ) const
	{
		return m_receiveTime;
	}
	/// updates the total time passed for this request
	void setTotalTime( clock_t totalTime )
	{
		m_totalTime = totalTime;
	}
	/// returns the total time passed for this request
	clock_t getTotalTime( void ) const
	{
		return m_totalTime;
	}
	/// increments the time passed to parse XML or HTML data
	void incrParseTime( clock_t value )
	{
		m_parseTime += value;
	}
	/// returns the time passed to parse XML or HTML data
	clock_t getParseTime( void ) const
	{
		return m_parseTime;
	}
	/// increments the fetch counter
	void incrFetchCount( void )
	{
		m_fetchCount++;
	}
	/// returns the fetch counter
	int getFetchCount( void ) const
	{
		return m_fetchCount;
	}

	/// sets the XML/HTML parser error
	void setHtmlParserErrors( const STRING &error )
	{
		m_htmlParserErrors = error;
	}
	/// returns the XML/HTML parser error
	const STRING &getHtmlParserErrors( void ) const
	{
		return m_htmlParserErrors;
	}

	/// returns the entire response header
	const STRING &getHeader( void ) const
	{
		return m_header;
	}
	/// returns the size of the entire response
	size_t getResponseSize( void ) const
	{
		return m_responseSize;
	}
	/// returns the content length
	size_t getContentLength( void ) const
	{
		return m_contentLength;
	}
	/// sets the referer header field
	void setReferer( const STRING &referer )
	{
		m_referer = referer;
	}
	/// returns the referer header field
	const STRING &getReferer( void ) const
	{
		return m_referer;
	}
	/// reads the HTTP header from a socket
	void readHttpHeader( SocketStreambuf *theSocket );
	/// reads the entire response
	void readHttpResponse( SocketStreambuf *theSocket, bool includeBody, size_t bufferSize );

	/// returns true if the data is expired
	bool isExpired( void ) const;
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
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif


#endif

