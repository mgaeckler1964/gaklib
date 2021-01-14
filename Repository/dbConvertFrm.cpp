/*
		Project:		GAKLIB
		Module:			dbConvertFrm.cpp
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

#include <iostream>
#include <cstring>

#include <vcl.h>

#include <gak/array.h>
#include <gak/string.h>
#include <gak/directory.h>
#include <gak/vcl_tools.h>
#include <gak/stringStream.h>

#pragma hdrstop

#include "StatusFrm.h"
#include "LoginDlg.h"
#include "dbConvertFrm.h"
#include "config.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDbConvertForm *DbConvertForm;
//---------------------------------------------------------------------------
#pragma option -RT-
class DBlogger : public ProgressLoger
{
	private:
	TStatusForm *StatusForm;

	public:
	DBlogger( std::ostream &out ) : ProgressLoger( out )
	{
		StatusForm = new TStatusForm( NULL );
	}
	~DBlogger()
	{
		StatusForm->Close();
		delete StatusForm;
	}
	virtual void showProgress( void ) const;
};
#pragma option -RT+
//---------------------------------------------------------------------------
void DBlogger::showProgress( void ) const
{
	if( !StatusForm->Visible )
	{
		StatusForm->Show();
	}

	STRING	status = tableName + " (" +
		formatNumber( recordCount ) +
		" Total:" + formatNumber( totalRecordCount ) + ')'
	; 

	StatusForm->setStatus( "Copy", status );
}
//---------------------------------------------------------------------------
static DBtype dbTypeForDriver( const CI_STRING &driverName )
{
	if( driverName == "ORACLE" )
		return dbtORACLE;
	if( driverName == "INTERBASE" )
		return dbtINTERBASE;
	if( driverName == "STANDARD" )
		return dbtPARADOX;

	return dbtOTHER;

}
//---------------------------------------------------------------------------
__fastcall TDbConvertForm::TDbConvertForm(TComponent* Owner)
	: TForm(Owner)
{
	m_mainVersion = m_backupVersion = -1;
}

//---------------------------------------------------------------------------
void __fastcall TDbConvertForm::SessionButtonClick(TObject *)
{
	TQuery	*delSessionQuery = new TQuery( NULL );
	TDatabase *theDatabase = m_mainDbConnector.connectDB( true );

	delSessionQuery->DatabaseName = theDatabase->DatabaseName;;
	delSessionQuery->SQL->Add(
		"delete from CONFIG "
		"where ENTRY_NAME like 'session%' "
		"or ENTRY_NAME like 'lock%'"
	);

	delSessionQuery->ExecSQL();

	delete delSessionQuery;

	theDatabase->Close();
	delete theDatabase;

	SessionButton->Caption = "Finish";
	SessionButton->Enabled = false;
}
//---------------------------------------------------------------------------
DatabaseSchema *TDbConvertForm::createSchema( void ) const
{
	return new DatabaseSchema();
}
//---------------------------------------------------------------------------
void TDbConvertForm::showParams( const TDatabase *db, std::ostream &out )
{
	TStrings *params = db->Params;

	for( int i=0; i<params->Count; ++i )
	{
		out << params->Strings[i] << std::endl;
	}

	DBDesc databaseInfo;
	std::memset( &databaseInfo, 0, sizeof(databaseInfo) );
	const DBIResult result = DbiGetDatabaseDesc(
		const_cast<char*>(db->AliasName.c_str()), &databaseInfo
	);
	if( result == DBIERR_NONE )
	{
		out << "Name: " << databaseInfo.szName << std::endl;
		out << "Descr: " << databaseInfo.szText << std::endl;
		out << "Physical: " << databaseInfo.szPhyName << std::endl;
		out << "Type: " << databaseInfo.szDbType << std::endl;
	}
}
//---------------------------------------------------------------------------
void TDbConvertForm::BackupRestoreDB(
	const DBconnector &sourceDB, const DBconnector &destDB
)
{
	doEnterFunction("TDbConvertForm::BackupRestoreDB");

	STRING			log;
	oSTRINGstream	out( log );
	DBlogger		progress( out );
	int				srcVersion = 0;
	int				destVersion = 0;
	size_t			recordCount;

	DatabaseSchema *sourceSchema = createSchema();
	DatabaseSchema *destSchema = createSchema();

	TDatabase *source = NULL;
	TDatabase *destination = NULL;

	progress.showProgress();

	try
	{
		out << "Copying tables\n";

		source = sourceDB.connectDB( true );
		destination = destDB.connectDB( true );

		out << "Source\n";
		showParams( source, out );
		out << "Destination\n";
		showParams( destination, out );

		srcVersion = ConfigDataModule->GetDBVersion( source );
		destVersion = ConfigDataModule->GetDBVersion( destination );

		sourceSchema->readSchema( source );
		destSchema->readSchema( destination );

		sourceSchema->setDbVersion( srcVersion );
		destSchema->setDbVersion( destVersion );
#if 0
		try
		{
			sourceSchema->upgradeSchema( destSchema );
		}
		catch( ... )
		{
		}
		destSchema->readSchema( destination );
#endif

		STRING	xmlFile = makeFullPath(
			STRING( Application->ExeName.c_str() ),
			m_mainDbConnector.m_aliasName + ".XML"
		);
		if( exists( xmlFile ) )
		{
			DatabaseSchema	*masterSchema = createSchema();
			masterSchema->readSchemaXml( xmlFile );
			STRING compareResult = destSchema->compareSchema( masterSchema );
			delete masterSchema;
			if( !compareResult.isEmpty() )
			{
				STRING	error = "Database check failed for ";
				error += destDB.m_aliasName;
				error += ":\n";
				error += compareResult;
/*@*/			throw Exception( (const char *)error );
			}
		}

		recordCount = destSchema->copyTables( sourceSchema, &progress );

		destination->Close();
		source->Close();

		delete destination;
		delete source;
	}
	catch( ... )
	{
		if( destVersion > 0 )
		{
			ConfigDataModule->SetDBVersion( destination, destVersion );
		}

		if( destination )
		{
			destination->Close();
			delete destination;
		}

		if( source )
		{
			source->Close();
			delete source;
		}

/*@*/	throw;
	}

	progress.showProgress();

	progress << recordCount << " records copied\n";
	out.flush();

	setMemoText( Memo, log );
}

