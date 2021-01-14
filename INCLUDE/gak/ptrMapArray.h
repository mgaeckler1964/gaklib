/*
		Project:		GAKLIB
		Module:			ptrMapArray.h
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

#ifndef PTR_MAP_ARRAY_H
#define PTR_MAP_ARRAY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/arrayBase.h>

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

/*
	a pointer map is an array which allows a mapping from an index type
	to an element the array stores pointers to the element only

	elements must have member functions
	getID returning the index type
	and setID expecting the index type

	the index types must have an operator ==
*/

template <typename OBJ, typename ALLOCATOR=Allocator<OBJ*> >
class PtrMapArray : private ArrayBase<OBJ *, ALLOCATOR>
{
	public:
	typedef typename OBJ::key_type											key_type;
	typedef OBJ																*mapped_type;
	typedef typename ArrayBase<OBJ *, ALLOCATOR>::value_type				value_type;

	typedef typename ArrayBase<OBJ *, ALLOCATOR>::iterator					iterator;
	typedef typename ArrayBase<OBJ *, ALLOCATOR>::const_iterator			const_iterator;
	typedef typename ArrayBase<OBJ *, ALLOCATOR>::reverse_iterator			reverse_iterator;
	typedef typename ArrayBase<OBJ *, ALLOCATOR>::const_reverse_iterator	const_reverse_iterator;


	private:
	// do nothing, just forbid
	PtrMapArray( const PtrMapArray<OBJ,ALLOCATOR> &source );
	PtrMapArray<OBJ, ALLOCATOR> &operator = ( const PtrMapArray<OBJ, ALLOCATOR> &source );

	protected:
	OBJ *getElementByID( const key_type &id ) const;

	public:
	PtrMapArray() : ArrayBase<OBJ*, ALLOCATOR>() {}
	OBJ *operator [] ( const key_type &id ) const
	{
		return getElementByID( id );
	}
	size_t getElementIndex( const key_type &id );
	size_t size( void ) const
	{
		return ArrayBase<OBJ*, ALLOCATOR>::size();
	}
	void addElement( OBJ *ptr )
	{
		ArrayBase<OBJ*, ALLOCATOR>::addElement( ptr );
	}
	void clear( void )
	{
		ArrayBase<OBJ*, ALLOCATOR>::clear();
	}
};

template <typename OBJ, typename ALLOCATOR >
size_t PtrMapArray<OBJ,ALLOCATOR>::getElementIndex( const key_type &id )
{
	size_t	i=size()-1;
	for( const_reverse_iterator it = this->crbegin(), endIT = this->crend(); it != endIT; ++it, --i )
	{
		OBJ *elem = *it;
		if( id == elem->getKey() )
		{
			break;
		}
	}

	return i;
}

template <typename OBJ, typename ALLOCATOR >
OBJ *PtrMapArray<OBJ,ALLOCATOR>::getElementByID( const key_type &id ) const
{
	for( const_iterator it = this->cbegin(), endIT = this->cend(); it != endIT; ++it )
	{
		OBJ *elem = *it;
		if( id == elem->getKey() )
		{
			return elem;
		}
	}

	return NULL;
}

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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// PTR_MAP_ARRAY_H
