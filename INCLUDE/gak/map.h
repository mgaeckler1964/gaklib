/*
		Project:		GAKLIB
		Module:			map.h
		Description:	Some maps
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

#ifndef GAK_MAP_ARRAY_H
#define GAK_MAP_ARRAY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/sortedArray.h>
#include <gak/array.h>
#include <gak/btree.h>
#include <gak/keyValuePair.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
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
	@brief Returns an Container with all keys of a map
	@param [in] map The map to scan
	@tparam ResultT The type of the result container
	@tparam MapT The type of the map
	@return The result Container filled
*/
template <typename ResultT, typename MapT> 
ResultT getKeys( const MapT &map );

/**
	@brief checks the existence of a key value in a map
	@param [in] map the map to scan
	@param [in] key the key value to search
	@return true if the key value was found
	@todo check we need this function
*/
template <class MAP>
bool hasElement( const MAP &map, const typename MAP::key_type &key );

/**
	@brief return the key value of a user data
	@param [in] map the map to scan
	@param [in] value the user data to search
	@return the key value if found or a default value if user data was not found
*/
template <class MAP>
typename MAP::key_type findValue( const MAP &map, const typename MAP::mapped_type &value );

/**
	@brief a map that is a Container that used to access data by a key value

	An unorderd map is an array which allows a mapping from an index type
	to an element. The map is not sorted. Adding is quite fast, but searching for
	elements may be slow.

	elements must have typedef key_type to define the type of the key alues

	elements must have member functions
	\li getKey returning the index type
	\li setKey expecting the index type

	the index type must have an operator ==

	@tparam OBJ the item type must define type key_type
	@tparam ALLOCATOR the allocator type see ArrayBase
*/
template < typename OBJ, typename ALLOCATOR=Allocator<OBJ> > 
class UnorderedMap : public Array<OBJ, ALLOCATOR>
{
	typedef Array<OBJ, ALLOCATOR>	Super;
	public:
	/// The type if the data stored in this Container
	typedef typename Super::value_type		value_type;
	/// the type of the key values
	typedef typename OBJ::key_type	key_type;
	/// the type of user data
	typedef OBJ						mapped_type;

	protected:
	/**
		@brief returns the first element with a given key

		creates a new one if it does not exist

		@param [in] key the key value to search for
		@return the (new) element found
	*/
	mapped_type &getElementByKey( const key_type &key );
	/**
		@brief returns the first element with a given key

		@param [in] key the key value to search for
		@return the element found
		@exception IndexError if key was not found

	*/
	const mapped_type &getElementByKey( const key_type &key ) const;

	public:
	/// @copydoc ArrayBase::getConstElementAt
	const mapped_type &getElementAt( size_t pos ) const
	{
		return this->getConstElementAt( pos );
	}
	/// @copydoc ArrayBase::getMutableElementAt
	mapped_type &getElementAt( size_t pos )
	{
		return this->getMutableElementAt( pos );
	}

	/**
		@brief returns the first element with a given key

		creates a new one if it does not exist

		@param [in] key the key value to search for
		@return the (new) element found
	*/
	mapped_type &operator [] ( const key_type &key )
	{
		return getElementByKey( key );
	}
	/**
		@brief returns the first element with a given key

		@param [in] key the key value to search for
		@return the element found
		@exception IndexError if key was not found

	*/
	const mapped_type &operator [] ( const key_type &key ) const
	{
		return getElementByKey( key );
	}
	/// returns the index of the first item with a given key. no_index if it was not found
	size_t getElementIndex( const key_type &key ) const;
	/// returns true if an element with a given key exists
	bool hasElement( const key_type &key ) const
	{
		return getElementIndex( key ) != this->no_index;
	}
	/// returns true if an element with a given key from a value exists
	bool hasElement( const value_type &value ) const
	{
		return hasElement( value.getKey() );
	}
	/// removes the first element with a given key
	void removeElementByKey( const key_type &key )
	{
		size_t	index = getElementIndex( key );
		if( index != this->no_index )
		{
			this->removeElementAt( index );
		}
	}
	/// Returns an array with all keys available in this map
	Array<key_type> getKeys() const
	{
		return gak::getKeys< Array<key_type> >( *this );
	}
	/**
		@brief search a value
		@param [in] value the value to search for
		@return the key of the value if present\n
			the default value of the key type if the searched value was not found
	*/
	key_type findElement( const mapped_type &value ) const
	{
		size_t idx = getElementIndex( value.getKey() );
		if( idx != this->no_index )
		{
			return (this->getDataBuffer() + idx)->getKey();
		}
		else
		{
			return key_type();
		}
	}
};

