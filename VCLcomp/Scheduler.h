//---------------------------------------------------------------------------
#ifndef SchedulerH
#define SchedulerH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "ccalendr.h"
#include <Grids.hpp>

//---------------------------------------------------------------------------
class PACKAGE TScheduler : public TCCalendar
{
private:
	TColor	FHolidayBackgrnd;
	TColor	FAppointmentBackgrnd;

	TFont	*FHolidayFont;
	TFont	*FAppointmentFont;

	bool	appointments[31];

	void __fastcall setHolidayBackground( TColor newBackground );
	TColor __fastcall getHolidayBackground( void );

	void __fastcall setAppointmentBackground( TColor newBackground );
	TColor __fastcall getAppointmentBackground( void );

	void __fastcall setHolidayFont( TFont *newFont );
	TFont *__fastcall getHolidayFont( void );

	void __fastcall setAppointmentFont( TFont *newFont );
	TFont *__fastcall getAppointmentFont( void );

protected:
	virtual void __fastcall DrawCell(	int ACol, int ARow,
										const TRect &ARect,
										TGridDrawState AState);
public:
	__fastcall TScheduler(TComponent* Owner);

	void __fastcall ClearAppointments( void );
	void __fastcall SetAppointment( int day, bool appFlag );

__published:

	__property TColor HolidayBackgrnd = { read=getHolidayBackground, write=setHolidayBackground };
	__property TColor AppointmentBackgrnd = { read=getAppointmentBackground, write=setAppointmentBackground };

	__property TFont *HolidayFont = { read=getHolidayFont, write=setHolidayFont };
	__property TFont *AppointmentFont = { read=getAppointmentFont, write=setAppointmentFont };
};
//---------------------------------------------------------------------------
#endif

