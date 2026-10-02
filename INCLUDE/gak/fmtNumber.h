/*
		Project:		GAKLIB
		Module:			fmtNumber.h
		Description:	format numbers
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

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4127 )	// Bedingter Ausdruck ist konstant
#	pragma warning ( disable: 4996 )	// 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead.

#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const size_t NUMBER_BUFFER_WIDTH=128;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

inline const char *formatBool( bool value )
{
	return value ? "true" : "false";
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <size_t BUFFER_SIZE>
class BaseBuffer
{
	protected:
	char	m_buffer[BUFFER_SIZE+1];	// let there be space for the 0 byte
	size_t	m_len;

	public:
	BaseBuffer() : m_len(0) {}

	// adding a character
	BaseBuffer &addDigit( char digit )
	{
		m_buffer[m_len++] = digit;
		return *this;
	}
	BaseBuffer &addDigit( char digit, size_t count )
	{
		for( size_t i=0; i<count; ++i )
			m_buffer[m_len++] = digit;
		return *this;
	}
	BaseBuffer &insDigit( char digit, size_t startPos, size_t count )
	{
		char *start = m_buffer+startPos;
		char *target = start+count;
		memmove( target, start, m_len );
		for( char *cp=start; cp<target; ++cp )
			*cp = digit;
		m_len += count;
		return *this;
	}
	

	BaseBuffer &operator += ( char digit )
	{
		addDigit( digit );
		return *this;
	}

	// adding a C-string
	protected:
	BaseBuffer &addCP ( const char *cp, size_t len )
	{
		strncpy( m_buffer+m_len, cp, len );
		m_len += len;

		return *this;
	}

	public:
	BaseBuffer &addCP ( const char *cp )
	{
		return addCP(cp, strlen(cp));
	}
	template <typename T, size_t N>
	BaseBuffer &add (const T (&arr)[N])
	{
		addCP( arr, N-1 );

		return *this;
	}
	template <typename T, size_t N>
	BaseBuffer &operator += (const T (&arr)[N])
	{
		addCP( arr, N-1 );

		return *this;
	}

	BaseBuffer &addBB ( const BaseBuffer &bb )
	{
		return addCP( bb.m_buffer, bb.m_len );
	}

#ifdef __BORLANDC__
	BaseBuffer &add( const char *arr )
	{
		addCP( arr, strlen(arr) );
		return *this;
	}
	BaseBuffer &operator += (const char *arr)
	{
		addCP( arr, strlen(arr) );
		return *this;
	}
#endif

	BaseBuffer &stripRight( int digit )
	{
		if( m_len > 0)
		{
			const char *cp = m_buffer+m_len-1;
			while( *cp == digit )
			{
				--cp;
				--m_len;
			}
		}
		return *this;
	}
	const char *c_str()
	{
		m_buffer[m_len]=0;
		return m_buffer;
	}
	size_t size() const
	{
		return m_len;
	}
	size_t remain() const
	{
		return BUFFER_SIZE - size();
	}

	BaseBuffer &clear()
	{
		m_len = 0;
		return *this;
	}

	void assertAdd( size_t 
#if !defined( NDEBUG ) || defined( _DEBUG )
		additional 
#endif
	)
	{
		assert( additional <= remain() );
	}
};

typedef BaseBuffer<NUMBER_BUFFER_WIDTH>	NumberBuffer;

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

STRING formatFloat( double value, int fieldLength=0, int precision=-1, char thousand=0, char decPoint='.' );

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

	template <typename NUMBER_BUFFER_T, typename UNSIGNED_T> 
	void formatUnsigned2(
		NUMBER_BUFFER_T *result, UNSIGNED_T value, int fieldLength, char filler, char thousand
	)
	{
		size_t													numDigits = 0;
		Stack<char, Fixed4Stack<char, NUMBER_BUFFER_WIDTH> >	tmp;

		do
		{
			tmp.push( char(modulo( value, UNSIGNED_T(10) ) + '0') );
			value = UNSIGNED_T(value / 10);
			if( thousand && value )
			{
				numDigits++;
				if( !(numDigits % 3) && value >= 1 )
				{
					tmp.push( thousand );
				}
			}
		} while( value >= 1 );
		if( fieldLength > 0 && filler )
		{
			if(fieldLength>NUMBER_BUFFER_WIDTH)
			{
				fieldLength=NUMBER_BUFFER_WIDTH;
			}
			while(tmp.size() < size_t(fieldLength))
			{
				tmp.push( filler );
			}
		}
		while( tmp.size() )
		{
			char c = tmp.pop();
			if( c && (c != thousand || result->size()) )
			{
				(*result) += c;
			}
		}
	}

	template <typename NUMBER_BUFFER_T, class UNSIGNED_T> 
	void formatUnsigned(
		NUMBER_BUFFER_T *result, UNSIGNED_T value, int fieldLength, char filler, char thousand
	)
	{
		formatUnsigned2(result, value, fieldLength, filler, thousand);

		int count = int(fieldLength-result->size()); 
		if( count>0 )
		{
			result->insDigit( filler, 0, count );
		}
		return;
	}

	template <typename NUMBER_BUFFER_T, class NUMBER_T>
	void formatNumber2(
		NUMBER_BUFFER_T *result, NUMBER_T value, int fieldLength, char filler, char thousand
	)
	{
	#if defined( __BORLANDC__ )
	#	pragma warn -8008
	#	pragma warn -8027
	#	pragma warn -8041
	#	pragma warn -8066
	#endif

		if( value >= 0 )
		{
			formatUnsigned( result, value, fieldLength, filler, thousand );
			return;
		}
		else
		{
			NUMBER_BUFFER_T	tmp;
			if( filler == '0' || !filler )
			{
				formatUnsigned( &tmp, NUMBER_T(value * (-1)), fieldLength-1, filler, thousand );
				result->addDigit('-').addBB( tmp );
				return;
			}
			else
			{
				formatUnsigned( &tmp, NUMBER_T(value * (-1)), 0, 0, thousand );
				result->clear();

				int		count = int(fieldLength-tmp.size())-1;
				if( count >  0 )
				{
					result->addDigit( filler, count );
				}
				result->addDigit( '-' ).addBB(tmp);
			}
		}

	#if defined( __BORLANDC__ )
	#	pragma warn .8008
	#	pragma warn .8027
	#	pragma warn .8041
	#	pragma warn .8066
	#endif
	}

	template <typename NUMBER_BUFFER_T, class NUMBER_T>
	void formatFraction( NUMBER_BUFFER_T *result, NUMBER_T value, int precision, char decPoint )
	{
		assert( precision != 0 );

		int		exponent;
		int		maxCount = std::numeric_limits<NUMBER_T>::digits10;
		value = fabs( value );
		bool	countZero = (value >= 1);
		value -= floor( value );

		result->clear();
		result->addDigit( decPoint );
		value = math::normalize( value, &exponent );
		if( value )
		{
			int		numZeros = -exponent -1;

			if( precision < 0 )	// do we need all fraction digits?
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
				result->addDigit( '0' );
			}
			while( value && maxCount > 0 )
			{
				--maxCount;

				result->addDigit( char('0' + int(value)) );
				value -= floor( value );
				value *= 10;
			}

			if( precision < 0 )	// do we need all fraction digits?
			{
				result->stripRight( '0' )
					.stripRight( '.' );
			}
		}
		else if( precision > 0 )
		{
			result->addDigit('0', precision);
		}
	}
}
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
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	-------------------------------------------------------------------------------------------------
		formatBinary with specialisations
	-------------------------------------------------------------------------------------------------
*/
template <typename NUMBER_T>
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

		if( std::numeric_limits<NUMBER_T>::is_signed && value < 0 )
		{
#if defined _MSC_VER
	#	pragma warning ( disable: 4146 )
#endif
			digit = static_cast<unsigned char>(-value%radix);
			value = -NUMBER_T(-value / radix);
#if defined _MSC_VER
	#	pragma warning ( default: 4146 )
#endif
		}
		else
		{
			digit = static_cast<unsigned char>(value%radix);
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

/*
	-------------------------------------------------------------------------------------------------
		formatNumber with specialisations
	-------------------------------------------------------------------------------------------------
*/
template <typename NUMBER_T>
inline STRING formatNumber(
	NUMBER_T value, int fieldLength=0, char filler='0', char thousand=0, char /* decPoint */ ='.'
)
{
	NumberBuffer result;
	internal::formatNumber2( &result, value, fieldLength, filler, thousand );
	return STRING( result.c_str(), result.size() );
}

template <>
inline STRING formatNumber(
	bool value, int fieldLength, char filler, char thousand, char /* decPoint */
)
{
	NumberBuffer result;
	internal::formatUnsigned( &result, value ? 1U : 0U, fieldLength, filler, thousand );
	return STRING( result.c_str(), result.size() );
}

template <>
inline STRING formatNumber<>(
	void *value, int fieldLength, char filler, char /* thousand */, char /* decPoint */
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
	float value, int fieldLength, char /* filler */, char thousand, char decPoint
)
{
	return formatFloat( value, fieldLength, -1, thousand, decPoint );
}

template <>
inline STRING formatNumber<>(
	double value, int fieldLength, char /* filler */, char thousand, char decPoint
)
{
	return formatFloat( value, fieldLength, -1, thousand, decPoint );
}

template <>
inline STRING formatNumber<>(
	long double value, int fieldLength, char /* filler */, char thousand, char decPoint
)
{
	return formatFloat( double(value), fieldLength, -1, thousand, decPoint );
}

template <typename NUMBER_BUFFER_T>
const char *appendFloatFast( NUMBER_BUFFER_T *result, double value, int fieldLength=0, int precision=-1, char thousand=0, char decPoint='.' )
{
	size_t startPos = result->size();
	result->assertAdd( fieldLength );
	if( precision >= 0 )
	{
		if( value > 0 )
		{
			value += 5*pow( 10.0, double(-precision-1) );
		}
		else if( value < 0 )
		{
			value -= 5*pow( 10.0, double(-precision-1) );
		}
	}

	internal::formatNumber2( result, value, 0, 0, thousand );

	if( precision != 0 )
	{
		NUMBER_BUFFER_T	tmp;
		internal::formatFraction( &tmp, value, precision, decPoint );
		result->addBB( tmp );
	}

	if( startPos )
		fieldLength += int(startPos);
	if( int(result->size()) < fieldLength )
	{
		result->insDigit( ' ', startPos, fieldLength - result->size() );
	}
	return result->c_str();
}

template <typename NUMBER_BUFFER_T>
const char *formatFloatFast( NUMBER_BUFFER_T *result, double value, int fieldLength=0, int precision=-1, char thousand=0, char decPoint='.' )
{
	result->clear();
	result->assertAdd( fieldLength );
	return appendFloatFast( result, value, fieldLength, precision, thousand, decPoint );
}

template <typename NUMBER_BUFFER_T, typename NUMBER_T>
inline const char * appendNumberFast(
	NUMBER_BUFFER_T *result, NUMBER_T value, int fieldLength=0, char filler=0, char thousand=0
)
{
	result->assertAdd( fieldLength );
	if( value < 0 )
	{
		result->addDigit('-');
		--fieldLength;
		value = -value;
	}
	internal::formatUnsigned2( result, value, fieldLength, filler, thousand );
	return result->c_str();
}

template <typename NUMBER_BUFFER_T, typename NUMBER_T>
inline const char * formatNumberFast(
	NUMBER_BUFFER_T *result, NUMBER_T value, int fieldLength=0, char filler=0, char thousand=0
)
{
	result->clear();
	result->assertAdd( fieldLength );
	return appendNumberFast(result, value, fieldLength, filler, thousand);
}


}	// namespace gak

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