template <typename OBJ, typename ALLOCATOR> 
size_t UnorderedMap<OBJ, ALLOCATOR>::getElementIndex( const key_type &key ) const
{
	size_t	i=0, numElements = this->size();
	const OBJ *data = this->getDataBuffer();
	for( ; i<numElements; ++i, ++data )
	{
		if( key == data->getKey() )
			return i;
	}

	return this->no_index;
}

template <typename OBJ, typename ALLOCATOR> 
OBJ &UnorderedMap<OBJ, ALLOCATOR>::getElementByKey( const key_type &key )
{
	size_t	index = getElementIndex( key );
	if( index != this->no_index )
	{
		return const_cast<OBJ*>(this->getDataBuffer())[index];
	}
	else
	{
		OBJ	&newData = this->createElement();
		newData.setKey( key );

		return newData;
	}
}

template <typename OBJ, typename ALLOCATOR> 
const OBJ &UnorderedMap<OBJ, ALLOCATOR>::getElementByKey( const key_type &key ) const
{
	size_t	index = getElementIndex( key );
	if( index != -1 )
	{
		return const_cast<OBJ*>(this->getDataBuffer())[index];
	}
	else
	{
		throw IndexError();
	}
}

/**
	@brief a map that is used to acces data by a key value

	a map is like an UnorderedMap, but the array is sorted by key. 
	Thus adding new elements is slower but searching may be faster.
	if you change the key of an object after adding, you must resort the array.

	the key type must be comparable

	@tparam OBJ the type of user data (must define key_type)
	@tparam ALLOCATOR the allocator
	@see UnorderedMap, SortedArray, KeyComparator, Allocator
*/
template <class OBJ, class ALLOCATOR=Allocator<OBJ> >
class Map : public SortedArray<OBJ, KeyComparator<typename OBJ::key_type,OBJ>, ALLOCATOR>
{
	typedef SortedArray<OBJ, KeyComparator<typename OBJ::key_type,OBJ>, ALLOCATOR>	Super;
	public:
	/// The type if the data stored in this Container
	typedef typename Super::value_type		value_type;

	public:
	/// the type of the key values
	typedef typename OBJ::key_type	key_type;
	/// the type of the user data
	typedef OBJ						mapped_type;

	private:
	size_t findInsertPos( const key_type &key ) const;

	protected:
	/**
		@brief returns the first element with a given key

		creates a new one if it does not exist

		@param [in] key the key value to search for
		@return the (new) element found
	*/
	mapped_type &getElementByKey( const key_type &key );
	/**
		@brief returns the first element with a given key

		@param [in] key the key value to search for
		@return the element found
		@exception IndexError if key was not found
	*/
	const mapped_type &getElementByKey( const key_type &key ) const;

	public:
	/// creates an empty map
	Map() : Super()
	{
	}

	/// @copydoc ArrayBase::getConstElementAt
	const mapped_type &getElementAt( size_t pos ) const
	{
		return this->getConstElementAt( pos );
	}
	/// @copydoc ArrayBase::getMutableElementAt
	mapped_type &getElementAt( size_t pos )
	{
		return this->getMutableElementAt( pos );
	}

