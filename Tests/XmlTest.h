/*
		Project:		GAKLIB
		Module:			XmlTest.h
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

#include <iostream>
#include <gak/unitTest.h>

#include <gak/xml.h>
#include <gak/xmlParser.h>

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
using namespace xml;

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

class XmlTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "XmlTest";
	}
	virtual void PerformTest( void )
	{
		{
			Any		theRoot( "root" );
			Any		*theRecord;

			theRecord = new Any( "record" );
			theRecord->setStringAttribute( "atribute1", "value1.1" );
			theRecord->setBoolAttribute( "atribute2", true );
			theRoot.addObject( theRecord );
			theRecord = new Any( "record" );
			theRecord->setStringAttribute( "atribute1", "value2.1" );
			theRecord->setIntegerAttribute( "atribute2", 22 );

			theRecord->addObject( new CData( "cdata2" ) );
			theRoot.addObject( theRecord );
			theRecord = new Any( "record" );
			theRecord->setStringAttribute( "atribute1", "value3.1" );
			theRecord->setFloatAttribute( "atribute2", 3.2 );

			theRecord->addObject( new CData( "cdata3" ) );
			theRoot.addObject( theRecord );


			STRING	xmlCode = theRoot.generateDoc( "test", "mydata.xsl" );
			STRING	expected =
				STRING("<?xml version=\"1.0\" encoding=\"") + ISO_8859_1 + "\" ?>\n"
				"<!DOCTYPE test SYSTEM \"test.dtd\">\n"
				"<?xml-stylesheet type=\"text/xsl\" href=\"mydata.xsl\" ?>\n"
				"<root>\n"
				"<record atribute1=\"value1.1\" atribute2=\"true\" />\n"
				"<record atribute1=\"value2.1\" atribute2=\"22\">\n"
				"<![CDATA[cdata2]]>\n"
				"</record>\n"
				"<record atribute1=\"value3.1\" atribute2=\"3.20\">\n"
				"<![CDATA[cdata3]]>\n"
				"</record>\n"
				"</root>"
			;

			UT_ASSERT_EQUAL( xmlCode, expected );
			if( xmlCode != expected )
			{
				for( size_t i=0; i<xmlCode.strlen(); i++ )
				{
					char c1 = xmlCode[i];
					char c2 = expected[i];
					UT_ASSERT_EQUAL( c1, c2 );
					if( c1 != c2 )
						break;
				}
			}

			iSTRINGstream				str( expected );
			Parser						theParser( &str, "internal" );
			std::auto_ptr<Document>		theDoc( theParser.readFile( false ) );

			xmlCode = theDoc->generate( XML_MODE );
			UT_ASSERT_EQUAL( xmlCode, expected );
			if( xmlCode != expected )
			{
				for( size_t i=0; i<xmlCode.strlen(); i++ )
				{
					char c1 = xmlCode[i];
					char c2 = expected[i];
					UT_ASSERT_EQUAL( c1, c2 );
					if( c1 != c2 )
						break;
				}
			}
		}

		{
			/// TODO allow PCDATA parsing
			STRING	xml =
				STRING("<?xml version=\"1.0\" encoding=\"") + UTF_8 + "\" ?>\n"
				"<root>\n"
//				"<![PCDATA[&auml;Ä]]>\n"
				"äÄ\n"
				"<record atribute1=\"value3.1\" atribute2=\"äÄ\">\n"
				"<![CDATA[cdata3]]>\n"
				"</record>\n"
				"</root>"
			;
			xml = xml.encodeUTF8();

			iSTRINGstream				str( xml );
			Parser						theParser( &str, "internal" );
			std::auto_ptr<Document>		theDoc( theParser.readFile( false ) );
			Element						*root = theDoc->getRoot(); 
			UT_ASSERT_EQUAL( STRING("root"), root->getTag() );
			Element						*cData = root->getElement( 0 );
			PCData						*pcData = dynamic_cast<PCData*>( cData );
			UT_ASSERT_TRUE( pcData != NULL );
			if( pcData != NULL )
			{
				STRING pcValue = pcData->getValue( PLAIN_MODE );
				UT_ASSERT_EQUAL( STRING("\näÄ\n"), pcValue );
				UT_ASSERT_EQUAL( STR_UTF8, pcValue.getCharSet() );
			}
			Element						*record = root->getElement( 1 );
			UT_ASSERT_EQUAL( STRING("record"), record->getTag() );

			STRING atribute1 = record->getAttribute( "atribute1" );
			UT_ASSERT_EQUAL( STRING("value3.1"), atribute1 );
			UT_ASSERT_EQUAL( STR_ASCII, atribute1.getCharSet() );

			STRING atribute2 = record->getAttribute( "atribute2" );
			UT_ASSERT_EQUAL( STRING("äÄ"), atribute2 );
			UT_ASSERT_EQUAL( STR_UTF8, atribute2.getCharSet() );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static XmlTest myXmlTest;

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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

