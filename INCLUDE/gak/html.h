/*
		Project:		GAKLIB
		Module:			HTML.H
		Description:	HTML-Generator
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

#ifndef GAK_HTML_GENERATOR_H
#define GAK_HTML_GENERATOR_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/xml.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable: 4996 ) // 'xxx': This function or variable may be unsafe. Consider using xxx instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#endif

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
	STRING getAttributes( xml::GeneratorMode mode ) const override;

	HtmlBase( const char *name = nullptr )
	{
		setName( name );
	}

	public:

	bool isCaseSensitive() const override;

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
	STRING getName()
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

	STRING generateDoc() const
	{
		return xml::Element::generate( xml::HTML_MODE );
	}
	virtual xml::Validator *getValidSubobjects( ArrayOfStrings *tags );
	virtual const char *getValidSubobjects() = 0;
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
	bool isInline() override;
	bool isBlock() override;
};

/*
	all HTML elements, that are visible within a block
*/
class InlineElements : public virtual HtmlBase
{
	public:
	bool isInline() override;
	bool isBlock() override;
};

/*
	all HTML elements, neither block nor inline
*/
class NonVisual : public virtual HtmlBase
{
	public:
	bool isInline() override;
	bool isBlock() override;
};

/*
	all HTML-Elements that can have sub elements
*/
class WithValue : public virtual HtmlBase
{
	public:
	bool isSimpleTag() const override;		// for HTML Generation
};

/*
	HTML-Tags that can have text value
*/
class WithPCData : public WithValue
{
	protected:
	void addText( const char *htmlCode, bool includeLineFeed = true  );

	WithPCData( const char *text = nullptr )
	{
		if( text )
			addText( text, false );
	}
	public:
	const char *getValidSubobjects() override;
	bool isValidSubobject( xml::Element *newObject ) override;
};

/*
	HTML-Tags that can have inline elements
*/
class WithInline : public WithPCData
{
	protected:
	WithInline( const char *text = nullptr )
	{	// cannot use consturctor of WithPCData because this does not
		// allow line feeds
		if( text )
			addText( text );
	};
	const char *getValidSubobjects() override;
};

/*
	HTML-Tags that can have block elements
*/
class WithBlock: public WithInline
{
	protected:
	const char *getValidSubobjects() override;
};

/*
	base for tags with no end tag (</end>)
*/
class SimpleTag : public virtual HtmlBase
{
	protected:

	const char *getValidSubobjects() override;
	bool isValidSubobject( xml::Element *newObject ) override;
	bool isSimpleTag() const override;		// for HTML Generation
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
	STRING getTag() const override;
	const char *getValidSubobjects() override;

	void setNoCache();
	xml::Element *createNew() override;
};

/*
	the <title> tag
*/
class TITLE : public WithPCData, public NonVisual
{
	public:
	TITLE( const char *htmlCode=nullptr ) : WithPCData( htmlCode ) {};

	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<meta>-tag
*/
class META : public SimpleTag, public NonVisual
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
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
	xml::Element *createNew() override;
};

class EXPIRES_META : public META
{
	public:
	EXPIRES_META( const char *date = "0")
	{
		setStringAttribute( "HTTP-EQUIV", "expires" );
		setStringAttribute( "CONTENT", date );
	}
	xml::Element *createNew() override;
};

