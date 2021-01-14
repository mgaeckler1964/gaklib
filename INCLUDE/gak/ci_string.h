/*
		Project:		GAKLIB
		Module:			ci_string.h
		Description:	case insesitive strings
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

#ifndef GAKLIB_CI_STRINGS_H
#define GAKLIB_CI_STRINGS_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/string.h>
#include <gak/ansiChar.h>

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

class CI_STRING : public STRING
{
	public:
	CI_STRING() {};
	explicit CI_STRING( char c ) : STRING( c ) {};
	CI_STRING( const char *c ) : STRING( c ) {};
	CI_STRING( const STRING &c ) : STRING( c ) {};
	CI_STRING( const CI_STRING &c ) : STRING( c ) {};

	int compare( const STRING &string )						const
	{
		return compareI( string );
	}
	int compare( const char *string )						const
	{
		return compareI( string );
	}

	bool operator != ( const char *string )					const
	{
		return compareI( string );
	}
	bool operator != ( const STRING &string )				const
	{
		return compareI( string );
	}

	/* why ? */
	bool operator != ( char *string )						const
	{
		return compareI( (const char *)string );
	}

	bool operator == ( const char *string )					const
	{
		return compareI( string ) == 0;
	}
	bool operator == ( const STRING &string )				const
	{
		return compareI( string ) == 0;
	}
	/* why ? */
	bool operator == ( char *string )						const
	{
		return compareI( (const char *)string ) == 0;
	}

	bool operator < ( const char *string )					const
	{
		return compareI( string ) < 0;
	}
	bool operator < ( const STRING &string )				const
	{
		return compareI( string ) < 0;
	}

	bool operator <= ( const char *string )					const
	{
		return compareI( string ) <= 0;
	}
	bool operator <= ( const STRING &string )				const
	{
		return compareI( string ) <= 0;
	}

	bool operator > ( const char *string )					const
	{
		return compareI( string ) > 0;
	}
	bool operator > ( const STRING &string )				const
	{
		return compareI( string ) > 0;
	}

	bool operator >= ( const char *string )					const
	{
		return compareI( string ) >= 0;
	}
	bool operator >= ( const STRING &string )				const
	{
		return compareI( string ) >= 0;
	}

	bool beginsWith( const char *text )						const
	{
		return bool(::beginsWithI( getText(), text ));
	}

	bool endsWith( const char *text )						const
	{
		return bool(::endsWithI( getText(), text ));
	}
	bool endsWith( char c )									const
	{
		return ansiToUpper( lastChar() ) == ansiToUpper( c );
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
#	pragma option -a.
#	pragma option -p.
#endif

#endif
