/*
		Project:		GAKLIB
		Module:			aiBrain.h
		Description:	The brain for AI
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

#ifndef GAK_AI_BRAIN_H
#define GAK_AI_BRAIN_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/array.h>
#include <gak/btree.h>
#include <gak/set.h>

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

struct AiNode
{
	Array<STRING>	words;
	size_t			count;

	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, words );
		gak::toBinaryStream( stream, count );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &words );
		gak::fromBinaryStream( stream, &count );
	}
};

struct AiIndex
{
	STRING			word;
	Set<size_t>		positions;

	AiIndex() {}
	AiIndex(const STRING &word) : word(word) {}
	int compare( const AiIndex &e2 ) const
	{
		return gak::compare( word, e2.word );
	}
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, word );
		gak::toBinaryStream( stream, positions );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &word );
		gak::fromBinaryStream( stream, &positions );
	}
};

class AiBrain
{
	Array<AiNode>	m_knowledge;
	Btree<AiIndex>	m_index;

	void createPair(const STRING &w1, const STRING &w2)
	{
		size_t	newPosition = m_knowledge.size();
		AiNode &newNode = m_knowledge.createElement();
		newNode.words.addElement(w1);
		newNode.words.addElement(w2);
		newNode.count = 1;

		AiIndex *wi1 = const_cast<AiIndex *>(m_index.findElement(AiIndex(w1)));
		if( wi1 )
		{
			wi1->positions.addElement(newPosition);
		}
		else
		{
			AiIndex wi(w1);
			wi.positions.addElement(newPosition);
			m_index.addElement(wi);
		}

		AiIndex *wi2 = const_cast<AiIndex *>(m_index.findElement(AiIndex(w2)));
		if( wi2 )
		{
			wi2->positions.addElement(newPosition);
		}
		else
		{
			AiIndex wi(w2);
			wi.positions.addElement(newPosition);
			m_index.addElement(wi);
		}
	}

	public:
	size_t getPairCount(const STRING &w1, const STRING &w2) const
	{
		if( w1 == w2 )
		{
			return 0;
		}
		size_t idx = findPair(w1, w2);
		if( idx != m_knowledge.no_index )
		{
			return m_knowledge[idx].count;
		}
		else
		{
			return 0;
		}
	}
	size_t findPair(const STRING &w1, const STRING &w2) const
	{
		if( w1 == w2 )
		{
			return m_index.no_index;
		}
		const AiIndex *wi1 = m_index.findElement(AiIndex(w1));
		if( wi1 )
		{
			const AiIndex *wi2 = m_index.findElement(AiIndex(w2));
			if( wi2 )
			{
				Set<size_t> is = intersect(wi1->positions, wi2->positions);
				for(
					Set<size_t>::const_iterator it = is.cbegin(), endIT = is.cend();
					it != endIT;
					++it
				)
				{
					const AiNode &node = m_knowledge[*it];
					if( node.words.size() == 2 )
					{
						return *it;
					}
				}
			}
		}

		return m_index.no_index;
	}

	void addPair(const STRING &w1, const STRING &w2)
	{
		if( w1 == w2 )
		{
			return;
		}
		size_t	existing = findPair(w1,w2);
		if( existing != m_knowledge.no_index )
		{
			AiNode &node = m_knowledge[existing];
			++node.count;
		}
		else
		{
			createPair(w1,w2);
		}
	}

	void toBinaryStream ( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, m_knowledge );
		gak::toBinaryStream( stream, m_index );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream(stream, &m_knowledge );
		gak::fromBinaryStream(stream, &m_index );
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

} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_AI_BRAIN_H