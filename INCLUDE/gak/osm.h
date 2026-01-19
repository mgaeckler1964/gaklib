/*
		Project:		GAKLIB
		Module:			osm.h
		Description:	Open Street Map Support
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
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

#ifndef GAK_OPEN_STREET_MAP_H
#define GAK_OPEN_STREET_MAP_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/geoGraph.h>
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

const uint32 OSM_MAGIC2 = 0x12873666;
const uint16 VERSION_MAGIC = 2;

#define BINMAP_EXTENSION ".bin"

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef int64	OsmKeyT;	// OSM uses 64 bit ids

typedef int64		OsmNodeKeyT;
typedef int64		OsmLinkKeyT;
typedef int32		OsmPlaceKeyT;
typedef OsmKeyT		OsmWayKeyT;
typedef OsmNodeKeyT	OsmAreaKeyT;

typedef int16		OsmLayerKeyT;

typedef math::GeoPosition<float>	OsmPosition;

typedef math::Polygon<OsmPosition>	Area;

struct OsmNode
{
	OsmPosition	pos;
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::binaryToBinaryStream( stream, pos );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::binaryFromBinaryStream( stream, &pos );
	}
	const math::GeoPosition<float> &getPosition() const
	{
		return pos;
	}
	math::tileid_t getTileID() const
	{
		return pos.getTileID();
	}
};

struct OsmPlace : public OsmNode
{
	enum Type
	{
		minRegion,
			continent,			// 0
			country,			// 0
			state,				// 1
			region,				// 5
			province,			// 0
			district,			// 0
			county,				// 9
			municipality,		// 1
		maxRegion,

		minTown,
			city,				// 2
			town,				// 103
			village,			// 2072
			hamlet,				// 8801
			allotments,			// 0
			isolated_dweling,	// 747
			farm,				// 729
		maxTown,

		minTownPart,
			borough,			// 0
			quarter,			// 17
			suburb,				// 297
			neighbourhood,		// 140
			city_block,			// 1
			plot,				// 0
		maxTownPart,

		minLocation,
			island,				// 2
			islet,				// 3
			square,				// 6
			locality,			// 2197
		maxLocation,

		Unkown,
	};
	//PlaceType	type;
	STRING		name;

	void toBinaryStream( std::ostream &stream ) const
	{
		OsmNode::toBinaryStream( stream );

		//int16 type = int16(this->type);
		//gak::binaryToBinaryStream( stream, type );

		gak::toBinaryStream( stream, name );
	}
	void fromBinaryStream( std::istream &stream )
	{
		OsmNode::fromBinaryStream( stream );

		//int16 type;
		//gak::binaryFromBinaryStream( stream, &type );
		//this->type = PlaceType( type );

		gak::fromBinaryStream( stream, &name );
	}
};

struct OsmLink
{
	enum Type { 
		minWayType,
			motorway, trunk, 
			primary, secondary, 
			tertiary, unclassified, 
			residential, service,
			living_street, pedestrian, track, bus_guideway, escape, raceway, road,
			footway, cycleway, bridleway, steps, path,
		maxWayType,
		minRailType,
			mainRailway,
			branchRailway,
			subway,
			tramway,
			secondRailway,
		maxRailType,
		minWaterType, 
			lake, anyWater,
			canal, riverbank, river, 
			bay, beach, 
			wetland,
			water, stream,
		maxWaterType,
		minAreaType,	// areas other than water
			wood,
			forest,
			scrub,
			farmland,
			farmyard,
			minGrassType,
				meadow,
				grass,
				grassland,
			maxGrassType,
		maxAreaType,

		Unkown,		// must be the last element
	};

	//OsmKeyT	osmID;
	float		length;
	Type		type;

	void toBinaryStream( std::ostream &stream ) const
	{
		//gak::binaryToBinaryStream( stream, osmID );
		gak::binaryToBinaryStream( stream, length );
		int16 type = int16(this->type);
		gak::binaryToBinaryStream( stream, type );
	}
	void fromBinaryStream( std::istream &stream )
	{
		//gak::binaryFromBinaryStream( stream, &osmID );
		gak::binaryFromBinaryStream( stream, &length );
		int16 type;
		gak::binaryFromBinaryStream( stream, &type );
		this->type = Type( type );
	}
};

template<template<typename, typename>class MapT, template<typename>class IndexT>
class BasicOpenStreetMap : public GeoGraph<OsmNode, OsmLink, MapT, IndexT, OsmLayerKeyT, OsmNodeKeyT, OsmLinkKeyT>
{
	typedef GeoGraph<OsmNode, OsmLink, MapT, IndexT, OsmLayerKeyT, OsmNodeKeyT, OsmLinkKeyT>	Super;
	typedef BasicOpenStreetMap<MapT,IndexT>														SelfT;

	public:

	typedef typename Super::node_container_type		node_container_type;
	typedef typename Super::link_container_type		link_container_type;
	typedef typename Super::link_key_types			link_key_types;
	typedef typename Super::link_key_type			link_key_type;
	typedef typename Super::node_key_type			node_key_type;
	typedef typename Super::LinkInfo				LinkInfo;
	typedef typename Super::layer_key_type			layer_key_type;
	typedef Super::Layers							Layers;

	/*
		------------------------------------------------------------------------------------------
			Places
		------------------------------------------------------------------------------------------
	*/
	public:
	typedef MapT<OsmPlaceKeyT, OsmPlace>			place_container_type;

	private:
	Layers					m_lonPlaceIndex;
	Layers					m_latPlaceIndex;
	place_container_type	m_places;

	void addPlace( OsmPlaceKeyT key, const OsmPlace &newPlace )
	{
		m_places[key] = newPlace;
		expandBoundingBox(newPlace.pos.longitude,newPlace.pos.latitude);
	}
	public:
	void addPlace( OsmPlaceKeyT key, OsmLayerKeyT layerKey, const OsmPlace &newPlace )
	{
		addPlace(key, newPlace);

		float	 longitude = newPlace.getPosition().longitude;
		float	 latitude = newPlace.getPosition().latitude;

		m_lonPlaceIndex[layerKey].addElement(
			PositionValue( longitude, key )
		);
		m_latPlaceIndex[layerKey].addElement(
			PositionValue( latitude, key )
		);
	}
	std::size_t getNumPlaces() const
	{
		return m_places.size();
	}
	const OsmPlace &getPlace( OsmPlaceKeyT id ) const
	{
		return m_places[id];
	}

	const Layers &getLonPlaceIndex() const
	{
		return m_lonPlaceIndex;
	}
	const Layers &getLatPlaceIndex() const
	{
		return m_latPlaceIndex;
	}

	const place_container_type &getAllPlaces() const
	{
		return m_places;
	}

	template <typename ScalarT>
	void getPlaces(
		const OsmLayerKeyT									&layerKey, 
		const math::Rectangle< math::GeoPosition<ScalarT> >	&region,
		Array<OsmPlaceKeyT>									*result
	) const;

	/*
		------------------------------------------------------------------------------------------
			Areas
		------------------------------------------------------------------------------------------
	*/
	public:
	typedef MapT<OsmAreaKeyT, Area>					area_container_type;

	private:
	Layers					m_lonAreaIndex;
	Layers					m_latAreaIndex;
	area_container_type		m_areas;

	void addArea( OsmAreaKeyT key, const Area &newArea );
	public:
	void addArea( OsmAreaKeyT key, OsmLayerKeyT layerKey, const Area &newArea );

	std::size_t getNumAreas() const
	{
		return m_areas.size();
	}
	const Area &getArea( OsmAreaKeyT id ) const
	{
		return m_areas[id];
	}

	bool hasArea( OsmAreaKeyT id ) const
	{
		return m_areas.hasElement( id );
	}

	const Layers &getLonAreaIndex() const
	{
		return m_lonAreaIndex;
	}
	const Layers &getLatAreaIndex() const
	{
		return m_latAreaIndex;
	}

	const area_container_type &getAllAreas() const
	{
		return m_areas;
	}
	template <typename ScalarT>
	void getAreas(
		const OsmLayerKeyT									&layerKey, 
		const math::Rectangle< math::GeoPosition<ScalarT> >	&region,
		Set<OsmAreaKeyT>									*result
	) const;

	/*
		------------------------------------------------------------------------------------------
			Common
		------------------------------------------------------------------------------------------
	*/
	void clear()
	{
		Super::clear();
		m_lonPlaceIndex.clear();
		m_latPlaceIndex.clear();
		m_places.clear();
		m_lonAreaIndex.clear();
		m_latAreaIndex.clear();
		m_areas.clear();
	}

	void toBinaryStream( std::ostream &stream ) const
	{
		Super::toBinaryStream( stream );

		gak::toBinaryStream( stream, m_lonPlaceIndex );
		gak::toBinaryStream( stream, m_latPlaceIndex );
		gak::toBinaryStream( stream, m_places );

		gak::toBinaryStream( stream, m_lonAreaIndex );
		gak::toBinaryStream( stream, m_latAreaIndex );
		gak::toBinaryStream( stream, m_areas );
	}
	void fromBinaryStream( std::istream &stream )
	{
		doEnterFunction("BasicOpenStreetMap::fromBinaryStream");
		Super::fromBinaryStream( stream );

		{
			doEnterFunction("m_lonPlaceIndex::fromBinaryStream");
			gak::fromBinaryStream( stream, &m_lonPlaceIndex );
		}
		{
			doEnterFunction("m_latPlaceIndex::fromBinaryStream");
			gak::fromBinaryStream( stream, &m_latPlaceIndex );
		}
		{
			doEnterFunction("m_places::fromBinaryStream");
			gak::fromBinaryStream( stream, &m_places );
		}
		{
			doEnterFunction("m_lonAreaIndex::fromBinaryStream");
			gak::fromBinaryStream( stream, &m_lonAreaIndex );
		}
		{
			doEnterFunction("m_latAreaIndex::fromBinaryStream");
			gak::fromBinaryStream( stream, &m_latAreaIndex );
		}
		{
			doEnterFunction("m_areas::fromBinaryStream");
			gak::fromBinaryStream( stream, &m_areas );
		}
	}

	template <class TileT>
	void mergeOsmTile( const STRING &mapFileName, TileT *tileMap )
	{
		readFromBinaryFile(
			mapFileName,
			tileMap, OSM_MAGIC2, VERSION_MAGIC, true 
		);
		mergeTile(*tileMap);
		const typename TileT::place_container_type &places = tileMap->getAllPlaces();
		for(
			typename TileT::place_container_type::const_iterator it = places.cbegin(), endIT = places.cend();
			it != endIT;
			++it
		)
		{
			addPlace(it->getKey(), it->getValue() );
		}
		const typename TileT::area_container_type &areas = tileMap->getAllAreas();
		for(
			typename TileT::area_container_type::const_iterator it = areas.cbegin(), endIT = areas.cend();
			it != endIT;
			++it
		)
		{
			addArea(it->getKey(), it->getValue() );
		}
		mergeIndex(m_lonPlaceIndex, tileMap->getLonPlaceIndex());
		mergeIndex(m_latPlaceIndex, tileMap->getLatPlaceIndex());
		mergeIndex(m_lonAreaIndex, tileMap->getLonAreaIndex());
		mergeIndex(m_latAreaIndex, tileMap->getLatAreaIndex());
	}
	template <class TileT>
	void mergeOsmTile( math::tileid_t tileID, const STRING &path, TileT *tileMap )
	{
		if( !getTileIDs().hasElement(tileID) )
		{
			STRING	mapFileName = getTileFileName( path, tileID );

			mergeOsmTile(mapFileName, tileMap);
		}
	}

	template <class TileT>
	void mergeOsmLayer( layer_key_type minLayerKey, layer_key_type maxLayerKey, const STRING &mapFileName, TileT *tileMap )
	{
		readFromBinaryFile(
			mapFileName,
			tileMap, OSM_MAGIC2, VERSION_MAGIC, true 
		);
		mergeLayer( minLayerKey, maxLayerKey, *tileMap );
	}

	template <class TileT>
	void mergeOsmLayer( layer_key_type minLayerKey, layer_key_type maxLayerKey, math::tileid_t tileID, const STRING &path, TileT *tileMap )
	{
		if( !getTileIDs().hasElement(tileID) )
		{
			STRING	mapFileName = getTileFileName( path, tileID );

			mergeOsmLayer(minLayerKey, maxLayerKey, mapFileName, tileMap);
		}
	}

};

