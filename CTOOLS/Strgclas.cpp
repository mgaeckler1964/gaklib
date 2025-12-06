/*
		Project:		GAKLIB
		Module:			STRGCLAS.CPP
		Description:	The String Class. This is a C++ frontend for the
						C STR-class
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <gak/gaklib.h>
#include <gak/string.h>
#include <gak/wideString.h>
#include <gak/wideChar.h>
#include <gak/exception.h>
#include <gak/numericString.h>
#include <gak/dynamic.h>
#include <gak/ansiChar.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

char STRING::defaultChar = 0;

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

void STRING::setText( const char *newText, size_t maxLen )
{
	if( text )
	{
		if( text->string == newText )
		{
			return;
		}

		release();
	}

	if( newText )
	{
		text = addStr( NULL, newText );
		if( text && maxLen != MAX_LEN )
		{
			text = resizeStr( text, maxLen, cFalse );
		}

		if( text )
		{
			text->usageCount = 1;
			text->charset = testCharSet();
		}
	}
}

void STRING::setText( const STRING &source )
{
	if( text != source.text && this != &source )
	{
		release();

		text = source.text;
		if( text )
		{
			text->usageCount++;
		}
	}
}

void STRING::setText( const DynamicVar &source )
{
	setText( source.getString() );
}

void STRING::setText( STR *source )
{
	release();
	text = source;
	if( text )
	{
		text->usageCount++;
	}
}

void STRING::setText( char first, size_t count )
{
	release();

	text = ::addCharStr( NULL, first, count );

	if( text )
	{
		text->usageCount = 1;
		text->charset = testCharSet();
	}
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

STRING STRING::subString( size_t start_pos, size_t len ) const
{
	STRING	newString;
	STR		*newText = ::subString( text, start_pos, len );

	if( newText )
	{
		newText->usageCount = 0;
		newString.setText( newText );
	}

	return newString;
}

STRING STRING::leftString( size_t len ) const
{
	STRING	newString;
	STR		*newText = ::leftString( text, len );

	if( newText )
	{
		newText->usageCount = 0;
		newString.setText( newText );
	}

	return newString;
}

STRING STRING::rightString( size_t len ) const
{
	STRING	newString;
	STR		*newText = ::rightString( text, len );

	if( newText )
	{
		newText->usageCount = 0;
		newString.setText( newText );
	}

	return newString;
}

// comparing
int STRING::compare( const char *string ) const
{
	if( isEmpty() && gak::isEmpty( string ) )
	{
		return 0;
	}
	else if( isEmpty() )
	{
		return -1;
	}
	else if( gak::isEmpty( string ) )
	{
		return 1;
	}

	return ansiCompare( text->string, string );
}

int STRING::compare( const STRING &string ) const
{
	if( isEmpty() && gak::isEmpty( string ) )
	{
		return 0;
	}
	else if( isEmpty() )
	{
		return -1;
	}
	else if( gak::isEmpty( string ) )
	{
		return 1;
	}

	if( getCharSet() == STR_UTF8 && string.getCharSet() != STR_UTF8 )
	{
		return ansiCompare( text->string, string.encodeUTF8() );
	}
	else if( getCharSet() != STR_UTF8 && string.getCharSet() == STR_UTF8 )
	{
		return ansiCompare( encodeUTF8(), string.text->string );
	}

	return ansiCompare( text->string, string.text->string );
}

int STRING::compareI( const char *string ) const
{
	if( isEmpty() && gak::isEmpty( string ) )
	{
		return 0;
	}
	else if( isEmpty() )
	{
		return -1;
	}
	else if( gak::isEmpty( string ) )
	{
		return 1;
	}

	return ansiCompareI( text->string, string );
}

int STRING::compareI( const STRING &string ) const
{
	if( isEmpty() && gak::isEmpty( string ) )
	{
		return 0;
	}
	else if( isEmpty() )
	{
		return -1;
	}
	else if( gak::isEmpty( string ) )
	{
		return 1;
	}

	if( getCharSet() == STR_UTF8 && string.getCharSet() != STR_UTF8 )
	{
		return ansiCompareI( text->string, string.encodeUTF8() );
	}
	else if( getCharSet() != STR_UTF8 && string.getCharSet() == STR_UTF8 )
	{
		return ansiCompareI( encodeUTF8(), string.text->string );
	}

	return ansiCompareI( text->string, string.text->string );
}

int STRING::comparenI( const char *string, size_t len ) const
{
	if( isEmpty() && gak::isEmpty( string ) )
	{
		return 0;
	}
	else if( isEmpty() )
	{
		return -1;
	}
	else if( gak::isEmpty( string ) )
	{
		return 1;
	}

	return ansiCompareNI( text->string, string, len );
}

int STRING::comparenI( const STRING &string, size_t len ) const
{
	if( isEmpty() && gak::isEmpty( string ) )
	{
		return 0;
	}
	else if( isEmpty() )
	{
		return -1;
	}
	else if( gak::isEmpty( string ) )
	{
		return 1;
	}

	if( getCharSet() == STR_UTF8 && string.getCharSet() != STR_UTF8 )
	{
		return ansiCompareNI( text->string, string.encodeUTF8(), len );
	}
	else if( getCharSet() != STR_UTF8 && string.getCharSet() == STR_UTF8 )
	{
		return ansiCompareNI( encodeUTF8(), string.text->string, len );
	}

	return ansiCompareNI( text->string, string.text->string, len );
}


// adding
STRING &STRING::operator += ( const char *source )
{
	char	c;

	makePrivate();

	if( this->getCharSet() == STR_UTF8 )
	{
		setMinSize( strlen() + ::strlen( source ) );
		while( (c=*source++) != 0 )
		{
			if( c>0 )
			{
				text = ::addCharStr( text, c, 1 );
			}
			else
			{
				unsigned long utf8Code = convertCharEncodeUTF8( c );
				while( utf8Code )
				{
					c = char((utf8Code & 0xFF000000) >> 24);
					if( c )
					{
						text = ::addCharStr( text, c, 1 );
					}
					utf8Code <<= 8;
				}
			}
		}
	}
	else
	{
		text = addStr( text, source );
		if( text
		&& (text->charset == STR_CS_UNKNOWN || text->charset == STR_ASCII ) )
		{
			text->charset = testCharSet();
		}
	}

	if( text )
	{
		text->usageCount = 1;
	}

	return *this;
}

void STRING::addCharStr( char c, std::size_t count )
{
	if( c && count )
	{
		makePrivate();
		if( this->getCharSet() == STR_UTF8 && !isAscii(c) )
		{
			const unsigned long utf8Code = convertCharEncodeUTF8( c );
			while( count-- )
			{
				char			utfPart;
				unsigned long	tmpUtf8Code = utf8Code;
				while( tmpUtf8Code )
				{
					utfPart = char((tmpUtf8Code & 0xFF000000) >> 24);
					if( utfPart )
					{
						text = ::addCharStr( text, utfPart, 1 );
					}
					tmpUtf8Code <<= 8;
				}
			}
		}
		else
		{
			text = ::addCharStr( text, c, count );
			if( text )
			{
				if( !isAscii(c)
				&& (text->charset==STR_CS_UNKNOWN || text->charset==STR_ASCII) )
				{
					text->charset = STR_ANSI;
				}
				else if( text->charset == STR_CS_UNKNOWN )
				{
					text->charset = STR_ASCII;
				}
			}
		}
		if( text )
		{
			text->usageCount = 1;
		}
	}
}

STRING &STRING::operator += ( const STRING &iSource )
{
	STRING		source = iSource;
	STR_CHARSET	myCharset = getCharSet();
	STR_CHARSET	sourceCharset = source.getCharSet();

	makePrivate();

	if( myCharset == STR_ASCII
	&&  sourceCharset != STR_ASCII
	&&  sourceCharset != STR_CS_UNKNOWN )
	{
		setCharSet( sourceCharset );
		myCharset = sourceCharset;
	}
	if( sourceCharset == STR_UTF8 && myCharset != STR_UTF8 )
	{
		*this = convertToCharset( STR_UTF8 );
	}
	else if( sourceCharset != STR_UTF8 && myCharset == STR_UTF8 )
	{
		source = source.convertToCharset( STR_UTF8 );
	}

	text = appendStr( text, source.text );
	if( text )
	{
		text->usageCount = 1;
		if( text->charset == STR_CS_UNKNOWN )
		{
			text->charset = testCharSet();
		}
	}

	return *this;
}

STRING &STRING::operator += ( size_t offset )
{
	makePrivate();

	if( text && offset < text->actSize )
	{
		text->actSize -= offset;
		memmove(text->string, text->string + offset, text->actSize+1 );
	}
	else
	{
		setText( static_cast<const char *>(NULL), MAX_LEN );
	}
	return *this;
}

const char &STRING::operator [] ( size_t index ) const
{
	if( text && index < text->actSize )
	{
		return text->string[index];
	}
	else
	{
		return defaultChar;
	}
}

char &STRING::operator [] ( size_t index )
{
	if( text && index < text->actSize )
	{
		return text->string[index];
	}
	else
	{
		return defaultChar;
	}
}

void STRING::toBinaryStream( std::ostream &stream ) const
{
	if( text )
	{
		binaryToBinaryStream( stream, uint64(text->actSize) );
		fixedArrayToBinaryStream( stream, text->string, text->actSize );
	}
	else
	{
		uint64	actSize = 0;
		binaryToBinaryStream( stream, actSize );
	}
}

void STRING::fromBinaryStream( std::istream &stream )
{
	uint64 newSize;
	binaryFromBinaryStream( stream, &newSize );
	setText( static_cast<const char *>(NULL), MAX_LEN );
	if( newSize > std::numeric_limits<std::size_t>::max() )
	{
		throw AllocError();
	}
	if( newSize )
	{
		text = resizeStr( NULL, std::size_t(newSize), cTrue );
		fixedArrayFromBinaryStream( stream, text->string, std::size_t(newSize) );
		text->string[std::size_t(newSize)] = 0;
		text->actSize = std::size_t(newSize);
		text->usageCount = 1;
	}
}

void STRING::readLine( std::istream &theStream )
{
	STR_CHARSET	charset = STR_ASCII;
	char	c;

	if( text )
	{
		if( !--(text->usageCount) )
		{
			free( text );
		}

		text = NULL;
	}

	while( theStream.good() )
	{
		theStream.get( c );
		if( !theStream.good() || c == EOF || c == '\n' || c == '\r' || !c )
		{
			break;
		}

		if( c < 0 )
		{
			charset = STR_ANSI;
		}

		text = ::addCharStr( text, c, 1 );
	}
	if( text )
	{
		text->usageCount = 1;
		text->charset = charset;
	}
}

void STRING::readFromFile( const char *fileName )
{
	STR *newText = gak::readFromFile( fileName );
	if( !newText )
		throw OpenReadError( fileName );
	newText->usageCount = 0;
	setText( newText );
}

STRING STRING::simplify() const
{
	static struct
	{
		char		replacement;
		const char	*replaced; 
	} replacements[] = {
		{ 'a', "àáâãäåæ" },
		{ 'b', "p" },
		{ 'c', "çzž" },
		{ 'd', "tðþ" },
		{ 'e', "èéêë" },
		{ 'f', "ƒ" },
		{ 'g', "kq" },
		{ 'i', "ìíîï" },
		{ 'j', "yýÿ" },
		{ 'n', "ñ" },
		{ 'o', "œñòóôõöø" },
		{ 's', "šß" },
		{ 'u', "ùúûü" },
	};

	const char *cp = text ? text->string : NULL;
	if( !cp )
	{
/*@*/	return NULL_STRING;
	}

	STRING	result;
	char	c, prevC = 0;
	while( (c = ansiToLower( *cp++ )) != 0 )
	{
		// ck -> k
		if( c == 'c' )
		{
			if( ansiToLower(*cp) == 'k' )
			{
				c = 'k';
				cp++;
			}
		}
		// ph -> f
		else if( c == 'p' )
		{
			if( ansiToLower(*cp) == 'h' )
			{
				c = 'f';
				cp++;
			}
		}
		else if( c == 's' )
		{
			char	lower = ansiToLower(*cp);
			// sh -> s
			// sz -> s
			if( lower == 'h' || lower == 'z' )
			{
				cp++;
			}
			// sch -> s
			else if( c == 's' && lower == 'c' && ansiToLower(cp[1]) == 'h' )
			{
				cp += 2;
			}
		}
		// th -> t
		else if( c == 't' )
		{
			if( ansiToLower(*cp) == 'h' )
			{
				cp++;
			}
		}

		for( size_t i=0; i<arraySize(replacements); ++i )
		{
			if( ::strchr( replacements[i].replaced, c ))
			{
				c = replacements[i].replacement;
/*v*/			break;
			}
		}
		if( c != prevC )
		{
			result += c;
			prevC = c;
		}
	}

	return result;
}

