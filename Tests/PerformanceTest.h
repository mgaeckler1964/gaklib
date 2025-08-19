/*
		Project:		GAKLIB
		Module:			PerformanceTest.h
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

#include <string>

#include <gak/btree.h>
#include <gak/sortedArray.h>

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

class PerformanceTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "PerformanceTest";
	}
	void doTest( size_t numData )
	{
		Btree<STRING>			myBtree;
		SortedArray<STRING>		myArray;

		clock_t		startTime = clock();
		for( size_t i=0; i<numData; i++ )
		{
			myBtree += formatNumber( randomNumber( int(numData) ) );
		}
		clock_t endTime = clock() - startTime;
		std::cout << "Btree " << numData << ',' << endTime << std::endl;
		size_t depth;
		myBtree.test(&depth);
		std::cout << "Depth: " << depth << std::endl;
		myArray.setCapacity( numData, true );
		startTime = clock();
		for( size_t i=0; i<numData; i++ )
		{
			myArray.addElement( formatNumber( randomNumber( int(numData) ) ) );
		}
		endTime = clock() - startTime;
		std::cout << "Array " << numData << ',' << endTime << std::endl;
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "PerformanceTest::PerformTest");
		TestScope scope( "PerformTest" );

		for( size_t i=0, num=10000; i<5; ++i, num += 10000 )
		{
			doTest( num );
		}
		/*
			compare std vs gak lib
		*/
		const size_t numItems = 100000;

		clock_t	startTime = clock();
		std::vector<std::string>	myStdVector;
		for( size_t i=0; i<numItems; i++ )
		{
			std::string	hello = "Hello world";
			myStdVector.push_back( hello );
		}
		clock_t endTimeStd = clock() - startTime;

		startTime = clock();
		gak::Array<gak::STRING>	myGakVector;
		myGakVector.setChunkSize( numItems );
		for( size_t i=0; i<numItems; i++ )
		{
			gak::STRING	&hello = myGakVector.createElement();
			hello = "Hello world";
		}
		clock_t endTimeGak = clock() - startTime;

		startTime = clock();
		gak::Array<std::string>	myStdGakVector;
		for( size_t i=0; i<numItems; i++ )
		{
			std::string	&hello = myStdGakVector.createElement();
			hello = "Hello world";
		}
		clock_t endTimeStdGak = clock() - startTime;

		startTime = clock();
		std::vector<gak::STRING>	myGakStdVector;
		for( size_t i=0; i<numItems; i++ )
		{
			gak::STRING	hello = "Hello world";
			myGakStdVector.push_back( hello );
		}
		clock_t endTimeGakStd = clock() - startTime;

		for( size_t i=0; i<numItems; i++ )
		{
			std::string test = myStdVector[i];
			if( test == "Hello world" )
			{
				myStdGakVector += test;
			}
		}
		UT_ASSERT_EQUAL( myStdGakVector.size(), size_t(numItems*2) );

		UT_ASSERT_LESSEQ( double(endTimeGak)/double(CLOCKS_PER_SEC), double(endTimeStd)/double(CLOCKS_PER_SEC) );

		std::cout << "GAKLIB: " << endTimeGak << " vs STDLIB: " << endTimeStd << '\n';
		std::cout << "std:string in gak::Array: " << endTimeStdGak << " vs " << "gak::STRING in std::vector: " << endTimeGakStd << '\n';
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static PerformanceTest myPerformanceTest;

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

