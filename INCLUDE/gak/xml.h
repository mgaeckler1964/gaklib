/*
		Project:		GAKLIB
		Module:			XML.H
		Description:	XML-Generator
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

#ifndef GAK_XML_GENERATOR_H
#define GAK_XML_GENERATOR_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/string.h>
#include <gak/array.h>
#include <gak/fieldSet.h>
#include <gak/css.h>
#include <gak/fmtNumber.h>
#include <gak/logfile.h>
#include <gak/textReader.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	we must allow RTTI
	otherwise we cannot use dynamioc_cast
*/
#ifdef __BORLANDC__
#	pragma option -RT+
#	pragma option -b
#	pragma option -a4
#	pragma option -pc

#	pragma warn -inl
#endif

namespace gak
{
namespace xml
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// Known namespaces

extern const char XMLNS_SCHEMA[];
extern const char XMLNS_SCHEMA_INSTANCE[];
extern const char XMLNS_XSLT[];
extern const char XMLNS_SOAP[];
extern const char XMLNS_SOAP_ENVELOPE[];
extern const char XMLNS_WSDL[];

// Known attributes
extern const char XMLNS_ATTRIBUTE[];
extern const char XMLNS_SCHEMALOCATION[];
extern const char XMLNS_NS_SCHEMALOCATION[];

extern const char XSLT_PUBLIC[];

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	Any XML Element
*/

enum GeneratorMode
{
	PLAIN_MODE, XML_MODE, XHTML_MODE, HTML_MODE
};

class XmlContainer;
class Validator;
class Element;

typedef Element					*ElementPtr;

class XmlArray : public PODarray<ElementPtr>
{
	size_t	currentPos;

	public:
	size_t	position() const
	{
		return currentPos+1;
	}
	size_t last() const
	{
		return size();
	}

	ElementPtr current() const
	{
		return currentPos < size() ? getConstElementAt( currentPos ) : NULL;
	}
	ElementPtr first()
	{
		currentPos = 0;
		return current();
	}
	ElementPtr next()
	{
		currentPos++;
		return current();
	}
	ElementPtr prev()
	{
		if( currentPos )
			currentPos--;
		return current();
	}
	ElementPtr removeCurrent()
	{
		removeElementAt( currentPos );
		return current();
	}
};

class Element
{
	friend	class XmlContainer;

	private:
	XmlContainer	*parent;

	// elements for the validator
	Validator		*theValidator;
	Element			*theComplexType;
	Element			*theSimpleType;
	STRING			type;

	css::Styles		*theCssStyle;

	protected:
	bool			hasPCdata,
					byParser;

	Element()
	{
		parent = NULL;
		theValidator = NULL;
		theComplexType = NULL;
		theSimpleType = NULL;
		hasPCdata = false;
		theCssStyle = NULL;
		byParser = false;
	};

	public:
	virtual ~Element();

	virtual STRING getTag() const = 0;
	virtual void setTag( const STRING &tag );
	STRING getPath( bool includeIndex=true );

	virtual STRING getValue( GeneratorMode mode ) const = 0;
	virtual void setValue( const STRING &newValue );
	virtual bool isInline() = 0;
	virtual bool isBlock() = 0;
	virtual bool isSimpleTag() const = 0;		// for HTML Generation
	virtual bool isVisual();
	virtual STRING generate( GeneratorMode mode ) const;
	STRING generateDoc(
		const STRING &dtd = NULL_STRING, const STRING &xslt=NULL_STRING
	) const;

	/*
		copying
	*/
	virtual Element *createNew() = 0;
	virtual Element *copy();
	virtual void transferData( Element *target );

	/*
		schema validation
	*/
	private:
	STRING getSchemaLocation( Element **theElement, const STRING &prefix );
	Validator *getValidator( const STRING &prefix ) const;

	public:
	Validator *getValidator() const
	{
		/*
			if there is no parent and no validator,
			we do not have any chance to find one
		*/
		if( !theValidator && !parent )
			return NULL;

		return getValidator( getPrefix() );
	}
	STRING getSchemaLocation( Element **theElement )
	{
		return getSchemaLocation( theElement, getPrefix() );
	}


