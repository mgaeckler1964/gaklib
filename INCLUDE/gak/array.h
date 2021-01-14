/*
		Project:		GAKLIB
		Module:			array.h
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

#ifndef GAK_ARRAY_H
#define GAK_ARRAY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstring>

#include <gak/arrayBase.h>

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
	@brief array type container

	an array is a container that stores all items in one large memory block

	@tparam OBJ the type of the items to store
	@tparam ALLOCATOR the class that allocates the memory
	@see Allocator, PODallocator
*/
template <class OBJ, class ALLOCATOR=Allocator<OBJ> >
class Array : public ArrayBase<OBJ, ALLOCATOR>
{
	public:
	/// creates an empty array
	Array() : ArrayBase<OBJ, ALLOCATOR>()
	{
	}
	/**
		@brief creates a new array
		@param [in] initialSize the number of items to create immediately
	*/
	Array( size_t initialSize ) : ArrayBase<OBJ, ALLOCATOR>( initialSize )
	{
	}
	/**
		@brief copy contructor
		@param [in] source the source to copy from
	*/
	Array( const Array<OBJ, ALLOCATOR> &source ) : ArrayBase<OBJ, ALLOCATOR>( source )
	{
	}

	/*
	-------------------------------------------------------------------------
		Creating new data
	-------------------------------------------------------------------------
	*/
	/// @name Creating new data
	///@{
	public:
	/// @copydoc ArrayBase::insertElement(size_t)
	OBJ &insertElement( size_t pos )
	{
		return ArrayBase<OBJ, ALLOCATOR>::insertElement( pos );
	}
	/// @copydoc ArrayBase::createElement
	OBJ &createElement( void )
	{
		return ArrayBase<OBJ, ALLOCATOR>::createElement();
	}
	/// @copydoc ArrayBase::createElements
	OBJ *createElements( size_t numElements )
	{
		return ArrayBase<OBJ, ALLOCATOR>::createElements( numElements );
	}
	/// @copydoc ArrayBase::getOrCreateElementAt
	OBJ& getOrCreateElementAt( size_t pos )
	{
		return ArrayBase<OBJ, ALLOCATOR>::getOrCreateElementAt( pos );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Adding existing data
	-------------------------------------------------------------------------
	*/
	/// @name Adding existing data
	///@{
	public:
	/// @copydoc ArrayBase::insertElement( const OBJ &elem, size_t pos )
	OBJ &insertElement( const OBJ &elem, size_t pos )
	{
		return ArrayBase<OBJ, ALLOCATOR>::insertElement( elem, pos );
	}
	///@}

	/*
	-------------------------------------------------------------------------
		Modify Data
	-------------------------------------------------------------------------
	*/
	/// @name Modify Data
	///@{
	public:
	/// @copydoc ArrayBase::moveElement( size_t from, size_t to )
	void moveElement( size_t from, size_t to )
	{
		ArrayBase<OBJ, ALLOCATOR>::moveElement( from, to );
	}
	/// @copydoc ArrayBase::moveElement( const OBJ &searchFor, size_t newPosition )
	void moveElement( const OBJ &searchFor, size_t newPosition )
	{
		ArrayBase<OBJ, ALLOCATOR>::moveElement( searchFor, newPosition );
	}
	///@}
};

/*
	some useful primitive arrays
*/
/**
	@brief an Array that uses PODallocator
	@tparam OBJ the type of the items to store (must not rely on any constructors)
*/
template <class OBJ>
class PODarray : public Array< OBJ, PODallocator<OBJ> >
{
	public:
	/**
		@brief creates a new array
		@param [in] initialSize the number of items to create immediately
	*/
	PODarray( size_t initialSize=0 ) : Array<OBJ, PODallocator<OBJ> >( initialSize )
	{
	}
};

/// shortcut for a PODarray storing items of type int
typedef PODarray<int>	ArrayOfInts;
/// shortcut for a PODarray storing items of type long
typedef PODarray<long>	ArrayOfLongs;

/**
	@brief a PODarray storing pointer objects

	when clearing the data, all objects will be deleted

	@tparam OBJ the item type that can be handled by this container
*/
template <class OBJ>
class ArrayOfPointer : public PODarray<OBJ*>
{
	private:
	// do nothing, just forbid
	ArrayOfPointer( const ArrayOfPointer &source );
	ArrayOfPointer &operator = ( const ArrayOfPointer &source );

	public:

	typedef typename Array< OBJ*, PODallocator<OBJ*> >::iterator			
		iterator;

	/// creates an empty array
	ArrayOfPointer() : PODarray<OBJ*>() {}

