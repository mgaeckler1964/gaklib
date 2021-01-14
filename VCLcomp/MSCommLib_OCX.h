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
#ifndef   __MSCommLib_OCX_h__
#define   __MSCommLib_OCX_h__

#pragma option push -b -w-inl

#include <vcl/utilcls.h>
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0101)
#error "This file requires an newer version of the header file UTILCLS.H"
#endif

#include <olectl.h>
#include <ocidl.h>
#if !defined(_NO_VCL)   // For Console applications that don't use VCL
#include <vcl/stdvcl.hpp>
#endif  //   _NO_VCL
#include <vcl/ocxproxy.h>

#include "MSCommLib_TLB.h"
namespace Mscommlib_tlb
{

// *********************************************************************//
// HelpString: Microsoft Comm Control
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// OCX PROXY CLASS DECLARATION
// Control Name     : TMSCommProxy
// Help String      : Microsoft Comm Control
// Default Interface: _DCMSCommCtrl
// Def. Intf. Object: _DCMSCommCtrlDisp
// Def. Intf. DISP? : Yes
// Event   Interface: _DCMSCommCtrlEvents
// TypeFlags        : (34) CanCreate Control
// *********************************************************************//

// *********************************************************************//
// Definition of closures to allow VCL handlers to catch OCX events.    //
// *********************************************************************//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// Proxy class to host Microsoft Comm Control in CBuilder IDE/Applications.
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class PACKAGE TMSCommProxy : public TOleControl
{
  OVERLOADED_PROP_METHODS;
  static TNoParam  OptParam;
  static GUID      DEF_CTL_INTF;

  // Instance of Closures to expose OCX Events as VCL ones
  //
  TNotifyEvent                   FOnComm;

  // Default Interace of OCX
  //
  _DCMSCommCtrlDisp m_OCXIntf;

  // VCL Property Getters/Setters which delegate to OCX
  //
  TVariant __fastcall Get_Output();
  void __fastcall Set_Output(TVariant param);

  // Static variables used by all instances of OCX proxy
  //
  static int          EventDispIDs[1];
  static WCHAR        wLicenseKey[20];
  static TControlData CControlData;
  static GUID         CTL_DEF_INTF;

  // Method providing access to interface as __property 
  //
  _DCMSCommCtrlDisp __fastcall GetControlInterface(void);

protected:
  void     __fastcall CreateControl  (void);
  void     __fastcall InitControlData(void);

public:
  virtual  __fastcall TMSCommProxy(TComponent* AOwner) : TOleControl(AOwner)
  {};
  virtual  __fastcall TMSCommProxy(HWND        Parent) : TOleControl(Parent)
  {};

  // OCX methods
  //
  void            __fastcall AboutBox(void);

  // OCX properties
  //
  __property System::OleVariant Output={ read=GetOleVariantProp, write=SetOleVariantProp, index=25 };
  __property _DCMSCommCtrlDisp ControlInterface={ read=GetControlInterface };

  // Published properties
  //
__published:

  // Standard/Extended properties
  //

  // OCX properties
  //
  __property bool CDHolding={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=1 };
  __property int CDTimeout={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=2 };
  __property short CommID={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=3 };
  __property short CommPort={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=4 };
  __property bool CTSHolding={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=5 };
  __property int CTSTimeout={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=6 };
  __property bool DSRHolding={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=7 };
  __property int DSRTimeout={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=8 };
  __property bool DTREnable={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=9 };
  __property TOleEnum Handshaking={ read=GetTOleEnumProp, write=SetTOleEnumProp, stored=false, index=10 };
  __property short InBufferSize={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=11 };
  __property short InBufferCount={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=12 };
  __property bool Break={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=13 };
  __property short InputLen={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=14 };
  __property int Interval={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=15 };
  __property bool NullDiscard={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=16 };
  __property short OutBufferSize={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=17 };
  __property short OutBufferCount={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=18 };
  __property System::WideString ParityReplace={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=19 };
  __property bool PortOpen={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=20 };
  __property short RThreshold={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=21 };
  __property bool RTSEnable={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=22 };
  __property System::WideString Settings={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=23 };
  __property short SThreshold={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=24 };
  __property System::WideString Input={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=26 };
  __property short CommEvent={ read=GetSmallintProp, write=SetSmallintProp, stored=false, index=27 };

  // OCX Events
  //
  __property TNotifyEvent OnComm={ read=FOnComm, write=FOnComm };
};
typedef TMSCommProxy  TMSCommProxyProxy;

};     // namespace Mscommlib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Mscommlib_tlb;
#endif

#pragma option pop

#endif // __MSCommLib_OCX_h__
