/*
		Project:		GAKLIB
		Module:			fmtNumber.h
		Description:	format numbers
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

#ifndef FORMATING_STRING_H
#define FORMATING_STRING_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>

#include <gak/types.h>
#include <gak/string.h>
#include <gak/stack.h>
#include <gak/math.h>

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

STRING formatFloat( double value, int fieldLength=0, int precision=-1, char thousand=0 );
STRING formatBool( bool value );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

/// @cond
namespace internal
{
	template<typename ValueT>
	inline ValueT modulo( ValueT left, ValueT right )
	{
		return ValueT( left % right );
	}

	template<>
	inline double modulo( double left, double right )
	{
		return fmod(left, right);
	}

	template <class UNSIGNED_T> 
	STRING formatUnsigned(
		UNSIGNED_T value, int fieldLength=0, char filler='0', char thousand=0
	)
	{
		size_t		numDigits = 0;
		Stack<char>	tmp;
		STRING		result;

		result.setMinSize( fieldLength );
		do
		{
			tmp.push( char(modulo( value, UNSIGNED_T(10) ) + '0') );
			value = UNSIGNED_T(value / 10);
			if( thousand && value )
			{
				numDigits++;
				if( !(numDigits % 3) )
				{
					tmp.push( thousand );
				}
			}
		} while( value >= 1 );

		int count = int(fieldLength-tmp.size()); 
		if( count>0 )
		{
			result.addCharStr( filler, count );
		}
		else
		{
			result.setMinSize( tmp.size() );
		}
		while( tmp.size() )
		{
			result += tmp.pop();
		}

		return result;
	}

	template <class NUMBER_T>
	STRING formatNumber(
		NUMBER_T value, int fieldLength=0, char filler='0', char thousand=0
	)
	{
	#if defined( __BORLANDC__ )
	#	pragma warn -8008
	#	pragma warn -8027
	#	pragma warn -8041
	#	pragma warn -8066
	#elif defined _MSC_VER
	//#	pragma warning ( suppress: 4146 )
	#endif

		if( value >= 0 )
		{
			return formatUnsigned(  value, fieldLength, filler, thousand );
		}
		else
		{
			if( filler == '0' || !filler )
			{
				return '-' + formatUnsigned( 
					NUMBER_T(value * (-1)), fieldLength-1, filler, thousand 
				);
			}
			else
			{
				STRING	result = '-' + formatUnsigned( 
					NUMBER_T(value * (-1)), 0, 0, thousand 
				);
				int		count = int(fieldLength-result.strlen());
				if( count >  0 )
				{
					result.insChar( 0, filler, count );
				}

				return result;
			}
		}

	#if defined( __BORLANDC__ )
	#	pragma warn .8008
	#	pragma warn .8027
	#	pragma warn .8041
	#	pragma warn .8066
	#endif
	}

	template <class NUMBER_T>
	STRING formatFraction( NUMBER_T value, int precision )
	{
		assert( precision != 0 );

		STRING	result;
		int		exponent;
		int		maxCount = std::numeric_limits<NUMBER_T>::digits10;
		value = fabs( value );
		bool	countZero = (value >= 1);
		value -= floor( value );

		value = math::normalize( value, &exponent );
		if( value )
		{
			result = '.';
			int		numZeros = -exponent -1;

			if( precision < 0 )
			{
				value += 5*pow( 10.0, double(-maxCount) );
				if( !countZero )
				{
					maxCount += numZeros;
				}
			}
			else
			{
				maxCount = precision;
			}

			for( int i=0; i<numZeros && maxCount > 0; ++i )
			{
				--maxCount;
				result += '0';
			}
			while( value && maxCount > 0 )
			{
				--maxCount;

				result += '0' + int(value);
				value -= floor( value );
				value *= 10;
			}

			if( precision < 0 )
			{
				result.stripRightChar( '0' );
				result.stripRightChar( '.' );
			}
		}
		return result;
	}
}
/// @endcond

template <class NUMBER_T>
STRING formatBinary(
	NUMBER_T value, size_t radix,
	unsigned fieldLength=0, char filler='0'
)
{
	Stack<char>	tmp;
	STRING		result;

	if( std::numeric_limits<NUMBER_T>::is_signed && value < 0 )
	{
		result = "-";
		fieldLength--;
	}
	do
	{
		unsigned char digit;

		if( value < 0 )
		{
			digit = (unsigned char)(-value%radix);
			value = -NUMBER_T(-value / radix);
		}
		else
		{
			digit = (unsigned char)(value%radix);
			value = NUMBER_T(value / radix);
		}

		if( digit < 10 )
			digit += '0';
		else if( digit < 36 )
			digit += 'A' - 10;
		else if( digit < 62 )
			digit += 'a' - 36;
		else
			digit = '?';

		tmp.push( digit );
	} while( value );

	for( int i=int(fieldLength-tmp.size()); i>0; i-- )
		result += filler;

	while( tmp.size() )
	{
		result += tmp.pop();
	}

	return result;
}

template <>
inline STRING formatBinary<void*>(
	void *value, size_t radix,
	unsigned fieldLength, char filler
)
{
	return formatBinary<size_t>( size_t( value ), radix, fieldLength, filler );
}

template <class NUMBER_T>
inline STRING formatNumber(
	NUMBER_T value, int fieldLength=0, char filler='0', char thousand=0
)
{
	return internal::formatNumber( value, fieldLength, filler, thousand );
}

template <>
inline STRING formatNumber(
	bool value, int fieldLength, char filler, char thousand
)
{
	return internal::formatUnsigned( value ? 1U : 0U, fieldLength, filler, thousand );
}

template <>
inline STRING formatNumber<>(
	void *value, int fieldLength, char filler, char /* thousand */
)
{
#if defined( __BORLANDC__ )
#	pragma warn -8027
#endif
	return "0x" + formatBinary( value, 16, fieldLength, filler );
#if defined( __BORLANDC__ )
#	pragma warn -8027
#endif
}

template <>
inline STRING formatNumber<>(
	float value, int fieldLength, char /* filler */, char /* thousand */
)
{
	return formatFloat( value, fieldLength );
}

template <>
inline STRING formatNumber<>(
	double value, int fieldLength, char /* filler */, char /* thousand */
)
{
	return formatFloat( value, fieldLength );
}

template <>
inline STRING formatNumber<>(
	long double value, int fieldLength, char /* filler */, char /* thousand */
)
{
	return formatFloat( double(value), fieldLength );
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
