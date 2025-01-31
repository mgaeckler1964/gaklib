/*
		Project:		GAKLIB
		Module:			wideString.cpp
		Description:	process strings with wide characters
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
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

#include <gak/wideString.h>
#include <gak/wideChar.h>
#include <gak/textReader.h>
#include <gak/logfile.h>
#include <gak/stringStream.h>

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

static uSTRING	dummy;

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

void uSTRING::fromWstring( const wchar_t *src )
{
	clear();

	wchar_t	c;
	while( (c=*src++) != 0 )
		addElement( c );

	addElement( 0 );
}

void uSTRING::fromString( const STRING &src )
{
	if( src.getCharSet() == STR_UTF8 )
		decodeUTF8( src );
	else
	{
		clear();
		setMinSize( src.strlen() );

		for( size_t i=0; i<src.strlen(); i++ )
		{
			unsigned char	c = src[i];
			if( c<128 )
				addElement( (wchar_t)c );
			else
				addElement( convertChar( c ) );
		}

		addElement( 0 );
	}
}

void uSTRING::privateDecodeUTF8( const STRING &src )
{
	unsigned char	c;
	wchar_t			nextChar;
	size_t			i=0;

	clear();
	setMinSize( src.strlen() );

	while( i<src.strlen() )
	{
		c = (unsigned char)src[i++];
		if( c <128 )
		{
			nextChar = c;
		}
		else
		{
			int				byteCount;
			unsigned long	byteValue = c;

			// count the number of bytes
			signed char byteMask = '\x80';
			signed char tmpVal;

			byteCount = 0;
			do
			{
				tmpVal = (char)(byteValue & byteMask);
				if( tmpVal != byteMask )
					break;

				byteCount++;
				byteMask >>= 1;
			} while( byteMask );

			// mask out the counter bits
			byteValue = byteValue & ~byteMask;
			while( --byteCount > 0 )
			{
				c = (unsigned char)src[i++];
				if( c <=127 )
				{
					i--;
					break;
				}
				byteValue = (byteValue << 6) | (c & ~0xC0);
			}

			nextChar = wchar_t(byteValue);
		}

		addElement( nextChar );
	}

	addElement( 0 );
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

STRING uSTRING::encodeUTF8( void ) const
{
	unsigned char	c;
	size_t			numChar = size();
	unsigned long	unicode, utf8Code;
	STRING			result;

	result.setMinSize( numChar );
	for( const_iterator it = cbegin(), endIT = cend(); it != endIT; ++it )
	{
		unicode = *it;
		if( unicode < 128 )
			result += (char)unicode;
		else
		{
			utf8Code = gak::encodeUTF8( (wchar_t)unicode );
			while( utf8Code )
			{
				c = (unsigned char)((utf8Code & 0xFF000000) >> 24);
				if( c )
					result += char(c);
				utf8Code <<= 8;
			}
		}
	}

	result.setCharSet( STR_UTF8 );

	return result;
}

STRING uSTRING::toString( void ) const
{
	bool			canAnsi = true;
	size_t			numChar = size();
	unsigned long	unicode;
	STRING			result;

	for( const_iterator it = cbegin(), endIT = cend(); it != endIT; ++it )
	{
		unicode = *it;
		if( unicode >= 128 )
		{
			if( !convertWChar( wchar_t(unicode ) ) )
			{
				canAnsi = false;
/*v*/			break;
			}
		}
	}

	if( canAnsi )
	{
		result.setMinSize( numChar );
		for( const_iterator it = cbegin(), endIT = cend(); it != endIT; ++it )
		{
			unicode = *it;
			if( unicode >= 128 )
			{
				result += char(convertWChar( wchar_t(unicode) ));
			}
			else
			{
				result += char(unicode);
			}
		}

		result.setCharSet( STR_ANSI );
	}
	else
	{
		result = encodeUTF8();
	}

	return result;
}

STRING STRING::decodeUTF8( void ) const
{
	doEnterFunctionEx(gakLogging::llDetail,"STRING::decodeUTF8");
	doLogValueEx(gakLogging::llDetail, strlen() );
	STR_CHARSET	cs = getCharSet();
	if( cs == STR_OEM || cs == STR_ASCII || !strlen() )
	{
		return *this;
	}
	else
	{
		char			c;
		STRING			result;

		iSTRINGstream	theInputStream( *this );
		TextReader		theReader( &theInputStream );

		theReader.setEncoding( UTF_8 );

		while( (c=theReader.getNextWithBlank()) != 0 && result.strlen() < strlen() )
		{
			result += c;
		}

		result.setCharSet( STR_ANSI );

		doLogValueEx(gakLogging::llDetail, result.strlen() );
		return result;
	}
}

STRING STRING::encodeUTF8( void ) const
{
	if( getCharSet() == STR_UTF8 || !strlen() )
	{
		return *this;
	}
	else
	{
		unsigned char	c;
		unsigned long	unicode, utf8Code;
		STRING			result;

		for( const char *cp = text->string; (c=(unsigned char)*cp) != 0; cp++ )
		{
			if( c < 128 )
				result += (char)c;
			else
			{
				unicode = convertChar( c );
				utf8Code = gak::encodeUTF8( (wchar_t)unicode );
				while( utf8Code )
				{
					c = (unsigned char)((utf8Code & 0xFF000000) >> 24);
					if( c )
						result += char(c);
					utf8Code <<= 8;
				}
			}
		}
		result.setCharSet( STR_UTF8 );
		return result;
	}
}

STR_CHARSET	STRING::testCharSet( void ) const
{
	size_t		i, len = strlen();
	signed char	c;
	const char	*cp = text ? text->string : NULL;
	STR_CHARSET	resultSet = getCharSet();

	if( resultSet == STR_CS_UNKNOWN || resultSet == STR_ASCII )
	{
		resultSet = STR_ASCII;
		for( i=0; i<len; i++ )
		{
			c = cp[i];
			if( c < 0 )
			{
				resultSet = STR_ANSI;
				break;
			}
		}
	}
	else if( resultSet == STR_UTF8 )
	{
		iSTRINGstream	theInputStream( *this );
		TextReader		theReader( &theInputStream );

		theReader.setEncoding( UTF_8 );
		resultSet = STR_ASCII;

		while( (c=theReader.getNextWithBlank()) != 0 )
		{
			if( c < 0 )
			{
				resultSet = STR_ANSI;
			}
			else if( theReader.hasUtf8failure() )
			{
				resultSet = STR_UNICODE;
				break;
			}
		}
	}

	return resultSet;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
