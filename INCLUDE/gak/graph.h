/*
		Project:		GAKLIB
		Module:			graph.h
		Description:	a container used to control linked data structures
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#ifndef GAK_GRAPH_H
#define GAK_GRAPH_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>

#include <gak/map.h>
#include <gak/optional.h>
#include <gak/priorityQueue.h>
#include <gak/set.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#define HAVE_INCOMING	0

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

template <typename KeyT, typename ValueT>
struct GraphTree : public TreeMap<KeyT, ValueT>
{
};

template <typename KeyT, typename ValueT>
struct GraphMap : public PairMap<KeyT, ValueT>
{
};
 
template <
	typename NodeT, typename LinkT,
	template <typename, typename> class MapT,
	typename NodeKeyT=typename NodeT::key_type, typename LinkKeyT=typename LinkT::key_type
>
class Graph
{
	public:
	typedef NodeT					node_type;
	typedef LinkT					link_type;
	typedef NodeKeyT				node_key_type;
	typedef Array<node_key_type>	node_key_types;
	typedef LinkKeyT				link_key_type;
	typedef Array<link_key_type>	link_key_types;
	typedef Graph<NodeT, LinkT, MapT, NodeKeyT, LinkKeyT>		SelfT;

	struct NodeInfo
	{
		link_key_types	m_outgoing;
#if HAVE_INCOMING
		link_key_types	m_incoming;
#endif
		node_type		m_node;
		void toBinaryStream( std::ostream &stream ) const
		{
			m_outgoing.toBinaryStream( stream );
#if HAVE_INCOMING
			m_incoming.toBinaryStream( stream );
#endif
			gak::toBinaryStream( stream, m_node );
		}
		void fromBinaryStream( std::istream &stream )
		{
			m_outgoing.fromBinaryStream( stream );
#if HAVE_INCOMING
			m_incoming.fromBinaryStream( stream );
#endif
			gak::fromBinaryStream( stream, &m_node );
		}
	};
	struct LinkInfo
	{
		node_key_type	m_startNodeID,
						m_endNodeID;
		link_type		m_link;

		void toBinaryStream( std::ostream &stream ) const
		{
			gak::toBinaryStream( stream, m_startNodeID );
			gak::toBinaryStream( stream, m_endNodeID );
			gak::toBinaryStream( stream, m_link );
		}
		void fromBinaryStream( std::istream &stream )
		{
			gak::fromBinaryStream( stream, &m_startNodeID );
			gak::fromBinaryStream( stream, &m_endNodeID );
			gak::fromBinaryStream( stream, &m_link );
		}
	};
	typedef MapT<NodeKeyT, NodeInfo>		node_container_type;
	typedef MapT<LinkKeyT, LinkInfo>		link_container_type;

	private:
	node_container_type		m_nodes;
	link_container_type		m_links;

	public:
	void toBinaryStream( std::ostream &stream ) const
	{
		m_nodes.toBinaryStream( stream );
		m_links.toBinaryStream( stream );
	}
	void fromBinaryStream( std::istream &stream )
	{
		doEnterFunction("Graph::fromBinaryStream");
		{
			doEnterFunction("m_nodes.fromBinaryStream");
			m_nodes.fromBinaryStream( stream );
		}
		{
			doEnterFunction("m_links.fromBinaryStream");
			m_links.fromBinaryStream( stream );
		}
	}

	const link_key_types &getOutgoing( const node_key_type &from ) const
	{
		return m_nodes[from].m_outgoing;
	}
	bool hasNode( const node_key_type &nodeID ) const
	{
		return m_nodes.hasElement( nodeID );
	}
	void addNode( const node_key_type &key, const node_type &node )
	{
		if( m_nodes.hasElement( key ) )
		{
			throw NodeExistsError();
		}
		m_nodes[key].m_node = node;
	}

	node_type &getNode( const node_key_type &nodeID )
	{
		if( !m_nodes.hasElement( nodeID ) )
		{
			throw NodeNotFoundError();
		}
		return m_nodes[nodeID].m_node;
	}
	const node_type &getNode( const node_key_type &nodeID ) const
	{
		if( !m_nodes.hasElement( nodeID ) )
		{
			throw NodeNotFoundError();
		}
		return m_nodes[nodeID].m_node;
	}
	const NodeInfo &getNodeInfo( const node_key_type &nodeID ) const
	{
		if( !m_nodes.hasElement( nodeID ) )
		{
			throw NodeNotFoundError();
		}
		return m_nodes[nodeID];
	}

#if HAVE_INCOMING
	void removeNode( const node_key_type &nodeID );
#endif
	size_t getNumNodes() const
	{
		return m_nodes.size();
	}
	const node_container_type &getNodes() const
	{
		return m_nodes;
	}

	bool hasLink( const link_key_type &key ) const
	{
		return m_links.hasElement( key );
	}
	void addLink( const link_key_type &key, const link_type &link, const node_key_type &from, const node_key_type &to )
	{
		if( m_links.hasElement( key ) )
		{
			throw LinkExistsError();
		}
		if( !m_nodes.hasElement( from ) )
		{
			throw NodeNotFoundError();
		}

		m_nodes[from].m_outgoing.addElement( key );

#if HAVE_INCOMING
		if( m_nodes.hasElement(to) )
			m_nodes[to].m_incoming.addElement( key );
#endif
		LinkInfo	&linkInfo = m_links[key];
		linkInfo.m_startNodeID = from;
		linkInfo.m_endNodeID = to;
		linkInfo.m_link = link;
	}
	link_type &getLink( const link_key_type &linkID )
	{
		if( !m_links.hasElement( linkID ) )
		{
			throw LinkNotFoundError();
		}
		return m_links[linkID].m_link;
	}
	const link_type &getLink( const link_key_type &linkID ) const
	{
		if( !m_links.hasElement( linkID ) )
		{
			throw LinkNotFoundError();
		}
		return m_links[linkID].m_link;
	}
	const LinkInfo &getLinkInfo( const link_key_type &linkID ) const
	{
		if( !m_links.hasElement( linkID ) )
		{
			throw LinkNotFoundError();
		}
		return m_links[linkID];
	}
	const node_key_type &getLinkStart( const link_key_type &linkID ) const
	{
		if( !m_links.hasElement( linkID ) )
		{
			throw LinkNotFoundError();
		}
		return m_links[linkID].m_startNodeID;
	}
	const node_key_type &getLinkEnd( const link_key_type &linkID ) const
	{
		if( !m_links.hasElement( linkID ) )
		{
			throw LinkNotFoundError();
		}
		return m_links[linkID].m_endNodeID;
	}
#if HAVE_INCOMING
	void removeLink( const link_key_type &linkID )
	{
		if( !m_links.hasElement( linkID ) )
		{
			throw LinkNotFoundError();
		}
		LinkInfo	&link = m_links[linkID];
		NodeInfo	&startNode = m_nodes[link.m_startNodeID];
		NodeInfo	&endNode = m_nodes[link.m_endNodeID];
		startNode.m_outgoing.removeElementVal( linkID );
		endNode.m_incoming.removeElementVal( linkID );
		m_links.removeElementByKey( linkID );
	}
#endif
	size_t getNumLinks() const
	{
		return m_links.size();
	}
	const link_container_type &getLinks() const
	{
		return m_links;
	}

	node_key_types getNodes( const link_key_types &route ) const;

	void clear()
	{
		m_nodes.clear();
		m_links.clear();
	}

	SelfT &merge( const SelfT &src )
	{
		m_nodes.merge( src.m_nodes );
		m_links.merge( src.m_links );
		return *this;
	};
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

#if HAVE_INCOMING
template <
	typename NodeT, typename LinkT,
	template <typename, typename> class MapT,
	typename NodeKeyT, typename LinkKeyT
>
void Graph<NodeT, LinkT, MapT, NodeKeyT, LinkKeyT>::removeNode(
	const node_key_type &nodeID
)
{
	if( !m_nodes.hasElement( nodeID ) )
	{
		throw NodeNotFoundError();
	}
	NodeInfo &node = m_nodes[nodeID];
	while( node.m_outgoing.size() )
	{
		removeLink( *node.m_outgoing.rbegin() );
	}
	while( node.m_incoming.size() )
	{
		removeLink( *node.m_incoming.rbegin() );
	}
	m_nodes.removeElementByKey( nodeID );
}
#endif

template <
	typename NodeT, typename LinkT,
	template <typename, typename> class MapT,
	typename NodeKeyT, typename LinkKeyT
>
Array<NodeKeyT> Graph<NodeT, LinkT, MapT, NodeKeyT, LinkKeyT>::getNodes(
	const link_key_types &route
) const
{
	node_key_types			result;
	node_key_type			start;
	Optional<node_key_type>	end;

	for(
		typename link_key_types::const_iterator it = route.cbegin(), 
			endIT = route.cend();
		it != endIT;
		++it
	)
	{
		link_key_type	linkID = *it;

		start = m_links[linkID].m_startNodeID;
		if( end && start != end.get() )
		{
			throw IndexError();
		}
		result.addElement( start );
		end = m_links[linkID].m_endNodeID;
	}
	if( end )
	{
		result.addElement( end.get() );
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

#endif	// GAK_GRAPH_H
