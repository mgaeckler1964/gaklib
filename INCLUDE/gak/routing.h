/*
		Project:		GAKLIB
		Module:			routing.h
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

#ifndef GAK_ROUTING_H
#define GAK_ROUTING_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

template <typename NodeT, typename CostT>
double getEstimatedCost( const NodeT &, const NodeT &, CostT  )
{
	return 0;
}

template <typename LinkT, typename CostT>
double getLinkCost( const LinkT &, CostT  )
{
	return 1;
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename GraphT>
class Routing
{
	public:
	typedef GraphT									graph_t;

	typedef typename graph_t::LinkInfo				LinkInfo;
	typedef typename graph_t::link_type				link_type;
	typedef typename graph_t::link_key_type			link_key_type;
	typedef typename graph_t::link_key_types		link_key_types;
	typedef typename graph_t::link_container_type	link_container_type;

	typedef typename graph_t::node_type				node_type;
	typedef typename graph_t::node_key_type			node_key_type;
	typedef typename graph_t::node_key_types		node_key_types;

	protected:
	const graph_t &m_graph;

	public: 
	struct RouteResult
	{
		link_key_types	links;
		double			cost;
	};

	public:
	Routing( const graph_t &graph ) : m_graph(graph)
	{
	}
};

template <typename GraphT>
class AstarRouting : public Routing<GraphT>
{
	public:
	typedef Routing<GraphT>						Super;

	typedef typename Super::graph_t				graph_t;
	typedef typename Super::RouteResult			RouteResult;

	typedef typename Super::LinkInfo			LinkInfo;
	typedef typename Super::link_type			link_type;
	typedef typename Super::link_key_type		link_key_type;
	typedef typename Super::link_key_types		link_key_types;
	typedef typename Super::link_container_type	link_container_type;

	typedef typename Super::node_type			node_type;
	typedef typename Super::node_key_type		node_key_type;
	typedef typename Super::node_key_types		node_key_types;

	private:
	class NodeRouteInfo
	{
		public:
		typedef double priority_type;

		private:
		double			m_routeCost, m_estimatedRemainigCost;
		double			m_priority;

		node_key_type	m_nodeID;
		link_key_type	m_linkID;
		NodeRouteInfo	*m_previous;

		void calcPrio()
		{
			m_priority = -m_routeCost - m_estimatedRemainigCost;
		}
		public:
		NodeRouteInfo( node_key_type nodeID, double remainingCost )
		{
			m_nodeID = nodeID;
			m_estimatedRemainigCost = remainingCost;
			setRouteCost( link_key_type(),  0, NULL );
		}
		NodeRouteInfo(
			node_key_type nodeID, double remainingCost,
			link_key_type linkID, double routeCost, NodeRouteInfo *previous 
		)
		{
			m_nodeID = nodeID;
			m_estimatedRemainigCost = remainingCost;
			setRouteCost( linkID,  routeCost, previous );
		}
		void setRouteCost( 
			link_key_type linkID, double routeCost, NodeRouteInfo *previous 
		)
		{
			m_linkID = linkID;
			m_routeCost = routeCost;
			m_previous = previous;
			calcPrio();
		}
		node_key_type getNodeID() const
		{
			return m_nodeID;
		}
		double getPriority() const
		{
			return m_priority;
		}
		double getRouteCost() const
		{
			return m_routeCost;
		}
		NodeRouteInfo *getPrevious() const
		{
			return m_previous;
		}
		link_key_type getLinkID() const
		{
			return m_linkID;
		}
	};

	typedef PriorityQueue2<NodeRouteInfo*, double, PointerPriorityExtractor<NodeRouteInfo, double> >	OpenNodes;
	typedef List<NodeRouteInfo>														NodeRouteInfos;
	typedef TreeMap<node_key_type, NodeRouteInfo*>									VisitedNodes;

	public:
	AstarRouting( const graph_t &graph ) : Super(graph)
	{
	}

	template <typename CostT>
	Optional<RouteResult> route( const node_key_type &from, const node_key_type &to, CostT costType ) const;
};

#ifndef __BORLANDC__
template <typename GraphT>
class RecursiveRouting : public Routing<GraphT>
{
	public:
	typedef Routing<GraphT>						Super;

	typedef typename Super::graph_t				graph_t;
	typedef typename Super::RouteResult			RouteResult;

	typedef typename Super::LinkInfo			LinkInfo;
	typedef typename Super::link_type			link_type;
	typedef typename Super::link_key_type		link_key_type;
	typedef typename Super::link_key_types		link_key_types;
	typedef typename Super::link_container_type	link_container_type;

	typedef typename Super::node_type			node_type;
	typedef typename Super::node_key_type		node_key_type;
	typedef typename Super::node_key_types		node_key_types;

	private:
	struct OutgoingInfo
	{
		double			m_cost;
		link_key_type	m_linkID;

		OutgoingInfo()
		{
		}
		OutgoingInfo( double cost, link_key_type linkID ) : m_cost(cost), m_linkID(linkID)
		{
		}
		int compare( const OutgoingInfo &other ) const
		{
			return gak::compare( m_cost, other.m_cost );
		}
	};
	class Route
	{
		link_key_types	m_links;
		double			m_cost;
		node_key_type	m_currentPosition;

		public:
		const link_key_types &getLinks( void ) const
		{
			return m_links;
		}
		double getCost( void ) const
		{
			return m_cost;
		}
		node_key_type getPosition( void ) const
		{
			return m_currentPosition;
		}

		Route( node_key_type start )
		{
			m_currentPosition = start;
			m_cost = 0;
		}

		void addLink( link_key_type linkID, double linkCost, node_key_type current )
		{
			m_links.addElement( linkID );
			m_currentPosition = current;
			m_cost += linkCost;
		}
		void removeLastLink( link_key_type linkID, double linkCost, node_key_type current )
		{
			assert( linkID == *m_links.crbegin() );
			m_links.removeElementAt( m_links.size() -1 );
			m_currentPosition = current;
			m_cost -= linkCost;
		}
	};

	typedef KeyValuePair<node_key_type, node_key_type>	RouteID;
	typedef PairMap<RouteID, double>					BestCosts;

	template <typename CostT>
	struct RoutingStatus
	{
		typedef PairMap<node_key_type, typename link_key_types::const_iterator >	NodeIndices;

		BestCosts		m_bestCosts;
		link_key_types	m_bestRoute;
		NodeIndices		m_routeNodes;
		double			m_bestCost;
		CostT			m_costType;
		node_key_type	m_start, m_destination;
		Route			m_currentRoute;

		RoutingStatus( CostT costType, node_key_type start, node_key_type destination ) : m_currentRoute( start )
		{
			m_bestCost = std::numeric_limits<double>::max();
			m_costType = costType;
			m_start = start;
			m_destination = destination;
		}

		void setBestRoute( const Route &currentRoute, const link_container_type &links )
		{
			m_routeNodes.clear();
			m_bestRoute = currentRoute.getLinks();
			m_bestCost = currentRoute.getCost();
			for(
				typename link_key_types::const_iterator it = m_bestRoute.cbegin(), endIT = m_bestRoute.cend();
				it != endIT;
				++it
			)
			{
				link_key_type	linkID = *it;
				m_routeNodes[links[linkID].m_startNodeID] = it;
			}
		}
	};

	template <typename CostT>
	SortedArray<OutgoingInfo> getOutgoing( const node_key_type &from, const node_key_type &to, CostT costType ) const
	{
		SortedArray<OutgoingInfo>	sortedOutgoing;
		const link_key_types		&outgoing = Super::m_graph.getOutgoing( from );

		for(
			typename link_key_types::const_iterator it = outgoing.cbegin(), endIT = outgoing.cend();
			it != endIT;
			++it
		)
		{
			link_key_type	linkID = *it;
			sortedOutgoing.addElement(
				OutgoingInfo( 
					getEstimatedCost( 
						Super::m_graph.getNode( Super::m_graph.getLinkEnd(linkID) ), 
						Super::m_graph.getNode( to ), costType
					), linkID 
				)
			);
		}
		return sortedOutgoing;
	}

	template <typename CostT>
	bool route( 
		RoutingStatus<CostT>	&routingStatus
	) const
	{
		bool			routeFound = false;
		node_key_type	startNodeID = routingStatus.m_start;
		node_key_type	currentEndNodeID = routingStatus.m_currentRoute.getPosition();
		BestCosts		&bestCosts = routingStatus.m_bestCosts;
		Route			&currentRoute = routingStatus.m_currentRoute;
		RouteID			currentRouteID( startNodeID, currentEndNodeID );

		// did we see a better route yet?
		if( bestCosts.hasElement( currentRouteID ) && bestCosts[currentRouteID] < currentRoute.getCost() )
		{
			return false;
		}
		bestCosts[currentRouteID] = currentRoute.getCost();
		// did we find a new candidate?
		if( currentEndNodeID == routingStatus.m_destination )
		{
			routingStatus.setBestRoute( currentRoute, Super::m_graph.getLinks() );
			return true;
		}
		// Is a better candidate possible?
		if( 
			currentRoute.getCost() + getEstimatedCost( 
				Super::m_graph.getNode(currentEndNodeID), Super::m_graph.getNode(routingStatus.m_destination), routingStatus.m_costType 
			) >= routingStatus.m_bestCost
		)
		{
			return false;
		}
		// do we know the best tail of this route?
		size_t	tailIndex = routingStatus.m_routeNodes.getElementIndex( currentEndNodeID );
		if( tailIndex != routingStatus.m_routeNodes.no_index )
		{
			Route newRoute = currentRoute;
			for(
				typename link_key_types::const_iterator it = routingStatus.m_routeNodes.getElementAt(tailIndex).getValue(),
					endIT = routingStatus.m_bestRoute.cend();
				it != endIT;
				++it
			)
			{
				link_key_type linkID = *it;
				newRoute.addLink(
					linkID,
					getLinkCost( Super::m_graph.getLink(linkID), routingStatus.m_costType ),
					Super::m_graph.getLinkEnd(linkID)
				);
				RouteID newRouteID( startNodeID, newRoute.getPosition() );
				bestCosts[newRouteID] = newRoute.getCost();
			}
			routingStatus.setBestRoute( newRoute, Super::m_graph.getLinks() );

			return true;
		}

		SortedArray<OutgoingInfo> outgoing = getOutgoing( 
			currentEndNodeID, routingStatus.m_destination, routingStatus.m_costType 
		);
		for(
			typename SortedArray<OutgoingInfo>::const_iterator it = outgoing.cbegin(), endIT = outgoing.cend();
 			it != endIT;
			++it
		)
		{ 
			link_key_type linkID = it->m_linkID;
			double linkCost = getLinkCost( Super::m_graph.getLink(linkID), routingStatus.m_costType );
			if( currentRoute.getCost() + linkCost < routingStatus.m_bestCost  )
			{
				currentRoute.addLink( linkID, linkCost, Super::m_graph.getLinkEnd(linkID) );
				size_t oldSize = currentRoute.getLinks().size();
				routeFound |= route( routingStatus );
				assert( oldSize == currentRoute.getLinks().size() );
				currentRoute.removeLastLink( linkID, linkCost, currentEndNodeID );
			}
		}

		return routeFound;
	}

	public:
	RecursiveRouting( const graph_t &graph ) : Super(graph)
	{
	}

	template <typename CostT>
	Optional<RouteResult> route( const node_key_type &from, const node_key_type &to, CostT costType ) const
	{
		Optional<RouteResult>	result;
		RoutingStatus<CostT>	routingStatus( costType, from, to );

		if( route( routingStatus ) )
		{
			RouteResult	&foundRoute = result.setDefault().get();
			foundRoute.links.moveFrom( routingStatus.m_bestRoute );
			foundRoute.cost = routingStatus.m_bestCost;
		}

		return result;
	}
};
#else
#	define RecursiveRouting AstarRouting
#endif	// __BORLANDC__

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

template <typename GraphT>
	template <typename CostT>
Optional<typename Routing<GraphT>::RouteResult>
AstarRouting<GraphT>::route(
	const node_key_type &from, const node_key_type &to, CostT costType
) const
{
	Optional<RouteResult>	result;
	bool					routeFound = false;
	OpenNodes				openNodes;
	NodeRouteInfos			nodeInfos;
	VisitedNodes			visited;

	const node_type			&destination = Super::m_graph.getNode( to );
	node_key_type			currentNodeID = from;
	NodeRouteInfo			*currentNodeInfo = nodeInfos.addElement(
		NodeRouteInfo(
			from, getEstimatedCost(
				Super::m_graph.getNode( from ), destination, costType
			)
		)
	);
	visited.setValue( from, currentNodeInfo );
	openNodes.push( currentNodeInfo );

	while( openNodes.size() )
	{
		currentNodeInfo = openNodes.pop();
		currentNodeID = currentNodeInfo->getNodeID();
		double currentRouteCost = currentNodeInfo->getRouteCost();

		if( currentNodeID == to )
		{
			routeFound = true;
			break;
		}

		const link_key_types &out = Super::m_graph.getOutgoing( currentNodeID );
		for(
			typename link_key_types::const_iterator it = out.cbegin(), endIT = out.cend();
			it != endIT;
			++it
		)
		{
			link_key_type	linkID = *it;
			const LinkInfo	&linkInfo = Super::m_graph.getLinkInfo(linkID);
			node_key_type	endNodeID = linkInfo.m_endNodeID;
			const node_type &endNode = Super::m_graph.getNode( endNodeID );

			double			linkCost = getLinkCost( linkInfo.m_link, costType );
			double			estimatedRemaining = getEstimatedCost( endNode, destination, costType );
			double			newRouteCost = currentRouteCost + linkCost;

			NodeRouteInfo	* const  *vPtr = visited.findValueByKey( endNodeID );
			if( vPtr )
			{
				NodeRouteInfo	*nextNodeInfo = *vPtr;
				if( newRouteCost < nextNodeInfo->getRouteCost() )
				{
					nextNodeInfo->setRouteCost( linkID, newRouteCost, currentNodeInfo );
					openNodes.push( nextNodeInfo );
				}
			}
			else
			{
				NodeRouteInfo	*nextNodeInfo = nodeInfos.addElement(
					NodeRouteInfo(
						endNodeID, estimatedRemaining,
						linkID, newRouteCost, currentNodeInfo
					)
				);
				visited.setValue( endNodeID, nextNodeInfo );
				openNodes.push( nextNodeInfo );
			}
		}
	}

	if( routeFound )
	{
		RouteResult	&foundRoute = result.setDefault().get();
		foundRoute.cost = currentNodeInfo->getRouteCost();

		Stack<link_key_type>	backRoute;
		while( currentNodeInfo->getPrevious() )
		{
			backRoute.push( currentNodeInfo->getLinkID() );
			currentNodeInfo = currentNodeInfo->getPrevious();
		}

		while( backRoute.size() )
		{
			foundRoute.links.addElement( backRoute.pop() );
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

#endif	// GAK_ROUTING_H
