/*
		Project:		GAKLIB
		Module:			STRING.H
		Description:	String functions (Using type STR)
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

#ifndef STRING_CLASS_H
#define STRING_CLASS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable: 4986 4820 4668 )
#endif

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#	include <iostream>
#	include <limits>
#endif

#ifdef _Windows
#	ifndef STRICT
#		define STRICT 1
#	endif
#include <windows.h>
#endif

#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#if defined( __BORLANDC__ ) && defined( __cplusplus )
#	include <vcl.h>
#endif

#include <gak/gaklib.h>

#undef strlen


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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#define STR_NOT_FOUND ((size_t)-1)

#define NULL_STRING		STRING( (const char *)0 )
#define EMPTY_STRING	STRING( "" )


/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

typedef enum
{
	STR_CS_UNKNOWN, STR_ASCII, STR_OEM, STR_ANSI, STR_UTF8, STR_UNICODE
} STR_CHARSET;

typedef enum
{
	RL_UNKONW =-1, RL_END_CR, RL_END_LF, RL_END_CRLF, RL_END_LFCR
} RLINE_ENDS;

typedef enum
{
	STR_P_LEFT, STR_P_RIGHT
} STR_PADDING;

typedef struct
{
	long		usageCount;
	size_t		bufSize,
				actSize,
				minSize;
	STR_CHARSET	charset;
	char 		string[1];
} STR;

/* --------------------------------------------------------------------- */
/* ----- prototypes ---------------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"
{
#endif

STR *newStr( void );
STR *setMinSizeStr( STR *str, size_t newSize );
STR *resizeStr( STR *str, size_t newSize, cBool exact );
STR *addStr( STR *dest, const char *source );
STR *delStr( STR *dest, size_t pos, size_t len );
STR *insStr( STR *dest, size_t pos, const char *source );
STR *addCharStr( STR *dest, char c, size_t count );
STR *insCharStr( STR *dest, size_t pos, char c, size_t count );
STR *appendStr( STR *dest, const STR *source );
STR *setStr( STR *dest, const char *source );
STR *copyStr( STR *dest, const STR *source );
STR *stripBlanks( STR *str );
STR *stripLeftChar( STR *str, const char c );
STR *stripRightChar( STR *str, const char c );
size_t searchText( STR *str, const char *string, size_t startPos, cBool wholeWord, cBool matchCase, cBool downSearch );
STR *replaceText( STR *str, size_t startPos, size_t endPos, const char *newText );
size_t searchChar( STR *str, char c, size_t startPos );
size_t searchRChar( STR *str, char c );
STR *subString( STR *str, size_t start_pos, size_t len );
STR *leftString( STR *str, size_t len );
STR *rightString( STR *str, size_t len );
cBool beginsWith( STR *str, const char *text );
cBool endsWith( STR *str, const char *text );
cBool beginsWithI( STR *str, const char *text );
cBool endsWithI( STR *str, const char *text );
cBool patternMatch( const char *string, const char *pattern );
cBool match( STR *str, const char *pattern );
STR *upperCase( STR *str );
STR *lowerCase( STR *str );
STR *replaceChar( STR *str, char searchFor, char replacement );
RLINE_ENDS getLineEnds( STR *str );
size_t getLineCount( STR *str );
STR *replaceLineEnds( STR *str, RLINE_ENDS newLineEnd );
STR *pad( STR *str, size_t len, STR_PADDING mode );

#define setSizeStr( str ) resizeStr( (str), strlen( (str)->string ) );

#ifdef __cplusplus
}	// extern "C"
#endif

/* --------------------------------------------------------------------- */
/* ----- class definitions --------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef __cplusplus

namespace gak
{

STR *freadSTR( FILE *fp );
STR *readNfromFile( FILE *fp, size_t n );
STR *readFromFile( const char *fileName );
bool writeToFile( const STR *str, const char *fileName );

class DynamicVar;

class STRING
{
	public:
	static const size_t no_index = STR_NOT_FOUND;
	static const size_t MAX_LEN = size_t(-1);

	private:
	static char defaultChar;

	STR			*text;

	void setText( STR *source );
	void setText( const char *newText, std::size_t maxLen );
	void setText( const STRING &source );
	void setText( const DynamicVar &source );
	void setText( char first, std::size_t count );

	void makePrivate( void )
	{
		if( text && text->usageCount > 1 )
		{
			text->usageCount--;
			text = copyStr( NULL, text );
			if( text )
			{
				text->usageCount = 1;
			}
		}
	}
	void release( void )
	{
		if( text )
		{
			if( !--(text->usageCount) )
			{
				free( text );
			}
			text  = NULL;
		}
	}

	protected:
	STR	*getText( void ) const
	{
		return text;
	}
	public:
	/*
		-----------------------------------------------------------------------
		Constructors/Destructor
		-----------------------------------------------------------------------
	*/
	STRING()
	{
		text = NULL;
	}
	STRING( const char *newText, size_t	maxLen = no_index)
	{
		text = NULL;
		if( newText )
		{
			setText( newText, maxLen );
		}
	}
	STRING &operator = ( const char *string )
	{
		setText( string, MAX_LEN );
		return *this;
	}

	explicit STRING( char first, size_t count=1 )
	{
		text = NULL;
		setText( first, count );
	}
	STRING &operator = ( char first )
	{
		setText( first, 1 );
		return *this;
	}

	STRING( const STRING &source )
	{
		text = NULL;
		setText( source );
	}
	STRING &operator = ( const STRING &source )
	{
		setText( source );
		return *this;
	}
	STRING( const DynamicVar &source )
	{
		text = NULL;
		setText( source );
	}
	STRING &operator = ( const DynamicVar &source )
	{
		setText( source );
		return *this;
	}
	~STRING()
	{
		release();
	}

	/*
		-----------------------------------------------------------------------
		Size
		-----------------------------------------------------------------------
	*/
	char *setMinSize( size_t newSize )
	{
		makePrivate();

		text = setMinSizeStr( text, newSize );
		if( text )
		{
			text->usageCount = 1;
			return text->string;
		}
		return NULL;
	}
	char *setActSize( size_t newSize )
	{
		makePrivate();
		text = resizeStr( text, newSize, cTrue );
		if( text )
		{
			text->actSize = newSize;
			text->string[newSize] = 0;
			text->usageCount = 1;
			return text->string;
		}
		return NULL;
	}
	size_t strlen( void ) const
	{
		return text ? text->actSize : 0;
	}

	/*
		-----------------------------------------------------------------------
		type conversion
		-----------------------------------------------------------------------
	*/
	operator const char *( void ) const
	{
		return text ? text->string : "";
	}
	operator char * ( void ) const
	{
		return text ? strdup( text->string ) : NULL;
	}

