/*
		Project:		GAKLIB
		Module:			prinme.cpp
		Description:	Prime numbers
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

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <gak/gaklib.h>
#include <gak/math.h>
#include <gak/logfile.h>

/* --------------------------------------------------------------------- */
/* ----- module switches ----------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{
namespace math
{

/* --------------------------------------------------------------------- */
/* ----- constants ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- macros -------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- prototypes ---------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- imported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module statics ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- exported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

static unsigned long getMinPrimeFactor( unsigned long startValue, unsigned long n )
{
	unsigned long primeFactor = startValue;

	while( primeFactor < n && n % primeFactor != 0 )
		primeFactor = getNextPrime( primeFactor );

	return primeFactor;
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

bool isPrime( unsigned long n )
{
	unsigned long	maxNum, i;

	if( n < 2 )
		return false;
	if( n == 2 )
		return true;

	maxNum = static_cast<unsigned long>(std::sqrt( double(n) ) +1);
	for( i=2; i<=maxNum; i++ )
		if( n%i == 0 )
			return false;

	return true;
}

unsigned long getNextPrime( unsigned long n )
{
	while( ++n )
	{
		if( isPrime( n ) )
			break;
	}

	return n;
}

PrimeFactorBuffer getPrimeFactors( unsigned long n )
{
	PrimeFactorBuffer	primeFactors;
	doEnterFunction("getPrimeFactors");

	if( n<=2 )
	{
		primeFactors[0] = n;
		primeFactors[1] = 0;
	}
	else
	{
		size_t i=0;
		unsigned long primeFactor = 2;
		while( n >= 2 )
		{
			primeFactor = getMinPrimeFactor( primeFactor, n );
			primeFactors[i++] = primeFactor;
			n = n/primeFactor;
		}
		primeFactors[i] = 0;
	}

	return primeFactors;
}

unsigned long kgV( unsigned long n1, unsigned long n2 )
{
	unsigned long kgVresult = 1;

	PrimeFactorBuffer pf1 = getPrimeFactors( n1 );
	PrimeFactorBuffer pf2 = getPrimeFactors( n2 );

	PrimeFactorBuffer::const_iterator pfPtr1 = pf1.cbegin();
	PrimeFactorBuffer::const_iterator pfPtr2 = pf2.cbegin();

	while( *pfPtr1 || *pfPtr2 )
	{
		unsigned long p1 = *pfPtr1;
		unsigned long p2 = *pfPtr2;
		if( p1 == p2 )
		{
			kgVresult *= p1;
			pfPtr1++;
			pfPtr2++;
		}
		else if( p1 && (p1 < p2 || !p2) )
		{
			kgVresult *= p1;
			pfPtr1++;
		}
		else if( p2 )
		{
			kgVresult *= p2;
			pfPtr2++;
		}
	}

	return kgVresult;
}

unsigned long ggT( unsigned long n1, unsigned long n2 )
{
	unsigned long kgVresult = 1;

	PrimeFactorBuffer pf1 = getPrimeFactors( n1 );
	PrimeFactorBuffer pf2 = getPrimeFactors( n2 );

	PrimeFactorBuffer::const_iterator pfPtr1 = pf1.cbegin();
	PrimeFactorBuffer::const_iterator pfPtr2 = pf2.cbegin();


	while( *pfPtr1 && *pfPtr2 )
	{
		unsigned long p1 = *pfPtr1;
		unsigned long p2 = *pfPtr2;
		if( p1 == p2 )
		{
			kgVresult *= p1;
			pfPtr1++;
			pfPtr2++;
		}
		else if( p1 < p2 )
		{
			pfPtr1++;
		}
		else
		{
			pfPtr2++;
		}
	}

	return kgVresult;
}

}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

