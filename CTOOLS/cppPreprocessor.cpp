/*
		Project:		GAKLIB
		Module:			cppPreprocessor.cpp
		Description:	C/C++ preprocessor
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

#include <memory>

#include <gak/directory.h>
#include <gak/cppPreprocessor.h>
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

CPreprocessor::CPreprocessor( OutputMode mode ) : outputMode( mode )
{
	ignore = false;
	includeLevel = 0;
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

ArrayOfStrings CPreprocessor::readActualParameter( ProgramParser *parser )
{
	ArrayOfStrings		actualParameter;

	unsigned char	lastChar = 0;
	unsigned char	inString = 0;
	unsigned int	level = 0;
	STRING			parameter;

	while( true )
	{
		unsigned char c = parser->getNextWithBlank();
		if( parser->eof() )
			break;
		if( (c == ',' || c == ')') && !level && !inString )
		{
			parameter.stripBlanks();
			if( !parameter.isEmpty() )
			{
				actualParameter += parameter;
				parameter = NULL_STRING;
			}
			if( c == ')' )
				break;
		}
		else
		{
			parameter += c;
			if( (c == '(' || c == '[') && !inString )
				level++;
			else if( (c == ')' || c == ']') && !inString && level )
				level--;
			else if( (c == '"' || c == '\'') && !inString )
				inString = c;
			else if( c == inString && lastChar != '\\' )
				inString = 0;

		}
		if( lastChar != '\\' )
			lastChar = c;
		else
			lastChar = 0;
	}

	return actualParameter;
}

void CPreprocessor::addActualParameter2Queue( ProgramParser *parser, Queue<ProgramToken> &newQueue )
{
	while( true )
	{
		ProgramToken	token = parser->getNextToken( true );
		if( token.type == ttEOF )
			break;
		else
			newQueue.push( token );
	}
		
}

ProgramToken CPreprocessor::createMacroQueue( const ProgramToken &macroToken, ProgramParser *parser )
{
	assert( macroToken.value.isDefined() );
	STRING	macroName = macroToken.value;
	assert( macros.hasElement( macroName ) );

	PairMap<STRING, STRING>		actualParameterMap;
	const Macro					&macro = macros[macroName];

	if( macro.hasParameter && parser )
	{
		ProgramToken	nextToken = getNextTokenFromMacro( parser, false );
		if( nextToken.type != ttOPERATOR || nextToken.subType != oOpenParnethesis )
		{
			putback( nextToken );
/***/		return macroToken;
		}
		ArrayOfStrings	actualParameter = readActualParameter( parser );
		if( actualParameter.size() < macro.formalParameter.size() )
		{
			addError( "cpp", parser, "too few parameter in macro expansion" );
		}
		else if( actualParameter.size() > macro.formalParameter.size() )
		{
			addError( "cpp", parser, "too many parameter in macro expansion" );
		}
		else
		{
			for( size_t i=0; i<macro.formalParameter.size(); ++i )
			{
				actualParameterMap[macro.formalParameter[i]] = actualParameter[i];
			}
		}

	}
	MacroQueuePtr	newQueue = m_macroQueues.push( MacroQueuePtr::makeShared() );

	for(
		Array<ProgramToken>::const_iterator	it = macro.text.cbegin(), endIT = macro.text.cend();
		it != endIT;
		++it
	)
	{
		if( it->type == ttIDENTIFIER && actualParameterMap.hasElement( it->value ) )
		{
			iSTRINGstream					stream( actualParameterMap[it->value] );
			std::auto_ptr<ProgramParser>	newParser( parser->createNew( &stream ) );

			addActualParameter2Queue( newParser.get(), *newQueue );
		}
		else
		{
			newQueue->push( *it );
		}
	}

	const Declaration	&declaration = macro.declaration;
	if( !declaration.sourceFile.isEmpty() )
	{
		DeclUsages			&usages = crossReference[declaration];
		TextPosition		usage;
	 
		// TODO use F_STRING
		usage.m_fileName = parser ? parser->getPosition().m_fileName : F_STRING();
		usage.m_lineNo = macroToken.lineNo;
		usage.m_column = macroToken.column;
		usages.addElement( usage );
	}

	return getNextTokenFromQueue( false );
}

