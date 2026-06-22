/*
		Project:		GAKLIB
		Module:			wsdlImporter.cpp
		Description:	WSDL import and C++ code generator
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdio.h>

#include <gak/xmlValidator.h>
#include <gak/wsdlImporter.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{
namespace net
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

static STRING matchType( const STRING &xsdType )
{
	if( xsdType == "string" )
		return "gak::STRING";
	else if( xsdType == "boolean" )
		return "bool";
	else if( xsdType == "integer" )
		return "int";
	else
		return xsdType;
}

static STRING repairName( const STRING &xsdName )
{
	if( tokenSearch(
		xsdName,
		"delete\0new\0return\0\0"
	) >= 0 )
		return xsdName + "_s";
	else
		return xsdName;
}

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

STRING WSDLimporter::searchSimpleType( xml::Element *theSchema, STRING type )
{
	type = xml::Element::getLocalName( type );
	type = matchType( type );
	if( type == "gak::STRING" || type == "int"  || type == "bool" )
		return type;

	for( size_t i=0; i<theSchema->getNumObjects(); i++ )
	{
		xml::Element	*theSimpleType = theSchema->getElement( i );
		if( theSimpleType->getLocalName() == "simpleType"
		&&	theSimpleType->getNamespace() == xml::XMLNS_SCHEMA
		&&  theSimpleType->getAttribute( "name" ) == type )
		{
			xml::Element	*theRestriction = theSimpleType->getElement( "restriction", xml::XMLNS_SCHEMA );
			if( theRestriction )
			{
				STRING baseType = theRestriction->getAttribute( "base" );
				if( baseType[0U] )
				{
					type = xml::Element::getLocalName( baseType );
					break;
				}
			}
		}
	}

	return matchType( type );
}

void WSDLimporter::writeParameters( std::ostream &fp, xml::Element *theSchema, xml::Element *theElement, STRING xmlPath, STRING cppPath )
{
	STRING	name = theElement->getAttribute( "name" );
	STRING	type = theElement->getAttribute( "type" );

	if( xmlPath[0U] )
		xmlPath += '/';
	xmlPath += name;

	if( cppPath[0U] )
		cppPath += '.';
	cppPath += name;

	// check for a simple type
	type = searchSimpleType( theSchema, type );
	if( type == "gak::STRING" || type == "int"  || type == "bool" )
	{
		if( xmlPath[0U] && cppPath[0U] )
		{
			fp << "setParameter( \""<<xmlPath<<"\", "<<cppPath<<" );\n";
		}
/***/	return;
	}

	// we have a complex type
	for( size_t i=0; i<theSchema->getNumObjects(); i++ )
	{
		xml::Element	*theComplexType = theSchema->getElement( i );
		if( theComplexType->getLocalName() == "complexType"
		&&	theComplexType->getNamespace() == xml::XMLNS_SCHEMA
		&&  theComplexType->getAttribute( "name" ) == type )
		{
			xml::Element	*theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
			if( !theSequence )
				theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
			if( theSequence )
			{
				for( size_t j=0; j<theSequence->getNumObjects(); j++ )
				{
					xml::Element *theElement = theSequence->getElement( j );
					if( theElement->getLocalName() == "element"
					&&	theElement->getNamespace() == xml::XMLNS_SCHEMA )
					{
						writeParameters( fp, theSchema, theElement, xmlPath, cppPath );
					}
				}
			}
			break;
		}
	}

}

