/*
		Project:		GAKLIB
		Module:			textReader.cpp
		Description:	Text Reader for XML- and HTML-Parser
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

#include <ctype.h>

#include <gak/textReader.h>
#include <gak/gaklib.h>
#include <gak/ci_string.h>
#include <gak/wideChar.h>
#include <gak/wideString.h>
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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// Known encodings
const char ISO_8859_1[]	= "iso-8859-1";
const char UTF_8[]		= "UTF-8";

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

static int getBase64Val( int c )
{
	if( c>='A' && c<='Z' )
		c -= 'A';
	else if( c>='a' && c<='z' )
		c -= 'a' - 26;
	else if( c>='0' && c<='9' )
		c -= '0' - 52;
	else if( c == '+' )
		c = 62;
	else if( c == '/' )
		c = 63;
	else if( c == '=' )
		c = -1;				// filler
	else
		c = -2;				// ignore

	return c;
}

/*
	convert a 6-bit value to ascii
*/
static char getBase64Code( unsigned char c )
{
	assert( c < 64 );

	if( c < 26 )
		c += 'A';
	else if( c < 52 )
		c += static_cast<unsigned char>('a' - 26);
	else if( c < 62 )
		c += static_cast<unsigned char>('0' - 52);
	else if( c == 62 )
		c = '+';
	else
		c = '/';

	return c;
}

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

char TextReader::getNextWithBlank( void )
{
	int c = m_inputStream->get();

	if( c == '\n' )
	{
		m_position.m_lineNo++;
		m_position.m_column = 0;
	}
	else if( c == EOF )
	{
		c = 0;
	}
	else
	{
		m_position.m_column++;
	}

	if( c>127 && m_encoding == UTF_8 )
	{
		int				byteCount;
		unsigned long	byteValue = c;

		// count the number of bytes
		signed char byteMask = '\x80';
		signed char tmpVal;

		byteCount = 0;
		do
		{
			tmpVal = char(byteValue & byteMask);
			if( tmpVal != byteMask )
				break;

			byteCount++;
			byteMask >>= 1;
		} while( byteMask );

		// mask out the counter bits
		byteValue = byteValue & ~byteMask;
		while( --byteCount > 0 )
		{
			c = m_inputStream->get();
			if( c <=127 )
			{
				putback( char(c) );
				break;
			}
			byteValue = (byteValue << 6) | (c & ~0xC0);
		}

		c = convertWChar( wchar_t(byteValue) );

		if( !c )
		{
			m_utf8failure = true;
			return '?';
		}
	}

	return char(c);
}

char TextReader::getNextNonBlank( void )
{
	char c;

	while( isSpace( c=getNextWithBlank() ) && c )
		;

	return c;
}

STRING TextReader::readString( char stringDelimiter )
{
	STRING	value;

	char	c;

	// read the string until delimiter
	while( !eof() )
	{
		c = getNextWithBlank();
		if( c != stringDelimiter )
		{
			value += c;
		}
		else
		{
			break;
		}
	}

	return value;
}

