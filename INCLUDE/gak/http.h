/*
		Project:		GAKLIB
		Module:			http.h
		Description:	Base classes used for an HTTP Client
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

#ifndef HTTP_H
#define HTTP_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/sslSocket.h>
#include <gak/fieldSet.h>
#include <gak/xml.h>
#include <gak/html.h>
#include <gak/httpResponse.h>
#include <gak/exception.h>

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
namespace net
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class HTTPerror : public LibraryException
{
	protected:
	HTTPerror( const STRING &errText, const STRING &url ) : LibraryException( errText )
	{
		if( !url.isEmpty() )
			addErrorText( url );
	}
};

class NoConnectionError : public HTTPerror
{
	public:
	NoConnectionError( const STRING &url ) : HTTPerror( "No Connection To Server", url ) 
	{
	}
};

class NoAuthorisationError : public HTTPerror
{
	public:
	NoAuthorisationError( const STRING &url ) : HTTPerror( "No Authorisation", url )
	{
	}
};

class HTTPstatusError : public HTTPerror
{
	public:
	HTTPstatusError( const STRING &url ) : HTTPerror( "HTTP Failure", url )
	{
	}
};

/// this is a simple HTTP client
class HTTPrequest
{
	SocketStreambuf	*socketStream;

	// input data
	STRING		extraHeader;
	FieldSet	cookies;
	STRING		referer, userAgent;

	STRING		httpUsername, httpPassword;

	static STRING			s_proxy;
	static unsigned short	s_proxyPort;
	static ArrayOfStrings	s_noProxy;
	static STRING			s_keyfile, s_password;

	// response
	STRING								lastUrl;
	UnorderedMap<HTTPclientResponse>	responseCache;

	private:
	size_t MakeRequest(
		const char *method, const char *url,
		int buffsize=10240,
		const char *contentType=NULL,
		const char *data=NULL, size_t numData=0
	);

	protected:
	/**
		@brief adds a new entry to the request header
		@param [in] headerName the name of the header field
		@param [in] headerValue the value
	*/
	void setExtraHeader( const char *headerName, const char *headerValue )
	{
		if( !extraHeader.isEmpty() )
		{
			extraHeader += "\r\n";
		}
		extraHeader += headerName;
		extraHeader += ": ";
		extraHeader += headerValue;
	}

	public:
	/**
		@brief returns a full URL for a given relative URL
		@param [in] baseUrl the complete URL for the source document
		@param [in] relative the relative link in the source document
		@return the full URL of the target document
	*/
	static STRING makeFullPath( const STRING &baseUrl, STRING relative );

	/// returns true if the given URL uses HTTP protocol
	static bool isHttpProtocol( const char *url )
	{
		return !strncmpi( url, "http://", 7 );
	}
	/// returns true if the given URL uses HTTPS protocol
	static bool isHttpsProtocol( const char *url )
	{
		return !strncmpi( url, "https://", 8 );
	}
	/// returns true if the given URL uses a supported protocol (HTTP or HTTPs)
	static bool isValidProtocol( const char *url )
	{
		return isHttpProtocol( url ) || isHttpsProtocol( url );
	}

	/// creates a new HTTP request object
	HTTPrequest()
	{
		socketStream = NULL;
	}
	/// destroys the client and the connectioon
	~HTTPrequest()
	{
		if( socketStream )
		{
			delete socketStream;
		}
	}
	/**
		@brief configures the global HTTP proxy settings
		@param [in] proxy the host name of the proxy server to use
		@param [in] proxyPort the IP port if the proxy server
		@param [in] noProxy a comma separated list of servers that should bypass the proxy
	*/
	static void setProxy(  const STRING &proxy, unsigned short proxyPort, const STRING &noProxy=NULL_STRING )
	{
		HTTPrequest::s_proxy = proxy;
		HTTPrequest::s_proxyPort = proxyPort;
		HTTPrequest::s_noProxy.createElements( noProxy.lowerCaseCopy(), ";,", true ); 
	}
	/**
		@brief configures the global SSL key file and password
		@param [in] keyfile the path to the SSL keys
		@param [in] password the password
	*/
	static void setSSLkeyfile( const STRING &keyfile, const STRING &password )
	{
		HTTPrequest::s_keyfile = keyfile;
		HTTPrequest::s_password = password;
	}

	/**
		@brief add cookies to the list of cookies
		@param [in] cookies a list of all cookies in the form \<name\>=\<value\> [;\<name\>=\<value\>] ...
	*/
	void addCookies( const STRING &cookies );
	/**
		@brief replaces all cookies
		@param [in] cookies a list of all cookies in the form \<name\>=\<value\> [;\<name\>=\<value\>] ...
	*/
	void setCookies( const STRING &cookies )
	{
		this->cookies.clear();

		addCookies( cookies );
	}
	/**
		@brief add cookies to the list of cookies
		@param [in] cookies a list of all cookies
	*/
	void addCookies( const FieldSet &cookies );
	/**
		@brief replaces all cookies
		@param [in] cookies a list of all cookies
	*/
	void setCookies( const FieldSet &cookies )
	{
		this->cookies.clear();

		addCookies( cookies );
	}
	///	returns all cookies in the form \<name\>=\<value\> [;\<name\>=\<value\>] ...
	STRING getCookies( void );

	/// sets the referer field
	void setReferer( const STRING &referer )
	{
		this->referer = referer;
	}
	/// returns the referer field
	const STRING &getReferer( void ) const
	{
		return referer;
	}
	/// sets the user agent field
	void setUserAgent( const STRING &userAgent )
	{
		this->userAgent = userAgent;
	}
	/// returns the user agent field
	const STRING &getUserAgent( void ) const
	{
		return userAgent;
	}
	/**
		@brief sets user name and password for basic authentication
		@param [in] username the name of the user
		@param [in] password the password
	*/
	void setCredentials( const STRING &username, const STRING &password )
	{
		httpUsername = username;
		httpPassword = password;
	}
	/**
		@brief establishes a connection to the server and performs a GET request
		@param [in] url the URL of the document to load
		@param [in] buffersize the size of the I/O buffer to use
		@return the number of bytes read
	*/
	size_t Get( const char *url, int buffersize=10240  )
	{
		return MakeRequest( "GET", url, buffersize );
	}
	/**
		@brief establishes a connection to the server and performs a POST request
		@param [in] url the URL of the document to load
		@param [in] contentType the type of the content
		@param [in] data the data to send in the body of the request
		@param [in] numData the number of bytes to send in the body of the request
		@param [in] buffersize the size of the I/O buffer to use
		@return the number of bytes read
	*/
	size_t Post(
		const char *url, const char *contentType,
		const char *data, size_t numData,
		int buffersize=10240
	)
	{
		return MakeRequest( "POST", url, buffersize, contentType, data, numData );
	}
	/**
		@brief establishes a connection to the server and performs a HEAD request
		@param [in] url the URL of the document to load
		@param [in] buffersize the size of the I/O buffer to use
		@return the number of bytes read
	*/
	size_t Head( const char *url, int buffersize=10240 )
	{
		return MakeRequest( "HEAD", url, buffersize );
	}
	/// returns the header field of the last request
	STRING	getHeader( void ) const
	{
		return responseCache[lastUrl].getHeader();
	}
	/// returns the content type of the last request
	STRING	getContentType( void ) const
	{
		return responseCache[lastUrl].getContentType();
	}

	/// returns the status code of the last request
	int getHttpStatusCode( void ) const
	{
		return responseCache[lastUrl].getStatusCode();
	}
	/// returns the status text of the last request
	STRING getHttpStatusText( void ) const
	{
		return responseCache[lastUrl].getStatusText();
	}
	/// returns the redirect URL of the last request
	STRING getLocation( void ) const
	{
		return responseCache[lastUrl].getLocation();
	}
	/// returns the content of the last request (use for text data)
	const char *getBody( void ) const
	{
		return responseCache[lastUrl].getBody();
	}
	/// returns the xml/html parser errors of the last request
	STRING getHtmlErrors( void ) const
	{
		return responseCache[lastUrl].getHtmlParserErrors();
	}

	/**
		@brief parses the content of the last request
		@param [in] ignoreMimeType if true the content type of the last request is not checked
		@return an XML document created from the content NULL if there is an error
	*/
	xml::Document *getXmlDocument( bool ignoreMimeType );
	/**
		@brief parses the content of the last request
		@return an HTML document created from the content NULL if there is an error
	*/
	html::Document	*getHtmlDocument( void );

	/// returns the number of elements on the response cache
	size_t getNumGets( void ) const
	{
		return responseCache.size();
	}
	/// returns an entry from the response cache by its index
	const HTTPclientResponse &getHttpResponse( size_t i ) const
	{
		return responseCache.getElementAt( i );
	}
	/// returns an entry from the response cache by its url
	const HTTPclientResponse &getHttpResponse( const STRING &url ) const
	{
		return responseCache[url];
	}
	/// returns the response of the last request
	const HTTPclientResponse &getHttpResponse( void ) const
	{
		return responseCache[lastUrl];
	}
	/// clears the entire response cache
	void clearCache( bool expiredOnly );

	/// returns the last socket error
	STRING	getSocketError( void ) const
	{
		if( socketStream )
			return socketStream->getSocketError();
		else
			return "No socket";
	}
	protected:
	/// sets the last URL
	void setLastUrl( const STRING &url )
	{
		lastUrl = url;
	}
};


// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
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

/**
	@brief transforms an escaped string to the ANSI encoding
	
	all characters in the form "%xx" will be transormed to their byte representation
	+ characters will be replaced by a space character

	@param [in] cp the escaped ASCII string
	@param [in] savePlus if true plus characters won't be transformed
	@return the unescaped ANSI string
*/
STRING webUnEscape( const char *cp, bool savePlus=false );
/**
	@brief transforms an ANSI string to the escaped encoding
	
	all control characters and non ASCII characters will be transformed to the form "%xx" 
	space characters will be replaced by a plus character

	@param [in] cp the ANSI string
	@return the escaped ASCII string
*/
STRING webEscape( const char *cp );

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

#	pragma warn +inl
#endif

#endif
