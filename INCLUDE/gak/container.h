/*
		Project:		GAKLIB
		Module:			container.h
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

#ifndef GAKLIB_CONTAINER_H
#define GAKLIB_CONTAINER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#define GET_NUM_ELEMENTS 0

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstddef>
#include <limits>

#include <gak/logfile.h>

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

/**
	this type is used to identify a container element
	it must be long enough to hold a pointer or a size_t
*/
typedef unsigned long ELEMENT_ID;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //


/// @brief the base for some containers
class Container
{
	public:
	/// the arithmetic type to store a size
	typedef size_t			size_type;
	/// the arithmetic type to store pointer differences
	typedef ptrdiff_t		difference_type;

	/// conhst value used to specify "no item"
	static const size_t no_index = size_t(-1);

	private:
	size_type	m_numElems;

	protected:
	/// create a new container
	Container()
	{
		m_numElems = 0;
	}
	void moveFrom( Container &source )
	{
		this->m_numElems = source.m_numElems;
	}

	/// decrements and returns the number of elements in this buffer
	size_type decNumElements( void )
	{
		return --m_numElems;
	}
	/// increments and returns the number of elements in this buffer
	size_type incNumElements( void )
	{
		return ++m_numElems;
	}
	/**
		@brief change the number of elements
		@param [in] numElements the new number of elements
	*/
	void setNumElements( size_type numElements )
	{
		m_numElems = numElements;
	}
	/// sets the number of elements to 0
	void clear( void )
	{
		m_numElems = 0;
	}

	public:
#if GET_NUM_ELEMENTS
	/// returns the number of items currently stored in this buffer
	/// @todo remove
	size_type	getNumElements( void ) const
	{
		return m_numElems;
	}
#endif
	/// returns the number of items currently stored in this buffer
	size_type size( void ) const
	{
		return m_numElems;
	}
};

template <typename IteratorT>
class Iterable
{
	public:
	typedef IteratorT	iterator;

	private:
	iterator	m_it, m_endIT;

	public:
	Iterable( const iterator &it, const iterator &endIT )
	: m_it(it), m_endIT(endIT)
	{
	}
	iterator begin() const
	{
		return m_it;
	}
	iterator end() const
	{
		return m_endIT;
	}
};

template <typename ConstIteratorT>
class ConstIterable
{
	public:
	typedef ConstIteratorT	const_iterator;

	private:
	const_iterator	m_it, m_endIT;

	public:
	ConstIterable( const const_iterator &it, const const_iterator &endIT )
	: m_it(it), m_endIT(endIT)
	{
	}
	const_iterator cbegin() const
	{
		return m_it;
	}
	const_iterator cend() const
	{
		return m_endIT;
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

template <typename ContainerT>
ContainerT unionContainer( const ContainerT &first, const ContainerT &second )
{
	ContainerT	result;

	result.addElements( first );
	result.addElements( second );

	return result;
}

template <typename ContainerT>
void mergeSet( ContainerT *first, const ContainerT &second )
{
	for( 
		typename ContainerT::const_iterator it = second.cbegin(), endIT = second.cend();
		it != endIT;
		++it
	)
	{
		if( !first->hasElement( *it ) )
		{
			first->addElement( *it );
		}
	}
}


template <typename ContainerT>
ContainerT unionSet( const ContainerT &first, const ContainerT &second )
{
	ContainerT	result;

	mergeSet( &result, first );
	mergeSet( &result, second );

	return result;
}

template <typename ContainerT>
ContainerT intersect( const ContainerT &first, const ContainerT &second )
{
	ContainerT	result;

	for( 
		typename ContainerT::const_iterator it = first.cbegin(), endIT = first.cend();
		it != endIT;
		++it
	)
	{
		if( !result.hasElement( *it ) && second.hasElement( *it ) )
		{
			result.addElement( *it );
		}
	}

	return result;
}

template <class IterableT, class InserterT> 
void intersectSorted( const IterableT &first, const IterableT &second, InserterT inserter )
{
	doEnterFunction("intersectSorted");
	for(
		typename IterableT::const_iterator 
			fIT = first.cbegin(), fEndIT = first.cend(),
			sIT = second.cbegin(), sEndIT = second.cend();
		fIT != fEndIT && sIT != sEndIT;
	)
	{
		int compareResult = compare( *fIT, *sIT );
		if( compareResult < 0 )
		{
			++fIT;
		}
		else if( compareResult > 0 )
		{
			++sIT;
		}
		else
		{
			*inserter = *fIT;
			++inserter;
			++fIT;
			++sIT;
		}
	}
}

template <typename ContainerT>
ContainerT substract( const ContainerT &first, const ContainerT &second )
{
	ContainerT	result;

	for( 
		typename ContainerT::const_iterator it = first.cbegin(), endIT = first.cend();
		it != endIT;
		++it
	)
	{
		if( !result.hasElement( *it ) && !second.hasElement( *it ) )
		{
			result.addElement( *it );
		}
	}

	return result;
}

template <typename ContainerT>
ContainerT substractSorted( const ContainerT &first, const ContainerT &second )
{
	ContainerT	result;

	for( 
		typename ContainerT::const_iterator 
			fIT = first.cbegin(), fEndIT = first.cend(),
			sIT = second.cbegin(), sEndIT = second.cend();
		fIT != fEndIT;
	)
	{
		if( sIT == sEndIT )
		{
			result.addElement( *fIT );
			++fIT;
		}
		else
		{
			int compareResult = compare( *fIT, *sIT );
			if( compareResult < 0 )
			{
				result.addElement( *fIT );
				++fIT;
			}
			else if( compareResult > 0 )
			{
				++sIT;
			}
			else
			{
				++fIT;
				++sIT;
			}
		}
	}

	return result;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAKLIB_CONTAINER_H
