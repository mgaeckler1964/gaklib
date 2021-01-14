/*
		Project:		GAKLIB
		Module:			XPath.cpp
		Description:	XPath functions
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

#include <ctype.h>

#include <gak/XPath.h>
#include <gak/xml.h>
#include <gak/numericString.h>
#include <gak/logfile.h>

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
namespace xml
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

void Element::getChildren( XmlArray *result, bool inclPcData )
{
	Element	*theChild;
	size_t		numElements = getNumObjects();
	for( size_t i=0; i<numElements; i++ )
	{
		theChild = getElement( i );
		if( inclPcData || !theChild->getTag().isEmpty() )		// test tag due to xslt
		{
			*result += theChild;
		}
	}
}

void Element::getDescendants( XmlArray *result, bool inclPcData )
{
	Element	*theChild;
	size_t		numElements = getNumObjects();
	for( size_t i=0; i<numElements; i++ )
	{
		theChild = getElement( i );
		if( inclPcData || !theChild->getTag().isEmpty() )		// test tag due to xslt
		{
			*result += theChild;
			theChild->getDescendants( result, inclPcData );
		}
	}
}

void Element::getReverseDescendants( XmlArray *result, bool inclPcData )
{
	Element	*theChild;
	size_t	numElements = getNumObjects();
	for( size_t i=numElements-1; i<numElements; i-- )
	{
		theChild = getElement( i );
		if( inclPcData || !theChild->getTag().isEmpty() )		// test tag due to xslt
		{
			theChild->getReverseDescendants( result, inclPcData );
			*result += theChild;
		}
	}
}

void Element::getFollowingSiblings( XmlArray *result, bool inclPcData )
{
	Element	*sibling;
	size_t		numElements;
	Element	*parent = getParent();
	if( parent )
	{
		numElements = parent->getNumObjects();
		for( size_t i = getIndex()+1; i<numElements; i++ )
		{
			sibling = parent->getElement( i );
			if( inclPcData || !sibling->getTag().isEmpty() )		// test tag due to xslt
			{
				*result += sibling;
			}
		}
	}
}

void Element::getFollowing( XmlArray *result, bool inclPcData )
{
	Element	*sibling;
	size_t	numElements;
	Element	*parent = getParent();
	if( parent )
	{
		numElements = parent->getNumObjects();
		for( size_t i = getIndex()+1; i<numElements; i++ )
		{
			sibling = parent->getElement( i );
			if( inclPcData || !sibling->getTag().isEmpty() )		// test tag due to xslt
			{
				*result += sibling;
				sibling->getDescendants( result, inclPcData );
			}
		}
	}
}

void Element::getPrecedingSiblings( XmlArray *result, bool inclPcData )
{
	Element	*sibling;
	Element	*parent = getParent();
	if( parent )
	{
		for( long i = getIndex()-1; i>=0; i-- )
		{
			sibling = parent->getElement( i );
			if( inclPcData || !sibling->getTag().isEmpty() )		// test tag due to xslt
			{
				*result += sibling;
			}
		}
	}
}

void Element::getPrecedings( XmlArray *result, bool inclPcData )
{
	Element	*sibling;
	Element	*parent = getParent();
	if( parent )
	{
		for( long i = getIndex()-1; i>=0; i-- )
		{
			sibling = parent->getElement( i );
			if( inclPcData || !sibling->getTag().isEmpty() )		// test tag due to xslt
			{
				sibling->getReverseDescendants( result, inclPcData );
				*result += sibling;
			}
		}
	}
}

STRING Element::locateElements( const STRING &path, XmlArray *result, bool inclPcData )
{
	/*
		handle abreviations
	*/
	if( path[0U] == '/' )
	{
		STRING newPath = path;

		newPath += (size_t)1;

		Element	*parent = this;
		Element *newParent = this;
		while( true )
		{
			newParent = newParent->getParent();
			if( newParent )
				parent = newParent;
			else
/*v*/			break;
		}

/***/	return parent->locateElements( newPath, result, inclPcData );
	}
	else if( path.beginsWith( "../" ) )
	{
		STRING newPath = path;
		newPath += (size_t)3;
		Element	*parent = getParent();
		if( !parent )
			parent = this;

/***/	return parent->locateElements( newPath, result, inclPcData );
	}

	STRING	attribute, localPath = path;
	if( localPath.beginsWith( "./" ) )
		localPath += (size_t)2;

	/*
		attribute selection
	*/
	if( localPath.beginsWith( '@' ) )
	{
		/*
			return my self and the attribute name
		*/
		*result += this;
		attribute = localPath;
		attribute += (size_t)1;
	}
	else if( localPath.isEmpty() || localPath == "." )
	{
		/*
			return my self and no attribute name
		*/
		*result += this;
		attribute = "";
	}
	else
	{
		// find the next slash
		size_t	i=0, len = localPath.strlen();
		size_t	parenthesisCount = 0;
		STRING	selector = localPath;
		char	c = 0;
		while( i<len )
		{
			c = localPath[i++];

			if( c=='(' || c=='[' )
				parenthesisCount++;
			else if( c==')' || c==']' )
				parenthesisCount--;
			else if (c == '"' || c == '\'' )
			{
				while( i<len && localPath[i++] != c )
					;
			}
			else if( c=='/' && !parenthesisCount )
			{
				break;
			}
		}
		if( c == '/' )
			selector = localPath.leftString( i-1 );
		localPath = localPath.subString( i );


		/*
			the rest of the path is the attribute selection
		*/
		if( localPath[0U] == '@' )
		{
			attribute = localPath;
			localPath = "";
			attribute += (size_t)1;
		}

		size_t	axisDelimiter = selector.searchText( "::" );
		STRING axis = ( axisDelimiter != selector.no_index )
			? selector.leftString(axisDelimiter)
			: (STRING)"child"
		;

		if( axisDelimiter != selector.no_index )
			selector += axisDelimiter+2;

		size_t	bracketPos = selector.searchChar( '[' );
		STRING	baseTag = ( bracketPos != selector.no_index )
			? selector.leftString( bracketPos )
			: selector;

		if( bracketPos != selector.no_index )
			selector += bracketPos;
		else
			selector = "";

		XmlArray	axisElements;

		axis.stripBlanks();
		if( axis == "child" )
			getChildren( &axisElements, inclPcData );
		else if( axis == "descendant" )
			getDescendants( &axisElements, inclPcData );
		else if( axis == "following" )
			getFollowing( &axisElements, inclPcData );
		else if( axis == "following-sibling" )
			getFollowingSiblings( &axisElements, inclPcData );
		else if( axis == "preceding" )
			getPrecedings( &axisElements, inclPcData );
		else if( axis == "preceding-sibling" )
			getPrecedingSiblings( &axisElements, inclPcData );
		else if( axis == "self" )
			axisElements += this;
		else if( axis == "parent" )
		{
			if( parent )
			{
				axisElements += static_cast<Element*>(parent);
			}
		}

		XmlArray	testElements;
		size_t numElements = axisElements.size();
		baseTag.stripBlanks();
		for( i=0; i<numElements; i++ )
		{
			Element	*theElement = axisElements[i];
			if( theElement->getTag() == baseTag || baseTag == "*" )
			{
				testElements += theElement;
			}
		}


		STRING	expression, expResult;
		bool	isNumber;
		size_t	position;
		selector.stripBlanks();
		while( selector.beginsWith('[') && testElements.size() )
		{
			/* extraxt the next expresseion */
			parenthesisCount = 0;
			len = selector.strlen();
			expression = "";
			i = 0;

			while( i<len )
			{
				c = selector[i++];

				if( c=='(' || c=='[' )
					parenthesisCount++;
				else if( c==')' || c==']' )
				{
					parenthesisCount--;
					if( !parenthesisCount )
					{
						expression = selector.subString( 1, i-2 );
						expression.stripBlanks();
						selector += i;
						selector.stripBlanks();
/*v*/					break;
					}

				}
				else if (c == '"' || c == '\'' )
				{
					while( i<len && localPath[i++] != c )
						;
				}
			}

			if( !expression.isEmpty() )
			{
				isNumber = true;
				for( i=0; i<expression.strlen(); i++ )
				{
					if( !isdigit( expression[i] ) )
					{
						isNumber = false;
/*v*/					break;
					}
				}

				if( isNumber )
				{
					position = expression.getValueE<std::size_t>();
					position--;
					Element *foundElement =
						(position < testElements.size())
						? testElements[position]
						: NULL;

					testElements.clear();
					testElements += foundElement;
				}
				else
				{
					testElements.first();
					while( testElements.current() )
					{
						expResult = evaluate( expression, &testElements );
						if( expResult != "1" )
						{
							testElements.removeCurrent();
						}
						else
							testElements.next();
					}
				}
			}
		}

		if( localPath.isEmpty() )
		{
			result->addElements( testElements );
		}
		else
		{
			for( i=0; i<testElements.size(); i++ )
			{
				if( attribute.isEmpty() )
					attribute = testElements[i]->locateElements(
						localPath, result, inclPcData
					);
				else
					testElements[i]->locateElements(
						localPath, result, inclPcData
					);
			}
		}
	}

	return attribute;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING valueOf( XmlArray *sourceArray, const STRING &select )
{
	STRING		result;
	if( select == "position()" )
	{
		result += formatNumber( sourceArray->position() );
	}
	else if( select == "last()" )
	{
		result += formatNumber( sourceArray->last() );
	}
	else
	{
		XmlArray	docElements;
		Element	*source = sourceArray->current();
		STRING		attribute = source->locateElements( select, &docElements, true );

		if( attribute.isEmpty() )
		{
			for( size_t i=0; i<docElements.size(); i++ )
				result += docElements[i]->getValue( PLAIN_MODE );
		}
		else
		{
			for( size_t i=0; i<docElements.size(); i++ )
				result += docElements[i]->getAttribute( attribute );
		}
	}

	return result;
}

