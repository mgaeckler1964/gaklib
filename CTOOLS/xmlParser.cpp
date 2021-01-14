/*
		Project:		GAKLIB
		Module:			xmlParser.cpp
		Description:	XML-Parser
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

#include <gak/xmlParser.h>
#include <gak/gaklib.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

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

Parser::~Parser()
{
	if( m_factory )
	{
		delete m_factory;
	}
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

bool Parser::processPcData( 
	Element *theXMLData, const STRING &pcData, bool includeBlanks, int level 
)
{
	STRING	stripedPCdata = pcData;
	bool	processed = false;

	if( !level || !includeBlanks )
	{
		stripedPCdata.stripBlanks();
	}
	if( !stripedPCdata.isEmpty() )
	{
		PCData	*newPCdata = new PCData( pcData );
		if( theXMLData->isValidSubobject( newPCdata ) )
		{
			theXMLData->addObject( newPCdata );
		}
		else
		{
			// we should add error handling here
			delete newPCdata;
			addError( "PCDATA not allowed here" );
		}
		processed = true;
	}

	return processed;
}

Element *Parser::processEndTag( Element *theXMLData, const STRING &endTag )
{
	// this is the end
	findNextMetaChar( ">" );
	STRING theTag = endTag + std::size_t(1);
	if( !strcmpi( theXMLData->getTag(), theTag ) )
	{
		return theXMLData;
	}
	else
	{
		STRING	theError = "end tag </" + theTag + "> does not match start tag <" + theXMLData->getTag() + '>';
		addError( theError );
		// may be our parent is terminated now: let's check
		Element	*theParent = theXMLData->getParent();
		if( theParent && !strcmpi( theParent->getTag(), theTag ) )
		{
			addError( "--> terminating parent" );
/*@*/		return theParent;
		}
		else
		{
			// it's not the end of our parent: let's ignore
			addError( "--> ignoring" );
		}
	}
	return NULL;
}
Declaration *Parser::processXMLdeclaration(  )
{
	Declaration	*subData = new Declaration();
	while( !eof() )
	{
		STRING	attribute, value;

		getNextAttribute( &attribute, &value );
		if( !attribute.isEmpty() )
		{
			subData->setStringAttribute( attribute, value );
		}
		else
		{
			findNextMetaChar( "?" );
			findNextMetaChar( ">" );
			break;
		}
	}
	//setEncoding( subData->getAttribute( "encoding" ) );
	CI_STRING encoding = subData->getAttribute( "encoding" );
	if( encoding == UTF_8 )
	{
		setUTF8mode();
	}

	return subData;
}

void Parser::processProcessorInstructions( Element *theXMLData, const STRING &tag )
{
	/*
		a prolog is also a special tag in the form
		<?tag attributes ?>
	*/
	STRING theTag = tag + std::size_t(1);
	if( theTag == "xml" || theTag == "XML" )
	{
		theXMLData->addObject( processXMLdeclaration() );
	}
	else if( theTag == "xml-stylesheet" )
	{
		Element	*subData = new StyleSheet();
		theXMLData->addObject( subData );
		while( !eof() )
		{
			STRING	attribute, value;

			getNextAttribute( &attribute, &value );
			if( !attribute.isEmpty() )
			{
				subData->setStringAttribute( attribute, value );
			}
			else
			{
				findNextMetaChar( "?" );
				findNextMetaChar( ">" );
				break;
			}
		}
	}
	else
	{
		STRING pi = readProcessingInstruction();
		theXMLData->addObject(
			new ProcInstruction( theTag, pi )
		);
	}
}

bool Parser::processTag( Element *theXMLData, bool includeBlanks, int level, const STRING &theTag, Element **result )
{
	/*
		this is a real XML Element
	*/
	Element	*subData = !eof() ? createObject( theTag )
							  : NULL;

	while( subData && !eof() )
	{
		Element	*container = theXMLData;
		STRING	prefix = subData->getPrefix();

		if( container->getPrefix() != prefix )
		{
			container = theXMLData->getPrefixParent( prefix );
		}
		if( !container || container->isValidSubobject( subData ) )
		{
			theXMLData->addObject( subData, true );
		}
		else
		{
			STRING theError = "<" + theTag + "> is not allowed within <" + theXMLData->getTag() + '>';
			addError( theError );
			// ask parent
			Element	*theParent = theXMLData->getParent();
			if( theParent && theParent->isValidSubobject( subData ) )
			{
				*result = subData;		// the parent can hold this element
/*@*/			return false;
			}
			else
			{
				theXMLData->addObject( subData, true );
			}
		}

		while( !eof() )
		{
			STRING	attribute, value;
			getNextAttribute( &attribute, &value );
			if( !attribute.isEmpty() )
			{
				subData->setStringAttribute( attribute, value );
			}
			else
			{
				break;
			}
		}

		char meta = findNextMetaChar( "/>" );
		if( meta == '/' )
		{
			// this element is terminated with its start tag
			// => it does not contain any elements search for
			// the terminating >
			findNextMetaChar( ">" );
/*v*/		break;
		}
		else if( subData->wantScriptCode() )
		{
			STRING	scriptCode = readScriptCode( theTag );
			CData	*newCdata = new CData( scriptCode );
			subData->addObject( newCdata );
/*v*/		break;
		}
		else if( subData->isSimpleTag() )
		{
/*v*/		break;
		}
		else
		{
			// this element is not yet terminated
			// search for elements within our elements
			subData = readObject( subData, includeBlanks, level+1 );
			if( subData == theXMLData )
			{
				// read object has found my own terminator
				*result = NULL;
/*@*/			return false;
			}
		}
	}

	return true;
}

