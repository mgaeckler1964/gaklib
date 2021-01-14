/*
		Project:		GAKLIB
		Module:			fraction.cpp
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

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <gak/math.h>

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

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

Fraction &Fraction::reduce()
{
	unsigned long reduce = ggT( abs(numerator), denominator );
	numerator /= reduce;
	denominator /= reduce;

	return *this;
}

Fraction operator + ( const Fraction &n1, const Fraction &n2 )
{
	unsigned long	newDenominator = kgV( n1.denominator, n2.denominator );

	return Fraction(
		n1.numerator * newDenominator/n1.denominator + n2.numerator * newDenominator/n2.denominator, 
		newDenominator
	).reduce();
}

Fraction operator - ( const Fraction &n1, const Fraction &n2 )
{
	unsigned long	newDenominator = kgV( n1.denominator, n2.denominator );

	return Fraction(
		n1.numerator * newDenominator/n1.denominator - n2.numerator * newDenominator/n2.denominator, 
		newDenominator
	).reduce();
}

Fraction operator * ( const Fraction &n1, const Fraction &n2 )
{
	return Fraction(
		n1.numerator * n2.numerator,
		n1.denominator * n2.denominator
	).reduce();
}

Fraction operator / ( const Fraction &n1, const Fraction &n2 )
{
	return n1 * n2.reziprokCopy();
}

MixedFraction operator + ( const MixedFraction &n1, const MixedFraction &n2 )
{
	return MixedFraction(static_cast<Fraction>(n1) + static_cast<Fraction>(n2));
}

MixedFraction operator - ( const MixedFraction &n1, const MixedFraction &n2 )
{
	return MixedFraction(static_cast<Fraction>(n1) - static_cast<Fraction>(n2));
}

MixedFraction operator * ( const MixedFraction &n1, const MixedFraction &n2 )
{
	return MixedFraction(static_cast<Fraction>(n1) * static_cast<Fraction>(n2));
}

MixedFraction operator / ( const MixedFraction &n1, const MixedFraction &n2 )
{
	return MixedFraction(static_cast<Fraction>(n1) / static_cast<Fraction>(n2));
}


}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

