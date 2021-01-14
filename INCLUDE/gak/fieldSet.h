/*
		Project:		GAKLIB
		Module:			fieldSet.h
		Description:	Named array with dynamic values
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

#ifndef GAK_FIELD_SET_H
#define GAK_FIELD_SET_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/ci_string.h>
#include <gak/map.h>
#include <gak/date.h>
#include <gak/time.h>
#include <gak/numericString.h>
#include <gak/exception.h>
#include <gak/dynamic.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class Named_Field
{
	public:
	typedef CI_STRING key_type;

	private:
	CI_STRING	name;
	DynamicVar	value;

	public:
	Named_Field()
	{
	}
	Named_Field( const char *name, const DynamicVar &value )
	{
		setNameValue( name, value );
	}
	Named_Field( const Named_Field &source )
	{
		setNameValue( source.name, source.value );
	};
	Named_Field	&operator = ( const Named_Field &source )
	{
		setNameValue( source.name, source.value );

		return *this;
	}

	void setKey( const CI_STRING &name )
	{
		this->name = name;
	}
	void setValue( const DynamicVar &value )
	{
		this->value = value;
	}
	void setNameValue( const char *name, const DynamicVar &value )
	{
		setKey( name );
		setValue( value );
	}
	const CI_STRING &getKey( void ) const
	{
		return name;
	}
	DynamicVar &getValue( void )
	{
		return value;
	}
	const DynamicVar &getValue( void ) const
	{
		return value;
	}
	operator DynamicVar & ( void )
	{
		return getValue();
	}
	DynamicVar::DV_TYPE getType( void ) const
	{
		return value.getType();
	}

	int operator ! ( void ) const
	{
		return !value;
	}
};

class FieldSet : public UnorderedMap<Named_Field>
{
	public:
	void updateField( const char *name, const DynamicVar &value );
	void addField( const char *name, const DynamicVar &value )
	{
		Named_Field		&elem = createElement();
		elem.setNameValue( name, value );
	}
	std::size_t getNumFields( void ) const
	{
		return size();
	}
	DynamicVar &operator [] ( const char *name )
	{
		return getElementByKey( name ).getValue();
	}
	const DynamicVar &operator [] ( const char *name ) const
	{
		return getElementByKey( name ).getValue();
	}
	const Named_Field &operator [] ( size_t pos ) const
	{
		return getConstElementAt( pos );
	}
	Named_Field &operator [] ( size_t pos )
	{
		return getMutableElementAt( pos );
	}

	void loadConfigFile( const char *fileName );
	void saveConfigFile( const char *fileName ) const;
};

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
