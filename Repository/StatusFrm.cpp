/*
		Project:		GAKLIB
		Module:			StatusFrm.cpp
		Description:	
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/thread.h>

#include <winlib/winlib.h>

#pragma hdrstop

#include "StatusFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStatusForm *StatusForm;
//---------------------------------------------------------------------------
__fastcall TStatusForm::TStatusForm(TComponent* Owner)
	: TForm(Owner)
{
	theThread = NULL;
}
__fastcall TStatusForm::~TStatusForm()
{
	doEnterFunction("TStatusForm::~TStatusForm");
	doLogValue( Handle );
	doLogPosition();
}
//---------------------------------------------------------------------------
bool TStatusForm::setStatus( const STRING &verb, const STRING &status )
{
	doEnterFunction("TStatusForm::setStatus");
	doLogValue( Handle );

	if( true || Visible )
	{
		try
		{
			STRING newCaption = verb + " " + status;

			StatusLabel->Caption = (const char *)newCaption;

			if( !theThread )
				idleLoop();

		}
		catch( ... )
		{
			// ignore
			doLogPosition();
		}
	}
	return theThread ? theThread->terminated : false;
}
//---------------------------------------------------------------------------
bool TStatusForm::pushStatus( const STRING &verb, const STRING &status )
{
	if( true || Visible )
	{
		try
		{
			STRING	oldCaption = StatusLabel->Caption.c_str();
			if( !oldCaption.isEmpty() )
				stack.push( oldCaption );
			setStatus( verb, status );
		}
		catch( ... )
		{
			// ignore
		}
	}

	return theThread ? (*theThread).terminated : false;
}
//---------------------------------------------------------------------------
bool TStatusForm::restore( void )
{
	if( stack.size() )
	{
		STRING newCaption = stack.pop();
		if( !newCaption.isEmpty() )
		{
			try
			{
				StatusLabel->Caption = (const char *)newCaption;
			}
			catch( ... )
			{
				// ignore
				doLogPosition();
			}
		}
	}

	return theThread ? theThread->terminated : false;
}
//---------------------------------------------------------------------------
void __fastcall TStatusForm::FormCloseQuery(TObject *,
	  bool &CanClose)
{
	Thread::CheckThreadCount();
	CanClose = !theThread || !theThread->isRunning || theThread->threadFinished;
}
//---------------------------------------------------------------------------
void StatusThread::ExecuteThread( void )
{
	doEnterFunction("StatusThread::ExecuteThread");
	try
	{
		perform();
		StatusForm->setStatus( "Finish", "" );
		threadFinished = true;
		StatusForm->Close();
	}
	catch( Exception &e )
	{
		StatusForm->setStatus( e.Message.c_str(), "" );
	}
	catch( std::exception &e )
	{
		StatusForm->setStatus( e.what(), "" );
	}
	catch( ... )
	{
		StatusForm->setStatus( "Unkown error", "" );
	}
}

//---------------------------------------------------------------------------

void __fastcall TStatusForm::FormClose(TObject *,
	  TCloseAction &)
{
	stack.clear();

	// remove unused thread objects
	Thread::CheckThreadCount();
	theThread = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TStatusForm::BitBtnStopClick(TObject *)
{
	if( !theThread || !theThread->isRunning || theThread->threadFinished )
		Close();
	else if( confirm )
	{
		STRING	confirmMsg = "Stop ";
		confirmMsg += (*theThread).getTitle();
		if( Application->MessageBox( confirmMsg, "Stop", MB_YESNO|MB_ICONSTOP ) == IDYES )
			theThread->StopThread();
	}
	else
		theThread->StopThread();
}
//---------------------------------------------------------------------------

void __fastcall TStatusForm::CheckThread(TObject *Sender)
{
	bool	CanClose;

	if( !Thread::CheckThreadCount() )
		theThread = NULL;

	FormCloseQuery( Sender, CanClose );
	if( CanClose )
	{
		ModalResult = mrOk;
//		Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TStatusForm::FormShow(TObject *Sender)
{
	if( theThread )
	{
		CheckThread( Sender );
		BitBtnStop->Enabled = true;
	}
	else
		BitBtnStop->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TStatusForm::Dispatch(void *Message)
{
	TForm::Dispatch(Message);
	if( hidden && ((PMessage)Message)->Msg == WM_SIZE )
	{
		TWindowState ws = WindowState;

		if( ws == wsMinimized )
		{
			Hide();
			Application->Minimize();
		}
	}
}
//---------------------------------------------------------------------------

