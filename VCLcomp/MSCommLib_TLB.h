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
#ifndef   __MSCommLib_TLB_h__
#define   __MSCommLib_TLB_h__

#pragma option push -b -w-inl

#include <vcl/utilcls.h>
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0101)
#error "This file requires an newer version of the header file UTILCLS.H"
#endif

#include <olectl.h>
#include <ocidl.h>
#if defined(USING_ATLVCL) || defined(USING_ATL)
#if !defined(__TLB_NO_EVENT_WRAPPERS)
#include <atl/atlmod.h>
#endif
#endif


// *********************************************************************//
// Forward reference of some VCL types (to avoid including STDVCL.HPP)  //
// *********************************************************************//
namespace Stdvcl {class IStrings; class IStringsDisp;}
using namespace Stdvcl;

namespace Mscommlib_tlb
{

// *********************************************************************//
// HelpString: Microsoft Comm Control
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// GUIDS declared in the TypeLibrary. Following prefixes are used:      //
//   Type Libraries     : LIBID_xxxx                                    //
//   CoClasses          : CLSID_xxxx                                    //
//   DISPInterfaces     : DIID_xxxx                                     //
//   Non-DISP interfaces: IID_xxxx                                      //
// *********************************************************************//
DEFINE_GUID(LIBID_MSCommLib, 0x648A5603, 0x2C6E, 0x101B, 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14);
DEFINE_GUID(DIID__DCMSCommCtrl, 0x648A5601, 0x2C6E, 0x101B, 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14);
DEFINE_GUID(DIID__DCMSCommCtrlEvents, 0x648A5602, 0x2C6E, 0x101B, 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14);
DEFINE_GUID(CLSID_MSComm, 0x648A5600, 0x2C6E, 0x101B, 0x82, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14);

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary                  //
// *********************************************************************//
enum      HiddenHSConstants;
enum      ErrorConstants;
enum      OnCommConstants;
interface DECLSPEC_UUID("{648A5601-2C6E-101B-82B6-000000000014}") _DCMSCommCtrl;
interface DECLSPEC_UUID("{648A5602-2C6E-101B-82B6-000000000014}") _DCMSCommCtrlEvents;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library                     //
// (NOTE: Here we map each CoClass to its Default Interface)            //
//                                                                      //
// The LIBID_OF_ macro(s) map a LIBID_OF_CoClassName to the GUID of this//
// TypeLibrary. It simplifies the updating of macros when CoClass name  //
// change.                                                              //
// *********************************************************************//
typedef _DCMSCommCtrl MSComm;

#define LIBID_OF_MSComm (&LIBID_MSCommLib)

// *********************************************************************//
// Declaration of Enumerations defined in Type Library                  //
// *********************************************************************//
typedef enum HiddenHSConstants
{
  NoHandshaking = 0, 
  XonXoff = 1, 
  RtsCts = 2, 
  XonXoffAndRtsCts = 3
} HiddenHSConstants;

typedef enum ErrorConstants
{
  comBreak = 1001, 
  comCTSTO = 1002, 
  comDSRTO = 1003, 
  comFrame = 1004, 
  comOverrun = 1006, 
  comCDTO = 1007, 
  comRxOver = 1008, 
  comRxParity = 1009, 
  comTxFull = 1010
} ErrorConstants;

typedef enum OnCommConstants
{
  comEvSend = 1, 
  comEvReceive = 2, 
  comEvCTS = 3, 
  comEvDSR = 4, 
  comEvCD = 5, 
  comEvRing = 6, 
  comEvEOF = 7
} OnCommConstants;

// *********************************************************************//
// Interface: _DCMSCommCtrl
// Flags:     (4112) Hidden Dispatchable
// GUID:      {648A5601-2C6E-101B-82B6-000000000014}
// *********************************************************************//
interface _DCMSCommCtrl : public IDispatch
{
// DispInterfaces have no methods. Their methods and properties are
// accessed via IDispatch::Invoke. See the class _DCMSCommCtrlDisp provided
// below for an easy way to access the methods/properties of this
// interface.
};

// *********************************************************************//
// Interface: _DCMSCommCtrlEvents
// Flags:     (4096) Dispatchable
// GUID:      {648A5602-2C6E-101B-82B6-000000000014}
// *********************************************************************//
interface _DCMSCommCtrlEvents : public IDispatch
{
// DispInterfaces have no methods. Their methods and properties are
// accessed via IDispatch::Invoke. See the class _DCMSCommCtrlEventsDisp provided
// below for an easy way to access the methods/properties of this
// interface.
};

#if !defined(__TLB_NO_INTERFACE_WRAPPERS)
// *********************************************************************//
// DispIntf:  _DCMSCommCtrl
// Flags:     (4112) Hidden Dispatchable
// GUID:      {648A5601-2C6E-101B-82B6-000000000014}
// *********************************************************************//
template<class T>
class _DCMSCommCtrlDispT : public TAutoDriver<_DCMSCommCtrl>
{
public:
  _DCMSCommCtrlDispT(){}

