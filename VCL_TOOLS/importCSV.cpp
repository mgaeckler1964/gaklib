/*
		Project:		GAKLIB
		Module:			importCSV.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdio.h>
#include <fstream.h>
#include <gak/csv.h>


#include <vcl.h>
#include <dbtables.hpp>

#include <gak/vcl_tools.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
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

int importTableFromCSV( const char *csvFile, TDBDataSet *theTable, const char *ID, const char *idTable )
{
	TLocateOptions	Opts;
	ArrayOfStrings	fieldDef;
	FieldSet 	  	record;
	int			  	numRecords = 0, j;
	TField		  	*theSourceField;
	TFieldType	  	theSourceType;

	ifstream fp( csvFile );
	if( fp.is_open() )
	{
		readCSVLine( fp, &fieldDef );
		while( !fp.eof() && !fp.fail() && readFIELD_SET( fp, fieldDef, &record ) )
		{
			if( ID && idTable )
			{
				if( !record[ID].isDefined()
				|| !theTable->Locate( ID, long(record[ID]), Opts ) )
				{
					theTable->Insert();
#if __BORLANDC__ >= 0x620
					theTable->FieldByName( ID )->AsInteger =
						getNewMaxValue(
							AnsiString( theTable->DatabaseName ).c_str(),
							idTable, ID
						);
#else
					theTable->FieldByName( ID )->AsInteger =
						getNewMaxValue(
							theTable->DatabaseName.c_str(),
							idTable, ID
						);
#endif
				}
				else
					theTable->Edit();
			}
			else
				theTable->Insert();

			try
			{
				for( j=0; j<theTable->FieldCount; j++ )
				{
					theSourceField = theTable->Fields->Fields[j];
					AnsiString FieldName = theSourceField->FieldName;
					if( record[FieldName.c_str()].isDefined() )
					{
						theSourceType = theSourceField->DataType;
						switch( theSourceType )
						{
							case ftString:
							case ftMemo:
								theSourceField->AsString = static_cast<const char*>(
									STRING(record[FieldName.c_str()])
								);
								break;
							case ftSmallint:
							case ftInteger:
							{
								const DynamicVar	&value = record[FieldName.c_str()];
								STRING 				tmp = value;
								if( !tmp.isEmpty() )
									theSourceField->AsInteger = long(value);
								break;
							}
							case ftFloat:
							{
								const DynamicVar	&value = record[FieldName.c_str()];
								STRING 				tmp = value;
								if( !tmp.isEmpty() )
									theSourceField->AsFloat = double(value);
								break;
							}
							case ftTime:
							case ftDate:
							case ftDateTime:
							{
#if __BORLANDC__ >= 0x620
								UnicodeString	tmpVal = static_cast<const char*>(
									STRING(record[FieldName.c_str()])
								);
#else
								AnsiString	tmpVal = static_cast<const char*>(
									STRING(record[FieldName.c_str()])
								);
#endif
								if( tmpVal > "" )
									theSourceField->AsDateTime = tmpVal;
								break;
							}
							default:
								throw( new Exception( "Unbekannter Typ" ) );
						}
					}
				}
				theTable->Post();
				numRecords++;
			}
			catch( ... )
			{
				theTable->CancelUpdates();
			}
		}
	}

	return numRecords;
}

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

