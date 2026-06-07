/*
		Project:		GAKLIB
		Module:			KmeansTest.h
		Description:	K-Means is used to cluster any amount of any data
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

#include <gak/kmeans.h>
#include <gak/map.h>
#include <gak/gps.h>

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

class KmeansTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "KmeansTest";
	}
	void RandomTest()
	{
		int test[] = {
			100, 2, 22, 500, 1, 9, 600, 3, 5, 505, 17, 555, 8, 666, 8, 99, 601, 114, 98
		};
		// sorted
		// 1, 2, 3, 5, 8, 8, 9, 17, 22, 98, 99, 100, 114, 500, 505, 555, 600, 601, 666
		gak::Array<int>		testData;
		testData.addCArray( test );

		gak::PairMap< int, gak::Array<const int*> >	theCluster = kMeans(testData, 3);
		UT_ASSERT_EQUAL( theCluster.size(), 3);
		UT_ASSERT_EQUAL( theCluster.getKeyAt(0), 8);
		UT_ASSERT_EQUAL( theCluster.getKeyAt(1), 102);
		UT_ASSERT_EQUAL( theCluster.getKeyAt(2), 571);
		UT_ASSERT_EQUAL( theCluster.getValueAt(0).size(), 9);
		UT_ASSERT_EQUAL( theCluster.getValueAt(1).size(), 4);
		UT_ASSERT_EQUAL( theCluster.getValueAt(2).size(), 6);
	}
	void MeanTest()
	{
		int test[] = {
			100, 2, 22, 500, 1, 9, 600, 3, 5, 505, 17, 555, 8, 666, 8, 99, 601, 114, 98
		};
		gak::Array<int>		testData;
		testData.addCArray( test );

		math::Mean<int>	mean( testData.cbegin(), testData.cend() );

		gak::PairMap< int, gak::Array<const int*> >	theCluster = kMeans(testData, 1);
		UT_ASSERT_EQUAL( theCluster.size(), 1);
		UT_ASSERT_EQUAL( theCluster.getKeyAt(0), mean.getMean());
		UT_ASSERT_EQUAL( theCluster.getValueAt(0).size(), testData.size());
	}
	void EqualTest()
	{
		int test[] = {
			5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
		};
		gak::Array<int>		testData;
		testData.addCArray( test );

		gak::PairMap< int, gak::Array<const int*> >	theCluster = kMeans(testData, 3);
		UT_ASSERT_EQUAL( theCluster.size(), 1);
		UT_ASSERT_EQUAL( theCluster.getKeyAt(0), 5);
		UT_ASSERT_EQUAL( theCluster.getValueAt(0).size(), testData.size());
	}
	template <typename GeoT>
	void GeoTest()
	{
		GeoT	point1( 15, 48 ),
				point2( 10, 48 ),
				point3( 10.5, 48 );
		gak::Array<GeoT>	testData;
		testData.push_back(point1);
		testData.push_back(point2);
		testData.push_back(point3);

		gak::PairMap< GeoT, gak::Array<const GeoT*> >	theCluster = kMeans(testData, 2);
		UT_ASSERT_EQUAL( theCluster.size(), 2);
		UT_ASSERT_EQUAL( theCluster.getValueAt(0).size(), 2);
		UT_ASSERT_EQUAL( theCluster.getValueAt(1).size(), 1);
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "KmeansTest::PerformTest");
		TestScope scope( "PerformTest" );
		RandomTest();
		MeanTest();
		EqualTest();
		{
			doEnterFunctionEx(gakLogging::llInfo, "GeoTest< math::GeoPosition<float> >");
			GeoTest< math::GeoPosition<float> >();
		}
		{
			doEnterFunctionEx(gakLogging::llInfo, "GeoTest< math::GpsPosition<float> >");
			GeoTest< math::GpsPosition<float> >();
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static KmeansTest myKmeansTest;

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
