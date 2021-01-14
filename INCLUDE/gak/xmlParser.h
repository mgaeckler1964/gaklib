/*
		Project:		GAKLIB
		Module:			xmlParser.h
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

#ifndef XML_PARSER_H
#define XML_PARSER_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <memory>

#include <gak/textReader.h>
#include <gak/xml.h>
#include <gak/set.h>

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
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class Factory
{
	public:
	virtual Element *createObject( const STRING &theTag );
};

class Parser : public TextReader
{
	Factory		*m_factory;
	Set<STRING>	m_identifiers;
	bool		m_utf8;

	private:
	bool processPcData( 
		Element *theXMLData, const STRING &stripedPCdata, 
		bool includeBlanks, int level 
	);
	Element *processEndTag( Element *theXMLData, const STRING &theTag );

	Declaration *processXMLdeclaration(  );
	void processProcessorInstructions( Element *theXMLData, const STRING &tag );
	bool processTag( 
		Element *theXMLData, 
		bool includeBlanks, int level, 
		const STRING &theTag, 
		Element **result 
	);

	// protected:
	char findNextMetaChar( const char *meta );
	Element *readSpecialTag( void );
	STRING readProcessingInstruction( void );

	STRING getNextTag( void );

	STRING readPCdata( void );
	STRING readScriptCode( const STRING &forTag );
	STRING readDocType( );

	virtual void getNextAttribute( STRING *attribute, STRING *value );
	STRING parseEntities( const STRING &value );
	virtual char parseEntity( const STRING &entity );

	virtual Element *createObject( const STRING &theTag );

	void setUTF8mode()
	{
		m_utf8 = true;
	}
	void fixCharset( STRING *text )
	{
		if( m_utf8 && text->getCharSet() == STR_ANSI )
		{
			text->setCharSet( STR_UTF8 );
		}
	}

	protected:
	void setFactory( Factory *factory )
	{
		if( m_factory )
		{
			delete m_factory;
		}
		m_factory = factory;
	}
	Factory *getFactory( void ) const
	{
		return m_factory;
	}
	STRING readIdentifier( void );
	Element *readObject( Element *theXMLData, bool includeBlanks, int level );

	public:
	Parser( const STRING &fileName ) : TextReader( fileName )
	{
		m_utf8 = false;
		m_factory = NULL;
	}
	Parser( std::istream *theInput, const STRING &fileName ) : TextReader( theInput, fileName )
	{
		m_utf8 = false;
		m_factory = NULL;
	}
	virtual ~Parser();

	Document *readFile( bool includeBlanks );

	template <typename ProcessorT>
	void parseXML( ProcessorT &processor );
};

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

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4100 )	// 'literal': Unreferenzierter formaler Parameter

#endif
struct XmlNullProcessor
{
	static void processProcessorInstructions( const STRING &tag, const STRING &pi )
	{
	}
	static void processSpecial( Element *subData )
	{
	}
	static void processTag( const STRING &tag )
	{
	}
	static void processAttribute( 
		const STRING &tag, const STRING &attribute, const STRING &value 
	)
	{
	}
	static void processAttributes( 
		const STRING &tag, const PairMap<STRING,STRING> &attributes 
	)
	{
	}
	static void processPcData( const STRING &pcData )
	{
	}
	static void processEndTag( const STRING &tag )
	{
	}
};
#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

template <typename ProcessorT>
void Parser::parseXML( ProcessorT &processor )
{
	doEnterFunction( "XML_PARSER::parseXML" );

	m_utf8 = false;
	while( !eof() )
	{
		// read PCDATA if avail until next TAG
		STRING	pcData = readPCdata().stripBlanks();
		if( !pcData.isEmpty() )
		{
			processor.processPcData( pcData );
		}

		// read the tag
		STRING theTag = getNextTag();

		if( theTag[0U] == '/' )
		{
			processor.processEndTag( theTag );
		}
		else if( theTag == "!" )
		{
			std::auto_ptr<Element>	subData( readSpecialTag() );
			if( subData.get() )
			{
				processor.processSpecial( subData.get() );
			}
		}
		else if( theTag.beginsWith( '?' ) )	// xml processor instruction
		{
			if( theTag == "?xml" || theTag == "?XML" )
			{
				delete processXMLdeclaration();
			}
			else
			{
				STRING pi = readProcessingInstruction();
				processor.processProcessorInstructions( theTag, pi );
			}
		}
		else
		{
			processor.processTag( theTag );
		}
		{
			PairMap<STRING,STRING>	attributes;
			while( !eof() )
			{
				STRING	attribute, value;
				getNextAttribute( &attribute, &value );
				if( !attribute.isEmpty() )
				{
					attributes[attribute] = value;
					processor.processAttribute( theTag, attribute, value );
				}
				else
				{
					break;
				}
			}
			processor.processAttributes( theTag, attributes );
		}

		char meta = findNextMetaChar( "?/>" );
		if( meta == '/' || meta == '?' )
		{
			if( meta == '/' )
			{
				processor.processEndTag( theTag );
			}
			// this element is terminated with its start tag
			// => it does not contain any elements search for
			// the terminating >
			findNextMetaChar( ">" );
		}
	}
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

#endif
