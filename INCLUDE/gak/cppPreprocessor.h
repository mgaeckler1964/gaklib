/*
		Project:		GAKLIB
		Module:			cppPreprocessor.h
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

#ifndef CPP_PREPROCESSOR_H
#define CPP_PREPROCESSOR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/stringStream.h>
#include <gak/cppParser.h>
#include <gak/map.h>
#include <gak/set.h>
#include <gak/queue.h>
#include <gak/stack.h>
#include <gak/directoryEntry.h>
#include <gak/directory.h>
#include <gak/shared.h>

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

struct Declaration
{
	enum DeclarationType
	{
		PREPROCESSOR
	};
	STRING				identifier;
	F_STRING			sourceFile;
	size_t				lineNo,
						column;
	DeclarationType		declType;

	int compare( const Declaration &other ) const
	{
		int compareResult = gak::compare( identifier, other.identifier );
		if( !compareResult )
			compareResult = gak::compare( sourceFile, other.sourceFile );
		if( !compareResult )
			compareResult = gak::compare( lineNo, other.lineNo );
		if( !compareResult )
			compareResult = gak::compare( column, other.column );

		return compareResult;
	}

	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, identifier );
		gak::toBinaryStream( stream, sourceFile );
		gak::toBinaryStream( stream, lineNo );
		gak::toBinaryStream( stream, column );
		gak::toBinaryStream( stream, int(declType) );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &identifier );
		gak::fromBinaryStream( stream, &sourceFile );
		gak::fromBinaryStream( stream, &lineNo );
		gak::fromBinaryStream( stream, &column );
		int declType;
		gak::fromBinaryStream( stream, &declType );
		this->declType = DeclarationType( declType );
	}
};

typedef Set< TextPosition >						DeclUsages;
typedef TreeMap< Declaration, DeclUsages >		CrossReference;
typedef TreeMap< F_STRING, Set<F_STRING> >		Includes;

struct Macro
{
	bool					hasParameter;
	ArrayOfStrings			formalParameter;
	Array<ProgramToken>		text;
	Declaration				declaration;

	Macro()
	{
		hasParameter = false;
	}
	void clear( void )
	{
		formalParameter.clear();
		text.clear();
	}
	void addElement( const ProgramToken &token )
	{
		text += token;
	}
	void addFormal( const STRING &name )
	{
		formalParameter += name;
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class ProgramParser;

class CPreprocessor
{
	struct IFstate
	{
		static const int doSkip		= 0x01;
		static const int entireSkip	= 0x02;
		static const int trueFound	= 0x04;
		static const int elseFound	= 0x08;
		static const int allowELIF	= 0x10;
		size_t	line;
		int		flags;

		IFstate( size_t line=0, bool isIF=false ) : line(line), flags(isIF ? allowELIF : 0)
		{
		}
		IFstate &setSkip( bool skip )
		{
			if( skip )
				flags |= doSkip;
			else
			{
				flags &= ~doSkip;
				flags |= trueFound;
			}
			return *this;
		}
		IFstate &setEntireSkip( bool skip )
		{
			if( skip )
				flags |= entireSkip;
			else
				flags &= ~entireSkip;
			return *this;
		}
		IFstate &setElseFound()
		{
			flags |= elseFound;
			return *this;
		}
	};
	typedef Queue<ProgramToken>			MacroQueue;
	typedef SharedPointer<MacroQueue>	MacroQueuePtr;
	typedef Stack<MacroQueuePtr>		MacroQueues;

	public:
	enum OutputMode
	{
		omBinary, omText
	};
	private:
	OutputMode				outputMode;

	Includes				includeFiles;
	Array<F_STRING>			searchPath;
	bool					ignore;

	PairMap<STRING, Macro>	macros;

	int						includeLevel;
	Stack<IFstate>			ifs;
	MacroQueues				m_macroQueues;

	CrossReference			crossReference;

	bool skip( void ) const
	{
		return ifs.size() ? ((ifs.top().flags & (IFstate::doSkip|IFstate::entireSkip)) != 0) : false;
	}
	void putback( const ProgramToken &token )
	{
		
		MacroQueuePtr	newQueue = m_macroQueues.push( MacroQueuePtr::makeShared() );
		newQueue->push( token );
	}

	void addMacro( const STRING &macroName, ProgramParser *parser, const Declaration &declaration = Declaration() );
	void removeMacro( const STRING &macroName )
	{
		macros.removeElementByKey( macroName );
	}

	ArrayOfStrings readActualParameter( ProgramParser *parser );
	void addActualParameter2Queue( ProgramParser *parser, Queue<ProgramToken> &newQueue );
	ProgramToken createMacroQueue( const ProgramToken &macroToken, ProgramParser *parser );
	ProgramToken getNextTokenFromQueue( bool doNotExpand );
	ProgramToken getNextTokenFromMacro( ProgramParser *parser, bool doNotExpand );
	ProgramToken getNextTokenWithContinue( ProgramParser *parser, bool doNotExpand );

	void streamToken( ProgramParser *parser, const ProgramToken &token, std::ostream &out ) const;
	F_STRING searchGlobalInclude( const F_STRING &includeFile ) const;
	void processError( ProgramParser *parser );

	ProgramToken evaluateDefined( ProgramParser *parser );
	ProgramToken getOperand( ProgramParser *parser, int level );
	int getPrecedence( CPPoperatorID theOperator );
	ProgramToken evaluate( ProgramParser *parser, const ProgramToken &first, CPPoperatorID theOperator, const ProgramToken &second );
	ProgramToken evaluate( ProgramParser *parser, int level );

	void processIf( size_t lineNo, ProgramParser *parser );
	void processElif( size_t lineNo, ProgramParser *parser );
	void processIfDef( size_t lineNo, ProgramParser *parser );
	void processIfnDef( size_t lineNo, ProgramParser *parser );
	void processElse( ProgramParser *parser );
	void processEndif( ProgramParser *parser );
	void processDefine( ProgramParser *parser );
	void processUndefine( ProgramParser *parser );
	void processInclude( ProgramParser *parser, std::ostream &out );
	void processPreprocessor( ProgramParser *parser, std::ostream &out );

	void addError( const STRING &functionName, ProgramParser *parser, const STRING &errText )
	{
		if( !skip() )
			parser->addError( functionName + ": " + errText );
	}
	void addError( const STRING &functionName, ProgramParser *parser, const STRING &errText, size_t lineNo, size_t column )
	{
		if( !skip() )
			parser->addError( functionName + ": " + errText, lineNo, column );
	}

	public:
	explicit CPreprocessor( OutputMode mode );

	void setOutputMode( OutputMode mode )
	{
		outputMode = mode;
	}

	void ignoreIncludeErrors()
	{
		ignore = true;
	}

	void precompile( ProgramParser *parser, std::ostream &out );
	void addIncludePath( const F_STRING &path )
	{
		F_STRING fullIncludePath = fullPath( path );
		if( fullIncludePath.endsWith( DIRECTORY_DELIMITER ) )
			searchPath += fullIncludePath;
		else
			searchPath += F_STRING(fullIncludePath + DIRECTORY_DELIMITER);
	}

	const Includes &getIncludeFiles( void ) const
	{
		return includeFiles;
	}
	Includes &getIncludeFiles( void )
	{
		return includeFiles;
	}

	const CrossReference &getCrossReference() const
	{
		return crossReference;
	}
	CrossReference &getCrossReference()
	{
		return crossReference;
	}

	void addMacro( const STRING &macroName, const STRING &value )
	{
		iSTRINGstream	input( value );
		CPPparser		parser( &input );
		addMacro( macroName, &parser );
	}
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

void MergeCrossReference( CrossReference &target, const CrossReference &source );

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

#endif
