/*
		Project:		GAKLIB
		Module:			xmlEntities.cpp
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

#include <ctype.h>
#include <gak/xml.h>
#include <gak/numericString.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{
namespace xml
{

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static struct
{
	char	charCode;
	char	entity[16];
} xmlEntities[] =
{
	{ '&', "&amp;" },
	{ '<', "&lt;" },
	{ '>', "&gt;" },
	{ '\xA0', "&nbsp;" },
	{ 0, "" }
};

static struct
{
	char	charCode;
	char	entity[16];
} htmlEntities[] =
{
	{ 'ä', "&auml;" },
	{ 'Ä', "&Auml;" },
	{ 'à', "&agrave;" },
	{ 'À', "&Agrave;" },
	{ 'á', "&aacute;" },
	{ 'Á', "&Aacute;" },
	{ 'â', "&acirc;" },
	{ 'Â', "&Acirc;" },

	{ 'è', "&egrave;" },
	{ 'È', "&Egrave;" },
	{ 'é', "&eacute;" },
	{ 'É', "&Eacute;" },
	{ 'ê', "&ecirc;" },
	{ 'Ê', "&Ecirc;" },

	{ 'ì', "&igrave;" },
	{ 'Ì', "&Igrave;" },
	{ 'í', "&iacute;" },
	{ 'Í', "&Iacute;" },
	{ 'î', "&icirc;" },
	{ 'Î', "&Icirc;" },

	{ 'ö', "&ouml;" },
	{ 'Ö', "&Ouml;" },
	{ 'ò', "&ograve;" },
	{ 'Ò', "&Ograve;" },
	{ 'ó', "&oacute;" },
	{ 'Ó', "&Oacute;" },
	{ 'ô', "&ocirc;" },
	{ 'Ô', "&Ocirc;" },

	{ 'ß', "&szlig;" },

	{ 'ü', "&uuml;" },
	{ 'Ü', "&Uuml;" },
	{ 'ù', "&ugrave;" },
	{ 'Ù', "&Ugrave;" },
	{ 'ú', "&uacute;" },
	{ 'Ú', "&Uacute;" },
	{ 'û', "&ucirc;" },
	{ 'Û', "&Ucirc;" },

	{ 'ý', "&yacute;" },
	{ 'Ý', "&Yacute;" },

	{ '§', "&sect;" },
	{ '¿', "&iquest;" },

	{ '&', "&amp;" },
	{ '<', "&lt;" },
	{ '>', "&gt;" },
	{ '"', "&quot;" },
	{ '\xA0', "&nbsp;" },
	{ '\xA9', "&copy;" },
	{ '\xFF', "&nbsp;" },
	{ 0, "" }
};


// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

STRING Element::ascii2XML( char c )
{
	STRING	xmlCode = STRING(c);

	for( size_t i=0; xmlEntities[i].charCode; i++ )
	{
		if( xmlEntities[i].charCode == c )
		{
			xmlCode = xmlEntities[i].entity;
			break;
		}
	}

	return xmlCode;
}

STRING Element::ascii2HTML( char c )
{
	STRING	htmlCode = STRING(c);

	for( size_t i=0; htmlEntities[i].charCode; i++ )
	{
		if( htmlEntities[i].charCode == c )
		{
			htmlCode = htmlEntities[i].entity;
			break;
		}
	}

	return htmlCode;
}

STRING Element::ascii2XML( const STRING &cData )
{
	STRING	xmlCode;
	size_t	i, maxLen = strlen( cData );

	for( i=0; i<maxLen; i++ )
	{
		xmlCode += ascii2XML( cData[i] );
	}

	return xmlCode;
}

STRING Element::ascii2HTML( const STRING &cData )
{
	STRING	htmlCode;
	size_t	i, maxLen = strlen( cData );

	for( i=0; i<maxLen; i++ )
	{
		htmlCode += ascii2HTML( cData[i] );
	}

	return htmlCode;
}

char Element::xml2ASCII( const STRING &entity )
{
	char c = 0;

	if( entity[1U] == '#' )
	{
		c = gak::getValue<char>( ((const char *)entity)+2 );
	}
	else
	{
		for( size_t i=0; xmlEntities[i].charCode; i++ )
		{
			if( entity == xmlEntities[i].entity )
			{
				c = xmlEntities[i].charCode;
				break;
			}
		}
	}

	return c;
}

char Element::html2ASCII( const STRING &entity )
{
	char c = 0;

	if( entity[1U] == '#' )
	{
		c = gak::getValue<char>( ((const char *)entity)+2 );
	}
	else
	{
		for( size_t i=0; htmlEntities[i].charCode; i++ )
		{
			if( entity == htmlEntities[i].entity )
			{
				c = htmlEntities[i].charCode;
				break;
			}
		}
	}

	return c;
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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace xml
}	// namespace gak

