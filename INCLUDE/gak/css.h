/*
		Project:		GAKLIB
		Module:			css.h
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

#ifndef GAKLIB_CSS_H
#define GAKLIB_CSS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>

#include <gak/ci_string.h>
#include <gak/array.h>

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

#ifdef __BORLANDC__
#	pragma option -RT+
#endif
namespace xml
{
	class Element;
	class Document;
}
#ifdef __BORLANDC__
#	pragma option -RT-
#endif


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

class Value : public STRING
{
	bool 	important,		// set by the CSS parser
			byShortForm;

	bool 	checked,		// set by the evaluator (document)
			inherited;

	public:
	Value()
	{
		important = false;
		inherited = byShortForm = checked = false;
	}
	explicit Value( char c ) : STRING( c )
	{
		important = false;
		inherited = byShortForm = checked = false;
	};
	Value( const char *c ) : STRING( c )
	{
		important = false;
		inherited = byShortForm = checked = false;
	};
	Value( const STRING &c ) : STRING( c )
	{
		important = false;
		inherited = byShortForm = checked = false;
	};
	Value( const Value &c ) : STRING( c )
	{
		this->important = c.important;
		this->byShortForm = c.byShortForm;
		this->inherited = c.inherited;
		this->checked = c.checked;
	};

	bool isImportant( void ) const
	{
		return important;
	}
	void setImportant( void )
	{
		important = true;
	}
	void clrImportant( void )
	{
		important = false;
	}

	bool isChecked( void ) const
	{
		return checked;
	}
	void setChecked( void )
	{
		checked = true;
	}

	bool isInherited( void ) const
	{
		return inherited;
	}
	void setInherited( void )
	{
		inherited = true;
	}

	bool isByShortForm( void ) const
	{
		return byShortForm;
	}
	void setByShortForm( void )
	{
		byShortForm = true;
	}
};

struct fieldInfo
{
	const char *cssName;
	size_t		offset;
	bool		inherited;
	bool		isShortForm;
	const char	*defValue;
};

struct Color
{
	unsigned char red, green, blue;
};

enum Display
{
	DSP_NONE, DSP_INLINE, DSP_INLINE_BLOCK,
	DSP_BLOCK, DSP_LIST_ITEM,
	DSP_INLINE_TABLE, DSP_TABLE,
	DSP_HEADER_GROUP, DSP_ROW_GROUP, DSP_FOOTER_GROUP,
	DSP_ROW, DSP_CELL
};

enum Float
{
	FLT_NONE, FLT_LEFT, FLT_RIGHT
};

enum TextAlign
{
	ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
};

enum Position
{
	POS_STATIC, POS_RELATIVE, POS_ABSOLUTE, POS_FIXED
};

enum Border
{
	BORD_NONE, BORD_HIDDEN, BORD_DOTTED, BORD_DASHED,
	BORD_SOLID, BORD_DOUBLE, BORD_GROOVE, BORD_RIDGE,
	BORD_INSET, BORD_OUTSET
};


enum Decoration
{
	DECO_NONE, DECO_UNDERLINE, DECO_STROKE
};

const  int DECO_FLAG_NONE		= (1<<DECO_NONE);
const  int DECO_FLAG_UNDERLINE	= (1<<DECO_UNDERLINE);
const  int DECO_FLAG_STROKE		= (1<<DECO_STROKE);

enum FontStyle
{
	FONT_NORMAL, FONT_ITALIC, FONT_OBLIQUE
};

enum ListStyle
{
	LIST_NONE, LIST_CIRCLE, LIST_DISC, LIST_SQUARE,
	LIST_DECIMAL, LIST_ZERO_DECIMAL, LIST_UPPER, LIST_LOWER
};

enum WhiteSpace
{
	WS_NORMAL, WS_NOWRAP, WS_PRE, WS_PRE_WRAP, WS_PRE_LINE
};

class Styles
{
	static Color defaultColors[];

	static unsigned char getColorComponentValue( const STRING &color );

	// background
	Value		background,
				background_color,
				background_image;

	// block formating
	Value		display, cssFloat, position;
	Value		left, right, top, bottom, width, height;

	Value		margin,
				margin_left,
				margin_right,
				margin_top,
				margin_bottom;
	Value		padding,
				padding_left,
				padding_right,
				padding_top,
				padding_bottom;

	Value		border,
				border_color,
				border_style,
				border_width,
				border_left,
				border_right,
				border_top,
				border_bottom,

				border_left_color,
				border_right_color,
				border_top_color,
				border_bottom_color,
				border_left_style,
				border_right_style,
				border_top_style,
				border_bottom_style,
				border_left_width,
				border_right_width,
				border_top_width,
				border_bottom_width;


	// Font
	Value		color, font_family, font_size, font_weight, font_style,
				text_decoration, letter_spacing;

	// Paragraph
	Value		line_height, text_align, text_indent, white_space;

	// Lists
	Value		list_style_type;

	Border getBorderStyle( const Value &border_style ) const
	{
		return (Border)tokenSearch(
			border_style.isEmpty() ? this->border_style : border_style,
			"none\0hidden\0dotted\0dashed\0solid\0double\0groove\0ridge\0inset\0outset\0\0"
		);
	}

	bool getBorderColor(  const Value &border_color, Color *result ) const
	{
		return parseColor( border_color, result )
			|| parseColor( this->border_color, result )
			|| parseColor( this->color, result );
	}

	Decoration getDecoration( const STRING &decoration ) const
	{
		return (Decoration)tokenSearch(
			decoration,
			"none\0underline\0line-through\0\0"
		);
	}

	void handleShortForm( size_t offset );

	public:
	static bool parseColor( const Value &color, Color *result );
	static fieldInfo	theCssFieldInfo[];
	Value *cssValue( size_t offset )
	{
		Value	*value = (Value *)(((char *)this)+offset);

		return value;
	}

	void readCssStyle( std::istream *theInput, bool toLowerCase );
	void readCssStyle( const STRING &theInput, bool toLowerCase );

	Border getBorderStyleLeft( void ) const
	{
		return getBorderStyle( border_left_style );
	}
	Border getBorderStyleRight( void ) const
	{
		return getBorderStyle( border_right_style );
	}
	Border getBorderStyleTop( void ) const
	{
		return getBorderStyle( border_top_style );
	}
	Border getBorderStyleBottom( void ) const
	{
		return getBorderStyle( border_bottom_style );
	}
	const Value &getBorderWidthLeft( void ) const
	{
		return border_left_width;
	}
	const Value &getBorderWidthRight( void ) const
	{
		return border_right_width;
	}
	const Value &getBorderWidthTop( void ) const
	{
		return border_top_width;
	}
	const Value &getBorderWidthBottom( void ) const
	{
		return border_bottom_width;
	}

	bool getBorderColorLeft( Color *result ) const
	{
		return getBorderColor(  border_left_color, result );
	}
	bool getBorderColorRight( Color *result ) const
	{
		return getBorderColor(  border_right_color, result );
	}
	bool getBorderColorTop( Color *result ) const
	{
		return getBorderColor(  border_top_color, result );
	}
	bool getBorderColorBottom( Color *result ) const
	{
		return getBorderColor(  border_bottom_color, result );
	}

	Value &getBackgroundImage( void )
	{
		return background_image;
	}
	const Value &getFontFamily( void ) const
	{
		return font_family;
	}
	const Value &getFontSize( void ) const
	{
		return font_size;
	}
	const Value &getFontWeight( void ) const
	{
		return font_weight;
	}
	const Value &getLineHeight( void ) const
	{
		return line_height;
	}
	bool getBackgroundColor( Color *result ) const
	{
		return parseColor( background_color, result );
	}
	bool getColor( Color *result ) const
	{
		return parseColor( color, result );
	}
	Float getCssFloat( void ) const
	{
		return (Float)tokenSearch( cssFloat, "none\0left\0right\0\0" );
	}
	Display getDisplay( void ) const
	{
		return (Display)tokenSearch(
			display,
			"none\0inline\0inline-block\0block\0list-item\0"
			"inline-table\0table\0"
			"table-header-group\0table-row-group\0table-footer-group\0"
			"table-row\0table-cell\0\0"
		);
	}
	Position getPosition( void ) const
	{
		return (Position)tokenSearch( position, "static\0relative\0absolute\0fixed\0\0" );
	}
	TextAlign getTextAlign( void ) const
	{
		return (TextAlign)tokenSearch( text_align, "left\0center\0right\0\0" );
	}


	const Value &getTextIndent( void ) const
	{
		return text_indent;
	}
	const Value &getLeft( void ) const
	{
		return left;
	}
	const Value &getTop( void ) const
	{
		return top;
	}
	const Value &getRight( void ) const
	{
		return right;
	}
	const Value &getBottom( void ) const
	{
		return bottom;
	}
	const Value &getWidth( void ) const
	{
		return width;
	}
	const Value &getHeight( void ) const
	{
		return height;
	}
	const Value &getLetterSpacing( void ) const
	{
		return letter_spacing;
	}

	const Value &getMarginLeft( void ) const
	{
		return margin_left;
	}
	const Value &getMarginRight( void ) const
	{
		return margin_right;
	}
	const Value &getMarginTop( void ) const
	{
		return margin_top;
	}
	const Value &getMarginBottom( void ) const
	{
		return margin_bottom;
	}

	const Value &getPaddingLeft( void ) const
	{
		return padding_left;
	}
	const Value &getPaddingRight( void ) const
	{
		return padding_right;
	}
	const Value &getPaddingTop( void ) const
	{
		return padding_top;
	}
	const Value &getPaddingBottom( void ) const
	{
		return padding_bottom;
	}

	int getTextDecorations( void ) const;
	FontStyle getFontStyle( void ) const
	{
		return (FontStyle)tokenSearch( font_style, "normal\0italic\0oblique\0\0" );
	}

	ListStyle getListStyle( void ) const
	{
		return (ListStyle)tokenSearch( list_style_type, "none\0circle\0disc\0square\0decimal\0decimal-leading-zero\0upper-alpha\0lower-alpha\0\0" );
	}
	WhiteSpace getWhiteSpace( void ) const
	{
		return (WhiteSpace)tokenSearch( white_space, "normal\0nowrao\0pre\0pre-wrap\0pre-line\0\0" );
	}
};

/*
	SelectorPart
	this is one part of an selector
	with tagname, id, pseudoClass, classes, attributes
*/
class SelectorPart
{
	enum { CSP_PREDESSOR, CSP_PARENT, CSP_SIBLING } predessorType;
	STRING				tag, id, pseudoClass;
	ArrayOfStrings		cssClasses;
	ArrayOfStrings		attributes;

