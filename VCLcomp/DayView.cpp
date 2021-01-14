//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#pragma hdrstop

#include "DayView.h"
#pragma package(smart_init)

#define TIME_ROW_WIDTH		40

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDayView *)
{
	new TDayView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDayView::TDayView(TComponent* Owner)
	: TStringGrid(Owner)
{
	int		i;
	char	timeStr[16];

	DefaultRowHeight = 18;
	RowCount = 48;
	ColCount = 2;
	FixedRows = 0;
	FixedCols = 0;

	for( i=0; i<48; i++ )
	{
		sprintf( timeStr, "%02d:%02d", i/2, (i%2)*30 );
		Cells[0][i] = timeStr;
	}
	ColWidths[0] = TIME_ROW_WIDTH;

	FFreeTimeBackgrnd = clRed;
	FWorkTimeBackgrnd = clYellow;
	FTimeRowBackgrnd = clBtnFace;
	FAppointmentBackgrnd = clWhite;

}

void __fastcall TDayView::setFreeTimeBackgrnd( TColor newBackground )
{
	FFreeTimeBackgrnd = newBackground;
}

TColor __fastcall TDayView::getFreeTimeBackgrnd( void )
{
	return FFreeTimeBackgrnd;
}

void __fastcall TDayView::setWorkTimeBackgrnd( TColor newBackground )
{
	FWorkTimeBackgrnd = newBackground;
}

TColor __fastcall TDayView::getWorkTimeBackgrnd( void )
{
	return FWorkTimeBackgrnd;
}

void __fastcall TDayView::setTimeRowBackgrnd( TColor newBackground )
{
	FTimeRowBackgrnd = newBackground;
}

TColor __fastcall TDayView::getTimeRowBackgrnd( void )
{
	return FTimeRowBackgrnd;
}

void __fastcall TDayView::setAppointmentBackground( TColor newBackground )
{
	FAppointmentBackgrnd = newBackground;
}

TColor __fastcall TDayView::getAppointmentBackground( void )
{
	return FAppointmentBackgrnd;
}

void __fastcall TDayView::SetBounds(int ALeft, int ATop, int AWidth, int AHeight)
{
	TStringGrid::SetBounds( ALeft, ATop, AWidth, AHeight );

	ColWidths[0] = TIME_ROW_WIDTH;
	ColWidths[1] = AWidth - TIME_ROW_WIDTH - 30;
}

void TDayView::SetAppointment( int row1, int row2, const char *text )
{
	Cells[1][row1] = text;
	while( ++row1 <= row2 )
		Cells[1][row1] = "|";
}

void TDayView::ClearAppointments()
{
	for( int i=0; i<48; i++ )
		Cells[1][i] = "";
}

void __fastcall TDayView::DrawCell(	int ACol, int ARow,
										const TRect &ARect,
										TGridDrawState AState)
{
	const	AnsiString  TheText = Cells[ACol][ARow];

	if( ACol == 0 )
		Canvas->Brush->Color = FTimeRowBackgrnd;
	else if( TheText.Length() > 0 )
		Canvas->Brush->Color = FAppointmentBackgrnd;
	else if( ARow < 16 || ARow > 32 )
		Canvas->Brush->Color = FFreeTimeBackgrnd;
	else
		Canvas->Brush->Color = FWorkTimeBackgrnd;

	Canvas->TextRect(
			ARect,
			ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
			ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
			TheText);
}

//---------------------------------------------------------------------------
namespace Dayview
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TDayView)};
		RegisterComponents("Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------