#ifdef __BORLANDC__
	operator AnsiString( void ) const
	{
		return AnsiString( text ? text->string : "" );
	}
#endif
	/*
		 number conversion
		 the counter parts can be found in fmtNumber.h
	*/
	template <typename NUMERIC>
	NUMERIC getValueE(unsigned base=10) const;
	template <typename NUMERIC>
	NUMERIC getValueN(unsigned base=10) const;

	/*
		-----------------------------------------------------------------------
		comparing
		-----------------------------------------------------------------------
	*/
	public:
	bool isNullPtr( void ) const
	{
		return text == NULL;
	}
	bool isEmpty( void ) const
	{
		return text == NULL || text->actSize == 0;
	}
	bool operator ! ( void ) const
	{
		return isEmpty();
	}

	int compare( const char *string )						const;
	int compare( const STRING &string )						const;
	int compareI( const char *string )						const;
	int compareI( const STRING &string )					const;
	int comparenI( const char *string, size_t len )			const;
	int comparenI( const STRING &string, size_t len )		const;

	bool operator != ( const char *string )					const
	{
		return compare( string ) != 0;
	}
	bool operator != ( char *string )						const
	{
		return compare( (const char *)string ) != 0;
	}
	bool operator != ( const STRING &string )				const
	{
		return compare( string ) != 0;
	}

	bool operator == ( const char *string )					const
	{
		return compare( string ) == 0;
	}
	bool operator == ( char *string )						const
	{
		return compare( (const char *)string ) == 0;
	}
	bool operator == ( const STRING &string )				const
	{
		return compare( string ) == 0;
	}


	bool operator < ( const char *string )					const
	{
		return compare( string ) < 0;
	}
	bool operator < ( const STRING &string )				const
	{
		return compare( string ) < 0;
	}

	bool operator <= ( const char *string )					const
	{
		return compare( string ) <= 0;
	}
	bool operator <= ( const STRING &string )				const
	{
		return compare( string ) <= 0;
	}

	bool operator > ( const char *string )					const
	{
		return compare( string ) > 0;
	}
	bool operator > ( const STRING &string )				const
	{
		return compare( string ) > 0;
	}

	bool operator >= ( const char *string )					const
	{
		return compare( string ) >= 0;
	}
	bool operator >= ( const STRING &string )				const
	{
		return compare( string ) >= 0;
	}

	/*
		-----------------------------------------------------------------------
		adding
		-----------------------------------------------------------------------
	*/
	void addCharStr( char c, std::size_t count );
	STRING &operator += ( const char *source );
	STRING &operator += ( char c )
	{
		addCharStr( c, 1 );
		return *this;
	}
	STRING &operator += ( int c )
	{
		addCharStr( char(c), 1 );
		return *this;
	}
	STRING &operator += ( const STRING &source );

	STRING operator + ( const char *source ) const
	{
		STRING	result = *this;

		result += source;

		return result;
	}
	STRING operator + ( char c ) const
	{
		STRING	result = *this;

		result += c;

		return result;
	}
	STRING operator + ( int c ) const
	{
		return *this + char(c);
	}
	STRING operator + ( const STRING &source ) const
	{
		STRING	result = *this;

		result += source;

		return result;
	}

	/*
		-----------------------------------------------------------------------
		querying
		-----------------------------------------------------------------------
	*/
	STRING subString( size_t start_pos, size_t len=no_index ) const;
	STRING leftString( size_t len ) const;
	STRING rightString( size_t len ) const;

	bool beginsWith( const char *text ) const
	{
		return bool(::beginsWith( this->text, text ));
	}
	bool beginsWith( char c ) const
	{
		if( text )
			return text->string[0] == c;
		else
			return false;
	}

	bool endsWith( const char *text ) const
	{
		return bool(::endsWith( this->text, text ));
	}
	bool endsWith( char c ) const
	{
		return lastChar() == c;
	}
	bool beginsWithI( const char *text ) const
	{
		return bool(::beginsWithI( this->text, text ));
	}

	bool endsWithI( const char *text ) const
	{
		return bool(::endsWithI( this->text, text ) );
	}

	bool match( const char *pattern ) const
	{
		return bool(::match( this->text, pattern ) );
	}

	size_t searchText( const char *string, size_t startPos=0, bool wholeWord=false, bool matchCase=true, bool downSearch=true  ) const
	{
		return ::searchText( text, string, startPos, (cBool)wholeWord, (cBool)matchCase, (cBool)downSearch );
	}

	size_t searchChar( char c, size_t startPos = 0 ) const
	{
		return ::searchChar( text, c, startPos );
	}
	const char *strchr( char c ) const
	{
		char	*cp;
		size_t	idx = searchChar( c );
		if( idx == no_index )
			cp = NULL;
		else
			cp = text->string + idx;

		return cp;
	}
	size_t searchRChar( char c ) const
	{
		return ::searchRChar( text, c );
	}
	const char *strrchr( char c ) const
	{
		char	*cp;
		size_t	idx = searchRChar( c );
		if( idx == no_index )
			cp = NULL;
		else
			cp = text->string + idx;

		return cp;
	}
	char lastChar( void ) const
	{
		return text && text->actSize ? text->string[text->actSize-1] : char(0);
	}
	RLINE_ENDS getLineEnds( void ) const
	{
		return ::getLineEnds( text );
	}
	const char *operator + ( std::size_t offset ) const
	{
		return text && offset <= text->actSize  ? text->string+offset : "";
	}
	STRING cString( void ) const;
	STRING csvString( void ) const;

	size_t getUsageCount( void ) const
	{
		return text ? text->usageCount : 0UL;
	}

	STRING uperCaseCopy( void ) const
	{
		STRING copy = *this;

		return copy.upperCase();
	}
	STRING lowerCaseCopy( void ) const
	{
		STRING copy = *this;

		return copy.lowerCase();
	}

	STRING padCopy( size_t len, STR_PADDING mode ) const
	{
		STRING pad = *this;

		return pad.pad( len, mode );
	}

	STRING simplify() const;

	/*
		-----------------------------------------------------------------------
		modifying
		-----------------------------------------------------------------------
	*/
	void replaceText( size_t startPos, size_t len, const char *newText  )
	{
		makePrivate();

		text = ::replaceText( text, startPos, len, newText );
		text->usageCount = 1;
	}

	char &operator [] ( size_t index );
	const char &operator [] ( size_t index ) const;
	STRING &operator += ( size_t offset );

	STRING &stripBlanks( void )
	{
		makePrivate();

		if( text )
		{
			text = ::stripBlanks( text );
			text->usageCount = 1;
		}
		return *this;
	}
	STRING &stripLeftChar( char c )
	{
		makePrivate();

		if( text )
		{
			text = ::stripLeftChar( text, c );
			text->usageCount = 1;
		}
		return *this;
	}
	STRING &stripRightChar( char c )
	{
		makePrivate();

		if( text )
		{
			text = ::stripRightChar( text, c );
			text->usageCount = 1;
		}
		return *this;
	}
	STRING &stripChar( char c )
	{
		makePrivate();

		if( text )
		{
			text = ::stripRightChar( text, c );
			text = ::stripLeftChar( text, c );
			text->usageCount = 1;
		}
		return *this;
	}

	STRING &cut( size_t newLen )
	{
		makePrivate();

		text = resizeStr( text, newLen, cFalse );
		text->usageCount = 1;

		return *this;
	}

	STRING &delStr( std::size_t pos, size_t len )
	{
		if( text )
		{
			makePrivate();

			text = ::delStr( text, pos, len );
			text->usageCount = 1;
		}

		return *this;
	}
	STRING &delChar( std::size_t pos )
	{
		return delStr( pos, 1 );
	}
	STRING &insStr( std::size_t pos, const char *source )
	{
		makePrivate();

		text = ::insStr( text, pos, source );
		text->usageCount = 1;

		return *this;
	}
	STRING &insChar( std::size_t pos, char c, std::size_t count=1 )
	{
		makePrivate();

		text = ::insCharStr( text, pos, c, count );
		text->usageCount = 1;

		return *this;
	}

	STRING &upperCase( void )
	{
		makePrivate();

		if( text )
		{
			::upperCase( text );
			text->usageCount = 1;
		}

		return *this;
	}
	STRING &lowerCase( void )
	{
		makePrivate();

		if( text )
		{
			::lowerCase( text );
			text->usageCount = 1;
		}

		return *this;
	}
	STRING &replaceChar( char searchFor, char replacement )
	{
		makePrivate();

		if( text )
		{
			::replaceChar( text, searchFor, replacement );
			text->usageCount = 1;
		}

		return *this;
	}

	STRING &replaceLineEnds( RLINE_ENDS lineEnd )
	{
		makePrivate();

		if( text )
		{
			text = ::replaceLineEnds( text, lineEnd );
			text->usageCount = 1;
		}

		return *this;
	}

	STRING &pad( size_t len, STR_PADDING mode )
	{
		makePrivate();
		text = ::pad( text, len, mode );
		text->usageCount = 1;
		return *this;
	}

	STRING &condAppend( char c )
	{
		if( lastChar() != c )
		{
			addCharStr( c, 1 );
		}
		return *this;
	}

	/*
		-----------------------------------------------------------------------
		file reading
		-----------------------------------------------------------------------
	*/
	void operator << ( FILE *fp )
	{
		STR	*newText = freadSTR( fp );
		if( newText )
			newText->usageCount = 0;
		setText( newText );
	}
	void operator >> ( FILE *fp )
	{
		if( text )
		{
			fputs( text->string, fp );
			fputc( '\n', fp );
		}
	}

	void readFromFile( const char *fileName );

	void readFromFile( FILE *fp, size_t n )
	{
		STR *newText = readNfromFile( fp, n );
		if( newText )
		{
			newText->usageCount = 0;
		}
		setText( newText );
	}

	bool writeToFile( const char *fileName ) const
	{
		return gak::writeToFile( text, fileName );
	}

	void toFmtStream( std::ostream &theStream ) const
	{
		if( text )
		{
			theStream << text->string;
		}
	}
	void toBinaryStream( std::ostream &stream ) const;
	void fromBinaryStream( std::istream &stream );

	void readLine( std::istream &theStream );

	/*
		-----------------------------------------------------------------------
		file reading
		-----------------------------------------------------------------------
	*/
	private:
	void convertToOEM( void )
	{
#ifdef _Windows
		makePrivate();

		if( text )
		{
			AnsiToOem( text->string, text->string );
			text->usageCount = 1;
			text->charset = STR_OEM;
		}
#endif
	}
	void convertToAnsi( void )
	{
#ifdef _Windows
		makePrivate();

		if( text )
		{
			OemToAnsi( text->string, text->string );
			text->usageCount = 1;
			text->charset = STR_ANSI;
		}
#endif
	}
	public:
	STR_CHARSET getCharSet( void ) const
	{
		return text ? text->charset : STR_CS_UNKNOWN;
	}
	void setCharSet( STR_CHARSET charset )
	{
		if( !text )
		{
			text = newStr();
			text->usageCount = 1;
		}

		text->charset = charset;
	}
	STRING deCanonical( void ) const;
	STRING convertToCharset( STR_CHARSET charset ) const;
	STRING convertToTerminal( void ) const
	{
#if defined( __GNUC__ )
		return convertToCharset( STR_UTF8 );
#elif defined( _Windows )
		return convertToCharset( STR_OEM );
#else
#	error "Don't know environment"
#endif
	}

	/*
		these functions are found in wideString.cpp
	*/
	STRING encodeUTF8( void ) const;
	STRING decodeUTF8( void ) const;
	STR_CHARSET	testCharSet( void ) const;

	/*
		-----------------------------------------------------------------------
		standard container compatibility
		-----------------------------------------------------------------------
	*/
	typedef char		value_type;
	typedef const char	*const_iterator;

	const_iterator cbegin() const
	{
		return text ? text->string : NULL;
	}
	const_iterator cend() const
	{
		return text ? text->string+text->actSize : NULL;
	}
	std::size_t size( void ) const
	{
		return strlen();
	}
	void push_back( value_type c )
	{
		*this += c;
	}
};

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING operator + ( const char *oper1, const STRING &oper2 );
STRING operator + ( char oper1, const STRING &oper2 );

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

inline STRING encodeUTF8( const char *text )
{
	return STRING( text ).encodeUTF8();
}

inline size_t strlen( const STRING &string )
{
	return string.strlen();
}

inline bool isEmpty( const char *cp )
{
	return (bool)(cp == NULL || !*cp);
}

inline bool isEmpty( const STRING &string )
{
	return string.isEmpty();
}

inline std::ostream &operator << ( std::ostream &theStream, const STRING &theSource )
{
	theSource.toFmtStream( theStream );
	return theStream;
}

inline std::istream &operator >> ( std::istream &theStream, STRING &destination )
{
	destination.readLine( theStream );
	return theStream;
}


} // namespace gak

#endif	/* __cplusplus */

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.

#	pragma warn +inl
#endif

#endif	/* STRING_CLASS_H */