	public:
	SelectorPart()
	{
		clear();
	}
	void clear()
	{
		predessorType = CSP_PREDESSOR;
		tag = id = pseudoClass = "";
		cssClasses.clear();
	}

	void setPredessor( void )
	{
		predessorType = CSP_PREDESSOR;
	}
	void setParent( void )
	{
		predessorType = CSP_PARENT;
	}
	void setSibling( void )
	{
		predessorType = CSP_SIBLING;
	}
	bool isPredessor( void ) const
	{
		return predessorType == CSP_PREDESSOR;
	}
	bool isParent( void ) const
	{
		return predessorType == CSP_PARENT;
	}
	bool isSibling( void ) const
	{
		return predessorType == CSP_SIBLING;
	}

	void setTag( const STRING &tag )
	{
		this->tag = tag;
	}
	void setId( const STRING &id )
	{
		this->id = id;
	}
	void addCssClass( const STRING &cssClass )
	{
		if( !cssClass.isEmpty() && cssClass != "*" )
			cssClasses += cssClass;
	}
	void addAttribute( const STRING &attribute )
	{
		if( !attribute.isEmpty() )
			attributes += attribute;
	}
	void setPseudoClass( const STRING &pseudoClass )
	{
		this->pseudoClass = pseudoClass;
	}

