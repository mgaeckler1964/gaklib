/*
		Project:		GAKLIB
		Module:			HTML.CPP
		Description:	HTML-Generator
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

// #include <strstrea.h>

#include <gak/html.h>

// -------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

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
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

extern const char HTML_STRICT_PUBLIC[]			= "-//W3C//DTD HTML 4.01//EN";
extern const char HTML_STRICT_SYSTEM[]			= "http://www.w3.org/TR/html4/strict.dtd";

extern const char HTML_TRANSITIONAL_PUBLIC[]	= "-//W3C//DTD HTML 4.01 Transitional//EN";
extern const char HTML_TRANSITIONAT_SYSTEM[]	= "http://www.w3.org/TR/html4/loose.dtd";

extern const char HTML_FRAMESET_PUBLIC[]		= "-//W3C//DTD HTML 4.01 Frameset//EN";
extern const char HTML_FRAMESET_SYSTEM[]		= "http://www.w3.org/TR/html4/frameset.dtd";

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

void WithPCData::addText( const char *htmlCode, bool includeLineFeed )
{
	if( includeLineFeed )
	{
		char	c;
		STRING	cData;

		while( (c = *htmlCode++) != 0 )
		{
			if( c=='\r' )	// ignore
				;
			else if( c == '\n' )
			{
				addObject( new xml::PCData(cData) );
				addObject( new LINE_BREAK );
				cData = "";
			}
			else
				cData += c;
		}
		if( cData[0U] )
			addObject( new xml::PCData(cData) );
	}
	else
		addObject( new xml::PCData(htmlCode) );
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

bool HtmlBase::isCaseSensitive( void ) const
{
	return false;
}

/*
--------------------------------------------------------------------------
	isInline/isBlock
--------------------------------------------------------------------------
*/
bool BlockElements::isInline( void )
{
	return false;
}

bool BlockElements::isBlock( void )
{
	return true;
}

bool InlineElements::isInline( void )
{
	return true;
}

bool InlineElements::isBlock( void )
{
	return false;
}

bool NonVisual::isInline( void )
{
	return false;
}

bool NonVisual::isBlock( void )
{
	return false;
}


/*
--------------------------------------------------------------------------
	isSimpleTag
--------------------------------------------------------------------------
*/
bool SimpleTag::isSimpleTag( void ) const
{
	return true;
}

bool WithValue::isSimpleTag( void ) const
{
	return false;
}

/*
--------------------------------------------------------------------------
	wantScriptCode
--------------------------------------------------------------------------
*/
bool JAVA_SCRIPT::wantScriptCode( void ) const
{
	return true;
}

bool CSS_STYLE::wantScriptCode( void ) const
{
	return true;
}
/*
--------------------------------------------------------------------------
	getValidSubobjects
--------------------------------------------------------------------------
*/
xml::Validator *HtmlBase::getValidSubobjects( ArrayOfStrings *tags )
{
	STRING		tag;
	char		c;
	const char	*cp = getValidSubobjects();

	while( 1 )
	{
		c = *cp++;
		if( c )
			tag += c;
		else
		{
			*tags += tag;
			tag = "";
			if( !*cp )
/*v*/			break;
		}
	}

	return NULL;
}

const char *WithPCData::getValidSubobjects( void )
{
	return "\0\0";		// nothing allowed
}

const char *WithInline::getValidSubobjects( void )
{
	return	"A\0ABBR\0ACRONYM\0APPLET\0B\0BASEFONT\0BDO\0BIG\0BLINK\0BR\0BUTTON\0"
			"CITE\0CODE\0DEL\0DFN\0EM\0FONT\0I\0IMG\0INS\0INPUT\0IFRAME\0"
			"KBD\0LABEL\0MAP\0OBJECT\0Q\0SAMP\0SCRIPT\0SELECT\0SMALL\0SPAN\0"
			"STRONG\0SUB\0SUP\0TEXTAREA\0TT\0U\0VAR\0"

			"\0";
}

const char *WithBlock::getValidSubobjects( void )
{
	return	"A\0ABBR\0ACRONYM\0APPLET\0B\0BASEFONT\0BDO\0BIG\0BLINK\0BR\0BUTTON\0"
			"CITE\0CODE\0DEL\0DFN\0EM\0FONT\0I\0IMG\0INS\0INPUT\0IFRAME\0"
			"KBD\0LABEL\0MAP\0OBJECT\0Q\0SAMP\0SCRIPT\0SELECT\0SMALL\0SPAN\0"
			"STRONG\0SUB\0SUP\0TEXTAREA\0TT\0U\0VAR\0"

			"ADDRESS\0BLOCKQUOTE\0CENTER\0DEL\0DIR\0DIV\0DL\0FIELDSET\0FORM\0"
			"H1\0H2\0H3\0H4\0H5\0H6\0HR\0INS\0ISINDEX\0MENU\0"
			"NOFRAMES\0NOSCRIPT\0OL\0P\0PRE\0TABLE\0UL\0\0";
}


const char *HEAD::getValidSubobjects( void )
{
	return "BASE\0ISINDEX\0LINK\0META\0OBJECT\0SCRIPT\0STYLE\0TITLE\0\0";
}

const char *TABLE::getValidSubobjects( void )
{
	return "CAPTION\0COL\0COLGROUP\0TBODY\0TFOOT\0THEAD\0TR\0\0";
}

const char *SimpleTag::getValidSubobjects( void )
{
	return "\0\0";		// nothing allowed
}

const char *TABLE_COLGROUP::getValidSubobjects( void )
{
	return "COL\0\0";
}

