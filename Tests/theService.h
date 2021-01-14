// Tests/theService.h
// Generated from file http://judith/WebService/service.php?wsdl
//(c) 2008-2014 by CRESD GmbH, Munich, http://www.cresd.de

#ifndef SOAP_theService_H
#define SOAP_theService_H
#include <gak/soap.h>
class theService : public gak::net::SoapRequest {
public:
theService() : gak::net::SoapRequest("http://judith/WebService/service.php?wsdl") {}
theService( const char *wsdlUrl ) : gak::net::SoapRequest(wsdlUrl) {}

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
};
#endif
