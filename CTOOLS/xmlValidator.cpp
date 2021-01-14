/*
		Project:		GAKLIB
		Module:			xmlValidator.cpp
		Description:	XML validation
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

#include <gak/array.h>

#include <gak/http.h>
#include <gak/xmlValidator.h>
#include <gak/xmlParser.h>
#include <gak/html.h>
#include <gak/ci_string.h>
#include <gak/t_string.h>
#include <gak/directory.h>
#include <gak/numericString.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

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

static Array<Validator*>	dummy;

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

Validator::Validator( const STRING &xsdFile )
{
	Document	*theSchemaDoc;

	this->schemaFile = xsdFile;
	this->schema = NULL;
	isMySchema = true;

	if( net::HTTPrequest::isValidProtocol( xsdFile ) )
	{
		net::HTTPrequest theRequest;
		theRequest.Get( xsdFile );
		STRING body = (const char *)theRequest.getBody();
		theRequest.getHttpStatusCode();
		theSchemaDoc = theRequest.getXmlDocument( true );
	}
	else
	{
		Parser	theParser( xsdFile );
		theSchemaDoc = theParser.readFile( false );
	}

	if( theSchemaDoc )
	{
		Element	*schema = theSchemaDoc->getElement( "schema", XMLNS_SCHEMA );
		if( schema )
		{
			this->schema = schema;
		}
	}
}

Validator::Validator( Element *schema, const STRING &xsdFile )
{
	isMySchema = false;
	this->schemaFile = xsdFile;
	this->schema = schema;
}

SchemaManager::~SchemaManager()
{
	clearValidators();
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

void Validator::getEnumerations(
	const Element *simpleType, ArrayOfStrings *enumerations
)
{
	assert( simpleType->getLocalName() == "simpleType" );

	enumerations->clear();
	Element *restriction = simpleType->getElement(
		"restriction", XMLNS_SCHEMA
	);
	if( restriction )
	{
		XmlArray	xmlEnumerations;
		restriction->getChildElements(
			&xmlEnumerations,
			"enumeration", XMLNS_SCHEMA
		);
		for( size_t i=0; i<xmlEnumerations.size(); i++ )
		{
			STRING	value = xmlEnumerations[i]->getAttribute( "value" );
			if( !value.isEmpty() )
			{
				*enumerations += value;
			}
		}
	}
}

void Validator::scanSchema( const STRING &xsdFile, Element *schema )
{
	STRING	curTag;
	STRING	schemaPrefix = schema->getPrefix4Namespace( XMLNS_SCHEMA );
	STRING	includeTag = schemaPrefix;
	if( includeTag[0U] )
		includeTag += ':';
	STRING	importTag = includeTag;
	includeTag += "include";
	importTag += "import";

	for( size_t i=0; i<schema->getNumObjects(); i++ )
	{
		Element	*theElement = schema->getElement( i );

		curTag = theElement->getTag();
		if( curTag == includeTag || curTag == importTag )
		{
			Document	*theSchemaDoc;

			STRING	schemaLocation = theElement->getAttribute(
				XMLNS_SCHEMALOCATION
			);
			if( net::HTTPrequest::isValidProtocol( xsdFile )
			|| net::HTTPrequest::isValidProtocol( schemaLocation ) )
			{
				net::HTTPrequest theRequest;
				schemaLocation = theRequest.makeFullPath( xsdFile, schemaLocation );
				theRequest.Get( schemaLocation );
				STRING body = (const char *)theRequest.getBody();
				theRequest.getHttpStatusCode();
				theSchemaDoc = theRequest.getXmlDocument( true );
			}
			else
			{
				schemaLocation = makeFullPath( xsdFile, schemaLocation );
				Parser	theParser( schemaLocation );

				theSchemaDoc = theParser.readFile( false );
			}
			if( theSchemaDoc )
			{
				Element	*subSchema = theSchemaDoc->getElement( "schema", XMLNS_SCHEMA );
				if( subSchema )
				{
					STRING	tns = subSchema->getAttribute( "targetNamespace" );
					if( !tns.isEmpty() )
						schema->setStringAttribute( "targetNamespace", tns );

					while( subSchema->getNumObjects() )
					{
						Element	*newElement = subSchema->removeObject( (size_t)0U );
						if( newElement )
						{
							newElement->changePrefixInTree( schemaPrefix );
							schema->addObject( newElement );
						}
						else
							break;
					}
				}

				delete theSchemaDoc;
			}
			schema->removeObject( theElement );
			i--;
		}
	}
}

Element *Validator::findComplexType( Element *theSchema, Element *theSchemaElement )
{
	Element *theComplexType = theSchemaElement->getElement(
		"complexType", XMLNS_SCHEMA
	);
	if( !theComplexType )
	{
		XmlArray	theComplexTypes;

		theSchema->getChildElements( &theComplexTypes, "complexType", XMLNS_SCHEMA );
		STRING type = Element::getLocalName(
			theSchemaElement->getAttribute( "type" )
		);
		for( size_t i=0; i<theComplexTypes.size(); i++ )
		{
			theComplexType = theComplexTypes[i];

			if( theComplexType->getAttribute( "name" ) == type )
				break;
			theComplexType = NULL;
		}
	}

	return theComplexType;
}

Element *Validator::findSimpleType( Element *theSchema, const STRING &xmlTypeName )
{
	XmlArray	theSimpleTypes;
	Element *theSimpleType = NULL;

	theSchema->getChildElements( &theSimpleTypes, "simpleType", XMLNS_SCHEMA );

	for( size_t i=0; i<theSimpleTypes.size(); i++ )
	{
		theSimpleType = theSimpleTypes[i];

		if( theSimpleType->getAttribute( "name" ) == xmlTypeName )
			break;
		theSimpleType = NULL;
	}

	return theSimpleType;
}

Element *Validator::findSimpleType( Element *theSchema, Element *theSchemaElement )
{
	Element *theSimpleType = theSchemaElement->getElement(
		"simpleType", XMLNS_SCHEMA
	);

	if( !theSimpleType )
	{
		STRING xmlTypeName = Element::getLocalName(
			theSchemaElement->getAttribute( "type" )
		);
		theSimpleType = findSimpleType( theSchema, xmlTypeName );
	}

	return theSimpleType;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

Validator *SchemaManager::findValidatorByFile( const STRING &schemaFile )
{
	Validator *tmp, *theValidator = NULL;

	for( size_t i=0; !theValidator && i<validators.size(); i++ )
	{
		tmp = validators[i];
		if( tmp->getSchemaFile() == schemaFile )
			theValidator = tmp;
	}

	if( !theValidator )
	{
		Document *theSchemaDoc = loadSchemaFile(
			schemaFile
		);

		if( theSchemaDoc )
		{
			Element	*schema = theSchemaDoc->getElement(
				"schema", XMLNS_SCHEMA
			);
			if( schema )
			{
				theValidator = new Validator(
					schema, schemaFile
				);
				validators += theValidator;
			}
		}
	}
	return theValidator;
}

Element *Validator::getGroupModel( Element *theGroup )
{
	Element	*theModel = NULL;

	STRING	ref=theGroup->getAttribute( "ref" );
	if( !ref.isEmpty() )
		theGroup = findGroupDef( ref );
	if( theGroup )
		theModel = getTypeModel( theGroup );

	return theModel;
}

Element *Validator::getTypeModel( Element *theComplexType )
{
	Element	*theModel = theComplexType->getElement( "sequence", XMLNS_SCHEMA );
	if( !theModel )
		theModel = theComplexType->getElement( "choice", XMLNS_SCHEMA );
	if( !theModel )
		theModel = theComplexType->getElement( "all", XMLNS_SCHEMA );

	if( !theModel )
	{
		Element *theGroup = theComplexType->getElement( "group", XMLNS_SCHEMA );
		if( theGroup )
			theModel = getGroupModel( theGroup );
	}

	return theModel;
}

Element *Validator::findGroupDef( const STRING &ref )
{
	if( !elementGroups.size() )
		schema->getChildElements( &elementGroups, "group", XMLNS_SCHEMA );

	for( size_t i=0; i<elementGroups.size(); i++ )
	{
		if( elementGroups[i]->getAttribute( "name" ) == ref )
			return elementGroups[i];
	}

	return NULL;
}

Element *Validator::findAttributeDef( const STRING &ref )
{
	if( !attributeGroups.size() )
		schema->getChildElements( &attributeGroups, "attributeGroup", XMLNS_SCHEMA );

	for( size_t i=0; i<attributeGroups.size(); i++ )
	{
		if( attributeGroups[i]->getAttribute( "name" ) == ref )
			return attributeGroups[i];
	}

	return NULL;
}

void Validator::setupComplexTypes4Model( Element *documentRoot, Element *theModel )
{
	doEnterFunction("Validator::setupComplexTypes4Model( Element *documentRoot, Element *theModel )");
	Element	*schemaElement, *groupModel;
	STRING		tag;
	XmlArray	schemaElements;
	STRING		targetNamespace = getTargetNamespace();

	// search for all elements in our model
	theModel->getChildElements( &schemaElements, "element", XMLNS_SCHEMA );
	for( size_t i = 0; i<schemaElements.size(); i++ )
	{
		schemaElement = schemaElements[i];
		tag = schemaElement->getAttribute( "name" );
		if( !tag.isEmpty() )
		{
			XmlArray	children;
			documentRoot->getPrefixChildElements(
				&children, documentRoot->getPrefix()
			);
			for( size_t j=0; j<children.size(); j++ )
			{
				Element *data = children[j];

				if( data
				&& data->testNamespaceAndTag( targetNamespace, tag ) )
				{
					setupTypes( data, schemaElement );
				}
			}
		}
	}

	// search for all "sequence in our model
	theModel->getChildElements( &schemaElements, "sequence", XMLNS_SCHEMA );
	for( size_t i = 0; i<schemaElements.size(); i++ )
	{
		setupComplexTypes4Model( documentRoot, schemaElements[i] );
	}

	// search for all "choice" in our model
	theModel->getChildElements( &schemaElements, "choice", XMLNS_SCHEMA );
	for( size_t i = 0; i<schemaElements.size(); i++ )
	{
		setupComplexTypes4Model( documentRoot, schemaElements[i] );
	}

	// search for all "all" in our model
	theModel->getChildElements( &schemaElements, "all", XMLNS_SCHEMA );
	for( size_t i = 0; i<schemaElements.size(); i++ )
	{
		setupComplexTypes4Model( documentRoot, schemaElements[i] );
	}

	// search for all "group" in our model
	theModel->getChildElements( &schemaElements, "group", XMLNS_SCHEMA );
	for( size_t i = 0; i<schemaElements.size(); i++ )
	{
		groupModel = getGroupModel( schemaElements[i] );
		if( groupModel )
			setupComplexTypes4Model( documentRoot, groupModel );
	}
}

bool Validator::setupTypes( Element *documentRoot, Element *schemaElement )
{
	doEnterFunction("Validator::setupTypes( Element *documentRoot, Element *schemaElement )");

	bool		success = false;
	STRING		xmlTypeName = Element::getLocalName(
		schemaElement->getAttribute( "type" )
	);

	documentRoot->setType( xmlTypeName );
	Element *theComplexType = findComplexType( schema, schemaElement );
	if( theComplexType )
	{
		if( documentRoot->getNamespace() == getTargetNamespace() )
		{
			documentRoot->setComplexType( this, theComplexType );
			success = true;
		}
		Element *theModel = getTypeModel( theComplexType );
		if( theModel )
			setupComplexTypes4Model( documentRoot, theModel );
	}
	else
	{
		Element *theSimpleType = findSimpleType( schema, schemaElement );
		if( theSimpleType
		&&	documentRoot->getNamespace() == getTargetNamespace() )
		{
			documentRoot->setSimpleType( this, theSimpleType );
			success = true;
		}
	}
	return success;
}

void Validator::setupTypes( Element *documentRoot )
{
	doEnterFunction("Validator::setupTypes( Element *documentRoot )");

	STRING		elementName;
	XmlArray	theElements;

	scanSchema( schemaFile, schema );

	STRING	targetNamespace = getTargetNamespace();

	schema->getAllElements( &theElements, "element", XMLNS_SCHEMA );
	for( size_t i=0; i<theElements.size(); i++ )
	{
		Element	*schemaElement = theElements[i];

		if(
			documentRoot->testNamespaceAndTag(
				targetNamespace, schemaElement->getAttribute( "name" )
			)
		)
		{
			if( setupTypes( documentRoot, schemaElement ) )
/*v*/			break;
		}
	}
}

