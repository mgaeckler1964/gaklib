/*
		Project:		GAKLIB
		Module:			matrix.h
		Description:	A 2 dimensional Array
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

#ifndef GAK_MATRIX_H
#define GAK_MATRIX_H

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

template <typename OBJ, typename ALLOCATOR=Allocator<OBJ> >
class Matrix
{
	public:
	typedef Array<OBJ, ALLOCATOR>	ArrayType;

	private:
	ArrayType	m_data;
	std::size_t	m_numCols, m_numRows;

	private:
	std::size_t index( std::size_t col, std::size_t row ) const
	{
		return row + col*m_numRows;
	}

	void forget()
	{
		m_numCols = m_numRows = 0;
	}
	public:
	Matrix( std::size_t numCols=0, std::size_t numRows=0 )
	: m_numCols(numCols), m_numRows(numRows), m_data( numRows*numCols )
	{
	}

	OBJ *create( std::size_t numCols, std::size_t numRows )
	{
		m_numCols = numCols;
		m_numRows = numRows;

		m_data.clear();
		return m_data.createElements( numRows*numCols );
	}
	void moveFrom( Matrix &source )
	{
		m_data.moveFrom( source.moveFrom );
		this->m_numCols = source.m_numCols;
		this->m_numRows = source.m_numRows;
		source.forget();
	}


	std::size_t getNumRows( void ) const
	{
		return m_numRows;
	}
	void setNumRows( std::size_t numRows );
	void removeRow( std::size_t row );

	std::size_t getNumCols( void ) const
	{
		return m_numCols;
	}
	void setNumCols( std::size_t numCols );

	const OBJ &operator () ( std::size_t col, std::size_t row ) const
	{
		if( col >= m_numCols || row >= m_numRows )
		{
			throw IndexError();
		}
		return m_data[index( col, row )];
	}
	OBJ &operator () ( std::size_t col, std::size_t row )
	{
		if( col >= m_numCols || row >= m_numRows )
		{
			throw IndexError();
		}

		return m_data[index( col, row )];
	}

	void fill( 
		std::size_t leftCol, std::size_t upperRow, 
		std::size_t rightCol, std::size_t lowerRow, 
		const OBJ &value 
	);
	void extract( 
		std::size_t leftCol, std::size_t upperRow, 
		std::size_t rightCol, std::size_t lowerRow, 
		Matrix<OBJ,ALLOCATOR> *result 
	);

	const OBJ *getColumn( std::size_t col ) const
	{
		if( col >= m_numCols )
		{
			throw IndexError();
		}
		return m_data.getDataBuffer() + index( col, 0 );
	}
	OBJ *getColumn( std::size_t col )
	{
		if( col >= m_numCols )
		{
			throw IndexError();
		}
		return m_data.getDataBuffer() + index( col, 0 );
	}
	const OBJ * operator [] ( std::size_t col ) const
	{
		return getColumn( col );
	}
	OBJ * operator [] ( std::size_t col )
	{
		return getColumn( col );
	}
	void getColumn( std::size_t col, ArrayType *colData );
	void getRow( std::size_t row, ArrayType *colData );
	const ArrayType &getArray( void ) const
	{
		return m_data;
	}
};

template<typename  OBJ>
class PODmatrix : public Matrix<OBJ, PODallocator<OBJ> >
{
	public:
	PODmatrix( std::size_t numCols=0, std::size_t numRows=0 )
	: Matrix<OBJ, PODallocator<OBJ> >( numCols, numRows )
	{}

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

template <typename OBJ, typename ALLOCATOR> 
void Matrix<OBJ, ALLOCATOR>::setNumRows( std::size_t newNumRows )
{
	std::size_t	oldNumRows = getNumRows();

	if( oldNumRows != newNumRows )
	{
		std::size_t				numRows = math::min( oldNumRows, newNumRows );
		std::size_t				numCols = getNumCols();
		Array<OBJ, ALLOCATOR>	newArray( numCols * newNumRows );

		for( std::size_t col=0; col<numCols; ++col )
		{
			OBJ *target = newArray.getDataBuffer() + col*newNumRows;
			OBJ *src = m_data.getDataBuffer() + col*oldNumRows;
		
			for( std::size_t row=0; row<numRows; ++row )
			{
				*target++ = *src++;
			}
		}
		m_data.moveFrom( newArray );
		m_numRows = newNumRows;
	}
}

template <typename OBJ, typename ALLOCATOR> 
void Matrix<OBJ, ALLOCATOR>::removeRow( std::size_t row2delete )
{
	std::size_t	numRows = getNumRows();

	if( row2delete < numRows )
	{
		std::size_t				newNumRows = numRows-1;
		std::size_t				numCols = getNumCols();
		Array<OBJ, ALLOCATOR>	newArray( numCols * newNumRows );

		for( std::size_t col=0; col<numCols; ++col )
		{
			OBJ *target = newArray.getDataBuffer() + col*newNumRows;
			OBJ *src = m_data.getDataBuffer() + col*numRows;
		
			for( std::size_t row=0; row<numRows; ++row )
			{
				if( row != row2delete )
				{
					*target++ = *src++;
				}
				else
				{
					src++;
				}
			}
		}
		m_data.moveFrom( newArray );
		m_numRows = newNumRows;
	}
	else
	{
		throw IndexError();
	}
}

template <typename OBJ, typename ALLOCATOR> 
void Matrix<OBJ, ALLOCATOR>::setNumCols( std::size_t newNumCols )
{
	if( m_numCols != newNumCols )
	{
		std::size_t	oldSize = m_data.size();
		std::size_t	newSize = newNumCols * m_numRows;
		if( oldSize < newSize )
		{
			m_data.createElements( newSize - oldSize );
		}
		else
		{
			m_data.removeElementsAt( newSize, oldSize-newSize );
		}
		m_numCols = newNumCols;
	}
}

template <typename OBJ, typename ALLOCATOR> 
void Matrix<OBJ, ALLOCATOR>::fill(
	std::size_t leftCol, std::size_t upperRow,
	std::size_t rightCol, std::size_t lowerRow,
	const OBJ &value
)
{
	if( leftCol < m_numCols && rightCol <= m_numCols
	&& upperRow < m_numRows && lowerRow <= m_numRows
	&& leftCol <= rightCol && upperRow <= lowerRow )
	{
		for( std::size_t row=upperRow; row < lowerRow; row++ )
		{
			for( std::size_t col = leftCol; col < rightCol; col++ )
			{
				m_data[index(col, row)] = value;
			}
		}
	}
	else
	{
		throw IndexError();
	}
}

template <typename OBJ, typename ALLOCATOR>
void Matrix<OBJ, ALLOCATOR>::extract(
	std::size_t leftCol, std::size_t upperRow,
	std::size_t rightCol, std::size_t lowerRow,
	Matrix<OBJ, ALLOCATOR> *result
)
{
	if( leftCol < m_numCols && rightCol <= m_numCols
	&& upperRow < m_numRows && lowerRow <= m_numRows
	&& leftCol <= rightCol && upperRow <= lowerRow )
	{
		std::size_t	numCols = rightCol - leftCol;
		std::size_t	numRows = lowerRow - upperRow;
		OBJ		*target = result->create( numCols, numRows );
		for( std::size_t col = leftCol; col < rightCol; col++ )
		{
			for( std::size_t row=upperRow; row < lowerRow; row++ )
			{
				*target++ = m_data[index(col, row)];
			}
		}
	}
	else
	{
		throw IndexError();
	}
}

template <typename OBJ, typename ALLOCATOR>
void Matrix<OBJ, ALLOCATOR>::getColumn( std::size_t col, ArrayType *colData )
{
	if( col < getNumCols() )
	{
		colData->clear();

		const OBJ	*src = m_data.getDataBuffer() + index( col, 0 );
		OBJ			*target = colData->createElements( getNumRows() );

		for( std::size_t row=0; row < getNumRows(); ++row )
		{
			*target++ = *src++;
		}
	}
	else
	{
		throw IndexError();
	}
}

template <typename OBJ, typename ALLOCATOR>
void Matrix<OBJ, ALLOCATOR>::getRow( std::size_t row, ArrayType *rowData )
{
	if( row < getNumRows() )
	{
		rowData->clear();

		const OBJ	*src = m_data.getDataBuffer() + index( 0, row );
		OBJ			*target = rowData->createElements( getNumCols() );

		for( std::size_t col=0; col < getNumCols(); ++col )
		{
			*target++ = *src;
			src += getNumRows();
		}
	}
	else
	{
		throw IndexError();
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename OBJ, typename ALLOCATOR>
inline void moveAssign( Matrix<OBJ, ALLOCATOR> &target, Matrix<OBJ, ALLOCATOR> &source )
{
	target.moveFrom( source );
}

template<typename OBJ>
inline void moveAssign( PODmatrix<OBJ> &target, PODmatrix<OBJ> &source )
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

#endif	// GAK_MATRIX_H