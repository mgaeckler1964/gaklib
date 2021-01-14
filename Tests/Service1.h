// Tests\Service1.h
// Generated from file http://saturn/WebService/Service1.asmx?WSDL
//(c) 1988-2018 by Martin Gaeckler, Munich

#ifndef SOAP_Service1_H
#define SOAP_Service1_H
#include <gak/soap.h>
class Service1 : public gak::net::SoapRequest {
public:
Service1() : gak::net::SoapRequest("http://saturn/WebService/Service1.asmx?WSDL") {}
Service1( const char *wsdlUrl ) : gak::net::SoapRequest(wsdlUrl) {}

typedef struct {
gak::STRING name;
int id;
}TestClass_t;
void fillTestClass( TestClass_t &item, gak::xml::Element *xml );
void fillTestClass( gak::Array<TestClass_t> &item, gak::xml::Element *xml );

gak::STRING HelloWorld( );
gak::STRING HelloWorld( gak::xml::Element *theParameter );

gak::STRING StrCat( const gak::STRING &str1, const gak::STRING &str2);
gak::STRING StrCat( gak::xml::Element *theParameter );

gak::STRING extractName( const TestClass_t &entry);
gak::STRING extractName( gak::xml::Element *theParameter );

gak::xml::Element * createUser( );
gak::xml::Element * createUser( gak::xml::Element *theParameter );
};
#endif