void Validator::getValidSubobjects4Model(
	Element *theModel,
	ArrayOfStrings *tags, XmlArray *schemaElements
)
{
	XmlArray	groupDefs;
	Element	*theElement;
	size_t		numElements;
	STRING		tag, name;

	STRING		prefix = theModel->getPrefix4Namespace( XMLNS_SCHEMA );
	STRING		elementTag = prefix;
	if( !elementTag.isEmpty() )
		elementTag += ':';
	STRING		groupTag = elementTag;
	STRING		choiceTag = elementTag;
	STRING		sequenceTag = elementTag;
	STRING		allTag = elementTag;

	elementTag += "element";

	groupTag += "group";
	choiceTag += "choice";
	sequenceTag += "sequence";
	allTag += "all";

	schema->getChildElements( &groupDefs, "group", XMLNS_SCHEMA );

	numElements = theModel->getNumObjects();
	for( size_t i=0; i<numElements; i++ )
	{
		theElement = theModel->getElement(i);
		tag = theElement->getTag();
		if( tag == elementTag )
		{
			name=theElement->getAttribute("name");
			if( !name.isEmpty() )
			{
				*tags += name;
				if( schemaElements )
				{
					*schemaElements += theElement;
				}
			}
		}
		else if( tag == choiceTag || tag == sequenceTag || tag == allTag )
		{
			getValidSubobjects4Model( theElement, tags, schemaElements );
		}
		else if( tag == groupTag )
		{
			Element	*theModel = getGroupModel( theElement );
			getValidSubobjects4Model( theModel, tags, schemaElements );
		}
	}
}

