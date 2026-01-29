/*
		Project:		GAKLIB
		Module:			array.cpp
		Description:	dynamic arrays
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <fstream>

#include <gak/ansiChar.h>
#include <gak/array.h>
#include <gak/t_string.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
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

#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0520
const int ArrayOfStrings::CHECK_STRINGS		= 0x01;		// ""
const int ArrayOfStrings::CHECK_CHARS		= 0x02;		// ''
const int ArrayOfStrings::CHECK_BRACES		= 0x04;		// {}
const int ArrayOfStrings::CHECK_BRACKETS	= 0x08;		// []
const int ArrayOfStrings::CHECK_PARENTESIS	= 0x10;		// ()
#endif

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

void ArrayOfStrings::createElements( const STRING &source, const char *delimiters, bool noEmpties )
{
	T_STRING	tSource = source;

	clear();

	STRING	element = tSource.getFirstToken( delimiters );

	while( !element.isNullPtr() )
	{
		if( !noEmpties || !element.isEmpty() )
			addElement( element );

		element = tSource.getNextToken();
	}
}

void ArrayOfStrings::createElements( const char *source )
{
	STRING	param;
	char	c;
	bool	inString = false;

	clear();
	while( (c=*source++) != 0 )
	{
		if( isSpace(c) && !inString )
		{
			if( !param.isEmpty() )
			{
				addElement( param );
				param = "";
			}
		}
		else if( c == '\"' )
		{
			inString = !inString;
		}
		else
		{
			param += c;
		}
	}
	if( !param.isEmpty() )
	{
		addElement( param );
	}
}

void ArrayOfStrings::createElements( const char *source, int flags )
{
	STRING	param;
	char	c;
	bool	inString = false;
	bool	inChar = false;
	int		parentesisLevel = 0,
			bracketLevel = 0,
			braceLevel = 0;

	clear();

	while( (c=*source++) != 0 )
	{
		if( c==' '
		&& !inString && !inChar
		&& !parentesisLevel && !bracketLevel && !braceLevel )
		{
			if( !param.isEmpty() )
			{
				addElement( param );
				param = "";
			}
		}
		else if( c == '\"' && (flags & CHECK_STRINGS) )
		{
			inString = !inString;
			param += c;
		}
		else if( c == '"' && (flags & CHECK_CHARS) )
		{
			inChar = !inChar;
			param += c;
		}
		else if( c == '(' && (flags & CHECK_PARENTESIS) )
		{
			parentesisLevel++;
			param += c;
		}
		else if( c == ')' && (flags & CHECK_PARENTESIS) )
		{
			parentesisLevel--;
			param += c;
		}
		else if( c == '{' && (flags & CHECK_BRACES) )
		{
			braceLevel++;
			param += c;
		}
		else if( c == '}' && (flags & CHECK_BRACES) )
		{
			braceLevel--;
			param += c;
		}
		else if( c == '[' && (flags & CHECK_BRACKETS) )
		{
			bracketLevel++;
			param += c;
		}
		else if( c == ']' && (flags & CHECK_BRACKETS) )
		{
			bracketLevel--;
			param += c;
		}
		else
			param += c;
	}
	if( !param.isEmpty() )
	{
		addElement( param );
	}
}

void ArrayOfStrings::writeToFile( const STRING &fileName ) const
{
	ofstream of( fileName );

	for( const_iterator it=cbegin(), endIT = cend(); it != endIT; ++it )
	{
		of << *it << '\n';
	}
}

void ArrayOfStrings::readFromFile( const STRING &fileName )
{
	std::ifstream ifs( fileName );

	while( ifs.good() )
	{
		STRING &theLine = createElement();
		ifs >> theLine;
		if( !ifs.good() && !theLine.strlen() )
		{
			removeElementAt(size()-1);
			break;
		}
	}
}

void ArrayOfStrings::fillArgV( PODarray<const char *> *argv ) const
{
	argv->setChunkSize( size() );
	for( const_iterator it=cbegin(), endIT = cend(); it != endIT; ++it )
	{
		argv->createElement() = it->c_str();
	}
}

double ArrayOfFloats::sum( void ) const
{
	double			sum = 0.0;
	const double	*data = getDataBuffer();

	for( size_t i=0; i<size(); i++ )
	{
		sum += *data++;
	}

	return sum;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