const char *ListBase::getValidSubobjects( void )
{
	return "LI\0\0";
}

const char *DEFINITION_LIST::getValidSubobjects( void )
{
	return "DD\0DT\0\0";
}

const char *TABLE_PART::getValidSubobjects( void )
{
	return "TR\0\0";
}


const char *TABLE_RECORD::getValidSubobjects( void )
{
	return "TH\0TD\0\0";
}

const char *SELECT::getValidSubobjects( void )
{
	return "OPTGROUP\0OPTION\0\0";
}

const char *HTML::getValidSubobjects( void )
{
	return "HEAD\0BODY\0FRAMESET\0\0";
}

const char *FRAMESET::getValidSubobjects( void )
{
	return "FRAMESET\0FRAME\0NOFRAMES\0\0";
}

const char *BUTTON::getValidSubobjects( void )
{
	return
		"ABBR\0ACRONYM\0ADDRESS\0APPLET\0""B\0BASEFONT\0BDO\0BIG\0"
		"BLOCKQUOTE\0BR\0CENTER\0CITE\0CODE\0DFN\0DL\0DIR\0DIV\0EM\0FONT\0"
		"H1\0H2\0H3\0H4\0H5\0H6\0\0HR\0I\0IMG\0KBD\0MAP\0MENU\0NOFRAMES\0"
		"NOSCRIPT\0OBJECT\0OL\0P\0PRE\0Q\0SAMP\0SCRIPT\0SMALL\0SPAN\0"
		"STRONG\0SUB\0SUP\0TABLE\0TT\0UL\0VAR\0\0"
	;
}

const char *NOFRAMES::getValidSubobjects( void )
{
	return "BODY\0\0";
}

/*
--------------------------------------------------------------------------
	isValidSubobject
--------------------------------------------------------------------------
*/

bool HtmlBase::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	STRING	prefix = newObject->getPrefix();
	if( !prefix.isEmpty() )
/***/	return true;

	STRING	tag = newObject->getTag();
	tag.upperCase();

	if( tokenSearch( tag, getValidSubobjects() ) >= 0 )
		return true;
	else
		return false;
}

bool WithPCData::isValidSubobject( xml::Element *newObject )
{
	STRING	tag = newObject->getTag();

	if( tag.isEmpty() || HtmlBase::isValidSubobject( newObject ) )
		return true;
	else
		return false;
}


bool SimpleTag::isValidSubobject( xml::Element * )
{
	return false;
}

bool ADDRESS::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	CI_STRING	tag = newObject->getTag();

	if( tag == "P"
	|| WithInline::isValidSubobject( newObject ) )
		return true;
	else
		return false;
}


bool ANCHOR::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	bool	isEnabled = WithInline::isValidSubobject( newObject );

	if( isEnabled )
	{
		CI_STRING	tag = newObject->getTag();
		if( tag == "A" )
			isEnabled = false;
	}

	return isEnabled;
}

bool FORM::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	bool	isEnabled = WithBlock::isValidSubobject( newObject );

	if( isEnabled )
	{
		CI_STRING	tag = newObject->getTag();
		if( tag == "FORM" )
			isEnabled = false;
	}

	return isEnabled;
}

bool FIELDSET::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	bool	isEnabled = WithBlock::isValidSubobject( newObject );

	if( !isEnabled )
	{
		CI_STRING	tag = newObject->getTag();
		if( tag == "LEGEND" )
			isEnabled = true;
	}

	return isEnabled;
}

bool EMBEDED::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	CI_STRING	tag = newObject->getTag();

	if( tag == "PARAM"
	|| WithBlock::isValidSubobject( newObject ) )
		return true;
	else
		return false;
}

bool Document::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	if( dynamic_cast<xml::DocType*>( newObject ) )
/***/	return true;

	CI_STRING	tag = newObject->getTag();

	if( tag == "HTML" )
		return true;
	else
		return false;
}

bool IMAGE_MAP::isValidSubobject( xml::Element *newObject )
{
	if( dynamic_cast<xml::Comment*>( newObject ) )
/***/	return true;

	bool	isEnabled = WithBlock::isValidSubobject( newObject );

	if( !isEnabled )
	{
		CI_STRING	tag = newObject->getTag();
		if( tag == "AREA" )
			isEnabled = true;
	}

	return isEnabled;
}

/*
--------------------------------------------------------------------------
	getTag
--------------------------------------------------------------------------
*/
STRING ANCHOR::getTag( void ) const
{
	return "A";
}

STRING ABBREVIATION::getTag( void ) const
{
	return "ABBR";
}

STRING ACRONYM::getTag( void ) const
{
	return "ACRONYM";
}

STRING ADDRESS::getTag( void ) const
{
	return "ADDRESS";
}

STRING APPLET::getTag( void ) const
{
	return "APPLET";
}

STRING IMAGE_AREA::getTag( void ) const
{
	return "AREA";
};

STRING BOLD::getTag( void ) const
{
	return "B";
};

STRING BLINK::getTag( void ) const
{
	return "BLINK";
};

STRING BASE_TARGET::getTag( void ) const
{
	return "BASE";
}

STRING BASE_FONT::getTag( void ) const
{
	return "BASEFONT";
}

STRING DIRECTION::getTag( void ) const
{
	return "BDO";
}

STRING BIG::getTag( void ) const
{
	return "BIG";
}

STRING BLOCKQUOTE::getTag( void ) const
{
	return "BLOCKQUOTE";
}

STRING BODY::getTag( void ) const
{
	return "BODY";
}

STRING LINE_BREAK::getTag( void ) const
{
	return "BR";
}