  _DCMSCommCtrlDispT(_DCMSCommCtrl *pintf)
  {
    TAutoDriver<_DCMSCommCtrl>::Bind(pintf);
  }

  _DCMSCommCtrlDispT& operator=(_DCMSCommCtrl *pintf)
  {
    TAutoDriver<_DCMSCommCtrl>::Bind(pintf);
    return *this;
  }

  HRESULT BindDefault(/*Binds to new instance of CoClass MSComm*/)
  {
    return OLECHECK(Bind(CLSID_MSComm));
  }

  HRESULT BindRunning(/*Binds to a running instance of CoClass MSComm*/)
  {
    return BindToActive(CLSID_MSComm);
  }

  void            __fastcall set_CDHolding(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_CDHolding();
  void            __fastcall set_CDTimeout(long Value);
  long            __fastcall get_CDTimeout();
  void            __fastcall set_CommID(short Value);
  short           __fastcall get_CommID();
  void            __fastcall set_CommPort(short Value);
  short           __fastcall get_CommPort();
  void            __fastcall set_CTSHolding(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_CTSHolding();
  void            __fastcall set_CTSTimeout(long Value);
  long            __fastcall get_CTSTimeout();
  void            __fastcall set_DSRHolding(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_DSRHolding();
  void            __fastcall set_DSRTimeout(long Value);
  long            __fastcall get_DSRTimeout();
  void            __fastcall set_DTREnable(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_DTREnable();
  void            __fastcall set_Handshaking(Mscommlib_tlb::HiddenHSConstants Value);
  Mscommlib_tlb::HiddenHSConstants __fastcall get_Handshaking();
  void            __fastcall set_InBufferSize(short Value);
  short           __fastcall get_InBufferSize();
  void            __fastcall set_InBufferCount(short Value);
  short           __fastcall get_InBufferCount();
  void            __fastcall set_Break(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_Break();
  void            __fastcall set_InputLen(short Value);
  short           __fastcall get_InputLen();
  void            __fastcall set_Interval(long Value);
  long            __fastcall get_Interval();
  void            __fastcall set_NullDiscard(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_NullDiscard();
  void            __fastcall set_OutBufferSize(short Value);
  short           __fastcall get_OutBufferSize();
  void            __fastcall set_OutBufferCount(short Value);
  short           __fastcall get_OutBufferCount();
  void            __fastcall set_ParityReplace(BSTR Value);
  BSTR            __fastcall get_ParityReplace();
  void            __fastcall set_PortOpen(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_PortOpen();
  void            __fastcall set_RThreshold(short Value);
  short           __fastcall get_RThreshold();
  void            __fastcall set_RTSEnable(TOLEBOOL Value);
  TOLEBOOL        __fastcall get_RTSEnable();
  void            __fastcall set_Settings(BSTR Value);
  BSTR            __fastcall get_Settings();
  void            __fastcall set_SThreshold(short Value);
  short           __fastcall get_SThreshold();
  void            __fastcall set_Output(TVariant Value);
  TVariant        __fastcall get_Output();
  void            __fastcall set_Input(BSTR Value);
  BSTR            __fastcall get_Input();
  void            __fastcall set_CommEvent(short Value);
  short           __fastcall get_CommEvent();
  void            __fastcall AboutBox();

  __property   TOLEBOOL        CDHolding = {read = get_CDHolding, write = set_CDHolding};
  __property   long            CDTimeout = {read = get_CDTimeout, write = set_CDTimeout};
  __property   short           CommID = {read = get_CommID, write = set_CommID};
  __property   short           CommPort = {read = get_CommPort, write = set_CommPort};
  __property   TOLEBOOL        CTSHolding = {read = get_CTSHolding, write = set_CTSHolding};
  __property   long            CTSTimeout = {read = get_CTSTimeout, write = set_CTSTimeout};
  __property   TOLEBOOL        DSRHolding = {read = get_DSRHolding, write = set_DSRHolding};
  __property   long            DSRTimeout = {read = get_DSRTimeout, write = set_DSRTimeout};
  __property   TOLEBOOL        DTREnable = {read = get_DTREnable, write = set_DTREnable};
  __property   Mscommlib_tlb::HiddenHSConstants Handshaking = {read = get_Handshaking, write = set_Handshaking};
  __property   short           InBufferSize = {read = get_InBufferSize, write = set_InBufferSize};
  __property   short           InBufferCount = {read = get_InBufferCount, write = set_InBufferCount};
  __property   TOLEBOOL        Break = {read = get_Break, write = set_Break};
  __property   short           InputLen = {read = get_InputLen, write = set_InputLen};
  __property   long            Interval = {read = get_Interval, write = set_Interval};
  __property   TOLEBOOL        NullDiscard = {read = get_NullDiscard, write = set_NullDiscard};
  __property   short           OutBufferSize = {read = get_OutBufferSize, write = set_OutBufferSize};
  __property   short           OutBufferCount = {read = get_OutBufferCount, write = set_OutBufferCount};
  __property   BSTR            ParityReplace = {read = get_ParityReplace, write = set_ParityReplace};
  __property   TOLEBOOL        PortOpen = {read = get_PortOpen, write = set_PortOpen};
  __property   short           RThreshold = {read = get_RThreshold, write = set_RThreshold};
  __property   TOLEBOOL        RTSEnable = {read = get_RTSEnable, write = set_RTSEnable};
  __property   BSTR            Settings = {read = get_Settings, write = set_Settings};
  __property   short           SThreshold = {read = get_SThreshold, write = set_SThreshold};
  __property   TVariant        Output = {read = get_Output, write = set_Output};
  __property   BSTR            Input = {read = get_Input, write = set_Input};
  __property   short           CommEvent = {read = get_CommEvent, write = set_CommEvent};
};
typedef _DCMSCommCtrlDispT<_DCMSCommCtrl> _DCMSCommCtrlDisp;

// *********************************************************************//
// DispIntf:  _DCMSCommCtrlEvents
// Flags:     (4096) Dispatchable
// GUID:      {648A5602-2C6E-101B-82B6-000000000014}
// *********************************************************************//
template <class T>
class _DCMSCommCtrlEventsDispT : public TAutoDriver<_DCMSCommCtrlEvents>
{
public:
  _DCMSCommCtrlEventsDispT(){}

  void Attach(LPUNKNOWN punk)
  { m_Dispatch = static_cast<T*>(punk); }

  void            __fastcall OnComm();

};
typedef _DCMSCommCtrlEventsDispT<_DCMSCommCtrlEvents> _DCMSCommCtrlEventsDisp;

// *********************************************************************//
// DispIntf:  _DCMSCommCtrl
// Flags:     (4112) Hidden Dispatchable
// GUID:      {648A5601-2C6E-101B-82B6-000000000014}
// *********************************************************************//
template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CDHolding(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CDHolding"), DISPID(1));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CDHolding()
{
  static _TDispID _dispid(*this, OLETEXT("CDHolding"), DISPID(1));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CDTimeout(long /*[VT_I4:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CDTimeout"), DISPID(2));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> long /*[VT_I4:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CDTimeout()
{
  static _TDispID _dispid(*this, OLETEXT("CDTimeout"), DISPID(2));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CommID(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CommID"), DISPID(3));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CommID()
{
  static _TDispID _dispid(*this, OLETEXT("CommID"), DISPID(3));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CommPort(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CommPort"), DISPID(4));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CommPort()
{
  static _TDispID _dispid(*this, OLETEXT("CommPort"), DISPID(4));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CTSHolding(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CTSHolding"), DISPID(5));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CTSHolding()
{
  static _TDispID _dispid(*this, OLETEXT("CTSHolding"), DISPID(5));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CTSTimeout(long /*[VT_I4:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CTSTimeout"), DISPID(6));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> long /*[VT_I4:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CTSTimeout()
{
  static _TDispID _dispid(*this, OLETEXT("CTSTimeout"), DISPID(6));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_DSRHolding(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("DSRHolding"), DISPID(7));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_DSRHolding()
{
  static _TDispID _dispid(*this, OLETEXT("DSRHolding"), DISPID(7));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_DSRTimeout(long /*[VT_I4:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("DSRTimeout"), DISPID(8));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> long /*[VT_I4:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_DSRTimeout()
{
  static _TDispID _dispid(*this, OLETEXT("DSRTimeout"), DISPID(8));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_DTREnable(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("DTREnable"), DISPID(9));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_DTREnable()
{
  static _TDispID _dispid(*this, OLETEXT("DTREnable"), DISPID(9));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_Handshaking(Mscommlib_tlb::HiddenHSConstants /*[VT_USERDEFINED:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("Handshaking"), DISPID(10));
  TAutoArgs<1> _args;
  _args[1] = (int) /*[VT_I4:0]*/ (Mscommlib_tlb::HiddenHSConstants /*[VT_USERDEFINED:0]*/)Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> Mscommlib_tlb::HiddenHSConstants /*[VT_USERDEFINED:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_Handshaking()
{
  static _TDispID _dispid(*this, OLETEXT("Handshaking"), DISPID(10));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return (Mscommlib_tlb::HiddenHSConstants /*[VT_USERDEFINED:0]*/)(int)_args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_InBufferSize(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("InBufferSize"), DISPID(11));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_InBufferSize()
{
  static _TDispID _dispid(*this, OLETEXT("InBufferSize"), DISPID(11));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_InBufferCount(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("InBufferCount"), DISPID(12));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_InBufferCount()
{
  static _TDispID _dispid(*this, OLETEXT("InBufferCount"), DISPID(12));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_Break(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("Break"), DISPID(13));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_Break()
{
  static _TDispID _dispid(*this, OLETEXT("Break"), DISPID(13));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_InputLen(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("InputLen"), DISPID(14));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_InputLen()
{
  static _TDispID _dispid(*this, OLETEXT("InputLen"), DISPID(14));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_Interval(long /*[VT_I4:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("Interval"), DISPID(15));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> long /*[VT_I4:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_Interval()
{
  static _TDispID _dispid(*this, OLETEXT("Interval"), DISPID(15));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_NullDiscard(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("NullDiscard"), DISPID(16));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_NullDiscard()
{
  static _TDispID _dispid(*this, OLETEXT("NullDiscard"), DISPID(16));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_OutBufferSize(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("OutBufferSize"), DISPID(17));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_OutBufferSize()
{
  static _TDispID _dispid(*this, OLETEXT("OutBufferSize"), DISPID(17));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_OutBufferCount(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("OutBufferCount"), DISPID(18));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_OutBufferCount()
{
  static _TDispID _dispid(*this, OLETEXT("OutBufferCount"), DISPID(18));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_ParityReplace(BSTR /*[VT_BSTR:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("ParityReplace"), DISPID(19));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> BSTR /*[VT_BSTR:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_ParityReplace()
{
  static _TDispID _dispid(*this, OLETEXT("ParityReplace"), DISPID(19));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_PortOpen(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("PortOpen"), DISPID(20));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_PortOpen()
{
  static _TDispID _dispid(*this, OLETEXT("PortOpen"), DISPID(20));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_RThreshold(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("RThreshold"), DISPID(21));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_RThreshold()
{
  static _TDispID _dispid(*this, OLETEXT("RThreshold"), DISPID(21));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_RTSEnable(TOLEBOOL /*[VT_BOOL:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("RTSEnable"), DISPID(22));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TOLEBOOL /*[VT_BOOL:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_RTSEnable()
{
  static _TDispID _dispid(*this, OLETEXT("RTSEnable"), DISPID(22));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_Settings(BSTR /*[VT_BSTR:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("Settings"), DISPID(23));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> BSTR /*[VT_BSTR:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_Settings()
{
  static _TDispID _dispid(*this, OLETEXT("Settings"), DISPID(23));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_SThreshold(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("SThreshold"), DISPID(24));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_SThreshold()
{
  static _TDispID _dispid(*this, OLETEXT("SThreshold"), DISPID(24));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_Output(TVariant /*[VT_VARIANT:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("Output"), DISPID(25));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> TVariant /*[VT_VARIANT:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_Output()
{
  static _TDispID _dispid(*this, OLETEXT("Output"), DISPID(25));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_Input(BSTR /*[VT_BSTR:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("Input"), DISPID(26));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> BSTR /*[VT_BSTR:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_Input()
{
  static _TDispID _dispid(*this, OLETEXT("Input"), DISPID(26));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::set_CommEvent(short /*[VT_I2:0]*/ Value)
{
  static _TDispID _dispid(*this, OLETEXT("CommEvent"), DISPID(27));
  TAutoArgs<1> _args;
  _args[1] = Value;
  OlePropertyPut(_dispid, _args);
}

template <class T> short /*[VT_I2:0]*/ __fastcall
_DCMSCommCtrlDispT<T>::get_CommEvent()
{
  static _TDispID _dispid(*this, OLETEXT("CommEvent"), DISPID(27));
  TAutoArgs<0> _args;
  OlePropertyGet(_dispid, _args);
  return _args.GetRetVariant();
}

template <class T> void __fastcall
_DCMSCommCtrlDispT<T>::AboutBox()
{
  static _TDispID _dispid(*this, OLETEXT("AboutBox"), DISPID(-552));
  OleProcedure(_dispid);
}

// *********************************************************************//
// DispIntf:  _DCMSCommCtrlEvents
// Flags:     (4096) Dispatchable
// GUID:      {648A5602-2C6E-101B-82B6-000000000014}
// *********************************************************************//
template <class T> void __fastcall
_DCMSCommCtrlEventsDispT<T>::OnComm()
{
  static _TDispID _dispid(/* OnComm */ DISPID(1));
  OleProcedure(_dispid);
}

#endif  //   __TLB_NO_INTERFACE_WRAPPERS


#if !defined(__TLB_NO_EVENT_WRAPPERS) && defined(USING_ATLVCL)
// *********************************************************************//
// CONNECTIONPOINT/EVENT PROXY
// CoClass         : MSComm
// Event Interface : _DCMSCommCtrlEvents
// *********************************************************************//
template <class T>
class TEvents_MSComm : public IConnectionPointImpl<T,
                                                 &DIID__DCMSCommCtrlEvents,
                                                 CComUnkArray<CONNECTIONPOINT_ARRAY_SIZE> >
{
public:
  void            Fire_OnComm(void);
protected:
  _DCMSCommCtrlEventsDisp m_EventIntfObj;
};

template <class T> void
TEvents_MSComm<T>::Fire_OnComm(void)
{
  T * pT = (T*)this;
  pT->Lock();
  IUnknown ** pp = m_vec.begin();
  while (pp < m_vec.end())
  {
    if (*pp != NULL)
    {
      m_EventIntfObj.Attach(*pp);
      m_EventIntfObj.OnComm();
      m_EventIntfObj.Attach(0);
    }
    pp++;
  }
  pT->Unlock();
}

#endif  //   __TLB_NO_EVENT_WRAPPERS

};     // namespace Mscommlib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Mscommlib_tlb;
#endif

#pragma option pop

#endif // __MSCommLib_TLB_h__
