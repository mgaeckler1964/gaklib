/*
		Project:		GAKLIB
		Module:			expressionEvaluator.h
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

#ifndef GAK_EXPRESSION_EVAL_H
#define GAK_EXPRESSION_EVAL_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <math.h>
#include <float.h>

#include <gak/array.h>
#include <gak/map.h>

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

#if defined( __BORLANDC__ ) || defined( _MSC_VER )
	#define isnan( a ) _isnan( a )
	#define finite( a ) _finite( a )
#endif

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <class T> 
class ExpressionEvaluator
{
	public:
	typedef T (ExpressionEvaluator<T>::*UnaryFunc)( T param );
	typedef T (ExpressionEvaluator<T>::*BinaryFunc)( T param1, T param2 );

	enum OperatorAssoc
	{
		ASSOC_LEFT, ASSOC_RIGHT
	};
	struct ExprOperator
	{
		const char		*exprOperator;
		size_t			length;
		int				precedence;
		OperatorAssoc	assoc;
		UnaryFunc		unaryLeft, unaryRight;
		BinaryFunc		binary;
	};
	struct ExprOperatorPosition : public ExprOperator
	{
		size_t	position;

		const ExprOperatorPosition & operator = ( const ExprOperator &source )
		{
			this->exprOperator = source.exprOperator;
			this->length = source.length;
			this->precedence = source.precedence;
			this->unaryLeft = source.unaryLeft;
			this->unaryRight = source.unaryRight;
			this->binary = source.binary;

			return *this;
		}
	};

	private:
	Array<ExprOperator>			operators;
	PairMap<CI_STRING, T>		variables;

	size_t skipParenthesis( const char *cp );
	size_t findOperator( const char *cp );
	void findOperator( const STRING &expression, ExprOperatorPosition &operatorPos );

	size_t isFunctionCall( const STRING &expression );

	void fillParameter( const char *cp, Array<T> *parameterList );
	virtual T evaluateFunction( const STRING &funcExpression, const Array<T> &parameterList );
	T evaluateFunction( const STRING &funcExpression, size_t startParenthesis );
	T internalEvaluate( STRING expression );

	protected:
	virtual void fillOperators( Array<ExprOperator>	*operators ) = 0;

	void replaceOperator( const char *from, const char *to )
	{
		size_t	operatorIndex = findOperator( from );
		if( operatorIndex != -1 )
		{
			operators[operatorIndex].exprOperator = to;
		}
	}
	public:
	virtual T evaluateConstant( const STRING &constExpression );
	T evaluate( const STRING &expression )
	{
		if( !operators.size() )
		{
			fillOperators( &operators );
		}

		return internalEvaluate( expression );
	}
	T evaluate( const STRING &expression, const T &x )
	{
		addVariable( "x", x );
		return evaluate( expression );
	}
	T evaluate( const STRING &expression, const T &x, const T &y )
	{
		addVariable( "y", y );
		return evaluate( expression, x );
	}
	T evaluate( const STRING &expression, const T &x, const T &y, const T &z )
	{
		addVariable( "z", z );
		return evaluate( expression, x, y );
	}
	void addVariable( const CI_STRING &name, const T &value )
	{
		variables[name] = value;
	}
	void clearVariables( void )
	{
		variables.clear();
	}
	bool isDefined( const CI_STRING &name )
	{
		return variables.hasElement( name );
	}
};

template <class T> 
class NumericEvaluator : public ExpressionEvaluator<T>
{
	protected:
	virtual void fillOperators( Array< typename ExpressionEvaluator<T>::ExprOperator >	*operators );

	public:
	NumericEvaluator();

	T binaryPlus( T oper1, T oper2 )
	{
		T result = oper1 + oper2;

		return result;
	}
	T unaryPlus( T oper )
	{
		T result = oper;

		return result;
	}
	T binaryMinus( T oper1, T oper2 )
	{
		T result = oper1 - oper2;

		return result;
	}
	T unaryMinus( T oper )
	{
		T result = - oper;

		return result;
	}
	T multiply( T oper1, T oper2 )
	{
		T result = oper1 * oper2;

		return result;
	}
	T division( T oper1, T oper2 )
	{
		if( !oper2 )
			throw DivisionByZeroError();

		T result = oper1 / oper2;

		return result;
	}
};

class MathExpression : public NumericEvaluator<double>
{
	virtual void fillOperators( Array<ExprOperator>	*operators );

	virtual double evaluateConstant( const STRING &constExpression );
	virtual double evaluateFunction( const STRING &funcExpression, const Array<double> &parameterList );

	double checkResult( double result )
	{
		if( isnan( result ) || !finite( result ) )
			throw InvalidResultError();

		return result;
	}

	public:
	MathExpression()
	{
		fillVariables();
	}
	void fillVariables( void )
	{
		addVariable( "e", M_E );
		addVariable( "pi", M_PI );
	}
	double power( double oper1, double oper2 )
	{
		return checkResult( pow( oper1, oper2 ) );
	}
	double faculty( double oper );

	double evaluate( const STRING &expression, double x, double y, double z )
	{
		return checkResult( NumericEvaluator<double>::evaluate( expression, x, y, z ) );
	}
	double evaluate( const STRING &expression, double x, double y )
	{
		return checkResult( NumericEvaluator<double>::evaluate( expression, x, y ) );
	}
	double evaluate( const STRING &expression, double x )
	{
		return checkResult( NumericEvaluator<double>::evaluate( expression, x ) );
	}
	double evaluate( const STRING &expression )
	{
		return checkResult( NumericEvaluator<double>::evaluate( expression ) );
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

void initFPUsignal( void );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

template <class T> 
inline NumericEvaluator<T>::NumericEvaluator()
{
	initFPUsignal();
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

template <class T> 
size_t ExpressionEvaluator<T>::skipParenthesis( const char *cp )
{
	size_t	countChars = 0;
	size_t	level = 0;

	while( true )
	{
		char	c = *cp++;
		countChars++;
		if( c == '(' )
			level++;
		else if( c == ')' )
		{
			level--;
			if( !level )
				break;
		}
		else if( !c )
/*@*/		throw MissingParenthesisError();
	}

	return countChars;
}

