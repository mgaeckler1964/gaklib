// Tests\Service1.cpp
// Generated from file http://saturn/WebService/Service1.asmx?WSDL
//(c) 1988-2018 by Martin Gäckler, Munich

#include <gak/numericString.h>
#include "Service1.h"
void Service1::fillTestClass( TestClass_t &item, gak::xml::Element *xml ) {
gak::xml::Element *name = xml->getElement( "name" );
if( name )
item.name = name->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *id = xml->getElement( "id" );
if( id )
item.id = atoi( id->getValue( gak::xml::PLAIN_MODE ) );
}
void Service1::fillTestClass( gak::Array<TestClass_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillTestClass( itemArray.createElement(), xml->getElement( i ) );
} }

gak::STRING Service1::HelloWorld( ) {
setOperation( "HelloWorld" );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::STRING Service1::HelloWorld( gak::xml::Element *theParameter ) {
setOperation( "HelloWorld" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::STRING Service1::StrCat( const gak::STRING &str1, const gak::STRING &str2) {
setOperation( "StrCat" );
setParameter( "str1", str1 );
setParameter( "str2", str2 );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::STRING Service1::StrCat( gak::xml::Element *theParameter ) {
setOperation( "StrCat" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::STRING Service1::extractName( const TestClass_t &entry) {
setOperation( "extractName" );
setParameter( "entry/name", entry.name );
setParameter( "entry/id", entry.id );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::STRING Service1::extractName( gak::xml::Element *theParameter ) {
setOperation( "extractName" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::xml::Element * Service1::createUser( ) {
setOperation( "createUser" );
gak::xml::Element *response = execute();
if( response )
response = response->getElement(0);
return response;
}

gak::xml::Element * Service1::createUser( gak::xml::Element *theParameter ) {
setOperation( "createUser" );
gak::xml::Element *response = execute( theParameter );
if( response )
response = response->getElement(0);
return response;
}
