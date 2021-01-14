/*
		Project:		GAKLIB
		Module:			STRING.C
		Description:	String functions (Using type STR)
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

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#if defined( __BORLANDC__ ) && __BORLANDC__ == 0x621
// otherweise I get al lot of useless warnings about unused identifiers
#pragma option -w-use
#pragma option -w-pch
#pragma option -w-eff
#pragma option -w-8058
#endif
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <gak/string.h>
#include <gak/ansiChar.h>

/* --------------------------------------------------------------------- */
/* ----- constants ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

#define DEFSIZE		32UL	/* this must be a power of 2 */
#define NUMELIPISIS	3
#define ELIPISIS	'.'

/* --------------------------------------------------------------------- */
/* ----- module statics ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

STR *newStr( void )
{
	return resizeStr( NULL, (size_t)DEFSIZE, cTrue );
}

STR *setMinSizeStr( STR *str, size_t newSize )
{
	if( str )
	{
		str->minSize = 0;
	}

	str = resizeStr( str, newSize, cTrue );

	if( str )
	{
		str->minSize = newSize;
	}

	return str;
}

STR *resizeStr( STR *str, size_t newSize, cBool exact )
{
	STR 	*newTmpStr;
	size_t	minSize;

	if( str )
	{
		minSize = str->minSize;
		if( newSize < str->actSize )
		{
			str->actSize = newSize;
			str->string[newSize] = 0;
		}
	}
	else
	{
		minSize = 0;
	}

	if( !exact )
	{
		newSize += (size_t)(DEFSIZE-1UL);
		newSize &= (size_t)(~(DEFSIZE-1UL));
	}

	if( !str || (newSize != str->bufSize && newSize >= str->minSize) )
	{
		if( (newTmpStr = (STR*)realloc( str, newSize + sizeof( STR ))) != NULL )
		{
			newTmpStr->bufSize = newSize;
			newTmpStr->minSize = minSize;

			if( !str )
			{
				newTmpStr->string[0] = 0;
				newTmpStr->actSize   = 0;
				newTmpStr->usageCount = 0;
				newTmpStr->charset = STR_CS_UNKNOWN;
			}
		}
		else if( str )
		{
			free( str );
		}
	}
	else
	{
		newTmpStr = str;
	}

	return newTmpStr;
}

STR *addStr( STR *dest, const char *source )
{
	size_t	newSize;

	if( !source || (!*source && dest) )
		return dest;

	newSize = dest ? dest->actSize : 0;
	newSize += strlen( source );
	dest = resizeStr( dest, newSize, cFalse );
	if( dest )
	{
		strcpy( dest->string + dest->actSize, source );
		dest->actSize = newSize;
	}
	return dest;
}

STR *delStr( STR *dest, size_t pos, size_t len )
{
	size_t	newSize;

	if( !dest )
		return dest;

	// pos beyoind end
	if( pos > dest->actSize )
		pos = dest->actSize;

	// len too large
	if( pos + len > dest->actSize )
		len = dest->actSize - pos;

	if( !len )
		return dest;

	newSize = dest->actSize - len;

	strcpy( dest->string + pos, dest->string + pos + len );
	dest->actSize = newSize;

	dest = resizeStr( dest, newSize, cFalse );

	return dest;
}

STR *insStr( STR *dest, size_t pos, const char *source )
{
	size_t	len, newSize, i, j;

	if( !source || !*source )
	{
		return dest;
	}

	if( dest )
	{
		newSize = dest->actSize;
		if( pos > dest->actSize )
		{
			pos = dest->actSize;
		}
	}
	else
	{
		newSize = 0;
		pos = 0;
	}

	len = strlen( source );
	newSize += len;
	dest = resizeStr( dest, newSize, cFalse );
	if( dest )
	{
		for( i=dest->actSize, j=newSize; i>pos; i--, j-- )
		{
			dest->string[j] = dest->string[i];
		}
		// special processing of last char (required if pos == 0)
		dest->string[j] = dest->string[i];

		strncpy( dest->string + pos, source, len );

		dest->actSize = newSize;
	}
	return dest;
}

STR *addCharStr( STR *dest, char c, size_t count )
{
	size_t actSize = dest ? dest->actSize : 0;

	if( c && count )
	{
		dest = resizeStr( dest, actSize+count, cFalse );
		if( dest )
		{
			while( count )
			{
				dest->string[actSize++] = c;
				count--;
			}
			dest->string[actSize]   = 0;
			dest->actSize = actSize;
		}
	}
	else if( dest )
	{
		dest->string[actSize] = 0;
	}

	return dest;
}

STR *insCharStr( STR *dest, size_t pos, char c, size_t count )
{
	size_t actSize = dest ? dest->actSize : 0;

	if( c && count )
	{
		size_t	newSize = actSize+count;
		if( pos > actSize )
		{
			pos = actSize;
		}
		dest = resizeStr( dest, newSize, cFalse );
		if( dest )
		{
			size_t	i, j;
			for( i=dest->actSize, j=newSize; i>pos; i--, j-- )
			{
				dest->string[j] = dest->string[i];
			}
			// special processing of last char (required if pos == 0)
			dest->string[j] = dest->string[i];

			while( count )
			{
				dest->string[pos++] = c;
				count--;
			}
			dest->string[newSize]   = 0;
			dest->actSize = newSize;
		}
	}
	else if( dest )
	{
		dest->string[actSize] = 0;
	}

	return dest;
}

STR *appendStr( STR *dest, const STR *source )
{
	size_t	newSize;

	if( !source )
		return dest;

	newSize = source->actSize + (dest ? dest->actSize : 0);
	dest = resizeStr( dest, newSize, cFalse );
	if( dest )
	{
		strcpy( dest->string + dest->actSize, source->string );
		dest->actSize = newSize;
	}
	return dest;
}

STR *setStr( STR *dest, const char *source )
{
	size_t newSize;

	if( !source )
	{
		return dest;
	}

	newSize = strlen( source );

	dest = resizeStr( dest, newSize, cFalse );
	if( dest )
	{
		strcpy( dest->string, source );
		dest->actSize = newSize;
	}
	return dest;
}

STR *copyStr( STR *dest, const STR *source )
{
	size_t newSize;

	if( !source )
		return dest;

	newSize = source->actSize;
	dest = resizeStr( dest, newSize, cFalse );
	if( dest )
	{
		strcpy( dest->string, source->string );
		dest->actSize = newSize;
		dest->charset = source->charset;
	}
	return dest;
}

STR *stripBlanks( STR *str )
{
	unsigned char	c;		// for msvc 8-(
	char			*src, *dest;
    size_t			numBlanks;

	if( !str )
	{
/*@*/	return NULL;
	}

	/* strip leading blanks */
	dest = src = str->string;
	while( (c=*src)!=0 && isspace( c ) )
	{
		src++;
	}

	if( c ) while( 1 )
	{
		/* copy chars until next blank */
		while( (c=*src)!=0 && !isspace( c ) )
		{
			*dest++ = c;
			src++;
		}

		/* test end of string */
		if( !c )
		{
/*v*/		break;
		}

		/* count number of blanks, following */
		numBlanks = 0;
		while( (c=src[numBlanks])!=0 && isspace( c ) )
		{
			numBlanks++;
		}

		/* test end of string */
		if( !c )
		{
/*v*/		break;
		}

		/* end of string not yet found -> copy the blanks */
		strncpy( dest, src, numBlanks );
		dest += numBlanks;
		src  += numBlanks; 
	}

	*dest = 0;

	/* store the new len */
	str->actSize = (size_t)(dest - (char *)(str->string));

	return str; 
}

