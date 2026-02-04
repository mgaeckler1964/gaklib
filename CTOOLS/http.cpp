/*
		Project:		GAKLIB
		Module:			http.cpp
		Description:	Base classes used for an HTTP Client
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#include <time.h>

#include <gak/http.h>
#include <gak/htmlParser.h>
#include <gak/t_string.h>
#include <gak/numericString.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

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

STRING			HTTPrequest::s_proxy;
unsigned short	HTTPrequest::s_proxyPort;
ArrayOfStrings	HTTPrequest::s_noProxy;
STRING			HTTPrequest::s_keyfile, HTTPrequest::s_password;

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

STRING HTTPrequest::makeFullPath( const STRING &baseUrl, STRING relative )
{
	size_t	slashPos, slashPosNew, slashPosBase, colonPosNew, colonPosBase, paramPos;
	STRING	newUrl, theBaseUrl = baseUrl;

	paramPos = theBaseUrl.searchChar( '?' );
	if( paramPos != theBaseUrl.no_index )
	{
		theBaseUrl.cut( paramPos );
	}

	if( isValidProtocol( relative ) )
	{
		newUrl = relative;
		colonPosNew = newUrl.searchChar( ':' );
		colonPosNew = newUrl.searchChar( ':', colonPosNew+1 );
		slashPosNew = newUrl.searchChar( '/' );
		slashPosNew = newUrl.searchChar( '/', slashPosNew+1 );
		slashPosNew = newUrl.searchChar( '/', slashPosNew+1 );

		if( slashPosNew > 8
		&& (colonPosNew == newUrl.no_index || colonPosNew > slashPosNew) )
		{
			colonPosBase = theBaseUrl.searchChar( ':' );
			colonPosBase = theBaseUrl.searchChar( ':', colonPosBase+1 );
			slashPosBase = theBaseUrl.searchChar( '/' );
			slashPosBase = theBaseUrl.searchChar( '/', slashPosBase+1 );
			slashPosBase = theBaseUrl.searchChar( '/', slashPosBase+1 );

			if( colonPosBase != theBaseUrl.no_index && colonPosBase < slashPosBase )
			{
				// we have a port number in base url but not in new url
				STRING portString = theBaseUrl.subString( colonPosBase, slashPosBase-colonPosBase );
				newUrl.insStr( slashPosNew, portString );
			}
		}
	}
	else
	{
		STRING oldUrl = theBaseUrl;
		newUrl = "http://";

		// remove protocol
		if( isHttpProtocol( oldUrl ) )
		{
			oldUrl += size_t(7);
		}
		else if( isHttpsProtocol( oldUrl ) )
		{
			oldUrl += size_t(8);
			newUrl = "https://";
		}

		slashPos = oldUrl.searchChar( '/' );
		if( slashPos != oldUrl.no_index )
		{
			newUrl += oldUrl.leftString( slashPos );
			oldUrl += slashPos;
		}
		else
		{
			newUrl += oldUrl;
			oldUrl = "/";
		}

		if( relative[0U] == '/' )
		{
			newUrl += relative;
		}
		else
		{
			// remove document name
			slashPos = oldUrl.searchRChar( '/' );
			if( slashPos != oldUrl.no_index )
			{
				oldUrl.cut(slashPos+1);
			}

			while( relative[0U] == '.' && oldUrl[0U] )
			{
				if( relative.leftString( 2 ) == "./" )
				{
					relative += size_t(2U);
				}
				else if( relative.leftString( 3 ) == "../" )
				{
					relative += size_t(3U);

					// remove directory name from path
					oldUrl.cut( oldUrl.strlen()-1 );
					slashPos = oldUrl.searchRChar( '/' );
					if( slashPos != oldUrl.no_index )
					{
						oldUrl.cut(slashPos+1);
					}
				}
				else
				{
					break;
				}
			}

			// add the directory
			newUrl += oldUrl;

			// add the source
			newUrl += relative;
		}
	}
	return newUrl;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void HTTPclientResponse::parseCacheControl( const STRING &iCacheControl )
{
	T_STRING	cacheControl = iCacheControl;
	T_STRING	theControl = cacheControl.getFirstToken( ";," );
	while( !theControl.isEmpty() )
	{
		STRING name = theControl.getFirstToken( "=" );
		STRING value = theControl.getNextToken();

		if( !strcmpi( name, "max-age" ) )
		{
			m_loadTime = long(time( NULL ));
			long maxAge = value.getValueE<long>();
			setMaxAge( maxAge );
		}
		else if( !strcmpi( name, "no-cache" ) )
			setNoCache();

		theControl = cacheControl.getNextToken();
	}
}

size_t HTTPrequest::MakeRequest(
	const char *method, const char *url,
	int bufferSize,
	const char *contentType, const char *data, size_t numData
)
{
	doEnterFunction("HTTPrequest::MakeRequest");

	char			c;
	STRING			serverName, proxyServer = HTTPrequest::s_proxy;
	STRING			path;
	unsigned short	port = 80;
	bool			useSSL = false;
	const char		*protocol = "http";
	unsigned short	defaultPort = 80;
	bool			includeBody = true;

	if( !strcmpi( method, "head" ) )
	{
		includeBody = false;
	}

	m_lastUrl = url;

	// remove protocl
	if( isHttpProtocol( url ) )
	{
		url += 7;
	}
	else if( isHttpsProtocol( url ) )
	{
		url += 8;
		useSSL = true;
		defaultPort = port = 443;
		protocol = "https";
	}

	// extract the server name
	while( (c = *url) != 0 )
	{
		if( c == ':' || c== '/' )		// port found
		{
			break;
		}
		serverName += c;
		url++;
	}
	// check for port
	if( c == ':' )
	{
		url++;
		port = getValue<unsigned short>( url, 10, &url );
		while( (c = *url) != 0 )
		{
			if( c == '/' )
			{
				break;
			}
			url++;
		}
	}
	serverName.lowerCase();

	// preserve server:port/path
	STRING host = serverName;
	int hostPort = port;
	if( hostPort != defaultPort )
	{
		host += ':';
		host += formatNumber( hostPort );
	}
	STRING hostPath = path = *url ? url : "/";

	if( !useSSL && !proxyServer.isEmpty() && strcmpi( host, "localhost" ) )
	{
		for( 
			ArrayOfStrings::const_iterator it = s_noProxy.cbegin(), endIT = s_noProxy.cend();
			it != endIT;
			++it
		)
		{
			if( serverName.match( *it ) )
			{
				proxyServer = NULL_STRING;
				break;
			}
		}

		if( !proxyServer.isEmpty() )
		{
			// prepare fully qualified url for proxy
			path = protocol;
			path += "://";
			path += host;
			path += hostPath;

			// overwrite server and port with proxy information
			serverName = proxyServer;
			port = s_proxyPort;
		}
	}

	if( m_socketStream )
	{
		delete m_socketStream;
	}
#if USE_SSL
	if( useSSL )
	{
		m_socketStream = new SSLsocketStreambuf( proxyServer, s_proxyPort, s_keyfile, s_password );
	}
	else
#endif
	{
		m_socketStream = new SocketStreambuf;
	}

	HTTPclientResponse	&theResponse = m_responseCache[m_lastUrl];

	if( !m_socketStream->connect( serverName, port, bufferSize ) )
	{

		STRING request = method;
		request += ' ';
		request += path;
		request += " HTTP/1.0\r\n";
/*
		do not use HTTP 1.1 until transfer encoding is implemented
		request += " HTTP/1.1\r\n";
*/
		if( m_cookies.getNumFields() )
		{
			request += "Cookie: ";
			request += getCookies();
			request += "\r\n";
		}
		request += "host: ";
		request += host;
		request += "\r\n";

		if( !m_referer.isEmpty() )
		{
			request += "Referer: ";
			request += m_referer;
			request += "\r\n";
		}

		request += "User-Agent: ";
		if( !m_userAgent.isEmpty() )
			request += m_userAgent;
		else
			request += "GAKLIB HTTP Client";

		request += "\r\n";

		if( !m_httpUsername.isEmpty() && !m_httpPassword.isEmpty() )
		{
			request += "Authorization: Basic ";
			STRING	credentials = m_httpUsername + ':' + m_httpPassword;

			request += encodeBase64( credentials );
			request += "\r\n";
		}
		if( contentType )
		{
			request += "Content-Type: ";
			request += contentType;
			request += "\r\n";
		}
		if( numData && data )
		{
			request += "Content-Length: ";
			request += formatNumber( numData );
			request += "\r\n";
		}
		if( m_extraHeader[0U] )
		{
			request += m_extraHeader;
			request += "\r\n";
			m_extraHeader.release();
		}

		request += "Connection: close\r\n\r\n";

		if( !m_socketStream->sendData( request, request.strlen() ) )
		{
			if( numData && data )
			{
				m_socketStream->sendData( data, numData );
			}

			theResponse.readHttpResponse( m_socketStream, includeBody, bufferSize );
			theResponse.incrFetchCount();
		}

		if( theResponse.getResponseSize() )
			addCookies(theResponse.getCookies());
		else
			theResponse.setStatusText( (STRING)"socket: " + m_socketStream->getSocketError() );

		m_socketStream->disconnect();
	}
	else
	{
		theResponse.setStatusText( (STRING)"socket: " + m_socketStream->getSocketError() );
	}

	theResponse.setConnectTime( m_socketStream->getConnectTime() );
	theResponse.setSendTime( m_socketStream->getSendTime() );
	theResponse.setReceiveTime( m_socketStream->getReceiveTime() );
	theResponse.setTotalTime( m_socketStream->getTotalTime() );
	theResponse.setReferer( m_referer );

	return theResponse.getResponseSize();
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void HTTPclientResponse::readHttpHeader( SocketStreambuf *theSocket )
{
	doEnterFunction("HTTPclientResponse::readHttpHeader");

	init();

	// extract the Status Code
	STRING	headerLine = theSocket->getNextLine();

	if( !headerLine[0U] )
		headerLine = theSocket->getNextLine();

	m_header = headerLine;
	m_header += "\r\n";

	int statusCode = 0;
	STRING statusText = "Invalid Response Header";

	size_t	spacePos = headerLine.searchChar( ' ' );
	// skip protocol version
	if( spacePos != headerLine.no_index )
	{
		headerLine += spacePos + 1;

		spacePos = headerLine.searchChar( ' ' );
		if( spacePos != headerLine.no_index )
		{
			STRING	httpStatus = headerLine.leftString( spacePos );
			statusCode = httpStatus.getValueE<unsigned>();
			headerLine += spacePos + 1;
			statusText = headerLine;
		}
	}

	setStatusCode( statusCode );
	setStatusText( statusText );

	// extract other Headers
	while( 1 )
	{
		headerLine = theSocket->getNextLine();
		if( !headerLine[0U] )
			break;				// finished the http header

		m_header += headerLine;
		m_header += "\r\n";


		size_t	colonPos = headerLine.searchChar( ':' );
		if( colonPos != (size_t)-1L )
		{
			STRING	variable = headerLine.leftString(
				colonPos
			);
			T_STRING	value = headerLine.rightString(
				strlen(headerLine) - colonPos -1
			);
			value.stripBlanks();

			if( !strcmpi( variable, "location" ) )
				setLocation( value );
			else if( !strcmpi( variable, "Set-Cookie" ) )
			{
				T_STRING newCookie = value.getFirstToken( ";" );

				STRING	name = newCookie.getFirstToken( "=" );
				STRING	value = newCookie.getNextToken();
				setCookie( name, value );
			}
			else if( !strcmpi( variable, "Content-Type" ) )
				setContentType( value );
			else if( !strcmpi( variable, "Expires" ) )
				setExpiration( value );
			else if( !strcmpi( variable, "Cache-Control" ) )
				parseCacheControl( value );
			else if( !strcmpi( variable, "Content-Length" ) )
				m_contentLength = value.getValueE<size_t>();
			else if( !strcmpi( variable, "Last-Modified" ) )
				setLastModified( value );
			else if( !strcmpi( variable, "ETag" ) )
				setETag( value );
			else if( !strcmpi( variable, "WWW-Authenticate" ) )
			{
				STRING	authScheme, realm;
				char	c;
				size_t	i;
				for( i=0; i<value.strlen(); i++ )
				{
					c = value[i];
					if( isspace( c ) )
						break;
				}
				value += i;
				value.stripBlanks();

				if( strcmpi( authScheme, "Basic" ) )
				{
					CI_STRING	name = value.getFirstToken( "=" );
					if( name == "realm" )
						realm = value.getNextToken();
					if( !realm.isEmpty() )
						setAuthScheme( HTTP_BASIC_AUTH, realm );
				}
			}
		}
	}
}