STRING WSDLimporter::getTypeString( ArrayOfStrings &knownTypes, xml::Element *theSchema, const STRING &typeName )
{
	STRING	requiredDef;
	STRING	typeDef = "\ntypedef struct {\n";

	knownTypes += typeName;

	for( size_t i=0; i<theSchema->getNumObjects(); i++ )
	{
		xml::Element	*theComplexType = theSchema->getElement( i );
		if( theComplexType->getLocalName() == "complexType"
		&&	theComplexType->getNamespace() == xml::XMLNS_SCHEMA
		&&  theComplexType->getAttribute( "name" ) == typeName )
		{
			xml::Element *theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
			if( !theSequence )
				theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
			if( theSequence )
			{
				for( size_t j=0; j<theSequence->getNumObjects(); j++ )
				{
					xml::Element	*theMember = theSequence->getElement( j );
					if( theMember->getLocalName() == "element"
					&&	theMember->getNamespace() == xml::XMLNS_SCHEMA )
					{
						STRING name = theMember->getAttribute( "name" );
						STRING type = theMember->getAttribute( "type" );

						type = searchSimpleType( theSchema, type );

						typeDef += type;
						if( type != "gak::STRING"
						&&  type != "int"
						&&  type != "bool" )
							typeDef += "_t";

						typeDef += ' ';
						typeDef += repairName( name );
						typeDef += ";\n";

						if( type != "gak::STRING"
						&&  type != "int"
						&&  type != "bool"
						&&  knownTypes.findElement( type ) == knownTypes.no_index )
							requiredDef += getTypeString( knownTypes, theSchema, type );
					}
				}
			}

			break;
		}
	}
	typeDef += '}';
	typeDef += typeName;
	typeDef += "_t;\n";

	return requiredDef + typeDef;
}

void WSDLimporter::writeTypedefs( std::ostream &fp, xml::Element *theSchema )
{
	ArrayOfStrings	knownTypes;

	for( size_t i=0; i<theSchema->getNumObjects(); i++ )
	{
		xml::Element	*theComplexType = theSchema->getElement( i );
		if( theComplexType->getLocalName() == "complexType"
		&&	theComplexType->getNamespace() == xml::XMLNS_SCHEMA )
		{
			STRING	typeName = theComplexType->getAttribute( "name" );
			if( !typeName.isEmpty() )
			{
				if( knownTypes.findElement( typeName ) == knownTypes.no_index )
				{
					STRING typedefs = getTypeString( knownTypes, theSchema, typeName );
					fp << typedefs;

					fp <<	"void fill"<<typeName<<"( "<<typeName<<"_t &item, gak::xml::Element *xml );\n"
							"void fill"<<typeName<<"( gak::Array<"<<typeName<<"_t> &item, gak::xml::Element *xml );\n"
					;
				}
			}
		}
	}
}

void WSDLimporter::writePrototypes( std::ostream &fp, xml::Element *theDefinitions, xml::Element *theSchema )
{
	doEnterFunction("void WSDLimporter::writePrototypes( std::ostream &fp, xml::Element *theDefinitions, xml::Element *theSchema )");

	xml::Element	*thePortType = theDefinitions->getElement( "portType", xml::XMLNS_WSDL );
	if( thePortType )
	{
		for( size_t i=0; i<thePortType->getNumObjects(); i++ )
		{
			xml::Element *theOperation = thePortType->getElement(i);
			if( theOperation->getLocalName() == "operation"
			&&	theOperation->getNamespace() == xml::XMLNS_WSDL )
			{
				STRING	operationName = theOperation->getAttribute( "name" );
				STRING	returnType = "void";

				setOperation( operationName );
				if( theOutputElement )
				{
					xml::Element	*theComplexType = xml::Validator::findComplexType( theSchema, theOutputElement );
					if( theComplexType )
					{
						xml::Element *theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
						if( !theSequence )
							theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
						if( theSequence )
						{
							for( size_t j=0; j<theSequence->getNumObjects(); j++ )
							{
								xml::Element	*theParam = theSequence->getElement(j);
								if( theParam->getLocalName() == "element"
								&&	theParam->getNamespace() == xml::XMLNS_SCHEMA )
								{
									returnType = theParam->getAttribute( "type" );
									returnType = searchSimpleType( theSchema, returnType );
									if( returnType != "gak::STRING" && returnType != "int" && returnType != "bool" )
										returnType = "gak::xml::Element *";
									break;
								}
							}
						}
					}
				}

				fp << '\n' << returnType << ' ' << repairName( operationName ) << "( ";
				if( theInputElement )
				{
					xml::Element	*theComplexType = xml::Validator::findComplexType( theSchema, theInputElement );
					if( theComplexType )
					{
						xml::Element *theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
						if( !theSequence )
							theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
						if( theSequence )
						{
							bool first = true;

							for( size_t j=0; j<theSequence->getNumObjects(); j++ )
							{
								xml::Element	*theParam = theSequence->getElement(j);
								if( theParam->getLocalName() == "element"
								&&	theParam->getNamespace() == xml::XMLNS_SCHEMA )
								{
									STRING	name = theParam->getAttribute( "name" );
									STRING	type = theParam->getAttribute( "type" );

									type = searchSimpleType( theSchema, type );

									if( !first )
										fp << ", ";
									first = false;

									if( type != "int" && type != "bool" )
										fp << "const ";
									fp << type;
									if( type != "int" && type != "bool" && type != "gak::STRING" )
										fp << "_t ";

									if( type != "int" && type != "bool" )
										fp << "_& ";
									else
										fp << ' ';
									fp << name;
								}
							}
						}
					}
				}

				fp << ");\n"
					<< returnType << ' ' <<  repairName( operationName ) << "( gak::xml::Element *theParameter );\n";
			}
		}
	}
}

