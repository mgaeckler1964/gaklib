/*
		Project:		GAKLIB
		Module:			StatusFrm.cpp
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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
	m_theThread = NULL;
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
	doEnterFunctionEx(gakLogging::llDetail, "TStatusForm::setStatus");
	doLogValueEx( gakLogging::llDetail, Handle );

	if( true || Visible )
	{
		try
		{
			STRING newCaption = verb + " " + status;

			StatusLabel->Caption = (const char *)newCaption;

			if( !m_theThread )
				idleLoop();

		}
		catch( ... )
		{
			// ignore
			doLogPosition();
		}
	}
	return m_theThread ? m_theThread->terminated : false;
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
				m_stack.push( oldCaption );
			setStatus( verb, status );
		}
		catch( ... )
		{
			// ignore
		}
	}

	return m_theThread ? m_theThread->terminated : false;
}
//---------------------------------------------------------------------------
bool TStatusForm::restore( void )
{
	if( m_stack.size() )
	{
		STRING newCaption = m_stack.pop();
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

	return m_theThread ? m_theThread->terminated : false;
}
//---------------------------------------------------------------------------
void __fastcall TStatusForm::FormCloseQuery(TObject *,
	  bool &CanClose)
{
	Thread::CheckThreadCount();
	CanClose = !m_theThread || !m_theThread->isRunning || m_theThread->m_threadFinished;
}
//---------------------------------------------------------------------------
void StatusThread::ExecuteThread( void )
{
	doEnterFunctionEx(gakLogging::llInfo, "StatusThread::ExecuteThread");
	try
	{
		perform();
		StatusForm->setStatus( "Finish", "" );
		m_threadFinished = true;
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
	m_stack.clear();

	// remove unused thread objects
	Thread::CheckThreadCount();
	m_theThread = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TStatusForm::BitBtnStopClick(TObject *)
{
	if( !m_theThread || !m_theThread->isRunning || m_theThread->m_threadFinished )
		Close();
	else if( m_confirm )
	{
		STRING	confirmMsg = "Stop ";
		confirmMsg += m_theThread->getTitle();
		if( Application->MessageBox( confirmMsg, "Stop", MB_YESNO|MB_ICONSTOP ) == IDYES )
			m_theThread->StopThread();
	}
	else
		m_theThread->StopThread();
}
//---------------------------------------------------------------------------

void __fastcall TStatusForm::CheckThread(TObject *Sender)
{
	bool	CanClose;

	if( !Thread::CheckThreadCount() )
		m_theThread = NULL;

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
	if( m_theThread )
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
	if( m_hidden && ((PMessage)Message)->Msg == WM_SIZE )
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
bool TStatusForm::waitForUserSleep( unsigned long timeOut )
{
	bool result = false;
	if( m_theThread )
	{
		unsigned long lastInput = gak::Thread::GetLastInputTime();
		while( lastInput < timeOut && !result)
		{
			IdleLabel->Caption = gak::STRING("Wait for system idle ") + gak::formatNumber((timeOut - lastInput + 500)/1000) + 's';
			result = m_theThread->waitForUserSleep( 1000 );
			lastInput = gak::Thread::GetLastInputTime();
		}
		IdleLabel->Caption = "";
	}

	return  result;
}
//---------------------------------------------------------------------------

