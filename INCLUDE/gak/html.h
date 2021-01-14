/*
		Project:		GAKLIB
		Module:			HTML.H
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

#ifndef GAK_HTML_GENERATOR_H
#define GAK_HTML_GENERATOR_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/xml.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc

#	pragma warn -hid
#	pragma warn -inl
#endif
#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable: 4250 4263 )
#endif

namespace gak
{
namespace html
{


// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

extern const char HTML_STRICT_PUBLIC[];
extern const char HTML_STRICT_SYSTEM[];

extern const char HTML_TRANSITIONAL_PUBLIC[];
extern const char HTML_TRANSITIONAT_SYSTEM[];

extern const char HTML_FRAMESET_PUBLIC[];
extern const char HTML_FRAMESET_SYSTEM[];

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/*
---------------------------------------------------------------------------
	abstract classes
---------------------------------------------------------------------------
*/
/*
	The base class for all HTML Tags
*/
class HtmlBase : public xml::XmlContainer
{
	css::Styles	styles;

	protected:
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;

	HtmlBase( const char *name = NULL )
	{
		setName( name );
	}

	public:

	virtual bool isCaseSensitive( void ) const;

	void setClass( const char *className )
	{
		if( className && *className )
			setStringAttribute( "CLASS", className );
	}
	void setName( const char *name )
	{
		if( name )
			setStringAttribute( "NAME", name );
	}
	STRING getName( void )
	{
		return getAttribute( "NAME" );
	}
	void setTitle( const char *title )
	{
		if( title )
			setStringAttribute( "TITLE", title );
	}
	void setOnClickHandler( const char *source )
	{
		if( source )
			setStringAttribute( "ONCLICK", source );
	}
	void setOnChangeHandler( const char *source )
	{
		if( source )
			setStringAttribute( "ONCHANGE", source );
	}
	void setBgColor( const char *color )
	{
		if( color )
			setStringAttribute( "BGCOLOR", color );
	}
	void setVerticalAlign( const char *valign )
	{
		if( valign )
			setStringAttribute( "VALIGN", valign );
	}
	void setHorizontalAlign( const char *halign )
	{
		if( halign )
			setStringAttribute( "ALIGN", halign );
	}
	void setWidth( const char *width )
	{
		if( width )
			setStringAttribute( "WIDTH", width );
	}
	void setHeight( const char *height )
	{
		if( height )
			setStringAttribute( "HEIGHT", height );
	}

	STRING generateDoc( void ) const
	{
		return xml::Element::generate( xml::HTML_MODE );
	}
	virtual xml::Validator *getValidSubobjects( ArrayOfStrings *tags );
	virtual const char *getValidSubobjects( void ) = 0;
	virtual bool isValidSubobject( xml::Element *newObject );
	virtual void getValidAttributes( ArrayOfStrings *names );
	virtual bool isValidAttribute( const STRING &name );
};

/*
	all HTML elements, that create a new block of lines
*/
class BlockElements : public virtual HtmlBase
{
	public:
	virtual bool isInline( void );
	virtual bool isBlock( void );
};

/*
	all HTML elements, that are visible within a block
*/
class InlineElements : public virtual HtmlBase
{
	public:
	virtual bool isInline( void );
	virtual bool isBlock( void );
};

/*
	all HTML elements, neither block nor inline
*/
class NonVisual : public virtual HtmlBase
{
	public:
	virtual bool isInline( void );
	virtual bool isBlock( void );
};

/*
	all HTML-Elements that can have sub elements
*/
class WithValue : public virtual HtmlBase
{
	public:
	virtual bool isSimpleTag( void ) const;		// for HTML Generation
};

/*
	HTML-Tags that can have text value
*/
class WithPCData : public WithValue
{
	protected:
	void addText( const char *htmlCode, bool includeLineFeed = true  );

	WithPCData( const char *text = NULL )
	{
		if( text )
			addText( text, false );
	}
	public:
	virtual const char *getValidSubobjects( void );
	virtual bool isValidSubobject( xml::Element *newObject );
};