char Parser::findNextMetaChar( const char *meta )
{
	char c;

	do
	{
		c = getNextNonBlank();
	} while( !eof() && !strchr( meta, c ) );

	return c;
}

/*
	search for the next element
*/
STRING Parser::getNextTag( void )
{
	STRING	theTag;
	char	c;

	findNextMetaChar( "<" );

	c = getNextNonBlank(); // first character may be a / which is a delimiter for read identifier
	if( c== '!' )
	{
		theTag = '!';
	}
	else
	{
		if( c == '/' || c == '?' )
		{
			theTag = c;
		}
		else
		{
			putback( c );	// some tags consist of only one character
							// if we would not put back this character
							// we would confuse readIdentifiet
		}
		theTag += readIdentifier();
	}
	return theTag;
}

/*
	read an entire tag (used for doctype, comments and CDATA)
*/
Element *Parser::readSpecialTag( void )
{
	char		c;
	STRING		tag = "<!";
	Element		*subData = NULL;

	while( !eof() )
	{
		c = getNextWithBlank();
		if( isSpace( c ) || c=='>' || c=='<' )
		{
			putback( c );
/*v*/		break;
		}

		tag += c;
		if( tag == "<!--"
		||  tag == "<![CDATA["
		||  tag == "<!DOCTYPE" )
		{
			break;
		}
	}

	if( tag== "<!--" || tag == "<![CDATA[" )
	{
		STRING	content;

		while( !eof() )
		{
			content += readString( '>' );
			content += '>';

			if( tag == "<!--" )
			{
				if(  content.endsWith( "-->" ) )
				{
					content.cut( content.strlen() - 3 );
					fixCharset( &content );
					subData = new Comment( content );
					break;	// comment
				}
			}
			else if( tag == "<![CDATA[" )
			{
				if(  content.endsWith( "]]>" ) )
				{
					content.cut( content.strlen() - 3 );
					fixCharset( &content );

					subData = new CData( content );
					break;	// CDATA Element
				}
			}
		}
	}
	else if( tag=="<!DOCTYPE" )
	{
		STRING	content;
		size_t	counter = 0;
		while( !eof() )
		{
			c = getNextWithBlank();
			if( c == '>' )
			{
				if( !counter )
					break;
				counter--;
			}
			if( c == '<' )
				counter++;

			content += c;
		}
		subData = new DocType( content );
	}

	return subData;
}

STRING Parser::readProcessingInstruction( void )
{
	STRING	procInstruction;

	while( !eof() )
	{
		procInstruction += readString( '>' );

		if( procInstruction.endsWith( "?" ) )
		{
			procInstruction.cut( procInstruction.strlen()-1 );
			break;	// end of PI
		}
	}
	fixCharset( &procInstruction );
	return procInstruction;
}

STRING Parser::parseEntities( const STRING &value )
{
	STRING		newValue, entity;
	char		c;
	std::size_t	i = 0;

	STR_CHARSET cs = value.getCharSet();
	while( (c=value[i++]) != 0 )
	{
		if( c=='&' )
		{
			entity = '&';
			while( (c=value[i++]) != 0 )
			{
				entity += c;
				if( c==';' )
				{
					newValue.setCharSet( cs );
					newValue += parseEntity( entity );
					newValue.setCharSet( STR_ANSI );
					break;
				}
				else if( isSpace( c ) )
				{
					addError( "Invalid entity "+entity );
					newValue += entity;
					break;
				}
			}
			if( !c )
			{
				break;
			}

		}
		else
		{
			newValue += c;
		}
	}
	newValue.setCharSet( cs );
	return newValue;
}

STRING Parser::readPCdata( void )
{
	STRING	pcData;
	char	c;

	while( !eof() )
	{
		c = getNextWithBlank();
		if( c == '<' )
		{
			putback( c );
			break;
		}
		else
		{
			pcData += c;
		}
	}

	fixCharset( &pcData );
	return parseEntities( pcData );
}

