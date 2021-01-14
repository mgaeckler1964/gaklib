// Tests/theService.cpp
// Generated from file http://judith/WebService/service.php?wsdl
//(c) 2008-2014 by CRESD GmbH, Munich, http://www.cresd.de

#include <gak/numericString.h>
#include "theService.h"
void theService::fillget_messageRequestType( get_messageRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *your_name = xml->getElement( "your_name" );
if( your_name )
item.your_name = your_name->getValue( gak::xml::PLAIN_MODE );
}
void theService::fillget_messageRequestType( gak::Array<get_messageRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumElements(); i++ )
{
fillget_messageRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void theService::fillget_messageResponseType( get_messageResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *message = xml->getElement( "message" );
if( message )
item.message = message->getValue( gak::xml::PLAIN_MODE );
}
void theService::fillget_messageResponseType( gak::Array<get_messageResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumElements(); i++ )
{
fillget_messageResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void theService::fillMyRecord( MyRecord_t &item, gak::xml::Element *xml ) {
gak::xml::Element *ID = xml->getElement( "ID" );
if( ID )
item.ID = atoi( ID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *YourName = xml->getElement( "YourName" );
if( YourName )
item.YourName = YourName->getValue( gak::xml::PLAIN_MODE );
}
void theService::fillMyRecord( gak::Array<MyRecord_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumElements(); i++ )
{
fillMyRecord( itemArray.createElement(), xml->getElement( i ) );
} }
void theService::fillMyTable( MyTable_t &item, gak::xml::Element *xml ) {
}
void theService::fillMyTable( gak::Array<MyTable_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumElements(); i++ )
{
fillMyTable( itemArray.createElement(), xml->getElement( i ) );
} }
void theService::fillHelloComplexWorldRequestType( HelloComplexWorldRequestType_t &item, gak::xml::Element *xml ) {
}
void theService::fillHelloComplexWorldRequestType( gak::Array<HelloComplexWorldRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumElements(); i++ )
{
fillHelloComplexWorldRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void theService::fillHelloComplexWorldResponseType( HelloComplexWorldResponseType_t &item, gak::xml::Element *xml ) {
}
void theService::fillHelloComplexWorldResponseType( gak::Array<HelloComplexWorldResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumElements(); i++ )
{
fillHelloComplexWorldResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
