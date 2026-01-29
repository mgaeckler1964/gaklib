/*
		Project:		GAKLIB
		Module:			bdeSchema.cpp
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <memory>

#include <gak/xmlParser.h>
#include <gak/vcl_tools.h>

#include <gak/numericString.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

namespace gak
{
namespace vcl
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

#if 0
typedef char *pCHAR;
typedef unsigned short UINT16;
inline DBIResult Chk( DBIResult status )
{
	Check( status );
	return status;
}


static DBIResult fDbiDoRestructure5(hDBIDb hTmpDb, pCHAR TblName, FLDDesc FldDesc, pCHAR NewTblName)
{
   DBIResult      rslt;
   CRTblDesc      TblDesc;
   pCROpType      AddOp;
   UINT16         i;
   pFLDDesc       pFldDesc;
   hDBICur        hCur;
   CURProps       Props;

   // Get an existing cursor on the source table.
   rslt = Chk(DbiGetCursorForTable(hTmpDb, TblName, NULL, hCur));
   if (rslt != DBIERR_NONE)
	  return rslt;

   // Get the amount of fields in the source table.

   rslt = Chk(DbiGetCursorProps(hCur, Props));
   if (rslt != DBIERR_NONE)
	  return rslt;

   // Get the existing field descriptor.
   pFldDesc = (pFLDDesc)malloc((Props.iFields + 1) * sizeof(FLDDesc));
   rslt = Chk(DbiGetFieldDescs(hCur, pFldDesc));
   if (rslt != DBIERR_NONE)
   {
	  free(pFldDesc);
	  return rslt;
   }

   // Close the source table so the restructure can occur.
   rslt = Chk(DbiCloseCursor(hCur));
   if (rslt != DBIERR_NONE)
   {
	  free(pFldDesc);
	  return rslt;
   }

   // Move the new field descriptor to the end of the source field descriptor.
   memcpy(&pFldDesc[Props.iFields], &FldDesc, sizeof(FLDDesc));

   // Put a crADD at the same position ad the new field descriptor
   AddOp = (pCROpType)malloc((Props.iFields + 1) * sizeof(CROpType));
   memset(AddOp, crNOOP, (Props.iFields + 1) * sizeof(CROpType));
   AddOp[Props.iFields] = crADD;

   memset(&TblDesc, 0, sizeof(TblDesc));
   strcpy(TblDesc.szTblName, TblName);

   TblDesc.iFldCount = (UINT16)(Props.iFields + 1);
   TblDesc.pecrFldOp = AddOp;
   TblDesc.pfldDesc = pFldDesc;

   // Resync the field numbers in order.
   for (i = 0; i < Props.iFields; i++)
		pFldDesc[i].iFldNum = (UINT16)(i + 1);

   rslt = Chk(DbiDoRestructure(hTmpDb, 1, &TblDesc, NewTblName, NULL, NULL, FALSE));
   free(AddOp);
   free(pFldDesc);
   return rslt;
}

static int compare( const void *iFld1, const void *iFld2 )
{
	const FLDDesc *fld1 = (const FLDDesc*)iFld1;
	const FLDDesc *fld2 = (const FLDDesc*)iFld2;

	if( fld1->iFldNum < fld2->iFldNum )
		return -1;

	if( fld1->iFldNum > fld2->iFldNum )
		return 1;

	if( fld1->iOffset < fld2->iOffset )
		return -1;

	if( fld1->iOffset > fld2->iOffset )
		return 1;

	return 0;
}
#endif

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

STRING	TableSchema::stripPath( const STRING &iTableName )
{
	STRING tableName = iTableName;

	size_t	slashPos = tableName.searchRChar( DIRECTORY_DELIMITER );
	if( slashPos != tableName.no_index )
	{
		tableName += size_t(slashPos +1);
	}
	size_t	dotPos = tableName.searchRChar( '.' );
	if( dotPos != (size_t)-1 )
	{
		tableName.cut( dotPos );
	}

	return tableName;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void DatabaseSchema::initProcessing( void )
{
	for(
		Map<TableSchema>::iterator it = m_tables.begin(), endIT = m_tables.end();
		it != endIT;
		++it
	)
	{
		TableSchema &theTable = *it;
		theTable.initProcessing();
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

TDatabase *DBconnector::connectDB( TDatabase *theDatabase, bool Exclusive ) const
{
	doEnterFunction("DBconnector::connectDB( TDatabase *theDatabase, bool Exclusive ) const");
	doLogValue(Exclusive);
	STRING	dbName = m_aliasName + "theDatabase";

	theDatabase->AliasName = static_cast<const char *>(m_aliasName);
	theDatabase->DatabaseName = static_cast<const char *>(dbName);

	if( !m_userName.isEmpty() )
		theDatabase->Params->Values["USER NAME"] = static_cast<const char *>(m_userName);
	if( !m_password.isEmpty() )
	{
		theDatabase->Params->Values["PASSWORD"] = static_cast<const char *>(m_password);
		theDatabase->LoginPrompt = false;
	}

	doLogPosition();
	theDatabase->Exclusive = Exclusive;
	doLogPosition();
	theDatabase->Open();
	doLogPosition();

	return theDatabase;
}

STRING TableSchema::getTableName( void ) const
{
	STRING theTableName = m_tableName;

	if( getType() == "DBASE" )
	{
		theTableName += ".DBF";
	}

	return theTableName;
}
/*
	=======================
	reading schema from BDE
	=======================
*/
void TableSchema::readSchema( TDatabase *theDatabase, const STRING &tableName, const STRING &tableType )
{
	doEnterFunction("TableSchema::readSchema");

	m_tableName = tableName;
	m_tableType = tableType;

	hDBICur		theCursor;
	FLDDesc		FieldDescr;
	IDXDesc		IndexDescr;
	VCHKDesc	ValidityDescr;
	RINTDesc	referenceDescr;

	m_fields.clear();
	m_indices.clear();
	m_validations.clear();
	m_references.clear();

	doLogPosition();
	Check( DbiOpenFieldList(
		theDatabase->Handle,
		tableName, NULL, FALSE, theCursor
	) );
	doLogPosition();
	while( DbiGetNextRecord(theCursor, dbiNOLOCK, &FieldDescr, NULL) == DBIERR_NONE )
	{
		doLogValue(FieldDescr.szName);
		m_fields.addElement( FieldDescr );
	}
	DbiCloseCursor(theCursor);

	// why this? INDEX description uses indices not fldNums
	// fields.qsort( compare );

	Check( DbiOpenIndexList(
		theDatabase->Handle,
		tableName, NULL, theCursor
	) );
	while( DbiGetNextRecord(theCursor, dbiNOLOCK, &IndexDescr, NULL) == DBIERR_NONE )
	{
		doLogValue(IndexDescr.szName);
		m_indices.addElement( IndexDescr );
	}
	DbiCloseCursor(theCursor);

	Check( DbiOpenVchkList(
		theDatabase->Handle,
		tableName, NULL, theCursor
	) );
	while( DbiGetNextRecord(theCursor, dbiNOLOCK, &ValidityDescr, NULL) == DBIERR_NONE )
	{
		m_validations.addElement( ValidityDescr );
	}
	DbiCloseCursor(theCursor);

	Check( DbiOpenRintList(
		theDatabase->Handle,
		tableName, NULL, theCursor
	) );
	while( DbiGetNextRecord(theCursor, dbiNOLOCK, &referenceDescr, NULL) == DBIERR_NONE )
	{
		doLogValue(referenceDescr.szRintName);
		m_references.addElement( referenceDescr );
	}
	DbiCloseCursor(theCursor);
}

