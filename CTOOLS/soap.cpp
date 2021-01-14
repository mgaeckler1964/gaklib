/*
		Project:		GAKLIB
		Module:			soap.cpp
		Description:	The interface to a soap server
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

#include <gak/soap.h>
#include <gak/xmlParser.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -x
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif


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

void SoapRequest::loadWSD( const STRING &theWSDURL, const STRING &userName, const STRING &password )
{
	doEnterFunction("SoapRequest::loadWSD");

	this->theWSDURL = theWSDURL;

	theValidator = NULL;
	theEnvelope = NULL;
	theResponseDoc = NULL;

	needCredentials = false;
	theWsdDoc = NULL;

	if( isValidProtocol( theWSDURL ) )
	{
		HTTPrequest::setCredentials( userName, password );
		if( Get( theWSDURL ) )
		{
			int httpStatus = getHttpStatusCode();
			if( httpStatus == 200 )
			{
				theWsdDoc = getXmlDocument( true );
				clearCache( false );
			}
			else if( !httpStatus )
/*@*/			throw NoConnectionError( theWSDURL );
			else if( httpStatus == 401 )
			{
				needCredentials = true;
/*@*/			throw NoAuthorisationError( theWSDURL );
			}
			else
/*@*/			throw HTTPstatusError( theWSDURL );
		}
		else
/*@*/		throw NoConnectionError( theWSDURL );
	}
	else
	{
		xml::Parser		theParser( theWSDURL );

		theWsdDoc = theParser.readFile( false );
	}

	if( theWsdDoc )
	{
		xml::Element	*theDefinitions = theWsdDoc->getRoot( "definitions", xml::XMLNS_WSDL );
		if( theDefinitions )
		{
			xml::Element	*theService = theDefinitions->getElement( "service", xml::XMLNS_WSDL );
			if( theService )
			{
				for( size_t i=0; i<theService->getNumObjects() && theServiceUrl.isEmpty(); i++ )
				{
					xml::Element *thePort = theService->getElement( i );
					if( thePort->getLocalName() == "port" && thePort->getNamespace() == xml::XMLNS_WSDL )
					{
						xml::Element *theSoapAdress = thePort->getElement( "address", xml::XMLNS_SOAP );
						if( theSoapAdress )
							theServiceUrl = theSoapAdress->getAttribute( "location" );
					}
				}
			}
		}
		else
/*@*/		throw NoWSDLdefinitionError( theWSDURL );
	}
	else
/*@*/	throw NoWSDLerror( theWSDURL );
}

xml::Element *SoapRequest::getSchema( xml::Element *theDefinitions )
{
	doEnterFunction("xml::Element *SoapRequest::getSchema( xml::Element *theDefinitions )");

	xml::Element	*theTypes = theDefinitions->getElement( "types", xml::XMLNS_WSDL );
	if( theTypes )
	{
		xml::Element	*theSchema = theTypes->getElement( "schema", xml::XMLNS_SCHEMA );
		if( theSchema )
		{
			xml::Validator::scanSchema( theWSDURL, theSchema );
			targetNamespace = theSchema->getAttribute( "targetNamespace" );

			return theSchema;
		}
	}

	return NULL;
}

xml::Element *SoapRequest::getSchemaElement( xml::Element *theDefinitions, const STRING &message, xml::Element **theSchemaOut )
{
	doEnterFunction("xml::Element *SoapRequest::getSchemaElement( xml::Element *theDefinitions, const STRING &message, xml::Element **theSchemaOut )");

	for( size_t i=0; i<theDefinitions->getNumObjects(); i++ )
	{
		xml::Element	*theMessage = theDefinitions->getElement( i );
		if( theMessage->getLocalName() == "message"
		&&	theMessage->getNamespace() == xml::XMLNS_WSDL
		&&  theMessage->getAttribute( "name" ) == message )
		{
			xml::Element *thePart = theMessage->getElement( "part", xml::XMLNS_WSDL );
			if( thePart )
			{
				STRING element = thePart->getAttribute( "element" );
				element = xml::Element::getLocalName( element );

				xml::Element	*theSchema = getSchema( theDefinitions );
				if( theSchema )
				{
					for( size_t j=0; j<theSchema->getNumObjects(); j++ )
					{
						xml::Element *theElement = theSchema->getElement( j );
						if( theElement->getLocalName() == "element"
						&&	theElement->getNamespace() == xml::XMLNS_SCHEMA
						&&  theElement->getAttribute( "name" ) == element )
						{
							if( theSchemaOut )
								*theSchemaOut = theSchema;

							return theElement;
						}
					}
				}
			}
		}
	}

	return NULL;
}

