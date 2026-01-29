/*
		Project:		GAKLIB
		Module:			cmdlineParser.h
		Description:	the command line and parameter handling
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#ifndef GAK_CMDLINE_PARSER_H
#define GAK_CMDLINE_PARSER_H


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/exception.h>
#include <gak/map.h>

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

#ifdef _Windows
inline bool isOptChar( char c )
{
	return c=='-' || c=='/';
}
#else
inline bool isOptChar( char c )
{
	return c=='-';
}
#endif

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class CmdlineError : public LibraryException
{
	public:
	CmdlineError() : LibraryException( "Command Line Error" )
	{
	}
	CmdlineError( const STRING &errText ) : LibraryException( "Command Line Error" )
	{
		addErrorText( errText );
	}
};

class IllegalOption : public CmdlineError
{
	public:
	template <typename STRING_T>
	IllegalOption( STRING_T optionFound ) : CmdlineError( STRING("Illegal option ") + optionFound ) 
	{
	}
};

class ArgumentMissing : public CmdlineError
{
	public:
	ArgumentMissing( const char *optionFound )
	{
		addErrorText( STRING("Argument missing for ") + optionFound );
	}
};

class MandatoryMissing : public CmdlineError
{
	public:
	MandatoryMissing( char optChar, const char *optionString )
	{
		addErrorText( STRING("Mandatory missing -") + optChar + " --" +  optionString );
	}
};

class DuplicateOptions : public CmdlineError
{
	public:
	DuplicateOptions( char optChar, const char *optionString )
	{
		addErrorText( STRING("Duplicate Option -") + optChar + " --" +  optionString );
	}
};

struct CommandLine
{
	struct Options
	{
		char		optionChar;
		const char	*optionString;
		unsigned	minCount, maxCount;
		int			flags;
		const char	*argumentDescription;
	};

	private:
	void parseCommandLine( const Options *opt, const char **argv );
	void readCommandFile( const Options *opt, const char *argv0 );

	public:
	static const int	needArg		= 0x01;
	static const int	noAssignOp	= 0x02;
	static const int	flagMask	= 0x0F;

	PairMap<char, ArrayOfStrings>	parameter;
	int								flags;
	int								argc;
	const char						**argv;

	CommandLine( const Options *opt=NULL, const char **argv=nullptr ) : flags(0), argc(0), argv(argv)
	{
		if( opt && argv )
		{
			readCommandFile( opt, *argv );
			parseCommandLine( opt, argv );
		}
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

std::ostream &operator << (std::ostream &out, const CommandLine::Options *options );

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

}	//namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
