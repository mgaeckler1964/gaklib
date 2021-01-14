/*
		Project:		GAKLIB
		Module:			htmlParser.cpp
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

#include <gak/html.h>
#include <gak/htmlParser.h>

// --------------------------------------------------------------------- //
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
namespace html
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

void Parser::getNextAttribute( STRING *attribute, STRING *value )
{
	char	stringDelimiter;
	char	c;

	*value = NULL_STRING;
	*attribute = readIdentifier();
	if( !attribute->isEmpty() )
	{
		c = getNextNonBlank();

		if( c == '=' )		// not all attributes have a value
		{
			c = getNextNonBlank();
			if( c == '\'' || c == '\"' )
			{
				stringDelimiter = c;
				*value = readString(stringDelimiter);
			}
			else
			{
				putback( c );
				*value = readIdentifier();
			}
		}
		else
		{
			putback( c );
			*value="1";
		}
	}
}

char Parser::parseEntity( const STRING &entity )
{
	return xml::PCData::html2ASCII( entity );
}

xml::Element *Factory::createObject( const STRING &theTag )
{
	if( !strcmpi( theTag, "a" ) )
		return new ANCHOR;
	if( !strcmpi( theTag, "abbr" ) )
		return new ABBREVIATION;
	if( !strcmpi( theTag, "acronym" ) )
		return new ACRONYM;
	if( !strcmpi( theTag, "address" ) )
		return new ADDRESS;
	if( !strcmpi( theTag, "applet" ) )
		return new APPLET;
	if( !strcmpi( theTag, "area" ) )
		return new IMAGE_AREA;
	if( !strcmpi( theTag, "b" ) )
		return new BOLD;
	if( !strcmpi( theTag, "base" ) )
		return new BASE_TARGET;
	if( !strcmpi( theTag, "basefont" ) )
		return new BASE_FONT;
	if( !strcmpi( theTag, "bdo" ) )
		return new DIRECTION;
	if( !strcmpi( theTag, "big" ) )
		return new BIG;
	if( !strcmpi( theTag, "blink" ) )
		return new BLINK;
	if( !strcmpi( theTag, "blockquote" ) )
		return new BLOCKQUOTE;
	if( !strcmpi( theTag, "body" ) )
		return new BODY;
	if( !strcmpi( theTag, "br" ) )
		return new LINE_BREAK;
	if( !strcmpi( theTag, "button" ) )
		return new BUTTON;
	if( !strcmpi( theTag, "caption" ) )
		return new TABLE_CAPTION;
	if( !strcmpi( theTag, "center" ) )
		return new CENTER_BLOCK;
	if( !strcmpi( theTag, "cite" ) )
		return new CITE;
	if( !strcmpi( theTag, "code" ) )
		return new SOURCE_CODE;
	if( !strcmpi( theTag, "col" ) )
		return new TABLE_COL;
	if( !strcmpi( theTag, "colgroup" ) )
		return new TABLE_COLGROUP;
	if( !strcmpi( theTag, "dd" ) )
		return new DEFINITION_LIST_DEF;
	if( !strcmpi( theTag, "dfn" ) )
		return new DEFINITION;
	if( !strcmpi( theTag, "dir" ) )
		return new DIRECTORY_LIST;
	if( !strcmpi( theTag, "div" ) )
		return new DIV_BLOCK;
	if( !strcmpi( theTag, "dl" ) )
		return new DEFINITION_LIST;
	if( !strcmpi( theTag, "dt" ) )
		return new DEFINITION_LIST_TERM;
	if( !strcmpi( theTag, "em" ) )
		return new EMPHASIS;
	if( !strcmpi( theTag, "fieldset" ) )
		return new FIELDSET;
	if( !strcmpi( theTag, "font" ) )
		return new FONT;
	if( !strcmpi( theTag, "form" ) )
		return new FORM;
	if( !strcmpi( theTag, "frame" ) )
		return new FRAME;
	if( !strcmpi( theTag, "frameset" ) )
		return new FRAMESET;
	if( !strcmpi( theTag, "h1" ) )
		return new HEADER( 1 );
	if( !strcmpi( theTag, "h2" ) )
		return new HEADER( 2 );
	if( !strcmpi( theTag, "h3" ) )
		return new HEADER( 3 );
	if( !strcmpi( theTag, "h4" ) )
		return new HEADER( 4 );
	if( !strcmpi( theTag, "h5" ) )
		return new HEADER( 5 );
	if( !strcmpi( theTag, "h6" ) )
		return new HEADER( 6 );
	if( !strcmpi( theTag, "head" ) )
		return new HEAD;
	if( !strcmpi( theTag, "hr" ) )
		return new HORIZONTAL_LINE;
	if( !strcmpi( theTag, "html" ) )
		return new HTML;
	if( !strcmpi( theTag, "i" ) )
		return new ITALIC;
	if( !strcmpi( theTag, "iframe" ) )
		return new IFRAME;
	if( !strcmpi( theTag, "img" ) )
		return new IMAGE;
	if( !strcmpi( theTag, "input" ) )
		return new INPUT_FIELD;
	if( !strcmpi( theTag, "isindex" ) )
		return new INDEX;
	if( !strcmpi( theTag, "kbd" ) )
		return new KEYBOARD;
	if( !strcmpi( theTag, "label" ) )
		return new LABEL;
	if( !strcmpi( theTag, "legend" ) )
		return new LEGEND;
	if( !strcmpi( theTag, "li" ) )
		return new LIST_ENTRY;
	if( !strcmpi( theTag, "link" ) )
		return new EXTERNAL_CSS;
	if( !strcmpi( theTag, "map" ) )
		return new IMAGE_MAP;
	if( !strcmpi( theTag, "menu" ) )
		return new MENU_LIST;
	if( !strcmpi( theTag, "meta" ) )
		return new META;
	if( !strcmpi( theTag, "noframes" ) )
		return new NOFRAMES;
	if( !strcmpi( theTag, "noscript" ) )
		return new NOSCRIPT;
	if( !strcmpi( theTag, "object" ) )
		return new EMBEDED_OBJECT;
	if( !strcmpi( theTag, "ol" ) )
		return new NUMERIC_LIST;
	if( !strcmpi( theTag, "option" ) )
		return new SELECT_OPTION;
	if( !strcmpi( theTag, "p" ) )
		return new PARAGRAPH;
	if( !strcmpi( theTag, "param" ) )
		return new APPLET_PARAM;
	if( !strcmpi( theTag, "pre" ) )
		return new PREFORMATED_TEXT;
	if( !strcmpi( theTag, "q" ) )
		return new QUOTE;
	if( !strcmpi( theTag, "s" ) || !strcmpi( theTag, "strike" ))
		return new STRIKE;
	if( !strcmpi( theTag, "samp" ) )
		return new EXAMPLE;
	if( !strcmpi( theTag, "script" ) )
		return new JAVA_SCRIPT;
	if( !strcmpi( theTag, "select" ) )
		return new SELECT;
	if( !strcmpi( theTag, "small" ) )
		return new SMALL;
	if( !strcmpi( theTag, "span" ) )
		return new HTML_TEXT;
	if( !strcmpi( theTag, "strong" ) )
		return new STRONG;
	if( !strcmpi( theTag, "style" ) )
		return new CSS_STYLE;
	if( !strcmpi( theTag, "sub" ) )
		return new SUBSCRIPT;
	if( !strcmpi( theTag, "sup" ) )
		return new SUPERSCRIPT;
	if( !strcmpi( theTag, "table" ) )
		return new TABLE;
	if( !strcmpi( theTag, "tbody" ) )
		return new TABLE_BODY;
	if( !strcmpi( theTag, "td" ) )
		return new TABLE_FIELD;
	if( !strcmpi( theTag, "textarea" ) )
		return new TEXTAREA;
	if( !strcmpi( theTag, "tfoot" ) )
		return new TABLE_FOOT;
	if( !strcmpi( theTag, "th" ) )
		return new TABLE_HEADER_FIELD;
	if( !strcmpi( theTag, "thead" ) )
		return new TABLE_HEAD;
	if( !strcmpi( theTag, "title" ) )
		return new TITLE;
	if( !strcmpi( theTag, "tr" ) )
		return new TABLE_RECORD;
	if( !strcmpi( theTag, "tt" ) )
		return new TYPE_WRITER;
	if( !strcmpi( theTag, "u" ) )
		return new UNDERLINE;
	if( !strcmpi( theTag, "ul" ) )
		return new BULLET_LIST;
	if( !strcmpi( theTag, "var" ) )
		return new VARIABLE;

	return xml::Factory::createObject( theTag );
}

xml::Element *Parser::createObject( const STRING &theTag )
{
	if( !getFactory() )
		setFactory( new Factory );

	return getFactory()->createObject( theTag );
}
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

Document *Parser::readFile( bool includeBlanks )
{
	Document		*theHTMLData = new Document(getPosition().m_fileName);
	xml::Element	*unknown = readObject( theHTMLData, includeBlanks, 0 );
	if( unknown )
	{
		delete unknown;
	}
	return theHTMLData;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace html
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

