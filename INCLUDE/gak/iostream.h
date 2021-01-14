/*
		Project:		GAKLIB
		Module:			iostream.h
		Description:	Usefull iostream functions
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

#ifndef GAK_IOSTREAM_H
#define GAK_IOSTREAM_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>

#include <gak/fstream.h>
#include <gak/exception.h>
#include <gak/types.h>

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

/// @cond
namespace internal
{
	const size_t	streamBufferSize = 32 * 1024;
}
/// @endcond

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

#if defined( __BORLANDC__ )
typedef std::ios::seek_dir	seekdir;
typedef int					openmode;
#else
typedef std::ios_base::openmode	openmode;
typedef std::ios_base::seekdir	seekdir;
#endif

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

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

/*
	for simple types
*/
/**
	Writes a scalar type to a binary stream
	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write
	@tparam ScalarT The type of the value to write Warning: DO NOT WRITE ANY POINTERS
	@throws WriteError In case of any I/O error
	@see binaryFromBinaryStream
*/
template <typename ScalarT> 
inline void binaryToBinaryStream( std::ostream &stream, const ScalarT &value )
{
	if( stream )
	{
		stream.write( reinterpret_cast<const char*>(&value), sizeof(ScalarT) );
	}

	if( !stream )
	{
		throw WriteError();
	}
}

/**
	Reads a scalar type from a binary stream
	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The value to read
	@tparam ScalarT The type of the value to read Warning: DO NOT READ ANY POINTERS
	@throws ReadError In case of any I/O error
	@see binaryToBinaryStream
*/
template <typename ScalarT> 
inline void binaryFromBinaryStream( std::istream &stream, ScalarT *value )
{
	if( stream )
	{
		stream.read( reinterpret_cast<char*>(value), sizeof( ScalarT ) );
	}

	if( !stream )
	{
		throw ReadError();
	}
}

/*
	for multiple simple types
*/
/**
	Writes scalar values to a binary stream
	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] values The address of the value array to write
	@param [in] count The number of values to write
	@tparam ScalarT The type of the values to write Warning: DO NOT WRITE ANY POINTERS
	@throws WriteError In case of any I/O error
	@see fixedArrayFromBinaryStream
*/
template <typename ScalarT> 
void fixedArrayToBinaryStream(
	std::ostream &stream, const ScalarT *values, size_t count
)
{
	const char	*buffer = reinterpret_cast<const char *>(values);

	count *= sizeof( ScalarT );

	while( count >= internal::streamBufferSize && stream )
	{
		stream.write( buffer, internal::streamBufferSize );
		buffer += internal::streamBufferSize;
		count -= internal::streamBufferSize;
	}

	if( count && stream )
	{
		stream.write( buffer, count );
	}

	if( !stream )
	{
		throw WriteError();
	}
}

/**
	Reads scalar values from a binary stream
	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] values The address of the value array to read
	@param [in] count The number of values to read
	@tparam ScalarT The type of the values to read Warning: DO NOT READ ANY POINTERS
	@throws ReadError In case of any I/O error
	@see fixedArrayToBinaryStream
*/
template <typename ScalarT> 
void fixedArrayFromBinaryStream(
	std::istream &stream, ScalarT *values, size_t count
)
{
	char	*buffer = static_cast<char *>(static_cast<void *>(values));

	count *= sizeof( ScalarT );

	while( count >= internal::streamBufferSize && stream )
	{
		stream.read( buffer, internal::streamBufferSize );
		buffer += internal::streamBufferSize;
		count -= internal::streamBufferSize;
	}

	if( count && stream )
	{
		stream.read( buffer, count );
	}

	if( !stream )
	{
		throw ReadError();
	}
}

/*
	for classes
*/

/**
	@brief Writes an object to a binary stream

	This function writes an object to a binary stream. The type of the object must
	provide a member void toBinaryStream ( std::ostream &stream ) that writes the data
	to the stream. For primitive types there are specializations that use binaryToBinaryStream
	to write the data to the stream.

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] obj The value to write
	@tparam OBJ The type of the value to write Warning: DO NOT WRITE ANY POINTERS

	@see binaryToBinaryStream, fromBinaryStream
*/
template <typename OBJ>
inline void toBinaryStream ( std::ostream &stream, const OBJ &obj )
{
	obj.toBinaryStream( stream );
}

