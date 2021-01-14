/*
		Project:		GAKLIB
		Module:			CGITOOLS.CPP
		Description:	Tools for CGI Applications
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/cgitools.h>
#include <gak/html.h>
#include <gak/t_string.h>
#include <gak/http.h>
#include <gak/numericString.h>

// -------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{
namespace net
{

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void CGIrequest::loadQueryString( const STRING &iQueryString )
{
	T_STRING	param;
	T_STRING	queryString = iQueryString;

	STRING		name, value;

	param = queryString.getFirstToken( "&" );

	while( param[0U] )
	{
		name = param.getFirstToken( "=" );
		value = param.subString( name.strlen()+1 );
		if( name[0U] && value[0U] && !(*this)[name] )
		{
			value = webUnEscape( value );
			updateField( name, (const char*)value );
		}
		param = queryString.getNextToken();
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void CGIrequest::loadQueryString( void )
{
	const char *env;
	STRING		queryString;
	size_t		i, contentLength;
	int			c;

	env = getenv( "QUERY_STRING" );
	if( env )
	{
		queryString = env;
		loadQueryString( queryString );
	}
	env = getenv( "CONTENT_LENGTH" );
	if( env && (contentLength = getValue<size_t>( env )) > 0 )
	{
		env = getenv( "CONTENT_TYPE" );
		if( env && !strcmp( env, "application/x-www-form-urlencoded" ) )
		{
			queryString = "";
			for( i=0; i<contentLength; i++ )
			{
				c = getc( stdin );
				if( c == EOF )
					break;
				queryString += (char)c;
			}
			loadQueryString( queryString );
		}
	}
}

void CGIrequest::loadCookies( void )
{
	const char *env;
	T_STRING	param, cookies;
	STRING		name, value;

	env = getenv( "HTTP_COOKIE" );
	if( env )
	{
		cookies = env;
		param = cookies.getFirstToken( ";" );

		while( param[0U] )
		{
			name = param.getFirstToken( "=" );
			value = param.getNextToken();
			if( name[0U] && value[0U] && !(*this)[name] )
			{
				value = webUnEscape( value );
				updateField( name, (const char*)value );
			}
			param = cookies.getNextToken();
		}
	}
}

void CGIrequest::loadEnvironment( void )
{
	const char *env;

	if( (env = getenv( "HTTP_COOKIE" )) != NULL )
		updateField( "HTTP_COOKIE", env );

	if( (env = getenv( "QUERY_STRING" )) != NULL )
		updateField( "QUERY_STRING", env );

	if( (env = getenv( "PATH_INFO" )) != NULL )
		updateField( "PATH_INFO", env );

	if( (env = getenv( "SCRIPT_NAME" )) != NULL )
		updateField( "SCRIPT_NAME", env );

	if( (env = getenv( "SERVER_NAME" )) != NULL )
		updateField( "SERVER_NAME", env );

	if( (env = getenv( "SERVER_PORT" )) != NULL )
		updateField( "SERVER_PORT", env );

	if( (env = getenv( "SERVER_URL" )) != NULL )
		updateField( "SERVER_URL", env );
}

}	// namespace net
}	// namespace gak


