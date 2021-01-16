/*
		Project:		GAKLIB
		Module:			DirSelFrm.cpp
		Description:	
		Author:			Martin G�ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Germany, Munich ``AS IS''
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

#include <gak/string.h>
#include <gak/directory.h>

#pragma hdrstop

#include "DirSelFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDirectorySelector *DirectorySelector;
//---------------------------------------------------------------------------
__fastcall TDirectorySelector::TDirectorySelector(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDirectorySelector::ButtonOKClick(TObject *)
{
	if( EditMkdir->Visible )
	{
		if( EditMkdir->Text > "" )
		{
			AnsiString	newDirectory = LabelDirectory->Caption;
			newDirectory += DIRECTORY_DELIMITER;
			newDirectory += EditMkdir->Text;

			gak::makeDirectory( newDirectory.c_str() );

			DirectoryListBox->Update();
			DirectoryListBox->Directory = newDirectory;
		}

		EditMkdir->Visible = false;
	}
	else
		ModalResult = mrOk;

}
//---------------------------------------------------------------------------
void __fastcall TDirectorySelector::FormShow(TObject *)
{
	EditMkdir->Visible = false;
	EditMkdir->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TDirectorySelector::ButtonMkdirClick(TObject *)
{
	EditMkdir->Visible = true;
	EditMkdir->Text = "";
	EditMkdir->SetFocus();
}
//---------------------------------------------------------------------------

