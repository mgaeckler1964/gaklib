/*
		Project:		GAKLIB
		Module:			vcl_tools.h
		Description:	Tools for C++ Builder apps, only
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

#ifndef GAK_VCL_TOOLS_H
#define GAK_VCL_TOOLS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if !defined( __BORLANDC__ )	// C++ Builder 5
#error "for C++ Builder, only"
#endif
// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>

#include <dbgrids.hpp>
#include <dbtables.hpp>
#include <gak/xml.h>
#include <gak/datetime.h>
#include <gak/t_string.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc

#	pragma warn -inl
#endif

namespace gak
{
namespace vcl
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

/* User Manager */
const int USER_NO_PERM		= 0;
const int USER_LOGIN_PERM	= 1;
const int USER_GROUP_PERM	= 2;
const int USER_USER_PERM	= 4;
const int USER_SYSTEM_PERM	= 8;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct UserOrGroup
{
	int			ID, type;
	STRING		userName,
				encryptedPassword,
				groupList;
	int			department,
				permissions;

	UserOrGroup()
	{
		ID = 0;
	}
};

enum DBtype
{
	dbtPARADOX, dbtORACLE, dbtINTERBASE, dbtOTHER
};

struct DBconnector
{
	STRING	m_aliasName, m_userName, m_password;

