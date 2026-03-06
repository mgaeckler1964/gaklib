/*
		Project:		GAKLIB
		Module:			StatusFrm.h
		Description:	The StatusForm with StatusThread
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#ifndef StatusFrmH
#define StatusFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#include <gak/thread.h>
#include <gak/Stack.h>
#include <gak/fmtNumber.h>

//---------------------------------------------------------------------------
#pragma option -RT-
class StatusThread;
#pragma option -RT.

class TStatusForm : public TForm
{
	virtual void __fastcall Dispatch(void *Message);

__published:	// IDE-managed Components
	TLabel *StatusLabel;
	TPanel *Panel1;
	TBitBtn *BitBtnStop;
	TLabel *IdleLabel;
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BitBtnStopClick(TObject *Sender);
	void __fastcall CheckThread(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations

	gak::SharedObjectPointer<StatusThread>	m_theThread;
	gak::Stack<gak::STRING>					m_stack;
	bool									m_confirm;
	bool									m_hidden;
	unsigned								m_counter;
	AnsiString								m_caption;

public:		// User declarations
	bool setStatus( const gak::STRING &verb, const gak::STRING &status );
	bool pushStatus( const gak::STRING &verb, const gak::STRING &status );
	bool restore();
	bool isTerminated();
	__fastcall TStatusForm(TComponent* Owner);
	virtual __fastcall ~TStatusForm();


	void setupWindow(
		const gak::SharedObjectPointer<StatusThread> &theThread,
		bool confirm=true, bool hidden=false
	)
	{
		m_stack.clear();
		m_theThread = theThread;
		m_confirm = confirm;
		m_hidden = hidden;
		m_counter = 0;
		m_caption = "";
	}
	void setCaption( const AnsiString &newCaption )
	{
		m_caption = newCaption;
		Caption = m_caption;
		StatusLabel->Caption = newCaption;
	}
	void incrementCounter()
	{
		++m_counter;
		AnsiString newCaption = m_caption +
			" (" + AnsiString(gak::formatNumber(m_counter)) + ')';
		Caption = newCaption;
	}

	void stopThread();
	bool waitForUserSleep( unsigned long timeOut );
};
//---------------------------------------------------------------------------
extern PACKAGE TStatusForm *StatusForm;
//---------------------------------------------------------------------------
#pragma option -RT-
class StatusThread : public gak::Thread
{
	public:
	bool m_threadFinished;

	private:
	virtual void perform() = 0;
	virtual void ExecuteThread();

	public:
	virtual const char *getTitle() const = 0;
	void StartThread( bool confirm=true, bool hidden=false )
	{
		AnsiString newCaption = getTitle();
		newCaption += " in progress...";
		StatusForm->setCaption( newCaption );

		gak::Thread::StartThread();
		StatusForm->setupWindow( this, confirm, hidden );
		if( !hidden )
		{
			StatusForm->ShowModal();
		}
	}
	StatusThread()
	{
		m_threadFinished = false;
	}

};
#pragma option -RT.

//---------------------------------------------------------------------------
inline bool TStatusForm::isTerminated()
{
//	doEnterFunctionEx(gakLogging::llDetail, "TStatusForm::isTerminated");
	return m_theThread ? m_theThread->terminated : false;
}
//---------------------------------------------------------------------------
inline void TStatusForm::stopThread()
{
	doEnterFunction("TStatusForm::stopThread");

	if( m_theThread )
	{
		m_theThread->StopThread();
	}
}
//---------------------------------------------------------------------------
#endif
