/*
		Project:		GAKLIB
		Module:			Config.cpp
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

#include <memory>

#include <vcl.h>

#include <gak/vcl_tools.h>

#pragma hdrstop

#include "Config.h"
//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfigDataModule *ConfigDataModule;
//---------------------------------------------------------------------------
__fastcall TConfigDataModule::TConfigDataModule(TComponent* Owner)
	: TDataModule(Owner)
{
	lockCount = 0;
}
//---------------------------------------------------------------------------
void TConfigDataModule::LockTable( void )
{
	doEnterFunction("TConfigDataModule::LockTable");

	int		lockValue, myProcessId = (int)GetCurrentProcessId();

	for( int i=10; i>0; i-- )
	{
		lockValue = GetValue( "lock", 0 );
		if( lockValue == myProcessId )
		{
			lockCount++;
			break;
		}
		else if( !lockValue )
			SetValue( "lock", myProcessId );
		else
			Sleep( 1000 );
	}

	if( lockValue != myProcessId )
	{
/*@*/	throw Exception( "Datensperrung nicht erfolgreich!" );
	}
}

void TConfigDataModule::UnlockTable( void )
{
	doEnterFunction("TConfigDataModule::UnockTable");

	if( !--lockCount )
		DeleteValue( "lock" );
}

//---------------------------------------------------------------------------

void TConfigDataModule::DeleteValue( const char *name )
{
	deleteSQL->Params->Items[0]->AsString = name;
	deleteSQL->ExecSQL();
}

//---------------------------------------------------------------------------

STRING TConfigDataModule::GetValue( const char *name, const char *defaultVal )
{
	doEnterFunction("TConfigDataModule::GetValue(const char *)");
	STRING	valueFound;

	try
	{

		selectStringSQL->Params->Items[0]->AsString = name;

		selectStringSQL->Open();
		if( !selectStringSQL->Eof )
		{
			valueFound = selectStringSQL->Fields->Fields[0]->AsString.c_str();
			doLogValue( valueFound );
		}
		else
		{
			valueFound = defaultVal;
			doLogPosition();
		}

		selectStringSQL->Close();
	}
	catch( Exception &e )
	{
		doLogValue( e.Message.c_str() );
		valueFound = defaultVal;
	}
	catch( std::exception &e )
	{
		doLogValue( e.what() );
		valueFound = defaultVal;
	}
	catch( ... )
	{
		doLogPosition();
		valueFound = defaultVal;
	}
	return valueFound;
}

//---------------------------------------------------------------------------

void TConfigDataModule::SetValue( const char *name, const char *defaultVal )
{
	updateStringSQL->Params->Items[0]->AsString = defaultVal;
	updateStringSQL->Params->Items[1]->AsString = name;

	updateStringSQL->ExecSQL();

	if( !updateStringSQL->RowsAffected )
	{
		insertStringSQL->Params->Items[0]->AsString = name;
		insertStringSQL->Params->Items[1]->AsString = defaultVal;

		insertStringSQL->ExecSQL();
	}
}

//---------------------------------------------------------------------------

int TConfigDataModule::GetValue( const char *name, int defaultVal )
{
	doEnterFunction("TConfigDataModule::GetValue(int)");
	int		valueFound = defaultVal;

	try
	{
		doLogValue(name);
		selectIntSQL->Params->Items[0]->AsString = name;
		doLogPosition();
		selectIntSQL->Open();
		doLogPosition();
		if( !selectIntSQL->Eof )
		{
			doLogPosition();
			valueFound = selectIntSQL->Fields->Fields[0]->AsInteger;
			doLogValue( valueFound );
		}

		doLogPosition();
		selectIntSQL->Close();
		doLogPosition();
	}
	catch( Exception &e )
	{
		doLogValue( e.Message.c_str() );
	}
	catch( std::exception &e )
	{
		doLogValue( e.what() );
	}
	catch( ... )
	{
		doLogPosition();
	}

	return valueFound;
}

//---------------------------------------------------------------------------

