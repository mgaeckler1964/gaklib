/*
		Project:		GAKLIB
		Module:			css.cpp
		Description:	cascading stylesheets
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

#include <gak/css.h>
#include <gak/xml.h>
#include <gak/ci_string.h>
#include <gak/t_string.h>
#include <gak/stringStream.h>
#include <gak/numericString.h>
#include <gak/logfile.h>

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
namespace css
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

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

fieldInfo Styles::theCssFieldInfo[] =
{
	{ "background", offsetof( Styles, background ), false, true, NULL },
	{ "background-color", offsetof( Styles, background_color ), true, false, "transparent" },
	{ "background-image", offsetof( Styles, background_image ), false, false, "none" },
	{ "border", offsetof( Styles, border ), false, true, NULL },
	{ "border-color", offsetof( Styles, border_color ), false, true, NULL },
	{ "border-bottom", offsetof( Styles, border_bottom ), false, true, NULL },
	{ "border-bottom-color", offsetof( Styles, border_bottom_color ), false, false, NULL },
	{ "border-bottom-style", offsetof( Styles, border_bottom_style ), false, false, "none" },
	{ "border-bottom-width", offsetof( Styles, border_bottom_width ), false, false, NULL },
	{ "border-left", offsetof( Styles, border_left ), false, true, NULL },
	{ "border-left-color", offsetof( Styles, border_left_color ), false, false, NULL },
	{ "border-left-style", offsetof( Styles, border_left_style ), false, false, "none" },
	{ "border-left-width", offsetof( Styles, border_left_width ), false, false, NULL },
	{ "border-top", offsetof( Styles, border_top ), false, true, NULL },
	{ "border-top-color", offsetof( Styles, border_top_color ), false, false, NULL },
	{ "border-top-style", offsetof( Styles, border_top_style ), false, false, "none" },
	{ "border-top-width", offsetof( Styles, border_top_width ), false, false, NULL },
	{ "border-right", offsetof( Styles, border_right ), false, true, NULL },
	{ "border-right-color", offsetof( Styles, border_right_color ), false, false, NULL },
	{ "border-right-style", offsetof( Styles, border_right_style ), false, false, "none" },
	{ "border-right-width", offsetof( Styles, border_right_width ), false, false, NULL },
	{ "border-style", offsetof( Styles, border_style ), false, true, NULL },
	{ "border-width", offsetof( Styles, border_width ), false, true, NULL },
	{ "bottom", offsetof( Styles, bottom ), false, false, NULL },
	{ "color", offsetof( Styles, color ), true, false, NULL },
	{ "display", offsetof( Styles, display ), false, false, "inline" },
	{ "float", offsetof( Styles, cssFloat ), false, false, "none" },
	{ "font-family", offsetof( Styles, font_family ), true, false, NULL },
	{ "font-size", offsetof( Styles, font_size ), true, false, NULL },
	{ "font-style", offsetof( Styles, font_style ), true, false, NULL },
	{ "font-weight", offsetof( Styles, font_weight ), true, false, NULL },
	{ "height", offsetof( Styles, height ), false, false, NULL },
	{ "left", offsetof( Styles, left ), false, false, NULL },
	{ "letter-spacing", offsetof( Styles, letter_spacing ), true, false, NULL },
	{ "line-height", offsetof( Styles, line_height ), true, false, NULL },
	{ "list-style-type", offsetof( Styles, list_style_type ), true, false, NULL },
	{ "margin", offsetof( Styles, margin ), false, true, NULL },
	{ "margin-left", offsetof( Styles, margin_left ), false, false, NULL },
	{ "margin-right", offsetof( Styles, margin_right ), false, false, NULL },
	{ "margin-top", offsetof( Styles, margin_top ), false, false, NULL },
	{ "margin-bottom", offsetof( Styles, margin_bottom ), false, false, NULL },
	{ "padding", offsetof( Styles, padding ), false, true, NULL },
	{ "padding-bottom", offsetof( Styles, padding_bottom ), false, false, NULL },
	{ "padding-left", offsetof( Styles, padding_left ), false, false, NULL },
	{ "padding-right", offsetof( Styles, padding_right ), false, false, NULL },
	{ "padding-top", offsetof( Styles, padding_top ), false, false, NULL },
	{ "padding-bottom", offsetof( Styles, padding_bottom ), false, false, NULL },
	{ "position", offsetof( Styles, position ), false, false, "static" },
	{ "right", offsetof( Styles, right ), false, false, NULL },
	{ "text-align", offsetof( Styles, text_align ), true, false, "left" },
	{ "text-decoration", offsetof( Styles, text_decoration ), false, false, NULL },
	{ "text-indent", offsetof( Styles, text_indent ), true, false, NULL },
	{ "top", offsetof( Styles, top ), false, false, NULL },
	{ "white-space", offsetof( Styles, white_space ), false, false, "normal" },
	{ "width", offsetof( Styles, width ), false, false, NULL },
	{ NULL, 0, false, false, NULL },
};

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static char getNextWithBlank( std::istream *theInput )
{
	int c = theInput->get();

	if( c == EOF )
		c = 0;

	return (char)c;
}

static char getNextNonBlank( std::istream *theInput )
{
	char c;

	while( isspace( c=getNextWithBlank( theInput ) ) && c )
		;

	return c;
}

static void skipComment( std::istream *theInput )
{
	char	c;
	STRING	endComment;

	while( endComment != "*/" )
	{
		c = getNextWithBlank( theInput );
		if( c == '*' && endComment.isEmpty() )
		{
			endComment += c;
		}
		else if( c == '/' && endComment == "*" )
		{
			endComment += c;
		}
		else if( !c )
/*v*/		break;
		else
			endComment = (const char *)NULL;
	}
}

