/*
		Project:		GAKLIB
		Module:			keyValuePair.h
		Description:	Template for a pair of values
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

#ifndef GAK_PAIR_H
#define GAK_PAIR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/iostream.h>

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

/**
	@brief Helper class for PairMap and UnorderedPairMap

	this class defines a pair of key values and user data

	@tparam KEY the type of the key values
	@tparam VALUE the type of the user values
*/
template <typename KEY, typename VALUE> 
class KeyValuePair
{
	public:
	/// the type of the key values
	typedef KEY		key_type;
	/// the type of the user data
	typedef VALUE	value_type;

	private:
	KEY		m_key;
	VALUE	m_value;

	public:
	/// creates a new pair of data
	KeyValuePair( const key_type &key=key_type(), const value_type &value=value_type() ) 
		: m_key( key ), m_value( value )
	{
	}

	/// returns the key value
	const KEY &getKey( void ) const
	{
		return m_key;
	}
	/// returns the key value
	KEY &getKey( void )
	{
		return m_key;
	}
	/// updates the key to a given value
	void setKey( const KEY &key )
	{
		m_key = key;
	}

	/// return the user data
	const VALUE &getValue( void ) const
	{
		return m_value;
	}
	/// return the user data
	VALUE &getValue( void )
	{
		return m_value;
	}
	/// updates the user data to a given value
	void setValue( const VALUE &value )
	{
		m_value = value;
	}

	/// writes the item to a given binary stream
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, m_key );
		gak::toBinaryStream( stream, m_value );
	}
	/// reads the item from a given binary stream
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &m_key );
		gak::fromBinaryStream( stream, &m_value );
	}

	/// compares the item's key value with a given key value
	int compare( const KEY &key ) const
	{
		return gak::compare( m_key, key );
	}
	/// compares the item with another value
	int compare( const KeyValuePair<KEY,VALUE> &oper ) const
	{
		int result = gak::compare( m_key, oper.m_key );
		if( !result )
		{
			result = gak::compare( m_value, oper.m_value );
		}

		return result;
	}

	void moveFrom( KeyValuePair<KEY, VALUE> &source )
	{
		moveAssign( this->m_key, source.m_key );
		moveAssign( this->m_value, source.m_value );
	}
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

template <typename KEY, typename VALUE>
inline void moveAssign( KeyValuePair<KEY, VALUE> &target, KeyValuePair<KEY, VALUE> &source )
{
	target.moveFrom( source );
}

/// writes the given KeyValuePair to a given text stream
template <typename KEY, typename VALUE> 
std::ostream &operator << (std::ostream &out, const KeyValuePair<KEY, VALUE> &value )
{
	return out << '(' << value.getKey() << ", " << value.getValue() << ')';
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