ProgramToken CPreprocessor::getNextTokenFromQueue( bool doNotExpand )
{
	MacroQueuePtr	queue = m_macroQueues.top();
	ProgramToken	token = queue->pop();

	if( !queue->size() )
	{
		m_macroQueues.removeTop();
	}

	if( !doNotExpand && token.type == ttIDENTIFIER && !skip() && macros.hasElement( token.value ) )
	{
		token = createMacroQueue( token, NULL );
	}

	return token;
}

ProgramToken CPreprocessor::getNextTokenFromMacro( ProgramParser *parser, bool doNotExpand )
{
	ProgramToken	token;
	
	if( m_macroQueues.size() )
	{
		token = getNextTokenFromQueue( doNotExpand ); 
	}
	else
	{
		token = parser->getNextToken( skip() );

		if( !doNotExpand && !skip() && macros.hasElement( token.value ) )
		{
			token = createMacroQueue( token, parser );
		}
	}
	return token;
}

ProgramToken CPreprocessor::getNextTokenWithContinue( ProgramParser *parser, bool doNotExpand )
{
	bool			continueFound = false;
	ProgramToken	result;
	while( true )
	{
		result = getNextTokenFromMacro( parser, doNotExpand );
		if( result.type == ttOPERATOR )
		{
			if( result.subType == oLineContinue )
				continueFound = true;
			else
				break;
		}
		else if( result.type == ttEOL )
		{
			if( !continueFound )
				break;
			continueFound = false;
		}
		else
			break;
	}

	return result;
}


ProgramToken CPreprocessor::evaluateDefined( ProgramParser *parser )
{
	ProgramToken	token = getNextTokenWithContinue( parser, true );
	if( token.type == ttIDENTIFIER || token.type == ttKEYWORD )
	{
		return ProgramToken( macros.hasElement( STRING(token.value) ) ? "1" : "0" );
	}

	if( token.type != ttOPERATOR || token.subType != oOpenParnethesis )
	{
		addError( "cpp defined", parser, "( expected" );
		return ProgramToken("0");
	}
	ProgramToken	identifier = getNextTokenWithContinue( parser, true );
	if( identifier.type != ttIDENTIFIER && token.type != ttKEYWORD )
	{
		addError( "cpp defined", parser, "identifier expected" );
		return ProgramToken("0");
	}
	token = getNextTokenWithContinue( parser, true );
	if( token.type != ttOPERATOR || token.subType != oCloseParenthesis )
	{
		addError( "cpp defined", parser, ") expected" );
		return ProgramToken("0");
	}

	return ProgramToken( macros.hasElement( identifier.value ) ? 1 : 0 );
}

ProgramToken CPreprocessor::getOperand( ProgramParser *parser, int level )
{
	ProgramToken	result;

	while( !result.value.isDefined() )
	{
		ProgramToken token = getNextTokenWithContinue( parser, false );

		switch( token.type ) 
		{
			case ttOPERATOR:
				if( token.subType == oLogicalNot )
				{
					ProgramToken operand = getOperand( parser, level );
					result = ProgramToken( DynamicVar( !operand.value ) );
				}
				else if( token.subType == oOpenParnethesis )
				{
					result = evaluate( parser, level+1);
				}
				else
				{
					addError( "cpp operand", parser, "unexpected operator" );
					result = ProgramToken("0");
				}
				break;
			case ttKEYWORD:
				if( token.subType == key_defined )
				{
					result = evaluateDefined( parser );
				}
				else
				{
					addError( "cpp operand", parser, "unexpected keyword" );
					result = ProgramToken("0");
				}
				break;
			case ttEOF:
				addError( "cpp operand", parser, "unexpected end of file" );
				result = ProgramToken("0");
				break;
			case ttEOL:
				addError( "cpp operand", parser, "unexpected end of line" );
				result = ProgramToken("0");
				break;
			case ttIDENTIFIER:
				result = ProgramToken("0");
				break;
			case ttLITERAL:
				result = token;
				break;
		}
	}

	return result;
}