	Element	*getComplexType() const
	{
		return theComplexType;
	}

	void setComplexType( Validator *validator, Element *complexType )
	{
		theValidator = validator;
		theComplexType = complexType;
		theSimpleType = NULL;
	}
	Element	*getSimpleType() const
	{
		return theSimpleType;
	}

	void setSimpleType( Validator *validator, Element *simpleType )
	{
		theValidator = validator;
		theSimpleType = simpleType;
		theComplexType = NULL;
	}

	void setType( const STRING &newType )
	{
		type = newType;
	}
	const STRING &getType() const
	{
		return type;
	}
	void clearTypes();

	/*
		attribute handling functions
	*/
	virtual bool		isValidAttribute( const STRING &name, const STRING &value );
	virtual void		getValidAttributes( ArrayOfStrings *names );
	virtual void		getValidAttributeValues(
		size_t attrIdx, ArrayOfStrings *values
	);
	virtual Element	*setStringAttribute( const STRING &name, const STRING &value );
	virtual Element	*setStringAttribute( size_t i, const STRING &value );

	template <typename IntegerT>
	Element	*setIntegerAttribute( const STRING &name, IntegerT value )
	{
		STRING	strValue = formatNumber( value );

		return setStringAttribute( name, strValue );
	}

	Element	*setFloatAttribute( const STRING &name, double value, int digits=2 )
	{
		STRING	strValue = formatFloat( value, 0, digits );

		return setStringAttribute( name, strValue );
	}
	Element	*setBoolAttribute( const STRING &name, bool value )
	{
		STRING	strValue = formatBool( value );
		return setStringAttribute( name, strValue );
	}

	virtual void		setAttributeName( size_t i, const STRING &name );
	virtual void		deleteAttribute( const STRING &name );
	virtual void		deleteAttribute( size_t i );
	virtual STRING		getAttributes( GeneratorMode mode ) const;
	virtual STRING		getAttribute( const STRING &name ) const;
	STRING 				getInheritedAttribute( const STRING &attr ) const;
	virtual STRING		getAttribute( size_t i ) const;
	virtual STRING		getAttributeName( size_t i ) const;
	virtual size_t		getNumAttributes() const;

	virtual bool		isCaseSensitive() const;

	STRING getClass()
	{
		return getAttribute( "class" );
	}
	STRING getId()
	{
		return getAttribute( "id" );
	}
	STRING getStyle()
	{
		return getAttribute( "style" );
	}

	/*
		object hierarchy functions
	*/
	virtual bool		wantScriptCode() const;

	virtual Validator *getValidSubobjects( ArrayOfStrings *tags );
	virtual bool isValidSubobject( Element *newObject );
	virtual Element	*addObject(
		Element *newObject, bool parserMode=false
	);
	virtual Element	*replaceObject(
		Element *oldObject, Element *newObject,
		bool transferChildren=false
	);
	virtual Element	*removeObject( Element *oldObject );
	virtual Element	*removeObject( size_t i );
	void remove();

	virtual Element *getElement( size_t i ) const;
	virtual Element *getElement(
		const STRING &tag, const STRING &nameSpace=NULL_STRING
	) const;

	void moveTo( Element *newParent, size_t newIndex )
	{
		remove();
		newParent->addObject( this );
		moveTo( newIndex );
	}

	//search recursively for an element
	Element *findElement(
		const STRING &tag, const STRING &nameSpace=NULL_STRING
	) const;
	// get recursively all child elements with a tag
	void getAllElements(
		XmlArray *elements, const STRING &tag, const STRING &nameSpace=NULL_STRING
	) const;
	// get all direct children with a tag
	void getChildElements(
		XmlArray *elements, const STRING &tag, const STRING &nameSpace=NULL_STRING
	) const;
//	void getChildElements( XmlArray *elements );
	void getPrefixChildElements(
		XmlArray *elements, const STRING &prefix
	) const;
	virtual std::size_t getNumObjects() const;
	XmlContainer *getParent() const
	{
		return parent;
	}
	XmlContainer *getPrefixParent( STRING prefix ) const;
	XmlContainer *getPrefixParent() const
	{
		return getPrefixParent( getPrefix() );
	}
	Element *findRoot() const;