STR *stripLeftChar( STR *str, const char c )
{
	char	*src, *dest;
	size_t	numStriped;

	if( !str )
	{
/*@*/	return NULL;
	}

	dest = src = str->string;
	while( *src == c )
	{
		src++;
	}
	numStriped = src-dest;

	if( numStriped )
	{
		strcpy( dest, src );

		str->actSize -= numStriped;
	}
	return str; 
}

STR *stripRightChar( STR *str, const char c )
{
	size_t	newSize;
	char	*current;

	if( !str )
	{
/*@*/	return NULL;
	}

	newSize = str->actSize;
	current = str->string + newSize;
	while( newSize > 0 )
	{
		--newSize;
		--current;
		if( *current != c )
		{
/*@*/		break;
		}
	}
	newSize++;
	current++;

	str->actSize = newSize;
	*current = 0;

	return str;
}

size_t searchText( STR *str, const char *string, size_t startPos, cBool wholeWord, cBool matchCase, cBool downSearch )
{
	char	*cp, *max;
	size_t	len, offset = STR_NOT_FOUND;
	cBool	testWord;

	if( downSearch )
	{
		offset = 1;
	}
	else
	{
		startPos--;
	}

	if( !str )
	{
/*@*/	return STR_NOT_FOUND;
	}

	len = strlen( string );
	max = str->string + (str->actSize - len );
	testWord = false;

	for( cp = str->string+startPos; cp >= str->string && cp <= max; cp += offset )
	{
		if( matchCase )
		{
			if( !strncmp( cp, string, len ) )
			{
				if( !wholeWord )
				{
					return (size_t)(cp - (char *)(str->string));
				}
				else
				{
					testWord = true;
				}
			}
		}
		else
		{
			if( !strncmpi( cp, string, len ) )
			{
				if( !wholeWord )
				{
/***/				return (size_t)(cp - (char *)(str->string));
				}
				else
				{
					testWord = true;
				}
			}
		}
		if( testWord )
		{
			if( (cp == str->string || isspace( cp[-1] ) || ispunct( cp[-1] ) )
			&&  (cp[len] == 0 || isspace( cp[len ] ) || ispunct( cp[len] ) ) )
			{
/***/			return (size_t)(cp - (char *)(str->string));
			}

			testWord = false;
		}
	}
	return STR_NOT_FOUND;
}

