// Tests\myService.cpp
// Generated from file http://judith/WebService/service.php?wsdl
//(c) 1988-2018 by Martin Gäckler, Munich

#include <gak/numericString.h>
#include "myService.h"
void myService::fillget_messageRequestType( get_messageRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *your_name = xml->getElement( "your_name" );
if( your_name )
item.your_name = your_name->getValue( gak::xml::PLAIN_MODE );
}
void myService::fillget_messageRequestType( gak::Array<get_messageRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillget_messageRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void myService::fillget_messageResponseType( get_messageResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *message = xml->getElement( "message" );
if( message )
item.message = message->getValue( gak::xml::PLAIN_MODE );
}
void myService::fillget_messageResponseType( gak::Array<get_messageResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillget_messageResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void myService::fillMyRecord( MyRecord_t &item, gak::xml::Element *xml ) {
gak::xml::Element *ID = xml->getElement( "ID" );
if( ID )
item.ID = atoi( ID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *YourName = xml->getElement( "YourName" );
if( YourName )
item.YourName = YourName->getValue( gak::xml::PLAIN_MODE );
}
void myService::fillMyRecord( gak::Array<MyRecord_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillMyRecord( itemArray.createElement(), xml->getElement( i ) );
} }
void myService::fillMyTable( MyTable_t &item, gak::xml::Element *xml ) {
}
void myService::fillMyTable( gak::Array<MyTable_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillMyTable( itemArray.createElement(), xml->getElement( i ) );
} }
void myService::fillHelloComplexWorldRequestType( HelloComplexWorldRequestType_t &item, gak::xml::Element *xml ) {
}
void myService::fillHelloComplexWorldRequestType( gak::Array<HelloComplexWorldRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillHelloComplexWorldRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void myService::fillHelloComplexWorldResponseType( HelloComplexWorldResponseType_t &item, gak::xml::Element *xml ) {
}
void myService::fillHelloComplexWorldResponseType( gak::Array<HelloComplexWorldResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillHelloComplexWorldResponseType( itemArray.createElement(), xml->getElement( i ) );
} }

gak::STRING myService::get_message( const gak::STRING &your_name) {
setOperation( "get_message" );
setParameter( "your_name", your_name );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::STRING myService::get_message( gak::xml::Element *theParameter ) {
setOperation( "get_message" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
return value;
}

gak::xml::Element * myService::HelloComplexWorld( const MyRecord_t &myStruct) {
setOperation( "HelloComplexWorld" );
setParameter( "myStruct/ID", myStruct.ID );
setParameter( "myStruct/YourName", myStruct.YourName );
gak::xml::Element *response = execute();
if( response )
response = response->getElement(0);
return response;
}

gak::xml::Element * myService::HelloComplexWorld( gak::xml::Element *theParameter ) {
setOperation( "HelloComplexWorld" );
gak::xml::Element *response = execute( theParameter );
if( response )
response = response->getElement(0);
return response;
}