/*
	<style>-tag
*/
class CSS_STYLE : public WithPCData, public NonVisual
{
	public:
	STRING	getTag() const override;
	bool	wantScriptCode() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<link>-tag
*/
class EXTERNAL_CSS : public SimpleTag, public NonVisual
{
	public:
	EXTERNAL_CSS( const char *src=nullptr )
	{
		setStringAttribute( "href", src ? src : "" );
		setStringAttribute( "rel", "stylesheet" );
		setStringAttribute( "type", "text/css" );
	}
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<base>-tag
*/
class BASE_TARGET : public SimpleTag, public NonVisual
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<basefont>-tag
*/
class BASE_FONT : public SimpleTag, public InlineElements
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<frameset>-tag
*/
class FRAMESET : public WithValue, public NonVisual
{
	public:
	const char *getValidSubobjects() override;
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<frame>-tag
*/
class FRAME : public SimpleTag, public NonVisual
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<Iframe>-tag
*/
class IFRAME : public WithBlock, public InlineElements
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

class NOFRAMES : public WithBlock, BlockElements
{
	public:
	STRING	getTag() const override;
	const char *getValidSubobjects() override;
	xml::Element *createNew() override;
};

/*
	<isindex>-tag
*/
class INDEX : public SimpleTag, public BlockElements
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

class BUTTON : public WithBlock, public InlineElements
{
	public:
	STRING getTag() const override;
	const char *getValidSubobjects() override;
	xml::Element *createNew() override;
};

/*
	<body>-tag
*/
class BODY : public WithBlock, public NonVisual
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
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
	HTML_TEXT( const char *text=nullptr ) : WithInline( text )
	{
		bold = underline = italic = false;
		size=0;
	}
	STRING getTag() const override;
	STRING getAttributes( xml::GeneratorMode mode ) const override;
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
	xml::Element *createNew() override;
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
	HEADER( int level, const char *htmlCode=nullptr ) : WithInline( htmlCode )
	{
		this->level = level;
	}
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<div>-tag
*/
class DIV_BLOCK : public WithBlock, public BlockElements
{
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<center>-tag
*/
class CENTER_BLOCK : public WithBlock, public BlockElements
{
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<p>-tag
*/
class PARAGRAPH : public WithInline, public BlockElements
{
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<address>-tag
*/
class ADDRESS : public WithInline, public BlockElements
{
	STRING getTag() const override;
	bool isValidSubobject( xml::Element *newObject );
	xml::Element *createNew() override;
};

/*
	<blockqoute>-tag
*/
class BLOCKQUOTE : public WithBlock, public BlockElements
{
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<hr>-tag
*/
class HORIZONTAL_LINE : public SimpleTag, public InlineElements
{
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<pre>-tag
*/
class PREFORMATED_TEXT : public WithBlock, public BlockElements
{
	STRING getTag() const override;
	xml::Element *createNew() override;
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
	IMAGE( const char *src = nullptr )
	{
		if( src )
			setImageSource( src );
	}
	void setImageSource( const char *src )
	{
		setStringAttribute( "SRC", src );
	}
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<map>-tag
*/
class IMAGE_MAP : public WithBlock, InlineElements
{
	public:
	STRING getTag() const override;
	bool isValidSubobject( xml::Element *newObject ) override;
	xml::Element *createNew() override;
};

/*
	<area>-tag
*/
class IMAGE_AREA : public SimpleTag, public NonVisual
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<a>-tag
*/
class ANCHOR : public WithInline, public InlineElements
{
	public:
	ANCHOR( const char *url = nullptr, const char *anchor = nullptr, const char *target = nullptr )
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