/*
	HTML-Tags that can have inline elements
*/
class WithInline : public WithPCData
{
	protected:
	WithInline( const char *text = NULL )
	{	// cannot use consturctor of WithPCData because this does not
		// allow line feeds
		if( text )
			addText( text );
	};
	virtual const char *getValidSubobjects( void );
};

/*
	HTML-Tags that can have block elements
*/
class WithBlock: public WithInline
{
	protected:
	virtual const char *getValidSubobjects( void );
};

/*
	base for tags with no end tag (</end>)
*/
class SimpleTag : public virtual HtmlBase
{
	protected:

	virtual const char *getValidSubobjects( void );
	virtual bool isValidSubobject( xml::Element *newObject );
	virtual bool isSimpleTag( void ) const;		// for HTML Generation
};

/*
---------------------------------------------------------------------------
	"real" classes
---------------------------------------------------------------------------
*/
/*
	HTML Meta information
	=====================
*/
/*
	HTML Header <head>
*/
class HEAD : public WithValue, public NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual const char *getValidSubobjects( void );

	void setNoCache( void );
	virtual xml::Element *createNew( void );
};

/*
	the <title> tag
*/
class TITLE : public WithPCData, public NonVisual
{
	public:
	TITLE( const char *htmlCode=NULL ) : WithPCData( htmlCode ) {};

	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<meta>-tag
*/
class META : public SimpleTag, public NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	Some usefull meta tags
*/
class NO_CACHE_META : public META
{
	public:
	NO_CACHE_META()
	{
		setStringAttribute( "HTTP-EQUIV", "pragma" );
		setStringAttribute( "CONTENT", "no-cache" );
	}
	virtual xml::Element *createNew( void );
};

class EXPIRES_META : public META
{
	public:
	EXPIRES_META( const char *date = "0")
	{
		setStringAttribute( "HTTP-EQUIV", "expires" );
		setStringAttribute( "CONTENT", date );
	}
	virtual xml::Element *createNew( void );
};

/*
	<style>-tag
*/
class CSS_STYLE : public WithPCData, public NonVisual
{
	public:
	virtual STRING	getTag( void ) const;
	virtual bool	wantScriptCode( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<link>-tag
*/
class EXTERNAL_CSS : public SimpleTag, public NonVisual
{
	public:
	EXTERNAL_CSS( const char *src=NULL )
	{
		setStringAttribute( "href", src ? src : "" );
		setStringAttribute( "rel", "stylesheet" );
		setStringAttribute( "type", "text/css" );
	}
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<base>-tag
*/
class BASE_TARGET : public SimpleTag, public NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<basefont>-tag
*/
class BASE_FONT : public SimpleTag, public InlineElements
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<frameset>-tag
*/
class FRAMESET : public WithValue, public NonVisual
{
	public:
	virtual const char *getValidSubobjects( void );
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<frame>-tag
*/
class FRAME : public SimpleTag, public NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<Iframe>-tag
*/
class IFRAME : public WithBlock, public InlineElements
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

class NOFRAMES : public WithBlock, BlockElements
{
	public:
	virtual STRING	getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual xml::Element *createNew( void );
};

/*
	<isindex>-tag
*/
class INDEX : public SimpleTag, public BlockElements
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

class BUTTON : public WithBlock, public InlineElements
{
	public:
	virtual STRING getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual xml::Element *createNew( void );
};

/*
	<body>-tag
*/
class BODY : public WithBlock, public NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	Text-formating
	--------------
*/
/*
	normal HTML text
*/
class HTML_TEXT : public WithInline, public InlineElements
{
	bool	bold,
			underline,
			italic;
	int		size;

	public:
	HTML_TEXT( const char *text=NULL ) : WithInline( text )
	{
		bold = underline = italic = false;
		size=0;
	}
	virtual STRING getTag( void ) const;
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;
	virtual STRING generate( xml::GeneratorMode mode );

	void setBold( bool newBold = true )
	{
		bold = newBold;
	}
	void setUnderline( bool newUnderline = true )
	{
		underline = newUnderline;
	}
	void setItalic( bool newItalic = true )
	{
		italic = newItalic;
	}
	void setSize( int newSize = 0 )
	{
		size = newSize;
	}
	virtual xml::Element *createNew( void );
};

/*
	-----
	text blocks
	-----
*/
/*
	the header lines <h1> ... <h6>
*/
class HEADER : public WithInline, public BlockElements
{
	int	level;

	public:
	HEADER( int level, const char *htmlCode=NULL ) : WithInline( htmlCode )
	{
		this->level = level;
	}
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<div>-tag
*/
class DIV_BLOCK : public WithBlock, public BlockElements
{
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<center>-tag
*/
class CENTER_BLOCK : public WithBlock, public BlockElements
{
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<p>-tag
*/
class PARAGRAPH : public WithInline, public BlockElements
{
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<address>-tag
*/
class ADDRESS : public WithInline, public BlockElements
{
	virtual STRING getTag( void ) const;
	bool isValidSubobject( xml::Element *newObject );
	virtual xml::Element *createNew( void );
};

/*
	<blockqoute>-tag
*/
class BLOCKQUOTE : public WithBlock, public BlockElements
{
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<hr>-tag
*/
class HORIZONTAL_LINE : public SimpleTag, public InlineElements
{
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<pre>-tag
*/
class PREFORMATED_TEXT : public WithBlock, public BlockElements
{
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};



/*
	inline text
	============
*/

/*
	<img>-tag
*/
class IMAGE : public SimpleTag, InlineElements
{
	public:
	IMAGE( const char *src = NULL )
	{
		if( src )
			setImageSource( src );
	}
	void setImageSource( const char *src )
	{
		setStringAttribute( "SRC", src );
	}
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<map>-tag
*/
class IMAGE_MAP : public WithBlock, InlineElements
{
	public:
	virtual STRING getTag( void ) const;
	virtual bool isValidSubobject( xml::Element *newObject );
	virtual xml::Element *createNew( void );
};

/*
	<area>-tag
*/
class IMAGE_AREA : public SimpleTag, public NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<a>-tag
*/
class ANCHOR : public WithInline, public InlineElements
{
	public:
	ANCHOR( const char *url = NULL, const char *anchor = NULL, const char *target = NULL )
	{
		setUrl( url );
		addAnchor( anchor );
		setTarget( target );
	}

	void addAnchor( const char *anchor )
	{
		if( anchor && *anchor )
			addObject( new xml::PCData( anchor ) );
	}
	void setUrl( const char *url )
	{
		if( url && *url )
			setStringAttribute( "HREF", url );
	}
	void setTarget( const char *target )
	{
		if( target && *target )
			setStringAttribute( "TARGET", target );
	}

	virtual STRING getTag( void ) const;
	virtual bool isValidSubobject( xml::Element *newObject );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<br>-tag
*/
class LINE_BREAK : public SimpleTag, public InlineElements
{
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<font>-tag
*/
class FONT : public WithInline, InlineElements
{
	public:
	FONT( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	void setSize( int size, const char *unit = "" )
	{
		STRING	sizeAttrib = formatNumber( size );

		sizeAttrib += unit;

		setStringAttribute( "size", sizeAttrib );
	}
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<abbr>-tag
*/
class ABBREVIATION : public WithInline, InlineElements
{
	public:
	ABBREVIATION( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<acronym>-tag
*/
class ACRONYM : public WithInline, InlineElements
{
	public:
	ACRONYM( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};


/*
	<b>-tag
*/
class BOLD : public WithInline, InlineElements
{
	public:
	BOLD( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<blink>-tag
*/
class BLINK : public WithInline, InlineElements
{
	public:
	BLINK( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<bdo>-tag
*/
class DIRECTION : public WithInline, InlineElements
{
	public:
	DIRECTION( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<big>-tag
*/
class BIG : public WithInline, InlineElements
{
	public:
	BIG( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<cite>-tag
*/
class CITE : public WithInline, InlineElements
{
	public:
	CITE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<code>-tag
*/
class SOURCE_CODE : public WithInline, InlineElements
{
	public:
	SOURCE_CODE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<dfn>-tag
*/
class DEFINITION : public WithInline, InlineElements
{
	public:
	DEFINITION( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<em>-tag
*/
class EMPHASIS : public WithInline, InlineElements
{
	public:
	EMPHASIS( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<i>-tag
*/
class ITALIC : public WithInline, InlineElements
{
	public:
	ITALIC( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<kbd>-tag
*/
class KEYBOARD : public WithInline, InlineElements
{
	public:
	KEYBOARD( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<q>-tag
*/
class QUOTE : public WithInline, InlineElements
{
	public:
	QUOTE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<s>-tag
*/
class STRIKE : public WithInline, InlineElements
{
	public:
	STRIKE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<samp>-tag
*/
class EXAMPLE : public WithInline, InlineElements
{
	public:
	EXAMPLE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<small>-tag
*/
class SMALL : public WithInline, InlineElements
{
	public:
	SMALL( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<sub>-tag
*/
class SUBSCRIPT : public WithInline, InlineElements
{
	public:
	SUBSCRIPT( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<sup>-tag
*/
class SUPERSCRIPT : public WithInline, InlineElements
{
	public:
	SUPERSCRIPT( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<tt>-tag
*/
class TYPE_WRITER : public WithInline, InlineElements
{
	public:
	TYPE_WRITER( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<u>-tag
*/
class UNDERLINE : public WithInline, InlineElements
{
	public:
	UNDERLINE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<var>-tag
*/
class VARIABLE : public WithInline, InlineElements
{
	public:
	VARIABLE( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<strong>-tag
*/
class STRONG : public WithInline, InlineElements
{
	public:
	STRONG( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	tables
	------
*/
/*
	<table>-tag
*/
class TABLE : public BlockElements, WithValue
{
	int	border;
	int cellpadding;
	public:
	TABLE( int newBorder = 0, int newPadding = 0 )
	{
		border=newBorder;
		cellpadding = newPadding;
	}
	virtual STRING getTag( void ) const;
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;
	virtual const char *getValidSubobjects( void );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

class TABLE_PART : public NonVisual, WithValue
{
	public:
	virtual const char *getValidSubobjects( void );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
};

/*
	<thead>-tag
*/
class TABLE_HEAD : public TABLE_PART
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<tbody>-tag
*/
class TABLE_BODY : public TABLE_PART
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<tfoot>-tag
*/
class TABLE_FOOT : public TABLE_PART
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<caption>-tag
*/
class TABLE_CAPTION : public NonVisual, WithInline
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<tr>-tag
*/
class TABLE_RECORD : public NonVisual, WithValue
{
	public:
	virtual STRING getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<th>-tag
*/
class TABLE_HEADER_FIELD : public WithBlock, NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<td>-tag
*/
class TABLE_FIELD : public WithBlock, NonVisual
{
	int		colspan;
	bool	nowrap;
	public:
	TABLE_FIELD()
	{
		colspan=1;
		nowrap = false;
	}
	virtual STRING getTag( void ) const;
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;

	void setColspan( int colspan )
	{
		this->colspan = colspan;
	}
	void setNowrap( void )
	{
		nowrap = true;
	}
	void clrNowrap( void )
	{
		nowrap = false;
	}
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<col>-tag
*/
class TABLE_COL : public SimpleTag, NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<colgroup>-tag
*/
class TABLE_COLGROUP : public WithValue, NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	lists
	-----
*/
class ListBase : public BlockElements, WithValue
{
	virtual const char *getValidSubobjects( void );
};

/*
	<dir>-tag
*/
class DIRECTORY_LIST : public ListBase
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<menu>-tag
*/
class MENU_LIST : public ListBase
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<ol>-tag
*/
class NUMERIC_LIST : public ListBase
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<ul>-tag
*/
class BULLET_LIST : public ListBase
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<li>-tag
*/
class LIST_ENTRY : public WithBlock, NonVisual
{
	public:
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	HTML FORMS
	----------
*/
/*
	<form>-tag
*/
class FORM : public WithBlock, BlockElements
{
	public:
	void setAction( const char *action )
	{
		setStringAttribute( "ACTION", action );
	}
	void setMethod( const char *method )
	{
		setStringAttribute( "METHOD", method );
	}
	FORM()
	{
		const char *env;

		if( (env=getenv( "SCRIPT_NAME" )) != NULL )
			setAction( env );
		setMethod( "POST" );
	}

	virtual STRING getTag( void ) const;
	virtual bool isValidSubobject( xml::Element *newObject );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

class FIELDSET : public WithBlock, BlockElements
{
	virtual STRING getTag( void ) const;
	virtual bool isValidSubobject( xml::Element *newObject );
	virtual xml::Element *createNew( void );
};

class LEGEND : public WithBlock, NonVisual
{
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<label>-tag
*/
class LABEL : public WithInline, InlineElements
{
	public:
	LABEL( const char *text = NULL ) : WithInline( text ) {}
	STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<input>-tag
*/
class INPUT_FIELD : public SimpleTag, public InlineElements
{
	public:
	enum FIELD_TYPE {	HTML_BUTTON, HTML_SUBMIT, HTML_RESET,
						HTML_TEXT_FIELD, HTML_PASSWORD,
						HTML_CHECKBOX, HTML_RADIO,
						HTML_HIDDEN };

	public:
	INPUT_FIELD( enum FIELD_TYPE type = HTML_BUTTON, const char *name = NULL, const char *value = NULL, bool checked=false, bool readOnly=false )
	{
		setType( type );
		setValue( value );
		setName( name );
		if( checked )
		{
			setChecked();
		}
		if( readOnly )
		{
			setReadOnly();
		}
	}
	void setValue( const char *value )
	{
		if( value && *value )
		{
			setStringAttribute( "value", value );
		}
		else
		{
			deleteAttribute( "value" );
		}
	}
	void setSize( unsigned long size )
	{
		if( size )
		{
			setIntegerAttribute( "size", size );
		}
		else
		{
			deleteAttribute( "size" );
		}
	}
	void setType( enum FIELD_TYPE type );

	void setChecked( void )
	{
		setStringAttribute( "checked", "1" );
	}

	void clrChecked( void )
	{
		deleteAttribute( "checked" );
	}

	void setReadOnly( void )
	{
		setStringAttribute( "readonly", "1" );
	}

	void clrReadOnly( void )
	{
		deleteAttribute( "readonly" );
	}

	virtual STRING getTag( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<select>-tag
*/
class SELECT : public InlineElements, WithPCData
{
	public:
	SELECT( const char *name = NULL )
	{
		setName( name );
	};

	virtual STRING getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<option-tag>
*/
class SELECT_OPTION : public WithPCData, public NonVisual
{
	private:
	STRING	m_value;
	bool	m_selected;

	public:
	SELECT_OPTION(
		const STRING	&value		= NULL_STRING,
		const STRING	&label		= NULL_STRING,
		bool			selected	= false
	) : WithPCData( !label.isEmpty() ? label : value ), m_value( value ), m_selected( selected )
	{
	}
	virtual STRING getTag( void ) const;
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;

	void setSelection( void )
	{
		m_selected = true;
	}
	void clrSelection( void )
	{
		m_selected = false;
	}
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

/*
	<textarea>
*/
class TEXTAREA : public WithPCData, public InlineElements
{
	unsigned int 	cols, rows;

	public:
	TEXTAREA( const char *name = NULL, const char *value = NULL, bool readOnly=false )
	: WithPCData( value )
	{
		rows = cols = 0;
		if( readOnly )
			setStringAttribute( "readonly", "1" );

		setName( name );
	}

	void setDimension( unsigned int cols, unsigned int rows )
	{
		this->cols = cols;
		this->rows = rows;
	}

	virtual STRING getTag( void ) const;
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};


/*
	JAVA and Javascript
*/
/*
	<applet>-tag
*/
class EMBEDED : public InlineElements, WithBlock
{
	protected:
	STRING	code, codebase, archive;
	int		width, height;

	public:
	EMBEDED()
	{
		width=0; height=0;
	}
	EMBEDED( const char *codebase, const char *code, int width, int height )
	{
		this->codebase = codebase;
		this->code = code;
		this->width = width;
		this->height = height;
	}
	void setArchive( const char *archive )
	{
		this->archive = archive;
	}
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;
	virtual bool isValidSubobject( xml::Element *newObject );
};

class APPLET : public EMBEDED
{
	virtual STRING getTag( void ) const;
	public:
	APPLET()
	{
	}
	APPLET( const char *codebase, const char *code, int width, int height )
	: EMBEDED( codebase, code, width, height )
	{
	}
	virtual xml::Element *createNew( void );
};

class EMBEDED_OBJECT : public EMBEDED
{
	virtual STRING getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<param>-tag
*/
class APPLET_PARAM : public SimpleTag, public NonVisual
{
	private:
	STRING	value;
	public:
	APPLET_PARAM() {};

	APPLET_PARAM( const STRING &name, const STRING &value )
	{
		setName( name );
		setValue( value );
	}
	void setValue( const char *value )
	{
		this->value = value;
	}

	virtual STRING getTag( void ) const;
	virtual STRING getAttributes( xml::GeneratorMode mode ) const;
	virtual xml::Element *createNew( void );
};

/*
	the <SCRIPT>-Tag we do support JavaScript only
*/
class JAVA_SCRIPT : public WithPCData, public InlineElements
{
	public:
	JAVA_SCRIPT( const char *src = NULL, const char *code = NULL ) : WithPCData( code )
	{
		setStringAttribute( "language", "javascript" );
		setSource( src );
	}
	void setSource( const char *src )
	{
		if( src && *src )
			setStringAttribute( "SRC", src );
	}
	virtual STRING	getTag( void ) const;
	virtual bool	wantScriptCode( void ) const;
	virtual void getValidAttributes( ArrayOfStrings *attributes );
	virtual xml::Element *createNew( void );
};

class NOSCRIPT : public WithBlock, BlockElements
{
	public:
	virtual STRING	getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	Definition lists
*/
/*
	<dl>-tag
*/
class DEFINITION_LIST : public BlockElements, WithValue
{
	public:
	virtual STRING	getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual xml::Element *createNew( void );
};

/*
	<dt>-tag
*/
class DEFINITION_LIST_TERM : public NonVisual, WithInline
{
	public:
	virtual STRING	getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
	<dd>-tag
*/
class DEFINITION_LIST_DEF : public NonVisual, WithBlock
{
	public:
	virtual STRING	getTag( void ) const;
	virtual xml::Element *createNew( void );
};

/*
=========================================================
	the <html>-tag
=========================================================
*/
class HTML : public NonVisual, WithValue
{
	HEAD	*theHead;
	BODY	*theBody;

	public:
	HTML( const char *title = NULL )
	{
		theHead = NULL;
		theBody = NULL;
		if( title && *title )
		{
			HEAD *theHead = getHead();

			theHead->addObject( new TITLE( title ) );
		}
	}
	HEAD *getHead( void )
	{
		if( !theHead )
			addObject( theHead = new HEAD );

		return theHead;
	}
	BODY *getBody( void )
	{
		if( !theBody )
			addObject( theBody = new BODY );

		return theBody;
	}

	void setNoCache( void )
	{
		HEAD *theHead = getHead();

		theHead->setNoCache();
	}
	virtual STRING getTag( void ) const;
	virtual const char *getValidSubobjects( void );
	virtual xml::Element *createNew( void );
};

class Document : public xml::Document
{
	public:
	Document( const STRING &fileName ) : xml::Document( fileName ) {}

	virtual bool isValidSubobject( xml::Element *newObject );
	virtual STRING generateDoc( void ) const;

	HTML *getHtmlElement( void )
	{
		HTML *theElement = dynamic_cast<HTML*>(getElement( "HTML" ) );

		return theElement;
	}
	virtual xml::Element *createNew( void );
};

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace html
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.

#	pragma warn +hid
#	pragma warn +inl
#endif
#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#endif	// GAK_HTML_GENERATOR_H