STRING STRING::cString( void ) const
{
	const char 		*hex="0123456789ABCDEF";
	unsigned char	c;
	STRING			theCstring = STRING('"');

	for( size_t i=0; i<strlen(); i++ )
	{
		c = (unsigned char)text->string[i];
		if( c == '\a' )					// bell
			theCstring += "\\a";
		else if( c == '\b' )			// backspace
			theCstring += "\\b";
		else if( c == '\f' )			// formfeed
			theCstring += "\\f";
		else if( c == '\n' )			// new line
			theCstring += "\\n\"\n\"";
		else if( c == '\r' )			// cariage return
			theCstring += "\\r";
		else if( c == '\t' )			// horizontal tab
			theCstring += "\\t";
		else if( c == '\v' )			// vertical tab
			theCstring += "\\v";
		else if( c == '\"' )
			theCstring += "\\\"";
		else if( c == '\'' )
			theCstring += "\\\'";
		else if( c == '\\' )
			theCstring += "\\\\";
		else if( isprint( c ) )
			theCstring += (char)c;
		else
		{
			theCstring += "\\x";
			theCstring += hex[c>>4];
			theCstring += hex[c&0xF];
			theCstring += "\"\"";
		}
	}

	theCstring += '"';

	return theCstring;
}

STRING STRING::csvString( void ) const
{
	unsigned char	c;
	STRING			theCsvString = STRING('"');

	for( size_t i=0; i<strlen(); i++ )
	{
		c = (unsigned char)text->string[i];
		if( c == '\"' )
			theCsvString += "\"\"";
		else if( c == '\'' )
			theCsvString += "\\\'";
		else if( c == '\\' )
			theCsvString += "\\\\";
		else if( c != '\r' )
			theCsvString += (char)c;
	}

	theCsvString += '"';

	return theCsvString;
}