void HTTPclientResponse::readHttpResponse( SocketStreambuf *theSocket, bool includeBody, size_t bufferSize )
{
	doEnterFunction("HTTPclientResponse::readHttpResponse");

	readHttpHeader( theSocket );

	if( includeBody )
	{
		ArrayOfData &body = getBodyArray();

		body.clear();
		body.setChunkSize( bufferSize );
		int		c;

		if( m_contentLength )
		{
			size_t	i=0;

			body.getOrCreateElementAt( m_contentLength+1 );
			while( (c=theSocket->getNextByte()) >= 0 && i < m_contentLength )
			{
				body[i++] = char(c);
			}

			body[i] = 0;		// add an extra 0 byte for convenience
			m_contentLength = i;	// if we did not get all bytes
		}
		else
		{
			// read until end of stream

			while( (c=theSocket->getNextByte()) >= 0 )
			{
				body.createElement() = (char)c;
			}

			m_contentLength = body.size();
			body.createElement() = 0; // add an extra 0 byte for convenience
		}
	}

	m_responseSize = strlen( m_header ) + m_contentLength + 2;
}

bool HTTPclientResponse::isExpired() const
{
	if( getNoCacheFlag() )
	{
/***/	return true;
	}

	long maxAge = getMaxAge();

	if( m_loadTime && maxAge >= 0 )
	{
		long maxTime = m_loadTime + maxAge;
		if( maxTime < long(time( NULL )) )
		{
/***/		return true;
		}
		else
		{
/***/		return false;
		}
	}

	T_STRING expires = getExpiration();
	if( expires.isEmpty() )
	{
/***/	return true;
	}

	int		hour, minute, second, day, month, year;

	STRING	weekday = expires.getFirstToken( " " );
	STRING	monthDay = expires.getNextToken();
	STRING	monthStr = expires.getNextToken();
	STRING	yearStr = expires.getNextToken();
	STRING	timeStr = expires.getNextToken();
	STRING	gmtStr = expires.getNextToken();

	day = monthDay.getValueE<unsigned>();
	month = tokenSearch( monthStr, "Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Noc\0Dec\0" );
	year = yearStr.getValueE<unsigned>();

	sscanf( timeStr, "%d:%d:%d", &hour, &minute, &second );

	time_t	now = time( NULL );
	struct	tm *gmt = gmtime( &now );

	gmt->tm_year += 1900;
	if( year < gmt->tm_year )
/***/	return true;
	if( year > gmt->tm_year )
/***/	return false;

	if( month < gmt->tm_mon )
/***/	return true;
	if( month > gmt->tm_mon )
/***/	return false;

	if( day < gmt->tm_mday )
/***/	return true;
	if( day > gmt->tm_mday )
/***/	return false;

	if( hour < gmt->tm_hour )
/***/	return true;
	if( hour > gmt->tm_hour )
/***/	return false;

	if( minute < gmt->tm_min )
/***/	return true;
	if( minute > gmt->tm_min )
/***/	return false;

	if( second < gmt->tm_sec )
/***/	return true;
	if( second > gmt->tm_sec )
/***/	return false;

	return true;
}