//---------------------------------------------------------------------------
void __fastcall TDbConvertForm::BackupButtonClick(TObject *)
{
	BackupRestoreDB( m_mainDbConnector, m_backupDbConnector );
	BackupButton->Caption = "Finish";
}
//---------------------------------------------------------------------------

void __fastcall TDbConvertForm::RestoreButtonClick(TObject *)
{
	BackupRestoreDB( m_backupDbConnector, m_mainDbConnector );
	RestoreButton->Caption = "Finish";
}
//---------------------------------------------------------------------------
void __fastcall TDbConvertForm::UpgradeButtonClick(TObject *)
{
	TDatabase		*theDatabase = new TDatabase( NULL );

	theDatabase->AliasName = static_cast<const char *>(m_mainDbConnector.m_aliasName);
	theDatabase->DatabaseName = "theDatabaseDB";
	theDatabase->Exclusive = true;

	theDatabase->Open();
	int dbVersion = ConfigDataModule->GetDBVersion( theDatabase );
	theDatabase->Close();

	try
	{
		BackupRestoreDB( m_oldDbConnector, m_mainDbConnector );
	}
	__finally
	{
		theDatabase->Open();
		ConfigDataModule->SetDBVersion( theDatabase, dbVersion );
		theDatabase->Close();

		delete theDatabase;

		UpgradeButton->Caption = "Finish";
		UpgradeButton->Enabled = false;
	}
}
//---------------------------------------------------------------------------