void DatabaseSchema::readSchema( TDatabase *theDatabase )
{
	doEnterFunction("DatabaseSchema::readSchema( TDatabase *theDatabase )");
	hDBICur		theCursor;
	TBLBaseDesc	tableDescr;
	TableSchema	theTable;

	if( theDatabase != m_database )
	{
		if( m_myOwnDB )
		{
			m_database->Close();
			delete m_database;
		}
		m_database = theDatabase;
		m_myOwnDB = false;
	}

	m_tables.clear();
	try
	{
		doLogPosition();
		Check( DbiOpenTableList(
			theDatabase->Handle,
			FALSE, FALSE, NULL, theCursor
		) );
		doLogPosition();
		while( DbiGetNextRecord(theCursor, dbiNOLOCK, &tableDescr, NULL) == DBIERR_NONE )
		{
			doLogValue( tableDescr.szName );
			if( !strchr( (const char *)tableDescr.szName, '.'  ) )
			{
				theTable.readSchema(
					theDatabase,
					(const char *)tableDescr.szName,
					(const char *)tableDescr.szType
				);
				m_tables[(const char *)tableDescr.szName] = theTable;
			}
		}
		doLogPosition();
		DbiCloseCursor(theCursor);
		doLogPosition();
	}
	catch( ... )
	{
		doLogMessage("*gak*: Excption");
		DbiCloseCursor(theCursor);
/*@*/	throw;
	}
}

/*
	=======================
	reading schema from XML
	=======================
*/
void TableSchema::readSchemaXml( xml::Element *tableXML )
{
	STRING			attrValue;
	size_t			numIndexFields, numReferenceFields;
	xml::XmlArray	fieldElements, indexElements, validityElements, referencesElements;

	tableXML->getChildElements( &fieldElements, "FIELD" );
	for( size_t fieldIdx=0; fieldIdx<fieldElements.size(); fieldIdx++ )
	{
		xml::Element*fieldXML = fieldElements[fieldIdx];
		FLDDesc		&theField = m_fields[fieldIdx];

		attrValue = fieldXML->getAttribute( "name" );
		strcpy( theField.szName, attrValue );

		attrValue = fieldXML->getAttribute( "fldNum" );
		theField.iFldNum = attrValue.getValueE<Word>();

		attrValue = fieldXML->getAttribute( "type" );
		theField.iFldType = attrValue.getValueE<Word>();

		attrValue = fieldXML->getAttribute( "subType" );
		theField.iSubType = attrValue.getValueE<Word>();

		attrValue = fieldXML->getAttribute( "unit1" );
		theField.iUnits1 = attrValue.getValueE<Word>();

		attrValue = fieldXML->getAttribute( "unit2" );
		theField.iUnits2 = attrValue.getValueE<Word>();
	}

	tableXML->getChildElements( &indexElements, "INDEX" );
	for( size_t indexIdx = 0; indexIdx<indexElements.size(); indexIdx++ )
	{
		xml::Element*indexXML = indexElements[indexIdx];
		IDXDesc		&IndexDescr = m_indices[indexIdx];

		attrValue = indexXML->getAttribute( "name" );
		strcpy( IndexDescr.szName, attrValue );

		attrValue = indexXML->getAttribute( "primary" );
		IndexDescr.bPrimary = (Word)(attrValue== "PRIMARY");

		attrValue = indexXML->getAttribute( "unique" );
		IndexDescr.bUnique = (Word)(attrValue== "UNIQUE");

		attrValue = indexXML->getAttribute( "descending" );
		IndexDescr.bDescending = (Word)(attrValue== "DESC");

		attrValue = indexXML->getAttribute( "maintained" );
		IndexDescr.bMaintained = (Word)(attrValue== "true");

		indexXML->getChildElements( &fieldElements, "FIELD" );
		numIndexFields = fieldElements.size();
		IndexDescr.iFldsInKey = (Word)numIndexFields;
		for( size_t fieldIdx=0; fieldIdx<numIndexFields; fieldIdx++ )
		{
			xml::Element*fieldXML = fieldElements[fieldIdx];
			attrValue = fieldXML->getAttribute( "fldNum" );
			IndexDescr.aiKeyFld[fieldIdx] = attrValue.getValueE<Word>();
		}
	}

	tableXML->getChildElements( &validityElements, "VALIDITY" );
	for( size_t i=0; i<validityElements.size(); i++ )
	{
		xml::Element*theValidity = validityElements[i];
		VCHKDesc	&ValidityDescr = m_validations[i];

		attrValue = theValidity->getAttribute( "fldNum" );
		ValidityDescr.iFldNum = attrValue.getValueE<Word>();

		attrValue = theValidity->getAttribute( "required" );
		ValidityDescr.bRequired = (Word)(attrValue== "true");
	}

	tableXML->getChildElements( &referencesElements, "REFERENCES" );
	for( size_t i=0; i<referencesElements.size(); i++ )
	{
		xml::Element*theReference = referencesElements[i];
		RINTDesc	&ReferenceDescr = m_references[i];

		attrValue = theReference->getAttribute( "name" );
		strcpy( ReferenceDescr.szRintName, attrValue );

		attrValue = theReference->getAttribute( "type" );
		if( attrValue == "MASTER" )
			ReferenceDescr.eType = rintMASTER;
		else
			ReferenceDescr.eType = rintDEPENDENT;

		attrValue = theReference->getAttribute( "refTable" );
		strcpy( ReferenceDescr.szTblName, attrValue );

		theReference->getChildElements( &fieldElements, "FIELD" );
		numReferenceFields = fieldElements.size();
		ReferenceDescr.iFldCount = (Word)numReferenceFields;
		for( size_t fieldIdx=0; fieldIdx<numReferenceFields; fieldIdx++ )
		{
			xml::Element*fieldXML = fieldElements[fieldIdx];

			attrValue = fieldXML->getAttribute( "thisFldNum" );
			ReferenceDescr.aiThisTabFld[fieldIdx] = attrValue.getValueE<Word>();

			attrValue = fieldXML->getAttribute( "otherFldNum" );
			ReferenceDescr.aiOthTabFld[fieldIdx] = attrValue.getValueE<Word>();
		}

	}
}
/* TODO : Add unique ptr */
void DatabaseSchema::readSchemaXml( const STRING &xmlFileName )
{
	xml::Parser		theParser( xmlFileName );
	xml::Document	*theDocument = theParser.readFile( false );

	if( theDocument )
	{
		xml::Element	*theDatabaseXML = theDocument->getElement( "DATABASE" );
		if( theDatabaseXML )
		{
			STRING dbVersionStr = theDatabaseXML->getAttribute( "dbVersion" );
			if( !dbVersionStr.isEmpty() )
			{
				m_dbVersion = dbVersionStr.getValueE<unsigned long>();
			}
			else
			{
				m_dbVersion = -1;
			}

			xml::XmlArray tableElements;
			theDatabaseXML->getChildElements( &tableElements, "TABLE" );
			m_tables.clear();

			for( size_t	tableIdx=0; tableIdx < tableElements.size(); tableIdx++ )
			{
				xml::Element*tableXML = tableElements[tableIdx];
				STRING		tableName = tableXML->getAttribute( "name" );

				TableSchema &theTable = m_tables[tableName];
				theTable.readSchemaXml( tableXML );
			}
		}
	}
}

