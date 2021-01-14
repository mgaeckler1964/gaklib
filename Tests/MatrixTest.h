/*
		Project:		GAKLIB
		Module:			MatrixTest.h
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
	virtual const char *GetClassName( void ) const
	{
		return "MatrixTest";
	}
	virtual void PerformTest( void )
	{
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

