/*
		Project:		GAKLIB
		Module:			xslt.cpp
		Description:	XSLT-Processor
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

#include <cctype>
#include <limits>

#include <gak/htmlParser.h>
#include <gak/xslt.h>
#include <gak/XPath.h>
#include <gak/directory.h>
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

extern const char XSL_OUTPUT[]	= "output";
extern const char XSL_METHOD[]	= "method";
extern const char XSL_HTML[]	= "html";
extern const char XSL_XML[]		= "xml";
extern const char XSL_SYSTEM[]	= "doctype-system";
extern const char XSL_PUBLIC[]	= "doctype-public";

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

Transformator::Transformator(
	Document *sourceDoc, STRING xslFile
)
{
	Element	*xsltRoot = NULL;
	this->sourceDoc = sourceDoc;

	
	if( xslFile.isEmpty() )
	{
		STRING	type;
		xslFile = sourceDoc->getStyleSheet( &type );

		if( type != "text/xsl" )
			xslFile = "";
		else if( !xslFile.isEmpty() )
			xslFile = makeFullPath( sourceDoc->getFilename(),xslFile );
	}
	if( !xslFile.isEmpty() )
	{
		Parser	theParser( xslFile );
		Document	*theXsl = theParser.readFile( false );

		if( theXsl )
			xsltRoot = theXsl->getElement( "stylesheet", XMLNS_XSLT );
	}

	this->xsltRoot = xsltRoot;
	theFactory = NULL;
	resultDoc = NULL;
	omitXmlDecl = false;
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void Transformator::forEach(
	Element	*theTemplate,
	const STRING &select,
	Element *source,
	Element *target
)
{
	XmlArray	docElements;

	source->locateElements( select, &docElements, false );

	for( docElements.first(); docElements.current(); docElements.next() )
	{
		transform( theTemplate, &docElements, target );
	}
}

void Transformator::applyTemplates(
	const STRING &select,
	const STRING &mode,
	Element *source,
	Element *target
)
{
	doEnterFunction("Transformator::applyTemplates");

	Element	*sourceChild, *minTemplate, *theTemplate;
	XmlArray	sourceChildren, templateMatch;
	STRING		match, tmplMode;
	size_t		minCount;

	if( !allTemplates.size() )
	{
		xsltRoot->getChildElements( &allTemplates, "template", XMLNS_XSLT );
	}
	source->locateElements( select, &sourceChildren, false	);

	for(
		sourceChildren.first();
		(sourceChild = sourceChildren.current()) != NULL;
		sourceChildren.next()
	)
	{
		minCount = std::numeric_limits<std::size_t>::max();
		minTemplate = NULL;
		for(
			allTemplates.first();
			(theTemplate = allTemplates.current()) != NULL;
			allTemplates.next()
		)
		{
			match = theTemplate->getAttribute( "match" );
			tmplMode = theTemplate->getAttribute( "mode" );
			if( !match.isEmpty() && mode == tmplMode )
			{
				templateMatch.clear();
				source->locateElements( match, &templateMatch, false );
				if( templateMatch.findElement( sourceChild ) != templateMatch.no_index )
				{
					if( templateMatch.size() < minCount )
					{
						minCount = templateMatch.size();
						minTemplate = theTemplate;
					}
				}
			}
		}

		if( minTemplate )
		{
			transform( minTemplate, &sourceChildren, target );
		}
		else
		{
			if( dynamic_cast<Any*>(sourceChild)
			|| dynamic_cast<Document*>(sourceChild) )
				applyTemplates( "*", "", sourceChild, target );
			else if( dynamic_cast<PCData*>(sourceChild) && target )
			{
				target->addObject(
					new PCData(
						sourceChild->getValue( PLAIN_MODE )
					),
					true
				);
			}
			else if( dynamic_cast<CData*>(sourceChild) && target )
			{
				target->addObject(
					new CData(
						sourceChild->getValue( PLAIN_MODE )
					),
					true
				);
			}
		}
	}
}

void Transformator::transform(
	Element *theTemplate, XmlArray *source, Element *target
)
{
	doEnterFunction("private Transformator::transform");
	for( size_t	i=0; i<theTemplate->getNumObjects(); i++ )
	{
		Element	*tmplElement = theTemplate->getElement( i );
		STRING		tmplTag = tmplElement->getLocalName();
		if( tmplTag.isEmpty() )
		{
			if( target )
				target->addObject(
					new CData( tmplElement->getValue( PLAIN_MODE ) ),
					true
				);
			// otherwise ignore
		}
		else if( tmplElement->getPrefix() == xsltRoot->getPrefix() )
		{
			if( tmplTag == "value-of" )
			{
				if( target )
				{
					target->addObject(
						new PCData(
							valueOf(
								source, tmplElement->getAttribute( "select" )
							)
						),
						true

					);
				}
				// otherwise ignore
			}
			else if( tmplTag == "attribute" )
			{
				if( target )
				{
					STRING	attrName = tmplElement->getAttribute( "name" );
					if( !attrName.isEmpty() )
					{
						Any *tmpTarget = new Any( "tmpTarget" );
						transform( tmplElement, source, tmpTarget );
						STRING	attrValue = tmpTarget->getValue( PLAIN_MODE );
						attrValue.stripBlanks();
						target->setStringAttribute( attrName, attrValue );

						delete tmpTarget;
					}
				}
				// otherwise ignore
			}
			else if( tmplTag == "text" )
			{
				if( target )
				{
					target->addObject(
						new CData(
							tmplElement->getValue( PLAIN_MODE )
						),
						true
					);
				}
				// otherwise ignore
			}
			else if( tmplTag == "if" )
			{
				STRING	test = tmplElement->getAttribute( "test" );
				STRING	result = evaluate( test, source );
				if( !result.isEmpty() && result != "0" )
				{
					transform( tmplElement, source, target );
				}

			}
			else if( tmplTag == "apply-templates" )
			{
				STRING	select = tmplElement->getAttribute( "select" );
				STRING	mode = tmplElement->getAttribute( "mode" );
				if( select.isEmpty() )
					select = "*";
				applyTemplates( select, mode, source->current(), target );
			}
			else if( tmplTag == "copy-of" )
			{
				STRING	select = tmplElement->getAttribute( "select" );
				copyOf( source->current(), select, target );
			}
			else if( tmplTag == "for-each" )
			{
				STRING	select = tmplElement->getAttribute( "select" );
				forEach( tmplElement, select, source->current(), target );
			}
			else if( tmplTag == "processing-instruction" )
			{
				if( !theFactory )
				{
					this->method = XO_XML;
					theFactory = new Factory;
					resultDoc = new Document( sourceDoc->getFilename() );
				}

				if( !target )
					target = resultDoc;

				STRING	name = tmplElement->getAttribute( "name" );
				STRING	value = tmplElement->getValue( PLAIN_MODE );
				if( name == "xml-stylesheet" )
				{
					size_t	pos = value.searchText( "href" );
					size_t	sPos = value.searchChar( '\"', pos );
					size_t	ePos = value.searchChar( '\"', sPos+1 );
					if( sPos == value.no_index )
					{
						sPos = value.searchChar( '\'', pos );
						ePos = value.searchChar( '\'', sPos+1 );
					}
					if( sPos != value.no_index && ePos != value.no_index )
					{
						STRING href=value.subString( sPos+1, ePos-sPos-1 );
						if( !href.isEmpty() )
						{
							target->addObject( new StyleSheet( href ) );
						}
					}
				}
				else
					target->addObject(
						new ProcInstruction( name, value )
					);
			}
		}
		else
		{
			if( !theFactory )
			{
				if( tmplTag == "html" || tmplTag == "HTML" )
				{
					this->method = XO_HTML;
					theFactory = new html::Factory;
					resultDoc = new html::Document( sourceDoc->getFilename() );
				}
				else
				{
					this->method = XO_XML;
					theFactory = new Factory;
					resultDoc = new Document( sourceDoc->getFilename() );
				}
			}

			if( !target )
				target = resultDoc;

			// still no target? --> Transformation failed
			if( target )
			{
				Element	*newElement = target->addObject(
					theFactory->createObject( tmplElement->getTag() ),
					true
				);
				for( size_t i=0; i<tmplElement->getNumAttributes(); i++ )
				{
					newElement->setStringAttribute(
						tmplElement->getAttributeName( i ),
						tmplElement->getAttribute( i )
					);
				}
				transform( tmplElement, source, newElement );
			}
		}
	}
}

void Transformator::copyOf(
	Element *source, const STRING &select, Element *target
)
{
	doEnterFunction( "Transformator::copyOf" );

	if( target )
	{
		XmlArray	docElements;

		source->locateElements( select, &docElements, true );

		for( size_t i=0; i<docElements.size(); i++ )
		{
			Element	*theSource = docElements[i];

			STRING newTag = theSource->getTag();
			if( !newTag.isEmpty() )
			{
				Element	*newElement = target->addObject(
					theFactory->createObject( theSource->getTag() ),
					true
				);

				for( size_t j=0; j<theSource->getNumAttributes(); j++ )
				{
					newElement->setStringAttribute(
						theSource->getAttributeName( j ),
						theSource->getAttribute( j )
					);
				}
				copyOf( theSource, "*", newElement );
			}
			else
			{
				target->addObject(
					new PCData( theSource->getValue( PLAIN_MODE ) ),
					true
				);
			}
		}
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

Document *Transformator::transform( void )
{
	doEnterFunction("Transformator::transform");

	XmlArray	xsltTemplates;

	Element	*output = xsltRoot->getElement( XSL_OUTPUT, XMLNS_XSLT );
	if( output )
	{
		STRING method = output->getAttribute( XSL_METHOD );
		if( method == "html" )
		{
			this->method = XO_HTML;
			theFactory = new html::Factory;
			resultDoc = new html::Document( sourceDoc->getFilename() );
		}
		else if( method == "xml" )
		{
			this->method = XO_XML;
			theFactory = new Factory;
			resultDoc = new Document( sourceDoc->getFilename() );
		}
		version = output->getAttribute( "version" );
		encoding = ISO_8859_1;
		systemDocType = output->getAttribute( XSL_SYSTEM );
		publicDocType = output->getAttribute( XSL_PUBLIC );
		STRING tmp = output->getAttribute( "omit-xml-declaration" );
		if( tmp=="yes" )
			omitXmlDecl = true;
	}

	applyTemplates( "/", "", sourceDoc, resultDoc );

	if( resultDoc )
	{
		Element		*xmlDecl = NULL, *xmlDocType = NULL;
		if( method == XO_XML )
		{
			if( !omitXmlDecl )
			{
				xmlDecl = resultDoc->addObject( new Declaration );
				if( version.isEmpty() )
					version = "1.0";

				xmlDecl->setStringAttribute( "version", version );
				xmlDecl->setStringAttribute( "encoding", encoding );
			}
		}
		if( !systemDocType.isEmpty() || !publicDocType.isEmpty() )
		{
			Element	*root = resultDoc->getRoot();
			if( root )
			{
				STRING docType = root->getTag();
				if( !publicDocType.isEmpty() )
				{
					docType += " PUBLIC \"";
					docType += publicDocType;
					docType += '\"';
				}
				if( !systemDocType.isEmpty() )
				{
					if( !publicDocType.isEmpty() )
						docType += " \"";
					else
						docType += " SYSTEM \"";
					docType += systemDocType;
					docType += '\"';
				}
				xmlDocType = resultDoc->addObject(
					new DocType( docType )
				);
			}
		}
		if( xmlDocType )
			xmlDocType->moveTo( 0 );
		if( xmlDecl )
			xmlDecl->moveTo( 0 );

		if( method==XO_HTML )
		{
			Element	*html = resultDoc->getElement( "HTML" );
			if( html )
			{
				Element *head = html->getElement( "HEAD" );
				if( head )
				{
					html::META *meta = new html::META;
					meta->setStringAttribute( "http-equiv", "Content-Type" );
					meta->setStringAttribute(
						"content", STRING("text/html; charset=") + encoding
					);

					head->addObject( meta );
				}
			}
		}
	}
	return resultDoc;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace xml
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