STR *replaceText( STR *str, size_t startPos, size_t len, const char *newText )
{
	char	*cp;
	char	c;
	size_t	newLen = strlen( newText );
	STR		*newTextStr;

	if( len >= newLen )
	{
		for( cp = str->string + startPos; (c=*newText) != 0; cp++, newText++ )
		{
			*cp = c;
		}
		if( len > newLen )
		{
			for( newText = str->string + startPos + len; (c=*newText) != 0; cp++, newText++ )
			{
				*cp = c;
			}
			//cp++;
			*cp = 0;
			str->actSize -= (len - newLen);
		}
		newTextStr = str;
	}
	else
	{
		if( startPos > 0 )
			newTextStr = subString( str, 0, startPos );
		else
			newTextStr = NULL;

		newTextStr = addStr( newTextStr, newText );

		if( str && startPos + len < str->actSize )
		{
			newTextStr = addStr( newTextStr, str->string + startPos + len );
		}
	}
	return newTextStr;
}

size_t searchChar( STR *str, char c, size_t startPos )
{
	char	*cp;

	if( !str || !str->actSize )
/*@*/	return STR_NOT_FOUND;

	cp = strchr( str->string+startPos, c );
	if( cp )
/***/	return (size_t)(cp-str->string);
	else
/*@*/	return STR_NOT_FOUND;
}

size_t searchRChar( STR *str, char c )
{
	char	*cp;

	if( !str || !str->actSize )
/*@*/	return STR_NOT_FOUND;

	cp = str->string + str->actSize;
	while( cp >= str->string )
	{
		if( *cp == c )
			break;

		cp--;
	}

	return (size_t)(cp-str->string);
}

STR *subString( STR *str, size_t start_pos, size_t len )
{
	STR *newStr;

	if( !str || start_pos > str->actSize )
		return NULL;

	if( str->actSize < len || str->actSize < start_pos + len )
		len = str->actSize - start_pos;

	newStr = resizeStr( NULL, len, cFalse );
	if( newStr )
	{
		newStr->actSize = len;
		strncpy( newStr->string, str->string+start_pos, len );

		newStr->string[len] = 0;
	}
	return newStr;
}

STR *leftString( STR *str, size_t len )
{
	return subString( str, 0, len );
}