bool Validator::testGroup(
	Element *docElement, XmlArray &children,
	size_t begIndex, Element *group, bool mixed,
	size_t	*endIndex
)
{
	doEnterFunction("Validator::testGroup");

	STRING	ref = group->getAttribute( "ref" );
	if( !ref.isEmpty() )
	{
		Element *group = findGroupDef( ref );
		if( !group )
		{
			errorElement = docElement;
			errorText = ref + ": Group Reference not solved.";
/*@*/		return false;
		}
	}

	Element	*all = group->getElement( "all", XMLNS_SCHEMA );
	if( all )
	{
		bool success = testAll(
			docElement, children, begIndex, all, mixed, endIndex
		);
/***/	return success;
	}

	Element	*choice = group->getElement( "choice", XMLNS_SCHEMA );
	if( choice )
	{
		bool success = testChoice(
			docElement, children, begIndex, all, mixed, endIndex
		);
/***/	return success;
	}

	Element	*sequence = group->getElement( "sequence", XMLNS_SCHEMA );
	if( sequence )
	{
		bool success = testSequence(
			docElement, children, begIndex, all, mixed, endIndex
		);
/***/	return success;
	}

	errorElement = docElement;
	errorText = group->getAttribute( "name" ) + ": Group empty.";

	return false;
}

