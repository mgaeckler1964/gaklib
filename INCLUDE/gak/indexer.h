/*
		Project:		GAKLIB
		Module:			indexer.h
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

#ifndef GAK_INDEXER_H
#define GAK_INDEXER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cstddef>

#include <gak/utils.h>
#include <gak/array.h>
#include <gak/string.h>
#include <gak/map.h>
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

#ifdef __BORLANDC__
#	pragma warn -inl
#endif
inline STRING makeFuzzyIndex( const STRING &fuzzy )
{
	return ' ' + fuzzy;
}

inline STRING makeLowerIndex( const STRING &lowerWord )
{
	return lowerWord + ' ';
}
#ifdef __BORLANDC__
#	pragma warn +inl
#endif

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //
struct Position
{
	std::size_t	m_start, 
				m_len;

	Position()
	{
	}
	Position(std::size_t start, std::size_t len)
	{
		m_start = start;
		m_len = len;
	}
	int compare( const Position &other ) const
	{
		int result = gak::compare( m_start, other.m_start );
		if( !result )
		{
			result = gak::compare( m_len, other.m_len );
		}
		return result;
	}
};

template <>
struct is_binary<Position> : public internal::integral_constant<true>
{
};

template <>
inline void toBinaryStream( std::ostream &stream, const Position &value )
{
	binaryToBinaryStream( stream, value );
}

template <>
inline void fromBinaryStream( std::istream &stream, Position *value )
{
	binaryFromBinaryStream( stream, value );
}

typedef Set<Position, FixedComparator<Position>, PODallocator<Position> >
									Positions;
typedef PairMap<STRING,Positions>	StringIndex;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct StatistikEntry
{
	std::size_t	m_count;
	STRING		m_word;

	StatistikEntry( std::size_t	count=0, const STRING &word=STRING() )
	: m_count(count), m_word( word )
	{
	}
	int compare( const StatistikEntry &other ) const
	{
		int result = gak::compare( other.m_count, m_count );
		if( !result )
		{
			result = gak::compare( m_word, other.m_word );
		}

		return result;
	}
};

typedef SortedArray<StatistikEntry>	StatistikData;

template<typename SourceT>
class Index
{
	public:
	struct HitRelevance
	{
		SourceT		m_source;
		Positions	m_positions;
		double		m_relevance;

		int compare( const HitRelevance &other ) const
		{
			return gak::compare( other.m_positions.size(), m_positions.size() );
		}
		void moveFrom( HitRelevance &other )
		{
			this->m_source = other.m_source;
			this->m_positions.moveFrom( other.m_positions );
			this->m_relevance = other.m_relevance;
		}
	};

	typedef SortedArray<HitRelevance>		RelevantHits;
	typedef PairMap<SourceT, Positions>		SearchResult;

	private:
	typedef PairMap<STRING, SearchResult>	SearchIndex;

	private:
	SearchIndex	m_searchIndex;


	private:
	void mergeWordHits( SearchResult *target, const SearchResult &source );
	void intersect(SearchResult *target, const SearchResult &source);
	void substract(SearchResult *target, const SearchResult &source);

	SearchResult findWord( const STRING &word )
	{
		SearchResult	result;

		std::size_t		index;
		if( (index = m_searchIndex.getElementIndex( word )) != m_searchIndex.no_index )
		{
			result = m_searchIndex.getElementAt(index).getValue();
		}
		return result;
	}
	SearchResult findPattern( const Array<STRING> &keys, const STRING &pattern );

	SearchResult findFuzzyWord( const STRING &word )
	{
		STRING			fuzyWord = word.simplify();
		SearchResult	result = findWord( fuzyWord );

		mergeWordHits(
			&result,
			findWord( makeFuzzyIndex( fuzyWord ) )
		);

		return result;
	}
	SearchResult findFuzzyPattern( const Array<STRING> &keys, const STRING &pattern )
	{
		STRING			fuzyPattern = pattern.simplify();
		SearchResult	result = findPattern( keys, fuzyPattern );

		mergeWordHits(
			&result,
			findPattern( keys, makeFuzzyIndex( fuzyPattern ) )
		);

		return result;
	}
	SearchResult findLowerWord( const STRING &word )
	{
		STRING			lowerWord = word.lowerCaseCopy();
		SearchResult	result = findWord( lowerWord );

		mergeWordHits(
			&result,
			findWord( makeLowerIndex( lowerWord ) )
		);

		return result;
	}
	SearchResult findLowerPattern(
		const Array<STRING> &keys, const STRING &pattern
	)
	{
		STRING			lowerPattern = pattern.lowerCaseCopy();
		SearchResult	result = findPattern( keys, lowerPattern );

		mergeWordHits(
			&result,
			findPattern( keys, makeLowerIndex( lowerPattern ) )
		);

		return result;
	}
	SearchResult findWord(
		const STRING &word, bool withFuzzy, bool caseInsensitive
	)
	{
		SearchResult result = findWord( word );
		if( withFuzzy )
		{
			mergeWordHits( &result, findFuzzyWord( word ) );
		}
		if( caseInsensitive )
		{
			mergeWordHits( &result, findLowerWord( word ) );
		}
		return result;
	}
	SearchResult findPattern(
		const Array<STRING> &keys, const STRING &pattern,
		bool withFuzzy, bool caseInsensitive
	)
	{
		SearchResult  result = findPattern( keys, pattern );
		if( withFuzzy )
		{
			mergeWordHits( &result, findFuzzyPattern( keys, pattern ) );
		}
		if( caseInsensitive )
		{
			mergeWordHits( &result, findLowerPattern( keys, pattern ) );
		}

		return result;
	}

	SearchResult findWords(
		const ArrayOfStrings &wordList, bool withFuzzy, bool caseInsensitive
	);
	SearchResult findPatterns(
		const ArrayOfStrings &patternList, bool withFuzzy, bool caseInsensitive
	);

	public:
	void mergeIndexPositions( const SourceT &source, const StringIndex &index );

	void toBinaryStream ( std::ostream &stream ) const
	{
		m_searchIndex.toBinaryStream ( stream );
	}
	void fromBinaryStream ( std::istream &stream )
	{
		m_searchIndex.fromBinaryStream ( stream );
	}

	const SearchResult &operator [] ( const STRING &text ) const
	{
		return m_searchIndex[text];
	}
	bool hasElement( const STRING &text ) const
	{
		return m_searchIndex.hasElement(text);
	}

	StatistikData getStatistik() const;
	SearchResult findWords(
		const STRING &words,
		bool withFuzzy, bool caseInsensitive, bool withWildcards
	);
	RelevantHits getRelevantHits( 
		const STRING &words,
		bool withFuzzy, bool caseInsensitive, bool withWildcards
	);
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

template<typename SourceT>
void Index<SourceT>::mergeWordHits( SearchResult *target, const SearchResult &source )
{
	for(
		typename SearchResult::const_iterator it = source.cbegin(), endIT = source.cend();
		it != endIT;
		++it
	)
	{
		Positions	&positions = (*target)[it->getKey()];
		positions.moveFrom(
			ref(
				unionSet(
					positions, it->getValue()
				)
			)
		);
	}
}

template<typename SourceT>
void Index<SourceT>::intersect(SearchResult *target, const SearchResult &source)
{
	for( std::size_t i = 0; i<target->size(); ++i )
	{
		const typename SearchResult::key_type	&srcKey = target->getKeyAt( i );
		if( source.hasElement(srcKey) )
		{
			Positions	&positions = target->getValueAt( i );
			positions.moveFrom(
				ref(
					unionSet(
						positions, source[srcKey]
					)
				)
			);
		}
		else
		{
			target->removeElementAt( i );
			--i;
		}
	}
}

template<typename SourceT>
void Index<SourceT>::substract(SearchResult *target, const SearchResult &source)
{
	for( std::size_t i = 0; i<target->size(); ++i )
	{
		const typename SearchResult::key_type	&srcKey = target->getKeyAt( i );
		if( source.hasElement(srcKey) )
		{
			target->removeElementAt( i );
			--i;
		}
	}
}

template<typename SourceT>
typename Index<SourceT>::SearchResult Index<SourceT>::findPattern(
	const Array<STRING> &keys, const STRING &pattern
)
{
	SearchResult	result;

	for(
		Array<STRING>::const_iterator it = keys.cbegin(),
			endIT = keys.cend();
		it != endIT;
		++it
	)
	{
		if( it->match( pattern ) )
		{
			mergeWordHits( &result, findWord( *it ) );
		}
	}
	return result;
}

template<typename SourceT>
typename Index<SourceT>::SearchResult Index<SourceT>::findWords(
	const ArrayOfStrings &wordList,
	bool withFuzzy, bool caseInsensitive
)
{
	SearchResult	result;

	for(
		ArrayOfStrings::const_iterator it = wordList.cbegin(),
			endIT = wordList.cend();
		it != endIT;
		++it
	)
	{
		const STRING	&word = *it;

		if( word[0U] == '+' )
		{
			intersect(
				&result, findWord(
					word+std::size_t(1), withFuzzy, caseInsensitive
				)
			);
		}
		else if( word[0U] == '-' )
		{
			substract(
				&result, findWord(
					word+std::size_t(1), withFuzzy, caseInsensitive
				)
			);
		}
		else
		{
			mergeWordHits( &result, findWord( word, withFuzzy, caseInsensitive ) );
		}
	}
	return result;
}

template<typename SourceT>
typename Index<SourceT>::SearchResult Index<SourceT>::findPatterns(
	const ArrayOfStrings &patternList,
	bool withFuzzy, bool caseInsensitive
)
{
	SearchResult	result;
	Array<STRING>	keys = m_searchIndex.getKeys();

	for(
		ArrayOfStrings::const_iterator it = patternList.cbegin(),
			endIT = patternList.cend();
		it != endIT;
		++it
	)
	{
		const STRING	&pattern = *it;

		if( pattern[0U] == '+' )
		{
			intersect(
				&result, findPattern(
					keys, pattern+std::size_t(1),
					withFuzzy, caseInsensitive
				)
			);
		}
		else if( pattern[0U] == '-' )
		{
			substract(
				&result, findPattern(
					keys, pattern+std::size_t(1),
					withFuzzy, caseInsensitive
				)
			);
		}
		else
		{
			result.addElements(
				findPattern( keys, pattern, withFuzzy, caseInsensitive )
			);
		}
	}
	return result;
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

template<typename SourceT>
StatistikData Index<SourceT>::getStatistik() const
{
	StatistikData	result;

	for(
		typename SearchIndex::const_iterator it = m_searchIndex.cbegin(),
			endIT = m_searchIndex.cend();
		it != endIT;
		++it
	)
	{
		std::size_t			count = 0;
		const STRING		&word = it->getKey();
		const SearchResult	&indexPositions = it->getValue();
		for(
			typename SearchResult::const_iterator it = indexPositions.cbegin(),
				endIT = indexPositions.cend();
			it != endIT;
			++it
		)
		{
			count += it->getValue().size();
		}
		result.addElement( StatistikEntry( count, word ) );
	}
	return result;
}

template<typename SourceT>
void Index<SourceT>::mergeIndexPositions( const SourceT &source, const StringIndex &index )
{
	doEnterFunction("SourcePosition::mergeIndexPositions");
	m_searchIndex.setChunkSize( index.size() );

	for(
		StringIndex::const_iterator it = index.cbegin(), endIT = index.cend();
		it != endIT;
		++it
	)
	{
		SearchResult &sourceIndexPos = m_searchIndex[it->getKey()];
		sourceIndexPos.setChunkSize(sourceIndexPos.size()/2);
		sourceIndexPos[source].moveFrom( const_cast<Positions&>(it->getValue()) );
	}
}

template<typename SourceT>
typename Index<SourceT>::SearchResult Index<SourceT>::findWords(
	const STRING &words,
	bool withFuzzy, bool caseInsensitive, bool withWildcards
)
{
	ArrayOfStrings wordList;
	wordList.createElements( words );

	if( !withWildcards )
	{
		return findWords( wordList, withFuzzy, caseInsensitive );
	}
	else
	{
		return findPatterns( wordList, withFuzzy, caseInsensitive );
	}
}

template<typename SourceT>
typename Index<SourceT>::RelevantHits Index<SourceT>::getRelevantHits(
	const STRING &words,
	bool withFuzzy, bool caseInsensitive, bool withWildcards
)
{
	SearchResult	seachResult = findWords( words, withFuzzy,caseInsensitive, withWildcards );
	double			totalCount = 0;
	HitRelevance	hit;
	RelevantHits	result;
	for(
		typename SearchResult::iterator it = seachResult.begin(), endIT = seachResult.end();
		it != endIT;
		++it
	)
	{
		hit.m_source = it->getKey();
		hit.m_positions.moveFrom( const_cast<Positions&>(it->getValue()) );
		totalCount += hit.m_positions.size();
		result.addElement( hit );
	}
	for(
		typename RelevantHits::iterator it = result.begin(), endIT = result.end();
		it != endIT;
		++it
	)
	{
		const_cast<HitRelevance&>(*it).m_relevance = double(it->m_positions.size())/totalCount;
	}
	return result;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

inline std::ostream &operator << ( std::ostream &out, const StatistikEntry &entry )
{
	out << entry.m_word << '\t' << entry.m_count;

	return out;
}
template<typename StringT, typename StringsT>
StringIndex indexString( const StringT &string, const StringsT &stopWords )
{
	StringIndex							positions;
	char								c;
	bool								hasLetter = false;
	STRING								lastWord, word, text;
	std::size_t							i, wordPosition, textPosition;
	typename StringT::const_iterator	it;

	for(
		it = string.cbegin(), i=0;
		(c=*it) != 0 || word.strlen() || text.strlen() ;
		++i, ++it
	)
	{
		if( isLetter( c ) )
		{
			if( word.isEmpty() )
			{
				wordPosition = i;
				hasLetter = true;
			}
			word += c;
		}
		else if( !word.isEmpty() )
		{
			if( !stopWords.hasElement( word ) )
			{
				Positions	&wordPositions = positions[word];
				wordPositions.addElement(Position(wordPosition, word.strlen()));

				CI_STRING	simple = word.simplify();
				if( simple != word )
				{
					Positions	&wordPositions = positions[makeFuzzyIndex(simple)];
					wordPositions.addElement(Position(wordPosition, word.strlen()));
				}
				STRING	lower = word.lowerCaseCopy();
				if( lower != word )
				{
					Positions	&wordPositions = positions[makeLowerIndex(lower)];
					wordPositions.addElement(Position(wordPosition, word.strlen()));
				}
			}
			lastWord = word;
			word = NULL_STRING;
		}

		if( !isControlOrSpace( c ) )
		{
			if( text.isEmpty() )
			{
				textPosition = i;
			}
			text += c;
		}
		else if( !text.isEmpty() )
		{
			if( hasLetter && text != lastWord && !stopWords.hasElement( text ) )
			{
				Positions	&textPositions = positions[text];
				textPositions.addElement(Position(textPosition, text.strlen()));
			}
			text = NULL_STRING;
			hasLetter = false;
		}

		if( !c )
		{
/*v*/		break;
		}
	}	// for( size_t i=0; (c=string[i]) != 0; ++i )

	return positions;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_INDEXER_H