	Element *getPrevious();
	long getIndex();
	void moveTo( size_t index );

#if 0
	void setParent(XmlContainer *parent)
	{
		if( this->parent )
			remove();
		this->parent = parent;
	}
#endif

	/*
		namespace relating functions
	*/
	STRING getPrefix4Namespace( const STRING &nameSpace );
	STRING getNamespace();
	STRING getPrefix() const
	{
		return getPrefix( getTag() );
	}
	void getPrefixe( ArrayOfStrings	*prefixe );
	void changePrefixInTree( const STRING &newPrefix );
	void changeElementPrefix( const STRING &newPrefix );
	STRING getLocalName() const
	{
		return getLocalName( getTag() );
	}
	static STRING getPrefix( const STRING &text );
	static STRING getLocalName( const STRING &text );
	bool testNamespaceAndTag( const STRING &nameSpace, const STRING &tag )
	{
		return getNamespace() == nameSpace && getLocalName() == tag;
	}
	void setNamespace( const STRING &nameSpace )
	{
		setStringAttribute( XMLNS_ATTRIBUTE, nameSpace );
	}
	void setNamespace( const STRING &prefix, const STRING &nameSpace )
	{
		setStringAttribute( STRING(XMLNS_ATTRIBUTE) + ':' + prefix, nameSpace );
	}

	/*
		conversion
	*/
	static STRING ascii2XML( char c );
	static STRING ascii2HTML( char c );
	static STRING ascii2XML( const STRING &cData );
	static STRING ascii2HTML( const STRING &cData );

	static char xml2ASCII( const STRING &c );
	static char html2ASCII( const STRING &c );

	/*
		XPath
	*/
	void getChildren( XmlArray *result, bool inclPcData );
	void getDescendants( XmlArray *result, bool inclPcData );
	void getReverseDescendants( XmlArray *result, bool inclPcData );
	void getFollowing( XmlArray *result, bool inclPcData );
	void getFollowingSiblings( XmlArray *result, bool inclPcData );
	void getPrecedingSiblings( XmlArray *result, bool inclPcData );
	void getPrecedings( XmlArray *result, bool inclPcData );

	STRING locateElements( const STRING &path, XmlArray *result, bool inclPcData );

	/*
		CSS Styles
	*/
	css::Styles *getCssStyle();
	void clearCss();
	int getTextDecorations();
};

/*
	a simple string object
*/
class XmlText : public Element
{
	protected:
	STRING	cData;

	public:
	XmlText( const STRING &theCdata )
	{
		cData = theCdata;
	}
	XmlText( const char *theCdata )
	{
		cData = theCdata;
	}

	virtual STRING	getTag() const;
	virtual STRING	getValue( GeneratorMode mode ) const;
	virtual void	setValue( const STRING &theCdata );
	virtual bool 	isInline();
	virtual bool 	isBlock();
	virtual bool 	isSimpleTag() const;
	virtual bool 	isVisual();
};

/*
	the parsed data string
*/
class PCData : public XmlText
{
	public:
	PCData( const STRING &theCdata ) : XmlText( theCdata ) {}
	PCData( const char *theCdata ) : XmlText( theCdata ) {}

	virtual STRING	getValue( GeneratorMode mode ) const;
	virtual Element *createNew();
};

class Comment : public XmlText
{
	public:
	Comment( const char *theCdata ) : XmlText( theCdata ) {}
	Comment( const STRING &theCdata ) : XmlText( theCdata ) {}
	virtual STRING generate( GeneratorMode mode ) const;
	virtual bool isVisual();
	virtual Element *createNew();
};

class DocType : public XmlText
{
	public:
	DocType( const char *theCdata ) : XmlText( theCdata ) {}
	DocType( const STRING &theCdata ) : XmlText( theCdata ) {}
	virtual STRING generate( GeneratorMode mode ) const;
	virtual bool isVisual();
	virtual Element *createNew();
};

class CData : public XmlText
{
	public:
	CData( const STRING &theCdata ) : XmlText( theCdata ) {}
	CData( const char *theCdata ) : XmlText( theCdata ) {}

