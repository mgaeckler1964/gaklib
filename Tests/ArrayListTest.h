/*
		Project:		GAKLIB
		Module:			ArrayListTest.h
		Description:	
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
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

#include <gak/sortedArray.h>
#include <gak/string.h>
#include <gak/list.h>

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

class vTEST
{
	int size;

	public:
	vTEST()
	{
		std::cout << "const\n";
		size = 0;
	}
	vTEST( const vTEST &src )
	{
		std::cout << "copy\n";
		size = src.size;
	}
};

class ArrayListTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "ArrayListTest";
	}
	const STRING &getConstString( const Array<STRING> &myArray, size_t index )
	{
		return myArray[index];
	}

	STRING &getString( Array<STRING> &myArray, size_t index )
	{
		return myArray[index];
	}
	struct LenPrint
	{
		static void perform( const STRING &src )
		{
			std::cout << "len "<< src << " = " << src.strlen() << '\n';
		}
		void operator() ( const STRING &src ) const
		{
			perform( src );
		}
	};

	virtual void PerformTest( void )
	{
		/*
			usage of LISTs and ARRAYs
		*/
		List<STRING>	myList;
		Array<STRING>	myArray;

		List<STRING>::Cursor	myListCursor;

		size_t	i;

		for( i=0; i<10; i++ )
		{
			STRING	&newItem = myList.createElement();

			newItem = "myList";
			newItem += formatNumber( i );
		}

		for( i=0; i<10; i++ )
		{
			STRING	&newItem = myArray.createElement();

			newItem = "myArray";
			newItem += formatNumber( i );
		}

		UT_ASSERT_EQUAL( myList.size(), size_t(10) );
		UT_ASSERT_EQUAL( myArray.size(), size_t(10) );

		for(
			i=0, myListCursor = myList.getFirstCursor();
			myListCursor.isValid();
			i++, myListCursor++
		)
		{
			const STRING &oldItem = myList[myListCursor];
			UT_ASSERT_EQUAL( oldItem, myList[i] );
		}

		for(
			i=9, myList.getLast(&myListCursor);
			myListCursor.isValid();
			i--, myListCursor--
		)
		{
			const STRING &oldItem = *myListCursor;
			UT_ASSERT_EQUAL( oldItem, myList[i] );
		}


		/*
			exceptions
		*/
		// std::string	val  = myStdVector[NUM_ITEMS];

//		gak::STRING &val2 = getString( myGakVector, NUM_ITEMS );
//		const gak::STRING &val3 = getConstString( myGakVector, NUM_ITEMS );
		/*
			sorted arrays
		*/
		{
			SortedArray<int>	sortedVals;
			for( i=0; i<10; i++ )
			{
				sortedVals.addElement( randomNumber( 255 ) );
			}
			int last = sortedVals[0];
			int	next;
			std::cout << last << ' ';
			for( i=1; i<10; i++ )
			{
				next = sortedVals[i];
				std::cout << next << ' ';

				UT_ASSERT_LESSEQ( last, next );
				last = next;
			}
			std::cout << '\n';
		}
		{
			SortedArray<int>	sortedVals;
			for( i=0; i<10; i++ )
			{
				sortedVals.addElement( randomNumber( 255 ) );
			}

			int last = sortedVals[0];
			int	next;
			std::cout << last << ' ';
			for( i=1; i<10; i++ )
			{
				next = sortedVals[i];
				std::cout << next << ' ';

				UT_ASSERT_LESSEQ( last, next );
				last = next;
			}
			std::cout << '\n';
		}
		{
			SortedArray<STRING>	sortedVals;
			for( i=0; i<10; i++ )
			{
				sortedVals.addElement( formatNumber( randomNumber( 255 ), 3 ) );
			}
			STRING last = sortedVals[0];
			STRING	next;
			std::cout << last << ' ';
			for( i=1; i<10; i++ )
			{
				next = sortedVals[i];
				std::cout << next << ' ';

				UT_ASSERT_LESSEQ( last, next );
				last = next;
			}
			std::cout << '\n';

			LenPrint	funktor;
			sortedVals.forEach( funktor );

#if !defined( __BORLANDC__ ) || __BORLANDC__ > 0x551
			sortedVals.forEach<LenPrint>();
#endif
		}

		std::vector<vTEST>	vtVect;
		vTEST	hello;
		vtVect.push_back( hello );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static ArrayListTest myArrayListTest;

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

