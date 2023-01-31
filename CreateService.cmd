rem create the soap binding from my C# Soap Server
impwsdl http://saturn/WebService/Service1.asmx?WSDL

rem create the soap binding from my PHP Soap Server
impwsdl -userName root -userPassword password http://judith/WebService/service.php?wsdl
pause