	virtual STRING	generate( GeneratorMode mode ) const;

	virtual Element *createNew();
};


/*
------------------------------------------------
Here begins the normal xml element
------------------------------------------------
*/
class XmlWithAttributes : public Element
{
	private:
	FieldSet	attributes;

	protected:
	static  STRING	formatAttribute(
		const STRING &name, const STRING &value, GeneratorMode mode
	);
	static  STRING	formatAttribute(
		const STRING &name, long value, GeneratorMode mode
	);

	public:
	virtual void getValidAttributes( ArrayOfStrings *names );
	virtual void getValidAttributeValues(
		size_t attrIdx, ArrayOfStrings *values
	);
	virtual bool isValidAttribute( const STRING &name, const STRING &value );
	virtual Element *setStringAttribute( size_t i, const STRING &value );
	virtual Element *setStringAttribute( const STRING &name, const STRING &value );

	using Element::setStringAttribute;

	virtual void setAttributeName( size_t i, const STRING &name );
	virtual void deleteAttribute( const STRING &name );
	virtual void deleteAttribute( size_t i );
	virtual STRING getAttribute( const STRING &attrName ) const;
	virtual STRING getAttribute( size_t i ) const;
	virtual STRING getAttributeName( size_t i ) const;
	virtual STRING getAttributes( GeneratorMode mode ) const;
	virtual size_t getNumAttributes() const;
};

/*
	this is a xml tag with a container
*/

class XmlContainer : public XmlWithAttributes
{
	private:
	XmlArray	objects;

	public:
	~XmlContainer();

	virtual STRING	getValue( GeneratorMode mode ) const;
	virtual Element *getElement( size_t i ) const;
	virtual Element *getElement(
		const STRING &tag, const STRING &nameSpace=""
	) const;
	virtual size_t		getNumObjects() const;

	virtual Validator *getValidSubobjects( ArrayOfStrings *tags );
	virtual bool isValidSubobject( Element *newObject );
	virtual Element *addObject(
		Element *newObject, bool parserMode=false
	);
	virtual Element	*replaceObject(
		Element *oldObject, Element *newObject,
		bool transferChildren=false
	);
	virtual Element *removeObject( Element *oldObject );
	virtual Element *removeObject( size_t i );

	template <class SortFunction, class SortType>
	void sort(
		SortFunction	fcmp,
		SortType		sortType
	)
	{
		objects.sort( fcmp, sortType );
	}

	long getIndex( const Element *oldObject );
	void moveTo( Element *oldObject, size_t index )
	{
		long oldIndex = getIndex( oldObject );
		if( oldIndex >= 0 )
			objects.moveElement( (size_t)oldIndex, index );
	}

	Element *getPrevious( Element *oldObject )
	{
		long index = getIndex( oldObject );
		if( index > 0 )
			return getElement( (size_t)(index-1) );
		else
			return NULL;
	}
};

/*
	any dynamicaly generated XML object
*/
class Any : public XmlContainer
{
	private:
	STRING	tag;

	public:
	Any( const char *theTag ) : tag( theTag ) {}
	Any( const STRING &theTag ) : tag( theTag ) {}

	Any( const STRING &theTag, const STRING &cData )
	{
		tag = theTag;
		addObject( new CData( cData ) );
	}

	virtual STRING getTag() const;
	virtual void setTag( const STRING &tag );
	virtual bool isInline();
	virtual bool isBlock();
	virtual bool isSimpleTag() const;
	virtual bool isVisual();
	virtual Element *createNew();
};

/*
	abstract processing instruction in form
	<?tag ... ?>
*/
class Mark : public XmlWithAttributes
{
	STRING	piTag;

	protected:
	STRING &getpiTag()
	{
		return piTag;
	}
	public:
	virtual STRING getTag() const;
	virtual void setTag( const STRING &tag );

	virtual STRING getValue( GeneratorMode mode ) const;
	virtual bool isInline();
	virtual bool isBlock();
	virtual bool isSimpleTag() const;		// for HTML Generation

	virtual STRING generate( GeneratorMode mode ) const;
	virtual Validator *getValidSubobjects( ArrayOfStrings *tags );
	virtual bool isValidSubobject( Element *newObject );