void TConfigDataModule::SetValue( const char *name, int defaultVal )
{
	doEnterFunction("TConfigDataModule::SetValue(int)");

	updateIntSQL->Params->Items[0]->AsInteger = defaultVal;
	updateIntSQL->Params->Items[1]->AsString = name;

	updateIntSQL->ExecSQL();

	if( !updateIntSQL->RowsAffected )
	{
		insertIntSQL->Params->Items[0]->AsString = name;
		insertIntSQL->Params->Items[1]->AsInteger = defaultVal;

		insertIntSQL->ExecSQL();
	}
}

//---------------------------------------------------------------------------

int TConfigDataModule::GetDBVersionByPath( const char *path )
{
	doEnterFunction("TConfigDataModule::GetDBVersionByPath");
	int	dbVersion = 0;

	doLogValue( path );
	try
	{
		std::auto_ptr<TDatabase> tmpDB( createDatabase( path ) );

		setDatabase( tmpDB.get() );
		dbVersion = GetDBVersion();
		setDatabase( NULL );

		tmpDB->Close();

		if( Session->IsAlias( "TMP_ALIAS" ) )
		{
			Session->DeleteAlias( "TMP_ALIAS" );
		}
	}
	catch( ... )
	{
		doLogPosition();
		setDatabase( NULL );
		if( Session->IsAlias( "TMP_ALIAS" ) )
		{
			Session->DeleteAlias( "TMP_ALIAS" );
		}
/*@*/	throw;
	}
	return dbVersion;
}

//---------------------------------------------------------------------------

int TConfigDataModule::GetDBVersionByAlias( const char *alias )
{
	doEnterFunction("TConfigDataModule::GetDBVersionByAlias");
	int	dbVersion = 0;

	doLogValue( alias );
	try
	{
		std::auto_ptr<TDatabase> tmpDB( new TDatabase(NULL) );

		tmpDB->DatabaseName = alias;
		tmpDB->AliasName = alias;
		tmpDB->Open();

		setDatabase( tmpDB.get() );
		dbVersion = GetDBVersion();
		setDatabase( NULL );

		tmpDB->Close();
	}
	catch( ... )
	{
		doLogPosition();
		setDatabase( NULL );
/*@*/	throw;
	}
	return dbVersion;
}

//---------------------------------------------------------------------------

STRING TConfigDataModule::OpenDatabase( TDatabase *theDatabase )
{
	STRING errorText;

	setDatabase( theDatabase );
	for( int i=0; i<10; i++ )
	{
		try
		{
			theDatabase->Open();
			errorText = "";
			break;
		}
		catch( Exception &e )
		{
			errorText = e.Message.c_str();
		}
		catch( ... )
		{
			errorText = "Unknown";
		}
		Sleep( 5000 );
	}

	return errorText;
}

//---------------------------------------------------------------------------

void TConfigDataModule::emptyTables( const STRING &directory )
{
	try
	{
		DatabaseSchema	theSchema;

		std::auto_ptr<TDatabase> theDatabase( createDatabase( directory ) );

		emptyTables( theDatabase.get() );

		theDatabase->Close();
		setDatabase( NULL );
	}
	catch( ... )
	{
		setDatabase( NULL );
/*@*/	throw;
	}
}

void TConfigDataModule::emptyTables( TDatabase *theDatabase )
{
	setDatabase( theDatabase );

	DatabaseSchema	theSchema;
	int				dbVersion = GetDBVersion();

	theSchema.readSchema( theDatabase );
	theSchema.emptySchema();

	SetDBVersion( dbVersion );
}
//---------------------------------------------------------------------------
#if CONF_USE_SEQUENCE
long TConfigDataModule::getNewMaxValue( const char *table, const char *field )
{
	doEnterFunction("TConfigDataModule::getNewMaxValue");

	long		newMaxValue;

	LockTable();

	STRING	configName = table;
	configName += '.';
	configName += field;

	configName.upperCase();

	newMaxValue = GetValue( configName, 0 );
	if( newMaxValue )
		newMaxValue++;
	else
		newMaxValue = ::getNewMaxValue( theDatabase->DatabaseName.c_str(), table, field );

	SetValue( configName, newMaxValue );

	UnlockTable();

	return newMaxValue;
}
#endif
//---------------------------------------------------------------------------

