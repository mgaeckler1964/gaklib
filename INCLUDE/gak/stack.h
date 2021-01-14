/*
		Project:		GAKLIB
		Module:			stack.h
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

/**
	@brief implements a LIFO buffer
	@tparam OBJ the type to store in the buffer
*/
template <class OBJ>
class Stack : private Array<OBJ>
{
	typedef Array<OBJ>	Super;

	public:
	/// @copydoc ArrayBase::addElement
	OBJ &push( const OBJ &item )
	{
		return Super::addElement( item );
	}
	/// @copydoc ArrayBase::createElement
	OBJ &createElement( void )
	{
		return Super::createElement();
	}
	/// removes the latest element from the buffer
	void removeTop( void )
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
	OBJ &top( void )
	{
		size_t	numItems = size();
		if( !numItems )
		{
			throw StackEmptyError();
		}
		return this->getMutableElementAt( --numItems );
	}
	/// returns the latest element in the buffer
	const OBJ &top( void ) const
	{
		size_t	numItems = size();
		if( !numItems )
		{
			throw StackEmptyError();
		}
		return this->getConstElementAt( --numItems );
	}
	/// returns and removes the latest element in the buffer
	OBJ pop( void )
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
	void clear( void )
	{
		Super::clear();
	}

	/// @copydoc Container::size
	size_t	size( void ) const
	{
		return Super::size();
	}
	/// @copydoc Container::size
	/// @todo remove
#if GET_NUM_ELEMENTS
	size_t	getNumElements( void ) const
	{
		return size();
	}
#endif
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
