/*
		Project:		
		Module:			
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

#ifndef GAK_ARRAY_FILE_H
#define GAK_ARRAY_FILE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/directoryEntry.h>
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

/**
	@brief writes binary data to a file
	@param [in] array the array to write
	@param [in] fileName the file name to write to
	@exception WriteError in case of an I/O error
*/
template <class OBJ, class ALLOCATOR>
void writeToFile( const ArrayBase<OBJ, ALLOCATOR> &array, const STRING &fileName )
{
	std::ofstream stream( fileName, std::ios_base::binary );
	if( stream )
	{
		try
		{
			if( is_binary<OBJ>::value )
			{
				fixedArrayToBinaryStream( stream, array.getDataBuffer(), array.size() );
			}
			else
			{
				containerDataToBinaryStream( stream, array );
			}
		}
		catch( LibraryException &e )
		{
			e.addErrorText( fileName );
/*@*/		throw;
		}

	}
	else
	{
/*@*/	throw OpenWriteError( fileName );
	}
}


/**
	@brief reads binary data from a file
	@param [out] array the PODarray to read
	@param [in] fileName the file name to read from
	@exception ReadError in case of an I/O error
*/
template <class OBJ, class ALLOCATOR>
void readFromFile( ArrayBase<OBJ, ALLOCATOR> *array, const STRING &fileName )
{
	DirectoryEntry	theFile( fileName );

	if( theFile.fileSize > std::numeric_limits<std::size_t>::max() )
	{
		throw AllocError();
	}

	std::size_t numElements = size_t(theFile.fileSize / sizeof( OBJ ));

	std::ifstream stream( fileName, std::ios_base::binary );
	if( stream )
	{
		try
		{
			array->setSize( numElements );

			if( is_binary<OBJ>::value )
			{
				fixedArrayFromBinaryStream( stream, array->getDataBuffer(), numElements );
			}
			else
			{
				containerDataFromBinaryStream( stream, *array );
			}
		}
		catch( LibraryException &e )
		{
			e.addErrorText( fileName );
/*@*/		throw;
		}

	}
	else
	{
/*@*/	throw OpenWriteError( fileName );
	}
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif // GAK_ARRAY_FILE_H