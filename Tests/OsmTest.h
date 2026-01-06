/*
		Project:		GAKLIB
		Module:			OsmTest.h
		Description:	Open Street Map test
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

#include <gak/osm.h>
#include <gak/tmpfile.h>

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

const STRING osmPath = "C:\\Cache\\OSM";

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class OsmTest : public UnitTest
{
	public:
	virtual const char *GetClassName() const
	{
		return "OsmTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "OsmTest::PerformTest");
		TestScope scope( "PerformTest" );
		SyntheticTest();
		AndorraTest();
	}
	void SyntheticTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "OsmTest::SyntheticTest");

		const OSMbuilder::layer_key_type	layer = 0;
		const OSMbuilder::node_key_type		node1key = 1;
		const OSMbuilder::node_key_type		node2key = 2;
		const OSMbuilder::link_key_type		link3key = 3;
		const OsmAreaKeyT					area4key = 4;
		const OsmPlaceKeyT					place5key = 5;

		const OSMbuilder::node_key_type		node6key = 6;
		const OSMbuilder::node_key_type		node7key = 7;
		const OSMbuilder::link_key_type		link8key = 8;

		const OSMbuilder::link_key_type		link9key = 9;	// node 7 to node2

		const OsmAreaKeyT					area10key = 10;
		const OsmPlaceKeyT					place11key = 11;

		OSMbuilder	builder3;

		TempFileName	tmpName1(true);
		TempFileName	tmpName2(true);

		// data for the first tile
		OsmNode	node1;
		node1.pos.latitude = 45;
		node1.pos.longitude = 14.6f;
		OsmNode	node2;
		node2.pos.latitude = 45.1f;
		node2.pos.longitude = 14.7f;
		OsmLink	link3;
		link3.length = 666;
		link3.type = OsmLink::subway;

		// data for the second tile
		OsmNode	node6;
		node6.pos.latitude = 45;
		node6.pos.longitude = 24.6f;
		OsmNode	node7;
		node7.pos.latitude = 55.1f;
		node7.pos.longitude = 24.7f;
		OsmLink	link8;
		link8.length = 333;
		link8.type = OsmLink::cycleway;
		OsmLink	link9;
		link9.length = 999;
		link9.type = OsmLink::tramway;

		{
			// construct the first tiile
			OSMbuilder	builder;

			builder.addNode(layer, node1key, node1 );

			builder.addNode(layer, node2key, node2 );

			builder.addLink(link3key, link3, node1key, node2key );

			builder.addArea( area4key, layer, Area() );					// add an empty area
			builder.addPlace( place5key, layer, OsmPlace() );			// add an empty place

			writeToBinaryFile(
				STRING(tmpName1),
				builder, OSM_MAGIC2, VERSION_MAGIC, owmOverwrite 
			);
		}

		{
			// construct the second tiile
			OSMbuilder	builder;

			builder.addNode(layer, node6key, node6 );

			builder.addNode(layer, node7key, node7 );

			builder.addLink(link8key, link8, node6key, node7key );
			builder.addLink(link9key, link9, node7key, node2key );

			builder.addArea( area10key, layer, Area() );					// add an empty area
			builder.addPlace( place11key, layer, OsmPlace() );				// add an empty place

			writeToBinaryFile(
				STRING(tmpName2),
				builder, OSM_MAGIC2, VERSION_MAGIC, owmOverwrite 
			);
		}

		{
			OSMbuilder	builder;

			readFromBinaryFile(
				STRING(tmpName1),
				&builder, OSM_MAGIC2, VERSION_MAGIC, false 
			);

			OsmLink &link = builder.getLink(link3key);
			UT_ASSERT_EQUAL( link.length, link3.length );
			UT_ASSERT_EQUAL( link.type, link3.type );

			OsmNode &node = builder.getNode(node2key);
			UT_ASSERT_EQUAL( node.pos.latitude, node2.pos.latitude );
			UT_ASSERT_EQUAL( node.pos.longitude, node2.pos.longitude );

			// it is OK, if there is no exception
			builder.getArea( area4key );
			builder.getPlace( place5key );
		}


		{
			OSMviewer	osmViewer;

			readFromBinaryFile(
				STRING(tmpName1),
				&osmViewer, OSM_MAGIC2, VERSION_MAGIC, true 
			);

			{
				OsmLink &link = osmViewer.getLink(link3key);
				UT_ASSERT_EQUAL( link.length, link3.length );
				UT_ASSERT_EQUAL( link.type, link3.type );
			}

			{
				OsmNode &node = osmViewer.getNode(node2key);
				UT_ASSERT_EQUAL( node.pos.latitude, node2.pos.latitude );
				UT_ASSERT_EQUAL( node.pos.longitude, node2.pos.longitude );
			}

			// it is OK, if there is no exception
			osmViewer.getArea( area4key );
			osmViewer.getPlace( place5key );

			osmViewer.appendTile( STRING(tmpName2) );
			{
				OsmNode &node = osmViewer.getNode(node6key);
				UT_ASSERT_EQUAL( node.pos.latitude, node6.pos.latitude );
				UT_ASSERT_EQUAL( node.pos.longitude, node6.pos.longitude );
			}
			{
				OsmLink &link = osmViewer.getLink(link8key);
				UT_ASSERT_EQUAL( link.length, link8.length );
				UT_ASSERT_EQUAL( link.type, link8.type );
			}
			{
				OsmLink &link = osmViewer.getLink(link9key);
				UT_ASSERT_EQUAL( link.length, link9.length );
				UT_ASSERT_EQUAL( link.type, link9.type );
			}

			osmViewer.getPlace( place11key );
			osmViewer.getArea( area10key );
		}
	}
	void AndorraTest()
	{
		OSMviewer	openStreetMap;

		STRING fName = getTileFileName( osmPath, 190443 );
		readFromBinaryFile(
			fName,
			&openStreetMap, OSM_MAGIC2, VERSION_MAGIC, true 
		);
		openStreetMap.appendTile( 190442, osmPath );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static OsmTest myOsmTest;

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
