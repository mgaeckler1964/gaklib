/*
		Project:		GAKLIB
		Module:			UserDlg.h
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
#ifndef UserDlgH
#define UserDlgH
//----------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <Messages.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include <ComCtrls.hpp>

//----------------------------------------------------------------------------
class TUserDialog : public TForm
{
__published:
	TDBNavigator *DBNavigatorUser;
	TPanel *Panel1;
	TDataSource *DataSourceUser;
	TPanel *Panel2;
	TTable *TableUser;
	TDBGrid *DBGridUser;
	TBitBtn *BitBtnUsers;
	TBitBtn *BitBtnGroups;
	TDBLookupComboBox *DBLookupComboBoxDepartment;
	TDataSource *DataSourceGroups;
	TLabel *Label1;
	TDBEdit *DBEditUsername;
	TLabel *Label2;
	TPageControl *PageControl;
	TTabSheet *TabSheetUser;
	TTabSheet *TabSheetGroup;
	TDBEdit *DBEditLastName;
	TLabel *Label3;
	TGroupBox *GroupBoxPermissions;
	TCheckBox *CheckBoxLogin;
	TCheckBox *CheckBoxGroupAdmin;
	TCheckBox *CheckBoxUserAdmin;
	TCheckBox *CheckBoxSystemAdmin;
	TTable *TableMember;
	TQuery *QueryGroups;
	TQuery *QueryAll;
	TIntegerField *TableMemberGROUP_ID;
	TIntegerField *TableMemberMEMBER_ID;
	TDBGrid *DBGridMember;
	TDataSource *DataSourceMember;
	TDBNavigator *DBNavigatorMember;
	TStringField *TableMemberMEMBERNAME;
	TIntegerField *TableUserID;
	TStringField *TableUserUSERNAME;
	TIntegerField *TableUserTYPE;
	TStringField *TableUserLASTNAME;
	TStringField *TableUserFIRSTNAME;
	TIntegerField *TableUserDEPARTMENT;
	TIntegerField *TableUserPERMISSIONS;
	TDBEdit *DBEditFirstName;
	TLabel *Label4;
	TStringField *TableUserMD5_PASSWD;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DBGridUserDblClick(TObject *Sender);
	void __fastcall BitBtnUsersClick(TObject *Sender);
	void __fastcall BitBtnGroupsClick(TObject *Sender);
	void __fastcall TableUserAfterInsert(TDataSet *DataSet);
	void __fastcall TableUserAfterScroll(TDataSet *DataSet);
	void __fastcall StartEditMode(TObject *Sender);
	void __fastcall TableUserBeforePost(TDataSet *DataSet);
	void __fastcall TableUserAfterPost(TDataSet *DataSet);
	void __fastcall TableUserBeforeDelete(TDataSet *DataSet);
private:
	// private declarations
	int userType;
public:
	int selectedUser;
	// public declarations
	__fastcall TUserDialog(TComponent *Owner);
	int ShowModalWithDB( const AnsiString &database );
};
//----------------------------------------------------------------------------
extern TUserDialog *UserDialog;
//----------------------------------------------------------------------------
#endif