void WSDLimporter::writeMemberFunctions( std::ostream &fp, const STRING &className, xml::Element *theDefinitions, xml::Element *theSchema )
{
	doEnterFunction("void WSDLimporter::writeMemberFunctions( std::ostream &fp, const STRING &className, xml::Element *theDefinitions, xml::Element *theSchema )");

	STRING			returnType;
	xml::Element	*theSequence = NULL;

	for( size_t i=0; i<theSchema->getNumObjects(); i++ )
	{
		xml::Element	*theComplexType = theSchema->getElement( i );
		if( theComplexType->getLocalName() == "complexType"
		&&	theComplexType->getNamespace() == xml::XMLNS_SCHEMA )
		{
			STRING	typeName = theComplexType->getAttribute( "name" );
			if( !typeName.isEmpty() )
			{
				fp << "void "<<className<<"::fill"<<typeName<<"( "<<typeName<<"_t &item, gak::xml::Element *xml ) {\n";

				xml::Element *theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
				if( !theSequence )
					theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
				if( theSequence )
				{
					for( size_t j=0; j<theSequence->getNumObjects(); j++ )
					{
						xml::Element	*theMember = theSequence->getElement( j );
						if( theMember->getLocalName() == "element"
						&&	theMember->getNamespace() == xml::XMLNS_SCHEMA )
						{
							STRING name = theMember->getAttribute( "name" );
							STRING type = theMember->getAttribute( "type" );

							type = searchSimpleType( theSchema, type );

							if( type == "gak::STRING"
							||  type == "int"
							||  type == "bool" )
							{
								fp << "gak::xml::Element *"<<name<<" = xml->getElement( \""<<name<<"\" );\n";
								fp << "if(" << name << ')\n';
								if( type == "gak::STRING" )
								{
									fp << "item."<<name<<" = "<<name<<"->getValue( gak::xml::PLAIN_MODE );\n";
								}
								else
								{
									fp << "item."<<name<<" = atoi( "<<name<<"->getValue( gak::xml::PLAIN_MODE ) );\n";
								}
							}
						}
					}
				}

				fp << "}\n";
				fp << "void "<<className<<"::fill"<<typeName<<"( gak::Array<"<<typeName<<"_t> &itemArray, gak::xml::Element *xml ) {\n";

				fp << "for( size_t i=0; i<xml->getNumObjects(); i++ )\n";
				fp << "{\n";
				fp << "fill"<<typeName<<"( itemArray.createElement(), xml->getElement( i ) );\n";

				fp << "} }\n";
			}
		}
	}

	xml::Element	*thePortType = theDefinitions->getElement( "portType", xml::XMLNS_WSDL );
	if( thePortType )
	{
		for( size_t i=0; i<thePortType->getNumObjects(); i++ )
		{
			xml::Element *theOperation = thePortType->getElement(i);
			if( theOperation->getLocalName() == "operation"
			&&	theOperation->getNamespace() == xml::XMLNS_WSDL )
			{
				STRING	operationName = theOperation->getAttribute( "name" );
				if( !operationName[0U] )
					operationName = "unknown";

				returnType = "void";

				// scan the WSD for this function
				setOperation( operationName );

				// find the return type
				if( theOutputElement )
				{
					xml::Element	*theComplexType = xml::Validator::findComplexType( theSchema, theOutputElement );
					if( theComplexType )
					{
						theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
						if( !theSequence )
							theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
						if( theSequence )
						{
							for( size_t j=0; j<theSequence->getNumObjects(); j++ )
							{
								xml::Element	*theParam = theSequence->getElement(j);
								if( theParam->getLocalName() == "element"
								&&	theParam->getNamespace() == xml::XMLNS_SCHEMA )
								{
									returnType = theParam->getAttribute( "type" );
									returnType = searchSimpleType( theSchema, returnType );
									if( returnType != "gak::STRING" && returnType != "int" && returnType != "bool" )
										returnType = "gak::xml::Element *";
								}
							}
						}
					}
				}

				// write the definition
				fp << '\n'<<returnType<<' ' << className <<"::"<<repairName( operationName )<<"( ";
				if( theInputElement )
				{
					// add each parameter

					xml::Element	*theComplexType = xml::Validator::findComplexType( theSchema, theInputElement );
					if( theComplexType )
					{
						theSequence = theComplexType->getElement( "sequence", xml::XMLNS_SCHEMA );
						if( !theSequence )
							theSequence = theComplexType->getElement( "all", xml::XMLNS_SCHEMA );
						if( theSequence )
						{
							bool first = true;

							for( size_t j=0; j<theSequence->getNumObjects(); j++ )
							{
								xml::Element	*theParam = theSequence->getElement(j);
								if( theParam->getLocalName() == "element"
								&&	theParam->getNamespace() == xml::XMLNS_SCHEMA )
								{
									STRING	name = theParam->getAttribute( "name" );
									STRING	type = theParam->getAttribute( "type" );
									type = searchSimpleType( theSchema, type );

									if( !first )
										fp << ", ";
									first = false;

									if( type != "int" && type != "bool" )
										fp << "const ";
									fp << type;
									if( type != "int" && type != "bool" && type != "gak::STRING" )
										fp << "_t";
									if( type != "int" && type != "bool" )
										fp << " &";
									else
										fp << ' ';
									fp << name;
								}
							}
						}
					}
				}

				fp << ") {\n";
				// add the body
				fp << "setOperation( \""<<operationName<<"\" );\n";
				if( theInputElement && theSequence )
				{
					for( size_t j=0; j<theSequence->getNumObjects(); j++ )
					{
						xml::Element	*theParam = theSequence->getElement(j);
						if( theParam->getLocalName() == "element"
						&&	theParam->getNamespace() == xml::XMLNS_SCHEMA )
						{
							writeParameters( fp, theSchema, theParam );
						}
					}
				}

				if( returnType == "gak::STRING" || returnType == "int"  || returnType == "bool" )
				{
					fp <<
						"gak::STRING value;\n"
						"gak::xml::Element *response = execute();\n"
						"if( response ) {\n"
							"response = response->getElement(0);\n"
							"if( response )\n"
								"value = response->getValue( gak::xml::PLAIN_MODE );\n"
						"}\n";
					if( returnType == "gak::STRING" )
						fp << "return value;\n";
					else if( returnType == "int" )
						fp << "int iVal = value.getValueE<int>(); return iVal;\n";
					else
						fp << "bool bVal = value.getBoolValue(); return bVal;\n";
				}
				else if( returnType == "gak::xml::Element *" )
				{
					fp << "gak::xml::Element *response = execute();\n"
						"if( response )\n"
							"response = response->getElement(0);\n"
						"return response;\n";
				}
				else
				{
					fp << "execute();\n";
				}

				fp << "}\n";

				// Now create the XML Version
				fp << '\n'<<returnType<<' '<<className<<"::"<<repairName( operationName )<<"( gak::xml::Element *theParameter ) {\n";
				fp << "setOperation( \""<<operationName<<"\" );\n";
				if( returnType == "gak::STRING" || returnType == "int"  || returnType == "bool" )
				{
					fp <<
						"gak::STRING value;\n"
						"gak::xml::Element *response = execute( theParameter );\n"
						"if( response ) {\n"
							"response = response->getElement(0);\n"
							"if( response )\n"
								"value = response->getValue( gak::xml::PLAIN_MODE );\n"
						"}\n";
					if( returnType == "gak::STRING" )
						fp << "return value;\n";
					else if( returnType == "int" )
						fp << "int iVal = value.getValueE<int>(); return iVal;\n";
					else
						fp << "bool bVal = value.getBoolValue(); return bVal;\n";
				}
				else if( returnType == "gak::xml::Element *" )
				{
					fp <<
						"gak::xml::Element *response = execute( theParameter );\n"
						"if( response )\n"
							"response = response->getElement(0);\n"
						"return response;\n";
				}
				else
				{
					fp << "execute( theParameter );\n";
				}

				fp << "}\n";
			}
		}
	}
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

void WSDLimporter::writeCppClass( const STRING &directory )
{
	doEnterFunction("WSDLimporter::writeCppClass");

	if( needCredentials )
	{
/*@*/	throw NoAuthorisationError( theWSDURL );
	}
	else if( !theWsdDoc )
	{
/*@*/	throw NoWSDLerror( theWSDURL );
	}

	STRING			className, fileName;
	xml::Element	*theDefinitions, *theSchema;

	theDefinitions = theWsdDoc->getRoot( "definitions", xml::XMLNS_WSDL );

	xml::Element	*theService = theDefinitions->getElement( "service", xml::XMLNS_WSDL );
	if( theService )
		className = theService->getAttribute( "name" );

	if( className.isEmpty() )
		className = theDefinitions->getAttribute( "name" );

	if( className.isEmpty() )
		className = "theService";

	fileName = directory + className;
	fileName += ".h";

	{
		std::ofstream fp( fileName );
		if( fp )
		{
			STRING	wsdUrl = theWSDURL.cString();

			fp	<< "// " << fileName << "n"
					"// Generated from file " << theWSDURL << "\n"
					"//(c) 1988-2026 by Martin Gaeckler, Linz\n\n"
					"#ifndef SOAP_" << className << "_H\n"
					"#define SOAP_" << className << "_H\n"
					"#include <gak/soap.h>\n"
					"class "<<className<<" : public gak::net::SoapRequest {\n"
					"public:\n"
				<< className << "() : gak::net::SoapRequest(<<"<<wsdUrl<<") {}\n"
				<< className << "( const char *wsdlUrl ) : gak::net::SoapRequest(wsdlUrl) {}\n"
			;

			theSchema = getSchema(theDefinitions);
			if( theSchema )
			{
				writeTypedefs( fp, theSchema );
				writePrototypes( fp, theDefinitions, theSchema );
			}

			fp << "};\n"
				"#endif\n";
		}
		else
/*@*/		throw OpenWriteError( fileName );
	}

	fileName = directory + className;
	fileName += ".cpp";

	{
		std::ofstream fp( fileName );
		if( fp )
		{
			fp << "// "<<fileName<<"\n"
				"// Generated from file "<<theWSDURL<<"\n"
				"// (c) 1988-2026 by Martin Gäckler, Munich\n\n"
				"#include <gak/numericString.h>\n"
				"#include \""<<className<<".h\"\n"
			;

			if( theSchema )
				writeMemberFunctions( fp, className, theDefinitions, theSchema );
		}
		else
/*@*/		throw OpenWriteError( fileName );
	}

	fileName = directory + className;
	fileName += "_wsdl.xml";

	{
		std::ofstream fp( fileName );
		if( fp )
		{
			STRING	xmlDoc = theWsdDoc->generateDoc();

			if( !xmlDoc.isEmpty() )
				fp << xmlDoc;
		}
		else
/*@*/		throw OpenWriteError( fileName );
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace net
}	// namespace gak

