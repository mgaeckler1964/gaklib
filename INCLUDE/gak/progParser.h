/*
		Project:		GAKLIB
		Module:			progParser.h
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

#ifndef GAK_PROGRAMM_PARSER_H
#define GAK_PROGRAMM_PARSER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cassert>

#include <gak/textReader.h>
#include <gak/sortedArray.h>
#include <gak/dynamic.h>

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

namespace typeDefinitions
{
	// used by unused function _getLiteralType
	const int charType			= 0x000;
	const int shortType			= 0x001;
	const int intType			= 0x002;
	const int longType			= 0x004;
	const int longLongType		= 0x008;
	const int floatType			= 0x010;
	const int doubleType		= 0x020;
	const int typeMask			= 0x03F;

	const int unsignedTypeFlag	= 0x040;

	const int pointerFlag		= 0x080;
	const int referenceFlag		= 0x100;
}	// namespace typeDefinitions

// flags for literals
const int unsignedFlag	= 0x01;		// U-postfix
const int longFlag		= 0x02;		// L-postfix
const int hexFlag		= 0x04;
const int octalFlag		= 0x08;
const int floatFlag		= 0x10;		// F-postfix
const int decimalFlag	= 0x20;		// decimal point
const int exponentFlag	= 0x40;		// exponent
const int cardinalFlags	= unsignedFlag|longFlag|hexFlag|octalFlag;
const int floatFlags	= floatFlag|decimalFlag|exponentFlag;



// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum TokenType
{
	ttUNKOWN,
	ttKEYWORD,
	ttIDENTIFIER,
	ttOPERATOR,
	ttLITERAL,
	ttEOL,
	ttEOF
};

enum TokenSubtype
{
	tstUNKOWN, tstSTRING, tstCHAR, tstDECIMAL, tstFLOAT
};

struct ProgramToken
{
	size_t			lineNo,
					column;
	TokenType		type;
	int				subType,
					flags;
	DynamicVar		value;

	ProgramToken() : type( ttUNKOWN )
	{
	}
	explicit ProgramToken( const DynamicVar &firstValue, int subType=tstDECIMAL ) : value( firstValue )
	{
		lineNo = column = 0;
		type = ttLITERAL;
		this->subType = subType;
		this->flags = 0;
	}
	void toBinaryStream( std::ostream &stream ) const
	{
		binaryToBinaryStream( stream, lineNo );
		binaryToBinaryStream( stream, column );
		binaryToBinaryStream( stream, type );
		binaryToBinaryStream( stream, subType );
		value.toBinaryStream( stream );
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class ProgramParser : public TextReader
{
	private:
	SortedArray<STRING>	keywords;
	STRING				singleCharOperators;
	SortedArray<STRING>	operators;
	int					commentStart, commentEnd, lineComment;

	protected:
	void setKeywords( const char *keywords[], size_t numElements );
	void setOperators( const char *operators[], size_t numElements, int commentStart, int commentEnd, int lineComment );

	virtual bool isIdentifierChar( const STRING &identifier, char c );
	virtual bool isOperatorChar( const STRING &oper, char c );

	// virtual bool isValidIdentifer( const STRING &identifier );
	virtual bool isLiteralChar( char c );
	virtual ProgramToken readLiteral( char c, bool ignoreErrors );

	public:
	ProgramParser( const STRING &theFileName ) : TextReader( theFileName )
	{
		commentStart = commentEnd = lineComment = -1;
	}
	ProgramParser( std::istream *theInput, const STRING &theFileName=NULL_STRING )
	: TextReader( theInput, theFileName )
	{
	}

	virtual ProgramParser *createNew( const STRING &theFileName ) const = 0;
	virtual ProgramParser *createNew( std::istream *theInput, const STRING &theFileName=NULL_STRING ) const = 0;
	virtual int _getLiteralType( const ProgramToken &literal ) const = 0;

	ProgramToken getNextTokenWithComment( bool ignoreErrors );
	ProgramToken getNextToken( bool ignoreErrors );
	void skipToToken( const char *endToken );
	void skipToEol( void );

	size_t streamToken( const ProgramToken &token, std::ostream &out ) const;
};

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

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

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

#endif	//  GAK_PROGRAMM_PARSER_H