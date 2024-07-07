/*
		Project:		GAKLIB
		Module:			osm.cpp
		Description:	Open Street Map Importer
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

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

#include <gak/osm.h>
#include <gak/xmlParser.h>
#include <gak/types.h>
#include <gak/iostream.h>
#include <gak/stopWatch.h>
#include <gak/directory.h>

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

using namespace gak;
using namespace xml;
using namespace math;

#define DEBUG_MODE 0

#if DEBUG_MODE
	static const OsmKeyT relationID = 2027269;
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const bool STREETS_ONLY = true;
static const bool ALLOW_AREAS = false;

// element tags
static const STRING NODE = "node";
static const STRING WAY = "way";
static const STRING RELATION = "relation";
static const STRING ND = "nd";
static const STRING TAG = "tag";
static const STRING MEMBER = "member";

// attribute names
static const STRING ID = "id";
static const STRING K = "k";
static const STRING V = "v";
static const STRING TYPE = "type";
static const STRING ROLE = "role";
static const STRING REF = "ref";


// attribute values
static const STRING HIGHWAY = "highway";
static const STRING MOTORWAY = "motorway";
static const STRING MOTORWAY_LINK = "motorway_link";
static const STRING TRUNK = "trunk";
static const STRING TRUNK_LINK = "trunk_link";
static const STRING PRIMARY = "primary";
static const STRING PRIMARY_LINK = "primary_link";
static const STRING SECONDARY = "secondary";
static const STRING SECONDARY_LINK = "secondary_link";
static const STRING TERTIARY = "tertiary";
static const STRING TERTIARY_LINK = "tertiary_link";
static const STRING UNCLASSIFIED = "unclassified";
static const STRING RESIDENTIAL = "residential";
static const STRING SERVICE = "service";
static const STRING LIVING_STREET = "living_street";
static const STRING PEDESTRIAN = "pedestrian";
static const STRING TRACK = "track";
static const STRING BUS_GUIDEWAY = "bus_guideway";
static const STRING WT_ESCAPE = "escape";
static const STRING RACEWAY = "raceway";
static const STRING ROAD = "road";
static const STRING FOOTWAY = "footway";
static const STRING CYCLEWAY = "cycleway";
static const STRING BRIDLEWAY = "bridleway";
static const STRING STEPS = "steps";
static const STRING PATH = "path";

static const STRING WATERWAY = "waterway";
static const STRING RIVER = "river";
static const STRING RIVERBANK = "riverbank";
static const STRING CANAL = "canal";
static const STRING STREAM = "stream";

static const STRING NATURAL = "natural";
static const STRING WATER = "water";
static const STRING BAY = "bay";
static const STRING WETLAND = "wetland";
static const STRING WOOD = "wood";
static const STRING SCRUB = "scrub";
static const STRING GRASSLAND = "grassland";

static const STRING RAILWAY = "railway";
static const STRING RAIL = "rail";
static const STRING SUBWAY = "subway";
static const STRING TRAM = "tram";
static const STRING MAIN = "main";
static const STRING BRANCH = "branch";
static const STRING USAGE = "usage";

static const STRING LANDUSE = "landuse";
static const STRING FOREST = "forest";
static const STRING FARMLAND = "farmland";
static const STRING FARMYARD = "farmyard";
static const STRING MEADOW = "meadow";
static const STRING GRASS = "grass";

static const STRING ONEWAY = "oneway";
static const STRING YES = "yes";
static const STRING MINUS1 = "-1";

static const STRING OUTER = "outer";
static const STRING LAKE = "lake";

static const STRING PLACE = "place";
static const STRING NAME = "name";

static const STRING CONTINENT = "continent";
static const STRING COUNTRY = "country";
static const STRING STATE = "state";
static const STRING REGION = "region";
static const STRING DISTRICT = "district";
static const STRING COUNTY = "county";
static const STRING MUNICIPALITY = "municipality";

static const STRING CITY = "city";
static const STRING TOWN = "town";
static const STRING VILLAGE = "village";
static const STRING HAMLET = "hamlet";
static const STRING ALLOTMENTS = "allotments";
static const STRING ISOLATED_DWELING = "isolated_dweling";
static const STRING FARM = "farm";

static const STRING BOROUGH = "borough";
static const STRING QUARTER = "quarter";
static const STRING SUBURB = "suburb";
static const STRING NEIGHBOURHOOD = "neighbourhood";
static const STRING CITY_BLOCK = "city_block";
static const STRING PLOT = "plot";

static const STRING ISLAND = "island";
static const STRING ISLET = "islet";
static const STRING SQUARE = "square";
static const STRING LOCALITY = "locality";

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct XmlProcessor : public XmlNullProcessor
{
	struct ProcessorNode : public OsmNode
	{
		OsmLink::Type	maxWayType;

		ProcessorNode()
		{
			maxWayType = OsmLink::Unkown;
		}
	};

	struct ProcessorPlace : public OsmPlace
	{
		Type		m_type;
	};

	typedef Array<OsmNodeKeyT>					WayPoints;

	enum OsmDirection
	{
		odBOTH, odFROM_START, odFROM_END
	};

	struct ProcessorWay : public OsmLink
	{
		OsmWayKeyT		m_newWayID;
		OsmKeyT			m_relationID;
		WayPoints		m_wayPoints;
		OsmDirection	m_direction;
	};


	typedef PairMap<STRING, std::size_t>		ValueCounter;
	typedef Array<OsmWayKeyT>					OuterWays;
	typedef TreeMap<OsmWayKeyT, ProcessorWay>	Ways;
	typedef TreeMap<OsmNodeKeyT, ProcessorNode>	Nodes;

	enum OsmElement
	{
		oeUNKOWN, oeNODE, oeWAY, oeRELATION
	};
	ValueCounter	m_tagCounter,
					m_attributeCounter,
					m_wayTypeCounter,
					m_waterWayCounter[3],
					m_waterCounter[3],
					m_naturalCounter[3],
					m_railTypeCounter,
					m_railUsageCounter,
					m_railServiceCounter,
					m_placeCounter,
					m_landuseCounter[3];

	OSMbuilder		m_map;

	Nodes			m_allNodes;
	Ways			m_ways;

	OsmElement		m_osmElement;

	OsmLink::Type	m_highway,
					m_waterway,
					m_water,
					m_natural,
					m_railway,
					m_landuse;

	// reading a place
	ProcessorPlace	m_newPlace;

	// reading a way
	OsmLinkKeyT		m_linkID;
	ProcessorWay	m_newWay;

	// for railways
	STRING			m_usage, m_service;

	// reading a relation
	OsmKeyT			m_relationID;
	OuterWays		m_outerWays;

	XmlProcessor()
	{
		m_osmElement = oeUNKOWN;
		m_linkID = 0;
	}

	static void showCounter( std::size_t count, const STRING &value )
	{
		static	StopWatch	watch( true );
		static	StopWatch	total( true );

		if( watch.getMillis() > 5000 )
		{
			std::cout	<< value.padCopy( 16, STR_P_LEFT ) << ": " 
						<< formatNumber( count, 0, 0, ',' ) << ' '
						<< total.get<Minutes<> >().toString() << "     \r" 
						<< std::flush;
			watch.start();
		}
	}
	static void countValue( ValueCounter &counter, const STRING &value )
	{
		std::size_t	&count = counter[value];
		count += 1;
		showCounter( count, value );
	}
	static void printCounter( std::ostream &out, const STRING name, const ValueCounter &counter )
	{
		out << name.padCopy( 32, STR_P_LEFT ) << "----------------------------------------\n";
		for(
			XmlProcessor::ValueCounter::const_iterator it = counter.cbegin(),
				endIT = counter.cend();
			it != endIT;
			++it
		)
		{
			out << it->getKey().padCopy( 32, STR_P_LEFT ) << ": " << it->getValue() << '\n';
		}
	}


	void initItem()
	{
		m_highway = m_waterway = m_water = m_natural = m_railway = m_landuse = m_newWay.type = OsmLink::Unkown;

		m_newPlace.m_type = OsmPlace::Unkown;
		m_newPlace.name = NULL_STRING;

		m_newWay.m_wayPoints.clear();
		m_newWay.m_direction = odBOTH;
		m_newWay.m_relationID = 0;

		m_usage = NULL_STRING;
		m_service = NULL_STRING;

		m_outerWays.clear();
	}
	void processTag( const STRING &tag )
	{
		if( tag == NODE )
		{
			m_osmElement = oeNODE;
			initItem();
		}
		else if( tag == WAY )
		{
			m_osmElement = oeWAY;
			initItem();
		}
		else if( tag == RELATION )
		{
			m_osmElement = oeRELATION;
			initItem();
		}

		countValue( m_tagCounter, tag );
	}
	void processAttribute( 
		const STRING &tag, const STRING &attribute, const STRING &value 
	)
	{
		countValue( m_attributeCounter, attribute );
	}

	void processNodeTagAttributes(
		const PairMap<STRING,STRING> &attributes 
	)
	{
		if( attributes[K] == PLACE )
		{
			STRING	placeType = attributes[V];
			countValue( m_placeCounter, placeType );

			if( placeType == CONTINENT )
			{
				m_newPlace.m_type = OsmPlace::continent;
			}
			else if( placeType == COUNTRY )
			{
				m_newPlace.m_type = OsmPlace::country;
			}
			else if( placeType == STATE )
			{
				m_newPlace.m_type = OsmPlace::state;
			}
			else if( placeType == REGION )
			{
				m_newPlace.m_type = OsmPlace::region;
			}
			else if( placeType == "province" )
			{
				m_newPlace.m_type = OsmPlace::province;
			}
			else if( placeType == DISTRICT )
			{
				m_newPlace.m_type = OsmPlace::district;
			}
			else if( placeType == COUNTY )
			{
				m_newPlace.m_type = OsmPlace::county;
			}
			else if( placeType == MUNICIPALITY )
			{
				m_newPlace.m_type = OsmPlace::municipality;
			}

			else if( placeType == CITY )
			{
				m_newPlace.m_type = OsmPlace::city;
			}
			else if( placeType == TOWN )
			{
				m_newPlace.m_type = OsmPlace::town;
			}
			else if( placeType == VILLAGE )
			{
				m_newPlace.m_type = OsmPlace::village;
			}
			else if( placeType == HAMLET )
			{
				m_newPlace.m_type = OsmPlace::hamlet;
			}
			else if( placeType == ALLOTMENTS )
			{
				m_newPlace.m_type = OsmPlace::allotments;
			}
			else if( placeType == ISOLATED_DWELING )
			{
				m_newPlace.m_type = OsmPlace::isolated_dweling;
			}
			else if( placeType == FARM )
			{
				m_newPlace.m_type = OsmPlace::farm;
			}

			else if( placeType == BOROUGH )
			{
				m_newPlace.m_type = OsmPlace::borough;
			}
			else if( placeType == QUARTER )
			{
				m_newPlace.m_type = OsmPlace::quarter;
			}
			else if( placeType == SUBURB )
			{
				m_newPlace.m_type = OsmPlace::suburb;
			}
			else if( placeType == NEIGHBOURHOOD )
			{
				m_newPlace.m_type = OsmPlace::neighbourhood;
			}
			else if( placeType == CITY_BLOCK )
			{
				m_newPlace.m_type = OsmPlace::city_block;
			}
			else if( placeType == PLOT )
			{
				m_newPlace.m_type = OsmPlace::plot;
			}

			else if( placeType == ISLAND )
			{
				m_newPlace.m_type = OsmPlace::island;
			}
			else if( placeType == ISLET )
			{
				m_newPlace.m_type = OsmPlace::islet;
			}
			else if( placeType == SQUARE )
			{
				m_newPlace.m_type = OsmPlace::square;
			}
			else if( placeType == LOCALITY )
			{
				m_newPlace.m_type = OsmPlace::locality;
			}

		}
		else if( attributes[K] == NAME )
		{
			m_newPlace.name = attributes[V];
			if( m_newPlace.name.getCharSet() == STR_UTF8 )
			{
				m_newPlace.name = m_newPlace.name.decodeUTF8();
			}
		}
	}

	static OsmLink::Type parseHighway( STRING wayType )
	{
		if( wayType == MOTORWAY || wayType == MOTORWAY_LINK )
		{
			return OsmLink::motorway;
		}
		else if( wayType == TRUNK || wayType == TRUNK_LINK )
		{
			return OsmLink::trunk;
		}
		else if( wayType == PRIMARY || wayType == PRIMARY_LINK )
		{
			return OsmLink::primary;
		}
		else if( wayType == SECONDARY || wayType == SECONDARY_LINK )
		{
			return OsmLink::secondary;
		}
		else if( wayType == TERTIARY || wayType == TERTIARY_LINK )
		{
			return OsmLink::tertiary;
		}
		else if( wayType == UNCLASSIFIED )
		{
			return OsmLink::unclassified;
		}
		else if( wayType == RESIDENTIAL )
		{
			return OsmLink::residential;
		}
		else if( wayType == SERVICE )
		{
			return OsmLink::service;
		}
		else if( wayType == LIVING_STREET )
		{
			return OsmLink::living_street;
		}
		else if( wayType == PEDESTRIAN )
		{
			return OsmLink::pedestrian;
		}
		else if( wayType == TRACK )
		{
			return OsmLink::track;
		}
		else if( wayType == BUS_GUIDEWAY )
		{
			return OsmLink::bus_guideway;
		}
		else if( wayType == WT_ESCAPE )
		{
			return OsmLink::escape;
		}
		else if( wayType == RACEWAY )
		{
			return OsmLink::raceway;
		}
		else if( wayType == ROAD )
		{
			return OsmLink::road;
		}
		else if( wayType == FOOTWAY )
		{
			return OsmLink::footway;
		}
		else if( wayType == CYCLEWAY )
		{
			return OsmLink::cycleway;
		}
		else if( wayType == BRIDLEWAY )
		{
			return OsmLink::bridleway;
		}
		else if( wayType == STEPS )
		{
			return OsmLink::steps;
		}
		else if( wayType == PATH )
		{
			return OsmLink::path;
		}

		return OsmLink::Unkown;
	}

	static OsmLink::Type parseWaterway( STRING waterType )
	{
		if( waterType == RIVER )
		{
			return OsmLink::river;
		}
		else if( waterType == RIVERBANK )
		{
			return OsmLink::riverbank;
		}
		else if( waterType == CANAL )
		{
			return OsmLink::canal;
		}
		else if( waterType == STREAM )
		{
			return OsmLink::stream;
		}

		return OsmLink::Unkown;
	}

	static OsmLink::Type parseWater( STRING waterType )
	{
		if( waterType == LAKE )
		{
			return OsmLink::lake;
		}

		return OsmLink::Unkown;
	}

	static OsmLink::Type parseNatural( STRING naturalType )
	{
		if( naturalType == WATER )
		{
			return OsmLink::water;
		}
		else if( naturalType == BAY )
		{
			return OsmLink::bay;
		}
		else if( naturalType == WETLAND )
		{
			return OsmLink::wetland;
		}
		else if( naturalType == WOOD )
		{
			return OsmLink::wood;
		}
		else if( naturalType == SCRUB )
		{
			return OsmLink::scrub;
		}
		else if( naturalType == GRASS )
		{
			return OsmLink::grass;
		}
		else if( naturalType == GRASSLAND )
		{
			return OsmLink::grassland;
		}

		return OsmLink::Unkown;
	}
	static OsmLink::Type parseLanduse( STRING usageType )
	{
		if( usageType == FOREST )
		{
			return OsmLink::forest;
		}
		else if( usageType == FARMLAND )
		{
			return OsmLink::farmland;
		}
		else if( usageType == FARMYARD )
		{
			return OsmLink::farmyard;
		}
		else if( usageType == MEADOW )
		{
			return OsmLink::meadow;
		}
		else if( usageType == GRASS )
		{
			return OsmLink::grass;
		}

		return OsmLink::Unkown;
	}

	static OsmLink::Type parseRailway( STRING railwayType )
	{
		if( railwayType == RAIL )
		{
			return OsmLink::secondRailway;
		}
		else if( railwayType == SUBWAY )
		{
			return OsmLink::subway;
		}
		else if( railwayType == TRAM )
		{
			return OsmLink::tramway;
		}

		return OsmLink::Unkown;
	}
	void processTagAttributes(
		OsmElement element, const PairMap<STRING,STRING> &attributes 
	)
	{
		if( attributes[K] == HIGHWAY )
		{
			STRING	wayType = attributes[V];
			countValue( m_wayTypeCounter, wayType );
			m_highway = parseHighway( wayType );
		}
		else if( attributes[V] == ONEWAY )
		{
			STRING direction = attributes[V];
			if( direction == YES )
			{
				m_newWay.m_direction = odFROM_START;
			}
			else if( direction == MINUS1 )
			{
				m_newWay.m_direction = odFROM_END;
			}
		}
		else if( attributes[K] == RAILWAY )
		{
			STRING	railwayType = attributes[V];
			countValue( m_railTypeCounter, railwayType );
			m_railway = parseRailway( railwayType );
		}
		else if( attributes[K] == USAGE )
		{
			m_usage = attributes[V];
			countValue( m_railUsageCounter, m_usage );
		}
		else if( attributes[K] == SERVICE )
		{
			m_service = attributes[V];
			countValue( m_railServiceCounter, m_service );
		}
		else if( attributes[K] == WATERWAY )
		{
			STRING	waterType = attributes[V];
			countValue( m_waterWayCounter[element-1], waterType );
			m_waterway = parseWaterway( waterType );
		}
		else if( attributes[K] == WATER )
		{
			STRING	waterType = attributes[V];
			countValue( m_waterCounter[element-1], waterType );
			m_water = parseWater( waterType );
		}
		else if( attributes[K] == NATURAL )
		{
			STRING	naturalType = attributes[V];
			countValue( m_naturalCounter[element-1], naturalType );
			m_natural = parseNatural( naturalType );
		}
		else if( attributes[K] == LANDUSE )
		{
			STRING landuse = attributes[V];
			countValue( m_landuseCounter[element-1], landuse );
			m_landuse = parseLanduse( landuse );
		}
	}
	void processAttributes(
		const STRING &tag, const PairMap<STRING,STRING> &attributes 
	)
	{
		if( m_osmElement == oeNODE )
		{
			if( tag == NODE )
			{
				ProcessorNode	newNode;
				OsmNodeKeyT		id = attributes["id"].getValueE<OsmNodeKeyT>();

				m_newPlace.pos.longitude = newNode.pos.longitude = attributes["lon"].getValueE<float>();
				m_newPlace.pos.latitude = newNode.pos.latitude = attributes["lat"].getValueE<float>();

				m_allNodes[id] = newNode;
			}
			else if( tag == TAG )
			{
				processNodeTagAttributes( attributes );
			}
		}
		else if( m_osmElement == oeWAY )
		{
			if( tag == WAY )
			{
				m_newWay.m_newWayID = attributes[ID].getValueE<OsmWayKeyT>();
			}
			else if( tag == ND )
			{
				OsmNodeKeyT ref = attributes.cbegin()->getValue().getValueE<OsmNodeKeyT>();
				m_newWay.m_wayPoints.addElement( ref );
			}
			else if( tag == TAG )
			{
				processTagAttributes( m_osmElement, attributes );
			}
		}
		else if( m_osmElement == oeRELATION )
		{
			if( tag == RELATION )
			{
				m_relationID = attributes[ID].getValueE<OsmKeyT>();
#if DEBUG_MODE
				if( m_relationID == relationID )
				{
					std::cout << "\nprocessAttributes: got relation\n";
				}
#endif
			}
			else if( tag == MEMBER )
			{
				if( attributes[TYPE] == WAY && attributes[ROLE] == OUTER )
				{
					m_outerWays.addElement( attributes[REF].getValueE<OsmWayKeyT>() );
				}
			}
			else if( tag == TAG )
			{
				processTagAttributes( m_osmElement, attributes );
			}
		}
	}

	short getPriority( 
		OsmLink::Type minWayType, 
		OsmLink::Type maxWayType, 
		OsmLink::Type wayType 
	)
	{
		return (maxWayType - wayType+1)*1000 / (maxWayType - minWayType-1);
	}
	short getPriority( OsmLink::Type wayType )
	{
		if( wayType >= OsmLink::minWayType && wayType <= OsmLink::maxWayType )
		{
			return getPriority(OsmLink::minWayType, OsmLink::maxWayType, wayType );
		}
		else if( wayType >= OsmLink::minWaterType && wayType <= OsmLink::maxWaterType )
		{
			return getPriority(OsmLink::minWaterType, OsmLink::maxWaterType, wayType );
		}
		else if( wayType >= OsmLink::minRailType && wayType <= OsmLink::maxRailType )
		{
			return getPriority(OsmLink::minRailType, OsmLink::maxRailType, wayType );
		}

		return 0;
	}
	const ProcessorNode &addWayPoint( OsmNodeKeyT nodeID, OsmLink::Type wayType )
	{
		ProcessorNode &node = m_allNodes[nodeID];
		if( !m_map.hasNode( nodeID ) )
		{
			node.maxWayType = wayType;
			m_map.addNode( node.maxWayType, nodeID, node );
		}
		else if( wayType != node.maxWayType && getPriority(wayType) > getPriority(node.maxWayType) )
		{
			m_map.moveNode( node.maxWayType, wayType, nodeID );
			node.maxWayType = wayType;
		}

		return node;
	}

	void addWayPoints( ProcessorWay &way )
	{
		WayPoints::const_iterator	it = way.m_wayPoints.cbegin(), 
									endIT = way.m_wayPoints.cend();
		OsmNodeKeyT					startNodeID = *it;
		OsmNode						startNode = addWayPoint( startNodeID, way.type );;
		++it;
		do
		{
			OsmNodeKeyT	endNodeID = *it;
			OsmNode		endNode = addWayPoint( endNodeID, way.type );

			float distance = float(getDistance( startNode.getPosition(), endNode.getPosition() ));
			way.length = distance;

			++m_linkID;
			if( way.m_direction == odBOTH )
			{
				m_map.addLink(
					m_linkID,
					way,
					startNodeID, endNodeID
				);
				m_map.addLink(
					-m_linkID,
					way,
					endNodeID, startNodeID
				);
			}
			else if( way.m_direction == odFROM_START )
			{
				m_map.addLink(
					m_linkID,
					way,
					startNodeID, endNodeID
				);
			}
			else if( way.m_direction == odFROM_END )
			{
				m_map.addLink(
					m_linkID,
					way,
					endNodeID, startNodeID
				);
			}
			startNodeID = endNodeID;
			startNode = endNode;

			{
				std::size_t	count = m_map.getNumLinks();
				showCounter( count, "map.getNumLinks" );
			}

			++it;
		} while( it != endIT );
	}

	void processEndNode()
	{
		if( m_newPlace.m_type != OsmPlace::Unkown && !m_newPlace.name.isEmpty() )
		{
			m_map.addPlace( m_map.getNumPlaces()+1, m_newPlace.m_type, m_newPlace );
		}
	}
	void processEndWay()
	{
		if( m_newWay.m_wayPoints.size() >= 2 )
		{
			if( m_highway != OsmLink::Unkown )
			{
				m_newWay.type = m_highway;
			}
			else if( m_railway != OsmLink::Unkown )
			{
				m_newWay.type = m_railway;
				if( m_railway == OsmLink::secondRailway )
				{
					if( m_usage == MAIN )
					{
						m_newWay.type = OsmLink::mainRailway;
					}
					else if( m_usage == BRANCH )
					{
						m_newWay.type = OsmLink::branchRailway;
					}
				}
			}
			else if( m_waterway != OsmLink::Unkown )
			{
				m_newWay.type = m_waterway;
			}
			else if( m_natural != OsmLink::Unkown )
			{
				m_newWay.type = m_natural;
			}
			else if( m_landuse != OsmLink::Unkown )
			{
				m_newWay.type = m_landuse;
			}

			if( m_newWay.type > OsmLink::minWayType && m_newWay.type < OsmLink::maxRailType )
			{
				if ( !STREETS_ONLY || m_newWay.type <= OsmLink::living_street )
				{
					addWayPoints( m_newWay );
				}
			}
			else if( m_newWay.type > OsmLink::minWaterType && m_newWay.type < OsmLink::maxAreaType )
			{
				if (ALLOW_AREAS)
				{
					if( *m_newWay.m_wayPoints.cbegin() == *m_newWay.m_wayPoints.rbegin() )
					{
						// this is a polygon =>
						addArea( m_newWay.type, m_newWay.m_newWayID, m_newWay.m_wayPoints );
					}
					else
					{
						// store this way for later usage (relation)
						m_ways[m_newWay.m_newWayID] = m_newWay;
					}
				}
			}
			else
			{
				// store this way for later usage (relation)
				m_ways[m_newWay.m_newWayID] = m_newWay;
			}
		}
	}
	void addArea( OsmLink::Type	areaType, OsmAreaKeyT areaID, const WayPoints &areaNodes )
	{
		while( m_map.hasArea( areaID ) )
		{
			std::cerr << "\narea " << areaID << " exists.\n";
			areaID++;
		}

		Area	area;
		area.points.setCapacity( areaNodes.size(), true );

		for( 
			WayPoints::const_iterator it = areaNodes.cbegin(), endIT = areaNodes.cend();
			it != endIT;
			++it
		)
		{
			const OsmNode &node = m_allNodes[*it];
			area.points.addElement( node.getPosition() );
		}

		m_map.addArea( areaID, areaType, area );
		{
			std::size_t	count = m_map.getNumAreas();
			showCounter( count, "map.getNumAreas" );
		}
	}

	void processEndRelation()
	{
		if( m_outerWays.size() > 0 )
		{
#if DEBUG_MODE
			if( m_relationID == relationID )
			{
				std::cout << "\nprocessEndRelation: got relation\n";
			}
#endif
			OsmLink::Type	areaType;
			if( m_natural != OsmLink::Unkown )
			{
				areaType = m_natural;
			}
			else if( m_waterway != OsmLink::Unkown )
			{
				areaType = m_waterway;
			}
			else if( m_landuse != OsmLink::Unkown )
			{
				areaType = m_landuse;
			}
			else
			{
#if DEBUG_MODE
				if( m_relationID == relationID )
				{
					std::cout << "\nprocessEndRelation: bad relation\n";
				}
#endif
/*@*/			return;	// unkown relation
			}

			while( m_outerWays.size() )
			{
				OsmAreaKeyT	areaID = *m_outerWays.cbegin();
				WayPoints	area;
#if DEBUG_MODE
				if( m_relationID == relationID )
				{
					std::cout << "\nm_outerWays.size " << m_outerWays.size() << "\n";
				}
#endif
				while( area.size() == 0 || *area.cbegin() != *area.rbegin() )
				{
					bool	wayAdded = false;
					for( 
						OuterWays::iterator it1 = m_outerWays.begin(),
							endIT1 = m_outerWays.end();
						it1 != endIT1;
						++it1
					)
					{
						if( !m_ways.hasElement( *it1 ) )
						{
							if( !m_map.hasArea( *it1 ) )
							{
								std::cerr << "\nway not found " << *it1 << " for relation " << m_relationID << '\n';
							}
							m_outerWays.erase( it1 );
							wayAdded = true;
							break;
						}

						ProcessorWay	&way = m_ways[*it1];
						const WayPoints	&wayPoints = way.m_wayPoints;
						way.m_relationID = m_relationID;

						if( !m_ways.size() )
						{
							std::cerr << "\nempty way " << *it1 << " for relation " << m_relationID << '\n';
							m_outerWays.erase( it1 );
							wayAdded = true;
							break;
						}


						if( area.size() == 0 || *area.rbegin() == *wayPoints.cbegin() )
						{
#if DEBUG_MODE
							if( m_relationID == relationID )
							{
								std::cout << "\nadd way " << *it1 << " to area\n";
							}
#endif
							area.addElements( wayPoints );
							m_outerWays.erase( it1 );
							wayAdded = true;
							break;
						}
						if( area.size() == 0 || *area.rbegin() == *wayPoints.crbegin() )
						{
#if DEBUG_MODE
							if( m_relationID == relationID )
							{
								std::cout << "\nadd reverted way " << *it1 << " to area\n";
							}
#endif
							for( 
								WayPoints::const_reverse_iterator it2 = wayPoints.crbegin(),
									endIT2 = wayPoints.crend();
								it2 != endIT2;
								++it2
							)
							{
								area.addElement( *it2 );
							}
							m_outerWays.erase( it1 );
							wayAdded = true;
							break;
						}
					}
					if( !wayAdded )
					{
						break;
					}
				}
				if( area.size() )
				{
#if DEBUG_MODE
					if( m_relationID == relationID )
					{
						std::cout << "\naddArea: got relation id: " << areaID << " type: " << areaType << " count: " << area.size() << '\n';
					}
#endif
					if (ALLOW_AREAS)
					{
						addArea( areaType, areaID, area );
					}
				}
				else
				{
					if( !m_map.hasArea( areaID ) )
					{
						std::cerr << "\nempty area " << areaID << " for relation " << m_relationID << '\n';
					}
				}
			}
			m_outerWays.clear();
		}
	}
	void processEndTag( const STRING &tag )
	{
		if( tag.endsWith( NODE ) )
		{
			processEndNode();
		}
		else if( tag.endsWith( WAY ) )
		{
			processEndWay();
		}
		else if( tag.endsWith( RELATION ) )
		{
			processEndRelation();
		}
	}

	void postProcess()
	{
		for(
			Ways::iterator it = m_ways.begin(), endIT = m_ways.end();
			it != endIT;
			++it
		)
		{
			if( it->getValue().m_relationID == 0 )
			{
				if( it->getValue().type > OsmLink::minAreaType && it->getValue().type < OsmLink::maxAreaType )
				{
					addArea( it->getValue().type, it->getKey(), it->getValue().m_wayPoints );
				}
				else if( it->getValue().type == OsmLink::lake || it->getValue().type == OsmLink::riverbank || it->getValue().type == OsmLink::wetland )
				{
					addArea( it->getValue().type, it->getKey(), it->getValue().m_wayPoints );
				}
				else if( it->getValue().type <= OsmLink::living_street || (it->getValue().type != OsmLink::Unkown && !STREETS_ONLY) )
				{
					addWayPoints( it->getValue() );
				}
			}
		}
	}
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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