/*
	===================
	creating XML schema
	===================
*/
xml::Element *TableSchema::getSchema( void ) const
{
	xml::Element	*theTableXML = new xml::Any( "TABLE" );

	theTableXML->setStringAttribute( "name", m_tableName );
	for( size_t i=0; i<m_fields.size(); i++ )
	{
		const FLDDesc &FieldDescr = m_fields[i];
		xml::Element *theField = theTableXML->addObject( new xml::Any( "FIELD" ) );
		theField->setIntegerAttribute( "fldNum", FieldDescr.iFldNum );
		theField->setStringAttribute( "name", FieldDescr.szName );
		theField->setIntegerAttribute( "type", FieldDescr.iFldType );
		theField->setIntegerAttribute( "subType", FieldDescr.iSubType );
		theField->setIntegerAttribute( "unit1", FieldDescr.iUnits1 );
		theField->setIntegerAttribute( "unit2", FieldDescr.iUnits2 );
	}

	for( size_t i=0; i<m_indices.size(); i++ )
	{
		const IDXDesc &IndexDescr = m_indices[i];
		xml::Element *theIndex = theTableXML->addObject( new xml::Any( "INDEX" ) );
		theIndex->setStringAttribute( "name", IndexDescr.szName );
		theIndex->setStringAttribute( "primary", IndexDescr.bPrimary ? "PRIMARY" : "SECONDARY" );
		theIndex->setStringAttribute( "unique", IndexDescr.bUnique ? "UNIQUE" : "NON-UNIQUE" );
		// theIndex->setAttribute( "abDescending", IndexDescr.abDescending ? true : false );
		theIndex->setStringAttribute( "descending", IndexDescr.bDescending ? "DESC" : "ASC" );
		theIndex->setBoolAttribute( "maintained", IndexDescr.bMaintained );
		for( size_t j=0; j<(size_t)IndexDescr.iFldsInKey; j++ )
		{
			xml::Element	*theField = theIndex->addObject( new xml::Any( "FIELD" ) );
			theField->setIntegerAttribute( "fldNum", IndexDescr.aiKeyFld[j] );
		}
	}

	for( size_t i=0; i<m_validations.size(); i++ )
	{
		const VCHKDesc &ValidityDescr = m_validations[i];
		xml::Element *theValidity = theTableXML->addObject( new xml::Any( "VALIDITY" ) );
		theValidity->setIntegerAttribute( "fldNum", ValidityDescr.iFldNum );
		theValidity->setBoolAttribute( "required", ValidityDescr.bRequired );
	}

	for( size_t i=0; i<m_references.size(); i++ )
	{
		const RINTDesc &referenceDescr = m_references[i];
		xml::Element *theReference = theTableXML->addObject(
			new xml::Any( "REFERENCES" )
		);
		theReference->setStringAttribute( "name", referenceDescr.szRintName );
		theReference->setStringAttribute(
			"type", referenceDescr.eType == rintMASTER ? "MASTER" : "DEPENDENT"
		);
		theReference->setStringAttribute(
			"refTable", stripPath( referenceDescr.szTblName )
		);
		for( size_t j=0; j<(size_t)referenceDescr.iFldCount; j++ )
		{
			xml::Element	*theField = theReference->addObject( new xml::Any( "FIELD" ) );
			theField->setIntegerAttribute( "thisFldNum", referenceDescr.aiThisTabFld[j] );
			theField->setIntegerAttribute( "otherFldNum", referenceDescr.aiOthTabFld[j] );
		}
	}

	return theTableXML;
}

xml::Element *DatabaseSchema::getSchema( void )
{
	xml::Element	*theDatabaseXML = new xml::Any( "DATABASE" );
	for(
		Map<TableSchema>::const_iterator it = m_tables.cbegin(), endIT = m_tables.cend();
		it != endIT;
		++it
	)
	{
		theDatabaseXML->addObject( it->getSchema() );
	}
	if( m_dbVersion >= 0 )
	{
		theDatabaseXML->setIntegerAttribute( "dbVersion", m_dbVersion );
	}

	return theDatabaseXML;
}

/*
FLDDesc *TableSchema::getField( Word fldNum )
{
	for( size_t i=0; i<fields.size(); i++ )
	{
		FLDDesc &theField = fields[i];
		if( fldNum == theField.iFldNum )
			return &theField;
	}

	return NULL;
}
*/

/*
	========================
	searching within schema
	========================
*/

// Fields
// ======
FLDDesc *TableSchema::getField( const CI_STRING &fieldName )
{
	for( size_t i=0; i<m_fields.size(); i++ )
	{
		FLDDesc &theField = m_fields[i];
		if( fieldName == (const char *)theField.szName )
		{
			return &theField;
		}
	}

	return NULL;
}


// Indices
// =======
IDXDesc *TableSchema::getPrimaryIndex( void )
{
	for( size_t i=0; i<m_indices.size(); i++ )
	{
		IDXDesc &theIndex = m_indices[i];
		if( theIndex.bPrimary )
		{
			return &theIndex;
		}
	}

	return NULL;
}

IDXDesc *TableSchema::getIndex( const CI_STRING &indexName )
{
	for( size_t i=0; i<m_indices.size(); i++ )
	{
		IDXDesc &theIndex = m_indices[i];
		if( indexName == (const char *)theIndex.szName )
		{
			return &theIndex;
		}
	}

	return NULL;
}

