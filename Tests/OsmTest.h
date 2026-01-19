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
	static Area getDefaultArea()
	{
		Area	def;

		def.points.addElement(OsmPosition(45,15));
		def.points.addElement(OsmPosition(45.1,15.1));
		def.points.addElement(OsmPosition(45.0,15.1));

		return def;
	}
	static OsmPlace getDefaultPlace()
	{
		OsmPlace	def;

		def.pos.latitude = 40;
		def.pos.longitude = 11;

		return def;
	}
	void SyntheticTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "OsmTest::SyntheticTest");

		const OSMbuilder::layer_key_type	layer1 = 111;
		const OSMbuilder::layer_key_type	layer2 = 333;

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

			builder.addNode(layer1, node1key, node1 );

			builder.addNode(layer1, node2key, node2 );

			builder.addLink(link3key, link3, node1key, node2key );

			builder.addArea( area4key, layer1, getDefaultArea() );
			builder.addPlace( place5key, layer1, getDefaultPlace() );

			{
				Array<OSMviewer::node_key_type>	nodes;
				builder.getRegion( layer1, builder.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
			}

			writeToBinaryFile(
				STRING(tmpName1),
				builder, OSM_MAGIC2, VERSION_MAGIC, owmOverwrite 
			);
		}

		{
			// construct the second tiile
			OSMbuilder	builder;

			builder.addNode(layer2, node6key, node6 );

			builder.addNode(layer2, node7key, node7 );

			builder.addLink(link8key, link8, node6key, node7key );
			builder.addLink(link9key, link9, node7key, node2key );

			builder.addArea( area10key, layer2, getDefaultArea() );
			builder.addPlace( place11key, layer2, getDefaultPlace() );

			{
				Array<OSMviewer::node_key_type>	nodes;
				builder.getRegion( layer2, builder.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
			}
			{
				Set<OsmAreaKeyT>	areas;
				Array<OsmPlaceKeyT>	places;

				builder.getAreas( layer2, builder.getFrameBox(0.01), &areas );
				UT_ASSERT_EQUAL( areas.size(), 1 );
				builder.getPlaces( layer2, builder.getFrameBox(0.01), &places );
				UT_ASSERT_EQUAL( places.size(), 1 );
			}

			writeToBinaryFile(
				STRING(tmpName2),
				builder, OSM_MAGIC2, VERSION_MAGIC, owmOverwrite 
			);
		}

		{
			OSMbuilder	builder1;

			readFromBinaryFile(
				STRING(tmpName1),
				&builder1, OSM_MAGIC2, VERSION_MAGIC, false 
			);

			{
				OsmLink &link = builder1.getLink(link3key);
				UT_ASSERT_EQUAL( link.length, link3.length );
				UT_ASSERT_EQUAL( link.type, link3.type );
			}

			OsmNode &node = builder1.getNode(node2key);
			UT_ASSERT_EQUAL( node.pos.latitude, node2.pos.latitude );
			UT_ASSERT_EQUAL( node.pos.longitude, node2.pos.longitude );

			// it is OK, if there is no exception
			builder1.getArea( area4key );
			builder1.getPlace( place5key );

			{
				Array<OSMviewer::node_key_type>	nodes;
				builder1.getRegion( layer1, builder1.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
			}

			OSMbuilder	builder2;
			readFromBinaryFile(
				STRING(tmpName2),
				&builder2, OSM_MAGIC2, VERSION_MAGIC, false 
			);

			builder1.mergeLayer( layer2, layer2, builder2 );
			{
				OsmLink &link = builder1.getLink(link9key);
				UT_ASSERT_EQUAL( link.length, link9.length );
				UT_ASSERT_EQUAL( link.type, link9.type );
			}
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

			{
				Array<OSMviewer::node_key_type>	nodes;
				osmViewer.getRegion( layer1, osmViewer.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
				nodes.clear();
				osmViewer.getRegion( layer2, osmViewer.getBoundingBox(), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 0 );
			}
			{
				Set<OsmAreaKeyT>	areas;
				Array<OsmPlaceKeyT>	places;

				// test layer 2 is not yet loaded
				osmViewer.getAreas( layer2, osmViewer.getFrameBox(0.01), &areas );
				UT_ASSERT_EQUAL( areas.size(), 0 );
				osmViewer.getPlaces( layer2, osmViewer.getFrameBox(0.01), &places );
				UT_ASSERT_EQUAL( places.size(), 0 );

				// test layer 1 is present
				osmViewer.getAreas( layer1, osmViewer.getFrameBox(0.01), &areas );
				UT_ASSERT_EQUAL( areas.size(), 1 );
				osmViewer.getPlaces( layer1, osmViewer.getFrameBox(0.01), &places );
				UT_ASSERT_EQUAL( places.size(), 1 );
			}

			{
				OSMviewer tmp;
				osmViewer.mergeOsmTile( STRING(tmpName2), &tmp );
			}

			{
				Array<OSMviewer::node_key_type>	nodes;
				osmViewer.getRegion( layer1, osmViewer.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
				nodes.clear();
				osmViewer.getRegion( layer2, osmViewer.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
			}


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

			{
				Array<OSMviewer::node_key_type>	nodes;
				osmViewer.getRegion( layer1, osmViewer.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
				nodes.clear();
				osmViewer.getRegion( layer2, osmViewer.getFrameBox(0.01), &nodes );
				UT_ASSERT_EQUAL( nodes.size(), 2 );
			}
			{
				Set<OsmAreaKeyT>	areas;
				Array<OsmPlaceKeyT>	places;

				osmViewer.getAreas( layer2, osmViewer.getFrameBox(0.01), &areas );
				UT_ASSERT_EQUAL( areas.size(), 1 );
				osmViewer.getPlaces( layer2, osmViewer.getFrameBox(0.01), &places );
				UT_ASSERT_EQUAL( places.size(), 1 );
			}
		}
	}
	void AndorraTest()
	{
		OSMbuilder	openStreetMap;

		STRING fName = getTileFileName( osmPath, 190442 );
		readFromBinaryFile(
			fName,
			&openStreetMap, OSM_MAGIC2, VERSION_MAGIC, true 
		);
		
		OSMviewer	layer;
		openStreetMap.mergeOsmLayer( OsmLink::minWayType, OsmLink::unclassified, 199104, osmPath, &layer );
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
