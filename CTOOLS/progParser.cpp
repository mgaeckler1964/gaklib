/*
		Project:		GAKLIB
		Module:			progParser.cpp
		Description:	Basic parser
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

#include <gak/progParser.h>

#include <gak/cppParser.h>
#include <gak/numericString.h>

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

void ProgramParser::setKeywords( const char *keywords[], size_t numElements )
{
#ifndef NDEBUG
	const char *last=NULL;
#endif
	for( size_t i=0; i<numElements; ++ i )
	{
#ifndef NDEBUG
		if( last )
			assert( ansiCompare( last, *keywords ) < 0 );
		last = *keywords;
#endif
		this->keywords.addElement( *keywords++ );
	}
}

void ProgramParser::setOperators( const char *operators[], size_t numElements, int commentStart, int commentEnd, int lineComment )
{
#ifndef NDEBUG
	const char *last=NULL;
#endif
	for( size_t i=0; i<numElements; ++ i )
	{
#ifndef NDEBUG
		if( last )
			assert( ansiCompare( last, *operators ) < 0 );
		last = *operators;
#endif
		const STRING &oper = this->operators.addElement( *operators++ );
		if( oper.strlen() == 1 )
			singleCharOperators += oper;
	}
#ifndef NDEBUG
	for( size_t i=0; i<numElements; ++ i )
	{
//		std::cout << this->operators[i] << std::endl;
	}
#endif
	this->commentStart = commentStart;
	this->commentEnd = commentEnd;
	this->lineComment = lineComment;
}


// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

bool ProgramParser::isIdentifierChar( const STRING &identifier, char c )
{
	if( c >= 'a' && c <= 'z' )
		return true;
	if( c >= 'A' && c <= 'Z' )
		return true;
	if( c == '_' )
		return true;
	if( !identifier.isEmpty() && c >= '0' && c <= '9' )
		return true;

	return false;
}

/*
	this operation works for those language
	whose multi char operator contains only singleCharOperators and do not have a length > 2
*/
bool ProgramParser::isOperatorChar( const STRING &oper, char c )
{
	assert( !singleCharOperators.isEmpty() );
	if( oper.isEmpty() )
	{
		return singleCharOperators.searchChar( c ) != singleCharOperators.no_index;
	}
	else
	{
		STRING	newOper = oper + c;
		for(
			SortedArray<STRING>::const_iterator it = operators.cbegin(), endIT = operators.cend();
			it != endIT;
			++it
		)
		{
			if( it->beginsWith( newOper ) )
				return true;
		}
	}

	return false;
}

bool ProgramParser::isLiteralChar( char c )
{
	if( c == '\'' || c == '\"' )
		return true;
	if( c >= '0' && c <= '9' )
		return true;

	return false;
}

