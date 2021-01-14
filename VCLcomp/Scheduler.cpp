//---------------------------------------------------------------------------
#include <stdlib.h>

#include <vcl.h>

#pragma hdrstop

#include "Scheduler.h"
#pragma link "ccalendr"
#pragma package(smart_init)

#include <gak/date.h>

using namespace gak;

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TScheduler *)
{
	new TScheduler(NULL);
}
//---------------------------------------------------------------------------
__fastcall TScheduler::TScheduler(TComponent* Owner)
	: TCCalendar(Owner)
{
	FHolidayFont = new TFont;
	FAppointmentFont = new TFont;

	FHolidayBackgrnd =
	FAppointmentBackgrnd =
	Color;
}
//---------------------------------------------------------------------------
void __fastcall TScheduler::setHolidayBackground( TColor newBackground )
{
	FHolidayBackgrnd = newBackground;
}

TColor __fastcall TScheduler::getHolidayBackground( void )
{
	return FHolidayBackgrnd;
}

void __fastcall TScheduler::setAppointmentBackground( TColor newBackground )
{
	FAppointmentBackgrnd = newBackground;
}

TColor __fastcall TScheduler::getAppointmentBackground( void )
{
	return FAppointmentBackgrnd;
}

void __fastcall TScheduler::setHolidayFont( TFont *newFont )
{
	FHolidayFont->Assign( newFont );
}

TFont *__fastcall TScheduler::getHolidayFont( void )
{
	return FHolidayFont;
}

void __fastcall TScheduler::setAppointmentFont( TFont *newFont )
{
	FAppointmentFont->Assign( newFont );
}

TFont *__fastcall TScheduler::getAppointmentFont( void )
{
	return FAppointmentFont;
}

//---------------------------------------------------------------------------
void __fastcall TScheduler::ClearAppointments( void )
{
	int	i;

	for( i=0; i<31; i++ )
		appointments[i] = false;
}

void __fastcall TScheduler::SetAppointment( int day, bool appFlag )
{
	appointments[day] = appFlag;
}
//---------------------------------------------------------------------------
void __fastcall TScheduler::DrawCell(	int ACol, int ARow,
										const TRect &ARect,
										TGridDrawState AState)
{
	const	AnsiString  TheText = CellText[ACol][ARow];

	if( !AState.Contains( gdSelected ) )
	{
		int cellDay = atoi( TheText.c_str() );

		if( appointments[cellDay] )
		{
			Canvas->Brush->Color = FAppointmentBackgrnd;
			Canvas->Font = FAppointmentFont;
		}
		else
		{
			gak::Date	holiday(
				(unsigned char)cellDay, gak::Date::Month(
					Month
				), (unsigned short)Year
			);

			if( *(holiday.holiday()) )
			{
				Canvas->Brush->Color = FHolidayBackgrnd;
				Canvas->Font = FHolidayFont;
			}
		}
	}

	Canvas->TextRect(
			ARect,
			ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
			ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
			TheText);
}

//---------------------------------------------------------------------------
namespace Scheduler
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TScheduler)};
		RegisterComponents("Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------

