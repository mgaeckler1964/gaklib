/*
		Project:		GAKLIB
		Module:			dbConvertFrm.h
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

#ifndef dbConvertFrmH
#define dbConvertFrmH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

#include <iostream>

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <gak/string.h>
#include <gak/vcl_tools.h>

//---------------------------------------------------------------------------
class TDbConvertForm : public TForm
{
__published:	// IDE-managed Components
	TButton *UpgradeButton;
	TMemo *Memo;
	TButton *EmptyButton;
	TButton *SessionButton;
	TButton *BackupButton;
	TButton *RestoreButton;
	TButton *ButtonCheck;
	TButton *ButtonCreate;
	void __fastcall SessionButtonClick(TObject *Sender);
	void __fastcall BackupButtonClick(TObject *Sender);
	void __fastcall RestoreButtonClick(TObject *Sender);
	void __fastcall UpgradeButtonClick(TObject *Sender);
	void __fastcall EmptyButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonCheckClick(TObject *Sender);
	void __fastcall ButtonCreateClick(TObject *Sender);
private:	// User declarations
	void __fastcall DoLoginDialog( TDatabase *theDatabase, TStrings *LoginParams );
	void BackupRestoreDB( const gak::vcl::DBconnector &sourceDB, const gak::vcl::DBconnector &destDB );
	virtual const char *getAliasName( void ) const = 0;
	virtual gak::vcl::DatabaseSchema *createSchema( void ) const;

	static void showParams( const TDatabase *db, std::ostream &out );
protected:
	int						m_mainVersion, m_backupVersion;
	gak::vcl::DBconnector	m_mainDbConnector, m_oldDbConnector, m_backupDbConnector;
	gak::vcl::DBtype		m_mainDbType, m_oldDbType, m_backupDbType;
public:		// User declarations
	__fastcall TDbConvertForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDbConvertForm *DbConvertForm;
//---------------------------------------------------------------------------
#endif