	/**
		@brief returns the first element with a given key

		creates a new one if it does not exist

		@param [in] key the key value to search for
		@return the (new) element found
	*/
	mapped_type &operator [] ( const key_type &key )
	{
		return getElementByKey( key );
	}
	/**
		@brief returns the first element with a given key

		@param [in] key the key value to search for
		@return the element found
		@exception IndexError if key was not found
	*/
	const mapped_type &operator [] ( const key_type &key ) const
	{
		return getElementByKey( key );
	}
	/// @copydoc UnorderedMap::getElementIndex
	std::size_t getElementIndex( const key_type &key ) const;
	/// @copydoc UnorderedMap::hasElement
	bool hasElement( const key_type &key ) const
	{
		return getElementIndex( key ) != this->no_index;
	}
	bool hasElement( const value_type &value ) const
	{
		return hasElement( value.getKey() );
	}
	/// @copydoc UnorderedMap::removeElementByKey
	void removeElementByKey( const key_type &key )
	{
		size_t	index = getElementIndex( key );
		if( index != this->no_index )
		{
			this->removeElementAt( index );
		}
	}
	/// @copydoc UnorderedMap::getKeys
	Array<key_type> getKeys() const
	{
		return gak::getKeys< Array<key_type> >( *this );
	}

	/// @copydoc UnorderedMap::findElement
	key_type findElement( const mapped_type &value ) const
	{
		size_t idx = getElementIndex( value.getKey() );
		if( idx != this->no_index )
		{
			return (this->getDataBuffer() + idx)->getKey();
		}
		else
		{
			return key_type();
		}
	}
#if 0
	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	public:
	/// @name Some standard typedefs:
	///@{
	typedef typename ArrayBase<OBJ, ALLOCATOR>::iterator			iterator;
	typedef typename ArrayBase<OBJ, ALLOCATOR>::reverse_iterator	reverse_iterator;
	///@}

	/// @name Some standard member functions:
	///@{
	iterator begin()
	{
		return ArrayBase<OBJ, ALLOCATOR>::begin();
	}
	iterator end()
	{
		return ArrayBase<OBJ, ALLOCATOR>::end();
	}
	reverse_iterator rbegin()
	{
		return ArrayBase<OBJ, ALLOCATOR>::rbegin();
	}
	reverse_iterator rend()
	{
		return ArrayBase<OBJ, ALLOCATOR>::rend();
	}
	///@}
#endif
};

template <class OBJ, class ALLOCATOR> 
size_t Map<OBJ,ALLOCATOR>::findInsertPos( const key_type &key ) const
{
	int		compareResult;
	size_t	min, max, pos;

	min = 0;
	max = Container::size();
	pos = (min+max)/2;

	while( min < max )
	{
		compareResult = this->getComparator().compareWithKey( *(this->getDataBuffer()+pos), key );
		if( !compareResult )
/*v*/		break;
		if( compareResult < 0 )
			min = pos+1;
		else
			max = pos;

		pos = (min+max)/2;
	}

	return pos;
}

template <class OBJ, class ALLOCATOR> 
size_t Map<OBJ,ALLOCATOR>::getElementIndex( const key_type &key ) const
{
	size_t	pos = findInsertPos( key );
	if( pos >= this->size() || this->getComparator().compareWithKey( *(this->getDataBuffer()+pos), key ) )
	{
		pos = this->no_index;
	}

	return pos;
}

template <class OBJ, class ALLOCATOR>
OBJ &Map<OBJ,ALLOCATOR>::getElementByKey( const key_type &key )
{
	size_t	pos = findInsertPos( key );
	if( pos >= this->size()
	|| this->getComparator().compareWithKey( *(this->getDataBuffer()+pos), key ) )
	{
		mapped_type	&newData = this->insertElement( pos );
		newData.setKey( key );

		return newData;
	}
	else
	{
		return this->getMutableElementAt( pos );
	}
}

template <class OBJ, class ALLOCATOR>
const OBJ &Map<OBJ,ALLOCATOR>::getElementByKey( const key_type &key ) const
{
	size_t	pos = findInsertPos( key );
	if( pos >= this->size()
	|| this->getComparator().compareWithKey( *(this->getDataBuffer()+pos), key ) )
	{
		throw IndexError();
	}
	else
	{
		const mapped_type &elem = this->getConstElementAt( pos );
		return elem;
	}
}

