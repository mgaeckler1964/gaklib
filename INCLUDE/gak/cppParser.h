/*
		Project:		GAKLIB
		Module:			cppParser.h
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

#ifndef GAK_CPP_PARSER_h
#define GAK_CPP_PARSER_h

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/progParser.h>

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

enum cppKeywordsIDs
{
	key_auto,
	key_bool,
	key_break,
	key_catch,
	key_char,
	key_class,
	key_const,
	key_const_cast,
	key_continue,
	key_decltype,
	key_default,
	key_define,
	key_defined,
	key_delete,
	key_do,
	key_double,
	key_elif,
	key_else,
	key_endif,
	key_enum,
	key_error,
	key_extern,
	key_false,
	key_float,
	key_for,
	key_friend,
	key_if,
	key_ifdef,
	key_ifndef,
	key_include,
	key_int,
	key_long,
	key_mutable,
	key_namespace,
	key_new,
	key_operator,
	key_pragma,
	key_private,
	key_protected,
	key_public,
	key_reinterpret_cast,
	key_return,
	key_short,
	key_signed,
	key_sizeof,
	key_static,
	key_static_cast,
	key_struct,
	key_switch,
	key_template,
	key_this,
	key_throw,
	key_true,
	key_try,
	key_typedef,
	key_typeid,
	key_typename,
	key_undef,
	key_union,
	key_unsigned,
	key_using,
	key_virtual,
	key_volatile,
	key_while,
};

// static char singleCharOperators[] = "+-*/%.><=!~|&^?:#[](){},;";

enum CPPoperatorID {
	oLogicalNot,
	oNotEqual,
	oOpenParnethesis,
	oCloseParenthesis,
	oComma,
	oDot,
	oDivide,
	oComment,
	oCommentStart,
	oDivideEqual,
	oColon,
	oAccess,
	oStatementEnd,
	oLess,
	oLeftShift,
	oLessEqual,
	oGreater,
	oRightShift,
	oGreaterEqual,
	oConditional,
	oOpenBracket,
	oCloseBracket,
	oLineContinue,
	oOpenBrace,
	oCloseBrace,
	oPreprocessor,
	oPreprocessorConcat,
	oModulo,
	oModuloEqual,
	oBinaryAnd,
	oLogicalAnd,
	oMultiply,
	oCommentEnd,
	oMultiplyEqual,
	oPlus,
	oIncrement,
	oPlusEqual,
	oMinus,
	oArrow,
	oDecrement,
	oMinusEqual,
	oAssign,
	oEqual,
	oXOR,
	oXorEqual,
	oBinaryOr,
	oLogicalOr,
	oBinaryNot,
	oBinaryNotEqual,
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class CPPparser : public ProgramParser
{
	public:
	CPPparser( const STRING &theFileName );
	CPPparser( std::istream *theInput, const STRING &theFileName=NULL_STRING );

	virtual ProgramParser *createNew( const STRING &theFileName ) const;
	virtual ProgramParser *createNew( std::istream *theInput, const STRING &theFileName=NULL_STRING ) const;
	virtual int _getLiteralType( const ProgramToken &literal ) const;
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

#endif	// GAK_CPP_PARSER_h