bool Validator::testSequence(
	Element *docElement, XmlArray &children,
	size_t begIndex, Element *sequence, bool mixed,
	size_t	*endIndex
)
{
	doEnterFunction( "Validator::testSequence" );

	Element		*child;
	STRING			modelElementTag, allowedTag, childTag, value, tmp;
	size_t			i, count, min, max,
					curIndex = begIndex,
					numChildren=children.size();
	STRING			targetNamespace = getTargetNamespace();

	for( i=0; i<sequence->getNumObjects(); i++ )
	{
		Element *theSchemaElement = sequence->getElement(i);
		modelElementTag = Element::getLocalName(
			theSchemaElement->getTag()
		);

		tmp = theSchemaElement->getAttribute( "minOccurs" );
		if( tmp.isEmpty() )
			min = 1;
		else
			min = tmp.getValueE<size_t>();
		tmp = theSchemaElement->getAttribute( "maxOccurs" );
		if( tmp.isEmpty() || tmp=="unbounded" )
			max = std::numeric_limits<size_t>::max();
		else
			max = tmp.getValueE<size_t>();
		count = 0;

		if( modelElementTag == "element" )
		{
			allowedTag = theSchemaElement->getAttribute( "name" );
			for( ; curIndex<numChildren; curIndex++ )
			{
				child = children[curIndex];
				if( dynamic_cast<Comment*>( child ) )
/*^*/				continue;

				childTag = child->getTag();
				if( childTag.isEmpty() )
				{
					value = child->getValue( PLAIN_MODE ).stripBlanks();
					if( value.isEmpty() || mixed )
/*^*/					continue;	// ignore blank PCDATA, too
				}
				if( child->testNamespaceAndTag( targetNamespace, allowedTag ) )
					count++;
				else
/*v*/				break;
			}
		}
		else if( modelElementTag == "group" )
		{
			while( curIndex<numChildren
			&& testGroup(
				docElement, children,
				curIndex, theSchemaElement, mixed, &curIndex
			) )
				count++;
		}
		else if( modelElementTag == "choice" )
		{
			while( curIndex<numChildren
			&& testChoice(
				docElement, children,
				curIndex, theSchemaElement, mixed, &curIndex
			) )
				count++;
		}
		else if( modelElementTag == "sequence" )
		{
			while( curIndex<numChildren
			&& testSequence(
				docElement, children,
				curIndex, theSchemaElement, mixed, &curIndex
			) )
			{
				count++;
			}
		}

		if( count < min || count > max )
		{
			if( curIndex < numChildren )
			{
				errorText = "Illegal child in sequence";
				errorElement = children[curIndex];
			}
			else
			{
				if( count < min )
					errorText = "Missing child (";
				else
					errorText = "Too many children (";

				errorText += formatNumber( i );
				errorText += '/';
				errorText += formatNumber( count );
				errorText += '/';
				errorText += formatNumber( min );
				errorText += '/';
				errorText += formatNumber( max );
				errorText += ") in sequence ";

				errorText += sequence->getPath();
				Document *xmlDoc = dynamic_cast<Document *>(
					sequence->findRoot()
				);
				if( xmlDoc )
				{
					STRING	fileName = xmlDoc->getFilename();
					if( !fileName.isEmpty() )
					{
						errorText += " in file ";
						errorText += fileName;
					}
				}

				errorElement = docElement;
			}
/*@*/		return false;
		}
	}

	*endIndex = curIndex;

	return true;
}

bool Validator::testChoice(
	Element *docElement, XmlArray &children,
	size_t begIndex, Element *choice, bool mixed,
	size_t	*endIndex
)
{
	doEnterFunction("Validator::testChoice");

	bool			found = false;
	Element		*child = NULL;
	STRING			modelElementTag, allowedTag, childTag, value, tmp;
	size_t			i,
					curIndex = begIndex,
					numChildren = children.size();
	STRING			targetNamespace = getTargetNamespace();

	while( curIndex < numChildren )
	{
		child = children[curIndex++];
		if( dynamic_cast<Comment*>( child ) )
		{
			child = NULL;
/*^*/		continue;
		}

		childTag = child->getTag();
		if( childTag.isEmpty() )
		{
			value = child->getValue( PLAIN_MODE ).stripBlanks();
			if( value.isEmpty() || mixed )
			{
				child = NULL;
/*^*/			continue;	// ignore blank PCDATA, too
			}
		}

/*v*/	break;
	}

	if( child )
	{
		for( i=0; !found && i<choice->getNumObjects(); i++ )
		{
			Element *theSchemaElement = choice->getElement(i);
			modelElementTag = Element::getLocalName(
				theSchemaElement->getTag()
			);
			if( modelElementTag == "element" )
			{
				allowedTag = theSchemaElement->getAttribute( "name" );

				if( child->testNamespaceAndTag( targetNamespace, allowedTag ) )
					found=true;
			}
		}
	}

	if( found )
	{
		*endIndex = curIndex;
	}
	else
	{
		// maybe we find a valid group, sequence or choice
		curIndex--;		// try again this child

		for( i=0; !found && i<choice->getNumObjects(); i++ )
		{
			Element *theSchemaElement = choice->getElement(i);
			modelElementTag = Element::getLocalName(
				theSchemaElement->getTag()
			);
			if( modelElementTag == "group" )
			{
				found = testGroup(
					docElement, children,
					curIndex, theSchemaElement, mixed, endIndex
				);
			}
			else if( modelElementTag == "sequence" )
			{
				found = testSequence(
					docElement, children,
					curIndex, theSchemaElement, mixed, endIndex
				);
			}
			else if( modelElementTag == "choice" )
			{
				found = testChoice(
					docElement, children,
					curIndex, theSchemaElement, mixed, endIndex
				);
			}
		}
		if( !found )
		{
			if( child )
			{
				errorElement = child;
				errorText = "Invalid child in choice";
			}
			else
			{
				errorElement = docElement;
				errorText = "Invalid element in choice";
			}
		}
	}
	return found;
}

