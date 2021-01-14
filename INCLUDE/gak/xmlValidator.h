/*
		Project:		GAKLIB
		Module:			xmlValidator.h
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

#ifndef GAK_XML_VALIDATOR_H
#define GAK_XML_VALIDATOR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //


#include <gak/xml.h>

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

#	pragma warn -inl
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

class Validator
{
	friend class SchemaManager;

	bool		isMySchema;
	Element	*schema;
	STRING		schemaFile;
	XmlArray	elementGroups, attributeGroups;

	Element	*errorElement;
	STRING		errorText;

	STRING getTargetNamespace( void )
	{
		STRING nameSpace;
		if( schema )
			nameSpace = schema->getAttribute( "targetNamespace" );

		return nameSpace;
	}
	Element *getGroupModel( Element *theGroup );
	Element *getTypeModel( Element *theComplexType );
	Element *getDocModel( Element *docElement )
	{
		Element *theModel = NULL;

		Element	*theComplexType = docElement->getComplexType();
		if( theComplexType )
			theModel = getTypeModel( theComplexType );

		return theModel;
	}

	void setupComplexTypes4Model( Element *root, Element *theModel );
	bool setupTypes( Element *root, Element *schemaElement );
	void addValidAttributes(
		Element *schemaElement,
		ArrayOfStrings *attributes, XmlArray *schemaAttributes = NULL
	);

	void getValidSubobjects4Model(
		Element *theModel,
		ArrayOfStrings *tags, XmlArray *schemaElements = NULL
	);
	bool testSequence(
		Element *docElement, XmlArray &children,
		size_t begIndex, Element *sequence, bool mixed,
		size_t	*endIndex
	);
	bool testChoice(
		Element *docElement, XmlArray &children,
		size_t begIndex, Element *choice, bool mixed,
		size_t	*endIndex
	);
	bool testAll(
		Element *docElement, XmlArray &children,
		size_t begIndex, Element *all, bool mixed,
		size_t	*endIndex
	);
	bool testGroup(
		Element *docElement, XmlArray &children,
		size_t begIndex, Element *all, bool mixed,
		size_t	*endIndex
	);

	Element *findGroupDef( const STRING &ref );
	Element *findAttributeDef( const STRING &ref );

	static void getEnumerations(
		const Element *simpleType, ArrayOfStrings *enumerations
	);
	bool testSimpleType( const STRING &value, const STRING &type );
	bool testXML( Element *docElement );

	public:
	Validator( Element *schema, const STRING &xsdFile );
	Validator( const STRING &xsdFile );
	~Validator()
	{
		if( isMySchema && schema )
		{
			Document	*schemaDoc = dynamic_cast<Document*>(
				schema->getParent()
			);
			if( schemaDoc )
				delete schemaDoc;
		}
	}

	static void scanSchema( const STRING &xsdFile, Element *theSchema );
	static Element *findComplexType(
		Element *theSchema, Element *theSchemaElement
	);
	static Element *findSimpleType(
		Element *theSchema, const STRING &xmlTypeName
	);
	static Element *findSimpleType(
		Element *theSchema, Element *theSchemaElement
	);

	void setupTypes( Element *root );

	Element *setValue(
		Element *theObject, const STRING &path, const STRING &value
	);
	bool isValidSubobject( Element *parent, Element *newObject );
	Element *getValidSubobjects(
		Element *docElement,
		ArrayOfStrings *tags
	)
	{
		Element *theModel = getDocModel( docElement );

		if( theModel )
			getValidSubobjects4Model( theModel, tags );

		return theModel;
	}

	bool		isValidAttribute(
		const Element *docElement,
		const STRING &attrName, const STRING &attrValue
	);
	STRING getAttributeType(
		const Element *docElement, const STRING &attrName
	);
	void 		getValidAttributes(
		const Element *docElement,
		ArrayOfStrings		*attributes,
		XmlArray			*schemaAttributes=NULL
	);
	void 		getValidAttributeValues(
		const Element	*docElement,
		size_t				attrIdx,
		ArrayOfStrings		*values
	);

	Element *getErrorElement( void ) const
	{
		return errorElement;
	}
	const STRING &getErrorText( void ) const
	{
		return errorText;
	}
	const STRING &getSchemaFile( void ) const
	{
		return schemaFile;
	}

	void getValidSubobjects( ArrayOfStrings *tags );
	void getAllValidSubobjects( ArrayOfStrings *tags );
};

class SchemaManager
{
	STRING					defaultPath;
	Element				*errorElement;
	STRING					errorText;

//	static
		Array<Validator*>	validators;


	Validator *findValidatorByFile( const STRING &fileName );

	protected:
	virtual STRING getSchemaFile4Namespace( const STRING &nameSpace );

	virtual Document *loadSchemaFile( const STRING &schemaFile );
	STRING scanElements( Element *theElement, const STRING &fileName );
	public:
	virtual ~SchemaManager();
	STRING scanElement( Element *theElement, const STRING &fileName );
	STRING scanDocument( Document *theDocument )
	{
		return scanElements( theDocument, theDocument->getFilename() );
	}
	bool testXML( Element *theElement );
	bool testHTML( Element *theElement );
	STRING testDocument( Document *theDocument );

	void clearValidators( void );
	void clearValidators( Document *theDocument, bool inclCache )
	{
		theDocument->clearTypes();
		if( inclCache )
			clearValidators();
	}
	STRING refreshDocument( Document *theDocument, bool inclCache )
	{
		clearValidators( theDocument, inclCache );
		return scanElements( theDocument, theDocument->getFilename() );
	}

	const STRING &getErrorText( void ) const
	{
		return errorText;
	}
	Element *getErrorElement( void ) const
	{
		return errorElement;
	}

	void setDefaultPath( const STRING &newPath )
	{
		defaultPath = newPath;
	}
	Validator *getValidator4Namespace( const STRING &nameSpace );
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

#	pragma warn +inl
#endif

#endif