void HTTPrequest::addCookies( const FieldSet &cookies )
{
	STRING	name, value;

	for( 
		FieldSet::const_iterator it = cookies.cbegin(), endIT = cookies.cend();
		it != endIT;
		++it
	)
	{
		const Named_Field	&cookie = *it;
		name = cookie.getKey();
		value = cookie.getValue();

		if( !name.isEmpty() && !value.isEmpty() )
		{
			m_cookies.updateField( name, value );
		}
	}
}

void HTTPrequest::addCookies( const STRING &cookies )
{
	T_STRING	newCookies = cookies;

	T_STRING	theCookie = newCookies.getFirstToken( ";" );
	while( !theCookie.isEmpty() )
	{
		STRING name = theCookie.getFirstToken( "=" );
		STRING value = theCookie.getNextToken();

		if( !name.isEmpty() && !value.isEmpty() )
		{
			m_cookies.updateField( name, value );
		}

		theCookie = newCookies.getNextToken();
	}
}

STRING HTTPrequest::getCookies()
{
	STRING	cookieString;
	size_t	numCookies = m_cookies.getNumFields();

	for( size_t i=0; i<numCookies; i++ )
	{
		cookieString += m_cookies[i].getKey();
		cookieString += '=';
		cookieString += STRING(m_cookies[i].getValue());
		cookieString += "; ";
	}

	return cookieString;
}

