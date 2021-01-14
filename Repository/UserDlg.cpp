/*
		Project:		GAKLIB
		Module:			UserDlg.cpp
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

//----------------------------------------------------------------------------
#include <vcl.h>

#include <gak/vcl_tools.h>

#pragma hdrstop

#include "UserDlg.h"

//----------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//----------------------------------------------------------------------------
#pragma resource "*.dfm"
TUserDialog *UserDialog;

__fastcall TUserDialog::TUserDialog(TComponent *Owner)
	: TForm(Owner)
{
}
//----------------------------------------------------------------------------
void __fastcall TUserDialog::FormShow(TObject *Sender)
{
	selectedUser = 0;
	QueryAll->Open();
	QueryGroups->Open();
	BitBtnUsersClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TUserDialog::FormClose(TObject *, TCloseAction &)
{
	TableMember->Close();
	TableUser->Close();
	QueryGroups->Close();
	QueryAll->Close();
}
//---------------------------------------------------------------------------
void __fastcall TUserDialog::DBGridUserDblClick(TObject *)
{
	selectedUser = TableUserID->AsInteger;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
int TUserDialog::ShowModalWithDB( const AnsiString &dbName )
{
	TableUser->DatabaseName = dbName;
	TableMember->DatabaseName = dbName;

	QueryGroups->DatabaseName = dbName;
	QueryAll->DatabaseName = dbName;

	return TForm::ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::BitBtnUsersClick(TObject *)
{
	userType=0;
	TableMember->Close();
	TableUser->Close();

	TableUser->Filter = "TYPE=0";
	TableUser->Filtered = true;
	if( canEditUsers() )
	{
		TableUser->ReadOnly = false;
		CheckBoxLogin->Enabled = true;
	}
	else
	{
		TableUser->ReadOnly = true;
		CheckBoxLogin->Enabled = false;
	}
	if( isSystemAdmin() )
	{
		CheckBoxGroupAdmin->Enabled = true;
		CheckBoxUserAdmin->Enabled = true;
		CheckBoxSystemAdmin->Enabled = true;
	}
	else
	{
		CheckBoxGroupAdmin->Enabled = false;
		CheckBoxUserAdmin->Enabled = false;
		CheckBoxSystemAdmin->Enabled = false;
	}

	TableUser->Open();
	TabSheetUser->TabVisible = true;
	PageControl->ActivePage = TabSheetUser;
	TabSheetGroup->TabVisible = false;
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::BitBtnGroupsClick(TObject *)
{
	userType=1;
	TableMember->Close();
	TableUser->Close();

	TableUser->Filter = "TYPE=1";
	TableUser->Filtered = true;
	if( canEditGroups() )
	{
		TableUser->ReadOnly = false;
		TableMember->ReadOnly = false;
	}
	else
	{
		TableUser->ReadOnly = true;
		TableMember->ReadOnly = true;
	}
	TableUser->Open();
	TableMember->Open();

	TabSheetGroup->TabVisible = true;
	PageControl->ActivePage = TabSheetGroup;
	TabSheetUser->TabVisible = false;
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::TableUserAfterInsert(TDataSet *)
{
	setNewMaxValue( TableUser, "ID" );
	TableUserTYPE->AsInteger = userType;
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::TableUserAfterScroll(TDataSet *)
{
	int perms = TableUserPERMISSIONS->AsInteger;

	CheckBoxLogin->Checked = perms & USER_LOGIN_PERM ? true : false;
	CheckBoxGroupAdmin->Checked = perms & USER_GROUP_PERM ? true : false;
	CheckBoxUserAdmin->Checked = perms & USER_USER_PERM ? true : false;
	CheckBoxSystemAdmin->Checked = perms & USER_SYSTEM_PERM ? true : false;

	if( TableUser->State == dsEdit )
		TableUser->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::StartEditMode(TObject *)
{
	if( !TableUser->ReadOnly && TableUser->State == dsBrowse )
		TableUser->Edit();
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::TableUserBeforePost(TDataSet *)
{
	int perms = 0;
	if( CheckBoxLogin->Checked )
		perms |= USER_LOGIN_PERM;
	if( CheckBoxGroupAdmin->Checked )
		perms |= USER_GROUP_PERM;
	if( CheckBoxUserAdmin->Checked )
		perms |= USER_USER_PERM;
	if( CheckBoxSystemAdmin->Checked )
		perms |= USER_SYSTEM_PERM;

	TableUserPERMISSIONS->AsInteger = perms;
}
//---------------------------------------------------------------------------

void __fastcall TUserDialog::TableUserAfterPost(TDataSet *)
{
	if( TableUserTYPE->AsInteger == 1 )
	{
		QueryGroups->Close();
		QueryGroups->Open();
	}
	QueryAll->Close();
	QueryAll->Open();
}
//---------------------------------------------------------------------------


void __fastcall TUserDialog::TableUserBeforeDelete(TDataSet *)
{
	if( !isSystemAdmin()
	&& TableUserTYPE->AsInteger == 0
	&& TableUserPERMISSIONS->AsInteger & USER_SYSTEM_PERM )
	{
		throw Exception( "Permission Denied" );
	}
}
//---------------------------------------------------------------------------


