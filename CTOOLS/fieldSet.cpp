/*
		Project:		GAKLIB
		Module:			fieldSet.cpp
		Description:	Named fields in an assoc with dynamic values
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <fstream>

#include <gak/fieldSet.h>
#include <gak/t_string.h>

// -------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void FieldSet::updateField( const char *name, const DynamicVar &value )
{
	Named_Field	&theField = getElementByKey( name );
	theField.setValue( value );
}

/*
DynamicVar &FieldSet::operator [] ( const char *name )
{
	int					i, numFields;
	static DynamicVar	undefined;

	numFields = getNumFields();
	for( i=0; i<numFields; i++ )
	{
		if( !strcmpi( name, fields[i].getName() ) )
			return fields[i].getValue();
	}

	return undefined;
}
*/

void FieldSet::loadConfigFile( const char *fileName )
{
	T_STRING	line;
	STRING		name, value;

	std::ifstream	fp( fileName );
	if( fp )
	{
		while( fp >> line )
		{
			name = line.getFirstToken( "=" );
			value = line.getNextToken();
			if( !name.isEmpty() && !value.isEmpty() )
				updateField( name, value.c_str() );
		}
	}
}

void FieldSet::saveConfigFile( const char *fileName ) const
{
	std::ofstream fp( fileName );
	if( fp )
	{
		for( const_iterator it = cbegin(), endIT = cend(); it != endIT; ++it )
		{
			const Named_Field	&theField = *it;

			STRING	name = theField.getKey();
			STRING	value = theField.getValue();
			if( !name.isEmpty() && !value.isEmpty() )
			{
				fp << name << '=' << value << '\n';
			}
		}
	}
}

}	// namespace gak