	Mark( const STRING &theTag) : piTag( theTag ) {}
};

/*
	any processing instruction not known by this lib
*/
class ProcInstruction : public Mark
{
	STRING	processingInstruction;

	public:
	ProcInstruction( const STRING &theTag, const STRING &pi )
	: Mark( theTag ), processingInstruction(pi)
	{}
	virtual void setValue( const STRING &newValue );
	virtual STRING	getValue( GeneratorMode mode ) const;
	virtual Element *createNew();
};

/*
	base for special processing instructions
*/
class Special : public Mark
{
	protected:
	Special( const STRING &theTag) : Mark( theTag )
	{
	}

};

/*
	the XML Declaration
	<?xml version="..." encoding="..." ?>
*/
class Declaration : public Special
{
	public:
	Declaration()
	: Special( "xml" )
	{
		setStringAttribute( "version", "1.0" );
		setStringAttribute( "encoding", ISO_8859_1 );
	}
	virtual Element *createNew();
};

/*
	the xml-stylesheet
	<?xml-stylesheet href="..." style="..." ?>
*/
class StyleSheet : public Special
{
	public:
	StyleSheet( const char *href = "" )
	: Special( "xml-stylesheet" )
	{
		setStringAttribute( "type", "text/xsl" );
		setStringAttribute( "href", href );
	}
	STRING getStyleSheet()
	{
		return getAttribute( "href" );
	}
	STRING getStylesheetType()
	{
		return getAttribute( "type" );
	}
	virtual Element *createNew();
};

/*
	a XML Document
*/
class Document : public XmlContainer
{
	css::Rules	cssRules;
	F_STRING	m_fileName;

	css::Value findCssValue(
		Element *element, const CI_STRING &media, size_t offset
	);

	css::Value *getCssValue(
		Element *element, const CI_STRING &media,
		size_t offset, bool inherit, const char *defValue
	);
	css::Value findBackgroundImage( Element *element, const CI_STRING &media );
	void applyCssRules( Element *theRoot, const CI_STRING &media );

	public:
	Document( const STRING &theFile ) : m_fileName( theFile ) {}

	virtual STRING getTag() const;
	virtual bool isInline();
	virtual bool isBlock();
	virtual bool isSimpleTag() const;

	virtual STRING generateDoc() const;
	const F_STRING &getFilename() const
	{
		return m_fileName;
	}
	void setFilename( const STRING &fileName )
	{
		m_fileName = fileName;
	}

	void clearCssRules()
	{
		cssRules.clear();
		clearCss();
	}
	void setCssRules( const css::Rules &source )
	{
		clearCssRules();
		cssRules = source;
	}
	void readCssRules( bool toLowerCase );
	void readCssRules( const STRING &styles, bool toLowerCase );
	void readCssRules( std::istream *theInputStream, bool toLowerCase )
	{
		doEnterFunction("Document::readCssRules( std::istream *theInputStream, bool toLowerCase )");
		cssRules.readCssFile( theInputStream, toLowerCase );
	}
	void applyCssRules( const CI_STRING &media="screen" )
	{
		Element *theRoot = getRoot();

		if( theRoot && cssRules.size() )
		{
			applyCssRules( theRoot, media );
		}
	}

	STRING getBackgroundImage( Element *element, const CI_STRING &media );

	STRING getStyleSheet( STRING *type );

	Element *getRoot();
	Element *getRoot( const STRING &tag, const STRING &nameSpace );

	virtual Element *setStringAttribute( size_t i, const STRING &value );
	virtual Element *setStringAttribute( const STRING &name, const STRING &value );
	void setAttributeName( size_t i, const STRING &name );
	bool isValidAttribute( const STRING &name, const STRING &value );
	virtual Element *createNew();
};

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
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

inline Element *Element::getPrevious()
{
	return parent->getPrevious( this );
}

inline long Element::getIndex()
{
	return parent->getIndex( this );
}

inline void Element::moveTo( size_t index )
{
	parent->moveTo( this, index );
}

inline void Element::remove()
{
	parent->removeObject( this );
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

}	// namespace xml
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.

#	pragma warn +inl
#endif

#endif