xml::Document *HTTPrequest::getXmlDocument( bool ignoreMimeType )
{
	doEnterFunction("HTTPrequest::getXmlDocument");

	clock_t				parseTime = clock();
	xml::Document		*theDoc = NULL;
	HTTPclientResponse	&theResponse = m_responseCache[m_lastUrl];
	STRING				contentType = theResponse.getContentType().leftString( 8 ).lowerCase();


	if( theResponse.getBodySize() && (ignoreMimeType || contentType == "text/xml") )
	{
		iBinaryStream	theInputStream( theResponse.getBodyArray() );
		xml::Parser		theXmlParser( &theInputStream, m_lastUrl );

		theDoc = theXmlParser.readFile( false );
		theResponse.setHtmlParserErrors( theXmlParser.getErrors() );
	}
	else
		theResponse.setHtmlParserErrors( "" );

	theResponse.incrParseTime( clock() - parseTime );

	return theDoc;
}

html::Document *HTTPrequest::getHtmlDocument()
{
	clock_t			  	parseTime = clock();
	html::Document		*theDoc = NULL;
	HTTPclientResponse	&theResponse = m_responseCache[m_lastUrl];
	STRING			  	contentType = theResponse.getContentType().leftString( 9 ).lowerCase();

	if( theResponse.getBodySize() && contentType == "text/html" )
	{
		iBinaryStream	theInputStream( theResponse.getBodyArray() );
		html::Parser	theHtmlParser( &theInputStream, m_lastUrl );

		theDoc = theHtmlParser.readFile( false );
		theResponse.setHtmlParserErrors( theHtmlParser.getErrors() );
	}
	else
		theResponse.setHtmlParserErrors( "" );

	theResponse.incrParseTime( clock() - parseTime );

	return theDoc;
}