static STRING getNextIdentifier( std::istream *theInput, bool toLowerCase )
{
	STRING	identifier;
	char	c;
	bool	operatorFlag;

	c = getNextNonBlank( theInput );
	operatorFlag = isalnum( c ) ? false : true;
	if( c )
	{
		identifier = c;
		if( !strchr( ",.#<>{}[]():+@'=", c ) )	// one character operator?
		{
			while( (c=getNextWithBlank( theInput )) != 0 && c!='\n' && c!='\r' )
			{
				if( operatorFlag )
				{
					identifier += c;
					if( identifier == "/*" || identifier == "*/" || identifier == "//" )
/*v*/					break;
				}
				else if( isalnum( c ) || c == '-' || c == '_' )
					identifier += c;
				else
				{
					theInput->putback( c );
/*v*/				break;
				}
			}
		}
	}

	if( identifier == "/*" )
	{
		// skip until end of comment then read the next identifier
		skipComment( theInput );
		identifier = getNextIdentifier( theInput, toLowerCase );
	}

	if( toLowerCase )
		identifier.lowerCase();

	return identifier;
}

static STRING getNextValue( std::istream *theInput, const char *until, bool toLowerCase )
{
	STRING	identifier, important;
	char  	c;

	while( (c=getNextWithBlank( theInput )) != 0 && !strchr( until, c ))
		identifier += c;

	identifier.stripBlanks();

	if( toLowerCase )
		identifier.lowerCase();

	return identifier;
}

static Value getNextValue( std::istream *theInput )
{
	Value	identifier, important;
	char	c;

	c = getNextNonBlank( theInput );
	if( c )
	{
		identifier = Value(c);
		if( isalnum( c ) || c == '#' )
		{
			while( (c=getNextWithBlank( theInput )) != 0 )
			{
				if( c!= '\r' && c!='\n' && c!= ';' && c!='}' && c != '!' )
					identifier += c;
				else
					break;
			}
		}
	}
	identifier.stripBlanks();

	if( c == '!' )
	{
		important = getNextValue( theInput );
		if( !strcmpi( important, "important" ) )
			identifier.setImportant();
	}

	return identifier;
}