	STRING getTag() const override;
	bool isValidSubobject( xml::Element *newObject ) override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<br>-tag
*/
class LINE_BREAK : public SimpleTag, public InlineElements
{
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<font>-tag
*/
class FONT : public WithInline, InlineElements
{
	public:
	FONT( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	void setSize( int size, const char *unit = "" )
	{
		STRING	sizeAttrib = formatNumber( size );

		sizeAttrib += unit;

		setStringAttribute( "size", sizeAttrib );
	}
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<abbr>-tag
*/
class ABBREVIATION : public WithInline, InlineElements
{
	public:
	ABBREVIATION( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<acronym>-tag
*/
class ACRONYM : public WithInline, InlineElements
{
	public:
	ACRONYM( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const;
	xml::Element *createNew() override;
};


/*
	<b>-tag
*/
class BOLD : public WithInline, InlineElements
{
	public:
	BOLD( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const;
	xml::Element *createNew() override;
};

/*
	<blink>-tag
*/
class BLINK : public WithInline, InlineElements
{
	public:
	BLINK( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<bdo>-tag
*/
class DIRECTION : public WithInline, InlineElements
{
	public:
	DIRECTION( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<big>-tag
*/
class BIG : public WithInline, InlineElements
{
	public:
	BIG( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<cite>-tag
*/
class CITE : public WithInline, InlineElements
{
	public:
	CITE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<code>-tag
*/
class SOURCE_CODE : public WithInline, InlineElements
{
	public:
	SOURCE_CODE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<dfn>-tag
*/
class DEFINITION : public WithInline, InlineElements
{
	public:
	DEFINITION( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<em>-tag
*/
class EMPHASIS : public WithInline, InlineElements
{
	public:
	EMPHASIS( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<i>-tag
*/
class ITALIC : public WithInline, InlineElements
{
	public:
	ITALIC( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<kbd>-tag
*/
class KEYBOARD : public WithInline, InlineElements
{
	public:
	KEYBOARD( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<q>-tag
*/
class QUOTE : public WithInline, InlineElements
{
	public:
	QUOTE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<s>-tag
*/
class STRIKE : public WithInline, InlineElements
{
	public:
	STRIKE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<samp>-tag
*/
class EXAMPLE : public WithInline, InlineElements
{
	public:
	EXAMPLE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<small>-tag
*/
class SMALL : public WithInline, InlineElements
{
	public:
	SMALL( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<sub>-tag
*/
class SUBSCRIPT : public WithInline, InlineElements
{
	public:
	SUBSCRIPT( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<sup>-tag
*/
class SUPERSCRIPT : public WithInline, InlineElements
{
	public:
	SUPERSCRIPT( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<tt>-tag
*/
class TYPE_WRITER : public WithInline, InlineElements
{
	public:
	TYPE_WRITER( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<u>-tag
*/
class UNDERLINE : public WithInline, InlineElements
{
	public:
	UNDERLINE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<var>-tag
*/
class VARIABLE : public WithInline, InlineElements
{
	public:
	VARIABLE( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<strong>-tag
*/
class STRONG : public WithInline, InlineElements
{
	public:
	STRONG( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
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
	STRING getTag() const override;
	STRING getAttributes( xml::GeneratorMode mode ) const override;
	const char *getValidSubobjects() override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

class TABLE_PART : public NonVisual, WithValue
{
	public:
	const char *getValidSubobjects() override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
};

/*
	<thead>-tag
*/
class TABLE_HEAD : public TABLE_PART
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<tbody>-tag
*/
class TABLE_BODY : public TABLE_PART
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<tfoot>-tag
*/
class TABLE_FOOT : public TABLE_PART
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<caption>-tag
*/
class TABLE_CAPTION : public NonVisual, WithInline
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<tr>-tag
*/
class TABLE_RECORD : public NonVisual, WithValue
{
	public:
	STRING getTag() const override;
	const char *getValidSubobjects() override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<th>-tag
*/
class TABLE_HEADER_FIELD : public WithBlock, NonVisual
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
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
	STRING getTag() const override;
	STRING getAttributes( xml::GeneratorMode mode ) const override;

	void setColspan( int colspan )
	{
		this->colspan = colspan;
	}
	void setNowrap()
	{
		nowrap = true;
	}
	void clrNowrap()
	{
		nowrap = false;
	}
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<col>-tag
*/
class TABLE_COL : public SimpleTag, NonVisual
{
	public:
	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<colgroup>-tag
*/
class TABLE_COLGROUP : public WithValue, NonVisual
{
	public:
	STRING getTag() const override;
	const char *getValidSubobjects() override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	lists
	-----
*/
class ListBase : public BlockElements, WithValue
{
	const char *getValidSubobjects() override;
};

/*
	<dir>-tag
*/
class DIRECTORY_LIST : public ListBase
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<menu>-tag
*/
class MENU_LIST : public ListBase
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<ol>-tag
*/
class NUMERIC_LIST : public ListBase
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<ul>-tag
*/
class BULLET_LIST : public ListBase
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<li>-tag
*/
class LIST_ENTRY : public WithBlock, NonVisual
{
	public:
	STRING getTag() const override;
	xml::Element *createNew() override;
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

		if( (env=getenv( "SCRIPT_NAME" )) != nullptr )
			setAction( env );
		setMethod( "POST" );
	}

	STRING getTag() const override;
	bool isValidSubobject( xml::Element *newObject ) override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

class FIELDSET : public WithBlock, BlockElements
{
	STRING getTag() const override;
	bool isValidSubobject( xml::Element *newObject ) override;
	xml::Element *createNew() override;
};

class LEGEND : public WithBlock, NonVisual
{
	STRING getTag() const override;
	xml::Element *createNew() override;
};

/*
	<label>-tag
*/
class LABEL : public WithInline, InlineElements
{
	public:
	LABEL( const char *text = nullptr ) : WithInline( text ) {}
	STRING getTag() const override;
	xml::Element *createNew() override;
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
	INPUT_FIELD( enum FIELD_TYPE type = HTML_BUTTON, const char *name = nullptr, const char *value = nullptr, bool checked=false, bool readOnly=false )
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

	void setChecked()
	{
		setStringAttribute( "checked", "1" );
	}

	void clrChecked()
	{
		deleteAttribute( "checked" );
	}

	void setReadOnly()
	{
		setStringAttribute( "readonly", "1" );
	}

	void clrReadOnly()
	{
		deleteAttribute( "readonly" );
	}

	STRING getTag() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<select>-tag
*/
class SELECT : public InlineElements, WithPCData
{
	public:
	SELECT( const char *name = nullptr )
	{
		setName( name );
	};

	STRING getTag() const override;
	const char *getValidSubobjects() override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
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
	STRING getTag() const override;
	STRING getAttributes( xml::GeneratorMode mode ) const override;

	void setSelection()
	{
		m_selected = true;
	}
	void clrSelection()
	{
		m_selected = false;
	}
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

/*
	<textarea>
*/
class TEXTAREA : public WithPCData, public InlineElements
{
	unsigned int 	cols, rows;

	public:
	TEXTAREA( const char *name = nullptr, const char *value = nullptr, bool readOnly=false )
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

	STRING getTag() const override;
	STRING getAttributes( xml::GeneratorMode mode ) const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
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
	STRING getAttributes( xml::GeneratorMode mode ) const override;
	bool isValidSubobject( xml::Element *newObject ) override;
};

class APPLET : public EMBEDED
{
	STRING getTag() const override;
	public:
	APPLET()
	{
	}
	APPLET( const char *codebase, const char *code, int width, int height )
	: EMBEDED( codebase, code, width, height )
	{
	}
	xml::Element *createNew() override;
};

class EMBEDED_OBJECT : public EMBEDED
{
	STRING getTag() const override;
	xml::Element *createNew() override;
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

	STRING getTag() const override;
	STRING getAttributes( xml::GeneratorMode mode ) const override;
	xml::Element *createNew() override;
};

/*
	the <SCRIPT>-Tag we do support JavaScript only
*/
class JAVA_SCRIPT : public WithPCData, public InlineElements
{
	public:
	JAVA_SCRIPT( const char *src = nullptr, const char *code = nullptr ) : WithPCData( code )
	{
		setStringAttribute( "language", "javascript" );
		setSource( src );
	}
	void setSource( const char *src )
	{
		if( src && *src )
			setStringAttribute( "SRC", src );
	}
	STRING	getTag() const override;
	bool	wantScriptCode() const override;
	void getValidAttributes( ArrayOfStrings *attributes ) override;
	xml::Element *createNew() override;
};

class NOSCRIPT : public WithBlock, BlockElements
{
	public:
	STRING	getTag() const override;
	xml::Element *createNew() override;
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
	STRING	getTag() const override;
	const char *getValidSubobjects() override;
	xml::Element *createNew() override;
};

/*
	<dt>-tag
*/
class DEFINITION_LIST_TERM : public NonVisual, WithInline
{
	public:
	STRING	getTag() const override;
	xml::Element *createNew() override;
};

/*
	<dd>-tag
*/
class DEFINITION_LIST_DEF : public NonVisual, WithBlock
{
	public:
	STRING	getTag() const override;
	xml::Element *createNew() override;
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
	HTML( const char *title = nullptr )
	{
		theHead = nullptr;
		theBody = nullptr;
		if( title && *title )
		{
			HEAD *theHead = getHead();

			theHead->addObject( new TITLE( title ) );
		}
	}
	HEAD *getHead()
	{
		if( !theHead )
			addObject( theHead = new HEAD );

		return theHead;
	}
	BODY *getBody()
	{
		if( !theBody )
			addObject( theBody = new BODY );

		return theBody;
	}

	void setNoCache()
	{
		HEAD *theHead = getHead();

		theHead->setNoCache();
	}
	STRING getTag() const override;
	const char *getValidSubobjects() override;
	xml::Element *createNew() override;
};

class Document : public xml::Document
{
	public:
	Document( const STRING &fileName ) : xml::Document( fileName ) {}

	bool isValidSubobject( xml::Element *newObject ) override;
	STRING generateDoc() const override;

	HTML *getHtmlElement()
	{
		HTML *theElement = dynamic_cast<HTML*>(getElement( "HTML" ) );

		return theElement;
	}
	xml::Element *createNew() override;
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

#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#endif	// GAK_HTML_GENERATOR_H