void HTTPrequest::clearCache( bool expiredOnly )
{
	if( !expiredOnly )
	{
		m_responseCache.clear();
	}
	else
	{
		size_t	i=0;
		while( i<m_responseCache.size() )
		{
			if( m_responseCache.getElementAt(i).isExpired() )
			{
				m_responseCache.removeElementAt( i );
			}
			else
			{
				i++;
			}
		}
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING webUnEscape( const char *cp, bool savePlus )
{
	STRING	result;

	if( !cp )
	{
		return result;
	}

	char c;
	while( (c=*cp++) != 0 )
	{
		if( !savePlus && c == '+' )
		{
			result += ' ';
		}
		else if( c == '%' )
		{
			char	temp[3];

			temp[0] = *cp++;
			temp[1] = temp[0] ? *cp++ : 0; 
			temp[2] = 0; 

			result += char(getValueE<unsigned char>(temp, 16 ));
		}
		else
		{
			result += c;
		}
	}

	return result;
}

STRING webEscape( const char *cp )
{
	STRING			result;
	char			hexCodes[]="0123456789ABCDEF";
	unsigned char	c;

	if( !cp )
	{
		return result;
	}

	while( (c = static_cast<unsigned char>(*cp++)) != 0 )
	{
		// we should escape blanks, too for the web server
		if( c < 33 || c=='%' || c=='+' || c>127 )
		{
			result += '%';

			result += hexCodes[c/16];
			result += hexCodes[c%16];
		}
		else
		{
			result += char(c);
		}
	}

	return result;
}

}	// namespace net
}	// namespace gak
