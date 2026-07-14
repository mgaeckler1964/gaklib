/*
		Project:		GAKLIB
		Module:			stack.h
		Description:	A Stack (LIFO, last in/first out)
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

#ifndef STACK_ARRAY_H
#define STACK_ARRAY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/array.h>

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

template <typename OBJ, size_t SIZE>
class Fixed4Stack : public FixedArray<OBJ, SIZE>
{
	size_t	m_pos;
	public:
	Fixed4Stack() : m_pos(0) {}
	OBJ &addElement( const OBJ &src )
	{
		OBJ	*result = this->begin();
		(*this)[m_pos++] = src;
		return *result;
	}
	void removeElementAt( size_t pos )
	{
		assert( pos == m_pos-1 );
		m_pos = pos;
	}
	OBJ &getMutableElementAt( size_t pos )
	{
		assert( pos == m_pos-1 );
		return (*this)[pos];
	}
	const OBJ &getConstElementAt( size_t pos ) const
	{
		assert( pos == m_pos-1 );
		return (*this)[pos];
	}
	void clear()
	{
		m_pos = 0;
	}
	size_t size() const
	{
		return m_pos;
	}
};

/**
	@brief implements a LIFO buffer
	@tparam OBJ the type to store in the buffer
*/
template <class OBJ, class SuperT=Array<OBJ> >
class Stack : private SuperT
{
	typedef SuperT Super;
	public:
	/// @copydoc ArrayBase::addElement
	OBJ &push( const OBJ &item )
	{
		return Super::addElement( item );
	}
	/// @copydoc ArrayBase::createElement
	OBJ &createElement()
	{
		return Super::createElement();
	}
	/// removes the latest element from the buffer
	void removeTop()
	{
		size_t	numItems = size();
		if( numItems )
		{
			this->removeElementAt( --numItems );
			if( !numItems )
			{
				clear();
			}
		}
	}

	/// returns the latest element in the buffer
	OBJ &top()
	{
		size_t	numItems = size();
		if( !numItems )
		{
			throw StackEmptyError();
		}
		return this->getMutableElementAt( --numItems );
	}
	/// returns the latest element in the buffer
	const OBJ &top() const
	{
		size_t	numItems = size();
		if( !numItems )
		{
			throw StackEmptyError();
		}
		return this->getConstElementAt( --numItems );
	}
	/// returns and removes the latest element in the buffer
	OBJ pop()
	{
		OBJ	obj = top();
		removeTop();

#ifdef __BORLANDC__
	#pragma warn -inl
#endif
		return obj;
#ifdef __BORLANDC__
#	pragma warn +inl
#endif
	}

	/// @copydoc ArrayBase::clear
	void clear()
	{
		Super::clear();
	}

	/// @copydoc Container::size
	size_t	size() const
	{
		return Super::size();
	}
	/**
		@copydoc ArrayBase::moveFrom
	*/
	void moveFrom( Stack<OBJ> &source )
	{
		Super::moveFrom( source );
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

template <class OBJ>
inline void moveAssign( Stack<OBJ> &target, Stack<OBJ> &source )
{
	target.moveFrom( source );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// STACK_ARRAY_H
