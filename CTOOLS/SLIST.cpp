/*
		Project:		GAKLIB
		Module:			SLIST.CPP
		Description:	Sorted linked list
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <assert.h>
#include <gak/slist.h>

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
	SortedListContainer	tmpContainer;

	SortedListEntry		**array;
	SortedListEntry		*object;
	ListCursor			cursor;
	size_t				i, numEntry;

	numEntry = (size_t)size();
	array = (SortedListEntry **)malloc( sizeof( SortedListEntry * ) * numEntry );
	if( array )
	{
		// create the temporary array
		i = 0;
		while( 1 )
		{
			object = (SortedListEntry *)getFirstItem();
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
		for( i=0; i<numEntry; i++ )
			insertObject( &cursor, array[i], false );

		free( array );
	}
}

}	// namespace gak

