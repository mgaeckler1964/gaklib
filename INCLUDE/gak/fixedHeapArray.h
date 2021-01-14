/*
		Project:		GAKLIB
		Module:			fixedHeapArray.h
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

#ifndef GAK_FIXED_HEAP_ARRAY_H
#define GAK_FIXED_HEAP_ARRAY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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
	@brief a buffer with a fixed number of elements

	unlike FixedArray the buffer is allocated from the heap

	@tparam OBJ the type of items to store in this container
*/
template <typename OBJ>
class FixedHeapArray
{
	OBJ		*m_data;
	size_t	m_size;

	private:
	// do not copy
	FixedHeapArray( const FixedHeapArray<OBJ> &source );
	FixedHeapArray & operator = ( const FixedHeapArray<OBJ> &source );

	public:
	/// an iterator
	typedef	OBJ			*iterator;
	/// a const iterator
	typedef	const OBJ	*const_iterator;

	/**
		@brief creates the buffer
		@param [in] size the size of the allocated buffer
	*/
	FixedHeapArray( size_t size )
	{
		m_data = new OBJ [size];
		m_size = size;
	}

	~FixedHeapArray()
	{
		delete [] m_data;
	}
	/**
		@brief retrieves an item in this buffer

		@param [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal the size
	*/
	OBJ &operator [] ( size_t i )
	{
		if( i>=m_size )
		{
			throw IndexError();
		}
		return m_data[i];
	}

	/**
		@brief retrieves an item in this buffer

		@param [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal the size
	*/
	const OBJ &operator [] ( size_t i ) const
	{
		if( i>=m_size )
		{
			throw IndexError();
		}
		return m_data[i];
	}

	/**
		@brief retrieves an item in this buffer

		@tparam [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal the size
	*/
	template <size_t i>
	OBJ &get()
	{
		if( i>=m_size )
		{
			throw IndexError();
		}
		return m_data[i];
	}

	/**
		@brief retrieves an item in this buffer

		@tparam [in] i the index of the item
		@return the item
		@exception IndexError if i is greater equal the size
	*/
	template <size_t i>
	const OBJ &get() const
	{
		if( i>=m_size )
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
		return m_data + m_size;
	}
	/// returns a const iterator to the first element of the buffer
	const_iterator cbegin() const
	{
		return m_data;
	}
	/// returns a const end iterator of the buffer
	const_iterator cend() const
	{
		return m_data+m_size;
	}

	/// returns the number of elements of this buffer
	size_t size() const
	{
		return m_size;
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

} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_FIXED_HEAP_ARRAY_H