static unsigned char hexToInt( char nibble )
{
	if( nibble >= '0' && nibble <= '9' )
		return (unsigned char)(nibble - '0');
	else
		return (unsigned char)(ansiToUpper( nibble ) - ('A' - 10));
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

Color Styles::defaultColors[] =
{
	{ 255, 255, 255 },		// White
	{ 192, 192, 192 },		// Silver
	{ 128, 128, 128 },		// gray
	{ 0, 0, 0 },			// Black

	{ 255, 0, 0 },			// Red
	{ 0, 255, 0 },			// Lime
	{ 0, 0, 255 },			// Blue

	{ 255, 255, 0 },		// yellow
	{ 0, 255, 255 },		// Aqua
	{ 255, 0, 255 },		// Fuchsia

	{ 128, 0, 0 },			// Marroon
	{ 0, 128, 0 },			// Green
	{ 0, 0, 128 },			// Navy

	{ 128, 128, 0 },		// Olive
	{ 0, 128, 128 },		// Teal
	{ 128, 0, 128 },		// Purple

	{ 255, 102, 0 },		// orange

};

unsigned char Styles::getColorComponentValue( const STRING &color )
{
	double	tmpDbl;
	int		tmpI;
	STRING	tmpStr = color;
	tmpStr.stripBlanks();

	if( tmpStr.endsWith( '%' ) )
	{
		tmpDbl = atof( tmpStr );
		tmpI = int(2.55 * tmpDbl +0.5);
	}
	else
	{
		tmpI = getValue<int>( tmpStr );
	}

	if( tmpI < 0 )
	{
		tmpI = 0;
	}
	else if( tmpI > 255 )
	{
		tmpI = 255;
	}

	return (unsigned char )tmpI;
}

bool Styles::parseColor( const Value &color, Color *result )
{
	if( color[0U] == '#' )
	{
		if( color.strlen() == 7 )
		{
			result->red = (unsigned char)((hexToInt(color[1U]) << 4) | hexToInt(color[2U]));
			result->green = (unsigned char)((hexToInt(color[3U]) << 4) | hexToInt(color[4U]));
			result->blue = (unsigned char)((hexToInt(color[5U]) << 4) | hexToInt(color[6U]));

			return true;
		}
		if( color.strlen() == 4 )
		{
			result->red = (unsigned char)((hexToInt(color[1U]) << 4) | hexToInt(color[1U]));
			result->green = (unsigned char)((hexToInt(color[2U]) << 4) | hexToInt(color[2U]));
			result->blue = (unsigned char)((hexToInt(color[3U]) << 4) | hexToInt(color[3U]));

			return true;
		}
	}
	else if( color.beginsWith( "rgb" ) )
	{
		STRING tmpColor = color + (size_t)3;
		tmpColor.stripBlanks();
		if( tmpColor.beginsWith( '(' ) && tmpColor.endsWith( ')' ) )
		{
			ArrayOfStrings	colorArray;
			colorArray.createElements( tmpColor, ",", true );
			if( colorArray.size() == 3 )
			{
				result->red = getColorComponentValue( colorArray[0] );
				result->green = getColorComponentValue( colorArray[1] );
				result->blue = getColorComponentValue( colorArray[2] );

				return true;
			}
		}
	}
	else
	{
		int colorIndex = tokenSearch(
			color,
			"white\0silver\0gray\0black\0"
			"red\0lime\0blue\0"
			"yellow\0aqua\0fuchsia\0"
			"maroon\0green\0navy\0"
			"olive\0teal\0purple\0"
			"orange\0\0"
		);
		if( colorIndex >= 0 )
		{
			*result = defaultColors[colorIndex];
			return true;
		}
	}

	return false;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void Rules::readRule( 
	std::istream *theInput, 
	STRING firstIdentifier, 
	STRING media, 
	bool toLowerCase 
)
{
	enum
	{
		EXP_HTML_TAG, EXP_HTML_CLASS, EXP_HTML_ID, EXP_DELIMITER
	} expectation = EXP_HTML_TAG;		// what do we expect

	Rule			theRule;			// the new rule
	Selector		theSelector;
	SelectorPart	theElement;

	STRING			name, value, identifier = firstIdentifier;

	theRule.media = media;

	// read all objects
	while( !identifier.isEmpty() )
	{
		if( identifier == ">" )
		{
			theElement.setParent();
			theSelector.addElement( theElement );
			theElement.clear();

			expectation = EXP_HTML_TAG;
		}
		else if( identifier == "+" )
		{
			theElement.setSibling();
			theSelector.addElement( theElement );
			theElement.clear();

			expectation = EXP_HTML_TAG;
		}
		else if( identifier == "." )
			expectation = EXP_HTML_CLASS;
		else if( identifier == "#" )
			expectation = EXP_HTML_ID;
		else if( identifier == "," || identifier == "{" )
		{
			theSelector.addElement( theElement );
			theRule.selectorList += theSelector;
			if( identifier == "{" )
/*v*/			break;

			expectation = EXP_HTML_TAG;
			theSelector.clear();
			theElement.clear();
		}
		else if( identifier == "[" )
		{
			STRING	attribute = getNextValue( theInput, "]", toLowerCase );
			theElement.addAttribute( attribute );
		}
		else if( expectation == EXP_HTML_TAG )
		{
			theElement.setTag( identifier );
			expectation = EXP_DELIMITER;
		}
		else if( expectation == EXP_HTML_CLASS )
		{
			theElement.addCssClass( identifier );
			expectation = EXP_DELIMITER;
		}
		else if( expectation == EXP_HTML_ID )
		{
			theElement.setId( identifier );
			expectation = EXP_DELIMITER;
		}
		else
		{
			theSelector.addElement( theElement );
			theElement.clear();
			theElement.setTag( identifier );
			expectation = EXP_DELIMITER;
		}

		identifier = getNextIdentifier( theInput, toLowerCase );
	}

	// read all styles
	theRule.styles.readCssStyle( theInput, toLowerCase );

	addElement( theRule );
}

void Rules::readMedia( std::istream *theInput, bool toLowerCase )
{
	STRING	firstIdentifier, identifier = getNextIdentifier( theInput, toLowerCase );
	if( !strcmpi( identifier, "media" ) )
	{
		STRING	media = getNextIdentifier( theInput, toLowerCase );
		STRING	brace = getNextIdentifier( theInput, toLowerCase );
		if( brace == "{" )
		{
			while( 1 )
			{
				firstIdentifier = getNextIdentifier( theInput, toLowerCase );
				if( firstIdentifier == "}" || firstIdentifier.isEmpty() )
					break;
				readRule( theInput, firstIdentifier, media, toLowerCase );
			}
		}
	}
}

static const int CSS_ARRAY_CHECK = 
	ArrayOfStrings::CHECK_STRINGS|
	ArrayOfStrings::CHECK_CHARS|
	ArrayOfStrings::CHECK_BRACKETS|
	ArrayOfStrings::CHECK_PARENTESIS
;

void Styles::handleShortForm( size_t offset )
{
	ArrayOfStrings	parts;
	if( offset == offsetof( Styles, background ) )
	{
		parts.createElements( background, CSS_ARRAY_CHECK );
		background_color = parts[0];
		background_image = parts[1];
		background_color.setByShortForm();
		background_image.setByShortForm();
	}
	else if( offset == offsetof( Styles, border ) )
	{
		parts.createElements( border, CSS_ARRAY_CHECK );
		border_width = parts[0];
		border_style = parts[1];
		border_color = parts[2];
		border_width.setByShortForm();
		border_style.setByShortForm();
		border_color.setByShortForm();
		handleShortForm( offsetof( Styles, border_width ) );
		handleShortForm( offsetof( Styles, border_style ) );
		handleShortForm( offsetof( Styles, border_color ) );
	}
	else if( offset == offsetof( Styles, border_bottom ) )
	{
		parts.createElements( border_bottom, CSS_ARRAY_CHECK );
		border_bottom_width = parts[0];
		border_bottom_style = parts[1];
		border_bottom_color = parts[2];
		border_bottom_width.setByShortForm();
		border_bottom_style.setByShortForm();
		border_bottom_color.setByShortForm();
	}
	else if( offset == offsetof( Styles, border_color ) )
	{
		parts.createElements( border_color, CSS_ARRAY_CHECK );
		if( parts.size() == 1 )
		{
			border_top_color = parts[0];
			border_right_color = parts[0];
			border_bottom_color = parts[0];
			border_left_color = parts[0];
		}
		else if( parts.size() == 2 )
		{
			border_top_color = parts[0];
			border_right_color = parts[1];
			border_bottom_color = parts[0];
			border_left_color = parts[1];
		}
		else if( parts.size() == 3 )
		{
			border_top_color = parts[0];
			border_right_color = parts[1];
			border_bottom_color = parts[2];
			border_left_color = parts[1];
		}
		else
		{
			border_top_color = parts[0];
			border_right_color = parts[1];
			border_bottom_color = parts[2];
			border_left_color = parts[3];
		}
		border_top_color.setByShortForm();
		border_right_color.setByShortForm();
		border_bottom_color.setByShortForm();
		border_left_color.setByShortForm();
	}
	else if( offset == offsetof( Styles, border_left ) )
	{
		parts.createElements( border_left, CSS_ARRAY_CHECK );
		border_left_width = parts[0];
		border_left_style = parts[1];
		border_left_color = parts[2];
		border_left_width.setByShortForm();
		border_left_style.setByShortForm();
		border_left_color.setByShortForm();
	}
	else if( offset == offsetof( Styles, border_right ) )
	{
		parts.createElements( border_right, CSS_ARRAY_CHECK );
		border_right_width = parts[0];
		border_right_style = parts[1];
		border_right_color = parts[2];
		border_right_width.setByShortForm();
		border_right_style.setByShortForm();
		border_right_color.setByShortForm();
	}
	else if( offset == offsetof( Styles, border_style ) )
	{
		parts.createElements( border_style, CSS_ARRAY_CHECK );
		if( parts.size() == 1 )
		{
			border_top_style = parts[0];
			border_right_style = parts[0];
			border_bottom_style = parts[0];
			border_left_style = parts[0];
		}
		else if( parts.size() == 2 )
		{
			border_top_style = parts[0];
			border_right_style = parts[1];
			border_bottom_style = parts[0];
			border_left_style = parts[1];
		}
		else if( parts.size() == 3 )
		{
			border_top_style = parts[0];
			border_right_style = parts[1];
			border_bottom_style = parts[2];
			border_left_style = parts[1];
		}
		else
		{
			border_top_style = parts[0];
			border_right_style = parts[1];
			border_bottom_style = parts[2];
			border_left_style = parts[3];
		}
		border_top_style.setByShortForm();
		border_right_style.setByShortForm();
		border_bottom_style.setByShortForm();
		border_left_style.setByShortForm();
	}
	else if( offset == offsetof( Styles, border_top ) )
	{
		parts.createElements( border_top, CSS_ARRAY_CHECK );
		border_top_width = parts[0];
		border_top_style = parts[1];
		border_top_color = parts[2];
		border_top_width.setByShortForm();
		border_top_style.setByShortForm();
		border_top_color.setByShortForm();
	}
	else if( offset == offsetof( Styles, border_width ) )
	{
		parts.createElements( border_width, CSS_ARRAY_CHECK );
		if( parts.size() == 1 )
		{
			border_top_width = parts[0];
			border_right_width = parts[0];
			border_bottom_width = parts[0];
			border_left_width = parts[0];
		}
		else if( parts.size() == 2 )
		{
			border_top_width = parts[0];
			border_right_width = parts[1];
			border_bottom_width = parts[0];
			border_left_width = parts[1];
		}
		else if( parts.size() == 3 )
		{
			border_top_width = parts[0];
			border_right_width = parts[1];
			border_bottom_width = parts[2];
			border_left_width = parts[1];
		}
		else
		{
			border_top_width = parts[0];
			border_right_width = parts[1];
			border_bottom_width = parts[2];
			border_left_width = parts[3];
		}
		border_top_width.setByShortForm();
		border_right_width.setByShortForm();
		border_bottom_width.setByShortForm();
		border_left_width.setByShortForm();
	}
	else if( offset == offsetof( Styles, margin ) )
	{
		parts.createElements( margin, CSS_ARRAY_CHECK );
		if( parts.size() == 1 )
		{
			margin_top = parts[0];
			margin_right = parts[0];
			margin_bottom = parts[0];
			margin_left = parts[0];
		}
		else if( parts.size() == 2 )
		{
			margin_top = parts[0];
			margin_right = parts[1];
			margin_bottom = parts[0];
			margin_left = parts[1];
		}
		else if( parts.size() == 3 )
		{
			margin_top = parts[0];
			margin_right = parts[1];
			margin_bottom = parts[2];
			margin_left = parts[1];
		}
		else
		{
			margin_top = parts[0];
			margin_right = parts[1];
			margin_bottom = parts[2];
			margin_left = parts[3];
		}
		margin_top.setByShortForm();
		margin_right.setByShortForm();
		margin_bottom.setByShortForm();
		margin_left.setByShortForm();
	}
	else if( offset == offsetof( Styles, padding ) )
	{
		parts.createElements( padding, CSS_ARRAY_CHECK );
		if( parts.size() == 1 )
		{
			padding_top = parts[0];
			padding_right = parts[0];
			padding_bottom = parts[0];
			padding_left = parts[0];
		}
		else if( parts.size() == 2 )
		{
			padding_top = parts[0];
			padding_right = parts[1];
			padding_bottom = parts[0];
			padding_left = parts[1];
		}
		else if( parts.size() == 3 )
		{
			padding_top = parts[0];
			padding_right = parts[1];
			padding_bottom = parts[2];
			padding_left = parts[1];
		}
		else
		{
			padding_top = parts[0];
			padding_right = parts[1];
			padding_bottom = parts[2];
			padding_left = parts[3];
		}
		padding_top.setByShortForm();
		padding_right.setByShortForm();
		padding_bottom.setByShortForm();
		padding_left.setByShortForm();
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

bool SelectorPart::match( xml::Element *theElement ) const
{
	doEnterFunction("HTML_SPEC::match");

	bool			caseSensitive = theElement->isCaseSensitive();
	bool			matchValue = true;
	STRING 			elementTag = theElement->getTag();
	STRING			elementId = theElement->getId();
	STRING			elementClass = theElement->getClass();
	ArrayOfStrings	elementClasses;

	if( !tag.isEmpty() && tag != "*" )
	{
		if( caseSensitive )
		{
			if( tag != elementTag )
			{
				matchValue = false;
			}
		}
		else
		{
			if( strcmpi( tag, elementTag ) )
			{
				matchValue = false;
			}
		}
	}

	if( matchValue && !id.isEmpty() )
	{
		if( caseSensitive )
		{
			if( id != elementId )
				matchValue = false;
		}
		else
		{
			if( strcmpi( id, elementId ) )
				matchValue = false;
		}
	}

	if( matchValue && cssClasses.size() > 0 )
	{
		matchValue = false;
		if( !elementClass.isEmpty() )
		{
			elementClasses.createElements( elementClass, " " );
			for( size_t i=0; i<cssClasses.size() && !matchValue; i++ )
			{
				const STRING &cssClass = cssClasses[i];
				if( elementClasses.findElement( cssClass ) != elementClasses.no_index  )
					matchValue = true;
			}
		}
	}

	if( matchValue && attributes.size() > 0 )
	{
		matchValue = false;
		for( size_t i=0; i<attributes.size() && !matchValue; i++ )
		{
			const STRING	&attribute = attributes[i];
			size_t			assignmentPos = attribute.searchChar( '=' );
			if( assignmentPos != attribute.no_index && assignmentPos != 0 )
			{
				STRING attributeName = attribute.leftString(assignmentPos);
				STRING attributeCssValue = attribute.subString(assignmentPos+1);
				if( attributeName.endsWith( "~" ) )
				{
					attributeName.cut( attributeName.strlen() -1 );
					STRING attributeValue = theElement->getAttribute( attributeName );
					ArrayOfStrings	attributeValues;
					attributeValues.createElements( attributeValue, " " );
					if( attributeValues.findElement( attributeCssValue ) != attributeValues.no_index  )
						matchValue = true;

				}
				else if( attributeName.endsWith( "|" ) )
				{
					attributeName.cut( attributeName.strlen() -1 );
					STRING attributeValue = theElement->getAttribute( attributeName );
					 if( attributeValue.beginsWith( attributeCssValue ) )
						matchValue = true;
				}
				else
				{
					STRING attributeValue = theElement->getAttribute( attributeName );
					 if( attributeValue == attributeCssValue )
						matchValue = true;
				}
			}
			else
			{
				STRING attributeValue = theElement->getAttribute( attribute );
				if( !attributeValue.isEmpty() )
					matchValue = true;
			}
		}

	}
	return matchValue;
}

unsigned long Selector::getSpecification( void )
{
	if( !specification )
		for( size_t i=0; i<path.size(); i++ )
			specification += path[i].getSpecification();

	return specification;
}

void Styles::readCssStyle( std::istream *theInput, bool toLowerCase )
{
	CI_STRING	identifier, name;
	Value		value;

	while( 1 )
	{
		identifier = getNextIdentifier( theInput, toLowerCase );
		if( identifier.isEmpty() || identifier == "}" )
			break;
		name = identifier;
		identifier = getNextIdentifier( theInput, toLowerCase );
		if( identifier == ":" )
		{
			value = getNextValue( theInput );

			for( size_t i=0; theCssFieldInfo[i].cssName; i++  )
			{
				if( name == theCssFieldInfo[i].cssName )
				{
					*cssValue( theCssFieldInfo[i].offset ) = value;
					if(  theCssFieldInfo[i].isShortForm )
						handleShortForm( theCssFieldInfo[i].offset );
					break;
				}
			}
		}
	}
}

void Styles::readCssStyle( const STRING &styles, bool toLowerCase )
{
	iSTRINGstream	theInputStream( styles );

	readCssStyle( &theInputStream, toLowerCase );
}

void Rules::readCssFile( std::istream *theInput, bool toLowerCase )
{
	doEnterFunction("Rules::readCssFile( istream *theInput, bool toLowerCase )");
	STRING	firstIdentifier;

	while( 1 )
	{
		firstIdentifier = getNextIdentifier( theInput, toLowerCase );
		if( firstIdentifier.isEmpty() )
/*v*/		break;

		if( firstIdentifier == "@" )
			readMedia( theInput, toLowerCase );
		else
			readRule( theInput, firstIdentifier, "", toLowerCase );
	}
}

int Styles::getTextDecorations( void ) const
{
	int				decorationFlags = 0;
	Decoration		decorationValue;
	T_STRING		decorationList = text_decoration;
	STRING			decoration = decorationList.getFirstToken( " " );
	while( 1 )
	{
		if( !decoration.isEmpty() )
		{
			decorationValue = getDecoration( decoration );
			if( decorationValue == DECO_NONE )
				decorationFlags = DECO_FLAG_NONE;
			else if( decorationValue == DECO_UNDERLINE )
				decorationFlags |= DECO_FLAG_UNDERLINE;
			else if( decorationValue == DECO_STROKE )
				decorationFlags |= DECO_FLAG_STROKE;
		}

		if( !decorationList.hasNextToken() )
			break;

		decoration = decorationList.getNextToken();
	}

	return decorationFlags;
}
// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace css
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

