/*
		Project:		GAKLIB
		Module:			StatusFrm.h
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

	gak::SharedObjectPointer<StatusThread>	theThread;
	gak::Stack<gak::STRING>					stack;
	bool									confirm;
	bool									hidden;

public:		// User declarations
	bool setStatus( const gak::STRING &verb, const gak::STRING &status );
	bool pushStatus( const gak::STRING &verb, const gak::STRING &status );
	bool restore( void );
	bool isTerminated( void );
	__fastcall TStatusForm(TComponent* Owner);
	virtual __fastcall ~TStatusForm();


	void setupWindow(
		const gak::SharedObjectPointer<StatusThread> &theThread,
		bool confirm=true, bool hidden=false
	)
	{
		stack.clear();
		this->theThread = theThread;
		this->confirm = confirm;
		this->hidden = hidden;
	}

	void stopThread( void );
	bool waitForUserSleep( unsigned long timeOut );
};
//---------------------------------------------------------------------------
extern PACKAGE TStatusForm *StatusForm;
//---------------------------------------------------------------------------
#pragma option -RT-
class StatusThread : public gak::Thread
{
	public:
	bool threadFinished;

	private:
	virtual void perform( void ) = 0;
	virtual void ExecuteThread( void );

	public:
	virtual const char *getTitle( void ) const = 0;
	void StartThread( bool confirm=true, bool hidden=false )
	{
		AnsiString newCaption = getTitle();
		newCaption += " in progress...";
		StatusForm->Caption = newCaption;
		StatusForm->StatusLabel->Caption = newCaption;

		gak::Thread::StartThread();
		StatusForm->setupWindow( this, confirm, hidden );
		if( !hidden )
		{
			StatusForm->ShowModal();
		}
	}
	StatusThread()
	{
		threadFinished = false;
	}

};
#pragma option -RT.

//---------------------------------------------------------------------------
inline bool TStatusForm::isTerminated( void )
{
	doEnterFunctionEx(gakLogging::llDetail, "TStatusForm::isTerminated");
	return theThread ? theThread->terminated : false;
}
//---------------------------------------------------------------------------
inline void TStatusForm::stopThread( void )
{
	doEnterFunction("TStatusForm::stopThread");

	if( theThread )
	{
		theThread->StopThread();
	}
}
//---------------------------------------------------------------------------
#endif
