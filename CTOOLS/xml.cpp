/*
		Project:		GAKLIB
		Module:			XML.CPP
		Description:	XML-Generator
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cctype>

#include <gak/xml.h>
#include <gak/xmlValidator.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

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

// Known namespaces

const char XMLNS_SCHEMA[]				= "http://www.w3.org/2001/XMLSchema";
const char XMLNS_SCHEMA_INSTANCE[]		= "http://www.w3.org/2001/XMLSchema-instance";
const char XMLNS_XSLT[]					= "http://www.w3.org/1999/XSL/Transform";
const char XMLNS_SOAP[]					= "http://schemas.xmlsoap.org/wsdl/soap/";
const char XMLNS_SOAP_ENVELOPE[]		= "http://schemas.xmlsoap.org/soap/envelope/";
const char XMLNS_WSDL[]					= "http://schemas.xmlsoap.org/wsdl/";

// Known attributes
const char XMLNS_ATTRIBUTE[]			= "xmlns";
const char XMLNS_SCHEMALOCATION[]		= "schemaLocation";
const char XMLNS_NS_SCHEMALOCATION[]	= "noNamespaceSchemaLocation";

const char XSLT_PUBLIC[]				= "doctype-public";

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

XmlContainer::~XmlContainer()
{
	size_t	i, numObj;

	numObj = objects.size();

	i = numObj;
	if( i ) do
	{
		i--;
		delete objects[i];
		objects.removeElementAt( i );

	} while( i );
}

Element::~Element()
{
	if( theCssStyle )
		delete theCssStyle;
};
// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

STRING Element::getPrefix( const STRING &text )
{
	STRING	prefix = text;
	size_t	colonPos = prefix.searchChar( ':' );
	if( colonPos != prefix.no_index )
		prefix.cut( colonPos );
	else
		prefix = "";

	return prefix;
}

STRING Element::getLocalName( const STRING &text )
{
	STRING	local = text;
	size_t	colonPos = local.searchChar( ':' );
	if( colonPos != local.no_index )
	{
		colonPos++;
		local += colonPos;
	}

	return local;
}

STRING XmlWithAttributes::formatAttribute( const STRING &name, const STRING &value, GeneratorMode mode )
{
/*
	STRING	convertedValue =
		mode == XML_MODE   ? ascii2XML( value )  :
		mode == XHTML_MODE ? ascii2HTML( value ) : value;
*/
	STRING	convertedValue;
	if( mode == XML_MODE )
	   convertedValue = ascii2XML( value );
	else if( mode == XHTML_MODE )
	   convertedValue = ascii2HTML( value );
	else
	   convertedValue = value;

	convertedValue.replaceChar( '\"', '\'' );
	STRING	attrValuePair = name + "=\"" + convertedValue + '\"';

	return attrValuePair;
}

STRING XmlWithAttributes::formatAttribute( const STRING &name, long value, GeneratorMode mode )
{
	STRING	tmpValue = formatNumber( value );

	return formatAttribute( name, tmpValue, mode );
}

Validator *Element::getValidator( const STRING &prefix ) const
{
	if( theValidator && prefix == getPrefix() )
		return theValidator;

	Element *parent = getParent();
	if( parent )
		return parent->getValidator( prefix );

	return NULL;
}

