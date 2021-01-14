// Tests\myService.h
// Generated from file http://judith/WebService/service.php?wsdl
//(c) 1988-2018 by Martin Gaeckler, Munich

#ifndef SOAP_myService_H
#define SOAP_myService_H
#include <gak/soap.h>
class myService : public gak::net::SoapRequest {
public:
myService() : gak::net::SoapRequest("http://judith/WebService/service.php?wsdl") {}
myService( const char *wsdlUrl ) : gak::net::SoapRequest(wsdlUrl) {}

typedef struct {
gak::STRING your_name;
}get_messageRequestType_t;
void fillget_messageRequestType( get_messageRequestType_t &item, gak::xml::Element *xml );
void fillget_messageRequestType( gak::Array<get_messageRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
gak::STRING message;
}get_messageResponseType_t;
void fillget_messageResponseType( get_messageResponseType_t &item, gak::xml::Element *xml );
void fillget_messageResponseType( gak::Array<get_messageResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int ID;
gak::STRING YourName;
}MyRecord_t;
void fillMyRecord( MyRecord_t &item, gak::xml::Element *xml );
void fillMyRecord( gak::Array<MyRecord_t> &item, gak::xml::Element *xml );

typedef struct {
MyRecord_t item;
}MyTable_t;
void fillMyTable( MyTable_t &item, gak::xml::Element *xml );
void fillMyTable( gak::Array<MyTable_t> &item, gak::xml::Element *xml );

typedef struct {
MyRecord_t myStruct;
}HelloComplexWorldRequestType_t;
void fillHelloComplexWorldRequestType( HelloComplexWorldRequestType_t &item, gak::xml::Element *xml );
void fillHelloComplexWorldRequestType( gak::Array<HelloComplexWorldRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
MyTable_t theTable;
}HelloComplexWorldResponseType_t;
void fillHelloComplexWorldResponseType( HelloComplexWorldResponseType_t &item, gak::xml::Element *xml );
void fillHelloComplexWorldResponseType( gak::Array<HelloComplexWorldResponseType_t> &item, gak::xml::Element *xml );

gak::STRING get_message( const gak::STRING &your_name);
gak::STRING get_message( gak::xml::Element *theParameter );

gak::xml::Element * HelloComplexWorld( const MyRecord_t &myStruct);
gak::xml::Element * HelloComplexWorld( gak::xml::Element *theParameter );
};
#endif
