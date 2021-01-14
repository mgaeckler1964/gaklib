// ************************************************************************ //
// WARNING                                                                  //
// -------                                                                  //
// The types declared in this file were generated from data read from a     //
// Type Library. If this type library is explicitly or indirectly (via      //
// another type library referring to this type library) re-imported, or the //
// 'Refresh' command of the Type Library Editor activated while editing the //
// Type Library, the contents of this file will be regenerated and all      //
// manual modifications will be lost.                                       //
// ************************************************************************ //

// C++ TLBWRTR : $Revision:   1.96.1.27  $
// File generated on 16.11.1999 20:22:26 from Type Library described below.

// ************************************************************************ //
// Type Lib: D:\WINNT4\system32\MSCOMM32.OCX
// IID\LCID: {648A5603-2C6E-101B-82B6-000000000014}\0
// Helpfile: D:\WINNT4\System32\CTRLREF.HLP
// DepndLst: 
//   (1) v1.0 stdole, (D:\WINNT4\System32\stdole32.tlb)
//   (2) v2.0 StdType, (D:\WINNT4\System32\OLEPRO32.DLL)
//   (3) v1.0 StdVCL, (D:\WINNT4\System32\STDVCL32.DLL)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "MSCommLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Mscommlib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TMSCommProxy which
// allows "Microsoft Comm Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TMSCommProxy::EventDispIDs[1] = {
    0x00000001};

WCHAR TMSCommProxy::wLicenseKey[20] = {
    0x0043, 0x006F, 0x0070, 0x0079, 0x0072, 0x0069, 0x0067, 0x0068, 0x0074, 0x0020,
    0x0028, 0x0063, 0x0029, 0x0020, 0x0031, 0x0039, 0x0039, 0x0034, 0x0020, 0x0000};

TControlData TMSCommProxy::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0x648A5600, 0x2C6E, 0x101B,{ 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14} }, // CoClass
  {0x648A5602, 0x2C6E, 0x101B,{ 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14} }, // Events

  // Count of Events and array of their DISPIDs
  1, &EventDispIDs,

  // Pointer to Runtime License string
  &wLicenseKey,  // HRESULT(0x00000000)

  // Flags for OnChanged PropertyNotification
  0x00000000,
  300,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  0, NULL,

  // Count of Pict Prop and array of their DISPIDs
  0, NULL,
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TMSCommProxy::DEF_CTL_INTF = {0x648A5601, 0x2C6E, 0x101B,{ 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14} };
TNoParam TMSCommProxy::OptParam;

static inline void ValidCtrCheck(TMSCommProxy *)
{
   delete new TMSCommProxy((TComponent*)(0));
};

void __fastcall TMSCommProxy::InitControlData(void)
{
  ControlData = &CControlData;
};

void __fastcall TMSCommProxy::CreateControl(void)
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

_DCMSCommCtrlDisp __fastcall TMSCommProxy::GetControlInterface(void)
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TMSCommProxy::AboutBox(void)
{
  GetControlInterface().AboutBox();
}

TVariant __fastcall TMSCommProxy::Get_Output()
{
  return GetControlInterface().get_Output();
}

void __fastcall TMSCommProxy::Set_Output(TVariant param)
{
  GetControlInterface().set_Output(param);
}

};     // namespace Mscommlib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Mscommlib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass classes[] = {
                              __classid(Mscommlib_tlb::TMSCommProxy)
                             };
  RegisterComponents("ActiveX", classes,
                     sizeof(classes)/sizeof(classes[0])-1);
}
};     // namespace Mscommlib_ocx