int main( void )
{
//	STRING	osmName = "C:\\Cache\\OSM\\andorra-latest.osm";
	STRING	osmName = "C:\\Cache\\OSM\\austria-latest.osm";
//	STRING	osmName = "C:\\Cache\\OSM\\oberbayern-latest.osm";
//	STRING	osmName = "C:\\Cache\\OSM\\unterfranken-latest.osm";

	STRING	resultFile = osmName + "bin";
	STRING	statFile = osmName + ".txt";

	gak::xml::Parser	myParser( osmName );
	XmlProcessor		myProcessor;

	std::cout	<< sizeof( OSMviewer::link_container_type::value_type ) << ' ' 
				<< sizeof( OSMbuilder::link_container_type::value_type ) << ' ' 
				<< sizeof( OsmLink ) << ' ' 
				<< sizeof( OsmLink::Type ) << ' ' 
				<< sizeof( OsmNode ) << std::endl;
	if( exists( resultFile ) )
	{
//		readFromFile( resultFile, &myProcessor.m_map, OSM_MAGIC );
	}

	myProcessor.m_map.clear();

	myParser.parseXML( myProcessor );
	myProcessor.postProcess();

	std::cout << std::endl;

	std::ofstream out( statFile );
	XmlProcessor::printCounter( out, "tags", myProcessor.m_tagCounter );
	XmlProcessor::printCounter( out, "attributesNames", myProcessor.m_attributeCounter );
	XmlProcessor::printCounter( out, HIGHWAY, myProcessor.m_wayTypeCounter );
	XmlProcessor::printCounter( out, WATERWAY + "-way", myProcessor.m_waterWayCounter[XmlProcessor::oeWAY-1] );
	XmlProcessor::printCounter( out, WATERWAY + "-relation", myProcessor.m_waterWayCounter[XmlProcessor::oeRELATION-1] );
	XmlProcessor::printCounter( out, WATER + "-way", myProcessor.m_waterCounter[XmlProcessor::oeWAY-1] );
	XmlProcessor::printCounter( out, WATER + "-relation", myProcessor.m_waterCounter[XmlProcessor::oeRELATION-1] );
	XmlProcessor::printCounter( out, NATURAL + "-way", myProcessor.m_naturalCounter[XmlProcessor::oeWAY-1] );
	XmlProcessor::printCounter( out, NATURAL + "-relation", myProcessor.m_naturalCounter[XmlProcessor::oeRELATION-1] );
	XmlProcessor::printCounter( out, RAILWAY, myProcessor.m_railTypeCounter );
	XmlProcessor::printCounter( out, SERVICE, myProcessor.m_railServiceCounter );
	XmlProcessor::printCounter( out, USAGE, myProcessor.m_railUsageCounter );
	XmlProcessor::printCounter( out, PLACE, myProcessor.m_placeCounter );
	XmlProcessor::printCounter( out, LANDUSE + "-way", myProcessor.m_landuseCounter[XmlProcessor::oeWAY-1] );
	XmlProcessor::printCounter( out, LANDUSE + "-relation", myProcessor.m_landuseCounter[XmlProcessor::oeRELATION-1] );

	writeToFile( resultFile, myProcessor.m_map, OSM_MAGIC );
	std::cout	<< "Nodes: "  << formatNumber( myProcessor.m_map.getNumNodes(),  0, 0, '.' ) 
				<< " Places: " << formatNumber( myProcessor.m_map.getNumPlaces(), 0, 0, '.' )
				<< " Links: "  << formatNumber( myProcessor.m_map.getNumLinks(),  0, 0, '.' ) 
				<< " Areas: "  << formatNumber( myProcessor.m_map.getNumAreas(),  0, 0, '.' ) 
				<< std::endl;
	return 0;
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
