/*
		Project:		GAKLIB
		Module:			aiBrain.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/aiBrain.h>
#include <gak/sortedArray.h>
#include <gak/math.h>

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

struct WordUsage
{
	STRING	word;
	size_t	count;
	WordUsage( const STRING &word="", size_t count=0 ) : word(word), count(count) {}

	int compare(const WordUsage &other ) const
	{
		int compareResult = gak::compare( count, other.count );
		if( !compareResult )
		{
			compareResult = gak::compare( word, other.word );
		}

		return compareResult;
	}
};

typedef SortedArray<WordUsage> WordUsageCounter;

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

static WordUsageCounter checkWordUsage( const StringIndex &index )
{
	WordUsageCounter	counter;
	for(
		StringIndex::const_iterator it = index.cbegin(), endIT = index.cend();
		it != endIT;
		++it
	)
	{
		const STRING &word = it->getKey();
		if( !word.beginsWith( ' ' ) && !word.endsWith( ' ' ) )
		{
			WordUsage wu(word, it->getValue().size() );
			counter.addElement(wu);
		}
	}
	return counter;
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

void AiBrain::createWordIndex(const STRING &w, size_t newPosition)
{
	AiIndex *wi = const_cast<AiIndex *>(m_index.findElement(AiIndex(w)));
	if( wi )
	{
		wi->positions.addElement(newPosition);
	}
	else
	{
		AiIndex wi(w);
		wi.positions.addElement(newPosition);
		m_index.addElement(wi);
	}

}

void AiBrain::createPair(const STRING &w1, const STRING &w2, size_t count)
{
	size_t	newPosition = m_knowledge.size();
	AiNode &newNode = m_knowledge.createElement();
	newNode.words.addElement(w1);
	newNode.words.addElement(w2);
	newNode.count = count;

	createWordIndex( w1, newPosition );
	createWordIndex( w2, newPosition );
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

size_t AiBrain::getPairCount(const STRING &w1, const STRING &w2) const
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

size_t AiBrain::findPair(const STRING &w1, const STRING &w2) const
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

Set<STRING> AiBrain::getPartners( const STRING &word ) const
{
	Set<STRING>	result;

	const AiIndex *wi = m_index.findElement(AiIndex(word));
	if( wi )
	{
		for(
			Set<size_t>::const_iterator it = wi->positions.cbegin(), endIT = wi->positions.cend();
			it != endIT;
			++it
		)
		{
			const AiNode &node = m_knowledge[*it];
			if( node.words.size() == 2 )
			{
				if( node.words[0] != word )
				{
					result.addElement(node.words[0]);
				}
				else
				{
					result.addElement(node.words[1]);
				}
			}
		}
	}
	return result;
}

void AiBrain::addPair(const STRING &w1, const STRING &w2, size_t count)
{
	if( w1 == w2 )
	{
		return;
	}
	size_t	existing = findPair(w1,w2);
	if( existing != m_knowledge.no_index )
	{
		AiNode &node = m_knowledge[existing];
		node.count += count;
	}
	else
	{
		createPair(w1,w2, count);
	}
}

void AiBrain::learnFromIndex( const StringIndex &source, size_t numWords )
{
	if( source.size() < 2 )
	{
		return;				// index too small
	}
	WordUsageCounter	counter = checkWordUsage( source );

	/// TODO: Better learning would be to combine 2 words with a smaller distance also avoid to combine fuzzy versions of the string

	if( counter.size() > numWords )
	{
		size_t toRemove = counter.size() - numWords;
		size_t	startIndex = (counter.size()>>1) - (toRemove>>1);
		counter.removeElementsAt(startIndex, toRemove );
	}
	for( size_t i=0; i<counter.size()-1; ++i )
	{
		const WordUsage &wu1 = counter[i];
		for( size_t j=0; j<counter.size(); ++j )
		{
			const WordUsage &wu2 = counter[j];
			size_t usage = math::min(wu1.count, wu2.count);
			addPair( wu1.word, wu2.word, usage );
		}
	}
}

void AiBrain::learnFromTokens( const STRING &source, const StringTokens &tokens, size_t numWords )
{
	if( tokens.size() < 2 )
	{
		return;				// index too small
	}

	for( size_t i=0; i<tokens.size()-1; ++i )
	{
		const Position &pos1 = tokens[i];
		STRING word1 = source.subString(pos1.m_start, pos1.m_len);
		size_t wordsToProcess = numWords;
		for( size_t j=i+1; j<tokens.size() && wordsToProcess; ++j )
		{
			const Position &pos2 = tokens[j];
			if( pos2.m_flags & IS_WORD )
			{
				STRING word2 = source.subString(pos2.m_start, pos2.m_len);
				if( word1 != word2 )
				{
					addPair( word1, word2 );
					--wordsToProcess;
				}
			}
		}
	}
}

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