STR *rightString( STR *str, size_t len )
{
	if( str )
	{
		if( str->actSize < len )
			len = str->actSize;

		return subString( str, str->actSize - len, len );
	}
	else
		return NULL;
}

cBool beginsWith( STR *str, const char *text )
{
	if( !text || !*text )
		return true;
	else if( str )
	{
		size_t i;

		for( i=0; *text; i++, text++ )
		{
			if( str->string[i] != *text )
				return false;
		}

		return true;
	}

	return false;
}

cBool endsWith( STR *str, const char *text )
{
	if( !text || !*text )
		return true;
	else if( str )
	{
		size_t	i;
		size_t	len = strlen( text );

		if( len > str->actSize )
			return false;

		for( i=str->actSize-len; *text; i++, text++ )
		{
			if( str->string[i] != *text )
				return false;
		}

		return true;
	}
	return false;
}

cBool beginsWithI( STR *str, const char *text )
{
	if( !text || !*text )
	{
		return true;
	}
	else if( str )
	{
		size_t i;

		for( i=0; *text; i++, text++ )
		{
			if( ansiToUpper(str->string[i]) != ansiToUpper(*text) )
			{
				return false;
			}
		}

		return true;
	}
	return false;
}

cBool endsWithI( STR *str, const char *text )
{
	if( !text || !*text )
	{
		return true;
	}
	else if( str )
	{
		size_t	i;
		size_t	len = strlen( text );

		if( len > str->actSize )
		{
			return false;
		}

		for( i=str->actSize-len; *text; i++, text++ )
		{
			if( ansiToUpper(str->string[i]) != ansiToUpper(*text) )
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

cBool patternMatch( const char *string, const char *pattern )
{
	cBool	result = false;
	char	s, p;

	while( !result )
	{
		s = *string;
		p = *pattern;
		if( !s || !p )
		{
			if( !s )
			{
				if( !p || !strcmp( pattern, "*" ) )
				{
					result = true;
				}
			}
			break;
		}

		if( p=='?' )
		{
			string++;
			pattern++;
		}
		else if( p=='*' )
		{
			while( (p = *++pattern) != 0 )
			{
				if( p!='?' && p!='*' )
					break;
			}
			if( p )
			{
				while( (s = *string) != 0)
				{
					if( s==p )
					{
						result = patternMatch( string, pattern );
						if( result )
							break;
					}
					string++;

				}
				break;
			}
			else
				result = true;
		}
		else if( s==p )
		{
			string++;
			pattern++;
		}
		else
		{
			break;
		}
	}
	return result;
}

cBool match( STR *str, const char *pattern )
{
	if( !pattern || !*pattern )
		return false;
	else if( str )
		return patternMatch( str->string, pattern );

	return false;
}

STR *upperCase( STR *str )
{
	if( str )
	{
		char *cp;

		for( cp=str->string; *cp; cp++ )
		{
			*cp = (char)ansiToUpper( *cp );
		}
	}
	return str;
}

STR *lowerCase( STR *str )
{
	if( str )
	{
		char *cp;

		for( cp=str->string; *cp; cp++ )
		{
			*cp = (char)ansiToLower( *cp );
		}
	}
	return str;
}

STR *replaceChar( STR *str, char searchFor, char replacement )
{
	if( str )
	{
		char *cp;

		for( cp=str->string; *cp; cp++ )
		{
			if( *cp == searchFor )
			{
				*cp = replacement;
			}
		}
	}
	return str;
}

RLINE_ENDS getLineEnds( STR *str )
{
	if( str )
	{
		char	*cp, c;

		for( cp=str->string; (c=*cp)!=0; cp++ )
		{
			if( c == '\n' )
			{
				cp++;
				c = *cp;
				if( c == '\r' )
					return RL_END_LFCR;
				else
					return RL_END_LF;
			}
			else if( c == '\r' )
			{
				cp++;
				c = *cp;
				if( c == '\n' )
					return RL_END_CRLF;
				else
					return RL_END_CR;
			}
		}
	}

#if defined( __MSDOS__ ) || defined ( _MSDOS )|| defined( __TOS__ ) || defined ( _Windows )
	return RL_END_CRLF;
#elif defined( __UNIX__ ) || defined( UNIX ) || defined( __MACH__ ) || defined( __unix__ )
	return RL_END_LF;
#else
#	error "Unkown OS"
#endif
}

size_t getLineCount( STR *str )
{
	size_t		i, count, len;
	RLINE_ENDS	lineEnd = getLineEnds( str );
	char		searchChar = (char)((lineEnd == RL_END_CR) ? '\r' : '\n');

	for( i=0, count=0, len=str->actSize; i<len; i++ )
	{
		if( str->string[i] == searchChar )
		{
			count++;
		}
	}

	return count;
}

STR *replaceLineEnds( STR *str, RLINE_ENDS newLineEnd )
{
	char	*cpSrc, *cpDest, *lineEnd, *lf, c, searchChar;
	size_t	oldSize, newSize;

	RLINE_ENDS oldLineEnd = getLineEnds( str );
	if( oldLineEnd == newLineEnd )
	{
/***/	return str;		// nowthing to change
	}
	searchChar = (char)(
		(oldLineEnd == RL_END_CR || oldLineEnd == RL_END_CRLF)
		? '\r' : '\n'
	);

	oldSize = (oldLineEnd == RL_END_CRLF || oldLineEnd == RL_END_LFCR)
		? 2 : 1;
	newSize = (newLineEnd == RL_END_CRLF || newLineEnd == RL_END_LFCR)
		? 2 : 1;

	switch( newLineEnd )
	{
		case RL_END_CRLF:	lineEnd="\r\n";		break;
		case RL_END_CR:		lineEnd="\r";		break;
		case RL_END_LFCR:	lineEnd="\n\r";		break;
		case RL_END_LF:		lineEnd="\n";		break;
		default:			lineEnd="\r\n";		break;
	}
	if( oldSize < newSize )
	{
		size_t	lineCount = getLineCount( str );
		STR		*newData = resizeStr( NULL, str->actSize+lineCount, cFalse );

		for( cpSrc = str->string, cpDest = newData->string; (c=*cpSrc)!=0; cpSrc++ )
		{
			if( c==searchChar )
			{
				for( lf=lineEnd; (c=*lf)!=0; lf++ )
					*cpDest++ = c;
			}
			else if( c!='\n' && c!='\r' )
				*cpDest++ = c;
		}
		*cpDest = 0;

		newData->actSize = cpDest-newData->string;
		free( str );
		str = newData;
	}
	else
	{
		for( cpSrc = str->string; (c=*cpSrc)!=0; cpSrc++ )
		{
			if( c == searchChar )
			{
				break;
			}
		}
		for( cpDest=cpSrc; (c=*cpSrc)!=0; cpSrc++ )
		{
			if( c==searchChar )
			{
				for( lf=lineEnd; (c=*lf)!=0; lf++ )
				{
					*cpDest++ = c;
				}

				if( oldSize == 2 )
				{
					cpSrc++;
				}
			}
			else if( c!='\n' && c!='\r' )
			{
				*cpDest++ = c;
			}
		}
		*cpDest = 0;

		str->actSize = cpDest-str->string;
	}
	return str;
}

STR *pad( STR *str, size_t len, STR_PADDING mode )
{
	size_t	actSize = str ? str->actSize : 0;

	if( actSize < len )
	{
		size_t	count = len - actSize;
		if( mode == STR_P_LEFT )
		{
			str = insCharStr( str, 0, ' ', count );
		}
		else
		{
			str = addCharStr( str, ' ', count );
		}
	}
	else if( actSize > len )
	{
		size_t	count = NUMELIPISIS;
		if( count > len )
		{
			count = len;
		}
		if( mode == STR_P_LEFT )
		{
			char	*cp;

			str = rightString( str, len );
			cp = str->string;
			while( count-- )
			{
				*cp++ = ELIPISIS;
			}
		}
		else
		{
			char *cp;

			str = resizeStr( str, len, cFalse );
			cp = str->string + len - count;
			while( *cp != 0 )
			{
				*cp++ = ELIPISIS;
			}
		}
	}

	return str;
}