/**
	@brief Writes a bool value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream( std::ostream &stream, const bool &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a char value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const char &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a signed char value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const signed char &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes an unsigned char value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const unsigned char &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a signed short value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const signed short &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes an unsigned short value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const unsigned short &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a signed int value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const signed int &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes an unsigned int value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const unsigned int &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a signed long value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const signed long &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes an unsigned long value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const unsigned long &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a signed long long value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const int64 &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes an unsigned long long value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const uint64 &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a float value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const float &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a double value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const double &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Writes a long double value to a binary stream

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] value The value to write

	@see binaryToBinaryStream, fromBinaryStream
*/
template <>
inline void toBinaryStream ( std::ostream &stream, const long double &value )
{
	binaryToBinaryStream( stream, value );
}

/**
	@brief Reads an object from a binary stream

	This function reads an object from a binary stream. The type of the object must
	provide a member void fromBinaryStream ( std::istream &stream ) that reads the data
	from the stream. For primitive types there are specializations that use binaryFromBinaryStream
	to read the data from the stream.

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] obj The address of value to read
	@tparam OBJ The type of the value to read Warning: DO NOT READ ANY POINTERS

	@see binaryFromBinaryStream, toBinaryStream
*/
template <typename OBJ>
inline void fromBinaryStream( std::istream &stream, OBJ *obj )
{
	obj->fromBinaryStream( stream );
}

/**
	@brief Reads a bool value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, bool *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a char value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, char *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed char value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, signed char *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads an unsigned char value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, unsigned char *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed short value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, signed short *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed short value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, unsigned short *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed int value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, signed int *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed int value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, unsigned int *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed long value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, signed long *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed long value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, unsigned long *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed long long value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, int64 *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a signed long long value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, uint64 *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a float value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, float *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a double value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, double *value )
{
	binaryFromBinaryStream( stream, value );
}

/**
	@brief Reads a long double value from a binary stream

	@param [in] stream The input stream (must be opened in binary mode)
	@param [out] value The address of value to read

	@see binaryFromBinaryStream, toBinaryStream
*/
template <>
inline void fromBinaryStream( std::istream &stream, long double *value )
{
	binaryFromBinaryStream( stream, value );
}

/*
	for containers
*/
/**
	@brief Writes the content of a container to a binary stream

	This function writes the content of a container to a stream. It does not
	write the number of items of the container. If you need the number of items 
	also written to the stream use containerToBinaryStream.
	The container must provide stanard support for a const iterator

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] container The container to write
	@tparam ContainerT The type of the container write Warning: DO NOT WRITE ANY POINTERS
	@see containerToBinaryStream, toBinaryStream, containerDataFromBinaryStream
*/
template <class ContainerT>
void containerDataToBinaryStream(
	std::ostream &stream, const ContainerT &container
)
{
	for(
		typename ContainerT::const_iterator it = container.cbegin(),
			endIT = container.cend();
		it != endIT;
		++it
	)
	{
		toBinaryStream( stream, *it );
	}
}

/**
	@brief Reads the content of a container from a binary stream

	This function reads the content of a container from a stream. It does not
	read the number of items of the container. The container must have the correct 
	number of items stored. If you need the number of items 
	also read from the stream use containerFromBinaryStream.
	The container must provide stanard support for an iterator

	@param [in] stream The output stream (must be opened in binary mode)
	@param [out] container The container to read
	@tparam ContainerT The type of the container write Warning: DO NOT READ ANY POINTERS
	@see containerFromBinaryStream, fromBinaryStream, containerDataToBinaryStream
*/
template <class ContainerT>
void containerDataFromBinaryStream(
	std::istream &stream, ContainerT &container
)
{
	for(
		typename ContainerT::iterator it = container.begin(),
			endIT = container.end();
		it != endIT;
		++it
	)
	{
		fromBinaryStream( stream, &(*it) );
	}
}

/**
	@brief Writes the content of a container to a binary stream

	This function writes the content of a container to a stream. The number of bytes
	is written as a 64-bit value. Thus the container can be read with 32-bit and 64-bit
	applications. To read the data back to the container, the container type must
	provide an own member function typicaly fromBinaryStream( std::istream &stream ).

	The container must provide stanard support for a const iterator

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] container The container to write
	@tparam ContainerT The type of the container write Warning: DO NOT WRITE ANY POINTERS
	@see containerDataToBinaryStream, toBinaryStream
*/
template <class ContainerT>
inline
void containerToBinaryStream( std::ostream &stream, const ContainerT &container )
{
	uint64	numElements = container.size();
	gak::toBinaryStream( stream, numElements );
//	uint64	elementSize = sizeof( typename ContainerT::value_type );
//	gak::toBinaryStream( stream, elementSize );

	containerDataToBinaryStream( stream, container );
}