/**
	@brief a map that is used to acces data by a key value

	this map is like an UnorderedMap but it uses KeyValuePair to define a type
	storing the key and the data itself

	@tparam KEY the type of the key values
	@tparam VALUE the type of the user data
*/
template <class KEY, class VALUE> 
class UnorderedPairMap : public UnorderedMap<KeyValuePair<KEY, VALUE> >
{
	public:
	/// the type of the key values
	typedef KEY		key_type;
	/// the type of the user data
	typedef VALUE	mapped_type;

	/**
		@brief changes/adds the user data for a given key
		@param [in] key the key value
		@param [in] value the user data
	*/
	void setValue( const key_type &key, const mapped_type &value )
	{
		getElementByKey( key ).setValue( value );
	}
	/**
		@brief returns the user data at a given position
		@param [in] pos the position
		@return the user data
		@exception IndexError in case of a range error
	*/
	mapped_type &getValueAt( size_t pos )
	{
		return this->getMutableElementAt( pos ).getValue();
	}
	/**
		@brief returns the user data at a given position
		@param [in] pos the position
		@return the user data
		@exception IndexError in case of a range error
	*/
	const mapped_type &getValueAt( size_t pos ) const
	{
		return this->getElementAt( pos ).getValue();
	}
	/**
		@brief returns the key value at a given position
		@param [in] pos the position
		@return the key value
		@exception IndexError in case of a range error
	*/
	const key_type &getKeyAt( size_t pos ) const
	{
		return this->getElementAt( pos ).getKey();
	}
	/**
		@brief returns the user data for a given key
		@param [in] key the key value
		@return the user data
	*/
	mapped_type &operator [] ( const key_type &key )
	{
		return getElementByKey( key ).getValue();
	}
	/**
		@brief returns the user data for a given key
		@param [in] key the key value
		@return the user data
		@exception IndexError in case of a range error
	*/
	const mapped_type &operator [] ( const key_type &key ) const
	{
		return getElementByKey( key ).getValue();
	}
	/**
		@brief return the key value of a user data
		@param [in] value the user data to search
		@return the key value if found or a default value if user data was not found
	*/
	key_type findValue( const mapped_type &value ) const
	{
		return gak::findValue( *this, value );
	}
};

/**
	@brief a map that is used to acces data by a key value

	this map is like an Map but it uses KeyValuePair to define a type
	storing the key and the data itself

	@tparam KEY the type of the key values
	@tparam VALUE the type of the user data
*/
template <class KEY, class VALUE, class ALLOCATOR=Allocator<KeyValuePair<KEY, VALUE> > >
class PairMap : public Map< KeyValuePair<KEY, VALUE>, ALLOCATOR >
{
	typedef Map< KeyValuePair<KEY, VALUE>, ALLOCATOR >		Super;

	public:
	/// the type of the key values
	typedef KEY		key_type;
	/// the type of the user data
	typedef VALUE	mapped_type;

	/// @copydoc UnorderedPairMap::setValue
	void setValue( const key_type &key, const mapped_type &value )
	{
		getElementByKey( key ).setValue( value );
	}

	/// @copydoc UnorderedPairMap::getValueAt
	mapped_type &getValueAt( size_t pos )
	{
		return this->getMutableElementAt( pos ).getValue();
	}
	/// @copydoc UnorderedPairMap::getValueAt
	const mapped_type &getValueAt( size_t pos ) const
	{
		return this->getElementAt( pos ).getValue();
	}
	/// @copydoc UnorderedPairMap::getKeyAt
	const key_type &getKeyAt( size_t pos ) const
	{
		return this->getElementAt( pos ).getKey();
	}
	/**
		@brief returns the first element with a given key

		creates a new one if it does not exist

		@param [in] key the key value to search for
		@return the (new) element found
	*/
	mapped_type &operator [] ( const key_type &key )
	{
		return getElementByKey( key ).getValue();
	}
	/**
		@brief returns the user data for a given key
		@param [in] key the key value
		@return the user data
		@exception IndexError in case of a range error
	*/
	const mapped_type &operator [] ( const key_type &key ) const
	{
		return getElementByKey( key ).getValue();
	}
	/// @copydoc UnorderedPairMap::findValue
	key_type findValue( const mapped_type &value ) const
	{
		return gak::findValue( *this, value );
	}
	/*
	-------------------------------------------------------------------------
		standard support
	-------------------------------------------------------------------------
	*/
	public:
	/// @name Some standard typedefs:
	///@{
	typedef typename Super::const_iterator			iterator;
	typedef typename Super::const_reverse_iterator	reverse_iterator;
	///@}