int CPreprocessor::getPrecedence( CPPoperatorID theOperator )
{
	int result = -1;
	switch( theOperator )
	{
		case oLess:
		case oLessEqual:
		case oGreaterEqual:
		case oGreater:
			result = 8;
			break;
		case oNotEqual:
		case oEqual:
			result = 9;
			break;
		case oLogicalAnd:
			result = 13;
			break;
		case oLogicalOr:
			result = 14;
			break;
	}

	return result;
}

ProgramToken CPreprocessor::evaluate( ProgramParser *parser, const ProgramToken &first, CPPoperatorID theOperator, const ProgramToken &second )
{
	ProgramToken		result;
	const DynamicVar	&left = first.value;
	const DynamicVar	&right = second.value;

	switch( theOperator )
	{
		case oPreprocessorConcat:
			result = ProgramToken(STRING(first.value) + STRING(second.value) );
			break;
		case oLogicalAnd:
			if( left && right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oLogicalOr:
			if( left || right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oLess:
			if( left <= right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oLessEqual:
			if( left <= right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oEqual:
			if( left == right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oNotEqual:
			if( left != right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oGreaterEqual:
			if( left >= right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		case oGreater:
			if( left > right )
				result = ProgramToken(1);
			else
				result = ProgramToken(0);
			break;
		default:
			addError( "cpp operator", parser, "Unexpected operator" );
			break;

		#pragma warning ( suppress: 4061 )
	}

	return result;
}

ProgramToken CPreprocessor::evaluate( ProgramParser *parser, int level )
{
	ProgramToken			result;
	Stack<ProgramToken>		operands;
	Stack<CPPoperatorID>	operators;

	while( true )
	{
		ProgramToken	operand = getOperand( parser, level );
		ProgramToken	token = getNextTokenWithContinue( parser, false );
		if( (token.type == ttEOL && !level)
		||  (level && token.type == ttOPERATOR && token.subType == oCloseParenthesis) )
		{
			operands.push( operand );
			break;
		}
		if( token.type != ttOPERATOR )
		{
			addError( "cpp evaluate", parser, "operator expected" );
			return ProgramToken("0");
		}
		while( operators.size() && getPrecedence( operators.top() ) <= getPrecedence( CPPoperatorID(token.subType) ) ) 
		{
			ProgramToken	first = operands.pop();
			CPPoperatorID	theOperator = operators.pop();
			operand = evaluate( parser, first, theOperator, operand );
		}
		operands.push( operand );
		operators.push( CPPoperatorID(token.subType) ); 
	}

	while( operators.size() && operands.size() >= 2 )
	{
		ProgramToken	second = operands.pop();
		ProgramToken	first = operands.pop();
		CPPoperatorID	theOperator = operators.pop();
		ProgramToken	tmpResult = evaluate( parser, first, theOperator, second );

		operands.push( tmpResult );
	}

	return operands.size() ? operands.pop() : ProgramToken("0");
}

void CPreprocessor::streamToken( ProgramParser *parser, const ProgramToken &token, std::ostream &out ) const
{
	if( skip() )
/***/	return;

	if( outputMode == omText )
	{
		if( token.type == ttEOL )
		{
			out << std::endl;
		}
		else
		{
			parser->streamToken( token, out );
		}
	}
	else
		token.toBinaryStream( out );
}

F_STRING CPreprocessor::searchGlobalInclude( const F_STRING &includeFile ) const
{
	for(
		Array<F_STRING>::const_iterator it = searchPath.cbegin(), endIT = searchPath.cend();
		it != endIT;
		++it
	)
	{
		F_STRING	newInclude = makeFullPath( *it, includeFile );
		if( exists( newInclude ) )
		{
			return newInclude;
		}
	}

	return includeFile;
}

void CPreprocessor::processError( ProgramParser *parser )
{
	ProgramToken	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttLITERAL && token.subType != tstSTRING )
	{
		addError( "cpp error", parser, "literal expected" );
/*@*/	return;
	}
	addError( "error directive", parser, token.value );
	
	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttEOL && token.type != ttEOF )
	{
		addError( "cpp error", parser, "eol expected" );
/*@*/	return;
	}
}

void CPreprocessor::processIf( size_t lineNo, ProgramParser *parser )
{
	bool entireSkip = skip();
	bool doSkip = evaluate( parser, 0 ).value == DynamicVar(0);

	ifs.push( IFstate( lineNo, true ).setSkip( doSkip ).setEntireSkip( entireSkip ) );
}

void CPreprocessor::processElif( size_t lineNo, ProgramParser *parser )
{
	if( !ifs.size() || (ifs.top().flags & IFstate::elseFound) || !(ifs.top().flags & IFstate::allowELIF))
	{
		addError( "cpp elif", parser, "Unexpected elif" );
/*@*/	return;
	}

	IFstate	&state = ifs.top();

	// bool entireSkip = skip();
	bool doSkip = evaluate( parser, 0 ).value == DynamicVar(0) || (state.flags & IFstate::trueFound);

	state.setSkip( doSkip );
}

void CPreprocessor::processIfDef( size_t lineNo, ProgramParser *parser )
{
	ProgramToken	token = getNextTokenWithContinue( parser, true );
	if( token.type != ttIDENTIFIER )
	{
		addError( "cpp ifdef", parser, "identifier expected" );
/*@*/	return;
	}

	bool entireSkip = skip();
	bool doSkip = !macros.hasElement( token.value );
	ifs.push( IFstate( lineNo, false ).setSkip( doSkip ).setEntireSkip( entireSkip ) );

	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttEOL )
	{
		addError( "cpp ifdef", parser, "syntax error" );
/*@*/	return;
	}
}

void CPreprocessor::processIfnDef( size_t lineNo, ProgramParser *parser )
{
	ProgramToken	token = getNextTokenWithContinue( parser, true );
	if( token.type != ttIDENTIFIER )
	{
		addError( "cpp ifndef", parser, "syntax error" );
/*@*/	return;
	}

	bool entireSkip = skip();
	bool doSkip = macros.hasElement( token.value );
	ifs.push( IFstate( lineNo, false ).setSkip( doSkip ).setEntireSkip( entireSkip ) );

	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttEOL )
	{
		addError( "cpp ifndef", parser, "syntax error" );
/*@*/	return;
	}
}

void CPreprocessor::processElse( ProgramParser *parser )
{
	if( !ifs.size() || (ifs.top().flags & IFstate::elseFound) )
	{
		addError( "cpp else", parser, "Unexpected else" );
/*@*/	return;
	}

	IFstate	&state = ifs.top();
	state.setSkip( state.flags & IFstate::trueFound ).setElseFound();

	ProgramToken	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttEOL )
	{
		addError( "cpp else", parser, "syntax error" );
/*@*/	return;
	}
}

void CPreprocessor::processEndif( ProgramParser *parser )
{
	doEnterFunction( "CPreprocessor::processEndif" );

	if( !ifs.size() )
	{
		addError( "cpp endif", parser, "Unexpected endif" );
/*@*/	return;
	}
	ifs.removeTop();

	ProgramToken	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttEOL && token.type != ttEOF )
	{
		addError( "cpp endif", parser, "syntax error" );
/*@*/	return;
	}
}

void CPreprocessor::addMacro( const STRING &macroName, ProgramParser *parser, const Declaration &declaration )
{
	Macro	&macro = macros[macroName];
	bool	first = true;
	enum {
		WANT_TEXT, WANT_COMMA, WANT_PARAMETER
	}		scanMacro = WANT_TEXT;

	size_t	startColumn = parser->getPosition().m_column;
	size_t	startLine = parser->getPosition().m_lineNo;

	macro.clear();
	macro.declaration = declaration;
	while( true )
	{
		ProgramToken	token = getNextTokenWithContinue( parser, false );
		if( token.type == ttEOL || token.type == ttEOF )
/*v*/		break;

		if( first 
		&&  token.type == ttOPERATOR 
		&&  token.subType == oOpenParnethesis 
		&&  startLine == parser->getPosition().m_lineNo 
		&&  startColumn == parser->getPosition().m_column-1 )
		{
			scanMacro = WANT_PARAMETER;
			macro.hasParameter = true;
		}
		else if( scanMacro == WANT_PARAMETER )
		{
			if( token.type == ttIDENTIFIER )
			{
				macro.addFormal( token.value );
				scanMacro = WANT_COMMA;
			}
			else if( token.type == ttOPERATOR && token.subType == oCloseParenthesis )
			{
				scanMacro = WANT_TEXT;
			}
			else
				addError( "cpp define", parser, "formal parameter expected" );
		}
		else if( scanMacro == WANT_COMMA )
		{
			if( token.type == ttOPERATOR && token.subType == oComma )
				scanMacro = WANT_PARAMETER;
			else if( token.type == ttOPERATOR && token.subType == oCloseParenthesis )
				scanMacro = WANT_TEXT;
			else
				addError( "cpp define", parser, "comma expected" );
		}
		else
			macro.addElement( token );

		first = false;
	}
}

void CPreprocessor::processDefine( ProgramParser *parser )
{
	ProgramToken	token = getNextTokenWithContinue( parser, true );
	if( token.type == ttEOF )
	{
		addError( "cpp define", parser, "Unexpected eof" );
/*@*/	return;
	}

	Declaration	newDeclaration;
	newDeclaration.identifier = token.value;
	newDeclaration.lineNo = token.lineNo;
	newDeclaration.column = token.column;
	newDeclaration.sourceFile = parser->getPosition().m_fileName;
	newDeclaration.declType = Declaration::PREPROCESSOR;
	crossReference.getOrCreateElement( newDeclaration );
	addMacro( token.value, parser, newDeclaration );
}

void CPreprocessor::processUndefine( ProgramParser *parser )
{
	ProgramToken	token = getNextTokenWithContinue( parser, true );
	if( token.type == ttEOF )
	{
		addError( "cpp undef", parser, "Unexpected eof" );
/*@*/	return;
	}
	macros.removeElementByKey( token.value );
	token = getNextTokenWithContinue( parser, false );
	if( token.type != ttEOL && token.type != ttEOF )
	{
		addError( "cpp undef", parser, "syntax error" );
/*@*/	return;
	}
}

void CPreprocessor::processInclude( ProgramParser *parser, std::ostream &out )
{
	F_STRING	includeFile;
	ProgramToken	token = getNextTokenWithContinue( parser, false );
	if( token.type == ttEOF )
	{
		addError( "cpp include", parser, "Unexpected eof" );
	}
	if( token.type == ttOPERATOR && token.subType == oLess )
	{
		includeFile = parser->readString( '>' );
		includeFile = searchGlobalInclude( includeFile );
	}
	else if( token.type == ttLITERAL && token.subType == tstSTRING )
	{
		includeFile = STRING(token.value);
	}

	if( !includeFile.isEmpty() )
	{
		includeFile.stripBlanks();
		if( includeFile[0U] == '\"' && includeFile.endsWith( '\"' ) )
		{
			includeFile = includeFile.subString( 1, includeFile.strlen()-2 );
		}
		F_STRING fullSource = makeFullPath( getcwd() + DIRECTORY_DELIMITER, parser->getPosition().m_fileName );
		fullSource = makeFullPath( fullSource, includeFile );

		if( exists( fullSource ) )
		{
			includeFile = fullSource;
		}
		else
		{
			includeFile = searchGlobalInclude( includeFile );
			if( !exists( includeFile ) )
			{
				if( !ignore )
				{
					parser->addError( STRING("Include file ") + includeFile + " not found" );
				}
				includeFile = NULL_STRING;
			}
		}

		if( !includeFile.isEmpty() )
		{
			std::auto_ptr<ProgramParser>	newParser( parser->createNew( includeFile ) );


			includeLevel++;
			precompile( newParser.get(), out );
			includeLevel--;
			STRING errors = newParser->getErrors();
			if( !errors.isEmpty() )
			{
				parser->addError( errors );
			}

			Set<F_STRING>	&includes = includeFiles[newParser->getPosition().m_fileName];
			includes.addElement( parser->getPosition().m_fileName );
		}

		token = getNextTokenWithContinue( parser, false );
		if( token.type != ttEOL && token.type != ttEOF )
		{
			addError( "cpp include", parser, "syntax error" );
		}
	}
	else
	{
		addError( "cpp include", parser, "Illegal include" );
	}
}

#ifdef _MSC_VER
#	pragma warning( disable: 4061 )
#endif

void CPreprocessor::processPreprocessor( ProgramParser *parser, std::ostream &out )
{
	ProgramToken	token = getNextTokenFromMacro( parser, false );
	if( token.type == ttEOF )
	{
		addError( "cpp", parser, "Unexpected eof" );
	}
	else if( token.type != ttKEYWORD )
	{
		addError( "cpp", parser, "keyword expected", token.lineNo, token.column );
	}
	else
	{
		cppKeywordsIDs key = cppKeywordsIDs(token.subType);
		switch( key )
		{
			case key_pragma:
				parser->skipToEol();
				break;

			case key_error:
				if( !skip() )
					processError( parser );
				break;

			case key_include:
				if( !skip() )
					processInclude( parser, out );
				else
					parser->skipToEol();
				break;
			case key_define:
				if( !skip() )
					processDefine( parser );
				else
					parser->skipToEol();
				break;
			case key_undef:
				if( !skip() )
					processUndefine( parser );
				else
					parser->skipToEol();
				break;
			case key_ifdef:
				processIfDef( token.lineNo, parser );
				break;
			case key_ifndef:
				processIfnDef( token.lineNo, parser );
				break;
			case key_if:
				processIf( token.lineNo, parser );
				break;
			case key_elif:
				processElif( token.lineNo, parser );
				break;
			case key_else:
				processElse( parser );
				break;
			case key_endif:
				processEndif( parser );
				break;
			default:
				addError( "cpp", parser, "unexpected keyword", token.lineNo, token.column );
		}
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

void CPreprocessor::precompile( ProgramParser *parser, std::ostream &out )
{
	enum
	{
		sNormal, sPreprocessorLine
	}		state = sNormal;
	bool	newLine = true;
	size_t	oldLevel = ifs.size();

	while( true )
	{
		ProgramToken	token = getNextTokenFromMacro( parser, false );

		if( token.type == ttEOF )
		{
			if( ifs.size() > oldLevel )
				addError( "cpp", parser, STRING("Unexpected end of file for conditional started at line ") + formatNumber( ifs.pop().line ) );
			break;
		}

		if( token.type == ttEOL )
		{
			streamToken( parser, token, out );
			newLine = true; 
		}
		else if( token.type == ttOPERATOR && token.subType == oPreprocessor && newLine )
		{
			processPreprocessor( parser, out );
		}
		else
		{
			streamToken( parser, token, out );
			newLine = false;
		}
	}
	while( oldLevel < ifs.size() )
		ifs.pop();
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

void MergeCrossReference( CrossReference &target, const CrossReference &source )
{
	for( 
		CrossReference::const_iterator it = source.cbegin(), endIT = source.cend();
		it != endIT;
		++it
	)
	{
		const DeclUsages	&sourceUsages = it->getValue();
		DeclUsages			&targetUsages = target[it->getKey()];
		targetUsages.addElements( sourceUsages );
	}
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

