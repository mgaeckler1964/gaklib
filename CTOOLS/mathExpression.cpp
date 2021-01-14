/*
		Project:		GAKLIB
		Module:			mathExpression.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <csignal>
// #include <float.h>

#include <gak/expressionEvaluator.h>
#include <gak/numericString.h>

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

static void FPUsignal( int /* sig */ )
{
	throw InvalidResultError();
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
   
void MathExpression::fillOperators( Array<ExprOperator>	*operators )
{
	NumericEvaluator<double>::fillOperators( operators );

	static ExprOperator	mathOperators[] =
	{
		{ "!",  1, 0,	ASSOC_LEFT,  NULL, UnaryFunc(&MathExpression::faculty), NULL },
		{ "^",  1, 50,	ASSOC_RIGHT, NULL, NULL,								BinaryFunc(&MathExpression::power) },
		{ ":",  1, 100, ASSOC_LEFT,  NULL, NULL,								BinaryFunc(&NumericEvaluator<double>::division) },
	};

	operators->addElements( mathOperators, 3 );
}

double MathExpression::evaluateFunction( const STRING &funcName, const Array<double> &parameterList )
{
	CI_STRING	myName = funcName;
	double	funcParam = parameterList.size() ? parameterList[0] : 0;
	double	result;

	if( myName == "sin" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = sin( funcParam );
	}
	else if( myName == "asin" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = asin( funcParam );
	}
	else if( myName == "cos" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = cos( funcParam );
	}
	else if( myName == "acos" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = acos( funcParam );
	}
	else if( myName == "tan" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = tan( funcParam );
	}
	else if( myName == "atan" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = atan( funcParam );
	}
	else if( myName == "ln" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = log( funcParam );
	}
	else if( myName == "log" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = log10( funcParam );
	}
	else if( myName == "ceil" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = ceil( funcParam );
	}
	else if( myName == "floor" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = floor( funcParam );
	}
	else if( myName == "round" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		if( funcParam >= 0 )
			result = floor( funcParam + 0.5 );
		else
			result = ceil( funcParam - 0.5 );
	}
	else if( myName == "sqrt" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = sqrt( funcParam );
	}
	else if( myName == "abs" )
	{
		if( parameterList.size() != 1 )
			throw IllegalNumberOfParamsError();
		result = fabs( funcParam );
	}
	else if( myName == "pi" )
	{
		if( parameterList.size() != 0 )
			throw IllegalNumberOfParamsError();
		result = M_PI;
	}
	else if( myName == "e" )
	{
		if( parameterList.size() != 0 )
			throw IllegalNumberOfParamsError();
		result = M_E;
	}
	else
		throw UnknownFunctionError();


	return checkResult( result );
}

double MathExpression::evaluateConstant( const STRING &constExpression )
{
	double result;

	if( isDefined( constExpression ) )
	{
		result = ExpressionEvaluator<double>::evaluateConstant( constExpression );
	}
	else
	{
		result = constExpression.getValueE<double>();
	}

	return result;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

double MathExpression::faculty( double oper )
{
	double	result = 1;
	int		maxI = int(oper+0.5);

	for( int i = 2; i<=maxI; i++ )
		result *= double(i);

	return checkResult( result );
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#if defined( __BORLANDC__ ) || defined( _MSC_VER )
/*
 *	BC++ needs this error handling too. This function is called by the
 *  floating point library.
 */
extern "C" int _matherr(struct _exception * /* a */ )
{
#if 0
	const char	*errTxt;

	switch( a->type )
	{
		case DOMAIN:
		case SING:		errTxt = "Nan";				break;
		case OVERFLOW:	errTxt = "+Inf";			break;
		case UNDERFLOW:	errTxt = "-Inf";			break;
		case TLOSS:		errTxt = "Inexact result";	break;
		default:		errTxt = "General Error";	break;
	}
#endif
	throw InvalidResultError();
}
#endif

void initFPUsignal( void )
{
	static bool first = true;

	if( first )
	{
		std::signal( SIGFPE, FPUsignal );
		first = false;
	}
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

