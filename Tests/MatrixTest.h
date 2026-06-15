/*
		Project:		GAKLIB
		Module:			MatrixTest.h
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/matrix.h>

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

class MatrixTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "MatrixTest";
	}
	void SimpleIteratorTest()
	{
		Matrix<int>	mat(3,3);

		Matrix<int>::row_iterator	it = mat.rowbegin( 2 );
		Matrix<int>::row_iterator	endIT = mat.rowend( 2 );

		// test + and +=
		Matrix<int>::row_iterator	tmpEnd = it + 3;
		UT_ASSERT_EQUAL( endIT, tmpEnd );
		it += 3;
		UT_ASSERT_EQUAL( endIT, it );

		it = mat.rowbegin( 2 );

		// test - and -=
		Matrix<int>::row_iterator	tmpIT = endIT - 3;
		UT_ASSERT_EQUAL( it, tmpIT );
		endIT -= 3;
		UT_ASSERT_EQUAL( it, endIT );

		it = mat.rowbegin( 2 );
		endIT = mat.rowend( 2 );

		// test (pre/post)increment
		Matrix<int>::row_iterator	it2 = it;
		UT_ASSERT_EQUAL( it, it2++ );
		UT_ASSERT_NOT_EQUAL( it, it2 );
		it2 = it;
		UT_ASSERT_NOT_EQUAL( it, ++it2 );
		UT_ASSERT_NOT_EQUAL( endIT, ++it2 );
		UT_ASSERT_EQUAL( endIT, ++it2 );


		// test (pre/post)increment
		it2 = endIT;
		UT_ASSERT_EQUAL( endIT, it2-- );
		UT_ASSERT_NOT_EQUAL( endIT, it2 );
		it2 = endIT;
		UT_ASSERT_NOT_EQUAL( it, --it2 );
		UT_ASSERT_NOT_EQUAL( it, --it2 );
		UT_ASSERT_EQUAL( it, --it2 );
	}

	void IteratorTest(const size_t numCols, const size_t numRows )
	{
		Matrix<int>	mat(numCols,numRows);
		int i = 0;
		const int	*ptr = nullptr;

		/*
			(const_)iterator over the entire matrix
		*/
		i = 0;
		for( 
			Matrix<int>::iterator it = mat.begin(), endIT = mat.end();
			it != endIT;
			++it
		)
		{
			*it = i++;
		}

		i = 0;
		for( 
			Matrix<int>::const_iterator it = mat.cbegin(), endIT = mat.cend();
			it != endIT;
			++it
		)
		{
			UT_ASSERT_EQUAL( *it, i++ );
		}

		/*
			(const_)reverse_iterator over the entire matrix
		*/
		ptr = nullptr;
		i = 0;
		for( 
			Matrix<int>::reverse_iterator it = mat.rbegin(), endIT = mat.rend();
			it != endIT;
			++it
		)
		{
			*it = i++;

			if( ptr )
				UT_ASSERT_LESS( (const int*)it, ptr );
			ptr = it;
		}
		i = 0;
		ptr = nullptr;
		for( 
			Matrix<int>::const_reverse_iterator it = mat.crbegin(), endIT = mat.crend();
			it != endIT;
			++it
		)
		{
			UT_ASSERT_EQUAL( *it, i++ );

			if( ptr )
				UT_ASSERT_LESS( (const int*)it, ptr );
			ptr = it;
		}


		/*
			(const_)iterator for columns
		*/
		i = 0;
		for( size_t col=0; col<numCols; ++col )
		{
			for( 
				Matrix<int>::iterator it = mat.begin(col), endIT = mat.end(col);
				it != endIT;
				++it
			)
			{
				int val = int(col << 8 | i++);
				*it = val;
			}
		}

		i = 0;
		for( size_t col=0; col<numCols; ++col )
		{
			for( 
				Matrix<int>::const_iterator it = mat.cbegin(col), endIT = mat.cend(col);
				it != endIT;
				++it
			)
			{
				int val = int(col << 8 | i++);
				UT_ASSERT_EQUAL( *it, val );
			}
		}

		/*
			(const_)reverse_iterator for columns
		*/
		ptr = nullptr;
		i = 0;
		for( size_t col=0; col<numCols; ++col )
		{
			for( 
				Matrix<int>::reverse_iterator it = mat.rbegin(col), endIT = mat.rend(col);
				it != endIT;
				++it
			)
			{
				int val = int(col << 8 | i++);
				*it = val;

				if( ptr )
					UT_ASSERT_LESS( (const int*)it, ptr );
				ptr = it;
			}
		}

		ptr = nullptr;
		i = 0;
		for( size_t col=0; col<numCols; ++col )
		{
			for( 
				Matrix<int>::const_reverse_iterator it = mat.crbegin(col), endIT = mat.crend(col);
				it != endIT;
				++it
			)
			{
				int val = int(col << 8 | i++);
				UT_ASSERT_EQUAL( *it, val );

				if( ptr )
					UT_ASSERT_LESS( (const int*)it, ptr );
				ptr = it;
			}
		}

		/*
			(const_)row_iterator for rows
		*/
		i = 0;
		for( size_t row=0; row<numRows; ++row )
		{
			ptr = nullptr;
			for( 
				Matrix<int>::row_iterator it = mat.rowbegin(row), endIT = mat.rowend(row);
				it != endIT;
				++it
			)
			{
				int val = int(row << 8 | i++);
				*it = val;

				if( ptr )
					UT_ASSERT_GREATER( (const int*)it, ptr );
				ptr = it;
			}
		}

		i = 0;
		for( size_t row=0; row<numRows; ++row )
		{
			ptr = nullptr;
			for( 
				Matrix<int>::const_row_iterator it = mat.crowbegin(row), endIT = mat.crowend(row);
				it != endIT;
				++it
			)
			{
				int val = int(row << 8 | i++);
				UT_ASSERT_EQUAL( *it, val );

				if( ptr )
					UT_ASSERT_GREATER( (const int*)it, ptr );
				ptr = it;
			}
		}
		/*
			(const_)row_reverse_iterator for rows
		*/
		i = 0;
		for( size_t row=0; row<numRows; ++row )
		{
			ptr = nullptr;
			for( 
				Matrix<int>::row_reverse_iterator it = mat.rowrbegin(row), endIT = mat.rowrend(row);
				it != endIT;
				++it
			)
			{
				int val = int(row << 8 | i++);
				*it = val;

				if( ptr )
					UT_ASSERT_LESS( (const int*)it, ptr );
				ptr = it;
			}
		}

		i = 0;
		for( size_t row=0; row<numRows; ++row )
		{
			ptr = nullptr;
			for( 
				Matrix<int>::const_row_reverse_iterator it = mat.crowrbegin(row), endIT = mat.crowrend(row);
				it != endIT;
				++it
			)
			{
				int val = int(row << 8 | i++);
				UT_ASSERT_EQUAL( *it, val );

				if( ptr )
					UT_ASSERT_LESS( (const int*)it, ptr );
				ptr = it;
			}
		}
	}
	void MatrixProductTest()
	{
		{
			PODmatrix<int>	mat1(3,5), mat2(4,3);
			PODmatrix<int>	result = matrixProduct( mat1, mat2 );

			UT_ASSERT_EQUAL(result.getNumCols(), 4);
			UT_ASSERT_EQUAL(result.getNumRows(), 5);
		}
		{
			PODmatrix<int>	mat1(3,1), mat2(1,3);
			mat1(0,0) = 1;
			mat2(0,0) = 1;
			PODmatrix<int>	result = matrixProduct( mat1, mat2 );
			UT_ASSERT_EQUAL(result.getNumCols(), 1);
			UT_ASSERT_EQUAL(result.getNumRows(), 1);
			UT_ASSERT_EQUAL(result(0,0), 1);
		}
	}

	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "MatrixTest::PerformTest");
		TestScope scope( "PerformTest" );

		const size_t			numMaxCols = 32;
		const size_t			numMaxRows = 16;
		int						matrix[numMaxCols][numMaxRows];
		Matrix<int>				xmatrix(numMaxCols,numMaxRows);
		Matrix<int>				extract;
		Matrix<int>::ArrayType	colData;
		Matrix<int>::ArrayType	rowData;

		const size_t			startCol = 3;
		const size_t			startRow = 5;
		const size_t			numCols = 2;
		const size_t			numRows = 3;

		ptrdiff_t				diff = &matrix[1][0] - &matrix[0][0];
		ptrdiff_t				xdiff = &xmatrix(1,0) - &xmatrix(0,0);

		UT_ASSERT_EQUAL( diff, xdiff );
		std::cout << diff << std::endl;

		diff = &matrix[0][1] - &matrix[0][0];
		xdiff = &xmatrix(0,1) - &xmatrix(0,0);
		UT_ASSERT_EQUAL( diff, xdiff );
		std::cout << diff << std::endl;
		int val=0;
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			for( size_t row = 0; row<numMaxRows; ++row )
			{
				xmatrix( col, row ) = val;
				matrix[col][row] = val;
				val++;
			}
		}
		
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			for( size_t row = 0; row<numMaxRows; ++row )
			{
				UT_ASSERT_EQUAL( xmatrix( col, row ), xmatrix[col][row] );
			}
		}

		xmatrix.extract( startCol, startRow, startCol+numCols, startRow+numRows, &extract );
		for( size_t col = 0; col<numCols; ++col )
		{
			for( size_t row = 0; row<numRows; ++row )
			{
				UT_ASSERT_EQUAL( extract( col, row ), xmatrix( col+startCol, row+startRow ) );
			}
		}