bool Validator::testAll(
	Element *docElement, XmlArray &children,
	size_t begIndex, Element *all, bool mixed,
	size_t	*endIndex
)
{
	size_t				curIndex,
						numChildren=children.size();
	ArrayOfStrings	tags;
	Element			*child;
	STRING				tag, value;

	STRING				targetNamespace = getTargetNamespace();

	getValidSubobjects4Model( all, &tags );

	for( curIndex=begIndex; curIndex<numChildren; curIndex++ )
	{
		child = children[curIndex];
		if( dynamic_cast<Comment*>( child ) )
/*^*/		continue;

		tag = child->getLocalName();
		if( tag.isEmpty() )
		{
			value = child->getValue( PLAIN_MODE ).stripBlanks();
			if( !value.isEmpty() && !mixed )
/*v*/			break;	// ignore blank PCDATA, too
		}
		else if( child->getNamespace() == targetNamespace )
		{
			if( tags.findElement( tag ) == tags.no_index )
/*v*/			break;
		}
	}

	*endIndex = curIndex;

	return true;
}

STRING SchemaManager::scanElement( Element *theChild, const STRING &fileName )
{
	STRING			schemaFile;

	Element		*root;
	Validator	*theValidator = theChild->getValidator();

	if( !theValidator )
	{
		STRING schemaLocation = theChild->getSchemaLocation(&root);
		if( !schemaLocation.isEmpty() )
		{
			schemaFile = makeFullPath( fileName, schemaLocation );
		}
		else
		{
			schemaFile = getSchemaFile4Namespace(theChild->getNamespace());
			root = theChild;
		}
		if( !schemaFile.isEmpty() )
		{
			theValidator = findValidatorByFile( schemaFile );

			if( theValidator )
				theValidator->setupTypes( root );
		}
	}

	STRING tmp = scanElements( theChild, fileName );
	if( schemaFile.isEmpty() )
		schemaFile = tmp;

	return schemaFile;
}

STRING SchemaManager::scanElements( Element *theElement, const STRING &fileName )
{
	doEnterFunction( "SchemaManager::scanElements( Element *theElement, const STRING &fileName )" );

	STRING		schemaFile, tmp;
	Element	*theChild;

	for( size_t i=0; i<theElement->getNumObjects(); i++ )
	{
		theChild = theElement->getElement(i);
		tmp = scanElement( theChild, fileName );
		if( schemaFile.isEmpty() )
			schemaFile = tmp;
	}
	return schemaFile;
}

bool Validator::testSimpleType( const STRING &iValue, const STRING &type )
{
	doEnterFunction("Validator::testSimpleType");
	bool		result = true;
	char		c;
	size_t		len = iValue.strlen();

	if( type=="integer" )
	{
		const char	*value = iValue;

		if( value[0U] == '+' || value[0U] == '-' )
		{
			value++;
			len--;
		}
		for( size_t i=0; i<len; i++ )
		{
			c = value[i];
			if( c < '0' || c>'9' )
			{
				errorText = "Invalid integer";
				result = false;
/*v*/			break;
			}
		}
	}
	else if( type == "boolean" )
	{
		const STRING &value = iValue;
		if( value != "0" && value != "1"
		&& value != "true" && value != "false" )
		{
			errorText = "Invalid boolean";
			result = false;
		}
	}
	else if( type == "nonNegativeInteger" )
	{
		const char	*value = iValue;

		if( value[0U] == '+' )
		{
			value++;
			len--;
		}
		for( size_t i=0; i<len; i++ )
		{
			c = value[i];
			if( c < '0' || c>'9' )
			{
				errorText = "Invalid nonNegativeInteger";
				result = false;
/*v*/			break;
			}
		}
	}
	else if( type != "string" )
	{
		const STRING &value = iValue;
 
 		Element *theSimpleType = findSimpleType( schema, type );
		if( theSimpleType )
		{
			ArrayOfStrings	values;

			getEnumerations( theSimpleType, &values );
			if( values.findElement( value ) == values.no_index )
			{
				errorText = "restricted value";
				result = false;
			}
		}

	}
	return result;
}