STRING BUTTON::getTag( void ) const
{
	return "BUTTON";
}

STRING TABLE_CAPTION::getTag( void ) const
{
	return "CAPTION";
}

STRING CENTER_BLOCK::getTag( void ) const
{
	return "CENTER";
}

STRING CITE::getTag( void ) const
{
	return "CITE";
}

STRING SOURCE_CODE::getTag( void ) const
{
	return "CODE";
}

STRING TABLE_COL::getTag( void ) const
{
	return "COL";
}

STRING TABLE_COLGROUP::getTag( void ) const
{
	return "COLGROUP";
}

STRING DEFINITION_LIST_DEF::getTag( void ) const
{
	return "DD";
}

STRING DEFINITION::getTag( void ) const
{
	return "DFN";
}

STRING DIRECTORY_LIST::getTag( void ) const
{
	return "DIR";
}

STRING DIV_BLOCK::getTag( void ) const
{
	return "DIV";
}

STRING DEFINITION_LIST::getTag( void ) const
{
	return "DL";
}

STRING DEFINITION_LIST_TERM::getTag( void ) const
{
	return "DT";
}

STRING EMPHASIS::getTag( void ) const
{
	return "EM";
}

STRING FIELDSET::getTag( void ) const
{
	return "FIELDSET";
}

STRING FONT::getTag( void ) const
{
	return "FONT";
}

STRING FORM::getTag( void ) const
{
	return "FORM";
}

STRING FRAME::getTag( void ) const
{
	return "FRAME";
}

STRING FRAMESET::getTag( void ) const
{
	return "FRAMESET";
}

STRING HEADER::getTag( void ) const
{
	char	buffer[32];

	sprintf( buffer, "H%d", level );

	return (STRING)buffer;
}

STRING HEAD::getTag( void ) const
{
	return "HEAD";
}

STRING HORIZONTAL_LINE::getTag( void ) const
{
	return "HR";
}

STRING HTML::getTag( void ) const
{
	return "HTML";
}

STRING ITALIC::getTag( void ) const
{
	return "I";
};

STRING IFRAME::getTag( void ) const
{
	return "IFRAME";
};

STRING IMAGE::getTag( void ) const
{
	return "IMG";
};

STRING INPUT_FIELD::getTag( void ) const
{
	return "INPUT";
}

STRING INDEX::getTag( void ) const
{
	return "ISINDEX";
}

STRING KEYBOARD::getTag( void ) const
{
	return "KBD";
}

STRING LABEL::getTag( void ) const
{
	return "LABEL";
}

STRING LEGEND::getTag( void ) const
{
	return "LEGEND";
}

STRING LIST_ENTRY::getTag( void ) const
{
	return "LI";
}

STRING EXTERNAL_CSS::getTag( void ) const
{
	return "LINK";
}

STRING IMAGE_MAP::getTag( void ) const
{
	return "MAP";
}

STRING MENU_LIST::getTag( void ) const
{
	return "MENU";
}

STRING META::getTag( void ) const
{
	return "META";
}

STRING NOFRAMES::getTag( void ) const
{
	return "NOFRAMES";
}

STRING NOSCRIPT::getTag( void ) const
{
	return "NOSCRIPT";
}

STRING EMBEDED_OBJECT::getTag( void ) const
{
	return "OBJECT";
}

STRING NUMERIC_LIST::getTag( void ) const
{
	return "OL";
}

STRING SELECT_OPTION::getTag( void ) const
{
	return "OPTION";
}

STRING PARAGRAPH::getTag( void ) const
{
	return "P";
}

STRING APPLET_PARAM::getTag( void ) const
{
	return "PARAM";
}

STRING PREFORMATED_TEXT::getTag( void ) const
{
	return "PRE";
}

STRING QUOTE::getTag( void ) const
{
	return "Q";
}

STRING STRIKE::getTag( void ) const
{
	return "S";
};

STRING EXAMPLE::getTag( void ) const
{
	return "SAMP";
};

STRING JAVA_SCRIPT::getTag( void ) const
{
	return "SCRIPT";
}

STRING SELECT::getTag( void ) const
{
	return "SELECT";
}

STRING SMALL::getTag( void ) const
{
	return "SMALL";
}

STRING HTML_TEXT::getTag( void ) const
{
	STRING	attributes = getAttributes( xml::HTML_MODE );

	return attributes[0U] ? "SPAN" : "";
}

STRING STRONG::getTag( void ) const
{
	return "STRONG";
};

STRING CSS_STYLE::getTag( void ) const
{
	return "STYLE";
}

STRING SUBSCRIPT::getTag( void ) const
{
	return "SUB";
}

STRING SUPERSCRIPT::getTag( void ) const
{
	return "SUP";
}

STRING TABLE::getTag( void ) const
{
	return "TABLE";
}

STRING TABLE_BODY::getTag( void ) const
{
	return "TBODY";
}

STRING TABLE_FIELD::getTag( void ) const
{
	return "TD";
}

STRING TEXTAREA::getTag( void ) const
{
	return "TEXTAREA";
}

STRING TABLE_FOOT::getTag( void ) const
{
	return "TFOOT";
}

STRING TABLE_HEADER_FIELD::getTag( void ) const
{
	return "TH";
}

STRING TABLE_HEAD::getTag( void ) const
{
	return "THEAD";
}

STRING TITLE::getTag( void ) const
{
	return "TITLE";
}

STRING TABLE_RECORD::getTag( void ) const
{
	return "TR";
}

STRING TYPE_WRITER::getTag( void ) const
{
	return "TT";
}

STRING UNDERLINE::getTag( void ) const
{
	return "U";
}

