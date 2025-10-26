/*
		Project:		GAKLIB
		Module:			indexer.h
		Description:	Indexer for any strings, and search in these indices
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

#ifndef GAK_INDEXER_H
#define GAK_INDEXER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef USE_PAIR_MAP
#define USE_PAIR_MAP	1		// for searching pair map is better than TreeMap that is better for indexing
#endif

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
namespace ai
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const uint16 IS_WORD	= 0x01;
static const uint16 IS_TEXT	= 0x02;
static const uint16 IS_IDENT= 0x04;

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

inline bool isSpecialIndex( const STRING &index )
{
	return index.size() > 1 && (index[0U] == ' ' || index[index.size()-1] == ' ');
}
#ifdef __BORLANDC__
#	pragma warn +inl
#endif

/// TODO PairMap has performance problems with large database. Analyse whether we should remove the option to use a pairmap
#if USE_PAIR_MAP
#define IndexerMap PairMap
#else
#define IndexerMap TreeMap
#endif

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //
struct Position
{
	std::size_t	m_start, 
				m_len;
	uint16		m_flags;

	Position(std::size_t start=0, std::size_t len=0, uint16 flags=0 )
	{
		m_start = start;
		m_len = len;
		m_flags = flags;
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

}	// namespace ai

template <>
struct is_binary<ai::Position> : public internal::integral_constant<true>
{
};

template <>
inline void toBinaryStream( std::ostream &stream, const ai::Position &value )
{
	binaryToBinaryStream( stream, value );
}

template <>
inline void fromBinaryStream( std::istream &stream, ai::Position *value )
{
	binaryFromBinaryStream( stream, value );
}

namespace ai
{

typedef Set<Position, FixedComparator<Position>, PODallocator<Position> >
									Positions;
typedef IndexerMap<STRING,Positions>	StringIndex;

typedef Array<Position>	StringTokens;

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

typedef Btree<StatistikEntry>	StatistikData;

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

	typedef SortedArray<HitRelevance>			RelevantHits;
	typedef IndexerMap<SourceT, Positions>		SearchResult;

	private:
	typedef IndexerMap<STRING, SearchResult>	SearchIndex;

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
	size_t size() const
	{
		return m_searchIndex.size();
	}
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

	void clear()
	{
		m_searchIndex.clear();
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
	doEnterFunctionEx(gakLogging::llInfo, "Index<SourceT>::getStatistik" );
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
	doEnterFunctionEx(gakLogging::llDetail,"SourcePosition::mergeIndexPositions");
#if USE_PAIR_MAP
	m_searchIndex.setChunkSize( index.size() );
#endif
	for(
		StringIndex::const_iterator it = index.cbegin(), endIT = index.cend();
		it != endIT;
		++it
	)
	{
		SearchResult &sourceIndexPos = m_searchIndex[it->getKey()];
#if USE_PAIR_MAP
		sourceIndexPos.setChunkSize(sourceIndexPos.size()/2);
#endif
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

template<typename StringT>
bool isIdentifierChar( const StringT &identifier, char c )
{
	if( isLetter( c ) 
	|| c == '_' 
	|| (isDigit( c ) && identifier.size()) )
		return true;
	return false;
}

template<typename StringT, typename StringsT>
StringTokens tokenString( const StringT &string, const StringsT &stopWords )
{
	StringTokens						positions;
	char								c;
	size_t								letterCount = 0;
	STRING								word, lastWord, identifier, lastIdentifier, text;
	std::size_t							i, wordPosition, identifierPosition, textPosition;
	typename StringT::const_iterator	it;

	for(
		it = string.cbegin(), i=0;
		(c=*it) != 0 || word.size() || text.size();
		++i, ++it
	)
	{
		if( isLetter( c ) )
		{
			if( word.isEmpty() )
			{
				wordPosition = i;
			}
			letterCount++;
			word += c;
		}
		else if( !word.isEmpty() )
		{
			size_t len = word.size();
			if( len>1							// not a single charater			
			&& !stopWords.hasElement( word ))	// not in stopWords
			{
				positions.addElement( Position(wordPosition,len, IS_WORD) );
			}
			lastWord = word;
			word = NULL_STRING;
		}

		if( isIdentifierChar( identifier, c ) )
		{
			if( identifier.isEmpty() )
			{
				identifierPosition = i;
			}
			identifier += c;
		}
		else if( !identifier.isEmpty() )
		{
			size_t len=identifier.strlen();
			if( len>1								// not a single charater			
			&& identifier != lastWord				// new identifier
			&& !stopWords.hasElement( identifier )	// not in stopWords
			&& letterCount > 0 )					// at least one letter
			{
				positions.addElement(Position(identifierPosition, len, IS_IDENT));
			}
			lastIdentifier = identifier;
			identifier = NULL_STRING;
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
			size_t len=text.strlen();
			if( len>1							// not a single charater			
			&& text != lastWord					// new text
			&& text != lastIdentifier			// new text
			&& !stopWords.hasElement( text )	// not in stopWords
			&& letterCount > 0					// at least one letter
			&& (len<5 || letterCount>len/2))	// less than 5 characters or more letters than other characters
			{
				positions.addElement(Position(textPosition, len, IS_TEXT));
			}
			text = NULL_STRING;
			letterCount = 0;
		}

		if( !c )
		{
/*v*/		break;
		}
	}	// for( size_t i=0; (c=string[i]) != 0; ++i )

	return positions;
}

template<typename StringT>
StringIndex processPositions( const StringT &string, const StringTokens &tokens )
{
	StringIndex positions;

	for(
		StringTokens::const_iterator it = tokens.cbegin(), endIT = tokens.cend();
		it != endIT;
		++it
	)
	{
		const Position  &pos = *it;
		STRING word = string.subString( pos.m_start, pos.m_len );

		if( pos.m_flags&(IS_WORD|IS_IDENT) )
		{
			CI_STRING	simple = word.simplify();
			if( simple.strlen() < 2 )
			{
				// skip too simple words
				continue;
			}
			if( simple != word )
			{
				positions[makeFuzzyIndex(simple)].addElement(pos);
			}
			STRING	lower = word.lowerCaseCopy();
			if( lower != word )
			{
				positions[makeLowerIndex(lower)].addElement(pos);
			}
		}
		positions[word].addElement(pos);
	}
	return positions;
}

template<typename StringT, typename StringsT>
StringIndex indexString( const StringT &string, const StringsT &stopWords )
{
	return processPositions( string, tokenString( string, stopWords ) );
}

}	// namespace ai
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_INDEXER_H
