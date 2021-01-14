/*
		Project:		GAKLIB
		Module:			httpProfiler.cpp
		Description:
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

#include <gak/httpProfiler.h>
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

size_t HTTPprofiler::includeData(
	html::Document	*baseDoc,
	STRING			&baseUrl,
	xml::Element	*theElement,
	int				buffSize,
	bool			anchorTags,
	bool			noExternalLinks
)
{
	doEnterFunction( "HTTPprofiler::includeData" );

	size_t			resultLen = 0;
	STRING			tag, timer, subUrl, tmpUrl, newUrl;
	bool			isExternalLink;

	size_t			numData = theElement->getNumObjects();

	ArrayOfStrings	sources;

	for( size_t i=0; i<numData && getNumGets() < 1000; i++ )
	{
		xml::Element *subObject = theElement->getElement( i );
		resultLen += includeData(
			baseDoc, baseUrl, subObject, buffSize, anchorTags, noExternalLinks
		);

		sources.clear();
		subUrl = "";
		isExternalLink = false;
		tag = subObject->getTag();
		/* for our own control */
		if( tag == "get" )
		{
			subUrl = subObject->getAttribute( "url" );
		}
		else if( tag == "sleep" )
		{
			timer = subObject->getAttribute( "time" );
			int		miliSec = timer.getValueN<int>();
			Sleep( miliSec );
			script += timer;
		}
		/* for html files */
		else if( tag == "BASE" )
		{
			tmpUrl = subObject->getAttribute( "HREF" );
			if( tmpUrl[0U] )
				baseUrl = tmpUrl;
		}
		else if( tag == "IMG" || tag == "SCRIPT" || tag == "FRAME" || tag == "IFRAME" )
			subUrl = subObject->getAttribute( "SRC" );
		else if( tag == "LINK" )
			subUrl = subObject->getAttribute( "HREF" );
		else if( tag == "BODY" )
			subUrl = subObject->getAttribute( "BACKGROUND" );
		else if( tag == "APPLET" )
		{
			STRING	codeBase = subObject->getAttribute( "CODEBASE" );
			STRING	archive = subObject->getAttribute( "ARCHIVE" );
			STRING	javaClass = subObject->getAttribute( "CODE" );

			subUrl = codeBase;
			if( archive[0U] )
				subUrl += archive;
			else if( javaClass[0U] )
				subUrl += javaClass;
			else
				subUrl = "";
		}
		else if( anchorTags && (tag == "A" || tag == "AREA") )
		{
			subUrl = subObject->getAttribute( "HREF" );
		}
		else if( tag == "STYLES" && baseDoc )
		{
			STRING	styles = subObject->getValue( xml::PLAIN_MODE );
			baseDoc->readCssRules( styles, true );
		}

		if( !strncmpi( subUrl, "http://", 7 ) )
			isExternalLink = true;
		else if( !strncmpi( subUrl, "https://", 8 ) )
			isExternalLink = true;
		else if( !strncmpi( subUrl, "file:", 5 )
		|| !strncmpi( subUrl, "mailto:", 7 )
		|| !strncmpi( subUrl, "ftp:", 4 ) )
			subUrl = "";

		if( !subUrl.isEmpty() )
		{
			sources += subUrl;
		}

		if( baseDoc )
		{
			subUrl = baseDoc->getBackgroundImage( subObject, "screen" );
			if( subUrl.isEmpty() )
			{
				sources += subUrl;
			}
		}

		for( size_t j=0; j<sources.size(); j++ )
		{
			subUrl = sources[j];
			if( subUrl[0U] )
			{
				size_t numPos = subUrl.searchChar( '#' );
				if( numPos != subUrl.no_index )
					subUrl.cut( numPos );
			}
			if( subUrl[0U] )
			{
				while( true )
				{
					size_t	spacePos = subUrl.searchChar( ' ' );
					if( spacePos == -1 )
						break;
					subUrl = subUrl.leftString( spacePos ) + "%20" + subUrl.subString( spacePos+1 );
				}

				newUrl = makeFullPath( baseUrl, subUrl );

				if( !isExternalLink || !noExternalLinks )
				{
					const HTTPclientResponse	&theResponse = getHttpResponse(newUrl);
					if( !theResponse.getFetchCount() )
					{
						if( logFunc )
							logFunc( logData, newUrl, getNumGets() );

						if( isExternalLink )	// do not load external links completely
						{
							if( !noExternalLinks )
							{
								setReferer( baseUrl );
								resultLen += Get( newUrl, buffSize );
							}
						}
						else
						{
							setReferer( baseUrl );
							resultLen += GetTree(
								baseDoc, newUrl, buffSize,
								anchorTags, noExternalLinks
							);
						}
					}
				}
			}
		}
	}
	return resultLen;
}