STRING BULLET_LIST::getTag( void ) const
{
	return "UL";
}

STRING VARIABLE::getTag( void ) const
{
	return "VAR";
}

/*
--------------------------------------------------------------------------
	isValidAttribute
--------------------------------------------------------------------------
*/

bool HtmlBase::isValidAttribute( const STRING &name )
{
	if( name == "xmlns" || name.beginsWith( "xmlns:" ) )
/***/	return true;

	ArrayOfStrings	attributes;

	getValidAttributes( &attributes );

	for( size_t i=0; i<attributes.size(); i++ )
	{
		if( !strcmpi( name, attributes[i] ) )
/***/		return true;
	}

	return false;
}

/*
--------------------------------------------------------------------------
	getValidAttributes
--------------------------------------------------------------------------
*/

void HtmlBase::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "id" );
	*attributes += STRING( "title" );
	*attributes += STRING( "style" );
	*attributes += STRING( "class" );
	*attributes += STRING( "dir" );
	*attributes += STRING( "lang" );
	*attributes += STRING( "onclick" );
	*attributes += STRING( "ondblclick" );
	*attributes += STRING( "onmousedown" );
	*attributes += STRING( "onmouseup" );
	*attributes += STRING( "onmouseover" );
	*attributes += STRING( "onmousemove" );
	*attributes += STRING( "onmouseout" );
	*attributes += STRING( "onkeypress" );
	*attributes += STRING( "onkeydown" );
	*attributes += STRING( "onkeyup" );
}

void META::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "name" );
	*attributes += STRING( "content" );
	*attributes += STRING( "http-equiv" );
	*attributes += STRING( "scheme" );

	HtmlBase::getValidAttributes( attributes );
}

void BODY::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "alink" );
	*attributes += STRING( "background" );
	*attributes += STRING( "bgcolor" );
	*attributes += STRING( "link" );
	*attributes += STRING( "onload" );
	*attributes += STRING( "onunload" );
	*attributes += STRING( "text" );
	*attributes += STRING( "vlink" );

	HtmlBase::getValidAttributes( attributes );
}

void FRAME::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "frameborder" );
	*attributes += STRING( "longdesc" );
	*attributes += STRING( "marginwidth" );
	*attributes += STRING( "marginheight" );
	*attributes += STRING( "name" );
	*attributes += STRING( "noresize" );
	*attributes += STRING( "scrolling" );
	*attributes += STRING( "src" );

	HtmlBase::getValidAttributes( attributes );
}

void FRAMESET::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "cols" );
	*attributes += STRING( "onload" );
	*attributes += STRING( "onunload" );
	*attributes += STRING( "rows" );

	HtmlBase::getValidAttributes( attributes );
}

void IFRAME::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );
	*attributes += STRING( "frameborder" );
	*attributes += STRING( "height" );
	*attributes += STRING( "longdesc" );
	*attributes += STRING( "marginwidth" );
	*attributes += STRING( "marginheight" );
	*attributes += STRING( "name" );
	*attributes += STRING( "scrolling" );
	*attributes += STRING( "src" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void ANCHOR::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "accesskey" );
	*attributes += STRING( "charset" );
	*attributes += STRING( "coords" );
	*attributes += STRING( "href" );
	*attributes += STRING( "hreflang" );
	*attributes += STRING( "name" );
	*attributes += STRING( "onblur" );
	*attributes += STRING( "onfocus" );
	*attributes += STRING( "rel" );
	*attributes += STRING( "rev" );
	*attributes += STRING( "shape" );
	*attributes += STRING( "tabindex" );
	*attributes += STRING( "target" );
	*attributes += STRING( "type" );

	HtmlBase::getValidAttributes( attributes );
}

void LINE_BREAK::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "clear" );

	HtmlBase::getValidAttributes( attributes );
}

void FONT::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "color" );
	*attributes += STRING( "face" );
	*attributes += STRING( "size" );

	HtmlBase::getValidAttributes( attributes );
}

void IMAGE::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );
	*attributes += STRING( "alt" );
	*attributes += STRING( "border" );
	*attributes += STRING( "height" );
	*attributes += STRING( "hspace" );
	*attributes += STRING( "ismap" );
	*attributes += STRING( "longdesc" );
	*attributes += STRING( "name" );
	*attributes += STRING( "src" );
	*attributes += STRING( "usemap" );
	*attributes += STRING( "vspace" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void HEADER::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );

	HtmlBase::getValidAttributes( attributes );
}

void PARAGRAPH::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );

	HtmlBase::getValidAttributes( attributes );
}

void DIV_BLOCK::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );
	*attributes += STRING( "border" );
	*attributes += STRING( "bgcolor" );
	*attributes += STRING( "cellpadding" );
	*attributes += STRING( "cellspacing" );
	*attributes += STRING( "frame" );
	*attributes += STRING( "rules" );
	*attributes += STRING( "summary" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_PART::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );
	*attributes += STRING( "char" );
	*attributes += STRING( "charoff" );
	*attributes += STRING( "valign" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_CAPTION::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_COL::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );
	*attributes += STRING( "char" );
	*attributes += STRING( "charoff" );
	*attributes += STRING( "span" );
	*attributes += STRING( "valign" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_COLGROUP::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "align" );
	*attributes += STRING( "char" );
	*attributes += STRING( "charoff" );
	*attributes += STRING( "span" );
	*attributes += STRING( "valign" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_FIELD::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "abbr" );
	*attributes += STRING( "align" );
	*attributes += STRING( "axis" );
	*attributes += STRING( "bgcolor" );
	*attributes += STRING( "char" );
	*attributes += STRING( "charoff" );
	*attributes += STRING( "colspan" );
	*attributes += STRING( "headers" );
	*attributes += STRING( "height" );
	*attributes += STRING( "nowrap" );
	*attributes += STRING( "rowspan" );
	*attributes += STRING( "scope" );
	*attributes += STRING( "valign" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_HEADER_FIELD::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "abbr" );
	*attributes += STRING( "align" );
	*attributes += STRING( "axis" );
	*attributes += STRING( "bgcolor" );
	*attributes += STRING( "char" );
	*attributes += STRING( "charoff" );
	*attributes += STRING( "colspan" );
	*attributes += STRING( "headers" );
	*attributes += STRING( "height" );
	*attributes += STRING( "nowrap" );
	*attributes += STRING( "rowspan" );
	*attributes += STRING( "scope" );
	*attributes += STRING( "valign" );
	*attributes += STRING( "width" );

	HtmlBase::getValidAttributes( attributes );
}