xml::Element *SoapRequest::setOperation( const char *operation )
{
	doEnterFunction("xml::Element *SoapRequest::setOperation( const char *operation )");

	if( needCredentials )
	{
/*@*/	throw NoAuthorisationError( theWSDURL );
	}

	if( lastOperation == operation )
	{
		// we do not need to scan the wsdl twice
		while( theParameter->getNumObjects() )
		{
			xml::Element	*param = theParameter->getElement( 0 );
			if( param )
				delete param;
		}

/***/	return theEnvelope;
	}

	if( theEnvelope )
	{
		delete theEnvelope;
		theParameter = theBody = theEnvelope = NULL;
	}
	if( theValidator )
	{
		delete theValidator;
		theValidator = NULL;
	}
	SOAPAction = "";
	theInputElement = NULL;
	theOutputElement = NULL;


	xml::Element	*theDefinitions = theWsdDoc->getRoot( "definitions", xml::XMLNS_WSDL );

	xml::Element	*theBinding = theDefinitions->getElement( "binding", xml::XMLNS_WSDL );
	if( theBinding )
	{
		for( size_t i=0; i<theBinding->getNumObjects(); i++ )
		{
			xml::Element *theOperation = theBinding->getElement( i );
			if( theOperation->getLocalName() == "operation"
			&&	theOperation->getNamespace() == xml::XMLNS_WSDL
			&&	theOperation->getAttribute( "name" ) == operation )
			{
				theOperation = theOperation->getElement( "operation", xml::XMLNS_SOAP );
				if( theOperation )
				{
					SOAPAction = '"';
					SOAPAction += theOperation->getAttribute( "soapAction" );
					SOAPAction += '"';
					break;
				}
			}
		}
	}
	xml::Element	*thePortType = theDefinitions->getElement( "portType", xml::XMLNS_WSDL );
	if( thePortType )
	{
		for( size_t i=0; i<thePortType->getNumObjects(); i++ )
		{
			xml::Element	*theOperation = thePortType->getElement( i );
			if( theOperation->getLocalName() == "operation"
			&&	theOperation->getNamespace() == xml::XMLNS_WSDL )
			{
				STRING	operationName = theOperation->getAttribute( "name" );
				if( operationName == operation )
				{
					xml::Element *theOutput = theOperation->getElement( "output", xml::XMLNS_WSDL );
					if( theOutput )
					{
						STRING message = theOutput->getAttribute( "message" );
						message = xml::Element::getLocalName( message );
						theOutputElement = getSchemaElement( theDefinitions, message );
					}
					xml::Element *theInput = theOperation->getElement( "input", xml::XMLNS_WSDL );
					if( theInput )
					{
						xml::Element	*theSchema;

						STRING message = theInput->getAttribute( "message" );
						message = xml::Element::getLocalName( message );
						theInputElement = getSchemaElement( theDefinitions, message, &theSchema );
						if( theInputElement )
						{
							// yes we got it
							theEnvelope = new xml::Any( "soap:Envelope" );
							theEnvelope->setStringAttribute( "xmlns:soap", xml::XMLNS_SOAP_ENVELOPE );
							theBody = theEnvelope->addObject( new xml::Any( "soap:Body" ) );
							theParameter = theBody->addObject( new xml::Any( operation ) );
							if( theParameter )
							{
								theValidator = new xml::Validator( theSchema, theWSDURL );
								theValidator->setupTypes( theParameter );
								if( !targetNamespace.isEmpty() )
									theParameter->setStringAttribute( "xmlns", targetNamespace );
							}
						}
					}
					break;
				}
			}
		}
	}


	if( !theEnvelope )
/*@*/	throw BadOperationError( theWSDURL );

	return theEnvelope;
}