template <class T> 
size_t ExpressionEvaluator<T>::findOperator( const char *cp )
{
	for( size_t i=0; i<operators.size(); i++ )
	{
		const ExprOperator &theOperator = operators[i];
		if( !strncmp( cp, theOperator.exprOperator,  theOperator.length ) )
		{
/***/		return i;
		}
	}

	return operators.no_index;
}

template <class T> 
void ExpressionEvaluator<T>::findOperator( const STRING &expression, ExprOperatorPosition &operatorPos )
{
	int		maxPrecedence = -1;
	size_t	theOperatorIndex = size_t(-1);
	size_t	theOperatorPos = size_t(-1);
	char	c;
	
	const char *cp=expression;
	for( size_t i=0; (c=*cp)!=0; i++, cp++ )
	{
		if( c == '(' )
		{
			size_t count = skipParenthesis( cp );
			cp += count;
			i += count;
			c = *cp;
			if( !c )
				break;
		}
		else if( c == ')' )
		{
/*@*/		throw UnexpectedParenthesisError();
		}

		if( !isspace( *cp ) )
		{
			size_t	tmpIndex = findOperator( cp );
			if( tmpIndex != -1 )
			{
				const ExprOperator &theOperator = operators[tmpIndex];
				if( maxPrecedence < theOperator.precedence
				|| (
					maxPrecedence == theOperator.precedence
					&& theOperator.assoc == ASSOC_LEFT
				) )
				{
					maxPrecedence = theOperator.precedence;
					theOperatorIndex = tmpIndex;
					theOperatorPos = i;
				}
			}
		}
	}

	if( theOperatorIndex != -1 )
	{
		operatorPos = operators[theOperatorIndex];
	}
	operatorPos.position = theOperatorPos;
}

template <class T> 
size_t ExpressionEvaluator<T>::isFunctionCall( const STRING &expression )
{
	size_t	startParenthesis = 0;
	if( expression.endsWith( ')' ) )
	{
		startParenthesis = expression.searchChar( '(' );
		if( startParenthesis == -1 || !startParenthesis )
			throw UnexpectedParenthesisError();
	}

	return startParenthesis;
}

template <class T> 
void ExpressionEvaluator<T>::fillParameter( const char *cp, Array<T>	*parameterList )
{
	int	parenthesisLevel = 0;

	STRING	param;
	char	c;

	while( (c = *cp++) != 0 )
	{
		if( c == '(' )
		{
			parenthesisLevel++;
			param += c;
		}
		else if( c == ')' )
		{
			parenthesisLevel--;
			param += c;
		}
		else if( c == ',' && !parenthesisLevel )
		{
			*parameterList += internalEvaluate( param );
			param = "";
		}
		else
			param += c;
	}
	*parameterList += internalEvaluate( param );
}