	/// destroy the array and all objects
	~ArrayOfPointer();
};

/// an aray that is used to store binary data
class ArrayOfData : public PODarray<char>
{
	public:
	/// creates anh empty array
	ArrayOfData()
	{
	}
	/// copy constructor
	ArrayOfData( const STRING &source )
	{
		addElements( static_cast<const char *>(source), source.strlen() );
	}
	/// assignment operator
	const ArrayOfData &operator = ( const STRING &source )
	{
		clear();
		addElements( static_cast<const char *>(source), source.strlen() );

		return *this;
	}
	/// assignment operator with a C-style string
	const ArrayOfData &operator = ( const char *source )
	{
		clear();
		addElements( source, std::strlen(source) );

		return *this;
	}
	/**
		@brief adds all characters of a STRING to this array
		@param [in] source the source string
		@return the array itself
	*/
	const ArrayOfData &operator += ( const STRING &source )
	{
		addElements( static_cast<const char *>(source), source.strlen() );

		return *this;
	}
	/**
		@brief adds all characters of a C-style string to this array
		@param [in] source the source string
		@return the array itself
	*/
	const ArrayOfData &operator += ( const char *source )
	{
		addElements( source, std::strlen(source) );

		return *this;
	}
	/**
		@brief adds one characters to this array
		@param [in] c the source character
		@return the array itself
	*/
	const ArrayOfData &operator += ( char c )
	{
		addElement( c );

		return *this;
	}
};

///	an array that stores doubles
class ArrayOfFloats : public PODarray<double>
{
	public:
	/// returns the sum of all elements
	double sum( void ) const;
	/// returns the average of all elements
	double average( void ) const
	{
		return sum() / double(size());
	}
};

///	an array that store STRING
class ArrayOfStrings : public Array<STRING>
{
	public:

	/// enable string delimiter "
	static const int CHECK_STRINGS		= 0x01;		// ""
	/// enable character delimiter '
	static const int CHECK_CHARS		= 0x02;		// ''
	/// enable brace delimiter { }
	static const int CHECK_BRACES		= 0x04;		// {}
	/// enable bracket delimiter [  ]
	static const int CHECK_BRACKETS		= 0x08;		// []
	/// enable parentesis delimiter ( )
	static const int CHECK_PARENTESIS	= 0x10;		// ()

	/**
		@brief parses a csv like string and creates for each field one element
		@param [in] source the STRING to parse
		@param [in] delimiters a C-style string containing all used field delimiters
		@param [in] noEmpties when true, no empty values will be added to the array.
	*/
	void createElements(
		const STRING &source,
		const char *delimiters,
		bool noEmpties = false
	);
	/**
		@brief parses a commandline and creates for each argument one element. 
		
		Respects arguments enclosed in "

		@param [in] source a C-style string containing the commandline arguments
	*/
	void createElements( const char *source );

	/**
		@brief more detailed parsing

		splits a string by searching for spaces. if the flags parameter has 
			\li CHECK_STRINGS set all characters between two " form one element
			\li CHECK_CHARS set all characters between two ' form one element
			\li CHECK_BRACES set all characters between { and } form one element
			\li CHECK_BRACKETS set all characters between [ and ] form one element
			\li CHECK_PARENTESIS set all characters between ( and ) form one element

		@param [in] source a C-style string to parse
		@param [in] flags some flags that control the parser
	*/
	void createElements( const char *source, int flags );

	/**
		@brief writes all strings to a file one line per STRING
		@param [in] fileName the name of the file (will be overwritten)
	*/
	void writeToFile( const STRING &fileName ) const;
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

template <class OBJ>
ArrayOfPointer<OBJ>::~ArrayOfPointer()
{
	for( iterator it = this->begin(), endIT = this->end(); it != endIT; ++it )
	{
		delete *it;
	}
}

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

template<typename OBJ, typename ALLOCATOR>
inline void moveAssign( Array<OBJ, ALLOCATOR> &target, Array<OBJ, ALLOCATOR> &source )
{
	target.moveFrom( source );
}

template<typename OBJ>
inline void moveAssign( PODarray<OBJ> &target, PODarray<OBJ> &source )
{
	target.moveFrom( source );
}

template<typename OBJ>
inline void moveAssign( ArrayOfPointer<OBJ> &target, ArrayOfPointer<OBJ> &source )
{
	target.moveFrom( source );
}

inline void moveAssign( ArrayOfData &target, ArrayOfData &source )
{
	target.moveFrom( source );
}

inline void moveAssign( ArrayOfFloats &target, ArrayOfFloats &source )
{
	target.moveFrom( source );
}

inline void moveAssign( ArrayOfStrings &target, ArrayOfStrings &source )
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

#endif	// GAK_ARRAY_BASE_H