ProgramToken ProgramParser::readLiteral( char firstChar, bool ignoreErrors )
{
	ProgramToken	result;

	result.type = ttLITERAL;
	result.lineNo = getPosition().m_lineNo;
	result.column = getPosition().m_column;
	STRING	value;

	if( firstChar == '\'' || firstChar == '\"' )
	{
		bool	finished = false;
		bool	backslash = false;

		value = firstChar;
		result.subType = firstChar == '\'' ? tstCHAR : tstSTRING;
		result.flags = 0;
		do
		{
			char	c = getNextWithBlank();
			if( eof() )
			{
				if( !ignoreErrors )
					addError( "unexpected eof" );
				break;
			}
			if( c == '\n' )
			{
				if( !ignoreErrors )
					addError( "unexpected eol" );
				break;
			}
			if( c == '\\' )
				backslash = !backslash;
			else if( c == firstChar && !backslash )
				finished = true;
			else
				backslash = false;

			value += c;
		}		
		while( !finished );
		result.value = value;
	}
	else
	{
		int flags = 0;

		result.subType = tstDECIMAL;
		char	c = firstChar;
		while( true )
		{
			size_t	len = value.strlen();
			char	upperC = ansiToUpper( c );

			if( len == 1 && firstChar == '0' && upperC == 'X' )
			{
				value += '0';	// otherwise this value will be interpreted as octal
				flags |= hexFlag;
			}
			else if( len == 1 && firstChar == '0' && isdigit( c ) )
			{
				flags |= octalFlag;

				value += c;
			}
			else if( isdigit( c ) )
			{
				value += c;
			}
			else if( upperC >= 'A' && upperC <= 'F' && (flags&hexFlag) )
			{
				value += c;
			}
			else if( c == '.' && !(flags&(decimalFlag|cardinalFlags)) )
			{
				flags |= decimalFlag;
				result.subType = tstFLOAT;
				value += c;
			} 
			else if( upperC == 'E' && !(flags&(exponentFlag|cardinalFlags)) )
			{
				flags |= exponentFlag;
				result.subType = tstFLOAT;
				value += c;
			} 
			else if( upperC == 'F' && !(flags&(floatFlag|cardinalFlags)) )
			{
				flags |= floatFlag;
				result.subType = tstFLOAT;
			} 
			else if( upperC == 'U' && !(flags&(unsignedFlag|floatFlags)) )
			{
				flags |= unsignedFlag;
			} 
			else if( upperC == 'L' && !(flags&(longFlag|floatFlags)) )
			{
				flags |= longFlag;
			}
			else
			{
				putback( c );
				break;
			}

			c = getNextWithBlank();
			if( eof() )
				break;
		}

		result.flags = flags;
		if( result.subType == tstFLOAT )
		{
			result.value = value.getValueE<double>();
		}
		else
		{
			unsigned base = flags & octalFlag ? 8 : (flags & hexFlag ? 16 : 10);
			if( flags & longFlag )
			{
				if( flags & unsignedFlag )
				{
					result.value = value.getValueE<unsigned long>(base);
				}
				else
				{
					result.value = value.getValueE<long>(base);
				}
			}
			else
			{
				if( flags & unsignedFlag )
				{
					result.value = value.getValueE<unsigned int>(base);
				}
				else
				{
					result.value = value.getValueE<int>(base);
				}
			}
		}
	}

	return result;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void ProgramParser::skipToToken( const char *endToken )
{
	STRING	processed;
	const char *cp = endToken;
	while( true )
	{
		char c = getNextWithBlank();
		if( eof() )
			break;

		if( c == *cp )
		{
			cp++;
			if( !*cp )
				break;
		}
		else
		{
			cp = endToken;
			/* this is OK for all languages that have only 1 or 2 character operators */
			if( c == *cp )
				cp++;
		}
	}
}

void ProgramParser::skipToEol( void )
{
	while( true )
	{
		char c = getNextWithBlank();
		if( eof() || c == '\n' )
			break;
	}
}

ProgramToken ProgramParser::getNextTokenWithComment( bool ignoreErrors )
{
	ProgramToken	result;
	STRING			value;

	while( true )
	{
		unsigned char	c = getNextWithBlank();
		if( eof() )
		{
			if( result.type == ttUNKOWN )
			{
				result.type = ttEOF;
				result.lineNo = getPosition().m_lineNo;
				result.column = getPosition().m_column;
			}
			break;
		}

		if( c == '\n' )
		{
			if( result.type == ttUNKOWN )
			{
				result.type = ttEOL;
				result.lineNo = getPosition().m_lineNo;
				result.column = getPosition().m_column;
			}
			else
			{
				putback( c );
			}

/*v*/		break;
		}
		if( isspace( c ) )
		{
			if( result.type != ttUNKOWN )
			{
				putback( c );
/*v*/			break;
			}
			else
			{
/*^*/			continue;
			}
		}

		if( result.type == ttUNKOWN )
		{
			result.lineNo = getPosition().m_lineNo;
			result.column = getPosition().m_column;
		}
		if( (result.type == ttUNKOWN || result.type == ttIDENTIFIER) && isIdentifierChar( value, c ) )
		{
			result.type = ttIDENTIFIER;
			value += c;
		}
		else if( (result.type == ttUNKOWN || result.type == ttOPERATOR) && isOperatorChar( value, c ) )
		{
			result.type = ttOPERATOR;
			value += c;
		}
		else if( result.type == ttUNKOWN && isLiteralChar( c ) )
		{
			result = readLiteral( c, ignoreErrors );
			break;
		}
		else if( result.type == ttUNKOWN )
		{
			if( !ignoreErrors )
				addError( STRING("Illegal character: ") + value + c, result.lineNo, result.column );
		}
		else
		{
			putback( c );
/*v*/		break;
		}
	}

	if( result.type == ttIDENTIFIER )
	{
		size_t	keywordIdx = keywords.findElement( value );
		if( keywordIdx != keywords.no_index )
		{
			result.type = ttKEYWORD;
			result.subType = int(keywordIdx);
		} 
	}
	else if( result.type == ttOPERATOR )
	{
		size_t	operIdx = operators.findElement( value );
		assert( operIdx != operators.no_index );
		result.subType = int(operIdx);
	}

	if( result.type != ttLITERAL )
	{
		result.value = value;
	}
	return result;
}

ProgramToken ProgramParser::getNextToken( bool ignoreErrors )
{
	ProgramToken	result;

	while( true )
	{
		result = getNextTokenWithComment(ignoreErrors);

		assert( commentStart >= 0 );
		assert( commentEnd >= 0 );
		assert( lineComment >= 0 );

		if( result.type == ttOPERATOR && result.subType == commentStart )
		{
			skipToToken( operators[commentEnd] );
		}
		else if( result.type == ttOPERATOR && result.subType == lineComment )
		{
			skipToEol();
			result.type = ttEOL;
			break;
		}
		else
			break;
	}

	return result;
}

size_t ProgramParser::streamToken( const ProgramToken &token, std::ostream &out ) const
{
	STRING	text;
	switch( token.type )
	{
		case ttOPERATOR:
			text = operators[token.subType];
			break;
		case ttKEYWORD:
			text = keywords[token.subType] + ' ';
			break;
		case ttLITERAL:
		case ttIDENTIFIER:
			text = STRING(token.value) + ' ';
			break;
	}

	out << text;

	return text.strlen();
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