STRING stripExpression( const STRING &expression )
{
	STRING	result = expression;

	result.stripBlanks();
	if( result.beginsWith( '(' ) )
	{
		size_t	parenthesis=0;
		size_t	len = result.strlen();

		for( size_t i=0;i<len; i++ )
		{
			if( result[i] == '(' )
				parenthesis++;
			else if( result[i] == ')' )
			{
				parenthesis--;
				if( !parenthesis )
				{
					if( i+1 >= len )
					{
						result.cut( len-1 );
						result += (size_t)1;

						result = stripExpression( result );
					}
					break;
				}
			}
		}
	}

	return result;
}

xsltOperator locateOperator( const STRING &expression, STRING *left, STRING *right )
{
	char			c;
	bool			checkSpace;
	const char		*exprCP = expression;
	xsltOperator	newOperator = NO_OP, found = NO_OP;
	size_t			parenthesis = 0;
	size_t			i = 0, len = strlen( expression );
	size_t			opPos = 0, opLen = 0, newOpLen;
	const size_t	maxPriority = std::numeric_limits<std::size_t>::max();
	size_t			newPriority, priority=maxPriority;

	while( i<len )
	{
		c = exprCP[i];
		if( c == '\'' || c == '\"' )
			while( i<len && exprCP[++i] != c )
				;
		else if( c == '(' || c == '[' )
			parenthesis++;
		else if( c == ')' || c == ']' )
			parenthesis--;
		else if( !parenthesis )
		{
			newPriority = maxPriority;
			newOpLen = 2;
			checkSpace = false;

			if( !strncmp( exprCP+i, "||", 2 ) )
			{
				newOperator = XSLT_LOG_OR;
				newPriority = 0;
			}
			else if( !strncmpi( exprCP+i, "or", 2 ) )
			{
				newOperator = XSLT_LOG_OR;
				newPriority = 0;
				checkSpace = true;
			}
			else if( !strncmp( exprCP+i, "&&", 2 ) )
			{
				newOperator = XSLT_LOG_AND;
				newPriority = 1;
			}
			else if( !strncmpi( exprCP+i, "and", 2 ) )
			{
				newOperator = XSLT_LOG_AND;
				newPriority = 1;
				newOpLen = 3;
				checkSpace = true;
			}
			else if( c == '=' )
			{
				newOperator = XSLT_EQUAL;
				newPriority = 2;
				newOpLen = 1;
			}
			else if( !strncmp( exprCP+i, "!=", 2 ) )
			{
				newOperator = XSLT_NOT_EQUAL;
				newPriority = 2;
			}
			/*
				relational oprators: later
				newPriority = 3;
			*/
			else if( c == '+' )
			{
				newOperator = XSLT_PLUS;
				newPriority = 4;
				newOpLen = 1;
			}
			else if( c == '-' )
			{
				newOperator = XSLT_MINUS;
				newPriority = 4;
				newOpLen = 1;
			}
			else if( c == '%' )
			{
				newOperator = XSLT_MODULO;
				newPriority = 5;
				newOpLen = 1;
			}
			else if( !strncmp( exprCP+i, "mod", 3 ) )
			{
				newOperator = XSLT_MODULO;
				newOpLen = 3;
				checkSpace = true;
				newPriority = 5;
			}
			else if( !strncmp( exprCP+i, "div", 3 ) )
			{
				newOperator = XSLT_DIVISION;
				newOpLen = 3;
				checkSpace = true;
				newPriority = 5;
			}
			else if( c == '*' )
			{
				newOperator = XSLT_MULTIPLY;
				newPriority = 5;
				newOpLen = 1;
			}

			if( checkSpace
			&& (!i || isalnum(exprCP[i-1]) || isalnum(exprCP[i+newOpLen]) ) )
				newPriority = maxPriority;

			if( newPriority < priority )
			{
				found = newOperator;
				opPos = i;
				opLen = newOpLen;
				priority = newPriority;
				if( !priority )
/*v*/				break;
			}
		}

		i++;
	}

	if( found != NO_OP )
	{
		*left = expression.leftString( opPos );
		*right = expression.subString( opPos + opLen );
	}

	return found;
}

