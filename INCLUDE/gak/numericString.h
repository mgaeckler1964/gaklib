/*
		Project:		GAKLIB
		Module:			numericString.h
		Description:	Parse numbers in string
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

#ifndef GAK_NUMERIC_STR_H
#define GAK_NUMERIC_STR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>
#include <cassert>
#include <math.h>		// otherwise i will loose isnan on Mac

#include <gak/string.h>
#include <gak/exception.h>
#include <gak/ansiChar.h>

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

#undef min
#undef max

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

/// @cond
namespace internal
{
#ifdef __BORLANDC__
#	pragma warn -ccc
#	pragma warn -rch
#	pragma warn -inl
#endif

/**
	parse string, return integer and bad position
*/
template <typename NUMERIC>
NUMERIC getIntegerValue( const char *cp, unsigned base, const char **end )
{
	bool		isNegative = false;
	NUMERIC		result = 0;

	if( cp && *cp )
	{
		unsigned		digit;
		unsigned char	c;

		if( std::numeric_limits<NUMERIC>::is_signed )
		{
			c = *cp;
			if( c == '+' )
			{
				cp++;
			}
			else if( c == '-' )
			{
				isNegative = true;
				cp++;
			}
		}
		while( (c = *cp) != 0 )
		{
			if( c >= '0' && c <= '9' )
			{
				digit = c - '0';
			}
			else
			{
				c = static_cast<unsigned char>(ansiToUpper( c ));
				if( c >= 'A' && c <= 'Z' )
				{
					digit = c - 'A' + 10;
				}
				else
				{
/*v*/				break;
				}
			}
			if( digit >= base )
			{
/*v*/			break;
			}

			result *= NUMERIC(base);
			if( isNegative )
			{
				result -= NUMERIC(digit);
			}
			else
			{
				result += NUMERIC(digit);
			}
			cp++;
		}
	}

	if( end )
	{
		*end = cp;
	}
	return result;
}

/**
	parse string, return integer or throw exception
*/
template <typename NUMERIC>
NUMERIC getIntegerValue( const char *cp, unsigned base )
{
#ifdef __BORLANDC__
	typedef uint32	BaseT;
	typedef int32	SignedBaseT;
#else
	typedef uint64	BaseT;
	typedef int64	SignedBaseT;
#endif

	const char * const start = cp;
	bool			isNegative = false;
	BaseT			result = 0;
	BaseT			digit;
	unsigned char	c = *cp;

	if( c == '+' )
	{
		cp++;
	}
	else if( c == '-' )
	{
		if( std::numeric_limits<NUMERIC>::is_signed )
		{
			isNegative = true;
			cp++;
		}
		else
		{
/*@*/		throw BadNumericFormatError( start );
		}
	}
	while( (c = *cp++) != 0 )
	{
		 if( c >= '0' && c <= '9' )
		 {
			digit = c - '0';
		}
		else
		{
			c = static_cast<unsigned char>(ansiToUpper( c ));
			if( c >= 'A' && c <= 'Z' )
				digit = c - 'A' + 10;
			else
			{
/*@*/			throw BadNumericFormatError( start );
			}
		}
		if( digit >= base )
		{
/*@*/		throw BadNumericFormatError( start );
		}

		result *= base;
		result += digit;
	}

	if( !isNegative
	&&  result > BaseT(std::numeric_limits<NUMERIC>::max()) )
	{
/*@*/	throw IntegerOverflowError( start );
	}

	if( std::numeric_limits<NUMERIC>::is_signed )
	{
		if( isNegative )
		{
			if( SignedBaseT(result*(-1)) < SignedBaseT(std::numeric_limits<NUMERIC>::min()) )
			{
/*@*/			throw IntegerUnderflowError( start );
			}
			return NUMERIC(result*(-1));
		}
	}

	return NUMERIC(result);
}