// Validations
// ===========
VCHKDesc *TableSchema::getValidation( const CI_STRING &fieldName )
{
	for( size_t i=0; i<m_validations.size(); i++ )
	{
		FLDDesc	*theField = getField( m_validations[i].iFldNum );
		if( fieldName == theField->szName )
		{
/***/		return &m_validations[i];
		}
	}

	return NULL;
}

// Reference constraints
// =====================
RINTDesc *TableSchema::getReference( const CI_STRING &referenceName )
{
	for( size_t i=0; i<m_references.size(); i++ )
	{
		RINTDesc &theReference = m_references[i];
		if( referenceName == static_cast<const char *>(theReference.szRintName) )
		{
			return &theReference;
		}
	}

	return NULL;
}


void TableSchema::getReferenceTables( ArrayOfStrings *tableNames, RINTType refType )
{
	tableNames->clear();
	for( size_t i=0; i<m_references.size(); i++ )
	{
		const RINTDesc &ref = m_references[i];
		if( ref.eType == refType )
		{
			tableNames->addElement( stripPath( (const char *)ref.szTblName ) );
		}
	}
}

#if 0
void DatabaseSchema::upgradeTable(
	const TDatabase *currentDB, TableSchema &currentTable, TableSchema &theTable
)
{
	ARRAY<FLDDesc>	newFields;
	ARRAY<CROpType>	allOperations;

	ARRAY<FLDDesc>	changedFields;
	ARRAY<CROpType>	changeOperations;
	CRTblDesc		restructureInfo;

	for( size_t i=0; i<theTable.getNumFields(); i++ )
	{
		FLDDesc *source = theTable.getField( i );
		FLDDesc *current = currentTable.getField( (const char *)source->szName );
		if( !current )
		{
			FLDDesc newElem;
			memset( &newElem, 0, sizeof( newElem ) );
			strcpy( newElem.szName, source->szName );
			newElem.iFldType = source->iFldType;
			newElem.iSubType = source->iSubType;
			newElem.iUnits1 = source->iUnits1;
			newElem.iUnits2 = source->iUnits2;

			changedFields.addElement( newElem );
			changeOperations.addElement( crADD );
		}
		else
		{
			newFields.addElement( *current );
			allOperations.addElement( crNOOP );
		}
	}

	if( changedFields.size() )
	{/*
		for( size_t i=0; i<newFields.size(); i++ )
		{
			FLDDesc	&fld = newFields[i];
			fld.iFldNum = i+1;
		}*/

		//newFields.addElements( changedFields );
		//allOperations.addElements( changeOperations );
/*		for( size_t i=0; i<changedFields.size(); i++ )
		{
			fDbiDoRestructure5(currentDB->Handle, currentTable.getName() + ".DB", changedFields[i], currentTable.getName() + "_NEW.DB" );

		}
*/
		memset( &restructureInfo, 0, sizeof( restructureInfo ) );
		strcpy( restructureInfo.szTblName, currentTable.getName() );
		strcpy( restructureInfo.szTblType, currentTable.getType() );
		restructureInfo.iFldCount = (Word)newFields.size();
		restructureInfo.pfldDesc = (FLDDesc*)newFields.getDataBuffer();
		restructureInfo.pecrFldOp = (CROpType*)allOperations.getDataBuffer();

		Check(
			DbiDoRestructure(
				currentDB->Handle, 1, &restructureInfo, NULL, NULL, NULL, FALSE
			)
		);


		for( size_t i=0; i<changedFields.size(); i++ )
		{
			if( changeOperations[i] == crADD )
				fieldAdded( theTable.getName(), (const char *)changedFields[i].szName );
		}
	}
}

void DatabaseSchema::upgradeTable( DatabaseSchema *current, TableSchema &theTable )
{
	ArrayOfStrings	masterTables;

	theTable.startProcessing();
	theTable.getMasterTables( &masterTables );
	for( size_t i=0; i<masterTables.size(); i++ )
	{
		TableSchema &masterTable = getTableSchema( masterTables[i] );
		if( !masterTable.isFinished() && !masterTable.isInProcess() )
			upgradeTable( current, masterTable );
	}

	TableSchema	&currentTable = current->getTableSchema( theTable.getName() );
	if( currentTable.getNumFields() )
		upgradeTable( current->getDatabase(), currentTable, theTable );
	// else
		// createTable
	 /* TODO -ogak -cDB : createTable */

	theTable.stopProcessing();
}

void DatabaseSchema::upgradeSchema( DatabaseSchema *current )
{
	initProcessing();
	for( size_t i=0; i<tables.size(); i++ )
	{
		const TableSchema &theTable = tables[i];
		if( !theTable.isFinished() && !theTable.isInProcess() )
			upgradeTable( current, tables[i] );
	}
}
#endif

/*
	============
	Clear tables
	============
*/
void DatabaseSchema::emptyTable( TableSchema &theTable )
{
	doEnterFunction("DatabaseSchema::emptyTable");
	doLogValue(theTable.getTableName());
	ArrayOfStrings	detailTables;

	theTable.startProcessing();
	theTable.getDetailTables( &detailTables );
	for( size_t i=0; i<detailTables.size(); i++ )
	{
		TableSchema &detailTable = getTableSchema( detailTables[i] );
		if( !detailTable.isFinished() && !detailTable.isInProcess() )
		{
			emptyTable( detailTable );
		}
	}

	TTable *dbTable = new TTable( NULL );
	try
	{
		if( m_database->IsSQLBased && !m_database->InTransaction )
		{
			m_database->StartTransaction();
		}
		dbTable->DatabaseName = m_database->DatabaseName;
		dbTable->TableName = static_cast<const char *>(theTable.getTableName());
		doLogPosition();
		dbTable->EmptyTable();
		doLogPosition();
		delete dbTable;
		if( m_database->IsSQLBased && m_database->InTransaction )
		{
			m_database->Commit();
		}
	}
	catch( ... )
	{
		doLogPosition();
		delete dbTable;
		if( m_database->IsSQLBased && m_database->InTransaction )
		{
			m_database->Rollback();
		}
		throw;
	}


	theTable.stopProcessing();
}

void DatabaseSchema::emptySchema( void )
{
	doEnterFunction("DatabaseSchema::emptySchema");
	initProcessing();
	for(
		Map<TableSchema>::iterator it = m_tables.begin(), endIT = m_tables.end();
		it != endIT;
		++it
	)
	{
		TableSchema &theTable = *it;
		if( !theTable.isFinished() && !theTable.isInProcess() )
		{
			emptyTable( theTable );
		}
	}
}

/*
	===========
	copy tables
	===========
*/
TDataSet *DatabaseSchema::openTable( const STRING &tableName )
{
	TTable	*theTable;

	theTable = new TTable( NULL );
	theTable->DatabaseName = m_database->DatabaseName;
	theTable->TableName = static_cast<const char *>(tableName);
	theTable->Open();

	return theTable;
}