void __fastcall TDbConvertForm::EmptyButtonClick(TObject *)
{
	if(
		Application->MessageBox(
			"Do you really want to empty your main database?",
			Application->Title.c_str(),
			MB_YESNO|MB_ICONQUESTION
		) == IDYES
	)
	{
		TDatabase *theDatabase = m_mainDbConnector.connectDB( true );

		ConfigDataModule->emptyTables( theDatabase );

		theDatabase->Close();

		delete theDatabase;

		EmptyButton->Caption = "Finish";
	}
}
//---------------------------------------------------------------------------
void __fastcall TDbConvertForm::DoLoginDialog( TDatabase *theDatabase, TStrings *LoginParams )
{
	AnsiString	caption = "Login ";
	caption += theDatabase->AliasName;

	LoginForm->Caption = caption;
	LoginForm->EditUserName->Text = theDatabase->Params->Values["USER NAME"];
	if( LoginForm->ShowModal( theDatabase->AliasName.c_str() ) == mrOk )
	{
		LoginParams->Values["USERNAME"] = LoginForm->EditUserName->Text;
		LoginParams->Values["PASSWORD"] = LoginForm->EditPassword->Text;

		if( m_mainDbConnector.m_aliasName == theDatabase->AliasName.c_str() )
		{
			m_mainDbConnector.m_userName = LoginForm->EditUserName->Text.c_str();
			m_mainDbConnector.m_password = LoginForm->EditPassword->Text.c_str();
		}
		else if( m_oldDbConnector.m_aliasName == theDatabase->AliasName.c_str() )
		{
			m_oldDbConnector.m_userName = LoginForm->EditUserName->Text.c_str();
			m_oldDbConnector.m_password = LoginForm->EditPassword->Text.c_str();
		}
		else if( m_backupDbConnector.m_aliasName == theDatabase->AliasName.c_str() )
		{
			m_backupDbConnector.m_userName = LoginForm->EditUserName->Text.c_str();
			m_backupDbConnector.m_password = LoginForm->EditPassword->Text.c_str();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TDbConvertForm::FormShow(TObject *)
{
	doEnterFunction("TDbConvertForm::FormShow");

	STRING		line;
	int			oldVersion;
	TDatabase	*theDatabase = new TDatabase( NULL );
	bool		createDB, createBackupDB;

	oldVersion = m_backupVersion = m_mainVersion = -1;

	m_mainDbConnector.m_aliasName = getAliasName();
	m_oldDbConnector.m_aliasName = m_mainDbConnector.m_aliasName + "_OLD";
	m_backupDbConnector.m_aliasName = m_mainDbConnector.m_aliasName + "_BACKUP";

	createDB = createBackupDB = false;

	Memo->Lines->Clear();

	Session->Open();

	theDatabase->DatabaseName = "theDatabaseDB";
	theDatabase->OnLogin = DoLoginDialog;

	if( Session->IsAlias( static_cast<const char *>(m_oldDbConnector.m_aliasName)) )
	{
		theDatabase->AliasName = static_cast<const char *>(m_oldDbConnector.m_aliasName);
		theDatabase->Open();
		m_oldDbType = dbTypeForDriver(
			Session->GetAliasDriverName(theDatabase->AliasName).c_str()
		);

		oldVersion = ConfigDataModule->GetDBVersion( theDatabase );
		theDatabase->Close();

		doLogValue( oldVersion );
		if( oldVersion < 0 )	// DB exists but no entry => assume 0
		{
			doLogPosition();
			oldVersion = 0;
		}
	}
	if( Session->IsAlias( static_cast<const char *>(m_mainDbConnector.m_aliasName)) )
	{
		theDatabase->AliasName = static_cast<const char *>(m_mainDbConnector.m_aliasName);
		theDatabase->Open();
		m_mainDbType = dbTypeForDriver(
			Session->GetAliasDriverName(theDatabase->AliasName).c_str()
		);
		m_mainVersion = ConfigDataModule->GetDBVersion( theDatabase );
		theDatabase->Close();
		if( m_mainVersion < 0 )
		{
			createDB = true;
		}

		doLogValue( mainVersion );
	}
	if( Session->IsAlias( static_cast<const char *>(m_backupDbConnector.m_aliasName)) )
	{
		theDatabase->AliasName = static_cast<const char *>(m_backupDbConnector.m_aliasName);
		theDatabase->Open();
		m_backupDbType = dbTypeForDriver(
			Session->GetAliasDriverName(theDatabase->AliasName).c_str()
		);
		m_backupVersion = ConfigDataModule->GetDBVersion( theDatabase );
		theDatabase->Close();
		if( m_backupVersion < 0 )
		{
			createBackupDB = true;
		}
	}

	if( createDB || createBackupDB )
	{
		line = "Click on \"Create\" to create the schema on ";
		if( createDB )
			line += m_mainDbConnector.m_aliasName;
		if( createDB && createBackupDB )
			line += " and ";
		if( createBackupDB )
			line += m_backupDbConnector.m_aliasName;
		line += " database";
		if( createDB && createBackupDB )
			line += 's';
		line += '.';
		Memo->Lines->Add( static_cast<const char *>(line)  );
		Memo->Lines->Add( "" );

		ButtonCreate->Enabled = true;
	}
	else
		ButtonCreate->Enabled = false;

	if( m_mainVersion >= 0 )
	{
		const STRING	xmlFile = makeFullPath(
			STRING( Application->ExeName.c_str() ),
			m_mainDbConnector.m_aliasName + ".XML"
		);
		if( exists( xmlFile ) )
		{
			line = "Click on \"Check\" to test the schema of ";
			line += m_mainDbConnector.m_aliasName;
			if( m_backupVersion >= 0 )
			{
				line += " and ";
				line += m_backupDbConnector.m_aliasName;
			}

			line += " database";
			if( m_backupVersion >= 0 )
			{
				line += 's';
			}
			line += '.';

			Memo->Lines->Add( static_cast<const char *>(line)  );
			Memo->Lines->Add( "" );
			ButtonCheck->Enabled = true;
		}
		else
			ButtonCheck->Enabled = false;

		const CI_STRING cmd = GetCommandLine();
		if( cmd.endsWith( "UPGRADE" ) 
		||	(oldVersion >= 0 && m_mainVersion > oldVersion)
		)
		{
			line = "Click on \"Upgrade\" to copy and convert your data from ";
			line += m_oldDbConnector.m_aliasName;
			line += " to ";
			line += m_mainDbConnector.m_aliasName;
			line += " databases.";
			Memo->Lines->Add( static_cast<const char *>(line)  );
			Memo->Lines->Add( ""  );

			UpgradeButton->Enabled = true;
		}
		else
			UpgradeButton->Enabled = false;

		line = "Click on \"Empty\" to remove all data from ";
		line += m_mainDbConnector.m_aliasName;
		line += " database.";
		Memo->Lines->Add( static_cast<const char *>(line)  );
		Memo->Lines->Add( "" );

		EmptyButton->Enabled = true;

	}
	else
	{
		EmptyButton->Enabled = false;
		ButtonCheck->Enabled = false;
		UpgradeButton->Enabled = false;
	}


	if( m_mainVersion >= 0 && m_backupVersion >= 0 && m_mainVersion == m_backupVersion )
	{
		line = "Click on \"Backup\" to backup your data from ";
		line += m_mainDbConnector.m_aliasName;
		line += " to ";
		line += m_backupDbConnector.m_aliasName;
		line += " databases.";
		Memo->Lines->Add( static_cast<const char *>(line)  );
		Memo->Lines->Add( ""  );

		line = "Click on \"Restore\" to restore your data from ";
		line += m_backupDbConnector.m_aliasName;
		line += " to ";
		line += m_mainDbConnector.m_aliasName;
		line += " databases.";
		Memo->Lines->Add( static_cast<const char *>(line)  );
		Memo->Lines->Add( ""  );

		BackupButton->Enabled = true;
		RestoreButton->Enabled = true;
	}
	else
	{
		BackupButton->Enabled = false;
		RestoreButton->Enabled = false;
	}

	if( m_mainVersion >= 0 && SessionButton->Enabled && SessionButton->Visible )
	{
		line = "Click on \"Unlock\" to unlock your data from ";
		line += m_mainDbConnector.m_aliasName;
		line += " database.";
		Memo->Lines->Add( static_cast<const char *>(line)  );
		Memo->Lines->Add( ""  );
	}
	else
		SessionButton->Enabled = false;


	Memo->Lines->Add(
		"Use BDE-Administration to check whether required databases exist."
	);

	delete theDatabase;
}
//---------------------------------------------------------------------------

void __fastcall TDbConvertForm::ButtonCheckClick(TObject *)
{
	doEnterFunction("TDbConvertForm::ButtonCheckClick");
	STRING	compareResult, currentResult;

	DatabaseSchema	*masterSchema = createSchema();

	STRING	xmlFile = makeFullPath(
		STRING( Application->ExeName.c_str() ),
		m_mainDbConnector.m_aliasName + ".XML"
	);
	masterSchema->readSchemaXml( xmlFile );

	DatabaseSchema	*currentSchema = createSchema();
	try
	{
		currentSchema->readSchema( m_mainDbConnector );

		currentResult = currentSchema->compareSchema( masterSchema );
		if( currentResult.isEmpty() )
			currentResult = "No errors";
		else
		{
			const char *tmpEnv = getenv( "TEMP" );
			if( tmpEnv )
			{
				STRING fileName = tmpEnv;
				if( !fileName.endsWith( DIRECTORY_DELIMITER ) )
					fileName += DIRECTORY_DELIMITER;
				fileName += m_mainDbConnector.m_aliasName;
				fileName += ".xml";

				xml::Element *schemaXML = currentSchema->getSchema();
				STRING schemaStr = schemaXML->generateDoc();
				schemaStr.writeToFile( fileName );
				delete schemaXML;
			}
		}
	}
	catch( Exception &e )
	{
		currentResult = e.Message.c_str();
	}
	catch( ... )
	{
		currentResult = "unknown error";
	}

	compareResult = m_mainDbConnector.m_aliasName + ":\n" + currentResult;
	delete currentSchema;

	if( BackupButton->Enabled )
	{
		STRING			backupResult;
		DatabaseSchema	*backupSchema = createSchema();
		try
		{
			doLogPosition();
			backupSchema->readSchema( m_backupDbConnector );
			doLogPosition();

			backupResult = backupSchema->compareSchema( masterSchema );
			doLogPosition();
			if( backupResult.isEmpty() )
				backupResult = "No errors";
			else
			{
				const char *tmpEnv = getenv( "TEMP" );
				if( tmpEnv )
				{
					STRING fileName = tmpEnv;
					if( !fileName.endsWith( DIRECTORY_DELIMITER ) )
						fileName += DIRECTORY_DELIMITER;
					fileName += m_backupDbConnector.m_aliasName;
					fileName += ".xml";

					xml::Element *schemaXML = backupSchema->getSchema();
					STRING schemaStr = schemaXML->generateDoc();
					schemaStr.writeToFile( fileName );
					delete schemaXML;
				}
			}
			doLogPosition();

		}
		catch( Exception &e )
		{
			backupResult = e.Message.c_str();
		}
		catch( ... )
		{
			backupResult = "unknown error";
		}

		compareResult += "\n\n" +
			m_backupDbConnector.m_aliasName + ":\n" +
			backupResult
		;


		delete backupSchema;
	}

	delete masterSchema;

	setMemoText( Memo, compareResult );
}
//---------------------------------------------------------------------------

void __fastcall TDbConvertForm::ButtonCreateClick(TObject *)
{
	STRING	xmlFile = makeFullPath(
		STRING( Application->ExeName.c_str() ),
		m_mainDbConnector.m_aliasName + ".XML"
	);
	if( exists( xmlFile ) )
	{
		DatabaseSchema	*masterSchema = createSchema();
		masterSchema->readSchemaXml( xmlFile );

		if( Session->IsAlias( static_cast<const char *>(m_backupDbConnector.m_aliasName) )
		&&  m_backupVersion < 0 )
		{
			masterSchema->createDropScript( m_backupDbConnector, m_backupDbType );
			masterSchema->createSynonymScript( m_backupDbConnector, m_backupDbType );
			masterSchema->createSchema( m_backupDbConnector, m_backupDbType );
		}

		if( Session->IsAlias( static_cast<const char *>(m_mainDbConnector.m_aliasName) )
		&&  m_mainVersion < 0 )
		{
			masterSchema->createDropScript( m_mainDbConnector, m_mainDbType );
			masterSchema->createSynonymScript( m_mainDbConnector, m_mainDbType );
			masterSchema->createSchema( m_mainDbConnector, m_mainDbType );
		}

		delete masterSchema;
	}
	ButtonCreate->Caption = "Finish";
	ButtonCreate->Enabled = false;
}
//---------------------------------------------------------------------------

