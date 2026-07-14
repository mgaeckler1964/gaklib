/*
		Project:		GAKLIB
		Module:			matrix.h
		Description:	A 2 dimensional Array
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

namespace internal
{
/***
	the RowIterator iterates throug all columns of a row
*/
template <class ReferenceT>
class RowIterator : public ReverseIterator<ReferenceT>
{
	typedef ReverseIterator<ReferenceT> Super;
	typedef typename Super::pointer	pointer;

	long m_numRows;
	public:
	RowIterator( pointer startValue, long numRows ) : Super( startValue ), m_numRows(numRows)
	{}

	const RowIterator &operator ++()				// pre inkrement
	{
		this->m_data += m_numRows;
		return *this;
	}
	RowIterator operator ++( int )					// post inkrement
	{
		RowIterator		tmp( *this );
		this->m_data += m_numRows;
		return tmp;
	}

	RowIterator operator + ( int offset ) const
	{
		RowIterator res = *this;
		res.m_data += offset*m_numRows;
		return res;
	}
	const RowIterator &operator += ( int offset )
	{
		this->m_data += offset*m_numRows;
		return *this;
	}


	const RowIterator &operator --()				// pre dekrement
	{
		this->m_data -= m_numRows;
		return *this;
	}
	RowIterator operator --( int )					// post dekrement
	{
		RowIterator		tmp( *this );
		this->m_data -= m_numRows;
		return tmp;
	}
	RowIterator operator - ( int offset ) const
	{
		RowIterator res = *this;
		res.m_data -= offset*m_numRows;
		return res;
	}
	const RowIterator &operator -= ( int offset )
	{
		this->m_data -= offset*m_numRows;
		return *this;
	}
};

}

template <typename OBJ, typename ALLOCATOR=Allocator<OBJ> >
class Matrix
{
	public:
	typedef Array<OBJ, ALLOCATOR>						ArrayType;
	typedef typename ArrayType::value_type				value_type;

	private:
	ArrayType	m_data;
	std::size_t	m_numCols, m_numRows;

	private:
	std::size_t index( std::size_t col, std::size_t row ) const
	{
		return col*m_numRows + row;
	}

	void forget()
	{
		m_numCols = m_numRows = 0;
	}

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


	const OBJ * operator [] ( std::size_t col ) const
	{
		return getColumn( col );
	}
	OBJ * operator [] ( std::size_t col )
	{
		return getColumn( col );
	}
#if 1	/// TODO: use the iterators instead
	void getColumn( std::size_t col, ArrayType *colData );
	void getRow( std::size_t row, ArrayType *colData );
#endif
	const ArrayType &getArray() const
	{
		return m_data;
	}
	/***************************************************************
		iterators
	****************************************************************/
	typedef typename ArrayType::iterator				iterator;
	iterator begin() { return m_data.begin(); }
	iterator end() { return m_data.end(); }
	iterator begin( size_t col ) { return getColumn( col ); }
	iterator end( size_t col ) { return getColumn( col ) + index( 1, 0 ); }

	typedef typename ArrayType::const_iterator			const_iterator;
	const_iterator cbegin() const { return m_data.cbegin(); }
	const_iterator cend() const { return m_data.cend(); }
	const_iterator cbegin( size_t col ) const { return getColumn( col ); }
	const_iterator cend( size_t col ) const { return getColumn( col ) + index( 1, 0 ); }

	typedef typename ArrayType::reverse_iterator		reverse_iterator;
	reverse_iterator rbegin() { return m_data.rbegin(); }
	reverse_iterator rend() { return m_data.rend(); }
	reverse_iterator rbegin( size_t col ) 
	{
		if( col >= m_numCols )
		{
			throw IndexError();
		}
		return reverse_iterator( m_data.getDataBuffer() + index( m_numCols-col-1, m_numRows-1) );
	}
	reverse_iterator rend( size_t col )
	{
		if( col >= m_numCols )
		{
			throw IndexError();
		}
		return reverse_iterator( m_data.getDataBuffer() + index( m_numCols-col-1, size_t(-1) ) );
	}