void DatabaseSchema::copyRecord(
	TDataSet *sourceTable, TDataSet *destTable, const STRING &tableName
)
{
	TField		*theSourceField, *theDestField;
	TFieldType	theSourceType;
	AnsiString	fieldName;

	destTable->Insert();

	for( size_t i=0; i<(size_t)sourceTable->FieldCount; i++ )
	{
		theSourceField = sourceTable->Fields->Fields[i];
		fieldName = theSourceField->FieldName;
		theDestField = destTable->FindField( fieldName );
		if( theDestField )
		{
			theSourceType = theSourceField->DataType;

			switch( theSourceType )
			{
				case ftString:
				case ftMemo:
				case ftOraClob:
					theDestField->AsString = theSourceField->AsString;
					break;
				case ftAutoInc:
				case ftSmallint:
				case ftInteger:
					if( !theSourceField->IsNull )
						theDestField->AsInteger = theSourceField->AsInteger;
					break;
				case ftFloat:
				case ftCurrency:
					if( !theSourceField->IsNull )
						theDestField->AsFloat = theSourceField->AsFloat;
					break;
				case ftBoolean:
					if( !theSourceField->IsNull )
						theDestField->AsBoolean = theSourceField->AsBoolean;
					break;
				case ftDateTime:
				case ftDate:
					if( !theSourceField->IsNull )
						theDestField->AsDateTime = theSourceField->AsDateTime;
					break;
				default:
					throw( Exception( "Unknown Type" ) );
			}
		}
	}

	checkFieldsBeforePost( destTable, tableName );

	destTable->Post();
}

void DatabaseSchema::closeTable( TDataSet *theTable, const STRING &tableName )
{
	theTable->Close();
	delete theTable;
}

size_t DatabaseSchema::copyTable(
	DatabaseSchema	*source,
	const STRING	&tableName,
	ProgressLoger	*theLoger
)
{
	doEnterFunction("DatabaseSchema::copyTable");
	doLogValue(tableName);
	TDataSet	*sourceTable, *destTable;

	size_t		recordCount = 0;

	if( theLoger )
	{
		theLoger->setTable( tableName );
		theLoger->showProgress();
		*theLoger << "Copy table " << tableName;
	}

	if( m_database->IsSQLBased && !m_database->InTransaction )
	{
		m_database->StartTransaction();
	}
	try
	{
		doLogMessage( "*gak*: open source" );
		sourceTable = source->openTable( tableName );
		doLogMessage( "*gak*: open destinamtion" );
		destTable = openTable( tableName );

		doLogPosition();
		while( !sourceTable->Eof )
		{
			if( source->checkFieldsBeforeInsert( sourceTable, tableName ) )
			{
				recordCount++;
				if( theLoger )
				{
					theLoger->incCount();
					if( !(recordCount % 1000) )
					{
						theLoger->showProgress();
					}
				}
				copyRecord( sourceTable, destTable, tableName );
			}
			sourceTable->Next();
		}
		doLogPosition();

		doLogMessage( "*gak*: close destinamtion" );
		closeTable( destTable, tableName );
		doLogMessage( "*gak*: close source" );
		source->closeTable( sourceTable, tableName );

		if( m_database->IsSQLBased && m_database->InTransaction )
		{
			m_database->Commit();
		}
	}
	catch( ... )
	{
		doLogMessage( "*gak*: Exception" );
		doLogValue(tableName);
		delete destTable;
		delete sourceTable;

		recordCount = 0;
		if( m_database->IsSQLBased && m_database->InTransaction )
		{
			m_database->Rollback();
		}

		if( theLoger )
		{
			*theLoger << " error\n";
		}
		throw;
	}

	if( theLoger )
	{
		*theLoger << ' ' << recordCount << " record(s)\n";
	}
	return recordCount;
}

size_t DatabaseSchema::copyTable(
	DatabaseSchema *source, TableSchema &theTable,
	ProgressLoger	*theLoger
)
{
	doEnterFunction("DatabaseSchema::copyTable");
	ArrayOfStrings	masterTables;
	size_t			recordCount = 0;
	STRING			srcName, destName;

	theTable.startProcessing();
	theTable.getMasterTables( &masterTables );
	for(
		ArrayOfStrings::const_iterator it = masterTables.cbegin(),
			endIT = masterTables.cend();
		it != endIT;
		++it
	)
	{
		TableSchema &masterTable = getTableSchema( *it );
		if( !masterTable.isFinished() && !masterTable.isInProcess() )
		{
			recordCount += copyTable( source, masterTable, theLoger );
		}
	}

	TableSchema	&sourceTable = source->getTableSchema( theTable.getKey() );
	if( sourceTable.getNumFields() )	// exists?
	{
		recordCount += copyTable( source, sourceTable.getTableName(), theLoger );
	}

	theTable.stopProcessing();

	return recordCount;
}

size_t DatabaseSchema::copyTables(
	DatabaseSchema *source, ProgressLoger *theLoger
)
{
	doEnterFunction("DatabaseSchema::copyTables");
	size_t recordCount = 0;

	emptySchema();
	initProcessing();
	for(
		Map<TableSchema>::iterator it = m_tables.begin(), endIT = m_tables.end();
		it != endIT;
		++it
	)
	{
		TableSchema &theTable = *it;
		if( !theTable.isFinished() && !theTable.isInProcess() )
		{
			recordCount += copyTable( source, theTable, theLoger );
		}
	}

	return recordCount;
}

