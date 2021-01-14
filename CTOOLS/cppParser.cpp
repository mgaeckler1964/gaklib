/*
		Project:		GAKLIB
		Module:			cppParser.cpp
		Description:	C++ parser
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

#include <gak/cppParser.h>
#include <gak/logfile.h>

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

static const char *keywords[] =
{
	"auto",
	"bool",
	"break",
	"catch",
	"char",
	"class",
	"const",
	"const_cast",
	"continue",
	"decltype",
	"default",
	"define",
	"defined",
	"delete",
	"do",
	"double",
	"elif",
	"else",
	"endif",
	"enum",
	"error",
	"extern",
	"false",
	"float",
	"for",
	"friend",
	"if",
	"ifdef",
	"ifndef",
	"include",
	"int",
	"long",
	"mutable",
	"namespace",
	"new",
	"operator",
	"pragma",
	"private",
	"protected",
	"public",
	"reinterpret_cast",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"static_cast",
	"struct",
	"switch",
	"template",
	"this",
	"throw",
	"true",
	"try",
	"typedef",
	"typeid",
	"typename",
	"undef",
	"union",
	"unsigned",
	"using",
	"virtual",
	"volatile",
	"while",
};

static const char *operators[] = {
	"!",
	"!=",
	"(",
	")",
	",",
	".",
	"/",
	"//",
	"/*",
	"/=",
	":",
	"::",
	";",
	"<",
	"<<",
	"<=",
	">",
	">>",
	">=",
	"?",
	"[",
	"]",
	"\\",
	"{",
	"}",
	"#",
	"##",
	"%",
	"%=",
	"&",
	"&&",
	"*",
	"*/",
	"*=",
	"+",
	"++",
	"+=",
	"-",
	"->",
	"--",
	"-=",
	"=",
	"==",
	"^",
	"^=",
	"|",
	"||",
	"~",
	"~=",
};

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static bool hasFlag( const STRING &literal, char flag )
{
	char lastChar = ansiToUpper( literal.lastChar() );
	if( lastChar == flag )
		return true;
	char otherChar = literal.strlen() >= 2 ? ansiToUpper( literal[literal.strlen()-2] ) : 0;

	return otherChar == flag;
}

static inline bool hasUnsignedFlag( const STRING &literal )
{
	return hasFlag( literal, 'U' );
}

static inline bool hasLongFlag( const STRING &literal )
{
	return hasFlag( literal, 'L' );
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

CPPparser::CPPparser( const STRING &theFileName ) : ProgramParser( theFileName )
{
	setKeywords( gak::keywords, arraySize( gak::keywords ) );
	setOperators( gak::operators, arraySize( gak::operators ), oCommentStart, oCommentEnd, oComment );
}

CPPparser::CPPparser( std::istream *theInput, const STRING &theFileName )
: ProgramParser( theInput, theFileName )
{
	setKeywords( gak::keywords, arraySize( gak::keywords ) );
	setOperators( gak::operators, arraySize( gak::operators ), oCommentStart, oCommentEnd, oComment );
}

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

ProgramParser *CPPparser::createNew( const STRING &theFileName ) const
{
	return new CPPparser( theFileName );
}
 
ProgramParser *CPPparser::createNew( std::istream *theInput, const STRING &theFileName ) const
{
	return new CPPparser( theInput, theFileName );
}

int CPPparser::_getLiteralType( const ProgramToken &literal ) const
{
#if 0  
	int type;

	assert( literal.type == ttLITERAL );
	if( literal.subType == tstSTRING )
	{
		type = charType | pointerFlag;
	}
	else if( literal.subType == tstCHAR )
	{
		type = charType;
	}
	else if( literal.subType == tstFLOAT || ansiToUpper( literal.value.lastChar() ) == 'F' )
	{
		type = doubleType;
	}
	else
	{
		type = hasLongFlag( literal.value ) ? longType : intType;
		if( hasUnsignedFlag(literal.value ) )
			type |= unsignedFlag;
	}

	return type;
#endif

	return 0;
}

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