STRING performEqual( STRING left, STRING right, XmlArray *source )
{
	doEnterFunction( "XML_TRANSFORMATOR::performEqual()" );

	left = evaluate( left, source );
	right = evaluate( right, source );

	return (left == right) ? "1" : "0";
}

STRING performNotEqual( STRING left, STRING right, XmlArray *source )
{
	doEnterFunction( "XML_TRANSFORMATOR::performNotEqual()" );

	left = evaluate( left, source );
	right = evaluate( right, source );

	return (left != right) ? "1" : "0";
}

STRING performLogAnd( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	bool	leftBool = (!left.isEmpty() && (left != "0"));
	bool	rightBool = (!right.isEmpty() && (right != "0"));

	return (leftBool && rightBool) ? "1" : "0";
}

STRING performLogOr( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	bool	leftBool = (!left.isEmpty() && (left != "0"));
	bool	rightBool = (!right.isEmpty() && (right != "0"));

	return (leftBool || rightBool) ? "1" : "0";
}

STRING performModulo( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	int		leftI, rightI;

	if( left.isEmpty() )
		leftI = 0;
	else
		leftI = left.getValueE<int>();

	if( right.isEmpty() )
		rightI = 0;
	else
		rightI = right.getValueE<int>();

	STRING	result;
	if( rightI )
	{
		int resultI = leftI % rightI;
		result += formatNumber( resultI );
	}
	else
		result = "NaN";

	return result;
}

