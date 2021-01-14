/*
		Project:		GAKLIB
		Module:			fixedArray.h
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

#ifndef GAK_FIXED_ARRAY_H
#define GAK_FIXED_ARRAY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/exception.h>
#include <gak/type_traits.h>

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

#	pragma warn -ccc		// condition is allways true/false
#	pragma warn -rch		// code never reached
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
	@brief a buffer with a fixed number of elements

	unlike FixedHeapArray the buffer is part of the object itself

	@tparam OBJ the type of items to store in this container
	@tparam SIZE the number of items to store in this container
*/
template <typename OBJ, size_t SIZE>
class FixedArray
{
	OBJ	m_data[SIZE];

	public:
	/// an iterator
	typedef	OBJ			*iterator;
	/// a const iterator
	typedef	const OBJ	*const_iterator;

	private:
	void copy( const FixedArray<OBJ, SIZE> &source );

	public:
	/// creates an array
	FixedArray()
	{
	}
	/// copies an array
	FixedArray( const FixedArray<OBJ, SIZE> &source )
	{
		copy( source );
	}
	/// copies an array
	const FixedArray<OBJ, SIZE> &operator = ( const FixedArray<OBJ, SIZE> &source )
	{
		copy( source );
		return *this;
	}

	/**
		@brief retrieves an item in this buffer

		@param [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal SIZE
	*/
	OBJ &operator [] ( size_t i )
	{
		if( i>=SIZE )
		{
			throw IndexError();
		}
		return m_data[i];
	}
	/**
		@brief retrieves an item in this buffer

		@param [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal SIZE
	*/
	const OBJ &operator [] ( size_t i ) const
	{
		if( i>=SIZE )
		{
			throw IndexError();
		}
		return m_data[i];
	}

	/**
		@brief retrieves an item in this buffer

		@tparam [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal SIZE
	*/
	template <size_t i>
	OBJ &get()
	{
		if( i>=SIZE )
		{
			throw IndexError();
		}
		return m_data[i];
	}

	/**
		@brief retrieves an item in this buffer

		@tparam [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal SIZE
	*/
	template <size_t i>
	const OBJ &get() const
	{
		if( i>=SIZE )
		{
			throw IndexError();
		}
		return m_data[i];
	}
	/// returns an iterator to the first element of the buffer
	iterator begin()
	{
		return m_data;
	}
	/// returns to an end iterator of the buffer
	iterator end()
	{
		return m_data+SIZE;
	}
	/// returns a const iterator to the first element of the buffer
	const_iterator cbegin() const
	{
		return m_data;
	}
	/// returns a const end iterator of the buffer
	const_iterator cend() const
	{
		return m_data+SIZE;
	}
	/// returns the number of elements of this buffer
	size_t size() const
	{
		return SIZE;
	}

	/**
		@brief compares this buffer with another buffer
		@return
			-1 if this buffer is prior the other buffer\n
			0 if this buffer is equal to the other buffer\n
			+1^if this buffer is past the other buffer
	*/
	int compare( const FixedArray<OBJ, SIZE> &other ) const
	{
		if( is_binary<OBJ>::value )
		{
			return memcmp( m_data, other.m_data, sizeof(OBJ)*SIZE );
		}
		else
		{
			return compareContainer( *this, other );
		}
	}
	/**
		@brief saves this buffer to a stream
		@param [in] stream the target stream
		@see fromBinaryStream
	*/
	void toBinaryStream( std::ostream &stream ) const
	{
		if( is_binary<OBJ>::value )
		{
			fixedArrayToBinaryStream( stream, m_data, SIZE );
		}
		else
		{
			containerDataToBinaryStream( stream, *this );
		}
	}
	/**
		@brief loads this buffer from a stream
		@param [in] stream the source stream
		@see toBinaryStream
	*/
	void fromBinaryStream( std::istream &stream )
	{
		if( is_binary<OBJ>::value )
		{
			fixedArrayFromBinaryStream( stream, m_data, SIZE );
		}
		else
		{
			containerDataFromBinaryStream( stream, *this );
		}
	}
	/// returns the address of the first item
	OBJ *getDataBuffer()
	{
		return m_data;
	}
	/// returns the address of the first item
	const OBJ *getDataBuffer() const
	{
		return m_data;
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

template <typename OBJ, size_t SIZE>
void FixedArray<OBJ, SIZE>::copy( const FixedArray<OBJ, SIZE> &source )
{
	if( is_binary<OBJ>::value )
	{
		memcpy( m_data, source.m_data, sizeof( OBJ ) * SIZE );
	}
	else
	{
		for( size_t i=0; i<SIZE; ++i )
		{
			m_data[i] = source.m_data[i];
		}
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

template <typename OBJ, size_t SIZE>
inline 
std::ostream &operator << ( std::ostream &stream, const FixedArray<OBJ, SIZE> &array )
{
	printContainer( stream, array );

	return stream;
}

} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.

#   pragma warn +ccc
#	pragma warn +rch
#endif

#endif
