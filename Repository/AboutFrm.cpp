/*
		Project:		GAKLIB
		Module:			AboutFrm.cpp
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

//---------------------------------------------------------------------
#include <stdio.h>

#include <vcl.h>

#include <gak/fmtNumber.h>

#pragma hdrstop

#include "AboutFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutProgramForm *AboutProgramForm;

//---------------------------------------------------------------------
__fastcall TAboutProgramForm::TAboutProgramForm(TComponent* AOwner)
	: TForm(AOwner), theVersionInfo( Application->ExeName.c_str() )
{
}
//---------------------------------------------------------------------

void __fastcall TAboutProgramForm::FormCreate(TObject *)
{
	doEnterFunctionEx(gakLogging::llInfo, "TAboutProgramForm::FormCreate");
	Caption  = (const char *)(
		getProductName() + " by " + getCompanyName()
	);
	ProductName->Caption = Application->Title;
	ProgramIcon->Picture-> Icon = Application->Icon;

	Version->Caption = (const char *)getVersionString();
	Copyright->Caption = (const char *)getLegalCopyRight();
	doLogValueEx(gakLogging::llDetail, getLegalCopyRight());
	Comments->Caption = (const char *)getFileDescription();
}
//---------------------------------------------------------------------------