STRING TextReader::getErrors( void )
{
	STRING	theErrors;
	STRING	fileName = m_position.m_fileName;

	if( !fileName )
	{
		fileName += ' ';
	}

	for( ArrayOfStrings::const_iterator it = m_errors.cbegin(), endIT = m_errors.cend(); it != endIT; ++it )
	{
		theErrors += fileName + *it + '\n';
	}
	return theErrors;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING encodeBase64( ArrayOfData &src )
{
	STRING base64;
	size_t	i=0, len = src.size();
	unsigned char	c1, c2, c3;

	while( i<len )
	{
		c1 = (unsigned char)src[i++];
		base64 += getBase64Code( (unsigned char)(c1>>2) );

		if( i<len )
		{
			c2 = (unsigned char)src[i++];
			base64 += getBase64Code( (unsigned char)(( (c1<<4) | (c2>>4) ) & 0x3F) );

			if( i<len )
			{
				c3 = (unsigned char)src[i++];
				base64 += getBase64Code( (unsigned char)(( (c2<<2) | (c3>>6) ) & 0x3F) );
				base64 += getBase64Code( (unsigned char)(c3 & 0x3F) );
			}
			else
			{
				base64 += getBase64Code( (unsigned char)((c2<<2) & 0x3F) );
				base64 += '=';
			}
		}
		else
		{
			base64 += getBase64Code( (unsigned char)((c1<<4)  & 0x3F) );
			base64 += "==";
		}
	}

	return base64;
}

STRING encodeBase64( const STRING &src )
{
	ArrayOfData	myData( src );

	return encodeBase64( myData );
}


void decodeBase64( const STRING &src, ArrayOfData &dest )
{
	doEnterFunction("decodeBase64");
	int		c1, c2, c3, c4;
	size_t	i = 0;

	dest.clear();
	while( i<src.strlen() )
	{
		c1 = c2 = c3 = c4 = -1;

		while( i<src.strlen() && (c1 = getBase64Val( src[i++] ))< -1  )
			;
		while( i<src.strlen() && (c2 = getBase64Val( src[i++] ))< -1  )
			;
		while( i<src.strlen() && (c3 = getBase64Val( src[i++] ))< -1  )
			;
		while( i<src.strlen() && (c4 = getBase64Val( src[i++] ))< -1  )
			;

		if( c1 >= 0 && c2 >= 0 )
		{
			c1 = (c1 << 2) | ((c2&0x30) >> 4);
			dest += ( (unsigned char)c1 );

			if( c3 >= 0 )
			{
				c2 = (c2 << 4) | ((c3&0x3C) >> 2);
				dest += ( (unsigned char)c2 );

				if( c4 >= 0 )
				{
					c3 = (c3 << 6) | c4;
					dest += ( (unsigned char)c3 );
				}
			}
		}
	}
}

STRING decodeQuotedPrintable( const STRING &src )
{
	doEnterFunction("decodeQuotedPrintable");
	STRING	result;
	bool	ignore;

	for( size_t i=0; i<src.strlen(); i++ )
	{
		if( src[i] != '=' )
		{
			result += src[i];
		}
		else
		{
			unsigned char	c;
			unsigned char	d1=(unsigned char)ansiToUpper( src[i+1] );
			unsigned char	d2=(unsigned char)ansiToUpper( src[i+2] );

			if( d1 && d2 )
			{
				ignore = false;

				if( d1 == '\r' || d1 == '\n' )
				{
					// ignore CRLF after '='
					i++;
					if( (d2=='\r' || d2=='\n') && d2 != d1 )
						i++;
				}
				else
				{
					if( d1 >= '0' && d1 <= '9' )
						d1 -= '0';
					else if( d1 >= 'A' && d1 <= 'Z' )
						d1 -= (unsigned char)('A' -10);
					else
						ignore = true;

					if( d2 >= '0' && d2 <= '9' )
						d2 -= '0';
					else if( d2 >= 'A' && d2 <= 'Z' )
						d2 -= (unsigned char)('A' - 10);
					else
						ignore = true;

					if( !ignore )
					{
						c = (unsigned char)((d1<<4)|d2);

						result += (char)c;

						i += 2;
					}
				}
			}
		}
	}
	return result;
}

STRING decodeMessageHeader( const STRING &src )
{
	doEnterFunction("decodeMessageHeader");

	STRING			result;
	STRING			tmp = src;

	while( !tmp.isEmpty() )
	{
		std::size_t	markerPos1 = tmp.searchText( "=?" ),
					markerPos3 = STRING::no_index,
					markerPos4 = STRING::no_index;
		if( markerPos1 != STRING::no_index )
		{
			std::size_t	markerPos2 = tmp.searchChar( '?', markerPos1+2 );
			if( markerPos2 != STRING::no_index )
			{
				markerPos3 = tmp.searchChar( '?', markerPos2+1 );
			}
			if( markerPos3 != STRING::no_index )
			{
				markerPos4 = tmp.searchText( "?=", markerPos3+1 );
			}

			if( markerPos1 )
			{
				result += tmp.leftString( markerPos1 );
			}

			if( markerPos2 != STRING::no_index && markerPos3 != STRING::no_index && markerPos4 != STRING::no_index )
			{
				CI_STRING	charSet = tmp.subString( markerPos1+2, markerPos2-markerPos1-2 );
				CI_STRING	transferEncoding = tmp.subString( markerPos2+1, markerPos3-markerPos2-1 );
				STRING		junk = tmp.subString( markerPos3 + 1, markerPos4-markerPos3 - 1 );

				if( transferEncoding == "q" )
				{
					junk = decodeQuotedPrintable( junk );
				}
				else if( transferEncoding == "b" )
				{
					ArrayOfData	tmpData;
					decodeBase64( junk, tmpData );
					junk = "";
					for( 
						ArrayOfData::const_iterator it = tmpData.cbegin(),
							endIT = tmpData.cend();
						it != endIT;
						++it
					)
					{
						junk += *it;
					}
				}
				if( charSet == UTF_8 )
				{
					junk = junk.decodeUTF8();
				}

				result += junk;

				tmp += size_t(markerPos4+2UL);
			}
			else
			{
				tmp += size_t(markerPos1 + 2);
				result += "=?";
			}
		}
		else	// if( markerPos1 != STRING::no_index )
		{
			result += tmp;
			tmp = NULL_STRING;
		}
	}

	return result;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

