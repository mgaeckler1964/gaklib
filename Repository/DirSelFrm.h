/*
		Project:		GAKLIB
		Module:			DirSelFrm.h
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

#ifndef DirSelFrmH
#define DirSelFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
#pragma option -w-hid
class TDirectorySelector : public TForm
{
__published:	// IDE-managed Components
	TDirectoryListBox *DirectoryListBox;
	TDriveComboBox *DriveComboBox;
	TLabel *LabelDirectory;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TEdit *EditMkdir;
	TButton *ButtonMkdir;
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonMkdirClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TDirectorySelector(TComponent* Owner);
	int ShowModal( const AnsiString &directory )
	{
		if( directory > "" )
			DirectoryListBox->Directory = directory;

		return TForm::ShowModal();
	}
};
#pragma option -w+hid
//---------------------------------------------------------------------------
extern PACKAGE TDirectorySelector *DirectorySelector;
//---------------------------------------------------------------------------
#endif
