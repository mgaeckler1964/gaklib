/*
		Project:		GAKLIB
		Module:			geoGraph.h
		Description:	a graph for geometry data
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

#ifndef GAK_GEO_GRAPH_H
#define GAK_GEO_GRAPH_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/graph.h>
#include <gak/btree.h>
#include <gak/geometry.h>
#include <gak/gps.h>
#include <gak/optional.h>

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

template <typename ValueT>
struct GeoBTree : public Btree<ValueT>
{
};

template <typename ValueT>
struct GeoArray : public SortedArray<ValueT>
{
};
 
template <
	typename NodeT, typename LinkT,
	template<typename, typename> class MapT,
	template<typename> class IndexT,
	typename LayerKeyT = short,
	typename NodeKeyT=typename NodeT::key_type, typename LinkKeyT=typename LinkT::key_type
>
class GeoGraph : public Graph<NodeT, LinkT, MapT, NodeKeyT, LinkKeyT>
{
	typedef Graph<NodeT, LinkT, MapT, NodeKeyT, LinkKeyT>						Super;
	typedef GeoGraph<NodeT, LinkT, MapT, IndexT, LayerKeyT, NodeKeyT, LinkKeyT>	SelfT;

	public:

	typedef typename Super::node_container_type				node_container_type;
	typedef typename Super::link_container_type				link_container_type;
	typedef typename Super::link_key_types					link_key_types;
	typedef typename Super::link_key_type					link_key_type;
	typedef typename Super::node_key_type					node_key_type;
	typedef typename Super::LinkInfo						LinkInfo;

	typedef LayerKeyT										layer_key_type;

	typedef math::Rectangle< math::GeoPosition<double> >	BoundingBox;

#ifdef _MSC_VER
#	pragma pack(push, 2)
#endif
	struct PositionValue
	{
		float		circleDegree;
		NodeKeyT	nodeID;

		PositionValue( float circleDegree = 0, NodeKeyT nodeID = NodeKeyT() )
		: circleDegree(circleDegree), nodeID(nodeID)
		{
			assert( sizeof(*this) == sizeof(float) + sizeof(NodeKeyT));
		}
		int compare( const PositionValue &other ) const
		{
			int compareResult = gak::compare( circleDegree, other.circleDegree );
			if( !compareResult )
			{
				compareResult = gak::compare( nodeID, other.nodeID );
			}
			return compareResult;
		}
		void toBinaryStream( std::ostream &stream ) const
		{
			gak::toBinaryStream( stream, circleDegree );
			gak::toBinaryStream( stream, nodeID );
		}
		void fromBinaryStream( std::istream &stream )
		{
			gak::fromBinaryStream( stream, &circleDegree );
			gak::fromBinaryStream( stream, &nodeID );
		}
	};
#ifdef _MSC_VER
#	pragma pack( pop )
#endif
	typedef IndexT<PositionValue>		Layer;
	typedef PairMap<LayerKeyT, Layer>	Layers;

	private:
	Layers				m_lonIndex;
	Layers				m_latIndex;
	BoundingBox			m_boundingBox;
	math::TileIDsSet	m_tileIDs;

	public:
	GeoGraph() : m_boundingBox( +180, -90, -180, +90 )
	{
	}

	protected:
	void expandBoundingBox( double longitude, double latitude )
	{
		if( m_boundingBox.topLeft.longitude > longitude )
		{
			m_boundingBox.topLeft.longitude = longitude;
		}
		if( m_boundingBox.bottomRight.longitude < longitude )
		{
			m_boundingBox.bottomRight.longitude = longitude;
		}
		if( m_boundingBox.topLeft.latitude < latitude )
		{
			m_boundingBox.topLeft.latitude = latitude;
		}
		if( m_boundingBox.bottomRight.latitude > latitude )
		{
			m_boundingBox.bottomRight.latitude = latitude;
		}
	}
	private:
	void addNode( const NodeKeyT &key, const NodeT &node )
	{
		double	 longitude = node.getPosition().longitude;
		double	 latitude = node.getPosition().latitude;

		expandBoundingBox(longitude, latitude);
		Super::addNode( key, node );
	}
	public:
	void addNode( const LayerKeyT &layerKey, const NodeKeyT &key, const NodeT &node )
	{
		double	 longitude = node.getPosition().longitude;
		double	 latitude = node.getPosition().latitude;

		addNode(key, node);
		m_lonIndex[layerKey].addElement(
			PositionValue( float(longitude), key )
		);
		m_latIndex[layerKey].addElement(
			PositionValue( float(latitude), key )
		);
		m_tileIDs.addElement(node.getTileID());

		assert( m_lonIndex[layerKey].size() == m_latIndex[layerKey].size() );
	}
	void moveNode( 
		const LayerKeyT &oldLayerKey, const LayerKeyT &newLayerKey, 
		const NodeKeyT &nodeID
	)
	{
		const NodeT	&node = this->getNode( nodeID );
		float	 longitude = node.getPosition().longitude;
		float	 latitude = node.getPosition().latitude;
		PositionValue	lonPosition ( longitude, nodeID );
		PositionValue	latPosition ( latitude, nodeID );

		m_lonIndex[oldLayerKey].removeElement( lonPosition );
		m_lonIndex[newLayerKey].addElement( lonPosition );

		m_latIndex[oldLayerKey].removeElement( latPosition );
		m_latIndex[newLayerKey].addElement( latPosition );

		assert( m_lonIndex[oldLayerKey].size() == m_latIndex[oldLayerKey].size() );
		assert( m_lonIndex[newLayerKey].size() == m_latIndex[newLayerKey].size() );
	}

	void mergeLayer( Layer &target, const Layer src )
	{
		for(
			Layer::const_iterator it = src.cbegin(), endIT = src.cend();
			it != endIT;
			++it
		)
		{
			target.addElement( *it );
		}
	}
	void mergeIndex( Layers &target, const Layers &src )
	{
		for(
			Layers::const_iterator it = src.cbegin(), endIT = src.cend();
			it != endIT;
			++it
		)
		{
			Layer &targetLayer = target[it->getKey()];
			const Layer &srcLayer = it->getValue();
			mergeLayer( targetLayer, srcLayer );
		}
	}
	void mergeTile( const SelfT &tileMap )
	{
		const node_container_type &nodes = tileMap.getNodes();
		for(
			node_container_type::const_iterator it = nodes.cbegin(), endIT = nodes.cend();
			it != endIT;
			++it
		)
		{
			addNode( it->getKey(), it->getValue().m_node );
		}

		const link_container_type &links = tileMap.getLinks();
		for(
			link_container_type::const_iterator it = links.cbegin(), endIT = links.cend();
			it != endIT;
			++it
		)
		{
			const LinkInfo &link = it->getValue();
			addLink(it->getKey(), link.m_link, link.m_startNodeID, link.m_endNodeID );
		}

		mergeIndex(m_lonIndex, tileMap.m_lonIndex);
		mergeIndex(m_latIndex, tileMap.m_latIndex);
	}


	void mergeLayerTile( layer_key_type layerKey, const Layer &src, const SelfT &srcMap )
	{
		for(
			Layer::const_iterator itN = src.cbegin(), endIT = src.cend();
			itN != endIT;
			++itN
		)
		{
			node_key_type nodeID = itN->nodeID;
			const NodeInfo &nInfo = srcMap.getNodeInfo( nodeID );
			addNode(layerKey, nodeID, nInfo.m_node);
			const link_key_types &outgoing = nInfo.m_outgoing;
			for(
				link_key_types::const_iterator itL = outgoing.cbegin(), endIT = outgoing.cend();
				itL != endIT;
				++itL
			)
			{
				link_key_type linkID = *itL;
				const LinkInfo &lInfo = srcMap.getLinkInfo(linkID);
				addLink(linkID,lInfo.m_link, lInfo.m_startNodeID, lInfo.m_endNodeID );
			}
		}
	}
	void mergeLayerIndex( layer_key_type layerKey, const Layers &srcIndex, const SelfT &srcMap )
	{
		for(
			Layers::const_iterator it = srcIndex.cbegin(), endIT = srcIndex.cend();
			it != endIT;
			++it
		)
		{
			layer_key_type curLayerKey = it->getKey();
			if( curLayerKey <= layerKey )
			{
				const Layer &srcLayer = it->getValue();
				mergeLayerTile( curLayerKey, srcLayer, srcMap );
			}
		}
	}
	void mergeLayer( layer_key_type layerKey, const SelfT &srcMap )
	{
		mergeLayerIndex(layerKey, srcMap.m_lonIndex, srcMap);
		//mergeTileIndex(layerKey, srcMap.m_latIndex, srcMap);
	}

	Optional<NodeKeyT> findNextNode( 
		const math::GeoPosition<double> &point, double searchRange, LayerKeyT layer
	) const;
	Optional<NodeKeyT> findNextNode( 
		const math::GeoPosition<double> &point, double searchRange 
	) const;

	const BoundingBox &getBoundingBox() const
	{
		assert( m_lonIndex.size() == m_latIndex.size() );

		return m_boundingBox;
	}
	BoundingBox getFrameBox(double frame)
	{
		BoundingBox frameBox = m_boundingBox;

		frameBox.topLeft.latitude += frame;
		frameBox.topLeft.longitude -= frame;
		frameBox.bottomRight.latitude -= frame;
		frameBox.bottomRight.longitude += frame; 

		return frameBox;
	}
	const math::TileIDsSet &getTileIDs() const
	{
		return m_tileIDs;
	}
	template <typename ScalarT>
	void getRegion(
		const LayerKeyT										&layerKey, 
		const math::Rectangle< math::GeoPosition<ScalarT> >	&region,
		Array<NodeKeyT>										*result
	) const;

	void clear()
	{
		Super::clear();
		m_lonIndex.clear();
		m_latIndex.clear();
		m_tileIDs.clear();
	}

	void toBinaryStream( std::ostream &stream ) const
	{
		Super::toBinaryStream( stream );
		m_lonIndex.toBinaryStream( stream );
		m_latIndex.toBinaryStream( stream );
		m_tileIDs.toBinaryStream( stream );
		binaryToBinaryStream( stream, m_boundingBox );
	}
	void fromBinaryStream( std::istream &stream )
	{
		doEnterFunction("GeoGraph::fromBinaryStream");
		Super::fromBinaryStream( stream );
		{
			doEnterFunction("m_lonIndex.fromBinaryStream");
			m_lonIndex.fromBinaryStream( stream );
		}
		{
			doEnterFunction("m_latIndex.fromBinaryStream");
			m_latIndex.fromBinaryStream( stream );
		}
		{
			doEnterFunction("m_tileIDs.fromBinaryStream");
			m_tileIDs.fromBinaryStream( stream );
		}

		binaryFromBinaryStream( stream, &m_boundingBox );
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

template <
	typename NodeT, typename LinkT,
	template<typename, typename> class MapT,
	template<typename> class IndexT,
	typename LayerKeyT,
	typename NodeKeyT, typename LinkKeyT
>
	template <typename ScalarT>
void GeoGraph<NodeT, LinkT, MapT, IndexT, LayerKeyT, NodeKeyT, LinkKeyT>::getRegion(
	const LayerKeyT										&layerKey, 
	const math::Rectangle< math::GeoPosition<ScalarT> >	&region,
	Array<NodeKeyT>										*result
) const
{
	doEnterFunction("GeoGraph<NodeT, LinkT, MapT, IndexT, LayerIdxT, NodeKeyT, LinkKeyT>::getRegion");

	Array<NodeKeyT>	lonNodes, latNodes;

	assert( m_lonIndex.size() == m_latIndex.size() );
	assert( m_lonIndex.hasElement(layerKey) == m_latIndex.hasElement(layerKey) );

	if( m_lonIndex.hasElement(layerKey) )
	{
		const Layer &lonLayerIndex = m_lonIndex[layerKey];

		assert( lonLayerIndex.size() == m_latIndex[layerKey].size() );

		ConstIterable<typename Layer::const_iterator>	rangeValues =
		lonLayerIndex.getRange(
			PositionValue(
				float(region.topLeft.longitude)
			),
			PositionValue(
				float(region.bottomRight.longitude)
			)
		);

		for(
			typename Layer::const_iterator	it = rangeValues.cbegin(),
				endIT = rangeValues.cend();
			it != endIT;
			++it
		)
		{
			lonNodes.addElement( it->nodeID );
		}
	}

	if( m_latIndex.hasElement(layerKey) )
	{
		const Layer &latLayerIndex = m_latIndex[layerKey];

		ConstIterable<typename Layer::const_iterator>	rangeValues =
		latLayerIndex.getRange(
			PositionValue(
				float(region.bottomRight.latitude)
			),
			PositionValue(
				float(region.topLeft.latitude)
			)
		);
		for(
			typename Layer::const_iterator	it = rangeValues.cbegin(),
				endIT = rangeValues.cend();
			it != endIT;
			++it
		)
		{
			latNodes.addElement( it->nodeID );
		}
	}
	lonNodes.sort( FixedComparator<NodeKeyT>() );
	latNodes.sort( FixedComparator<NodeKeyT>() );
	intersectSorted( lonNodes, latNodes, std::back_inserter( *result ) );
}

template <
	typename NodeT, typename LinkT,
	template<typename, typename> class MapT,
	template<typename> class IndexT,
	typename LayerKeyT,
	typename NodeKeyT, typename LinkKeyT
>
Optional<NodeKeyT> GeoGraph<NodeT, LinkT, MapT, IndexT, LayerKeyT, NodeKeyT, LinkKeyT>::findNextNode(
	const math::GeoPosition<double> &point, double searchRange, LayerKeyT layer
) const
{
	typedef math::Rectangle< math::GeoPosition<double> >	BoundingBox;

	Optional<NodeKeyT>	result;
	BoundingBox			bBox(
		point.longitude-searchRange, point.latitude+searchRange,
		point.longitude+searchRange, point.latitude-searchRange
	);

	Array<NodeKeyT>		candidates;
	getRegion( layer, bBox, &candidates );

	double minDistance = std::numeric_limits<double>::max();
	for(
		typename Array<NodeKeyT>::const_iterator it = candidates.cbegin(),
			endIT = candidates.cend();
		it != endIT;
		++it
	)
	{
		NodeKeyT	nodeID = *it;
		const NodeT	&node = getNode( nodeID );
		double		distance = getDistance( point, node.getPosition() );
		if( minDistance > distance )
		{
			minDistance = distance;
			result = nodeID;
		}
	}
	return result;
}

template <
	typename NodeT, typename LinkT,
	template<typename, typename> class MapT,
	template<typename> class IndexT,
	typename LayerKeyT,
	typename NodeKeyT, typename LinkKeyT
>
Optional<NodeKeyT> GeoGraph<NodeT, LinkT, MapT, IndexT, LayerKeyT, NodeKeyT, LinkKeyT>::findNextNode(
	const math::GeoPosition<double> &point, double searchRange
) const
{
	Optional<NodeKeyT>	result;
	double				minDistance = std::numeric_limits<double>::max();
	for(
		typename Layers::const_iterator it = m_lonIndex.cbegin(), endIT = m_lonIndex.cend();
		it != endIT;
		++it
	)
	{
		Optional<NodeKeyT>	tmpResult = findNextNode( point, searchRange, it->getKey() );
		if( tmpResult )
		{
			NodeKeyT	nodeID = tmpResult.get();
			const NodeT	&node = getNode( nodeID );
			double		distance = getDistance( point, node.getPosition() );
			if( minDistance > distance )
			{
				minDistance = distance;
				result = nodeID;
			}
		}
	}
	return result;
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

#endif	// GAK_GEO_GRAPH_H
