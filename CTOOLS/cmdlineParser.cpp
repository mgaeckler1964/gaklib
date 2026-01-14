/*
		Project:		GAKLIB
		Module:			cmdlineParser.cpp
		Description:	
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/cmdlineParser.h>
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

static bool beginsWith( const char *arg, const char *opt )
{
	char ca, co;
	do
	{
		ca = *arg++;
		co = *opt++;
	} while( ca && co && ca == co );

	return co == 0;
}

static size_t searchOptionString( const char *arg, const CommandLine::Options *opt )
{
	const CommandLine::Options * const options = opt;
	while( opt->optionChar )
	{
		if( opt->optionString && *opt->optionString && beginsWith( arg, opt->optionString ) )
			break;
		++opt;
	}
	if( !opt->optionChar )
		throw IllegalOption( arg );

	return opt - options;
}

static const char *searchAssignment( const char *cmdLineOption )
{
	char	c;
	size_t	index;

	for( index = 0; 0!=(c=cmdLineOption[index]) &&  '='!=c && ':'!=c; ++index )
		;

	return c ? cmdLineOption + index + 1: NULL;
}

static size_t searchOptionChar( char arg, const CommandLine::Options *opt )
{
	const CommandLine::Options * const options = opt;
	while( opt->optionChar )
	{
		if( opt->optionChar == arg )
			break;
		++opt;
	}
	if( !opt->optionChar )
		throw IllegalOption( arg );

	return opt - options;
}

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

void CommandLine::parseCommandLine( const Options *opt, const char ** const iargv )
{
	const char ** argv = iargv + 1; // ignore program name
	int argc = 1;
	int flags = 0;

	const char	**source = argv;
	const char	**target = argv;
	const char	*arg;

	while( (arg = *argv++) != NULL )
	{
		size_t		optIndex = Container::no_index;
		const char	*cmdLineOption;
		if( !isOptChar( arg[0] ) )
		{
			if( target != source )
				*target = arg;
			++target;
			++source;
			argc++;
		}
		else if( arg[0] == '-' && arg[1] == '-' )
		{
			cmdLineOption = arg+2;
			optIndex = searchOptionString( cmdLineOption, opt );
		}
		else
		{
			cmdLineOption = arg+1;
			optIndex = searchOptionChar( ansiToUpper(*cmdLineOption), opt );
		}
		if( optIndex != Container::no_index )
		{
			++source;
			flags |= opt[optIndex].flags;
			if( opt[optIndex].flags & needArg )
			{
				const char *param = opt[optIndex].flags & noAssignOp
					? NULL
					: searchAssignment( cmdLineOption )					// works for -XYZ=value
				;
				if( !param && cmdLineOption == arg+1 && arg[2] )		// something like -Xvalue?
				{
					param = arg+2;
				}
				if( !param )											// expect -XYZ value
				{
					param = *argv++;
					++source;
				}
				if( param )
				{
					parameter[opt[optIndex].optionChar] += STRING(param);
				}
				else
				{
					throw ArgumentMissing( cmdLineOption );
				}
			}
			else
			{
				if( cmdLineOption == arg+1 )		// something like -XYZ
				{
					char c;
					while( (c = *(++cmdLineOption)) != 0 )
					{
						optIndex = searchOptionChar( ansiToUpper(c), opt );
						if( optIndex != Container::no_index && !(opt[optIndex].flags & needArg) )
						{
							flags |= opt[optIndex].flags;
						}
						else
						{
							throw IllegalOption( arg+1 );
						}
					}
				}
			}
		}
	}
	while( opt->optionChar )
	{
		if( opt->flags & needArg )
		{
			size_t	numParam = parameter[opt->optionChar].size();
			if( opt->minCount > numParam )
			{
				throw MandatoryMissing( opt->optionChar, opt->optionString );
			}
			else if( opt->maxCount < numParam )
			{
				throw DuplicateOptions( opt->optionChar, opt->optionString );
			}
		}
		else if( opt->minCount && !(flags & opt->flags) )
		{
			throw MandatoryMissing( opt->optionChar, opt->optionString );
		}
		opt++;
	}


	iargv[argc] = NULL;
	this->argc = argc;
	this->argv = iargv;
	this->flags = flags;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

std::ostream &operator << (std::ostream &out, const CommandLine::Options *options )
{
	out << "Options are\n";

	while( options->optionChar )
	{
		out << '-' << options->optionChar << " --" << options->optionString;
		if( options->argumentDescription )
			out << ' ' << options->argumentDescription;
		
		out << std::endl;
		++options;
	}

	return out;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

