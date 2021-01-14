/*
		Project:		GAKLIB
		Module:			exportCSV.cpp
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

void exportTable2CSV(
	TDataSet *theTable,
	const char *csvFile,
	bool convertUTF8,
	bool (*traceFunc)( TDataSet *theTable )
)
{
	doEnterFunction("exportTable2CSV");

	int			j;
	TField		*theSourceField;
	TFieldType	theSourceType;
	ofstream	fStream( csvFile );

	if( fStream.is_open() )
	{
		fStream.setf( ios_base::fixed );

		for( j=0; j<theTable->FieldCount; j++ )
		{
			theSourceField = theTable->Fields->Fields[j];
			fStream << theSourceField->FieldName.LowerCase().c_str();
			if( j<theTable->FieldCount-1 )
				fStream << ',';
		}
		fStream << '\n';

		for( theTable->First(); !theTable->Eof; theTable->Next() )
		{
			doLogPosition();

			for( j=0; j<theTable->FieldCount; j++ )
			{
				theSourceField = theTable->Fields->Fields[j];
				if( !theSourceField->IsNull )
				{
					theSourceType = theSourceField->DataType;
					switch( theSourceType )
					{
						case ftFixedChar:
						case ftString:
						case ftMemo:
						case ftOraClob:
						{
#if __BORLANDC__ >= 0x620
							STRING	value = AnsiString( theSourceField->AsString ).c_str();
#else
							STRING	value = theSourceField->AsString.c_str();
#endif
							if( convertUTF8 )
								value = value.encodeUTF8();

							fStream << value.csvString();
							break;
						}
						case ftSmallint:
						case ftLargeint:
						case ftInteger:
						case ftAutoInc:
							fStream << theSourceField->AsInteger;
							break;
						case ftBoolean:
							fStream << theSourceField->AsBoolean;
							break;
						case ftFloat:
						case ftCurrency:
							fStream << theSourceField->AsFloat;
							break;
						case ftDate:
							fStream << theSourceField->AsDateTime.DateString().c_str();
							break;
						case ftTime:
							fStream << theSourceField->AsDateTime.TimeString().c_str();
							break;
						case ftDateTime:
							fStream << theSourceField->AsDateTime.DateTimeString().c_str();
							break;
						default:
							doLogValue( theSourceType );
							doLogValue( theSourceType );
							throw( Exception( "Unbekannter Typ" ) );
					}
				}
				if( j<theTable->FieldCount-1 )
					fStream << ',';
			}
			fStream << '\n';
			if( traceFunc && traceFunc( theTable ) )
/*v*/			break;
		}

		fStream.close();
	}
}

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