	/// @name Some standard member functions:
	///@{
	iterator begin()
	{
		return Super::cbegin();
	}
	iterator end()
	{
		return Super::cend();
	}
	reverse_iterator rbegin()
	{
		return Super::crbegin();
	}
	reverse_iterator rend()
	{
		return Super::crend();
	}
	///@}
};

/// A short cut for a PairMap that uses CI_STRING as key type and STRING as value type
typedef PairMap<CI_STRING, STRING>	StrStrMap;


/// A short cut for a PairMap that uses int as key type and STRING as value type
typedef PairMap<int,STRING> IntStrMap;

/**
	@brief a tree map is a Map based on a Btree

	you can change the value, but not the key after adding an item to the tree.

	@tparam KEY the type of the key values
	@tparam VALUE the type of the user data
	@tparam Comparator the type comparing values

	@see KeyComparator, KeyValuePair
*/
template <class KEY, class VALUE, class Comparator = KeyComparator<KEY, KeyValuePair<KEY, VALUE> > >
class TreeMap : public Btree<KeyValuePair<KEY, VALUE>, Comparator>
{
	typedef Btree<KeyValuePair<KEY, VALUE>, Comparator>	Super;

	public:
	/// the type of the key values
	typedef KEY									key_type;
	/// the type of the user data
	typedef VALUE								mapped_type;
	/// the type stored in this Container
	typedef KeyValuePair<key_type, mapped_type>	value_type;

	private:
	typedef typename Btree<KeyValuePair<key_type, mapped_type>, Comparator>::Node	Node;

	mutable key_type m_lastKey;
	mutable Node	*m_lastNode;
	mutable bool	m_cacheValid;

	Node *findNodeByKey( const key_type &key ) const
	{
		Node *result = (m_cacheValid && key == m_lastKey)
			? m_lastNode
			: this->m_root
				? this->m_root->findNode( key, Comparator::compareWithKey )
				: NULL
		;
		m_cacheValid = true;
		m_lastKey = key;
		m_lastNode = result;

		return result;
	}
	const value_type *findElementByKey( const key_type &key ) const
	{
		const Node *node = this->findNodeByKey( key );
		return  node ? &node->data : NULL;
	}

	mapped_type &getElementByKey( const key_type &key )
	{
		mapped_type *val = findValueByKey( key );
		if( val )
		{
			return *val;
		}

		return addElement( value_type( key ) ).getValue();
	}
	const mapped_type &getElementByKey( const key_type &key ) const
	{
		const mapped_type *val = findValueByKey( key );
		if( !val )
		{
			throw IndexError();
		}
		return *val;
	}
	public:
	TreeMap()
	{
		m_cacheValid = false;
	}
	/**
		@brief searches for a user data with a given key

		creates new item if key does not exist

		@param [in] key the key value to search for
		@return the value found/created
	*/
	mapped_type &getOrCreateElement ( const key_type &key )
	{
		return getElementByKey( key );
	}
	/**
		@brief searches for a user data with a given key and updates the data

		creates new item if key does not exist

		@param [in] key the key value to search for
		@param [in] value the new/updated user data
	*/
	void setValue( const key_type &key, const mapped_type &value )
	{
		getElementByKey( key ) = value;
	}
	/// searches for a user data with a given key
	mapped_type &operator [] ( const key_type &key )
	{
		return getElementByKey( key );
	}
	/**
		@brief searches for a user data with a given key
		@param [in] key the key value to search for
		@exception IndexError if key does not exist
	*/
	const mapped_type &operator [] ( const key_type &key ) const
	{
		return getElementByKey( key );
	}
	/**
		@brief Returns an Array with all key values
		@return The result Array with all keys
		@see gak::getKeys
	*/
	Array<key_type> getKeys() const
	{
		return gak::getKeys< Array<key_type> >( *this );
	}
	/**
		@brief returns the key value of a given user data
		@param [in] value the user data to search for
		@return the key value
		@see gak::findValue
	*/
	key_type findValue( const mapped_type &value ) const
	{
		return gak::findValue( *this, value );
	}
	/**
		@brief returns user data by a given key value
		@param [in] key the key value to search for
		@return Address of found value NULL if not found
	*/
	mapped_type *findValueByKey( const key_type &key )
	{
		Node *node = this->findNodeByKey( key );
		return  node ? &node->m_data.getValue() : NULL;
	}
	/**
		@brief returns user data by a given key value
		@param [in] key the key value to search for
		@return Address of found value NULL if not found
	*/
	const mapped_type *findValueByKey( const key_type &key ) const
	{
		Node *node = findNodeByKey( key );
		return  node ? &node->m_data.getValue() : NULL;
	}
	/**
		@brief checks existence of a given key value
		@param [in] key the key value to search for
		@return true if key exists
		@todo fasten implementation ???
	*/
	bool hasElement( const key_type &key ) const
	{
		Node *node = findNodeByKey( key );
		return  node ? true : false;
	}
	/// returns true if an element with a given key from a value exists
	bool hasElement( const value_type &value ) const
	{
		return hasElement( value.getKey() );
	}