template <class T> 
T ExpressionEvaluator<T>::evaluateFunction( const STRING &funcExpression, size_t startParenthesis )
{
	STRING		functionName = funcExpression.leftString( startParenthesis ).stripBlanks();
	STRING		parameterStr = funcExpression.subString( startParenthesis+1, funcExpression.strlen()-startParenthesis-2 ).stripBlanks();
	Array<T>	parameterList;

	if( !parameterStr.isEmpty() )
		fillParameter( parameterStr, &parameterList );

	return evaluateFunction( functionName, parameterList );
}

template <class T> 
T ExpressionEvaluator<T>::internalEvaluate( STRING expression )
{
	size_t					startParenthesis;
	T						result;
	ExprOperatorPosition	operatorPos;

	expression.stripBlanks();

	while( expression.beginsWith( '(' ) && expression.endsWith( ')' ) )
	{
		if( skipParenthesis( expression ) == expression.strlen() )
		{
			expression.cut( expression.strlen() -1 );
			expression += size_t(1);
		}
		else
			break;
	}
	findOperator( expression, operatorPos );

	if( operatorPos.position != -1 )
	{
		STRING	leftOperandStr = expression.leftString( operatorPos.position ).stripBlanks();
		STRING	rightOperandStr = expression.subString( operatorPos.position+operatorPos.length ).stripBlanks();

		if( operatorPos.binary && !leftOperandStr.isEmpty() && !rightOperandStr.isEmpty() )
		{
			T		leftOperand = internalEvaluate( leftOperandStr );
			T		rightOperand = internalEvaluate( rightOperandStr );
			result = (this->*operatorPos.binary)( leftOperand, rightOperand );
		}
		else if( operatorPos.unaryLeft && leftOperandStr.isEmpty() && !rightOperandStr.isEmpty() )
		{
			T		rightOperand = internalEvaluate( rightOperandStr );
			result = (this->*operatorPos.unaryLeft)( rightOperand );
		}
		else if( operatorPos.unaryRight && !leftOperandStr.isEmpty() && rightOperandStr.isEmpty() )
		{
			T		leftOperand = internalEvaluate( leftOperandStr );
			result = (this->*operatorPos.unaryRight)( leftOperand );
		}
		else
			throw OperatorError();
	}
	else if( (startParenthesis = isFunctionCall( expression )) != 0 )
		result = evaluateFunction( expression, startParenthesis );
	else
		result = evaluateConstant( expression );

	return result;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
template <class T> 
T ExpressionEvaluator<T>::evaluateFunction( const STRING &, const Array<T> & )
{
	throw UnknownFunctionError();
}

template <class T> 
void NumericEvaluator<T>::fillOperators( Array<typename ExpressionEvaluator<T>::ExprOperator>	*operators )
{
	static typename ExpressionEvaluator<T>::ExprOperator	numericOperators[] =
	{
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0551
#define typename /* no support of C++ Builder 5 */
#endif
		{ "*",  1, 100, ExpressionEvaluator<T>::ASSOC_LEFT, NULL,																			NULL, typename ExpressionEvaluator<T>::BinaryFunc(&NumericEvaluator<T>::multiply) },
		{ "/",  1, 100, ExpressionEvaluator<T>::ASSOC_LEFT, NULL,																			NULL, typename ExpressionEvaluator<T>::BinaryFunc(&NumericEvaluator<T>::division) },
		{ "+",  1, 200, ExpressionEvaluator<T>::ASSOC_LEFT, typename ExpressionEvaluator<T>::UnaryFunc(&NumericEvaluator<T>::unaryPlus),	NULL, typename ExpressionEvaluator<T>::BinaryFunc(&NumericEvaluator<T>::binaryPlus) },
		{ "-",  1, 200, ExpressionEvaluator<T>::ASSOC_LEFT, typename ExpressionEvaluator<T>::UnaryFunc(&NumericEvaluator<T>::unaryMinus),	NULL, typename ExpressionEvaluator<T>::BinaryFunc(&NumericEvaluator<T>::binaryMinus) }
#if defined( __BORLANDC__ ) && __BORLANDC__ <= 0x0551
#undef typename
#endif
	};

	operators->addElements( numericOperators, 4 );
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <class T> 
T ExpressionEvaluator<T>::evaluateConstant( const STRING &constExpression )
{
	return variables[constExpression];
}

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