/*
	==========================
	creating SQL Server tables
	==========================
*/
void TableSchema::createTableScript(
	DBtype dbType, DatabaseSchema *theSchema, ArrayOfStrings *sqlScript
)
{
	FLDDesc *theField;
	IDXDesc	*primaryIndex = getPrimaryIndex();

	STRING refTableName, tableName = getTableName();
	STRING sqlCmd = "CREATE TABLE ";
	sqlCmd += tableName;
	sqlCmd += " (";
	for( size_t fieldIdx=0; fieldIdx<m_fields.size(); fieldIdx++ )
	{
		if( fieldIdx )
		{
			sqlCmd += ',';
		}

		FLDDesc	&theField = m_fields[fieldIdx];
		sqlCmd += theField.szName;
		sqlCmd += ' ';

		if( theField.iFldType == 1 )
		{
			sqlCmd += "VARCHAR(",
			sqlCmd += formatNumber( theField.iUnits1 );
			sqlCmd += ')';
		}
		else if( theField.iFldType == 3 )
		{
			if( dbType == dbtORACLE )
			{
				sqlCmd += "CLOB";
			}
			else
			{
				sqlCmd += "BLOB(",
				sqlCmd += formatNumber( theField.iUnits1 );
				sqlCmd += ",1)";
			}
		}
		else if( theField.iFldType == 5 || theField.iFldType == 6 )
		{
			if( dbType == dbtORACLE )
			{
				sqlCmd += "NUMBER(10,0)";
			}
			else
			{
				sqlCmd += "INTEGER";
			}
		}
		else if( theField.iFldType == 7 )
		{
			if( dbType == dbtORACLE )
			{
				sqlCmd += "NUMBER";
			}
			else
			{
				sqlCmd += "FLOAT";
			}
		}
		else if( theField.iFldType == 11 )
		{
			if( dbType == dbtORACLE )
			{
				sqlCmd += "DATE";
			}
			else
			{
				sqlCmd += "TIMESTAMP";
			}
		}
		else
		{
			throw Exception( "unkown type" );
		}

		for( size_t j=0; j<m_validations.size(); j++ )
		{
			VCHKDesc	&check = m_validations[j];
			if( check.bRequired && check.iFldNum == Word(fieldIdx+1) )
			{
				sqlCmd += " NOT NULL";
			}
		}

		if( primaryIndex && primaryIndex->iFldsInKey == 1 )
		{
			for( Word j=0; j<primaryIndex->iFldsInKey; j++ )
			{
				if( (Word)(fieldIdx+1) == primaryIndex->aiKeyFld[j] )
				{
					sqlCmd += " PRIMARY KEY";
/*v*/				break;
				}
			}
		}
	}

	if( primaryIndex && primaryIndex->iFldsInKey > 1 )
	{
		sqlCmd += ", CONSTRAINT ";
		if( primaryIndex->szName[0] )
		{
			sqlCmd += primaryIndex->szName;
		}
		else
		{
			sqlCmd += tableName + "pk";
		}

		sqlCmd += " PRIMARY KEY (";
		for( Word j=0; j<primaryIndex->iFldsInKey; j++ )
		{
			if( j )
			{
				sqlCmd += ',';
			}

			theField = getField( primaryIndex->aiKeyFld[j] );
			sqlCmd += theField->szName;
		}
		sqlCmd += ')';
	}

	for( size_t i=0; i<m_references.size(); i++ )
	{
		RINTDesc	&ref = m_references[i];
		if( ref.eType == rintDEPENDENT )
		{
			sqlCmd += ", CONSTRAINT ";
			sqlCmd += ref.szRintName;
			sqlCmd += " FOREIGN KEY (";

			for( size_t j=0; j<(size_t)ref.iFldCount; j++ )
			{
				if( j )
				{
					sqlCmd += ',';
				}
				theField = getField( ref.aiThisTabFld[j] );
				sqlCmd += theField->szName;
			}
			sqlCmd += ") REFERENCES ";
			refTableName = stripPath( ref.szTblName );
			sqlCmd += refTableName;

			sqlCmd += '(';
			TableSchema &refTable = theSchema->getTableSchema( refTableName );
			for( size_t j=0; j<(size_t)ref.iFldCount; j++ )
			{
				if( j )
				{
					sqlCmd += ',';
				}
				theField = refTable.getField( ref.aiOthTabFld[j] );
				sqlCmd += theField->szName;
			}
			sqlCmd += ')';
		}
	}
	sqlCmd += ')';

	sqlScript->addElement( sqlCmd );

	for( size_t i=0; i<getNumIndices(); i++ )
	{
		IDXDesc &theIndex = m_indices[i];
		if( !theIndex.bPrimary )
		{
			sqlCmd = "CREATE ";
			if( theIndex.bUnique )
			{
				sqlCmd += "UNIQUE ";
			}
			sqlCmd += "INDEX ";
			if( getField( theIndex.szName ) )
			{
				sqlCmd += tableName;
				sqlCmd += '_';
				sqlCmd += theIndex.szName;
				sqlCmd += "_IDX";
			}
			else
			{
				sqlCmd += theIndex.szName;
			}

			sqlCmd += " ON ";
			sqlCmd += tableName;
			sqlCmd += " (";
			for( Word j=0; j<theIndex.iFldsInKey; j++ )
			{
				if( j )
				{
					sqlCmd += ',';
				}

				theField = getField( theIndex.aiKeyFld[j] );
				sqlCmd += theField->szName;
//				if( theIndex.bDescending )
//					sqlCmd += " DESC";
			}
			sqlCmd += ')';

			sqlScript->addElement( sqlCmd );
		}
	}
}

void DatabaseSchema::createTableScript( TableSchema &theTable, DBtype dbType, ArrayOfStrings *sqlScript )
{
	ArrayOfStrings	masterTables;
	STRING		 	srcName, destName;

	theTable.startProcessing();
	theTable.getMasterTables( &masterTables );
	for( size_t i=0; i<masterTables.size(); i++ )
	{
		TableSchema &masterTable = getTableSchema( masterTables[i] );
		if( !masterTable.isFinished() && !masterTable.isInProcess() )
		{
			createTableScript( masterTable, dbType, sqlScript );
		}
	}

	theTable.createTableScript( dbType, this, sqlScript );

	theTable.stopProcessing();
}

void DatabaseSchema::createSchema( const DBconnector &dbConnector, DBtype dbType )
{
	ArrayOfStrings	sqlScript;

	initProcessing();
	for(
		Map<TableSchema>::iterator it = m_tables.begin(), endIT = m_tables.end();
		it != endIT;
		++it
	)
	{
		TableSchema &theTable = *it;
		if( !theTable.isFinished() && !theTable.isInProcess() )
		{
			createTableScript( theTable, dbType, &sqlScript );
		}
	}

	if( m_dbVersion > 0 )
	{
		STRING sqlCmd =
			"INSERT INTO CONFIG "
			"( ENTRY_NAME, INT_VALUE ) "
			"VALUES "
			"('dbVersion', ";
		sqlCmd += formatNumber( m_dbVersion );
		sqlCmd += ')';

		sqlScript.addElement( sqlCmd );
	}
	const char *tmpEnv = getenv( "TEMP" );
	if( tmpEnv )
	{
		STRING sqlFile = tmpEnv;
		if( !sqlFile.endsWith( DIRECTORY_DELIMITER ) )
		{
			sqlFile += DIRECTORY_DELIMITER;
		}
		sqlFile += dbConnector.m_aliasName;
		sqlFile += ".sql";

		ofstream of;

		of.open( sqlFile );
		if( of.is_open() )
		{
			for( size_t i=0; i<sqlScript.size(); i++ )
			{
				of << sqlScript[i] << "\n/\n";
			}
			of.close();
		}
	}

	connectDB( dbConnector );
	TQuery *sql = new TQuery( NULL );
	sql->DatabaseName = m_database->DatabaseName;

	try
	{
		if( m_database->IsSQLBased && !m_database->InTransaction )
		{
			m_database->StartTransaction();
		}
		for( size_t i=0; i<sqlScript.size(); i++ )
		{
			const STRING	&sqlCmd = sqlScript[i];
			doLogValue( sqlCmd );
			sql->SQL->Clear();
			sql->SQL->Add( (const char *)sqlCmd );

			sql->ExecSQL();
		}

		if( m_database->IsSQLBased && m_database->InTransaction )
		{
			m_database->Commit();
		}

		delete sql;
	}
	catch( ... )
	{
		if( m_database->IsSQLBased && m_database->InTransaction )
		{
			m_database->Rollback();
		}

		delete sql;
	}
}