void TABLE_RECORD::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "abbr" );
	*attributes += STRING( "bgcolor" );
	*attributes += STRING( "char" );
	*attributes += STRING( "charoff" );
	*attributes += STRING( "valign" );

	HtmlBase::getValidAttributes( attributes );
}

void FORM::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "action" );
	*attributes += STRING( "accept" );
	*attributes += STRING( "accept-charset" );
	*attributes += STRING( "enctype" );
	*attributes += STRING( "method" );
	*attributes += STRING( "name" );
	*attributes += STRING( "onreset" );
	*attributes += STRING( "onsubmit" );
	*attributes += STRING( "target" );

	HtmlBase::getValidAttributes( attributes );
}

void INPUT_FIELD::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "accept" );
	*attributes += STRING( "accesskey" );
	*attributes += STRING( "align" );
	*attributes += STRING( "alt" );
	*attributes += STRING( "checked" );
	*attributes += STRING( "disabled" );
	*attributes += STRING( "ismap" );
	*attributes += STRING( "maxlength" );
	*attributes += STRING( "name" );
	*attributes += STRING( "onblur" );
	*attributes += STRING( "onchange" );
	*attributes += STRING( "onfocus" );
	*attributes += STRING( "onselect" );
	*attributes += STRING( "readonly" );
	*attributes += STRING( "size" );
	*attributes += STRING( "src" );
	*attributes += STRING( "tabindex" );
	*attributes += STRING( "type" );
	*attributes += STRING( "usemap" );
	*attributes += STRING( "value" );

	HtmlBase::getValidAttributes( attributes );
}

void TEXTAREA::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "accesskey" );
	*attributes += STRING( "cols" );
	*attributes += STRING( "disabled" );
	*attributes += STRING( "name" );
	*attributes += STRING( "onblur" );
	*attributes += STRING( "onchange" );
	*attributes += STRING( "onfocus" );
	*attributes += STRING( "onselect" );
	*attributes += STRING( "readonly" );
	*attributes += STRING( "rows" );
	*attributes += STRING( "tabindex" );

	HtmlBase::getValidAttributes( attributes );
}

void SELECT::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "disabled" );
	*attributes += STRING( "multiple" );
	*attributes += STRING( "name" );
	*attributes += STRING( "onblur" );
	*attributes += STRING( "onchange" );
	*attributes += STRING( "onfocus" );
	*attributes += STRING( "size" );
	*attributes += STRING( "tabindex" );

	HtmlBase::getValidAttributes( attributes );
}

void SELECT_OPTION::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "disabled" );
	*attributes += STRING( "label" );
	*attributes += STRING( "selected" );
	*attributes += STRING( "value" );

	HtmlBase::getValidAttributes( attributes );
}

void JAVA_SCRIPT::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "charset" );
	*attributes += STRING( "defer" );
	*attributes += STRING( "event" );
	*attributes += STRING( "language" );
	*attributes += STRING( "for" );
	*attributes += STRING( "src" );
	*attributes += STRING( "type" );
}

void EXTERNAL_CSS::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "charset" );
	*attributes += STRING( "href" );
	*attributes += STRING( "hreflang" );
	*attributes += STRING( "media" );
	*attributes += STRING( "rel" );
	*attributes += STRING( "rev" );
	*attributes += STRING( "target" );
	*attributes += STRING( "type" );

	HtmlBase::getValidAttributes( attributes );
}

void CSS_STYLE::getValidAttributes( ArrayOfStrings *attributes )
{
	*attributes += STRING( "media" );
	*attributes += STRING( "title" );
	*attributes += STRING( "type" );
	*attributes += STRING( "dir" );
	*attributes += STRING( "lang" );
}

/*
--------------------------------------------------------------------------
	getAttributes
--------------------------------------------------------------------------
*/

STRING HtmlBase::getAttributes( xml::GeneratorMode mode ) const
{
	STRING attributes = xml::XmlWithAttributes::getAttributes( mode );
	STRING style = xml::XmlWithAttributes::getAttribute( "style" );

	if( style.isEmpty() )
	{
		STRING bgColor = getAttribute( "bgcolor" );
		if( !bgColor.isEmpty() )
			style += "background-color:" + bgColor + ";";

		STRING valign = getAttribute( "valign" );
		if( !valign.isEmpty() )
			style += " vertical-align:" + valign + ";";

		STRING halign = getAttribute( "align" );
		if( !halign.isEmpty() )
			style += "  text-align:" + halign + ";";

		STRING width = getAttribute( "width" );
		if( !width.isEmpty() )
			style += "  width:" + width + ";";

		STRING height = getAttribute( "height" );
		if( !height.isEmpty() )
			style += "  height:" + height + ";";

		if( !style.isEmpty() )
			attributes += ' ' + formatAttribute( "style", style, mode );
	}

	return attributes;
}