	TDatabase *connectDB( TDatabase *theDatabase, bool Exclusive = false ) const;
	TDatabase *connectDB( bool Exclusive = false ) const
	{
		return connectDB( new TDatabase( NULL ), Exclusive );
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class ProgressLoger
{
	private:
	std::ostream	&m_out;

	protected:
	STRING	tableName;
	size_t	recordCount, totalRecordCount;

	public:
	ProgressLoger( std::ostream &out ) : m_out(out)
	{
		recordCount = totalRecordCount = 0;
	}

	void setTable( const STRING &newTableName )
	{
		tableName = newTableName;
		recordCount = 0;
	}
	void incCount( void )
	{
		recordCount++;
		totalRecordCount++;
	}

	virtual void showProgress( void ) const = 0;

	template <typename T>
	std::ostream & operator << ( const T &val )
	{
		return m_out << val;
	}
};

/*
	bde Schema
*/
class DatabaseSchema;
class xml::Element;
class TableSchema
{
	public:
	typedef CI_STRING	key_type;

	private:
	bool	m_inProcess, m_finished;

	CI_STRING	m_tableName;
	CI_STRING	m_tableType;

	Array<FLDDesc>	m_fields;
	Array<IDXDesc>	m_indices;
	Array<VCHKDesc>	m_validations;
	Array<RINTDesc>	m_references;

	public:
	TableSchema()
	{
		m_finished = m_inProcess = false;
	}
	STRING getTableName( void ) const;
	const CI_STRING &getKey( void ) const
	{
		return m_tableName;
	}
	void setKey( const CI_STRING &tableName )
	{
		m_tableName = tableName;
	}
	const CI_STRING &getType( void ) const
	{
		return m_tableType;
	}
	void initProcessing( void )
	{
		m_inProcess = false;
		m_finished = false;
	}
	void startProcessing( void )
	{
		m_inProcess = true;
		m_finished = false;
	}
	void stopProcessing( void )
	{
		m_inProcess = false;
		m_finished = true;
	}
	bool isInProcess( void ) const
	{
		return m_inProcess;
	}
	bool isFinished( void ) const
	{
		return m_finished;
	}
	void readSchema( TDatabase *theDatabase, const STRING &tableName, const STRING &tableType );

	void readSchemaXml( xml::Element *tableXML );
	xml::Element *getSchema( void ) const;

	static STRING stripPath( const STRING &iTableName );

	FLDDesc *getField( size_t i )
	{
		if( i < m_fields.size() )
		{
			return &m_fields[i];
		}
		else
		{
			return NULL;
		}
	}
	FLDDesc *getField( const CI_STRING &fieldName );
	FLDDesc *getField( Word fldNum )
	{
		return getField( (size_t)(fldNum-1) );
	}
	size_t getNumFields( void ) const
	{
		return m_fields.size();
	}

	IDXDesc *getIndex( size_t i )
	{
		if( i < m_indices.size() )
		{
			return &m_indices[i];
		}
		else
		{
			return NULL;
		}
	}
	IDXDesc *getPrimaryIndex( void );
	IDXDesc *getIndex( const CI_STRING &indexName );
	size_t getNumIndices( void ) const
	{
		return m_indices.size();
	}

	VCHKDesc *getValidation( size_t i )
	{
		if( i < m_validations.size() )
		{
			return &m_validations[i];
		}
		else
		{
			return NULL;
		}
	}
	VCHKDesc *getValidation( const CI_STRING &fieldName );
	size_t getNumValidations( void ) const
	{
		return m_validations.size();
	}

	RINTDesc *getReference( size_t i )
	{
		if( i < m_references.size() )
		{
			return &m_references[i];
		}
		else
		{
			return NULL;
		}
	}
	RINTDesc *getReference( const CI_STRING &referenceName );
	size_t getNumReferences( void ) const
	{
		return m_references.size();
	}
	void getReferenceTables( ArrayOfStrings *tableNames, RINTType refType );
	void getMasterTables( ArrayOfStrings *tableNames )
	{
		getReferenceTables( tableNames, rintDEPENDENT );
	}
	void getDetailTables( ArrayOfStrings *tableNames )
	{
		getReferenceTables( tableNames, rintMASTER );
	}


	STRING	compareTable( TableSchema &masterTable );

	void createTableScript( DBtype dbType, DatabaseSchema *theSchema, ArrayOfStrings *sqlScript );
};

class DatabaseSchema
{
	TDatabase			*m_database;
	bool				m_myOwnDB;
	Map<TableSchema>	m_tables;
	int					m_dbVersion;

	private:
	void connectDB( const DBconnector &dbConnector )
	{
		if( m_myOwnDB )
		{
			m_database->Close();
			delete m_database;
		}
		m_database = dbConnector.connectDB();
		m_myOwnDB = true;
	}
	void initProcessing( void );
	public:
	DatabaseSchema()
	{
		m_database = NULL;
		m_myOwnDB = false;
		m_dbVersion = -1;
	}
	~DatabaseSchema()
	{
		if( m_myOwnDB )
		{
			m_database->Close();
			delete m_database;
		}
	}
	TDatabase *getDatabase( void ) const
	{
		return m_database;
	}
	void closeDatabase( void )
	{
		if( m_myOwnDB )
		{
			m_database->Close();
			delete m_database;
			m_database = NULL;
			m_myOwnDB = false;
		}
	}

	void readSchema( TDatabase *theDatabase );
	void readSchema( const DBconnector &dbConnector )
	{
		doEnterFunction("DatabaseSchema::readSchema( const DB_CONNECTOR &dbConnector )");
		connectDB( dbConnector );
		readSchema( m_database );
	}
	void setDbVersion( int dbVersion )
	{
		m_dbVersion = dbVersion;
	}
	int getDbVersion( void ) const
	{
		return m_dbVersion;
	}
	void readSchemaXml( const STRING &xmlFileName );
	xml::Element *getSchema( void );

	size_t getNumTables( void )
	{
		return m_tables.size();
	}
	TableSchema &getTableSchema( const CI_STRING &tableName )
	{
		return m_tables[tableName];
	}
	TableSchema &getTableSchema( size_t index )
	{
		return m_tables.getElementAt( index );
	}
	void upgradeTable(
		const TDatabase *currentDB, TableSchema &currentTable, TableSchema &theTable
	);
	void upgradeTable( DatabaseSchema *current, TableSchema &theTable );
	void upgradeSchema( DatabaseSchema *current );

	void emptyTable( TableSchema &theTable );
	void emptySchema( void );

	virtual TDataSet *openTable( const STRING &tableName );
	virtual void copyRecord(
		TDataSet *sourceTable, TDataSet *destTable, const STRING &tableName
	);
	virtual void closeTable( TDataSet *theTable, const STRING &tableName );
	virtual size_t copyTable(
		DatabaseSchema *source, const STRING &tableName, ProgressLoger *theLoger
	);

	size_t copyTable(
		DatabaseSchema *source, TableSchema &theTable, ProgressLoger *theLoger
	);
	size_t copyTables(
		DatabaseSchema *source, ProgressLoger *theLoger
	);

	void createTableScript( TableSchema &theTable, DBtype dbType, ArrayOfStrings *sqlScript );
	void createSchema( const DBconnector &dbConnector, DBtype dbType );

	void createTableDropScript( TableSchema &theTable, DBtype dbType, ArrayOfStrings *sqlScript );
	void createDropScript( const DBconnector &dbConnector, DBtype dbType );
	void createSynonymScript( const DBconnector &dbConnector, DBtype dbType );

	STRING compareSchema( DatabaseSchema *master );

	// Subclass actions
	virtual void fieldAdded( const STRING &tableName, const STRING &fieldName );
	virtual bool checkFieldsBeforeInsert( TDataSet *sourceTable, const STRING &tableName );
	virtual void checkFieldsBeforePost( TDataSet *destTable, const STRING &tableName );
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

extern UserOrGroup	actUser;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	GUI Tools
*/
void setMemoText( TMemo *theMemo, T_STRING theText );

/*
	database tools
*/
long getNewMaxValue( const char *database, const char *table, const char *field, const char *filter=NULL );
void exportTable2CSV(
	TDataSet *theTable,
	const char *csvFile,
	bool convertUTF8 = false,
	bool (*traceFunc)( TDataSet *theTable ) = NULL
);
int importTableFromCSV( const char *csvFile, TDBDataSet *theTable, const char *ID=NULL, const char *idTable=NULL );
xml::Any *makeQueryXML( TDataSet *theQuery, const char *tag, int fltRound );
xml::Any *exportTable2XML( TDataSet *theTable, const char *xmlRoot, const char *xmlEntry, int fltRound );

void saveColumSettings( const TDBGrid *theGrid, const char *registryBase );
void restoreColumSettings( const TDBGrid *theGrid, const char *registryBase );

TDatabase *createDatabase( const char *path );

DBDesc getDatabaseDesc( const char *aliasName );

/*
	User Manager
*/
void getUserById( const AnsiString &database, int id, UserOrGroup *result );
void getUserByName( const AnsiString &database, const char *name, UserOrGroup*result );
const UserOrGroup &getActUser( const AnsiString &database );

void loginUser(
	const AnsiString &database,
	const AnsiString &userName,
	const AnsiString &password,
	UserOrGroup *userSession
);

inline const UserOrGroup &loginUser(
	const AnsiString &database,
	const AnsiString &userName,
	const AnsiString &password
)
{
	loginUser( database, userName, password, &actUser );

	return actUser;
}

void changePassword(
	const AnsiString &database,
	const AnsiString &oldPassword,
	const AnsiString &newPassword
);

size_t getUserCount( const AnsiString &database );
void fillUserList(
	TCustomComboBox *comboBox, const AnsiString &database, int selectedUser
);
int getUserByIndex( int index );

void bringWindowToFront( TForm *window );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

inline TDateTime EncodeDateTime( time_t theTime )
{
	return DateTime( theTime ).calcOriginalTime();
}

/*
	User Manager
*/
inline bool isSystemAdmin( void )
{
	return (actUser.permissions & USER_SYSTEM_PERM) ? true : false;
}

inline bool canEditUsers( void )
{
	return (actUser.permissions & (USER_SYSTEM_PERM|USER_USER_PERM)) ? true : false;
}

inline bool canEditGroups( void )
{
	return (actUser.permissions & (USER_SYSTEM_PERM|USER_GROUP_PERM)) ? true : false;
}

inline int getActUserID( void )
{
	return actUser.ID;
}
inline void setNewMaxValue( TTable *theTable, const char *field, const char *filter=NULL )
{
#if __BORLANDC__ >= 0x620
	theTable->FieldByName( field )->AsInteger = getNewMaxValue(
		AnsiString( theTable->DatabaseName ).c_str(),
		AnsiString( theTable->TableName ).c_str(),
		field, filter
	);
#else
	theTable->FieldByName( field )->AsInteger = getNewMaxValue(
		theTable->DatabaseName.c_str(),
		theTable->TableName.c_str(),
		field, filter
	);
#endif
}
inline void setNewMaxValue( TTable *theTable, TField *field, const char *filter=NULL )
{
#if __BORLANDC__ >= 0x620
	field->AsInteger = getNewMaxValue(
		AnsiString( theTable->DatabaseName ).c_str(),
		AnsiString( theTable->TableName ).c_str(),
		AnsiString( field->FieldName ).c_str(), filter
	);
#else
	field->AsInteger = getNewMaxValue(
		theTable->DatabaseName.c_str(),
		theTable->TableName.c_str(),
		field->FieldName.c_str(), filter
	);
#endif
}

inline bool isEmpty( TDataSet *query )
{
	return query->Eof && query->Bof;
}

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.

#	pragma warn +inl
#endif

#endif	// GAK_VCL_TOOLS_H
