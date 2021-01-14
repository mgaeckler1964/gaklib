/*
		Project:		GAKLIB
		Module:			dynamic.h
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

#ifndef GAK_DYNAMIC_H
#define GAK_DYNAMIC_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/types.h>
#include <gak/date.h>
#include <gak/time.h>
#include <gak/string.h>
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

class DynamicVar
{
	friend class STRING;
	friend class Date;
	friend class Time;

	public:
	typedef double FloatType;

	enum DV_TYPE
	{
		DV_UNDEFINED, DV_VARCHAR, DV_INTEGER, DV_FLOAT, DV_DATE, DV_TIME
	};

	private:
	DV_TYPE		fieldType;
	union value_union
	{
		int64		integer;
		FloatType	floatValue;
		char		dateValue[sizeof( Date )];
		char		timeValue[sizeof( Time )];
		char		varchar[sizeof( STRING )];
	} value;

	STRING *getSTRINGAddress()
	{
		return reinterpret_cast<STRING*>(value.varchar);
	}
	const STRING *getSTRINGAddress() const
	{
		return reinterpret_cast<STRING const *>(value.varchar);
	}
	Date *getDateAddress()
	{
		return reinterpret_cast<Date*>(value.dateValue);
	}
	const Date *getDateAddress() const
	{
		return reinterpret_cast<const Date*>(value.dateValue);
	}
	Time *getTimeAddress()
	{
		return reinterpret_cast<Time*>(value.timeValue);
	}
	const Time *getTimeAddress() const
	{
		return reinterpret_cast<const Time*>(value.timeValue);
	}
	STRING getString() const;
	Date getDate( void ) const;
	Time getTime( void ) const;

	void init( void )
	{
		fieldType = DV_UNDEFINED;
	}
	void clear( void );

	// conversions
	//
	template<typename NUMERIC_T>
	NUMERIC_T getNumeric() const;

	template<typename NUMERIC_T>
	void setNumeric( NUMERIC_T value )
	{
#ifdef __BORLANDC__
#   pragma warn -ccc
#	pragma warn -rch
#endif
		if( std::numeric_limits<NUMERIC_T>::is_integer )
		{
			fieldType = DV_INTEGER;
			this->value.integer = int64(value);
		}
		else if( std::numeric_limits<NUMERIC_T>::max_exponent10 )
		{
			fieldType = DV_FLOAT;
			this->value.floatValue = static_cast<FloatType>(value);
		}
		else
		{
			throw InvalidConversionError();
		}
#ifdef __BORLANDC__
#   pragma warn +ccc
#	pragma warn +rch
#endif
	}


	public:
	// undefined
	// =========
	DynamicVar()
	{
		init();
	}

	// using as varchar
	// ================
	DynamicVar( const STRING &varchar )
	{
		fieldType = DV_VARCHAR;
		new (value.varchar) STRING( varchar );
	}
	DynamicVar( const char *varchar )
	{
		fieldType = DV_VARCHAR;
		new (value.varchar) STRING( varchar );
	}

	DynamicVar &operator = ( const STRING &varchar )
	{
		clear();

		fieldType = DV_VARCHAR;
		new (value.varchar) STRING( varchar );

		return *this;
	}
	DynamicVar &operator = ( const char *varchar )
	{
		clear();

		fieldType = DV_VARCHAR;
		new (value.varchar) STRING( varchar );

		return *this;
	}

	// using as integer
	// ================
	DynamicVar( int64 value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( int64 value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( uint64 value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( uint64 value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( signed long value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( signed long value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( unsigned long value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( unsigned long value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( signed int value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( signed int value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( unsigned int value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( unsigned int value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( signed short value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( signed short value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( unsigned short value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( unsigned short value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( char value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( char value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( signed char value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( signed char value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( unsigned char value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( unsigned char value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( bool value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( bool value )
	{
		clear();

		setNumeric( value );

		return *this;
	}

	operator char ( void ) const
	{
		return getNumeric<char>();
	}
	operator signed char ( void ) const
	{
		return getNumeric<signed char>();
	}
	operator unsigned char ( void ) const
	{
		return getNumeric<unsigned char>();
	}
	operator signed short ( void ) const
	{
		return getNumeric<signed short>();
	}
	operator unsigned short ( void ) const
	{
		return getNumeric<unsigned short>();
	}
	operator signed int ( void ) const
	{
		return getNumeric<signed int>();
	}
	operator unsigned int ( void ) const
	{
		return getNumeric<unsigned int>();
	}
	operator signed long ( void ) const
	{
		return getNumeric<signed long>();
	}
	operator unsigned long ( void ) const
	{
		return getNumeric<unsigned long>();
	}
	operator int64 ( void ) const
	{
		return getNumeric<int64>();
	}
	operator uint64 ( void ) const
	{
		return getNumeric<uint64>();
	}


	// using as float
	// ==============
	DynamicVar( long double value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( long double value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( double value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( double value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	DynamicVar( float value )
	{
		setNumeric( value );
	}
	DynamicVar &operator = ( float value )
	{
		clear();

		setNumeric( value );

		return *this;
	}
	operator float ( void ) const
	{
		return getNumeric<float>();
	}
	operator double ( void ) const
	{
		return getNumeric<double>();
	}
	operator long double ( void ) const
	{
		return getNumeric<long double>();
	}

	// using as date
	// =============
	DynamicVar( const Date &dateValue )
	{
		fieldType = DV_DATE;
		new (value.dateValue) Date( dateValue );
	}
	DynamicVar &operator = ( const Date &dateValue )
	{
		if( fieldType != DV_DATE )
		{
			clear();

			fieldType = DV_DATE;
			new (value.dateValue) Date( dateValue );
		}
		else
		{
			*getDateAddress() = dateValue;
		}

		return *this;
	}

	// using as time
	// =============
	DynamicVar( const Time &timeValue )
	{
		fieldType = DV_TIME;
		new (value.timeValue) Time( timeValue );
	}
	DynamicVar &operator = ( const Time &timeValue )
	{
		if( fieldType != DV_TIME )
		{
			clear();

			fieldType = DV_TIME;
			new (value.timeValue) Time( timeValue );
		}
		else
		{
			*getTimeAddress() = timeValue;
		}

		return *this;
	}

	// copying
	// =======
	DynamicVar( const DynamicVar &source )
	{
		fieldType = source.fieldType;
		if( fieldType == DV_VARCHAR )
		{
			new (value.varchar) STRING( *source.getSTRINGAddress() );
		}
		else if( fieldType == DV_DATE )
		{
			new (value.dateValue) Date( *source.getDateAddress() );
		}
		else if( fieldType == DV_TIME )
		{
			new (value.timeValue) Time( *source.getTimeAddress() );
		}
		else
		{
			value = source.value;
		}
	}
	DynamicVar &operator = ( const DynamicVar &source )
	{
		clear();

		fieldType = source.fieldType;
		if( fieldType == DV_VARCHAR )
		{
			new (value.varchar) STRING( *source.getSTRINGAddress() );
		}
		else if( fieldType == DV_DATE )
		{
			new (value.dateValue) Date( *source.getDateAddress() );
		}
		else if( fieldType == DV_TIME )
		{
			new (value.timeValue) Time( *source.getTimeAddress() );
		}
		else
		{
			value = source.value;
		}

		return *this;
	}

	// miscelaneous
	// =============
	bool isDefined( void ) const
	{
		return fieldType != DV_UNDEFINED;
	}
	DV_TYPE getType( void ) const
	{
		return fieldType;
	}
	// removing from memory
	// ====================
	~DynamicVar()
	{
		clear();
	}

	// operators
	// =========

	// arithmetik operators
	DynamicVar operator + ( const DynamicVar &other ) const;

	DynamicVar operator - ( const DynamicVar &other ) const
	{
		if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
		{
			return DynamicVar( getNumeric<FloatType>() - other.getNumeric<FloatType>() );
		}
		return DynamicVar( getNumeric<int64>() - other.getNumeric<int64>() );
	}
	DynamicVar operator * ( const DynamicVar &other ) const
	{
		if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
		{
			return DynamicVar( getNumeric<FloatType>() * other.getNumeric<FloatType>() );
		}
		return DynamicVar( getNumeric<int64>() * other.getNumeric<int64>() );
	}
	DynamicVar operator / ( const DynamicVar &other ) const
	{
		if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
		{
			return DynamicVar( getNumeric<FloatType>() / other.getNumeric<FloatType>() );
		}
		return DynamicVar( getNumeric<int64>() / other.getNumeric<int64>() );
	}
	DynamicVar operator % ( const DynamicVar &other ) const
	{
		if( getType() != DV_INTEGER || other.getType()  != DV_INTEGER )
		{
			throw InvalidConversionError();
		}
		return DynamicVar( getNumeric<int64>() % other.getNumeric<int64>() );
	}

	// logical operators
	int operator ! ( void ) const
	{
		return fieldType == DV_UNDEFINED
			|| (fieldType == DV_INTEGER && !value.integer)
			|| (fieldType == DV_FLOAT && !value.floatValue)
			|| (fieldType == DV_VARCHAR && !value.varchar);
	}
	operator bool ( void ) const
	{
		return !this->operator!();
	}
	DynamicVar operator && ( const DynamicVar &other ) const
	{
		if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
		{
			return DynamicVar(
				static_cast<FloatType>(
					getNumeric<FloatType>() && other.getNumeric<FloatType>()
				)
			);
		}
		return DynamicVar(
			int64( getNumeric<int64>() && other.getNumeric<int64>() )
		);
	}
	DynamicVar operator || ( const DynamicVar &other ) const
	{
		if( getType() == DV_FLOAT || other.getType() == DV_FLOAT )
		{
			return DynamicVar(
				static_cast<FloatType>(
					getNumeric<FloatType>() || other.getNumeric<FloatType>()
				)
			);
		}
		return DynamicVar(
			int64( getNumeric<int64>() || other.getNumeric<int64>() )
		);
	}

	// binary operators
	DynamicVar operator & ( const DynamicVar &other ) const
	{
		if( getType() != DV_INTEGER || other.getType() != DV_INTEGER )
		{
			throw InvalidConversionError();
		}
		return DynamicVar( getNumeric<int64>() & other.getNumeric<int64>() );
	}
	DynamicVar operator | ( const DynamicVar &other ) const
	{
		if( getType() != DV_INTEGER || other.getType() != DV_INTEGER )
		{
			throw InvalidConversionError();
		}
		return DynamicVar( getNumeric<int64>() | other.getNumeric<int64>() );
	}
	DynamicVar operator ^ ( const DynamicVar &other ) const
	{
		if( getType() != DV_INTEGER || other.getType() != DV_INTEGER )
		{
			throw InvalidConversionError();
		}
		return DynamicVar( getNumeric<int64>() ^ other.getNumeric<int64>() );
	}
	DynamicVar operator ~ () const
	{
		if( getType() != DV_INTEGER )
		{
			throw InvalidConversionError();
		}
		return DynamicVar( ~getNumeric<int64>() );
	}

	void toFmtStream( std::ostream &stream ) const;
	void toBinaryStream( std::ostream &stream ) const;
	void fromBinaryStream( std::istream &stream );

	int compare( const DynamicVar &other ) const;
};

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

template<typename NUMERIC_T>
NUMERIC_T DynamicVar::getNumeric() const
{
	if( fieldType == DV_UNDEFINED )
	{
		return NUMERIC_T(0);
	}
	else if( fieldType == DV_VARCHAR )
	{
		return getSTRINGAddress()->getValueN<NUMERIC_T>();
	}
	else if( fieldType == DV_INTEGER )
	{
		return NUMERIC_T(value.integer);
	}
	else if( fieldType == DV_FLOAT )
	{
		return NUMERIC_T(value.floatValue);
	}
	else
	{
		throw InvalidConversionError();
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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

inline std::ostream &operator << ( std::ostream &stream, const DynamicVar &value )
{
	value.toFmtStream( stream );
	return stream;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
