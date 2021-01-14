//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("ocx_pack.res");
USEUNIT("MSCommLib_OCX.cpp");
USERES("MSCommLib_OCX.dcr");
USEPACKAGE("vcl50.bpi");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------