STRING performDivision( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	int		leftI, rightI;

	if( left.isEmpty() )
		leftI = 0;
	else
		leftI = left.getValueE<int>();

	if( right.isEmpty() )
		rightI = 0;
	else
		rightI = right.getValueE<int>();

	STRING	result;
	if( rightI )
	{
		int resultI = leftI / rightI;
		result += formatNumber( resultI );
	}
	else
		result = "NaN";

	return result;
}

STRING performPlus( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	int		leftI, rightI;

	if( left.isEmpty() )
		leftI = 0;
	else
		leftI = left.getValueE<int>();

	if( right.isEmpty() )
		rightI = 0;
	else
		rightI = right.getValueE<int>();

	int		resultI = leftI + rightI;
	STRING	result = formatNumber( resultI );

	return result;
}

STRING performMinus( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	int		leftI, rightI;

	if( left.isEmpty() )
		leftI = 0;
	else
		leftI = left.getValueE<int>();

	if( right.isEmpty() )
		rightI = 0;
	else
		rightI = right.getValueE<int>();

	int		resultI = leftI - rightI;
	STRING	result = formatNumber( resultI );

	return result;
}

STRING performMultiply( STRING left, STRING right, XmlArray *source )
{
	left = evaluate( left, source );
	right = evaluate( right, source );

	int		leftI, rightI;

	if( left.isEmpty() )
		leftI = 0;
	else
		leftI = left.getValueE<int>();

	if( right.isEmpty() )
		rightI = 0;
	else
		rightI = right.getValueE<int>();

	int		resultI = leftI * rightI;
	STRING	result = formatNumber( resultI );

	return result;
}

STRING evaluate( STRING expression, XmlArray *source )
{
	doEnterFunction( "evaluate()" );

	STRING	result, left, op, right;

	expression = stripExpression( expression );

	xsltOperator theOperator = locateOperator( expression, &left, &right );
	if( theOperator )
	{
		if( theOperator == XSLT_EQUAL )
			result = performEqual( left, right, source );
		else if( theOperator == XSLT_NOT_EQUAL )
			result = performNotEqual( left, right, source );
		else if( theOperator == XSLT_LOG_AND )
			result = performLogAnd( left, right, source );
		else if( theOperator == XSLT_LOG_OR )
			result = performLogOr( left, right, source );
		else if( theOperator == XSLT_MODULO )
			result = performModulo( left, right, source );
		else if( theOperator == XSLT_PLUS )
			result = performPlus( left, right, source );
		else if( theOperator == XSLT_MINUS )
			result = performMinus( left, right, source );
		else if( theOperator == XSLT_MULTIPLY )
			result = performMultiply( left, right, source );
		else if( theOperator == XSLT_DIVISION )
			result = performDivision( left, right, source );
	}
	else if( (expression[0U] == '\'' || expression[0U] == '\"')
	&& expression.strlen() >= 2
	&& expression[expression.strlen()-1] == expression[0U] )
	{
		result = expression.subString( 1, expression.strlen() -2 );
	}
	else
	{
		char c;
		result = expression;
		for( size_t i=0; i<expression.strlen(); i++ )
		{
			c = expression[i];
			if( !isdigit( c ) && c != '.' )
			{
				result = valueOf( source, expression );
				break;
			}
		}
	}
	return result;
}

}	// namespace xml
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