STRING HTML_TEXT::getAttributes( xml::GeneratorMode mode ) const
{
	STRING	style = "";
	STRING	attributes = WithInline::getAttributes( mode );

	if( italic )
		style += "font-style:italic;";

	if( underline )
		style += "text-decoration:underline;";

	if( bold )
		style += "font-weight:bold;";

	if( size )
	{
		style += "font-size:";
		static int percents[8] = { 0, 66, 90, 100, 112, 150, 200, 300  };
		int	tmpSize = size;		// the be compatibe with the old font tag
		if( tmpSize > 7 )
			tmpSize = 7;
		else if( tmpSize < 0 )
			tmpSize = 0;
		tmpSize = percents[tmpSize];
		style += formatNumber( tmpSize );
		style += "%;";
	}

	if( !style.isEmpty() )
		attributes += ' ' + formatAttribute( "style", style, mode );

	return attributes;
}

STRING TABLE::getAttributes( xml::GeneratorMode mode ) const
{
	STRING	attributes = HtmlBase::getAttributes( mode );

	if( border )
		attributes += ' ' + formatAttribute( "border", border, mode );

	if( cellpadding )
		attributes += ' ' + formatAttribute( "cellpadding", cellpadding, mode );

	return attributes;
}

STRING TABLE_FIELD::getAttributes( xml::GeneratorMode mode ) const
{
	STRING	attributes = HtmlBase::getAttributes( mode );

	if( colspan > 1 )
		attributes += ' ' + formatAttribute( "colspan", colspan, mode );

	if( nowrap )
		attributes += ' ' + formatAttribute( "nowrap", "nowrap", mode );

	return attributes;
}

STRING SELECT_OPTION::getAttributes( xml::GeneratorMode mode ) const
{
	doEnterFunction("SELECT_OPTION::getAttributes");
	STRING	attributes = HtmlBase::getAttributes( mode );

	if( !m_value.isEmpty() )
	{
		attributes += ' ' + formatAttribute( "value", m_value, mode );
	}

	if( m_selected )
	{
		attributes += ' ' + formatAttribute( "selected", "selected", mode );
	}

	doLogValue(attributes);
	return attributes;
}

STRING TEXTAREA::getAttributes( xml::GeneratorMode mode ) const
{
	STRING	attributes = WithPCData::getAttributes( mode );

	if( cols )
		attributes += ' ' + formatAttribute( "cols", cols, mode );

	if( rows )
		attributes += ' ' + formatAttribute( "rows", rows, mode );

	return attributes;
}

STRING EMBEDED::getAttributes( xml::GeneratorMode mode ) const
{
	STRING	attributes = HtmlBase::getAttributes(mode);

	attributes += ' ' + formatAttribute( "code", code, mode );
	attributes += ' ' + formatAttribute( "codebase", codebase, mode );
	attributes += ' ' + formatAttribute( "width", width, mode );
	attributes += ' ' + formatAttribute( "height", height, mode );

	if( archive[0U] )
		attributes += ' ' + formatAttribute( "archive", archive, mode );

	return attributes;
}

STRING APPLET_PARAM::getAttributes( xml::GeneratorMode mode ) const
{
	STRING	attributes = SimpleTag::getAttributes( mode                                                                                            );

	attributes += ' ' + formatAttribute( "value", value, mode );

	return attributes;
}

/*
--------------------------------------------------------------------------
	other
--------------------------------------------------------------------------
*/

void HEAD::setNoCache( void )
{
	addObject( new NO_CACHE_META );
	addObject( new EXPIRES_META );
}

STRING HTML_TEXT::generate( xml::GeneratorMode mode )
{
	STRING	tag=getTag();

	if( tag[0U] )
		return WithInline::generate( mode );
	else
		return getValue( mode );
}

STRING Document::generateDoc() const
{
	return getValue( xml::HTML_MODE );
}

/*
--------------------------------------------------------------------------
	Copy an XML Element
--------------------------------------------------------------------------
*/
xml::Element *HEAD::createNew( void )
{
	xml::Element *newElement = new HEAD();

	return newElement;
}

xml::Element *TITLE::createNew( void )
{
	xml::Element *newElement = new TITLE();

	return newElement;
}

xml::Element *META::createNew( void )
{
	xml::Element *newElement = new META();

	return newElement;
}

xml::Element *NO_CACHE_META::createNew( void )
{
	xml::Element *newElement = new NO_CACHE_META();

	return newElement;
}

xml::Element *EXPIRES_META::createNew( void )
{
	xml::Element *newElement = new EXPIRES_META();

	return newElement;
}

xml::Element *CSS_STYLE::createNew( void )
{
	xml::Element *newElement = new CSS_STYLE();

	return newElement;
}

xml::Element *EXTERNAL_CSS::createNew( void )
{
	xml::Element *newElement = new EXTERNAL_CSS();

	return newElement;
}

xml::Element *BASE_TARGET::createNew( void )
{
	xml::Element *newElement = new BASE_TARGET();

	return newElement;
}

xml::Element *BASE_FONT::createNew( void )
{
	xml::Element *newElement = new BASE_FONT();

	return newElement;
}

xml::Element *FRAMESET::createNew( void )
{
	xml::Element *newElement = new FRAMESET();

	return newElement;
}

xml::Element *FRAME::createNew( void )
{
	xml::Element *newElement = new FRAME();

	return newElement;
}

