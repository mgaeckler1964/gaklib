/*
		Project:		GAKLIB
		Module:			makeXML.cpp
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

xml::Any *makeQueryXML( TDataSet *theQuery, const char *tag, int fltRound )
{
	TField		*theSourceField;
	TFieldType	theSourceType;
	AnsiString	fieldName;
	xml::Any	*theXML = new xml::Any( tag );

	for( int i = 0; i<theQuery->FieldCount; i++ )
	{
		theSourceField = theQuery->Fields->Fields[i];
		fieldName = theSourceField->FieldName;

		theSourceType = theSourceField->DataType;
		if( theSourceType ==  ftMemo  || theSourceType == ftOraClob )
			theXML->addObject(
				new xml::Any(
					fieldName.LowerCase().c_str(),
#if __BORLANDC__ >= 0x620
					AnsiString( theSourceField->AsString ).c_str()
#else
					theSourceField->AsString.c_str()
#endif
				)
			);
		else if( theSourceType == ftFloat || theSourceType == ftCurrency )
			theXML->setFloatAttribute(
				fieldName.LowerCase().c_str(),
				theSourceField->AsFloat, fltRound
			);
		else if( theSourceType == ftDateTime )
		{
			TDateTime theDate = theSourceField->AsDateTime;
			if( !(int)theDate )
			{
				theXML->setStringAttribute(
					fieldName.LowerCase().c_str(),
					theDate.TimeString().c_str()
				);
			}
			else
			{
				theXML->setStringAttribute(
					fieldName.LowerCase().c_str(),
					theSourceField->AsString.c_str()
				);
			}
		}
		else
			theXML->setStringAttribute(
				fieldName.LowerCase().c_str(),
				theSourceField->AsString.c_str()
			);
	}

	return theXML;
}

xml::Any *exportTable2XML( TDataSet *theTable, const char *xmlRoot, const char *xmlEntry, int fltRound )
{
	xml::Any *theData = new xml::Any( xmlRoot );
	for( theTable->First(); !theTable->Eof; theTable->Next() )
		theData->addObject( makeQueryXML( theTable, xmlEntry, fltRound ) );

	return theData;
}

}	// namespace vcl
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