/**
	parse string, return float and bad position
*/
template <typename NUMERIC>
NUMERIC getFloatValue( const char *cp, const char **end )
{
	bool			isNegative = false;
	NUMERIC			result = 0;
	NUMERIC			digit;
	unsigned char	c = *cp;

	if( c == '+' )
	{
		cp++;
	}
	else if( c == '-' )
	{
		isNegative = true;
		cp++;
	}

	while( (c = *cp) != 0 && c != '.' && c != ',' && ansiToUpper( c ) != 'E' )
	{
		 if( c >= '0' && c <= '9' )
		 {
			digit = NUMERIC(c - '0');
		}
		else
		{
/*v*/		break;
		}

		result *= NUMERIC(10.0);
		result += digit;
		cp++;
	}

	if( c == '.' || c == ',' )
	{
		cp++;
		NUMERIC factor = 1;
		while( (c = *cp) != 0 && ansiToUpper( c ) != 'E' )
		{
			if( c >= '0' && c <= '9' )
			{
				digit = NUMERIC(c - '0');
			}
			else
			{
/*@*/			break;
			}

			factor /= NUMERIC(10);
			result += NUMERIC(digit * factor);
			cp++;
		}
	}

	if( ansiToUpper( c ) == 'E' )
	{
		cp++;
		int exponent = getIntegerValue<int>( cp, 10, &cp );
		if( exponent )
		{
			result *= NUMERIC(pow( 10.0, exponent ));
		}
	}

	*end = cp;
	if( !isNegative )
	{
		return result;
	}
	else
	{
		return NUMERIC(0U-result);
	}
}

/**
	parse string, return float or throw exception
*/
template <typename NUMERIC>
NUMERIC getFloatValue( const char *cp )
{
	const char * const start = cp;
	bool			isNegative = false;
	long double		result = 0;
	unsigned		digit;
	unsigned char	c = *cp;

	if( c == '+' )
	{
		cp++;
	}
	else if( c == '-' )
	{
		isNegative = true;
		cp++;
	}
	while( (c = *cp++) != 0 && c != '.' && c != ',' && ansiToUpper( c ) != 'E' )
	{
		 if( c >= '0' && c <= '9' )
		 {
			digit = c - '0';
		}
		else
		{
/*@*/		throw BadNumericFormatError( start );
		}

		result *= 10.0;
		result += digit;
	}

	if( c == '.' || c == ',' )
	{
		double factor = 1;
		while( (c = *cp++) != 0 && ansiToUpper( c ) != 'E' )
		{
			 if( c >= '0' && c <= '9' )
			 {
				digit = c - '0';
			}
			else
			{
/*@*/			throw BadNumericFormatError( start );
			}

			factor /= 10.0;
			result += digit * factor;
		}
	}

	if( ansiToUpper( c ) == 'E' )
	{
		int exponent = getIntegerValue<int>( cp, 10 );
		if( exponent > std::numeric_limits<NUMERIC>::max_exponent10 )
		{
/*@*/		throw ExponentOverflowError( start );
		}
		if( exponent < std::numeric_limits<NUMERIC>::min_exponent10 )
		{
/*@*/		throw ExponentUnderflowError( start );
		}

		result *= pow( 10.0, exponent );
	}
	#pragma warning ( suppress: 4804 )	// unsichere Verwendung the Typs bool
	if( result > std::numeric_limits<NUMERIC>::max() )
	{
		if( !isNegative )
		{
/*@*/		throw FloatOverflowError( start );
		}
		else
		{
/*@*/		throw FloatUnderflowError( start );
		}
	}

	if( !isNegative )
	{
		return NUMERIC(result);
	}
	else
	{
		return NUMERIC(-result);
	}
}