typedef BasicOpenStreetMap<GraphTree, GeoBTree>	OSMbuilder;
typedef BasicOpenStreetMap<GraphMap, GeoArray>	OSMviewer;

template <>
struct is_binary<OSMviewer::PositionValue> : public internal::integral_constant<true>
{
};

template <>
inline void toBinaryStream( std::ostream &stream, const OSMviewer::PositionValue &value )
{
	binaryToBinaryStream( stream, value );
}

template <>
inline void fromBinaryStream( std::istream &stream, OSMviewer::PositionValue *value )
{
	binaryFromBinaryStream( stream, value );
}

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

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

inline STRING getTileFileName( const STRING &tilesPath, math::tileid_t tileID )
{
	return tilesPath + DIRECTORY_DELIMITER + formatNumber(tileID) + BINMAP_EXTENSION;
}

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

template<template<typename, typename>class MapT, template<typename>class IndexT>
void BasicOpenStreetMap<MapT, IndexT>::addArea( OsmAreaKeyT key, const Area &newArea )
{
	m_areas[key] = newArea;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template<template<typename, typename>class MapT, template<typename>class IndexT>
template <typename ScalarT>
void BasicOpenStreetMap<MapT, IndexT>::getPlaces(
	const OsmLayerKeyT									&layerKey, 
	const math::Rectangle< math::GeoPosition<ScalarT> >	&region,
	Array<OsmPlaceKeyT>									*result
) const
{
	doEnterFunction("BasicOpenStreetMap<MapT, IndexT>::getPlaces");

	Array<OsmNodeKeyT>	lonNodes, latNodes;

	assert( m_lonPlaceIndex.size() == m_latPlaceIndex.size() );
	assert( m_lonPlaceIndex.hasElement(layerKey) == m_latPlaceIndex.hasElement(layerKey) );

	if( m_lonPlaceIndex.hasElement(layerKey) )
	{
		assert( m_lonPlaceIndex[layerKey].size() == m_latPlaceIndex[layerKey].size() );

		ConstIterable<Layer::const_iterator>	rangeValues =
		m_lonPlaceIndex[layerKey].getRange(
			PositionValue(
				float(region.topLeft.longitude)
			),
			PositionValue(
				float(region.bottomRight.longitude)
			)
		);

		for(
			Layer::const_iterator	it = rangeValues.cbegin(),
				endIT = rangeValues.cend();
			it != endIT;
			++it
		)
		{
			lonNodes.addElement( it->nodeID );
		}
	}

	if( m_latPlaceIndex.hasElement(layerKey) )
	{
		ConstIterable<Layer::const_iterator>	rangeValues =
		m_latPlaceIndex[layerKey].getRange(
			PositionValue(
				float(region.bottomRight.latitude)
			),
			PositionValue(
				float(region.topLeft.latitude)
			)
		);
		for(
			Layer::const_iterator	it = rangeValues.cbegin(),
				endIT = rangeValues.cend();
			it != endIT;
			++it
		)
		{
			latNodes.addElement( it->nodeID );
		}
	}
	lonNodes.sort( FixedComparator<OsmNodeKeyT>() );
	latNodes.sort( FixedComparator<OsmNodeKeyT>() );
	intersectSorted( lonNodes, latNodes, std::back_inserter( *result ) );
}

template<template<typename, typename>class MapT, template<typename>class IndexT>
template <typename ScalarT>
void BasicOpenStreetMap<MapT, IndexT>::getAreas(
	const OsmLayerKeyT									&layerKey,
	const math::Rectangle< math::GeoPosition<ScalarT> >	&region,
	Set<OsmAreaKeyT>									*result
) const
{
	doEnterFunction("BasicOpenStreetMap<MapT, IndexT>::getAreas");

	Set<OsmAreaKeyT>	lonAreas, latAreas;

	assert( m_lonAreaIndex.size() == m_latAreaIndex.size() );
	assert( m_lonAreaIndex.hasElement(layerKey) == m_latAreaIndex.hasElement(layerKey) );

	if( m_lonAreaIndex.hasElement(layerKey) )
	{
		assert( m_lonAreaIndex[layerKey].size() == m_latAreaIndex[layerKey].size() );

		ConstIterable<Layer::const_iterator>	rangeValues =
		m_lonAreaIndex[layerKey].getRange(
			PositionValue(
				float(region.topLeft.longitude)
			),
			PositionValue(
				float(region.bottomRight.longitude)
			)
		);

		for(
			Layer::const_iterator	it = rangeValues.cbegin(),
				endIT = rangeValues.cend();
			it != endIT;
			++it
		)
		{
			lonAreas.addElement( it->nodeID );
		}
	}

	if( m_latAreaIndex.hasElement(layerKey) )
	{
		ConstIterable<Layer::const_iterator>	rangeValues =
		m_latAreaIndex[layerKey].getRange(
			PositionValue(
				float(region.bottomRight.latitude)
			),
			PositionValue(
				float(region.topLeft.latitude)
			)
		);
		for(
			Layer::const_iterator	it = rangeValues.cbegin(),
				endIT = rangeValues.cend();
			it != endIT;
			++it
		)
		{
			latAreas.addElement( it->nodeID );
		}
	}
	lonAreas.sort( FixedComparator<OsmAreaKeyT>() );
	latAreas.sort( FixedComparator<OsmAreaKeyT>() );
	intersectSorted( lonAreas, latAreas, std::back_inserter( *result ) );
}

template<template<typename, typename>class MapT, template<typename>class IndexT>
void BasicOpenStreetMap<MapT, IndexT>::addArea( OsmAreaKeyT key, OsmLayerKeyT layerKey, const Area &newArea )
{
	addArea(key, newArea);

	for(
		Area::const_iterator it = newArea.cbegin(), endIT = newArea.cend();
		it != endIT;
		++it
	)
	{
		float	 longitude = it->longitude;
		float	 latitude = it->latitude;

		expandBoundingBox(longitude,latitude);

		m_lonAreaIndex[layerKey].addElement(
			PositionValue( longitude, key )
		);
		m_latAreaIndex[layerKey].addElement(
			PositionValue( latitude, key )
		);
	}
}

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

#endif	// GAK_OPEN_STREET_MAP_H
