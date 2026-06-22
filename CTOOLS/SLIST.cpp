/*
		Project:		GAKLIB
		Module:			SLIST.CPP
		Description:	Sorted linked list
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <assert.h>
#include <gak/slist.h>
#include <gak/stdlib.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static void *sortOrder;

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static int compare( const void *entry1, const void *entry2 )
{
	SortedListEntry	*e1 = *(SortedListEntry **)entry1;
	SortedListEntry	*e2 = *(SortedListEntry **)entry2;
	
	return e1->compare( e2, sortOrder );
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void SortedListContainer::sort( void *sortOrder )
{
	const size_t	numEntry = size();
	Buffer<SortedListEntry*>	array( sizeof( SortedListEntry * ) * numEntry );
	if( array )
	{
		// create the temporary array
		size_t i = 0;
		while( 1 )
		{
			SortedListEntry	*object = (SortedListEntry *)getFirstItem();
			if( !object )
/*v*/			break;

			removeObject( object );

			assert( i<numEntry );

			array[i] = object;
			i++;
		}
		
		// sort the array
		gak::sortOrder = sortOrder;
		qsort( array, numEntry, sizeof( SortedListEntry * ), compare );
		
		// create the new list
		ListCursor	cursor;
		for( i=0; i<numEntry; i++ )
			insertObject( &cursor, array[i], false );
	}
}

}	// namespace gak