STRING STRING::convertToCharset( STR_CHARSET charset ) const
{
	if( text && text->charset != charset )
	{
		if( text->charset == STR_UTF8 )
		{
			STRING	result = decodeUTF8();
			if( charset == STR_ANSI )
				return result;
			else
				return result.convertToCharset( charset );
		}
		else if( charset == STR_UTF8 )
		{
			if( text->charset == STR_OEM )
				return convertToCharset( STR_ANSI ).convertToCharset( STR_UTF8 );
			else if( text->charset == STR_ANSI )
				return encodeUTF8();
			else
				return *this;
		}
		else if( charset == STR_ANSI )
		{
			if( text->charset == STR_OEM )
			{
				STRING result = *this;
				result.convertToAnsi();
				return result;
			}
			else
				return *this;
		}
		else
		{
			if( text->charset == STR_ANSI )
			{
				STRING result = *this;
				result.convertToOEM();
				return result;
			}
			else
				return *this;
		}
	}

	return *this;
}

STRING STRING::deCanonical( void ) const
{
	if( text && text->charset == STR_UTF8 )
	{
		// we do only support UTF-8 STRINGS and german umlauts
		STRING	result;
		size_t	len = strlen();
		unsigned char	lastChar = 0, c;
		bool	ccFound = false;

		for( size_t i=0; i<len; i++ )
		{
			c = (unsigned char)text->string[i];
			if( c == 0xcc )
			{
				ccFound = true;
			}
			else if( c == 0x88 )
			{
				if( ccFound )
				{
					switch( lastChar )
					{
						case 'a':
							result += (const char *)STRING('ä').encodeUTF8();
							break;
						case 'o':
							result += (const char *)STRING('ö').encodeUTF8();
							break;
						case 'u':
							result += (const char *)STRING('ü').encodeUTF8();
							break;
						case 'A':
							result += (const char *)STRING('Ä').encodeUTF8();
							break;
						case 'O':
							result += (const char *)STRING('Ö').encodeUTF8();
							break;
						case 'Ü':
							result += (const char *)STRING('Ü').encodeUTF8();
							break;

						default:
							result += (char)lastChar;
							result += '\xCC';
							result += '\x88';
							break;
					}
					ccFound = false;
					lastChar = 0;
				}
				else
				{
					result += char(lastChar);
					result += '\x88';
					lastChar = 0;
				}
			}
			else
			{
				if( lastChar )
				{
					result += char(lastChar);
				}
				if( ccFound )
				{
					result += '\xCC';
					ccFound = false;
				}

				lastChar = c;
			}
		}

		if( lastChar )
			result += (char)lastChar;
		result.setCharSet( STR_UTF8 );
		return result;
	}

	return *this;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING operator + ( const char *oper1, const STRING &oper2 )
{
	STRING	op1( oper1 );
	STRING	result = op1 + oper2;

	return result;
}

STRING operator + ( char oper1, const STRING &oper2 )
{
	STRING	op1( oper1 );
	STRING	result = op1 + oper2;

	return result;
}


}	// namespace gak

