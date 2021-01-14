//---------------------------------------------------------------------------
#ifndef DayViewH
#define DayViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class PACKAGE TDayView : public TStringGrid
{
	TColor	FFreeTimeBackgrnd;
	TColor	FWorkTimeBackgrnd;
	TColor	FTimeRowBackgrnd;
	TColor	FAppointmentBackgrnd;

private:
	virtual void __fastcall SetBounds(int ALeft, int ATop, int AWidth, int AHeight);

	void __fastcall setFreeTimeBackgrnd( TColor newBackground );
	TColor __fastcall getFreeTimeBackgrnd( void );

	void __fastcall setWorkTimeBackgrnd( TColor newBackground );
	TColor __fastcall getWorkTimeBackgrnd( void );

	void __fastcall setTimeRowBackgrnd( TColor newBackground );
	TColor __fastcall getTimeRowBackgrnd( void );

	void __fastcall setAppointmentBackground( TColor newBackground );
	TColor __fastcall getAppointmentBackground( void );

protected:
	virtual void __fastcall DrawCell(	int ACol, int ARow,
										const TRect &ARect,
										TGridDrawState AState);
public:
	__fastcall TDayView(TComponent* Owner);
	void SetAppointment( int row1, int row2, const char *text );
	void ClearAppointments();
__published:
	__property TColor FreeTimeBackgrnd = { read=getFreeTimeBackgrnd, write=setFreeTimeBackgrnd };
	__property TColor WorkTimeBackgrnd = { read=getWorkTimeBackgrnd, write=setWorkTimeBackgrnd };
	__property TColor TimeRowBackgrnd = { read=getTimeRowBackgrnd, write=setTimeRowBackgrnd };
	__property TColor AppointmentBackgrnd = { read=getAppointmentBackground, write=setAppointmentBackground };
};
//---------------------------------------------------------------------------
#endif

