/*
		Project:		GAKLIB
		Module:			dynamic.h
		Description:	Dynamic data type
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

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4127 )	// Bedingter Ausdruck ist konstant
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
	DV_TYPE		m_fieldType;
	union value_union
	{
		int64		integer;
		FloatType	floatValue;
		char		dateValue[sizeof( Date )];
		char		timeValue[sizeof( Time )];
		char		varchar[sizeof( STRING )];
	} m_value;

	STRING *getSTRINGAddress()
	{
		return reinterpret_cast<STRING*>(m_value.varchar);
	}
	const STRING *getSTRINGAddress() const
	{
		return reinterpret_cast<STRING const *>(m_value.varchar);
	}
	Date *getDateAddress()
	{
		return reinterpret_cast<Date*>(m_value.dateValue);
	}
	const Date *getDateAddress() const
	{
		return reinterpret_cast<const Date*>(m_value.dateValue);
	}
	Time *getTimeAddress()
	{
		return reinterpret_cast<Time*>(m_value.timeValue);
	}
	const Time *getTimeAddress() const
	{
		return reinterpret_cast<const Time*>(m_value.timeValue);
	}
	STRING getString() const;
	Date getDate() const;
	Time getTime() const;

	void init()
	{
		m_fieldType = DV_UNDEFINED;
	}
	void clear();

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
			m_fieldType = DV_INTEGER;
			m_value.integer = int64(value);
		}
		else if( std::numeric_limits<NUMERIC_T>::max_exponent10 )
		{
			m_fieldType = DV_FLOAT;
			m_value.floatValue = static_cast<FloatType>(value);
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
		m_fieldType = DV_VARCHAR;
		new (m_value.varchar) STRING( varchar );
	}
	DynamicVar( const char *varchar )
	{
		m_fieldType = DV_VARCHAR;
		new (m_value.varchar) STRING( varchar );
	}

	DynamicVar &operator = ( const STRING &varchar )
	{
		clear();

		m_fieldType = DV_VARCHAR;
		new (m_value.varchar) STRING( varchar );

		return *this;
	}
	DynamicVar &operator = ( const char *varchar )
	{
		clear();

		m_fieldType = DV_VARCHAR;
		new (m_value.varchar) STRING( varchar );

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

	operator char () const
	{
		return getNumeric<char>();
	}
	operator signed char () const
	{
		return getNumeric<signed char>();
	}
	operator unsigned char () const
	{
		return getNumeric<unsigned char>();
	}
	operator signed short () const
	{
		return getNumeric<signed short>();
	}
	operator unsigned short () const
	{
		return getNumeric<unsigned short>();
	}
	operator signed int () const
	{
		return getNumeric<signed int>();
	}
	operator unsigned int () const
	{
		return getNumeric<unsigned int>();
	}
	operator signed long () const
	{
		return getNumeric<signed long>();
	}
	operator unsigned long () const
	{
		return getNumeric<unsigned long>();
	}
	operator int64 () const
	{
		return getNumeric<int64>();
	}
	operator uint64 () const
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
	operator float () const
	{
		return getNumeric<float>();
	}
	operator double () const
	{
		return getNumeric<double>();
	}
	operator long double () const
	{
		return getNumeric<long double>();
	}

	// using as date
	// =============
	DynamicVar( const Date &dateValue )
	{
		m_fieldType = DV_DATE;
		new (m_value.dateValue) Date( dateValue );
	}
	DynamicVar &operator = ( const Date &dateValue )
	{
		if( m_fieldType != DV_DATE )
		{
			clear();

			m_fieldType = DV_DATE;
			new (m_value.dateValue) Date( dateValue );
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
		m_fieldType = DV_TIME;
		new (m_value.timeValue) Time( timeValue );
	}
	DynamicVar &operator = ( const Time &timeValue )
	{
		if( m_fieldType != DV_TIME )
		{
			clear();

			m_fieldType = DV_TIME;
			new (m_value.timeValue) Time( timeValue );
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
		m_fieldType = source.m_fieldType;
		if( m_fieldType == DV_VARCHAR )
		{
			new (m_value.varchar) STRING( *source.getSTRINGAddress() );
		}
		else if( m_fieldType == DV_DATE )
		{
			new (m_value.dateValue) Date( *source.getDateAddress() );
		}
		else if( m_fieldType == DV_TIME )
		{
			new (m_value.timeValue) Time( *source.getTimeAddress() );
		}
		else
		{
			m_value = source.m_value;
		}
	}
	DynamicVar &operator = ( const DynamicVar &source )
	{
		clear();

		m_fieldType = source.m_fieldType;
		if( m_fieldType == DV_VARCHAR )
		{
			new (m_value.varchar) STRING( *source.getSTRINGAddress() );
		}
		else if( m_fieldType == DV_DATE )
		{
			new (m_value.dateValue) Date( *source.getDateAddress() );
		}
		else if( m_fieldType == DV_TIME )
		{
			new (m_value.timeValue) Time( *source.getTimeAddress() );
		}
		else
		{
			m_value = source.m_value;
		}

		return *this;
	}

	// miscelaneous
	// =============
	bool isDefined() const
	{
		return m_fieldType != DV_UNDEFINED;
	}
	DV_TYPE getType() const
	{
		return m_fieldType;
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
	int operator ! () const
	{
		return m_fieldType == DV_UNDEFINED
			|| (m_fieldType == DV_INTEGER && !m_value.integer)
			|| (m_fieldType == DV_FLOAT && !m_value.floatValue)
			|| (m_fieldType == DV_VARCHAR && !m_value.varchar);
	}
	operator bool () const
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
	if( m_fieldType == DV_UNDEFINED )
	{
		return NUMERIC_T(0);
	}
	else if( m_fieldType == DV_VARCHAR )
	{
		return getSTRINGAddress()->getValueN<NUMERIC_T>();
	}
	else if( m_fieldType == DV_INTEGER )
	{
		return NUMERIC_T(m_value.integer);
	}
	else if( m_fieldType == DV_FLOAT )
	{
		return NUMERIC_T(m_value.floatValue);
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

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