STRING Parser::readScriptCode( const STRING &forTag )
{
	CI_STRING	wantedEndTag = "</"+forTag+'>';
	CI_STRING	endTag;
	STRING		scriptCode;
	char		c;
	bool		isTag = false;

	while( !eof() )
	{
		c = getNextWithBlank();
		if( c == '<' && !isTag )
		{
			endTag = CI_STRING(c);
			isTag = true;
		}
		else if( isTag )
		{
			endTag += c;
			if( endTag == wantedEndTag )
			{
				break;
			}
			if( strncmpi( wantedEndTag, endTag, strlen( endTag ) ) )
			{
				scriptCode += endTag;
				isTag = false;
			}
		}
		else
		{
			scriptCode += c;
		}
	}

	fixCharset( &scriptCode );
	return scriptCode.stripBlanks();
}

/*
STRING Parser::readCdata( void )
{
	STRING	cData;
	char	c;

	while( !inputStream->eof() )
	{
		c = (char)inputStream->get();
		if( c == '<' )
		{
			inputStream->putback( c );
			break;
		}
		cData += c;
	}

	fixCharset( &cData );
	return cData;
}
*/

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	read an identifier (element name, attribute name)
*/
STRING Parser::readIdentifier( void )
{
	STRING	token;
	const char *delimiters = "?=&<>\"\'/";

	char	c;

	// eat white space
	do
	{
		c = getNextNonBlank();
	} while( !eof() && isspace( c ) );

	// read the identifier
	while( !eof()
	&& !isspace( c )
	&& !strchr( delimiters, c ) )
	{
		token += c;
		c = getNextWithBlank();
	}

	// put back the delimiter
	putback( c );

	fixCharset( &token );
	return m_identifiers.addElement( token );
}

Element *Parser::readObject( Element *theXMLData, bool includeBlanks, int level )
{
	doEnterFunction( "XML_PARSER::readObject" );

	STRING	pcData;
	bool	terminatorFound = false;

	while( !eof() )
	{
		// read PCDATA if avail until next TAG
		pcData = readPCdata();
		if( processPcData( theXMLData, pcData, includeBlanks, level ) )
		{
			pcData = NULL_STRING;
		}

		// read the tag
		STRING theTag = m_identifiers.addElement( getNextTag() );

		if( theTag[0U] == '/' )
		{
			Element *endElement = processEndTag( theXMLData, theTag );
			if( endElement == theXMLData )
			{
				terminatorFound = true;
/*v*/			break;
			}
			else if( endElement )
			{
/*@*/			return endElement;
			}
		}
		else
		{
			if( !pcData.isEmpty() )
			{
				pcData = NULL_STRING;
			}
			// here we have found a new element
			if( theTag == "!" )
			{
				Element	*subData = readSpecialTag();
				if( subData )
				{
					theXMLData->addObject( subData );
				}
			}
			else if( theTag.beginsWith( '?' ) )	// xml processor instruction
			{
				processProcessorInstructions( theXMLData, theTag );
			}
			else
			{
				Element	*subData;
				if( !processTag( theXMLData, includeBlanks, level, theTag, &subData ) )
				{
					return subData;
				}
			}
		}
	}

	if( !terminatorFound )
	{
		STRING myTag = theXMLData->getTag();
		if( !myTag.isEmpty() )
		{
			addError( myTag + " was not terminated correctly" );
		}
	}
	else if( !theXMLData->getNumObjects() && !pcData.isEmpty() )
	{
		PCData	*newPCdata = new PCData( pcData );
		if( theXMLData->isValidSubobject( newPCdata ) )
		{
			theXMLData->addObject( newPCdata );
		}
		else
		{
			// we should add error handling here
			delete newPCdata;
			addError( "PCDATA not allowed here" );
		}
	}

	return NULL;
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
void Parser::getNextAttribute( STRING *attribute, STRING *value )
{
	char	stringDelimiter;

	*value = NULL_STRING;
	*attribute = readIdentifier();
	if( !attribute->isEmpty() )
	{
		findNextMetaChar( "=" );
		stringDelimiter = findNextMetaChar( "\"'" );

		STRING val = readString(stringDelimiter);
		fixCharset( &val );
		*value = parseEntities( val );
	}
}

char Parser::parseEntity( const STRING &entity )
{
	return PCData::xml2ASCII( entity );
}

Element *Parser::createObject( const STRING &theTag )
{
	if( !getFactory() )
	{
		setFactory( new Factory );
	}

	return getFactory()->createObject( theTag );
}

Element *Factory::createObject( const STRING &theTag )
{
	return new Any( theTag );
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

Document *Parser::readFile( bool includeBlanks )
{
	Document	*theXMLData = new Document( getPosition().m_fileName );

	m_utf8 = false;
	readObject( theXMLData, includeBlanks, 0 );

	return theXMLData;
}

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
#endif
