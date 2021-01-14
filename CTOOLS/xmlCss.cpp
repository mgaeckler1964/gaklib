/*
		Project:		GAKLIB
		Module:			XMLCSS.CPP
		Description:	CSS-specific functions of the XML-classes
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

#include <fstream>

#include <gak/xml.h>
#include <gak/directory.h>
#include <gak/stringStream.h>
#include <gak/logfile.h>

// -------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -x
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{
namespace xml
{

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

css::Value Document::findCssValue(
	Element *element, const CI_STRING &media, size_t offset
)
{
	doEnterFunction("Document::findCssValue");

	bool			matchFound;
	unsigned long	newSpecification, specification = 0;
	css::Value		cssValue, *styleValue;
	Element			*parent;

	size_t			numElements = cssRules.size();
	size_t			i = numElements - 1;

	while( i < numElements )
	{
		css::Rule &theRule = cssRules[i--];

		if( !theRule.media.isEmpty() && theRule.media != media )
/*^*/		continue;

		styleValue = theRule.styles.cssValue( offset );
		if( styleValue->isEmpty() )
/*^*/		continue;

		for( size_t j=0; j<theRule.selectorList.size(); j++ )
		{
			matchFound = true;

			css::Selector	&theSelector = theRule.selectorList[j];

			if( !theSelector.match( element ) )
/*^*/			continue;

			if( theSelector.hasPredessor() )
			{
				parent = element;
				for( size_t k=theSelector.size()-2; k<theSelector.size(); k-- )
				{
					matchFound = false;
					const css::SelectorPart &theSpec = theSelector.getElement(k);

					if( theSpec.isSibling() )
					{
						parent = parent->getPrevious();
						if( theSpec.match( parent ) )
						{
							matchFound = true;
						}
					}
					else
					{
						for(
							parent = parent->getParent();
							parent;
							parent = parent->getParent()
						)
						{

							if( theSpec.match( parent ) )
							{
								matchFound = true;
/*v*/							break;
							}

							if( theSpec.isParent() )	// only one parent ?
/*v*/							break;
						}
					}
					if( !parent || !matchFound )
/*v*/					break;
				}
			}

			// this rule matches
			if( matchFound )
			{
				newSpecification = theSelector.getSpecification();
				if( cssValue.isEmpty()
				||  (
					newSpecification > specification
					&& cssValue.isImportant() == styleValue->isImportant()
				)
				||  cssValue.isImportant() < styleValue->isImportant() )
				{
					cssValue = *styleValue;
					specification = newSpecification;
				}
			}
		}
	}
	return cssValue;
}

css::Value *Document::getCssValue(
	Element *element, const CI_STRING &media, size_t offset,
	bool inherit, const char *defValue
)
{
	doEnterFunction("Document::getCssValue");

	css::Value	*cssValue = element->getCssStyle()->cssValue( offset );

	if( !cssValue->isChecked() )
	{
		*cssValue = findCssValue( element, media, offset );

		if( (cssValue->isEmpty() && inherit) || *cssValue == "inherit" )
		{
			Element *parent = element->getParent();
			if( parent )
			{
				css::Value *parentValue = getCssValue(
					parent, media, offset, true, defValue
				);
				*cssValue = *parentValue;
				cssValue->setInherited();
			}
		}

		if( cssValue->isEmpty() && defValue )
			*cssValue = defValue;

		cssValue->setChecked();
	}
	// element->getCssStyle()->cssValue( offset ) = cssValue;

	return cssValue;
}