STRING Element::getSchemaLocation( Element **theElement, const STRING &prefix )
{
	STRING schemaLocation;
	if( prefix == getPrefix() )
	{
		STRING	prefix = getPrefix4Namespace( XMLNS_SCHEMA_INSTANCE );
		STRING	attrName = prefix;
		if( !attrName.isEmpty() )
			attrName += ':';
		attrName += XMLNS_NS_SCHEMALOCATION;
		schemaLocation = getAttribute( attrName );
		if( schemaLocation.isEmpty() )
		{
			attrName = prefix;
			if( !attrName.isEmpty() )
				attrName += ':';
			attrName += XMLNS_SCHEMALOCATION;
			schemaLocation = getAttribute( attrName );

			if( !schemaLocation.isEmpty() )
			{
				size_t	i;
				for( i=0; i<schemaLocation.strlen(); i++ )
					if( isspace( schemaLocation[i] ) )
						break;
				for( ;i<schemaLocation.strlen(); i++ )
					if( !isspace( schemaLocation[i] ) )
						break;

				schemaLocation += i;
			}
		}
	}
	if( schemaLocation.isEmpty() )
	{
		Element *parent = getParent();
		if( parent )
			return parent->getSchemaLocation( theElement, prefix );
		*theElement = NULL;
	}
	else
		*theElement = this;

	return schemaLocation;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

bool Element::isCaseSensitive( void ) const
{
	return true;
}

/*
---------------------------------------------------------------------------
	isSimpleTag
---------------------------------------------------------------------------
*/
bool XmlText::isSimpleTag( void ) const
{
	return false;
}

bool Document::isSimpleTag( void ) const
{
	return false;
}

bool Any::isSimpleTag( void ) const
{
	return false;
}

bool Mark::isSimpleTag( void ) const
{
	return false;
}

/*
---------------------------------------------------------------------------
	wantScriptCode
---------------------------------------------------------------------------
*/
bool Element::wantScriptCode( void ) const
{
	return false;
}

/*
---------------------------------------------------------------------------
	isValidSubobject
---------------------------------------------------------------------------
*/
Validator *Element::getValidSubobjects( ArrayOfStrings *tags )
{
	tags->clear();

	return NULL;
}

Validator *XmlContainer::getValidSubobjects( ArrayOfStrings *tags )
{
	tags->clear();

	Validator	*theValidator = getValidator();
	if( theValidator )
		theValidator->getValidSubobjects( this, tags );

	return theValidator;
}

Validator *Mark::getValidSubobjects( ArrayOfStrings *tags )
{
	tags->clear();

	return NULL;
}

/*
---------------------------------------------------------------------------
	isValidSubobject
---------------------------------------------------------------------------
*/
bool Element::isValidSubobject( Element * )
{
	return false;
}

bool XmlContainer::isValidSubobject( Element *newObject )
{
	Validator	*theValidator = getValidator();
	if( theValidator )
		return theValidator->isValidSubobject( this, newObject );
	else
		return true;	// accept all sub objects
}

bool Mark::isValidSubobject( Element * )
{
	return false;	// accept no sub objects
}

/*
---------------------------------------------------------------------------
	addObject
---------------------------------------------------------------------------
*/
Element *Element::addObject( Element *newObject, bool )
{
	return newObject;	// no container => ignore
}

Element *XmlContainer::addObject( Element *newObject, bool parserMode )
{
	if( newObject->getParent() )
	{
		STRING	error = "Object stored elsewhere: " + newObject->getTag() + " within " + getTag() +"\n";
		newObject = new PCData( error );
	}
	else if( !parserMode && !isValidSubobject( newObject ) )
	{
		STRING	error = "Invalid object found: " + newObject->getTag() + " within " + getTag() +"\n";
		newObject = new PCData( error );
	}

	newObject->parent = this;
	newObject->byParser = parserMode;

	if( dynamic_cast<PCData*>(newObject) )
		hasPCdata = true;

	objects += newObject;

	return newObject;
}

/*
---------------------------------------------------------------------------
	removeObject
---------------------------------------------------------------------------
*/
Element *Element::removeObject( Element *oldObject )
{
	oldObject->parent = NULL;
	return oldObject;	// no container => ignore
}

Element *Element::removeObject( size_t )
{
	return NULL;	// no container => ignore
}

Element *XmlContainer::removeObject( size_t i )
{
	Element	*oldObject = getElement( i );
	if( oldObject )
	{
		oldObject->parent = NULL;
		objects.removeElementAt( i );
	}

	if( dynamic_cast<PCData*>(oldObject) )
	{
		hasPCdata = false;
		for( size_t i=0; i<objects.size(); i++ )
		{
			if( dynamic_cast<PCData*>(objects[i]) )
			{
				hasPCdata = true;
/*v*/			break;
			}
		}
	}

	return oldObject;
}

Element *XmlContainer::removeObject( Element *oldObject )
{
	long	index;

	index = getIndex( oldObject );
	if( index >= 0 )
	{
		oldObject->parent = NULL;
		objects.removeElementAt( (size_t)index );

		if( dynamic_cast<PCData*>(oldObject) )
		{
			hasPCdata = false;
			for( size_t i=0; i<objects.size(); i++ )
			{
				if( dynamic_cast<PCData*>(objects[i]) )
				{
					hasPCdata = true;
/*v*/				break;
				}
			}
		}

		return oldObject;
	}
	else
		return NULL;
}

/*
---------------------------------------------------------------------------
	replaceObject
---------------------------------------------------------------------------
*/
Element	*Element::replaceObject(
	Element *, Element *, bool
)
{
	return NULL;	// no container => ignore
}

Element	*XmlContainer::replaceObject(
	Element *oldObject, Element *newObject, bool transferChildren
)
{
	long	index;

	index = getIndex( oldObject );
	if( index >= 0 )
	{
		oldObject->parent = NULL;
		objects[(size_t)index] = newObject;
		newObject->parent = this;

		if( transferChildren )
		{
			Element	*child;
			size_t		numAttributes = oldObject->getNumAttributes();
			while( oldObject->getNumObjects() )
			{
				child = oldObject->removeObject( (size_t)0 );
				newObject->addObject( child, true );
			}
			for( size_t i=0; i<numAttributes; i++ )
			{
				newObject->setStringAttribute(
					oldObject->getAttributeName( i ),
					oldObject->getAttribute( i )
				);
			}
		}

		if( dynamic_cast<PCData*>(newObject) )
			hasPCdata = true;
		else if( dynamic_cast<PCData*>(oldObject) )
		{
			hasPCdata = false;
			for( size_t i=0; i<objects.size(); i++ )
			{
				if( dynamic_cast<PCData*>(objects[i]) )
				{
					hasPCdata = true;
/*v*/				break;
				}
			}
		}

		return oldObject;
	}
	else
		return NULL;
}


long XmlContainer::getIndex( const Element *oldObject )
{
	size_t	index, max;

	if( !oldObject->getParent() || oldObject->getParent() != this )
	{
		return -1;
	}

	max = getNumObjects();
	for( index=0; index < max; index++ )
		if( getElement( index ) == oldObject )
			return (long)index;

	return -1;
}

/*
---------------------------------------------------------------------------
	getNumObjects
---------------------------------------------------------------------------
*/
size_t Element::getNumObjects( void ) const
{
	return 0;
}

size_t XmlContainer::getNumObjects( void ) const
{
	return objects.size();
}

/*
---------------------------------------------------------------------------
	getElement (by index)
---------------------------------------------------------------------------
*/
Element *Element::getElement( size_t ) const
{
	return NULL;
}

Element *XmlContainer::getElement( size_t i ) const
{
	if( i < getNumObjects() )
		return objects[i];
	else
		return NULL;
}

/*
---------------------------------------------------------------------------
	getElement (by tag)
---------------------------------------------------------------------------
*/
Element *Element::getElement( const STRING &,  const STRING &  ) const
{
	return NULL;
}

Element *XmlContainer::getElement(
	const STRING &tag, const STRING &nameSpace
) const
{
	size_t		i, numElems = getNumObjects();
	STRING		theTag, xmlnsTag, prefix;
	Element	*theElem;

	for( i = 0; i<numElems; i++ )
	{
		theElem = objects[i];
		if( theElem->getLocalName() == tag
		&& (nameSpace.isEmpty() || theElem->getNamespace()== nameSpace) )
			return theElem;
	}

	return NULL;
}

/*
---------------------------------------------------------------------------
	findElement (by tag)
---------------------------------------------------------------------------
*/
Element *Element::findElement( const STRING &tag, const STRING &nameSpace ) const
{
	Element *theElement = getElement( tag, nameSpace );
	if( !theElement )
	{
		size_t i, numElems = getNumObjects();
		for( i = 0; i<numElems; i++ )
		{
			theElement = getElement(i)->findElement( tag, nameSpace );
			if( theElement )
/*v*/			break;
		}
	}

	return theElement;
}

/*
---------------------------------------------------------------------------
	getAllElements (by tag)
---------------------------------------------------------------------------
*/
void Element::getAllElements( XmlArray *elements, const STRING &tag, const STRING &nameSpace ) const
{
	size_t		i, numElems = getNumObjects();
	STRING		theTag, xmlnsTag, prefix;
	Element	*theElem;
	XmlArray	childElements;

	elements->clear();

	for( i = 0; i<numElems; i++ )
	{
		theElem = getElement(i);
		theElem->getAllElements( &childElements, tag, nameSpace );
		elements->addElements( childElements );

		theTag = theElem->getTag();
		xmlnsTag = theElem->getPrefix4Namespace( nameSpace );
		if( xmlnsTag[0U] )
			xmlnsTag += ':';
		xmlnsTag += tag;
		if( theTag == xmlnsTag )
		{
			*elements += theElem;
		}
	}
}

/*
---------------------------------------------------------------------------
	getChildElements (by tag)
---------------------------------------------------------------------------
*/
void Element::getChildElements( XmlArray *elements, const STRING &tag, const STRING &nameSpace ) const
{
	size_t		i, numElems = getNumObjects();
	STRING		theTag, xmlnsTag, prefix;
	Element	*theElem;

	elements->clear();

	for( i = 0; i<numElems; i++ )
	{
		theElem = getElement(i);
		theTag = theElem->getTag();
		xmlnsTag = theElem->getPrefix4Namespace( nameSpace );
		if( xmlnsTag[0U] )
		{
			xmlnsTag += ':';
		}
		xmlnsTag += tag;
		if( theTag == xmlnsTag )
		{
			*elements+= theElem;
		}
	}
}

/*
---------------------------------------------------------------------------
	getPrefixChildElements (by namespace/prefix)
---------------------------------------------------------------------------
*/
void Element::getPrefixChildElements(
	XmlArray *elements, const STRING &prefix
) const
{
	XmlArray	prefixChildren;
	size_t		i, numElems = getNumObjects();
	Element	*theElem;

	elements->clear();

	for( i = 0; i<numElems; i++ )
	{
		theElem = getElement(i);
		if( prefix == theElem->getPrefix() )
		{
			*elements += theElem;
		}
		else
		{
			theElem->getPrefixChildElements( &prefixChildren, prefix );
			elements->addElements( prefixChildren );
		}
	}
}

/*
---------------------------------------------------------------------------
	isInline/isBlock
---------------------------------------------------------------------------
*/
bool XmlText::isInline( void )
{
	return true;
}
bool XmlText::isBlock( void )
{
	return false;
}


bool Document::isInline( void )
{
	return false;
}
bool Document::isBlock( void )
{
	return true;
}

bool Any::isInline( void )
{
	css::Styles		*style = getCssStyle();
	css::Display	display = style->getDisplay();

/*
	if( display < 0 )
	{
		css::Float	cssFloat = style->getCssFloat();

		return cssFloat <= css::FLT_NONE;
	}
	else
*/
		return display == css::DSP_INLINE;
}
bool Any::isBlock( void )
{
	return !isInline();
}

bool Mark::isInline( void )
{
	return false;
}

bool Mark::isBlock( void )
{
	return true;
}

/*
---------------------------------------------------------------------------
	isVisible
---------------------------------------------------------------------------
*/
bool Element::isVisual( void )
{
	return false;
}

bool Comment::isVisual( void )
{
	return false;
}

bool DocType::isVisual( void )
{
	return false;
}

bool XmlText::isVisual( void )
{
	return true;
}

bool Any::isVisual( void )
{
	bool visual = getCssStyle()->getDisplay() != css::DSP_NONE;

	return visual;
}

/*
---------------------------------------------------------------------------
	getValidAttributes
---------------------------------------------------------------------------
*/
void Element::getValidAttributes( ArrayOfStrings * )
{
}

void XmlWithAttributes::getValidAttributes(
	ArrayOfStrings *attributes
)
{
	Validator *theValidator = getValidator();
	if( theValidator )
		theValidator->getValidAttributes( this, attributes );
}

/*
---------------------------------------------------------------------------
	getValidAttributes
---------------------------------------------------------------------------
*/
void Element::getValidAttributeValues( size_t, ArrayOfStrings * )
{
}

void XmlWithAttributes::getValidAttributeValues(
	size_t attrIdx, ArrayOfStrings *values
)
{
	if( attrIdx < getNumAttributes() )
	{
		Validator *theValidator = getValidator();
		if( theValidator )
			theValidator->getValidAttributeValues( this, attrIdx, values );
	}
}
/*
---------------------------------------------------------------------------
	setAttribute
---------------------------------------------------------------------------
*/
Element *Element::setStringAttribute( const STRING &, const STRING & )
{
	return this;
}

Element *Element::setStringAttribute( size_t, const STRING & )
{
	return this;
}

Element *Document::setStringAttribute( const STRING &, const STRING & )
{
	return this;
}

Element *Document::setStringAttribute( size_t, const STRING & )
{
	return this;
}

Element *XmlWithAttributes::setStringAttribute( const STRING &name, const STRING &value )
{
	if( byParser || isValidAttribute( name, value ) )
	{
//		if( isCaseSensitive() )
			attributes.updateField( name, value );
/*
	// allways store with case, otherwise some javascripts and or urls may not work
		else
		{
			STRING lowerName = name;
			STRING lowerValue = value;

			lowerName.lowerCase();
			lowerValue.lowerCase();

			attributes.updateField( lowerName, lowerValue );
		}
*/
	}
	return this;
}

Element *XmlWithAttributes::setStringAttribute( size_t i, const STRING &value )
{
	attributes[i].setValue( value );
	return this;
}

/*
---------------------------------------------------------------------------
	deleteAttribute
---------------------------------------------------------------------------
*/

void Element::deleteAttribute( const STRING & )
{
}

void Element::deleteAttribute( size_t )
{
}

void XmlWithAttributes::deleteAttribute( const STRING &name )
{
	STRING	attrName = name;
	if( !isCaseSensitive() )
		attrName.lowerCase();

	size_t	idx = attributes.getElementIndex( attrName );
	if( idx != attributes.no_index )
		deleteAttribute( idx );
}

void XmlWithAttributes::deleteAttribute( size_t i )
{
	attributes.removeElementAt( i );
}

/*
---------------------------------------------------------------------------
	getAttribute
---------------------------------------------------------------------------
*/
STRING Element::getAttribute( const STRING & ) const
{
	return "";
}

STRING XmlWithAttributes::getAttribute( const STRING &name  ) const
{
	STRING	attrName = name;
	if( !isCaseSensitive() )
		attrName.lowerCase();

	size_t	idx = attributes.getElementIndex( attrName );
	if( idx != attributes.no_index )
	{
		return attributes[idx].getValue();
	}
	else
	{
		return NULL_STRING;
	}
}

/*
---------------------------------------------------------------------------
	getAttribute
---------------------------------------------------------------------------
*/
STRING Element::getAttribute( size_t ) const
{
	return EMPTY_STRING;
}

STRING XmlWithAttributes::getAttribute( size_t i ) const
{
	return attributes[i].getValue();
}

/*
---------------------------------------------------------------------------
	setAttributeName
---------------------------------------------------------------------------
*/
void Element::setAttributeName( size_t, const STRING & )
{
}

void Document::setAttributeName( size_t, const STRING & )
{
}

void XmlWithAttributes::setAttributeName( size_t i, const STRING &name )
{
	attributes[i].setKey( name );
}

/*
---------------------------------------------------------------------------
	getAttributeName
---------------------------------------------------------------------------
*/
STRING Element::getAttributeName( size_t ) const
{
	return NULL_STRING;
}

STRING XmlWithAttributes::getAttributeName( size_t i ) const
{
	if( i<attributes.size() )
		return attributes[i].getKey();
	else

	return NULL_STRING;
}

/*
---------------------------------------------------------------------------
	getAttributes
---------------------------------------------------------------------------
*/
STRING Element::getAttributes( GeneratorMode ) const
{
	return EMPTY_STRING;
}

STRING XmlWithAttributes::getAttributes( GeneratorMode mode ) const
{
	size_t		i, numObj;
	STRING		xmlCode;
	STRING		theAttribute;

	numObj = attributes.getNumFields();
	for( i=0; i<numObj; i++ )
	{
		if( i )
			xmlCode += ' ';

		const DynamicVar	&attr = attributes[i].getValue();
		xmlCode += formatAttribute(
			attributes[i].getKey(),
			STRING( attr ),
			mode
		);
	}
	return xmlCode;
}

/*
---------------------------------------------------------------------------
	getNumAttributes
---------------------------------------------------------------------------
*/
size_t Element::getNumAttributes( void ) const
{
	return 0;
}

size_t XmlWithAttributes::getNumAttributes( void ) const
{
	return attributes.getNumFields();
}
/*
---------------------------------------------------------------------------
	isValidAttribute
---------------------------------------------------------------------------
*/
bool Element::isValidAttribute( const STRING &, const STRING & )
{
	return false;	// accept no attributes
}

bool XmlWithAttributes::isValidAttribute( const STRING &name, const STRING &value )
{
	Validator *theValidator = getValidator();
	if( theValidator )
		return theValidator->isValidAttribute( this, name, value );
	return true;	// accept all attributes
}

bool Document::isValidAttribute( const STRING &, const STRING & )
{
	return false;	// accept no attributes
}

/*
---------------------------------------------------------------------------
	getTag
---------------------------------------------------------------------------
*/
STRING XmlText::getTag( void ) const
{
	return "";
}

STRING Document::getTag( void ) const
{
	return "";
}

STRING Any::getTag( void ) const
{
	return tag;
}

STRING Mark::getTag( void ) const
{
	return piTag;
}

/*
---------------------------------------------------------------------------
	getPath
---------------------------------------------------------------------------
*/
STRING Element::getPath( bool includeIndex )
{
	STRING path = parent ? parent->getPath( includeIndex ) : (STRING)"";
	STRING	myTag = getTag();

	if( !myTag.isEmpty() )
	{
		path += '/';
		path += myTag;
		if( includeIndex )
		{
			long	i = getIndex();
			path += '[';
			path += formatNumber( i+1 );
			path += ']';
		}
	}

	return path;
}

/*
---------------------------------------------------------------------------
	setTag
---------------------------------------------------------------------------
*/
void Element::setTag( const STRING & )
{
}

void Any::setTag( const STRING &tag )
{
	this->tag = tag;
}

void Mark::setTag( const STRING &tag )
{
	piTag = tag;
}

/*
---------------------------------------------------------------------------
	setValue
---------------------------------------------------------------------------
*/
void Element::setValue( const STRING & )
{
}

void XmlText::setValue( const STRING &src )
{
	cData = src;
}

void ProcInstruction::setValue( const STRING &src )
{
	processingInstruction = src;
}

/*
---------------------------------------------------------------------------
	getValue
---------------------------------------------------------------------------
*/
STRING XmlText::getValue( GeneratorMode ) const
{
	return cData;
}

STRING PCData::getValue( GeneratorMode mode ) const
{
	if( mode == PLAIN_MODE || (getParent() && getParent()->wantScriptCode()) )
		return cData;
	else if( mode == XML_MODE )
		return ascii2XML( cData );
	else
		return ascii2HTML( cData );
}

STRING XmlContainer::getValue( GeneratorMode mode ) const
{
	size_t		i, numObj;
	STRING		xmlCode;
	STRING		subTag, subData;

	Element	*lastElement, *curElement, *nextElement;

	lastElement = NULL;
	numObj = objects.size();
	if( numObj )
	{
		nextElement = objects[i=0];
		while( nextElement )
		{
			curElement = nextElement;

			subTag = curElement->getTag();
			subData = curElement->generate( mode );

			i++;
			nextElement = i<numObj ? objects[i] : NULL;

			if( mode == HTML_MODE
			&& subTag == "BR"
			&& xmlCode.rightString(1) != "\n" )
				xmlCode += '\n';

			if( subData != " "
			|| (lastElement && lastElement->isInline())
			|| (nextElement && nextElement->isInline()) )
			{
				xmlCode += subData;
				lastElement = curElement;
			}

			if( nextElement
			&& !xmlCode.endsWith( '\n' )
			&& mode != PLAIN_MODE
			&& !hasPCdata )
			{
				if( (mode == XML_MODE)
				|| (
						!curElement->isInline()
						|| !nextElement->isInline()
					)
				)
				{
					xmlCode += '\n';
				}
			}
		}
	}

	return xmlCode;
}

STRING Mark::getValue( GeneratorMode ) const
{
	return "";
}

STRING ProcInstruction::getValue( GeneratorMode ) const
{
	return processingInstruction;
}
/*
---------------------------------------------------------------------------
	generate
---------------------------------------------------------------------------
*/
STRING Element::generate( GeneratorMode mode ) const
{
	STRING		xmlCode;

	CI_STRING	tag = getTag();
	STRING		attributes = getAttributes( mode );
	STRING		value = getValue( mode );

	if( tag.isEmpty() && attributes.isEmpty() )
/***/	return value;

	xmlCode = ('<' + tag);

	if( !attributes.isEmpty() )
	{
		if( xmlCode[0U] )
			xmlCode += ' ';
		xmlCode += attributes;
	}

	if( !value.isEmpty() )
	{
		xmlCode += '>';

		if( (mode == XML_MODE && !hasPCdata) || wantScriptCode() )
			xmlCode += '\n';

		xmlCode += value;

		if( (mode == XML_MODE && !hasPCdata) || wantScriptCode() )
			xmlCode += '\n';

		xmlCode += "</" + tag + '>';
	}
	else
	{
		if( !isSimpleTag() )	// old html is not a real xml
								// but we still need this generation
		{
			if( mode == HTML_MODE )
			{
				xmlCode += '>';
				if( tag != "img" && tag != "br" && tag != "hr" && tag != "link"
				&& tag != "meta" )
					xmlCode += "</"+tag +">";		// end start tag and create a
													// html like end tag
			}
			else
				xmlCode += " />";
		}
		else
			xmlCode += '>';		// start tag termination
	}

	return xmlCode;
}

STRING CData::generate( GeneratorMode mode ) const
{
	if( mode == XML_MODE )
	{
		STRING value = "<![CDATA[";

		value += cData;
		value += "]]>";

		return value;
	}
	else
		return cData;
}

STRING Comment::generate( GeneratorMode ) const
{
	STRING xmlCode;

	if( cData > "" )
	{
		xmlCode = "<!--";
		if( !isspace( cData[0U] ) )
			xmlCode += ' ';

		xmlCode += cData;

		if( !isspace( cData.lastChar() ) )
			xmlCode += ' ';

		xmlCode += "-->";
	}

	return xmlCode;
}

STRING DocType::generate( GeneratorMode ) const
{
	STRING xmlCode;

	if( cData > "" )
	{
		xmlCode = "<!DOCTYPE";
		if( !isspace( cData[0U] ) )
			xmlCode += ' ';

		xmlCode += cData;

#if 0
		// why ???
		// XmlTest will fail
		if( !isspace( cData.lastChar() ) )
			xmlCode += ' ';
#endif

		xmlCode += '>';
	}

	return xmlCode;
}

STRING Mark::generate( GeneratorMode mode ) const
{
	STRING		xmlCode;

	STRING		tag = getTag();
	STRING		attributes = getAttributes( mode );
	STRING		value = getValue( mode );

	xmlCode = "<?";
	xmlCode+= tag;
	if( !attributes.isEmpty() )
	{
		xmlCode += ' ';
		xmlCode += attributes;
	}

	if( !value.isEmpty() )
	{
		xmlCode += ' ';
		xmlCode += value;
	}

	xmlCode += " ?>";

	return xmlCode;
}

/*
--------------------------------------------------------------------------
	Copy an XML Element
--------------------------------------------------------------------------
*/
Element *Any::createNew( void )
{
	Element *newElement = new Any( tag );

	return newElement;
}

Element *PCData::createNew( void )
{
	Element *newElement = new PCData( cData );

	return newElement;
}

Element *CData::createNew( void )
{
	Element *newElement = new CData( cData );

	return newElement;
}

Element *Comment::createNew( void )
{
	Element *newElement = new Comment( cData );

	return newElement;
}

Element *DocType::createNew( void )
{
	Element *newElement = new DocType( cData );

	return newElement;
}

Element *ProcInstruction::createNew( void )
{
	Element *newElement = new ProcInstruction(
		getpiTag(), processingInstruction
	);

	return newElement;
}

Element *Declaration::createNew( void )
{
	Element *newElement = new Declaration();

	return newElement;
}

Element *StyleSheet::createNew( void )
{
	Element *newElement = new StyleSheet();

	return newElement;
}

Element *Document::createNew( void )
{
	Element *newElement = new Document( m_fileName );

	return newElement;
}

void Element::transferData( Element *target )
{
	Element	*oldElement, *newElement;
	STRING	attributeName, attributeValue;

	for( size_t i=0; i<getNumObjects(); i++ )
	{
		oldElement = getElement( i );
		newElement = oldElement->copy();
		target->addObject( newElement, true );
	}
	for( size_t i=0; i<getNumAttributes(); i++ )
	{
		attributeName = getAttributeName( i );
		attributeValue = getAttribute( i );
		target->setStringAttribute( attributeName, attributeValue );
	}

	target->theValidator = theValidator;
	target->theComplexType = theComplexType;
	target->type = type;

	if( !theCssStyle && target->theCssStyle )
	{
		delete target->theCssStyle;
		target->theCssStyle = NULL;
	}
	else if( theCssStyle && !target->theCssStyle )
	{
		css::Styles	*targetStyle = target->getCssStyle();

		*targetStyle = *theCssStyle;
	}
	else if( theCssStyle && target->theCssStyle )
	{
		*theCssStyle = *theCssStyle;
	}
}

Element *Element::copy( void )
{
	Element *newElement = createNew();
	transferData( newElement );

	return newElement;
}


// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING Element::getInheritedAttribute( const STRING &attr ) const
{
	STRING	value = getAttribute( attr );
	if( value.isEmpty() )
	{
		Element *parent = getParent();
		if( parent )
		{
			value = parent->getInheritedAttribute( attr );
		}
	}

	return value;
}

STRING Element::getNamespace( void )
{
	STRING prefix = getPrefix();
	STRING namespaceAttr = "xmlns";

	if( !prefix.isEmpty() )
	{
		namespaceAttr += ':';
		namespaceAttr += prefix;
	}

	return getInheritedAttribute( namespaceAttr );
}

STRING Element::getPrefix4Namespace( const STRING &nameSpace )
{
	bool			prefixFound = false;
	STRING			attributeName, attributeValue;
	STRING			attributePrefix, thePrefix;
	Element		*parent;
	size_t			numAttributes = getNumAttributes();
	for( size_t i=0; i<numAttributes; i++ )
	{
		attributeValue = getAttribute( i );
		if( attributeValue == nameSpace )
		{
			attributeName = getAttributeName( i );
			if( attributeName == "xmlns" )
			{
				thePrefix = "";
				prefixFound = true;
				break;
			}
			attributePrefix = getPrefix( attributeName );
			if( attributePrefix == "xmlns" )
			{
				thePrefix = getLocalName( attributeName );
				prefixFound = true;
				break;
			}
		}
	}

	if( !prefixFound )
	{
		parent = getParent();
		if( parent )
			thePrefix = parent->getPrefix4Namespace( nameSpace );
	}

	return thePrefix;
}

XmlContainer *Element::getPrefixParent( STRING prefix ) const
{
	XmlContainer	*parent = getParent();

	while( parent )
	{
		if( parent->getPrefix() == prefix )
/*v*/		break;

		parent = parent->getParent();
	}

	return parent;
}

void Element::getPrefixe( ArrayOfStrings	*prefixe )
{
	STRING		attributeName, attributePrefix;
	Element		*parent;

	size_t			numAttributes = getNumAttributes();
	for( size_t i=0; i<numAttributes; i++ )
	{
		attributeName = getAttributeName( i );
		if( attributeName == "xmlns" )
		{
			*prefixe += STRING( "" );
		}
		else if( getPrefix( attributeName ) == "xmlns" )
		{
			*prefixe += getLocalName( attributeName );
		}
	}

	parent = getParent();
	if( parent )
		parent->getPrefixe( prefixe );
}

STRING Element::generateDoc( const STRING &dtd, const STRING &xslt ) const
{
	STRING	xmlCode = STRING("<?xml version=\"1.0\" encoding=\"")+ISO_8859_1+"\" ?>\n";
	if( !dtd.isEmpty() )
	{
		xmlCode += "<!DOCTYPE " + dtd + " SYSTEM \"" + dtd + ".dtd\">\n";
	}

	if( !xslt.isEmpty() )
	{
		xmlCode += "<?xml-stylesheet type=\"text/xsl\" href=\"" + xslt + "\" ?>\n";
	}

	xmlCode += generate( XML_MODE );

	return xmlCode;
}

STRING Document::generateDoc() const
{
	return getValue( XML_MODE );
}

void Element::changeElementPrefix( const STRING &newPrefix )
{
	STRING oldTag = getTag();
	size_t	colonPos = oldTag.searchChar( ':' );
	if( colonPos != oldTag.no_index )
	{
		oldTag += (colonPos+1U);
	}

	STRING newTag = newPrefix;
	if( !newTag.isEmpty() )
	{
		newTag += ':';
	}
	newTag += oldTag;

	setTag( newTag );
}

void Element::changePrefixInTree( const STRING &newPrefix )
{
	changeElementPrefix( newPrefix );

	size_t numElements = getNumObjects();
	for( size_t i=0; i<numElements; i++ )
	{
		Element	*theChild = getElement( i );
		if( theChild )
			theChild->changePrefixInTree( newPrefix );
	}
}

void Element::clearTypes( void )
{

	setType( NULL_STRING );
	setComplexType( NULL, NULL );
	for( size_t i=0; i<getNumObjects(); i++ )
	{
		getElement(i)->clearTypes();
	}
}

STRING Document::getStyleSheet( STRING *type )
{
	Element *theElement;

	for( size_t i=0; i<getNumObjects(); i++ )
	{
		theElement = getElement(i);
		StyleSheet	*theStyleSheet = dynamic_cast<StyleSheet *>(
			theElement
		);
		if( theStyleSheet )
		{
			*type = theStyleSheet->getStylesheetType();
			return theStyleSheet->getStyleSheet();
		}
	}

	*type = "";
	return "";
}

Element *Document::getRoot( void )
{
	Element *theElement, *theRoot = NULL;

	for( size_t i=0; i<getNumObjects(); i++ )
	{
		theElement = getElement(i);
		theRoot = dynamic_cast<XmlContainer *>(
			theElement
		);
		if( theRoot )
			break;
	}

	return theRoot;
}

Element *Document::getRoot( const STRING &tag, const STRING &nameSpace )
{
	Element	*theRoot = getRoot();
	if( theRoot )
	{
		if( theRoot->getLocalName() != tag && theRoot->getNamespace() != nameSpace )	// our root is not the one
		{
			// try to walk down the tree for our element
			theRoot = theRoot->findElement( tag, nameSpace );
		}
	}

	return theRoot;
}

Element *Element::findRoot( void ) const
{
	if( parent )
		return parent->findRoot();
	else
		return (Element *)this;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace xml
}	// namespace gak