#if 0
		xmatrix.getColumn( startCol, &colData );
		for( size_t row = 0; row<numMaxRows; ++row )
		{
			UT_ASSERT_EQUAL( colData[row], xmatrix( startCol, row ) );
		}

		xmatrix.getRow( startRow, &rowData );
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			UT_ASSERT_EQUAL( rowData[col], xmatrix( col, startRow ) );
		}
#endif

		xmatrix.setNumRows( numMaxRows*2 );
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			for( size_t row = 0; row<numMaxRows; ++row )
			{
				UT_ASSERT_EQUAL( xmatrix[col][row], matrix[col][row] );
			}
		}

		xmatrix.setNumCols( numMaxCols*2 );
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			for( size_t row = 0; row<numMaxRows; ++row )
			{
				UT_ASSERT_EQUAL( xmatrix[col][row], matrix[col][row] );
			}
		}

		xmatrix.setNumCols( numMaxCols );
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			for( size_t row = 0; row<numMaxRows; ++row )
			{
				UT_ASSERT_EQUAL( xmatrix[col][row], matrix[col][row] );
			}
		}

		xmatrix.setNumRows( numMaxRows );
		for( size_t col = 0; col<numMaxCols; ++col )
		{
			for( size_t row = 0; row<numMaxRows; ++row )
			{
				UT_ASSERT_EQUAL( xmatrix[col][row], matrix[col][row] );
			}
		}

		MatrixProductTest();
		IteratorTest(5,3);
		IteratorTest(3,3);
		IteratorTest(3,5);
		SimpleIteratorTest();

		/* TODO 1 -ogak -cTest : Add assertions */
		xmatrix.removeRow( xmatrix.getNumRows()/2 );
		xmatrix.fill( 0, 0, 1, 1, 666 );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static MatrixTest myMatrixTest;

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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

