/*
		Project:		GAKLIB
		Module:			soap.h
		Description:	A SOAP client
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

#ifndef SOAP_H
#define SOAP_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/http.h>
#include <gak/xmlValidator.h>
#include <gak/logfile.h>

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

/// Base class for all exceptions thrown by SoapRequest
class SOAPerror : public HTTPerror
{
	protected:
	SOAPerror( const STRING &errText, const STRING &url ) : HTTPerror( errText, url )
	{
	}
};

/// This exception is thrown if the URL did not return a WSDL file
class NoWSDLerror : public SOAPerror
{
	public:
	NoWSDLerror( const STRING &url ) : SOAPerror( "No WSDL Document", url )
	{
	}
};

/// This exception is thrown if the URL did returned a corrupted WSDL file without a definition
class NoWSDLdefinitionError : public SOAPerror
{
	public:
	NoWSDLdefinitionError( const STRING &url ) : SOAPerror( "No WSDL DEfinition in Document", url )
	{
	}
};

/// This exception is thrown if the application tried to send a bad or unkown operation to the soap server
class BadOperationError : public SOAPerror
{
	public:
	BadOperationError( const STRING &url ) : SOAPerror( "Bad SOAP Operation", url )
	{
	}
};

/// This exception is thrown if the soap server did not answer
class NoAnswerError : public SOAPerror
{
	public:
	NoAnswerError( const STRING &url ) : SOAPerror( "No Answer from SOAP-Server", url )
	{
	}
};

/**
	@brief base for all SOAP requests

	Use impwsdl tool to create C++ bindings for your soap server
*/
class SoapRequest : public HTTPrequest
{
	friend class WSDLimporter;

	bool			needCredentials;

	STRING			theWSDURL, theServiceUrl;
	// the WSDL
	xml::Document	*theWsdDoc;
	// "definitions/portType/operation/input" and "../output" of the
	// current operation withing the actual WSDL
	xml::Element	*theInputElement, *theOutputElement;

	// Here are the elements of the current soap request
	xml::Element	*theEnvelope, *theBody, *theParameter;

	// this is the validator which is used to validate the request
	xml::Validator	*theValidator;

	STRING			lastOperation, SOAPAction, targetNamespace;

	// this is the soap response:
	xml::Document	*theResponseDoc;

	xml::Element *getSchema( xml::Element *theDefinitions );
	xml::Element *getSchemaElement( xml::Element *theDefinitions, const STRING &message, xml::Element **theSchemaOut=NULL );

	void loadWSD( const STRING &theServiceUrl, const STRING &userName, const STRING &password );

	protected:
	xml::Element *setOperation( const char *operation );

	xml::Element *setParameter( const char *parameterName, const char *value );
	xml::Element *setParameter( const char *parameterName, int value )
	{
		char	buffer[32];

		sprintf( buffer, "%d", value );
		return setParameter( parameterName, buffer );
	}

	xml::Element *execute( xml::Element *newParameter );
	xml::Element *execute( void )
	{
		return execute( theParameter );
	}

	public:
	/// exception thrown in case of a SOAP error
	class SoapException : public std::exception
	{
		public:
		STRING	faultCode, faultActor, faultString, faultDetail;

		SoapException(
			const STRING &theFaultCode, const STRING &theFaultActor,
			const STRING &theFaultString,  const STRING &theFaultDetail
		) :
		faultCode(theFaultCode), faultActor(theFaultActor),
		faultString(theFaultString), faultDetail(theFaultDetail)
		{
		}
		virtual ~SoapException() throw()
		{
		}
		virtual const char * what() const throw()
		{
			return faultString;
		}
	};
	protected:
	SoapRequest(
		const STRING &theWSDURL,
		const STRING &userName=NULL_STRING,
		const STRING &password=NULL_STRING
	)
	{
		doEnterFunction("SoapRequest::SoapRequest");
		try
		{
			loadWSD( theWSDURL, userName, password );
		}
		catch( NoAuthorisationError & )
		{
			// ignore
		}
	};
	public:
	~SoapRequest()
	{
		if( theWsdDoc )
		{
			delete theWsdDoc;
		}
		if( theEnvelope )
		{
			delete theEnvelope;
		}
		if( theValidator )
		{
			delete theValidator;
		}
		if( theResponseDoc )
		{
			delete theResponseDoc;
		}
	}
	/// returns true, if the server requires basic authentication
	bool getNeedCredentials( void ) const
	{
		return needCredentials;
	}
	/// returns true if the URL returned a WSDL document
	bool hasWsdDoc( void )
	{
		return theWsdDoc != NULL;
	}
	/// sets the credential for the SOAP requests if the WSDL was not yet loaded
	void setCredentials( const STRING &userName, const STRING &password )
	{
		if( !theWsdDoc )
		{
			loadWSD( theWSDURL, userName, password );
		}
	}
	/// returns the XML document of the last request
	STRING	getXmlRequest( void )
	{
		return ( theEnvelope ) ? theEnvelope->generateDoc() : NULL_STRING;
	}

	/// returns the body of the HTTP response for the last request
	const char *getBody()
	{
		return getHttpResponse().getBody();
	}
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

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.

#	pragma warn +inl
#endif

#endif