size_t HTTPprofiler::GetTree(
	html::Document *baseDoc, const char *url, int buffsize,
	bool anchorTags, bool noExternalLinks
)
{
	doEnterFunction( "HTTPprofiler::GetTree" );

	size_t	resultLen = 0;
	STRING	location;

	STRING	baseUrl = url;
	while( 1 )
	{
		resultLen += Get( baseUrl, buffsize );
		if( resultLen )
		{
			location = getLocation();
			if( location[0U] )
				baseUrl = location;		// redirect -> follow
			else
				break;					// no redirect -> ready
		}
		else
			break;						// no result or no inlcudes
	}

	int httpStatus = getHttpStatusCode();

	if( httpStatus == 200 )
	{
		if( baseDoc && getContentType() == "text/css" )
		{
			STRING	styles = (char *)getBody();
			baseDoc->readCssRules( styles, true );
		}
		else
		{
			html::Document	*theHTMLDoc = getHtmlDocument();
			xml::Document	*theXMLDoc = theHTMLDoc;
			if( !theXMLDoc )
				theXMLDoc = getXmlDocument( false );

			if( theXMLDoc )
			{
				resultLen += includeData(
					theHTMLDoc, baseUrl, theXMLDoc, buffsize,
					anchorTags, noExternalLinks
				);
				if( theHTMLDoc )
					delete theHTMLDoc;
				else
					delete theXMLDoc;
			}
		}
	}
	return resultLen;
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

size_t HTTPprofiler::Get(
	const char *url, bool complete, bool anchorTags,
	bool noExternalLinks, bool clearExpiredOnly, int buffsize
)
{
	doEnterFunction( "HTTPprofiler::Get1" );

	clearCache( clearExpiredOnly );
	size_t	resultLen = complete
		? GetTree( NULL, url, buffsize, anchorTags, noExternalLinks )
		: Get( url, buffsize )
	;

	setLastUrl( url );

	return resultLen;
}

size_t HTTPprofiler::Get( const char *url, int buffsize )
{
	doEnterFunction( "HTTPprofiler::Get2" );

	STRING	theUrl = url;

	script += theUrl;

	return HTTPrequest::Get( url, buffsize );
}

size_t HTTPprofiler::playScript( bool clearExpiredOnly )
{
	doEnterFunction( "HTTPprofiler::playScript" );

	size_t	resultLen = 0;

	clearCache( clearExpiredOnly );

	for(
		Array<STRING>::const_iterator it = script.cbegin(), endIT = script.cend();
		it != endIT;
		++it
	)
	{
		int		miliSec = getValue<int>( *it );
		if( miliSec )
		{
			Sleep( miliSec );
		}
		else
		{
			const HTTPclientResponse	&theResponse = getHttpResponse( *it );
			if( !theResponse.getFetchCount() )
			{
				resultLen += HTTPrequest::Get( *it );
			}
			else
			{
				resultLen += theResponse.getResponseSize();
			}
		}
	}
	return resultLen;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace net
}	// namespace gak



