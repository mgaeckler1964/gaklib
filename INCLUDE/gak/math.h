/*
		Project:		GAKLIB
		Module:			math.h
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

#ifndef GAK_MATH_H
#define GAK_MATH_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <math.h>		// otherwise i will loose isnan on Mac
#include <iostream>

#include <gak/fixedArray.h>

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
namespace math
{
// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

const std::size_t	numPrimeFactors = 65;		// enough for 64 bit systems

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

#undef max
#undef min

template <class T>	
inline T max(T x, T y)
{
	return (x > y) ? x : y;
};

template <class T>	
inline T min(T x, T y)
{
	return (x < y) ? x : y;
};

template<typename T>
inline T medium( T a, T b )
{
	return (a+b)/2;
}
	 
template<typename T>
inline T abs( T a )
{
	return a < 0 ? -a : a;
}

template<typename T>
inline T sign( T a )
{
	return a < 0 ? -1 : (a == 0 ? 0 : +1);
}

template<typename ValueT>
inline int getExponent( ValueT value )
{
	ValueT	numDigits = value ? log10( abs( value ) ) : 0;

	return int( floor(numDigits) );
}

template<typename ValueT>
inline ValueT normalize( ValueT value, int *exponent )
{
	int			localExponent = getExponent( value );
	ValueT		factor = pow( 10., -localExponent );

	*exponent = localExponent;
	return value * factor;
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef FixedArray<unsigned long, numPrimeFactors>	PrimeFactorBuffer;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct MixedFraction;
struct Fraction
{
	long			numerator;
	unsigned long	denominator;

	explicit Fraction( long newNumerator, unsigned long newDenominator=1 )
		: numerator(newNumerator), denominator(newDenominator)
	{
	}
	Fraction( const MixedFraction &src )
	{
		readMixedFraction( src );
	}
	const Fraction &operator = ( const MixedFraction &src )
	{
		readMixedFraction( src );
		return *this;
	}


	Fraction &reduce();
	Fraction reduceCopy() const
	{
		Fraction	result = *this;

		return result.reduce();
	}

	Fraction &reziprok()
	{
		long tmp = numerator;
		numerator = denominator;
		denominator = abs( tmp );
		if( tmp < 0 )
			numerator *= -1;

		return *this;
	}

	Fraction reziprokCopy() const
	{
		Fraction tmp = *this;

		return tmp.reziprok();
	}

	operator double ( void ) const
	{
		return double(numerator) / double(denominator);
	}

	bool operator == ( const Fraction &right ) const
	{
		return numerator == right.numerator && denominator == right.denominator;
	}

	private:
	void readMixedFraction( const MixedFraction &src );
};

struct MixedFraction
{
	long		integer;
	Fraction	fraction;

	MixedFraction( long newInteger, long newNumerator, unsigned long newDenominator )
		: integer(newInteger), fraction(newNumerator, newDenominator)
	{
	}
	MixedFraction( long newNumerator, unsigned long newDenominator )
		: integer(0), fraction(newNumerator, newDenominator)
	{
	}
	MixedFraction( long newInteger )
		: integer(newInteger), fraction(0)
	{
	}
	MixedFraction( const Fraction &src )
		: integer(src.numerator/long(src.denominator)), fraction(src.numerator%long(src.denominator), src.denominator)
	{
	}
	const MixedFraction &operator = ( const Fraction &src )
	{
		readFraction( src );
		return *this;
	}


	MixedFraction &reduce()
	{
		Fraction	tmpFraction = static_cast<Fraction>(*this);

		*this = tmpFraction.reduce();

		return *this;
	}
	MixedFraction reduceCopy() const
	{
		MixedFraction	result = *this;
		result.reduce();
		return result;
	}

	MixedFraction &reziprok()
	{
		Fraction	tmpFraction = static_cast<Fraction>(*this);

		*this = tmpFraction.reziprok();

		return *this;
	}
	MixedFraction reziprokCopy() const
	{
		MixedFraction	result = *this;
		result.reziprok();
		return result;
	}

	operator double ( void ) const
	{
		return double(integer) + double(fraction);
	}

	bool operator == ( const MixedFraction &right ) const
	{
		return integer == right.integer && fraction == right.fraction;
	}

	private:
	void readFraction( const Fraction &src )
	{
		integer = src.numerator/long(src.denominator);
		fraction.numerator = src.numerator%long(src.denominator);
		fraction.denominator = src.denominator;
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

bool isPrime( unsigned long n );
unsigned long getNextPrime( unsigned long n );
PrimeFactorBuffer getPrimeFactors( unsigned long n );
unsigned long kgV( unsigned long n1, unsigned long n2 );
unsigned long ggT( unsigned long n1, unsigned long n2 );

Fraction operator + ( const Fraction &n1, const Fraction &n2 );
Fraction operator - ( const Fraction &n1, const Fraction &n2 );
Fraction operator * ( const Fraction &n1, const Fraction &n2 );
Fraction operator / ( const Fraction &n1, const Fraction &n2 );

MixedFraction operator + ( const MixedFraction &n1, const MixedFraction &n2 );
MixedFraction operator - ( const MixedFraction &n1, const MixedFraction &n2 );
MixedFraction operator * ( const MixedFraction &n1, const MixedFraction &n2 );
MixedFraction operator / ( const MixedFraction &n1, const MixedFraction &n2 );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

inline void Fraction::readMixedFraction( const MixedFraction &src )
{
	numerator = src.integer * src.fraction.denominator + src.fraction.numerator;
	denominator = src.fraction.denominator;
}

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

inline std::ostream &operator << ( std::ostream &out, const Fraction &x )
{
	out << x.numerator << '/' << x.denominator;

	return out;
}

inline std::ostream &operator << ( std::ostream &out, const MixedFraction &x )
{
	out << x.integer << ' ' << x.fraction;

	return out;
}

}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_MATH_H