/**
	@brief Writes the content of a container to a binary stream

	This function writes the content of a container to a stream. The number of bytes
	is written as a 64-bit value. Thus the container can be read and write with 32-bit 
	and 64-bit applications.

	The container must provide a member function getDataBuffer() returning the address
	of the memory block to write. WARNING: Use this for POD containers, only 

	@param [in] stream The output stream (must be opened in binary mode)
	@param [in] container The container to write
	@tparam ContainerT The type of the container write Warning: DO NOT WRITE ANY POINTERS
	@see fixedArrayToBinaryStream, toBinaryStream, varArrayFromBinaryStream
*/
template <typename ContainerT>
inline
void varArrayToBinaryStream( std::ostream &stream, const ContainerT &container )
{
	uint64	numElements = container.size();

	toBinaryStream( stream, numElements );
	fixedArrayToBinaryStream( stream, container.getDataBuffer(), size_t(numElements) );
}

#undef max

/**
	@brief Reads the content of a container from a binary stream

	This function reads the content of a container from a stream. The number of bytes
	is read as a 64-bit value. Thus the container can be read and write with 32-bit and
	64-bit applications.

	The container must provide a member function getDataBuffer() returning the address
	of the memory block to write and a member function setSize( std::size_t ) to allocate
	the memory from the system. WARNING: Use this for POD containers, only 

	@param [in] stream The output stream (must be opened in binary mode)
	@param [out] container The container to write
	@tparam ContainerT The type of the container write Warning: DO NOT WRITE ANY POINTERS
	@throws AllocError If the size does not fit in a std::size_t variable.
	@see fixedArrayToBinaryStream, toBinaryStream, varArrayToBinaryStream
*/
template <typename ContainerT>
void varArrayFromBinaryStream( std::istream &stream, ContainerT &container )
{
	uint64	numElements;

	gak::fromBinaryStream( stream, &numElements );
	if( numElements > std::numeric_limits<std::size_t>::max() )
	{
		throw AllocError();
	}
	if( numElements )
	{
		container.setSize( std::size_t(numElements) );
		fixedArrayFromBinaryStream(
			stream, container.getDataBuffer(), std::size_t(numElements)
		);
	}
}

/*
	for files
*/
/**
	@brief Writes an object to a file
	@param [in] fileName The name of the file to create
	@param [in] obj The object to write
	@param [in] magic A 32-bit magic that will be check when reading the file
	@tparam OBJ The class of the object. It must be possible to write the object using toBinaryStream
	@throws OpenWriteError If the file could not be created
	@see toBinaryStream, readFromFile
*/
template <class OBJ> 
void writeToFile( 
	const STRING &fileName, 
	const OBJ &obj, 
	uint32 magic, 
	OverwriteMode owm = owmOverwrite 
)
{
	ofstream	stream( fileName, std::ios_base::binary, owm );

	if( !stream )
	{
		throw OpenWriteError( fileName );
	}

	try
	{
		toBinaryStream( stream, magic );
		toBinaryStream( stream, obj );

		stream.close();
	}
	catch( LibraryException &e )
	{
		e.addErrorText( fileName );
		throw;
	}
}

/**
	@brief Reads an object from a file
	@param [in] fileName The name of the file to read
	@param [out] obj The object to read
	@param [in] magicRequired A 32-bit magic that will be checked
	@tparam OBJ The class of the object. It must be possible to read the object using fromBinaryStream
	@throws OpenReadError If the file could not be opened
	@throws BadHeaderError If the magic in the file read does not match magicRequired
	@see fromBinaryStream, writeToFile
*/
template <class OBJ>
void readFromFile( const char *fileName, OBJ *obj, uint32 magicRequired )
{
	std::ifstream	stream( fileName, std::ios_base::binary );
	if( !stream )
	{
		throw OpenReadError( fileName );
	}

	uint32	magicFound;
	fromBinaryStream( stream, &magicFound );
	if( magicRequired != magicFound )
	{
		throw BadHeaderError( fileName );
	}
	try
	{
		fromBinaryStream( stream, obj );
	}
	catch( LibraryException &e )
	{
		e.addErrorText( fileName );
		throw;
	}
}

/*
	human readable stream io
*/

/**
	@brief Prints the content of a container to an output stream

	The container type must provide const_iterator. There must be an output 
	operator available for the value type of the container.

	@param [in] stream The output stream, should be opened in text mode
	@param [in] container The container to print
	@tparam ContainerT The container class.
*/
template <typename ContainerT>
void printContainer( std::ostream &stream, const ContainerT &container )
{
	for(
		typename ContainerT::const_iterator it = container.cbegin(), endIT = container.cend();
		it != endIT;
		++it
	)
	{
		stream << *it << '\n';
	}
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
