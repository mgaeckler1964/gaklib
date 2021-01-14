/*
		Project:		GAKLIB
		Module:			CSV.CPP
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/csv.h>

#include <gak/gaklib.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static bool getNextCsvField( std::istream &is, STRING &theField, char *delimiter )
{
	bool	delimiterFound = false;
	bool	stringField = false;
	bool	eolFound = false;
	char  	c, lastChar;

	theField = "";
	lastChar = 0;

	// search next "
	while( true )
	{
		c = (char)is.get();
		if( is.eof() || is.fail() )
		{
			eolFound = true;
			break;
		}
		else if( c == '\n' )
		{
			if( stringField && !delimiterFound )
				theField += c;
			else
			{
				eolFound = true;
				break;
			}
		}
		else if( c == '"' )
		{
			if( lastChar == c )
			{
				theField += c;
				delimiterFound = false;
				c = lastChar = 0;
			}
			else if( stringField )
				delimiterFound = true;
			else if( theField == "" )
			{
				stringField = true;
				c = lastChar = 0;
			}
			else
				theField += c;
		}
		else if( c == ',' || c == ';' )
		{
			if( stringField && !delimiterFound )
				theField += c;
			else if( delimiter && *delimiter && *delimiter != c )
				theField += c;
			else
			{
				if( delimiter )
					*delimiter = c;
				break;
			}
		}
		else if( c == '\\' )
		{
			c = (char)is.get();
			if( c != '\n' )
				theField += c;
		}
		else if( !delimiterFound && c != '\r' )
			theField += c;

		lastChar = c;
	}

	return eolFound;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

void writeCSVLine( std::ostream &os, ArrayOfStrings &source, char delimiter )
{
	size_t	i, numFields = source.size();

	for( i=0; i<numFields; i++ )
	{
		if( i )
			os << delimiter;

		STRING	field = source[i];
		if( field.searchChar( ',' ) != field.no_index
		||  field.searchChar( ';' ) != field.no_index
		||  field.searchChar( '\n' ) != field.no_index
		||  field.searchChar( '\r' ) != field.no_index )
			os << '"' << field << '"';
		else
			os << field;
	}

	os << '\n';
}

char readCSVLine( std::istream &is, ArrayOfStrings *destination, char delimiter )
{
	bool	eolFound = false;
	STRING	theField;

	destination->clear();

	while( !is.fail() && !is.eof() && !eolFound )
	{
		eolFound = getNextCsvField( is, theField, &delimiter );
		if( !eolFound || theField[0U] )
		{
			*destination += theField;
		}
	}

	return delimiter;
}

size_t createFIELD_SET( ArrayOfStrings &fieldDef, ArrayOfStrings &fieldData, FieldSet *record )
{
	size_t	numFieldDefs	= fieldDef.size();
	size_t	numElems		= math::min( numFieldDefs, fieldData.size() );

	record->clear();

	for( size_t i=0; i<numElems; i++ )
		record->addField( (const char*)(fieldDef[i]), (const char*)(fieldData[i]) );

	for( size_t i=numElems; i<numFieldDefs; i++ )
		record->addField( (const char*)(fieldDef[i]), "" );

	return numElems;
}

size_t readFIELD_SET( std::istream &is, ArrayOfStrings &fieldDef, FieldSet *record )
{
	ArrayOfStrings	fieldData;

	readCSVLine( is, &fieldData );

	return createFIELD_SET( fieldDef, fieldData, record );;
}

}	// namespace gak