css::Value Document::findBackgroundImage(
	Element *element, const CI_STRING &media
)
{
	bool			matchFound;
	unsigned long	newSpecification, specification = 0;
	css::Value		backgroundImage;
	Element			*parent;

	STRING			elementStyle = element->getStyle();

	if( !elementStyle.isEmpty() )
	{
		css::Styles	theStyle;

		theStyle.readCssStyle( elementStyle, true );
		backgroundImage = theStyle.getBackgroundImage();
		if( !backgroundImage.isEmpty() )
			specification = 0x01000000;
	}

	size_t	numElements = cssRules.size();
	size_t	i = numElements - 1;
	while( i < numElements )
	{
		css::Rule &theRule = cssRules[i--];

		if( !theRule.media.isEmpty() && theRule.media != media )
/*^*/		continue;

		for( size_t j=0; j<theRule.selectorList.size(); j++ )
		{
			matchFound = true;

			css::Selector	&theSelector = theRule.selectorList[j];

			if( !theSelector.match( element ) )
/*^*/			continue;

			if( theSelector.hasPredessor() )
			{
				parent = element;
				for( size_t k=theSelector.size()-2; k<theSelector.size(); k-- )
				{
					matchFound = false;
					const css::SelectorPart &theSpec = theSelector.getElement(k);

					if( theSpec.isSibling() )
					{
						parent = parent->getPrevious();
						if( theSpec.match( parent ) )
						{
							matchFound = true;
						}
					}
					else
					{
						for(
							parent = parent->getParent();
							parent;
							parent = parent->getParent()
						)
						{

							if( theSpec.match( parent ) )
							{
								matchFound = true;
/*v*/							break;
							}

							if( theSpec.isParent() )	// only one parent ?
/*v*/							break;
						}
					}
					if( !parent || !matchFound )
/*@*/					break;
				}
			}

			// this rule matches
			if( matchFound && !theRule.styles.getBackgroundImage().isEmpty() )
			{
				newSpecification = theSelector.getSpecification();
				if( backgroundImage.isEmpty()
				||  (newSpecification > specification && backgroundImage.isImportant() == theRule.styles.getBackgroundImage().isImportant())
				||  backgroundImage.isImportant() < theRule.styles.getBackgroundImage().isImportant() )
				{
					backgroundImage = theRule.styles.getBackgroundImage();
					specification = newSpecification;
				}
			}
		}
	}

	if( !backgroundImage.isEmpty() )
	{
		if( !strcmpi( backgroundImage, "none" ) )
			backgroundImage = "";
		else if( !strncmpi( backgroundImage, "url", 3 ) )
		{
			size_t	parenthesisPos = backgroundImage.searchChar( '(' );
			backgroundImage += parenthesisPos+1;
			parenthesisPos = backgroundImage.searchChar( ')' );
			if( parenthesisPos != backgroundImage.no_index )
				backgroundImage.cut( parenthesisPos );
			else
				backgroundImage = "";
		}
	}
	return backgroundImage;
}

void Document::applyCssRules( Element *theRoot, const CI_STRING &media )
{
	doEnterFunction("Document::applyCssRules( Element *theRoot, const CI_STRING &media )");
	Element	*theElement;

	for( size_t i=0; css::Styles::theCssFieldInfo[i].cssName; i++ )
	{
		getCssValue(
			theRoot, media,
			css::Styles::theCssFieldInfo[i].offset,
			css::Styles::theCssFieldInfo[i].inherited,
			css::Styles::theCssFieldInfo[i].defValue
		);
	}

	for( size_t i=0; i<theRoot->getNumObjects(); i++ )
	{
		theElement = theRoot->getElement( i );
		applyCssRules( theElement, media );
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

void Document::readCssRules( bool toLowerCase )
{
	doEnterFunction("Document::readCssRules( bool toLowerCase )");

	STRING		type;
	F_STRING	cssFile = getStyleSheet( &type );

	if( !cssFile.isEmpty() && type == "text/css" )
	{
		if( !m_fileName.isEmpty() )
			cssFile = makeFullPath( m_fileName, cssFile );

		std::ifstream	istream( cssFile );

		readCssRules( &istream, toLowerCase );
	}
}

void Document::readCssRules( const STRING &styles, bool toLowerCase )
{
	doEnterFunction("Document::readCssRules( const STRING &styles, bool toLowerCase )");

	iSTRINGstream	theInputStream( styles );

	readCssRules( &theInputStream, toLowerCase );
}

STRING Document::getBackgroundImage(
	Element *element, const CI_STRING &media
)
{
	css::Value	backgroundImage = element->getCssStyle()->getBackgroundImage();

	if( backgroundImage.isEmpty() )
		backgroundImage = findBackgroundImage( element, media );

	if( !strcmpi( backgroundImage, "none" ) )
	{
		element->getCssStyle()->getBackgroundImage() = backgroundImage;
		backgroundImage = "";
	}
	else if( backgroundImage.isEmpty() )
	{
		element->getCssStyle()->getBackgroundImage() = "none";
	}
	else
	{
		element->getCssStyle()->getBackgroundImage() = backgroundImage;
	}

	return backgroundImage;
}

css::Styles *Element::getCssStyle( void )
{
	doEnterFunction("Element::getCssStyle");

	if( !theCssStyle )
	{
		theCssStyle = new css::Styles;

		STRING	elementStyle = getStyle();

		if( !elementStyle.isEmpty() )
		{
			theCssStyle->readCssStyle( elementStyle, false );
		}
	}
	return theCssStyle;
}


void Element::clearCss( void )
{
	if( theCssStyle )
	{
		delete theCssStyle;
		theCssStyle = NULL;
	}
	for( size_t i=0; i<getNumObjects(); i++ )
	{
		getElement( i )->clearCss();
	}

}

int Element::getTextDecorations( void )
{
	int decoration = getCssStyle()->getTextDecorations();

	if( !(decoration & css::DECO_FLAG_NONE) )
	{
		Element	*parent = getParent();
		if( parent )
		{
			while( 1 )
			{
				decoration |= parent->getCssStyle()->getTextDecorations();
				if( decoration & css::DECO_FLAG_NONE )
					break;

				parent = parent->getParent();
				if( !parent )
					break;
			}
		}
	}


	return decoration;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace xml
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -x.
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

