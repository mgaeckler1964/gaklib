//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("gak_components.res");
USEUNIT("DayView.cpp");
USEUNIT("Scheduler.cpp");
USEPACKAGE("vcl50.bpi");
USEPACKAGE("bcbsmp50.bpi");
USELIB("..\..\..\Object\gaklib\gaklib_bcb.lib");
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