xml::Element *SoapRequest::setParameter( const char *parameterName, const char *value )
{
	doEnterFunction("xml::Element *SoapRequest::setParameter( const char *parameterName, const char *value )");

	xml::Element	*newParameter = NULL;

	if( theValidator )
		newParameter = theValidator->setValue( theParameter, parameterName, value );
	else
	{
/*@*/	throw BadOperationError( theWSDURL );
	}

	return newParameter;
}

xml::Element *SoapRequest::execute( xml::Element *newParameter )
{
	doEnterFunction("xml::Element *SoapRequest::execute( xml::Element *newParameter )");

	xml::Element *theReturn = NULL;

	if( theResponseDoc )
	{
		delete theResponseDoc;
		theResponseDoc = NULL;
	}
	if( theEnvelope && !theServiceUrl.isEmpty() )
	{
		// if there is not our own parameters used, set them instead of ours
		if( newParameter != theParameter )
		{
			theBody->addObject( newParameter );
			theBody->removeObject( theParameter );
		}
 /* TODO -csoap : FIX */
//		if( theValidator->testDocument() )
		{
			STRING	request = theEnvelope->generateDoc();

			if( SOAPAction[0U] )
				setExtraHeader( "SOAPAction", SOAPAction );
			size_t numData = Post(
				theServiceUrl, "text/xml", request, request.strlen()
			);
			if( numData && theOutputElement )
			{
				theResponseDoc = getXmlDocument( true );
				if( theResponseDoc )
				{
					xml::Element	*theReturnEnvelope =
						theResponseDoc->getElement(
							"Envelope", xml::XMLNS_SOAP_ENVELOPE
						)
					;
					if( theReturnEnvelope )
					{
						xml::Element *theReturnBody =
							theReturnEnvelope->getElement(
								"Body", xml::XMLNS_SOAP_ENVELOPE
							)
						;
						if( theReturnBody )
						{
							theReturn = theReturnBody->getElement(
								theOutputElement->getAttribute( "name" ),
								targetNamespace
							);
							if( !theReturn )
							{
								theReturn = theReturnBody->getElement(
									"Fault", xml::XMLNS_SOAP_ENVELOPE
								);
								xml::Element *theFaultCode =
									theReturn->getElement( "faultcode" )
								;
								xml::Element *theFaultActor =
									theReturn->getElement( "faultactor" )
								;
								xml::Element *theFaultString =
									theReturn->getElement( "faultstring" )
								;
								xml::Element *theFaultDetail =
									theReturn->getElement( "detail" )
								;
								STRING theFaultCodeStr = theFaultCode
									? theFaultCode->getValue( xml::PLAIN_MODE )
									: NULL_STRING;
								STRING theFaultActorStr = theFaultActor
									? theFaultActor->getValue( xml::PLAIN_MODE )
									: NULL_STRING;
								STRING theFaultStringStr = theFaultString
									? theFaultString->getValue( xml::PLAIN_MODE )
									: NULL_STRING;
								STRING theFaultDetailStr = theFaultDetail
									? theFaultDetail->getValue( xml::PLAIN_MODE )
									: NULL_STRING;

/*@*/							throw SoapException(
									theFaultCodeStr,
									theFaultActorStr,
									theFaultStringStr,
									theFaultDetailStr
								);
							}
						}
						else
						{
/*@*/						throw BadOperationError( theWSDURL );
						}
					}
					else
					{
/*@*/					throw BadOperationError( theWSDURL );
					}
				}
				else
				{
/*@*/				throw BadOperationError( theWSDURL );
				}
			}
			else
			{
				if( !numData )
/*@*/				throw NoAnswerError( theWSDURL );
				else
/*@*/				throw BadOperationError( theWSDURL );
			}
		}

		// if there is not our own parameters used, restore them
		if( newParameter != theParameter )
		{
			theBody->addObject( theParameter );
			theBody->removeObject( newParameter );
		}
	}
	else
	{
/*@*/	throw BadOperationError( theWSDURL );
	}

	return theReturn;
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

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