/**
	parse string, return integer, bool or float or throw exception
*/
template <typename NUMERIC>
inline NUMERIC getValue( const char *cp, unsigned base )
{
	assert( cp );
	assert( *cp );

	if( std::numeric_limits<NUMERIC>::is_integer && std::numeric_limits<NUMERIC>::digits == 1 )
	{
		if( !strcmpi( cp, "T" ) || !strcmpi( cp, "TRUE" ) )
		{
			return true;
		}
		if( !strcmpi( cp, "F" ) || !strcmpi( cp, "FALSE" ) )
		{
			return false;
		}
		return NUMERIC( getValue<int>(cp,base) );
	}
	else if( std::numeric_limits<NUMERIC>::max_exponent10 )
	{
		return getFloatValue<NUMERIC>( cp );
	}
	else if( std::numeric_limits<NUMERIC>::is_integer )
	{
		return getIntegerValue<NUMERIC>( cp, base );
	}
	else
	{
		throw InvalidConversionError();
	}
}

/**
	parse string, return integer, float and bad position
*/
template <typename NUMERIC>
inline NUMERIC getValue( const char *cp, unsigned base, const char **end )
{
	if( std::numeric_limits<NUMERIC>::max_exponent10 )
	{
		return internal::getFloatValue<NUMERIC>( cp, end );
	}
	else if( std::numeric_limits<NUMERIC>::is_integer )
	{
		return internal::getIntegerValue<NUMERIC>( cp, base, end );
	}
	else
	{
		throw InvalidConversionError();
	}
}

#ifdef __BORLANDC__
#	pragma warn +rch
#	pragma warn +ccc
#	pragma warn +inl
#endif

}	// namespace internal
/// @endcond

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
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief parses a c-style string and returns the numeric value
	@param [in] cp the c-string containg a numeric value
	@param [in] base the numeric base
	@tparam NUMERIC a numeric type or bool
	@return the numeric value
	@exception MathError in case of a conversion error
	@exception InvalidConversionError if the type is supported
	@see getValueN
*/
template <typename NUMERIC>
inline NUMERIC getValueE( const char *cp, unsigned base=10 )
{
	if( !cp )
	{
/*@*/	throw BadNumericFormatError( LibraryException::NULL_STRING_ERROR );
	}
	if( !*cp )
	{
/*@*/	throw BadNumericFormatError( LibraryException::EMPTY_STRING_ERROR );
	}

	return internal::getValue<NUMERIC>( cp, base );
}

/**
	@brief parses a c-style string and returns the numeric value
	@param [in] cp the c-string containg a numeric value
	@param [in] base the numeric base
	@tparam NUMERIC a numeric type or bool
	@return if cp is not defined 0 otherwise the numeric value
	@exception MathError in case of a conversion error
	@exception InvalidConversionError if the type is supported
	@see getValueE
*/
template <typename NUMERIC>
inline NUMERIC getValueN( const char *cp, unsigned base=10 )
{
	if( !cp || !*cp )
	{
		return NUMERIC(0);
	}

	return internal::getValue<NUMERIC>( cp, base );
}

/**
	@brief parses a c-style string and returns the numeric value
	@param [in] cp the c-string containg a numeric value
	@param [in] base the numeric base
	@param [out] end the address of the first character not parsed
	@tparam NUMERIC a numeric type (char, int, float, double)
	@return if cp is not defined 0 otherwise the numeric value
	@exception InvalidConversionError if the type is not a numeric type
*/
template <typename NUMERIC>
inline NUMERIC getValue( const char *cp, unsigned base=10, const char **end=NULL )
{
	return internal::getValue<NUMERIC>( cp, base, end );
}

/**
	@brief parses a c-style string with a base value of 10 and returns the numeric value
	@param [in] cp the c-string containg a numeric value
	@param [out] end the address of the first character not parsed
	@tparam NUMERIC a numeric type (char, int, float, double)
	@return if cp is not defined 0 otherwise the numeric value
	@exception InvalidConversionError if the type is not a numeric type
*/
template <typename NUMERIC>
inline NUMERIC getValue( const char *cp, const char **end )
{
	return getValue<NUMERIC>( cp, 10, end );
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename NUMERIC>
inline NUMERIC STRING::getValueE(unsigned base) const
{
	return gak::getValueE<NUMERIC>(*this, base);
}

template <typename NUMERIC>
inline NUMERIC STRING::getValueN(unsigned base) const
{
	return gak::getValueN<NUMERIC>(*this, base);
}



} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