bool Validator::testXML( Element *docElement )
{
	doEnterFunction("Validator::testXML");

	Element	*child;
	XmlArray	children;
	STRING		tag, attrName, value, prefix;
	STRING		type = docElement->getType();

	docElement->getPrefixChildElements(
		&children, docElement->getPrefix()
	);

	size_t		numChildren = children.size();

	/*
		test the element itself
	*/

	// built in type
	if( type == "string" || type == "integer"  || type == "boolean"
	|| type == "nonNegativeInteger" )
	{
		if( docElement->getNumObjects() != 1 )
		{
			errorElement = docElement;
			errorText = "Invalid type";
/*@*/		return false;
		}
		if( !testSimpleType( value, type ) )
		{
			errorElement = docElement;
/*@*/		return false;
		}

		value=docElement->getValue(PLAIN_MODE);
	}
	else
	{
		// complex type
		Element *theComplexType = docElement->getComplexType();
		if( theComplexType )
		{
			CI_STRING			mixedStr = theComplexType->getAttribute("mixed");
			bool				mixed = ((mixedStr == "true") ? true : false);
			Element			*theModel = getTypeModel( theComplexType );
			if( theModel )
			{
				size_t		curIndex = 0;

				STRING		modelType = Element::getLocalName(
					theModel->getTag()
				);


				if( modelType == "sequence" )
				{
					if( !testSequence(
						docElement, children, 0, theModel, mixed, &curIndex
					) )
					{
/*@*/					return false;
					}
				}
				else if( modelType == "choice" )
				{
					if( !testChoice(
						docElement, children, 0, theModel, mixed, &curIndex
					) )
					{
/*@*/					return false;
					}
				}
				else if( modelType == "all" )
				{
					if( !testAll(
						docElement, children, 0, theModel, mixed, &curIndex
					) )
					{
/*@*/					return false;
					}
				}
				else if( modelType == "group" )
				{
					if( !testGroup(
						docElement, children, 0, theModel, mixed, &curIndex
					) )
					{
/*@*/					return false;
					}
				}

				while( curIndex<numChildren )
				{
					child = children[curIndex];
					tag = child->getTag();
					if( tag.isEmpty() )
					{
						value = child->getValue( PLAIN_MODE ).stripBlanks();
						if( value.isEmpty() )
						{
							curIndex++;
/*^*/						continue;		// ignore blank PCDATA
						}
					}

					if( !dynamic_cast<Comment*>(child)
					&& (!mixed || !tag.isEmpty()) )
					{
						errorElement = child;
						errorText = "Invalid child for model ";
						errorText += theModel->getPath();
						Document *xmlDoc = dynamic_cast<Document *>(
							theModel->findRoot()
						);
						if( xmlDoc )
						{
							STRING	fileName = xmlDoc->getFilename();
							if( !fileName.isEmpty() )
							{
								errorText += " in file ";
								errorText += fileName;
							}
						}
/*@*/					return false;
					}
					curIndex++;
				}
			}
		}

		/*
			test the attributes
		*/
		ArrayOfStrings	attributes;
		XmlArray			schemaAttributes;
		getValidAttributes( docElement, &attributes, &schemaAttributes );
		for( size_t i=0; i<docElement->getNumAttributes(); i++ )
		{
			attrName = docElement->getAttributeName( i );
			if( attrName == XMLNS_ATTRIBUTE )
/*^*/			continue;
			prefix = Element::getPrefix( attrName );
			if( prefix == XMLNS_ATTRIBUTE )
/*^*/			continue;

			if( !prefix.isEmpty() )
/*^*/			continue;

			size_t	pos = attributes.findElement( attrName );
			if( pos == attributes.no_index )
			{
				errorElement = docElement;
				errorText = "Invalid attribute ";
				errorText += attrName;

/*@*/			return false;
			}
			type = schemaAttributes[pos]->getAttribute( "type" );
			value = docElement->getAttribute( i );
			if( !testSimpleType( value, type ) )
			{
				errorElement = docElement;
				errorText += " in attribute "; 
				errorText += attrName;

/*@*/			return false;
			}
		}
	}
	return true;	// no problem found
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

Document *SchemaManager::loadSchemaFile( const STRING &schemaFile )
{
	Parser	theParser( schemaFile );

	return theParser.readFile( false );
}

STRING SchemaManager::getSchemaFile4Namespace( const STRING &nameSpace )
{
	STRING			schemaFile;
	Validator	*theValidator;

	if( !nameSpace.isEmpty() )
	{
		for( size_t i=0; schemaFile.isEmpty() && i<validators.size(); i++ )
		{
			theValidator = validators[i];
			if( theValidator->getTargetNamespace() == nameSpace )
				schemaFile = theValidator->getSchemaFile();
		}

		if( schemaFile.isEmpty() )
		{
			if( nameSpace == XMLNS_SCHEMA )
				schemaFile = makeFullPath( defaultPath, "xschema.xsd" );
			else if( nameSpace == XMLNS_XSLT )
				schemaFile = makeFullPath( defaultPath, "xslt.xsd" );
		}
	}

	return schemaFile;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

bool SchemaManager::testXML( Element *theElement )
{
	Element		*child;
	bool			result = true;

	Validator	*theValidator = theElement->getValidator();
	if( theValidator )
	{
		result = theValidator->testXML( theElement );
		if( !result )
		{
			errorElement = theValidator->getErrorElement();
			errorText = theValidator->getErrorText();
		}
	}

	for( size_t i=0; result && i<theElement->getNumObjects(); i++ )
	{
		child = theElement->getElement(i);
		if( dynamic_cast<html::HtmlBase*>( child ) )
			result = testHTML( child );
		else
			result = testXML( child );
	}

	return result;
}

bool SchemaManager::testHTML( Element *theElement )
{
	Element		*child;
	bool			result = true;

	for( size_t i=0; result && i<theElement->getNumAttributes(); i++ )
	{
		STRING	attrName = theElement->getAttributeName( i );
		STRING	attrValue = theElement->getAttribute( i );
		if( !theElement->isValidAttribute( attrName, attrValue ) )
		{
			result = false;
			errorText = "Illegal attribute ";
			errorText += attrName;
			errorElement = theElement;
		}
	}

	if( !result )
/*@*/	return false;

	for( size_t i=0; result && i<theElement->getNumObjects(); i++ )
	{
		child = theElement->getElement(i);
		if( dynamic_cast<html::HtmlBase*>( child ) )
		{
			if( !theElement->isValidSubobject( child ) )
			{
				result = false;
				errorText = "Illegal child";
				errorElement = child;
			}
			else
				result = testHTML( child );
		}
		else
			result = testXML( child );
	}

	return result;
}

STRING SchemaManager::testDocument( Document *theDocument )
{
	STRING	theError;
	bool	result;

	refreshDocument( theDocument, false );
	if( dynamic_cast<html::Document*>( theDocument ) )
		result = testHTML( theDocument );
	else
		result = testXML( theDocument );

	if( !result )
	{
		theError = errorText;
		if( errorElement )
		{
			theError += ' ';
			theError += errorElement->getPath();
		}
	}

	return theError;
}

Element *Validator::setValue(
	Element *theObject, const STRING &pathI, const STRING &value
)
{
	T_STRING	path = pathI;
	STRING		objName = path.getFirstToken( "/" );
	Element	*subObject;

	while( theObject && objName[0U] && objName[0U] != '@' )
	{
		subObject = theObject->getElement( objName );
		if( !subObject )
		{
			subObject = new Any( objName );
			if( theObject->isValidSubobject( subObject ) )
				theObject->addObject( subObject );
			else
			{
				delete subObject;
				subObject = NULL;
			}
		}
		theObject = subObject;
		objName = path.getNextToken();
	}
	if( theObject )
	{
		if( objName[0U] != '@' )
		{
			subObject = new PCData( value );

			if( theObject->isValidSubobject( subObject ) )
				theObject->addObject( subObject );
			else
			{
				delete subObject;
				subObject = NULL;
			}

			theObject = subObject;
		}
		else
		{
			objName += (size_t)1U;
			theObject->setStringAttribute( objName, value );
		}
	}

	return theObject;
}

bool Validator::isValidSubobject( Element *parent, Element *newObject )
{
	doEnterFunction("Validator::isValidSubobject()" );
	bool			isValid = false;
	XmlArray		theSchemaTypes;		// complexType or simpleType
	ArrayOfStrings	tags;
	XmlArray		theSchemaElements;

	if( dynamic_cast<Comment*>( newObject ) )
		isValid = true;
	else if( parent->getPrefix() != newObject->getPrefix() )
	{
		isValid = true;
		parent = parent->getPrefixParent( newObject->getPrefix() );
		if( parent )
		{
			Validator *nextValidator = parent->getValidator();
			if( nextValidator )
				isValid = nextValidator->isValidSubobject( parent, newObject );
		}
	}
	else if( parent )
	{
		Element	*theComplexType = parent->getComplexType();
		if( theComplexType )
		{
			CI_STRING	mixedStr = theComplexType->getAttribute("mixed");
			bool		mixed = ((mixedStr == "true") ? true : false);

			if( dynamic_cast<PCData*>(newObject)
			|| dynamic_cast<CData*>(newObject) )
			{
				isValid = mixed;
			}
			else
			{
				Element *theModel = getTypeModel( theComplexType );
				if( theModel )
				{
					getValidSubobjects4Model(
						theModel, &tags, &theSchemaElements
					);

					for( size_t i=0; i<theSchemaElements.size(); i++ )
					{
						Element	*theElement = theSchemaElements[i];
						STRING	testTag = parent->getPrefix();
						if( !testTag.isEmpty() )
							testTag += ':';
						testTag += theElement->getAttribute( "name" );
						if( testTag == newObject->getTag() )
						{
							STRING	type=theElement->getAttribute("type");
							if( !type.isEmpty() )
							{
								bool	typeFound = false;
								type = Element::getLocalName( type );
								newObject->setType( type );
								schema->getChildElements(
									&theSchemaTypes, "complexType", XMLNS_SCHEMA
								);
								for(
									size_t j=0;
									j<theSchemaTypes.size();
									j++
								)
								{
									Element	*theNewType = theSchemaTypes[j];
									STRING ntName = theNewType->getAttribute(
										"name"
									);
									if( ntName == type )
									{
										newObject->setComplexType(
											this, theNewType
										);
										typeFound = true;
										break;
									}

								}
								if( !typeFound )
								{
									schema->getChildElements(
										&theSchemaTypes, "simpleType", XMLNS_SCHEMA
									);
									for(
										size_t j=0;
										j<theSchemaTypes.size();
										j++
									)
									{
										Element	*theNewType = theSchemaTypes[j];
										STRING ntName = theNewType->getAttribute(
											"name"
										);
										if( ntName == type )
										{
											newObject->setSimpleType(
												this, theNewType
											);
											break;
										}

									}
								}
								isValid = true;
							}
						}
					}
				}
			}
		}
		else
		{
			STRING	newTag = newObject->getTag();
			if( newTag.isEmpty() )
				isValid = true;
		}
	}
	else
	{
		isValid = true;
	}
	return isValid;
}

bool Validator::isValidAttribute(
	const Element *docElement,
	const STRING &attrName, const STRING &attrValue
)
{
	if( attrName == XMLNS_ATTRIBUTE )
		return true;

	STRING	prefix = ((Element *)docElement)->getPrefix4Namespace(
		XMLNS_SCHEMA_INSTANCE
	);
	if( !prefix.isEmpty() )
		prefix += ':';

	if( attrName == prefix + XMLNS_SCHEMALOCATION
	||  attrName == prefix + XMLNS_NS_SCHEMALOCATION )
		return true;

	STRING	type = getAttributeType( docElement, attrName );
	if( !type.isEmpty() )
		return testSimpleType( type, attrValue );

	return false;
}

STRING Validator::getAttributeType(
	const Element *docElement, const STRING &attrName
)
{
	STRING			type;
	ArrayOfStrings	attributes;
	XmlArray		schemaAttributes;

	getValidAttributes( docElement, &attributes, &schemaAttributes );

	for( size_t i=0; i<attributes.size(); i++ )
	{
		if( attributes[i] == attrName )
		{
			type = schemaAttributes[i]->getAttribute( "type" );
/*v*/		break;
		}
	}

	return type;
}

void Validator::addValidAttributes(
	Element *schemaElement,
	ArrayOfStrings *attributes, XmlArray *schemaAttributes
)
{
	STRING 		ref, name;
	size_t		i, numChildren, numRefs;
	XmlArray	attributeElements;
	XmlArray	attributeRefElements;
	schemaElement->getChildElements(
		&attributeElements, "attribute", XMLNS_SCHEMA
	);

	numChildren = attributeElements.size();
	for( i=0; i<numChildren; i++ )
	{
		Element	*theAttribute = attributeElements[i];
		name = theAttribute->getAttribute("name");
		if( !name.isEmpty() )
		{
			*attributes += name;
			if( schemaAttributes )
			{
				*schemaAttributes += theAttribute;
			}
		}
	}

	schemaElement->getChildElements(
		&attributeRefElements, "attributeGroup", XMLNS_SCHEMA
	);
	numRefs = attributeRefElements.size();
	if( numRefs )
	{
		for( i=0; i<numRefs; i++ )
		{
			Element	*theAttrRef = attributeRefElements[i];
			ref = theAttrRef->getAttribute( "ref" );
			Element *theAttrGroup = findAttributeDef( ref );
			if( theAttrGroup )
			{
				addValidAttributes( theAttrGroup, attributes, schemaAttributes );
			}
		}
	}
}

void Validator::getValidAttributes(
	const Element *docElement,
	ArrayOfStrings *attributes, XmlArray *schemaAttributes
)
{
	attributes->clear();
	if( schemaAttributes )
		schemaAttributes->clear();

	Element	*theComplexType = docElement->getComplexType();
	if( theComplexType )
	{
		addValidAttributes( theComplexType, attributes, schemaAttributes );
		Element *simpleContent = theComplexType->getElement(
			"simpleContent", XMLNS_SCHEMA
		);
		if( simpleContent )
		{
			Element *extension = simpleContent->getElement( "extension", XMLNS_SCHEMA );
			if( extension )
			{
				addValidAttributes( extension, attributes, schemaAttributes );
			}
		}
	}
}

void Validator::getValidAttributeValues(
	const Element	*docElement,
	size_t				attrIdx,
	ArrayOfStrings		*values
)
{
	values->clear();
	STRING attrType = getAttributeType(
		docElement, docElement->getAttributeName( attrIdx )
	);
	if( !attrType.isEmpty() )
	{
		Element *theSimpleType = findSimpleType( schema, attrType );
		if( theSimpleType )
		{
			getEnumerations( theSimpleType, values );
		}
	}
}

void Validator::getValidSubobjects( ArrayOfStrings *tags )
{
	if( schema )
	{
		XmlArray	theElements;

		schema->getChildElements( &theElements, "element", XMLNS_SCHEMA );
		for( size_t i=0; i<theElements.size(); i++ )
		{
			*tags += theElements[i]->getAttribute( "name" );
		}
	}
}

void Validator::getAllValidSubobjects( ArrayOfStrings *tags )
{
	if( schema )
	{
		XmlArray	theElements;

		schema->getAllElements( &theElements, "element", XMLNS_SCHEMA );
		for( size_t i=0; i<theElements.size(); i++ )
		{
			*tags += theElements[i]->getAttribute( "name" );
		}
	}
}

void SchemaManager::clearValidators( void )
{
	if( validators.size() )
	{
		for( size_t i=0; i<validators.size(); i++ )
		{
			delete validators[i];
		}

		validators.clear();
	}
}

Validator *SchemaManager::getValidator4Namespace( const STRING &nameSpace )
{
	Validator *tmp, *theValidator = NULL;

	if( !nameSpace.isEmpty() )
	{
		for( size_t i=0; !theValidator && i<validators.size(); i++ )
		{
			tmp = validators[i];
			if( tmp->getTargetNamespace() == nameSpace )
			{
				theValidator = tmp;
			}
		}
	}

	return theValidator;
}


// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace xml
}	// namespace gak

