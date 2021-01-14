/*
		Project:		GAKLIB
		Module:			dynamic.cpp
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

#include <gak/dynamic.h>
#include <gak/fmtNumber.h>
#include <gak/numericString.h>
#include <gak/compare.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

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

void DynamicVar::clear( void )
{
	if( fieldType == DV_VARCHAR )
	{
		getSTRINGAddress()->~STRING();
	}
	else if( fieldType == DV_DATE )
	{
		getDateAddress()->~Date();
	}
	else if( fieldType == DV_TIME )
	{
		getTimeAddress()->~Time();
	}

	init();
}

STRING DynamicVar::getString() const
{
	if( fieldType == DV_UNDEFINED )
	{
		return NULL_STRING;
	}
	else if( fieldType == DV_VARCHAR )
	{
		return *getSTRINGAddress();
	}
	else if( fieldType == DV_FLOAT )
	{
		return formatFloat( value.floatValue );
	}
	else if( fieldType == DV_INTEGER )
	{
		return formatNumber( value.integer );
	}
	else
	{
		throw InvalidConversionError();
	}
}

Date DynamicVar::getDate( void ) const
{
	Date	tmpVal;

	if( fieldType == DV_DATE )
	{
		return *getDateAddress();
	}

	return tmpVal;
}

Time DynamicVar::getTime( void ) const
{
	Time	tmpVal;

	if( fieldType == DV_TIME )
	{
		return *getTimeAddress();
	}

	return tmpVal;
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

DynamicVar DynamicVar::operator + ( const DynamicVar &other ) const
{
	if( getType() == DV_VARCHAR || other.getType() == DV_VARCHAR )
	{
		return DynamicVar( STRING(*this) + STRING(other) );
	}
	else if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
	{
		return DynamicVar(
			getNumeric<FloatType>() + other.getNumeric<FloatType>()
		);
	}
	else if( getType() == DV_INTEGER || other.getType() == DV_INTEGER )
	{
		return DynamicVar(
			getNumeric<int64>() + other.getNumeric<int64>()
		);
	}
	else
	{
		throw InvalidConversionError();
	}
}

void DynamicVar::toFmtStream( std::ostream &stream ) const
{
	if( fieldType == DV_VARCHAR )
	{
		stream << *getSTRINGAddress();
	}
	else if( fieldType == DV_DATE )
	{
		stream << *getDateAddress();
	}
	else if( fieldType == DV_TIME )
	{
		stream << *getTimeAddress();
	}
	else if( fieldType == DV_INTEGER )
	{
		stream << value.integer;
	}
	else if( fieldType == DV_FLOAT )
	{
		stream << value.floatValue;
	}
}

void DynamicVar::toBinaryStream( std::ostream &stream ) const
{
	gak::binaryToBinaryStream( stream, fieldType );
	if( fieldType == DV_VARCHAR )
	{
		getSTRINGAddress()->toBinaryStream( stream );
	}
	else if( fieldType == DV_DATE )
	{
		getDateAddress()->toBinaryStream( stream );
	}
	else if( fieldType == DV_TIME )
	{
		getTimeAddress()->toBinaryStream( stream );
	}
	else
	{
		gak::binaryToBinaryStream( stream, value );
	}
}

void DynamicVar::fromBinaryStream( std::istream &stream )
{
	init();
	gak::binaryFromBinaryStream( stream, &fieldType );
	if( fieldType == DV_VARCHAR )
	{
		new (this->value.varchar) STRING();
		getSTRINGAddress()->fromBinaryStream( stream );
	}
	else if( fieldType == DV_DATE )
	{
		new (value.dateValue) Date();
		getDateAddress()->fromBinaryStream( stream );
	}
	else if( fieldType == DV_TIME )
	{
		new (value.timeValue) Time();
		getTimeAddress()->fromBinaryStream( stream );
	}
	else
	{
		gak::binaryFromBinaryStream( stream, &value );
	}
}

int DynamicVar::compare( const DynamicVar &other ) const
{
	if( getType() == DV_VARCHAR || other.getType() == DV_VARCHAR )
	{
		return gak::compare( STRING(*this), STRING(other) );
	}
	else if( getType() == DV_DATE && other.getType() == DV_DATE )
	{
		return gak::compare( Date(*this), Date(other) );
	}
	else if( getType() == DV_TIME && other.getType() == DV_TIME )
	{
		return gak::compare( Time(*this), Time(other) );
	}
	else if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
	{
		return gak::compare(
			getNumeric<FloatType>(), other.getNumeric<FloatType>()
		);
	}
	else if( getType() == DV_INTEGER || other.getType() == DV_INTEGER )
	{
		return gak::compare(
			getNumeric<int64>(), other.getNumeric<int64>()
		);
	}
	else
	{
		throw InvalidConversionError();
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