	typedef typename ArrayType::const_reverse_iterator	const_reverse_iterator;
	const_reverse_iterator crbegin() const { return m_data.crbegin(); }
	const_reverse_iterator crend() const { return m_data.crend(); }
	const_reverse_iterator crbegin( size_t col ) const
	{
		if( col >= m_numCols )
		{
			throw IndexError();
		}
		return const_reverse_iterator( m_data.getDataBuffer() + index(m_numCols-col-1, m_numRows-1) );
	}

	const_reverse_iterator crend( size_t col ) const
	{
		if( col >= m_numCols )
		{
			throw IndexError();
		}
		return const_reverse_iterator( m_data.getDataBuffer() + index(m_numCols-col-1, size_t(-1)) );
	}


	typedef internal::RowIterator< internal::Reference< OBJ > >			
						row_iterator;
	row_iterator rowbegin( size_t row ) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return row_iterator( m_data.begin() + index( 0, row ), long(m_numRows) ); 
	}
	row_iterator rowend(size_t row) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return row_iterator( m_data.begin() + index( m_numCols, row ), long(m_numRows) ); 
	}

	typedef internal::RowIterator< internal::ConstReference< OBJ > >	
						const_row_iterator;
	const_row_iterator crowbegin( size_t row ) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return const_row_iterator( m_data.begin() + index( 0, row ), long(m_numRows) ); 
	}
	const_row_iterator crowend(size_t row) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return const_row_iterator( m_data.begin() + index( m_numCols, row ), long(m_numRows) ); 
	}

	typedef row_iterator	row_reverse_iterator;
	row_reverse_iterator rowrbegin( size_t row ) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return row_reverse_iterator( m_data.begin() + index( m_numCols-1, m_numRows-row-1 ), long(0UL-m_numRows) ); 
	}
	row_reverse_iterator rowrend(size_t row) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return row_reverse_iterator( m_data.begin() + index( size_t(-1), m_numRows-row-1 ), long(0UL-m_numRows) ); 
	}
	typedef const_row_iterator	const_row_reverse_iterator;
	const_row_reverse_iterator crowrbegin( size_t row ) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return const_row_reverse_iterator( m_data.begin() + index( m_numCols-1, m_numRows-row-1 ), long(0UL-m_numRows) ); 
	}
	const_row_reverse_iterator crowrend(size_t row) 
	{ 
		if( row >= m_numRows )
		{
			throw IndexError();
		}
		return const_row_reverse_iterator( m_data.begin() + index( size_t(-1), m_numRows-row-1 ), long(0UL-m_numRows) ); 
	}
};

template<typename  OBJ>
class PODmatrix : public Matrix<OBJ, PODallocator<OBJ> >
{
	typedef Matrix<OBJ, PODallocator<OBJ> >	Super;
	public:
	PODmatrix( std::size_t numCols=0, std::size_t numRows=0 )
	: Matrix<OBJ, PODallocator<OBJ> >( numCols, numRows )
	{}
	PODmatrix( const Super &src ) : Super(src) {}

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

#if 1	/// TODO: use the iterators instead
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
#endif

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

template <typename MatrixT>
MatrixT matrixProduct(  const MatrixT &mat1, const MatrixT &mat2 )
{
	const size_t maxIndex = math::min( mat1.getNumCols(), mat2.getNumRows() );

	MatrixT	result( mat2.getNumCols(), mat1.getNumRows() );
	for( size_t col=0; col<result.getNumCols(); ++col )
	{
		for( size_t row=0; row<result.getNumRows(); ++row )
		{
			// Warning: Old compiler like Boland C++ Builder 5 may produce bad code
#ifdef __BORLANDC__
			typename MatrixT::value_type sum = MatrixT::value_type();
#else
			typename MatrixT::value_type sum = typename MatrixT::value_type();
#endif
			for( size_t i=0; i<maxIndex; ++i )
			{
				sum += mat1(i,row)*mat2(col,i);
			}
			result(col, row) = sum;
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

#endif	// GAK_MATRIX_H