void DatabaseSchema::createTableDropScript( TableSchema &theTable, DBtype dbType, ArrayOfStrings *sqlScript )
{
	ArrayOfStrings	detailTables;
	STRING				srcName, destName;

	theTable.startProcessing();
	theTable.getDetailTables( &detailTables );
	for( size_t i=0; i<detailTables.size(); i++ )
	{
		TableSchema &detailTable = getTableSchema( detailTables[i] );
		if( !detailTable.isFinished() && !detailTable.isInProcess() )
			createTableDropScript( detailTable, dbType, sqlScript );
	}

	STRING sqlCmd = "DROP TABLE ";
	sqlCmd += theTable.getTableName();

	sqlScript->addElement( sqlCmd );
	theTable.stopProcessing();
}

void DatabaseSchema::createDropScript( const DBconnector &dbConnector, DBtype dbType )
{
	ArrayOfStrings	sqlScript;

	initProcessing();
	for(
		Map<TableSchema>::iterator it = m_tables.begin(), endIT = m_tables.end();
		it != endIT;
		++it
	)
	{
		TableSchema &theTable = *it;
		if( !theTable.isFinished() && !theTable.isInProcess() )
		{
			createTableDropScript( theTable, dbType, &sqlScript );
		}
	}
	const char *tmpEnv = getenv( "TEMP" );
	if( tmpEnv )
	{
		STRING sqlFile = tmpEnv;
		if( !sqlFile.endsWith( DIRECTORY_DELIMITER ) )
			sqlFile += DIRECTORY_DELIMITER;
		sqlFile += dbConnector.m_aliasName;
		sqlFile += "_drop.sql";

		ofstream of;

		of.open( sqlFile );
		if( of.is_open() )
		{
			for( size_t i=0; i<sqlScript.size(); i++ )
			{
				of << sqlScript[i] << "\n/\n";
			}
			of.close();
		}
	}
}

void DatabaseSchema::createSynonymScript( const DBconnector &dbConnector, DBtype  )
{
	STRING		  	sqlCmd, tableName;
	ArrayOfStrings	sqlScript;

	for(
		Map<TableSchema>::iterator it = m_tables.begin(), endIT = m_tables.end();
		it != endIT;
		++it
	)
	{
		TableSchema &theTable = *it;
		tableName = theTable.getTableName();

		sqlCmd = "CREATE OR REPLACE PUBLIC SYNONYM ";
		sqlCmd += tableName;
		sqlCmd += " FOR ";
		sqlCmd += dbConnector.m_userName;
		sqlCmd += '.';
		sqlCmd += tableName;

		sqlScript.addElement( sqlCmd );

		sqlCmd = "GRANT SELECT ON ";
		sqlCmd += dbConnector.m_userName;
		sqlCmd += '.';
		sqlCmd += tableName;
		sqlCmd += " TO PUBLIC";

		sqlScript.addElement( sqlCmd );

		sqlCmd = "GRANT INSERT ON ";
		sqlCmd += dbConnector.m_userName;
		sqlCmd += '.';
		sqlCmd += tableName;
		sqlCmd += " TO PUBLIC";

		sqlScript.addElement( sqlCmd );

		sqlCmd = "GRANT UPDATE ON ";
		sqlCmd += dbConnector.m_userName;
		sqlCmd += '.';
		sqlCmd += tableName;
		sqlCmd += " TO PUBLIC";

		sqlScript.addElement( sqlCmd );

		sqlCmd = "GRANT DELETE ON ";
		sqlCmd += dbConnector.m_userName;
		sqlCmd += '.';
		sqlCmd += tableName;
		sqlCmd += " TO PUBLIC";

		sqlScript.addElement( sqlCmd );
	}
	const char *tmpEnv = getenv( "TEMP" );
	if( tmpEnv )
	{
		STRING sqlFile = tmpEnv;
		if( !sqlFile.endsWith( DIRECTORY_DELIMITER ) )
			sqlFile += DIRECTORY_DELIMITER;
		sqlFile += dbConnector.m_aliasName;
		sqlFile += "_synonym.sql";

		ofstream of;

		of.open( sqlFile );
		if( of.is_open() )
		{
			for( size_t i=0; i<sqlScript.size(); i++ )
			{
				of << sqlScript[i] << "\n/\n";
			}
			of.close();
		}
	}
}

