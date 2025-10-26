/*
		Project:		GAKLIB
		Module:			aiBrain.h
		Description:	The brain for the AI
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
#include <gak/indexer.h>

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
namespace ai
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

struct Node
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

struct BrainIndexEntry
{
	STRING			word;
	Set<size_t>		positions;

	BrainIndexEntry() {}
	BrainIndexEntry(const STRING &word) : word(word) {}
	int compare( const BrainIndexEntry &e2 ) const
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

struct CountIdxEntry
{
	const Node *node;

	CountIdxEntry(const Node *node) : node(node) {}

	int compare( const CountIdxEntry &e2 ) const
	{
		int result = gak::compare( node->count, e2.node->count );
		if( !result )
		{
			result = gak::compare( node->words.size(), e2.node->words.size() );
		}
		return result;
	}
};

typedef Btree<CountIdxEntry>	BrainCountIdx;
typedef Btree<BrainIndexEntry>	BrainIndex;

class Brain
{
	Array<Node>		m_knowledge;
	BrainIndex		m_index;

	void createWordIndex(const STRING &w, size_t newPosition);
	void createPair(const STRING &w1, const STRING &w2, size_t count);

	public:
	size_t size() const
	{
		return m_knowledge.size();
	}
	size_t getPairCount(const STRING &w1, const STRING &w2) const;
	size_t findPair(const STRING &w1, const STRING &w2) const;
	Set<STRING> getPartners( const STRING &word ) const;
	void addPair(const STRING &w1, const STRING &w2, size_t count=1);
	void learnFromIndex( const StringIndex &source, size_t numWords );
	void learnFromTokens( const STRING &source, const StringTokens &tokens, size_t numWords );

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
	void clear()
	{
		m_knowledge.clear();
		m_index.clear();
	}

	void getCounterArray(BrainCountIdx *idx) const;
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

} // namespace ai
} // namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_AI_BRAIN_H