xml::Element *IFRAME::createNew( void )
{
	xml::Element *newElement = new IFRAME();

	return newElement;
}

xml::Element *NOFRAMES::createNew( void )
{
	xml::Element *newElement = new NOFRAMES();

	return newElement;
}

xml::Element *INDEX::createNew( void )
{
	xml::Element *newElement = new INDEX();

	return newElement;
}

xml::Element *BUTTON::createNew( void )
{
	xml::Element *newElement = new BUTTON();

	return newElement;
}

xml::Element *BODY::createNew( void )
{
	xml::Element *newElement = new BODY();

	return newElement;
}

xml::Element *HTML_TEXT::createNew( void )
{
	HTML_TEXT *newElement = new HTML_TEXT();

	newElement->bold = bold;
	newElement->underline = underline;
	newElement->italic = italic;
	newElement->size = size;

	return newElement;
}

xml::Element *HEADER::createNew( void )
{
	xml::Element *newElement = new HEADER( level );

	return newElement;
}

xml::Element *DIV_BLOCK::createNew( void )
{
	xml::Element *newElement = new DIV_BLOCK();

	return newElement;
}

xml::Element *CENTER_BLOCK::createNew( void )
{
	xml::Element *newElement = new CENTER_BLOCK();

	return newElement;
}

xml::Element *PARAGRAPH::createNew( void )
{
	xml::Element *newElement = new PARAGRAPH();

	return newElement;
}

xml::Element *ADDRESS::createNew( void )
{
	xml::Element *newElement = new ADDRESS();

	return newElement;
}

xml::Element *BLOCKQUOTE::createNew( void )
{
	xml::Element *newElement = new BLOCKQUOTE();

	return newElement;
}

xml::Element *HORIZONTAL_LINE::createNew( void )
{
	xml::Element *newElement = new HORIZONTAL_LINE();

	return newElement;
}

xml::Element *PREFORMATED_TEXT::createNew( void )
{
	xml::Element *newElement = new PREFORMATED_TEXT();

	return newElement;
}

xml::Element *IMAGE::createNew( void )
{
	xml::Element *newElement = new IMAGE();

	return newElement;
}

xml::Element *IMAGE_MAP::createNew( void )
{
	xml::Element *newElement = new IMAGE_MAP();

	return newElement;
}

xml::Element *IMAGE_AREA::createNew( void )
{
	xml::Element *newElement = new IMAGE_AREA();

	return newElement;
}

xml::Element *ANCHOR::createNew( void )
{
	xml::Element *newElement = new ANCHOR();

	return newElement;
}

xml::Element *LINE_BREAK::createNew( void )
{
	xml::Element *newElement = new LINE_BREAK();

	return newElement;
}

xml::Element *FONT::createNew( void )
{
	xml::Element *newElement = new FONT();

	return newElement;
}

xml::Element *ABBREVIATION::createNew( void )
{
	xml::Element *newElement = new ABBREVIATION();

	return newElement;
}

xml::Element *ACRONYM::createNew( void )
{
	xml::Element *newElement = new ACRONYM();

	return newElement;
}

xml::Element *BOLD::createNew( void )
{
	xml::Element *newElement = new BOLD();

	return newElement;
}

xml::Element *BLINK::createNew( void )
{
	xml::Element *newElement = new BLINK();

	return newElement;
}

xml::Element *DIRECTION::createNew( void )
{
	xml::Element *newElement = new DIRECTION();

	return newElement;
}

xml::Element *BIG::createNew( void )
{
	xml::Element *newElement = new BIG();

	return newElement;
}

xml::Element *CITE::createNew( void )
{
	xml::Element *newElement = new CITE();

	return newElement;
}

xml::Element *SOURCE_CODE::createNew( void )
{
	xml::Element *newElement = new SOURCE_CODE();

	return newElement;
}

xml::Element *DEFINITION::createNew( void )
{
	xml::Element *newElement = new DEFINITION();

	return newElement;
}

xml::Element *EMPHASIS::createNew( void )
{
	xml::Element *newElement = new EMPHASIS();

	return newElement;
}

xml::Element *ITALIC::createNew( void )
{
	xml::Element *newElement = new ITALIC();

	return newElement;
}

xml::Element *KEYBOARD::createNew( void )
{
	xml::Element *newElement = new KEYBOARD();

	return newElement;
}

xml::Element *QUOTE::createNew( void )
{
	xml::Element *newElement = new QUOTE();

	return newElement;
}

xml::Element *STRIKE::createNew( void )
{
	xml::Element *newElement = new STRIKE();

	return newElement;
}

xml::Element *EXAMPLE::createNew( void )
{
	xml::Element *newElement = new EXAMPLE();

	return newElement;
}

xml::Element *SMALL::createNew( void )
{
	xml::Element *newElement = new SMALL();

	return newElement;
}

xml::Element *SUBSCRIPT::createNew( void )
{
	xml::Element *newElement = new SUBSCRIPT();

	return newElement;
}

xml::Element *SUPERSCRIPT::createNew( void )
{
	xml::Element *newElement = new SUPERSCRIPT();

	return newElement;
}

xml::Element *TYPE_WRITER::createNew( void )
{
	xml::Element *newElement = new TYPE_WRITER();

	return newElement;
}

xml::Element *UNDERLINE::createNew( void )
{
	xml::Element *newElement = new UNDERLINE();

	return newElement;
}

xml::Element *VARIABLE::createNew( void )
{
	xml::Element *newElement = new VARIABLE();

	return newElement;
}

xml::Element *STRONG::createNew( void )
{
	xml::Element *newElement = new STRONG();

	return newElement;
}