/*
	========================
	checking table structure
	========================
*/
STRING	TableSchema::compareTable( TableSchema &masterTable )
{
	doEnterFunction("TableSchema::compareTable");

	STRING	compareResult;

	STRING	tableName = getTableName();

	if( getNumFields() != masterTable.getNumFields() )
	{
		compareResult += "Field count differ: ";
		compareResult += tableName;
		compareResult += '\n';
	}
	for( size_t i=0; i<masterTable.getNumFields(); i++ )
	{
		FLDDesc *masterField = masterTable.getField( i );
		FLDDesc *theField = getField( masterField->szName );
		if( !theField )
		{
			compareResult += "Field missing: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterField->szName;
			compareResult += '\n';
		}
		else
		{
			/*
				Integer expected
			*/
/*
			if( (masterField->iFldType == 5 || masterField->iFldType == 6)
			&& theField->iFldType == 7 && theField->iUnits2 == 0 )
			{
				theField->iFldType = masterField->iFldType;
				theField->iUnits1 = masterField->iUnits1;
			}
*/
			/*
				Short Integer
			*/
			if( masterField->iFldType == 5 && theField->iFldType == 6 )
				theField->iFldType = 5;

			/*
				number expected
			*/
			if( masterField->iFldType == 7 && theField->iFldType == 7 )
			{
				theField->iUnits1 = masterField->iUnits1;
				theField->iUnits2 = masterField->iUnits2;
			}
			/*
				memo expected
			*/
			if( masterField->iFldType == 3 && theField->iFldType == 3 )
			{
				theField->iUnits1 = masterField->iUnits1;
				theField->iUnits2 = masterField->iUnits2;
			}

			if( masterField->iFldType != theField->iFldType )
			{
				compareResult += "Bad field type: ";
				compareResult += tableName;
				compareResult += '.';
				compareResult += masterField->szName;
				compareResult += " expected ";
				compareResult += formatNumber( masterField->iFldType );
				compareResult += " found ";
				compareResult += formatNumber( theField->iFldType );
				compareResult += '\n';
			}
			if( masterField->iUnits1 != theField->iUnits1 )
			{
				compareResult += "Bad field size: ";
				compareResult += tableName;
				compareResult += '.';
				compareResult += masterField->szName;
				compareResult += " expected ";
				compareResult += formatNumber( masterField->iUnits1 );
				compareResult += " found ";
				compareResult += formatNumber( theField->iUnits1 );
				compareResult += '\n';
			}
			if( masterField->iUnits2 != theField->iUnits2 )
			{
				compareResult += "Bad field scale: ";
				compareResult += tableName;
				compareResult += '.';
				compareResult += masterField->szName;
				compareResult += " expected ";
				compareResult += formatNumber( masterField->iUnits2 );
				compareResult += " found ";
				compareResult += formatNumber( theField->iUnits2 );
				compareResult += '\n';
			}
		}
	}

	if( getNumIndices() != masterTable.getNumIndices() )
	{
		compareResult += "Index count differ: ";
		compareResult += tableName;
		compareResult += '\n';
	}
	for( size_t i=0; i<masterTable.getNumIndices(); i++ )
	{
		IDXDesc *masterIndex = masterTable.getIndex( i );

		/*
			search Index by name
		*/
		IDXDesc *theIndexByName = masterIndex->bPrimary
			? getPrimaryIndex()
			: getIndex( masterIndex->szName );

		if( !theIndexByName && getField( masterIndex->szName ) )
		{
			STRING indexName = tableName + '_' + masterIndex->szName + "_IDX";
			theIndexByName = getIndex( indexName );
		}

		/*
			search Index by definition
		*/
		IDXDesc *theIndexByDef = NULL;
		for( size_t j=0; j<m_indices.size(); j++ )
		{
			IDXDesc &myIndex = m_indices[j];
			if( myIndex.bUnique == masterIndex->bUnique
			&&  myIndex.iFldsInKey == masterIndex->iFldsInKey )
			{
				theIndexByDef = &myIndex;
				for( size_t k=0; (Word)k<myIndex.iFldsInKey; k++ )
				{
					FLDDesc *myField = getField( myIndex.aiKeyFld[k] );
					FLDDesc *masterField = masterTable.getField(
						masterIndex->aiKeyFld[k]
					);
					if( strcmpi( myField->szName, masterField->szName ) )
					{
						theIndexByDef = NULL;
/*v*/					break;
					}
				}
			}
			if( theIndexByDef )
/*v*/			break;
		}

		if( !masterIndex->szName[0] && theIndexByDef && !theIndexByName )
			theIndexByName = theIndexByDef;

		if( !theIndexByName )
		{
			compareResult += "Index missing: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterIndex->szName;
			compareResult += '\n';
		}
		else if( theIndexByName != theIndexByDef )
		{
			compareResult += "Index definition is wrong: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterIndex->szName;
			compareResult += '\n';
		}
	}

#if 0
	if( getNumValidations() != masterTable.getNumValidations() )
	{
		compareResult += "Validity count differ: ";
		compareResult += tableName;
		compareResult += " expected ";
		masterTable.getNumValidations() >> compareResult;
		compareResult += " found ";
		getNumValidations() >> compareResult;
		compareResult += '\n';
	}
#endif
	for( size_t i=0; i< masterTable.getNumValidations(); i++ )
	{
		VCHKDesc	*masterValidation = masterTable.getValidation( i );
		FLDDesc		*masterField = masterTable.getField( masterValidation->iFldNum );

		VCHKDesc	*theValidation = getValidation( masterField->szName );
		if( !theValidation )
		{
			compareResult += "Validation missing: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterField->szName;
			compareResult += '\n';
		}
		else if( (bool)theValidation->bRequired != (bool)masterValidation->bRequired )
		{
			compareResult += "Validation wrong: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterField->szName;
			compareResult += '\n';
		}
	}

	if( getNumReferences() != masterTable.getNumReferences() )
	{
		compareResult += "Reference count differ: ";
		compareResult += tableName;
		compareResult += " expected ";
		compareResult += formatNumber( masterTable.getNumReferences() );
		compareResult += " found ";
		compareResult += formatNumber( getNumReferences() );
		compareResult += '\n';
	}
	for( size_t i=0; i<masterTable.getNumReferences(); i++ )
	{
		RINTDesc *masterReference = masterTable.getReference( i );
		RINTDesc *theReference = getReference( masterReference->szRintName );
		if( !theReference )
		{
			compareResult += "Reference missing: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterReference->szRintName;
			compareResult += '\n';
		}
		else if( masterReference->eType != theReference->eType )
		{
			compareResult += "Reference has wrong type: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterReference->szRintName;
			compareResult += '\n';
		}
		else if( masterReference->iFldCount != theReference->iFldCount )
		{
			compareResult += "Reference has wrong field count: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterReference->szRintName;
			compareResult += '\n';
		}
		else if(
			strcmpi(
				masterReference->szTblName,
				stripPath( theReference->szTblName )
			)
		)
		{
			compareResult += "Reference has wrong reference table: ";
			compareResult += tableName;
			compareResult += '.';
			compareResult += masterReference->szRintName;
			compareResult += " expected ";
			compareResult += masterReference->szTblName;
			compareResult += " found ";
			compareResult += theReference->szTblName;
			compareResult += '\n';
		}
#if 0
		else
		{
			for( size_t fldIdx = 0; fldIdx<masterReference->iFldCount; fldIdx++ )
			{
				if( masterReference->aiThisTabFld[fldIdx] != theReference->aiThisTabFld[fldIdx]
				||  masterReference->aiOthTabFld[fldIdx] != theReference->aiOthTabFld[fldIdx] )
				{
					compareResult += "Reference has wrong definition: ";
					compareResult += tableName;
					compareResult += '.';
					compareResult += masterReference->szRintName;
					compareResult += '\n';
				}
			}
		}
#endif
	}

	return compareResult;
}

STRING DatabaseSchema::compareSchema( DatabaseSchema *master )
{
	doEnterFunction("DatabaseSchema::compareSchema");

	STRING compareResult;
	for( size_t tableIdx = 0; tableIdx<master->getNumTables(); tableIdx++ )
	{
		TableSchema &masterTable = master->getTableSchema( tableIdx );
		STRING		tableName = masterTable.getKey();

		if( !m_tables.hasElement( tableName ) )
		{
			compareResult += "Missing table ";
			compareResult += tableName;
			compareResult += '\n';
		}
		else
		{
			TableSchema	&theTable = getTableSchema( tableName );
			compareResult += theTable.compareTable( masterTable );
		}
	}
	return compareResult;
}

/*
	================
	Subclass actions
	================
*/
void DatabaseSchema::fieldAdded( const STRING &, const STRING & )
{
}

bool DatabaseSchema::checkFieldsBeforeInsert( TDataSet *, const STRING & )
{
	return true;
}

void DatabaseSchema::checkFieldsBeforePost( TDataSet *, const STRING & )
{
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

