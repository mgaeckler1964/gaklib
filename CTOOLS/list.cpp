/*
		Project:		GAKLIB
		Module:			LIST.CPP
		Description:	The linked list implementation
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <cassert>
#include <limits>

#include <gak/gaklib.h>
#include <gak/list.h>
#include <gak/math.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#if __BORLANDC__ >= 0x620 || defined( __GNUC__ )
using namespace std;
#endif


namespace gak
{

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

void ListEntry::appendObject( ListEntry *newObj )
{
	assert( !newObj->m_next );
	assert( !newObj->m_prev );

	if( m_next )
	{
		m_next->m_prev = newObj;
		newObj->m_next = m_next;
	}
	m_next = newObj;
	newObj->m_prev = this;
}

inline void ListEntry::remove( void )
{
	if( m_prev )
	{
		m_prev->m_next = m_next;
	}
	if( m_next )
	{
		m_next->m_prev = m_prev;
	}
	m_prev = NULL;
	m_next = NULL;
}

inline void ListEntry::putFirstObject( ListEntry *first )
{
	if( m_prev )
	{
		// remove from chain
		remove();

		// put ourself as first
		m_next = first;
		first->m_prev = this;
	}
}

inline void ListEntry::putLastObject( ListEntry *last )
{
	if( m_next )
	{
		// remove from chain
		remove();

        // put ourself as last
		m_prev = last;
		last->m_next = this;
	}
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

ListEntry::~ListEntry()
{
	remove();
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

bool ListEntry::isVisible( void ) const
{
	return true;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

ListEntry *ListContainer::getEntry(	
	size_t		num,
	ListCursor	*cursor 
) const
{
	ssize_t			fromStart,
					fromEnd,
					fromCurrent,
					minDistance;
	ListEntry		*point;
	size_t			currentPos = cursor->getPos();

	if( num == currentPos )
	{
/***/	return cursor->getCurrent();
	}
	if( num >= size() )
	{
/*@*/	return NULL;
	}

	// calculate the distance from each starting point
	fromStart   = num;
	fromEnd     = num - (size() - 1);
	fromCurrent = cursor->isValid() ? num - currentPos : std::numeric_limits<ssize_t>::max();

	// calculate the minimun distance
	minDistance = std::numeric_limits<ssize_t>::max();
	minDistance = math::min( fromStart,                minDistance );
	minDistance = math::min( -fromEnd,                 minDistance );
	minDistance = math::min( math::abs( fromCurrent ), minDistance );	// ignore the sign

	if( minDistance == fromStart )
	{
		point = m_first;
	}
	else if( minDistance == -fromEnd )
	{
		point = m_last;
		minDistance = fromEnd;								// now we need the
															// sign
	}
	else
	{
		point = cursor->getCurrent();
		minDistance = fromCurrent;							// now we need the
															// sign
	}

	if( minDistance < 0 )									// search backward
	{
		while( minDistance++ )
		{
			point = point->getPrev();
			if( !point )
				break;
		}
	}
	else if( minDistance > 0 )								//search forward
	{
		while( minDistance-- )
		{
			point = point->getNext();
			if( !point )
				break;
		}
	}

	cursor->setPosition( point, num );
	if( !point )
	{
		cursor->invalidate();
	}

	return point;
}

ListEntry *ListContainer::getFilteredEntry(	
	size_t		num,
	ListCursor	*cursor
)
{
	ssize_t			fromStart,
					fromCurrent,
					minDistance;
	ListEntry		*point;
	size_t			currentPos = cursor->getPos();

	if( num == currentPos )
	{
/***/	return cursor->getCurrent();
	}
	if( num >= size() )
	{
/*@*/	return NULL;
	}

	// calculate the distance from each starting point
	fromStart   = num;
	fromCurrent = cursor->isValid() ? num - currentPos : std::numeric_limits<ssize_t>::max();

	// calculate the minimun distance
	minDistance = std::numeric_limits<ssize_t>::max();
	minDistance = math::min( fromStart,                minDistance );
	minDistance = math::min( math::abs( fromCurrent ), minDistance );	// ignore the sign

	if( minDistance == fromStart )
	{
		point = m_first;
		while( point && !point->isVisible() )				// search forward
			point = point->getNext();						// first visible
	}
	else
	{
		point = cursor->getCurrent();
		minDistance = fromCurrent;							// now we need the
															// sign
	}

	if( !point )
/*@*/	return NULL;

	if( minDistance < 0 )									// search backward
	{
		while( minDistance++ )
		{
			point = point->getPrev();
			if( !point )
				break;

			if( !point->isVisible() )
				minDistance--;
		}
	}
	else if( minDistance > 0 )								//search forward
	{
		while( minDistance-- )
		{
			point = point->getNext();
			if( !point )
				break;
			if( !point->isVisible() )
				minDistance++;
		}
	}

	cursor->setPosition( point, num );
	if( !point )
	{
		cursor->invalidate();
	}

	return point;
}

long ListContainer::getNumVisibleEntries( void ) const
{
	long 			num = 0;
	ListEntry		*point = m_first;

	while( point )
	{
		if( point->isVisible() )
			num++;
		point = point->getNext();
	}

	return num;
}

void ListContainer::addElement( ListEntry *newObj )
{
	if( m_shutdown )
	{
/***/	return;
	}

	if( !m_last )
	{
		m_first = m_last = newObj;
	}
	else
	{
		m_last->appendObject( newObj );
		m_last = newObj;
	}
	incNumElements();
}

void ListContainer::removeObject( ListEntry *obj )
{
	if( m_shutdown )
	{
/***/	return;
	}

	assert( obj );

	if( obj == m_first )
	{
		m_first = obj->getNext();
	}
	if( obj == m_last )
	{
		m_last = obj->getPrev();
	}

	obj->remove();
	decNumElements();
}

void ListContainer::clear( void )
{
	doEnterFunctionEx(gakLogging::llDetail, "ListContainer::clear" );
	ListEntry		*entry,
					*next;

	m_shutdown = true;

	entry = m_first;
	while( entry )
	{
		next = entry->m_next;
		delete entry;
		entry = next;
	}

	forget();
}

/*
	Warning
	-------

	the following two functions invalidate all search buffers pointing to the
	container.
*/

void ListContainer::putFirstObject( ListEntry *obj )
{
	if( m_shutdown )
	{
/***/	return;
	}

	assert( obj );

	obj->putFirstObject( m_first );
	m_first = obj;
}

void ListContainer::putLastObject( ListEntry *obj )
{
	if( m_shutdown )
	{
/***/	return;
	}

	assert( obj );

	obj->putLastObject( m_last );
	m_last = obj;
}

}	// namespace gak