xml::Element *TABLE::createNew( void )
{
	xml::Element *newElement = new TABLE( border, cellpadding );

	return newElement;
}

xml::Element *TABLE_HEAD::createNew( void )
{
	xml::Element *newElement = new TABLE_HEAD();

	return newElement;
}

xml::Element *TABLE_BODY::createNew( void )
{
	xml::Element *newElement = new TABLE_BODY();

	return newElement;
}

xml::Element *TABLE_FOOT::createNew( void )
{
	xml::Element *newElement = new TABLE_FOOT();

	return newElement;
}

xml::Element *TABLE_CAPTION::createNew( void )
{
	xml::Element *newElement = new TABLE_CAPTION();

	return newElement;
}

xml::Element *TABLE_RECORD::createNew( void )
{
	xml::Element *newElement = new TABLE_RECORD();

	return newElement;
}

xml::Element *TABLE_HEADER_FIELD::createNew( void )
{
	xml::Element *newElement = new TABLE_HEADER_FIELD();

	return newElement;
}

xml::Element *TABLE_FIELD::createNew( void )
{
	TABLE_FIELD *newElement = new TABLE_FIELD();

	newElement->colspan = colspan;
	newElement->nowrap = nowrap;

	return newElement;
}

xml::Element *TABLE_COL::createNew( void )
{
	xml::Element *newElement = new TABLE_COL();

	return newElement;
}

xml::Element *TABLE_COLGROUP::createNew( void )
{
	xml::Element *newElement = new TABLE_COLGROUP();

	return newElement;
}

xml::Element *DIRECTORY_LIST::createNew( void )
{
	xml::Element *newElement = new DIRECTORY_LIST();

	return newElement;
}

xml::Element *MENU_LIST::createNew( void )
{
	xml::Element *newElement = new MENU_LIST();

	return newElement;
}

xml::Element *NUMERIC_LIST::createNew( void )
{
	xml::Element *newElement = new NUMERIC_LIST();

	return newElement;
}

xml::Element *BULLET_LIST::createNew( void )
{
	xml::Element *newElement = new BULLET_LIST();

	return newElement;
}

xml::Element *LIST_ENTRY::createNew( void )
{
	xml::Element *newElement = new LIST_ENTRY();

	return newElement;
}

xml::Element *FORM::createNew( void )
{
	xml::Element *newElement = new FORM();

	return newElement;
}

xml::Element *FIELDSET::createNew( void )
{
	xml::Element *newElement = new FIELDSET();

	return newElement;
}

xml::Element *LEGEND::createNew( void )
{
	xml::Element *newElement = new LEGEND();

	return newElement;
}

xml::Element *LABEL::createNew( void )
{
	xml::Element *newElement = new LABEL();

	return newElement;
}

xml::Element *INPUT_FIELD::createNew( void )
{
	xml::Element *newElement = new INPUT_FIELD();

	return newElement;
}

xml::Element *SELECT::createNew( void )
{
	xml::Element *newElement = new SELECT();

	return newElement;
}

xml::Element *SELECT_OPTION::createNew( void )
{
	xml::Element *newElement = new SELECT_OPTION(m_value,NULL,m_selected);

	return newElement;
}

xml::Element *TEXTAREA::createNew( void )
{
	TEXTAREA *newElement = new TEXTAREA();

	newElement->cols = cols;
	newElement->rows = rows;

	return newElement;
}

xml::Element *APPLET::createNew( void )
{
	APPLET *newElement = new APPLET(codebase, code, width, height);

	newElement->archive = archive;

	return newElement;
}

xml::Element *EMBEDED_OBJECT::createNew( void )
{
	xml::Element *newElement = new EMBEDED_OBJECT();

	return newElement;
}

xml::Element *APPLET_PARAM::createNew( void )
{
	xml::Element *newElement = new APPLET_PARAM( "", value );

	return newElement;
}

xml::Element *JAVA_SCRIPT::createNew( void )
{
	xml::Element *newElement = new JAVA_SCRIPT();

	return newElement;
}

xml::Element *NOSCRIPT::createNew( void )
{
	xml::Element *newElement = new NOSCRIPT();

	return newElement;
}

xml::Element *DEFINITION_LIST::createNew( void )
{
	xml::Element *newElement = new DEFINITION_LIST();

	return newElement;
}

xml::Element *DEFINITION_LIST_TERM::createNew( void )
{
	xml::Element *newElement = new DEFINITION_LIST_TERM();

	return newElement;
}

xml::Element *DEFINITION_LIST_DEF::createNew( void )
{
	xml::Element *newElement = new DEFINITION_LIST_DEF();

	return newElement;
}

xml::Element *HTML::createNew( void )
{
	xml::Element *newElement = new HTML();

	return newElement;
}

xml::Element *Document::createNew( void )
{
	xml::Element *newElement = new Document(getFilename());

	return newElement;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void INPUT_FIELD::setType( enum FIELD_TYPE type )
{
	STRING tag = "button";

	switch( type )
	{
		case HTML_BUTTON:		tag = "button";		break;
		case HTML_SUBMIT:		tag = "submit";		break;
		case HTML_RESET:		tag = "reset";		break;
		case HTML_TEXT_FIELD:	tag = "text";		break;
		case HTML_PASSWORD:		tag = "password";	break;
		case HTML_HIDDEN:		tag = "hidden";		break;
		case HTML_CHECKBOX:		tag = "checkbox";	break;
		case HTML_RADIO:		tag = "radio";		break;
	}

	setStringAttribute( "type", tag );
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace html
}	// namespace gak