	/**
		@brief removes an item by given key
		@param [in] key the key value to search for
	*/
	void removeElementByKey( const key_type &key )
	{
		Node *node = findNodeByKey( key );
		if( node )
		{
			m_cacheValid = false;
			this->removeElement( node );
		}
	}
	value_type &addElement( const value_type &data )
	{
		m_cacheValid = false;
		return Super::addElement( data );
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

template <typename ResultT, typename MAP> 
ResultT getKeys( const MAP &map )
{
	ResultT	result;

	result.setMinSize( map.size() );
	for( 
		typename MAP::const_iterator it = map.cbegin(), endIT = map.cend();
		it != endIT;
		++it
	)
	{
		result.addElement( it->getKey() );
	}

	return result;
}

template <class MAP>
bool hasElement( const MAP &map, const typename MAP::key_type &key )
{
	for(
		typename MAP::const_iterator it = map.cbegin(), endIT = map.cend();
		it != endIT;
		++it 
	)
	{
		if( it->getKey() == key )
		{
			return true;
		}
	}

	return false;
}

template <class MAP>
typename MAP::key_type findValue( const MAP &map, const typename MAP::mapped_type &value )
{
	typedef typename MAP::key_type KeyT;
	for(
		typename MAP::const_iterator it = map.cbegin(), endIT = map.cend();
		it != endIT;
		++it
	)
	{
		if( it->getValue() == value )
		{
			return it->getKey();
		}
	}

	return KeyT();
}

template<typename OBJ, typename ALLOCATOR>
inline void moveAssign( UnorderedMap<OBJ, ALLOCATOR> &target, UnorderedMap<OBJ, ALLOCATOR> &source )
{
	target.moveFrom( source );
}

template <class KEY, class VALUE> 
inline void moveAssign( UnorderedPairMap<KEY, VALUE> &target, UnorderedPairMap<KEY, VALUE> &source )
{
	target.moveFrom( source );
}

template<typename OBJ, typename ALLOCATOR>
inline void moveAssign( Map<OBJ, ALLOCATOR> &target, Map<OBJ, ALLOCATOR> &source )
{
	target.moveFrom( source );
}

template <class KEY, class VALUE, class ALLOCATOR>
inline void moveAssign( PairMap<KEY, VALUE, ALLOCATOR> &target, PairMap<KEY, VALUE, ALLOCATOR> &source )
{
	target.moveFrom( source );
}

template <class KEY, class VALUE, class Comparator>
inline void moveAssign( TreeMap<KEY, VALUE, Comparator> &target, TreeMap<KEY, VALUE, Comparator> &source )
{
	target.moveFrom( source );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_MAP_ARRAY_H