	bool match( xml::Element *theElement ) const;

	unsigned long getSpecification( void ) const
	{
		unsigned long specification = 0;

		if( !id.isEmpty() && id != "*" )
			specification += 0x010000;
		if( !pseudoClass.isEmpty() && pseudoClass != "*" )
			specification += 0x0100;
		specification += 0x0100UL * (unsigned long)(cssClasses.size() + attributes.size());
		if( !tag.isEmpty() && tag != "*" )
			specification += 0x01;

		return specification;
	}
};

/*
	Selector
	this is a selector in the form
	tag class id pseudoclass ...
	eg:
	H1 BODY
	H1.myTitle
*/
class Selector
{
	unsigned long		specification;
	Array<SelectorPart>	path;

	public:
	Selector()
	{
		specification = 0;
	}
	bool match( xml::Element *theElement ) const
	{
		if( path.size() )
		{
			return path[path.size()-1].match( theElement );
		}
		else
		{
			return false;
		}
	}

	bool hasPredessor( void ) const
	{
		return path.size() > 1;
	}

	void addElement( const SelectorPart &theSpec )
	{
		path += theSpec;
		specification = 0;
	}
	void clear( void )
	{
		path.clear();
		specification = 0;
	}
	size_t size( void ) const
	{
		return path.size();
	}
	const SelectorPart &getElement( size_t index ) const
	{
		return path[index];
	}
	unsigned long getSpecification( void );
};

struct Rule
{
	CI_STRING		media;
	Array<Selector>	selectorList;
	Styles			styles;
};

/*
	Rules is a list of all CSS specifijations in one file
*/
class Rules : public Array<Rule>
{
	void readRule( std::istream *theInput, STRING firstIdentifier, STRING media, bool toLowerCase );
	void readMedia( std::istream *theInput, bool toLowerCase );

	public:
	void readCssFile( std::istream *theInput, bool toLowerCase );
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

}	// namespace css
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

#endif
