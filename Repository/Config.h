/*
		Project:		GAKLIB
		Module:			Config.h
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

#ifndef ConfigH
#define ConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include <gak/string.h>
#include <gak/vcl_tools.h>

#define CONF_USE_SEQUENCE	0

//---------------------------------------------------------------------------
class TConfigDataModule : public TDataModule
{
__published:	// IDE-managed Components
	TQuery *deleteSQL;
	TQuery *selectStringSQL;
	TQuery *selectIntSQL;
	TQuery *updateStringSQL;
	TQuery *insertStringSQL;
	TQuery *updateIntSQL;
	TQuery *insertIntSQL;
private:	// User declarations
	int			lockCount;
	TDatabase	*theDatabase;

	void LockTable( void );
	void UnlockTable( void );

public:		// User declarations
	__fastcall TConfigDataModule(TComponent* Owner);

	void setDatabase( TDatabase *newDB )
	{
		doEnterFunctionEx(gakLogging::llInfo, "TConfigDataModule::setDatabase");
		theDatabase = newDB;

		if( newDB )
		{
			deleteSQL->DatabaseName = newDB->DatabaseName;

			selectStringSQL->DatabaseName = newDB->DatabaseName;
			updateStringSQL->DatabaseName = newDB->DatabaseName;
			insertStringSQL->DatabaseName = newDB->DatabaseName;

			selectIntSQL->DatabaseName = newDB->DatabaseName;
			updateIntSQL->DatabaseName = newDB->DatabaseName;
			insertIntSQL->DatabaseName = newDB->DatabaseName;
		}
	}

	void DeleteValue( const char *name );
	gak::STRING GetValue( const char *name, const char *defaultVal );
	int GetValue( const char *name, int defaultVal );
	void SetValue( const char *name, const char *newVal );
	void SetValue( const char *name, int newVal );

	int GetDBVersion( void )
	{
		return GetValue( "dbVersion", -1 );
	}
	int GetDBVersion( TDatabase *theDatabase )
	{
		setDatabase( theDatabase );
		return GetDBVersion();
	}
	int GetDBVersionByPath( const char *path );
	int GetDBVersionByAlias( const char *alias );

	void SetDBVersion( TDatabase *theDatabase, int versionCode )
	{
		setDatabase( theDatabase );
		SetValue( "dbVersion", versionCode );
	}
	void SetDBVersion( int versionCode )
	{
		SetValue( "dbVersion", versionCode );
	}

	gak::STRING OpenDatabase( TDatabase *theDatabase, int expectedDbVersion=0 );

	void emptyTables( const gak::STRING &directory );
	void emptyTables( TDatabase *theDatabase );

#if CONF_USE_SEQUENCE
	long getNewMaxValue( const char *table, const char *field );
#else
	long getNewMaxValue( const char *table, const char *field )
	{
		return gak::vcl::getNewMaxValue( theDatabase->DatabaseName.c_str(), table, field );
	}
#endif
};
//---------------------------------------------------------------------------
extern PACKAGE TConfigDataModule *ConfigDataModule;
//---------------------------------------------------------------------------
#endif
