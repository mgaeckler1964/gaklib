/*
		Project:		GAKLIB
		Module:			SortedListTest.H
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
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/sortedList.h>

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

struct MySortedListEntry : public SortedListEntry
{
	enum SortOrder
	{
		soNAME, soAGE
	};

	char *name;
	int age;

	MySortedListEntry(char *name, int age ) : name(name), age(age) {};

	int compare( const SortedListEntry *sortEntry, void *sortOrder )
	{
		SortOrder so = *(SortOrder*)(sortOrder);
		if( so == soNAME )
			return strcmp( name, ((MySortedListEntry*)sortEntry)->name );
		else if( age < ((MySortedListEntry*)sortEntry)->age )
			return -1;
		else if( age == ((MySortedListEntry*)sortEntry)->age )
			return 0;
		else
			return 1;
	}
};

class SortedListTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "SortedListTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "SortedListTest::PerformTest");
		TestScope scope( "PerformTest" );

		MySortedListEntry	*martin = new MySortedListEntry( "Martin", 62 );
		MySortedListEntry	*judith = new MySortedListEntry( "Judith", 36 );
		MySortedListEntry	*nadja = new MySortedListEntry( "Nadja", 33 );

		SortedList	myList;

		myList.addElement( martin );
		myList.addElement( nadja );
		myList.addElement( judith );

		{
			ListCursor	crs;
			MySortedListEntry *elem = (MySortedListEntry *)myList.getFirst( &crs );
			UT_EXPECT_EQUAL( elem->name, martin->name );
			UT_EXPECT_EQUAL( elem->age, martin->age );

			elem = (MySortedListEntry *)myList.getNext( &crs );
			UT_EXPECT_EQUAL( elem->name, nadja->name );
			UT_EXPECT_EQUAL( elem->age, nadja->age );

			elem = (MySortedListEntry *)myList.getNext( &crs );
			UT_EXPECT_EQUAL( elem->name, judith->name );
			UT_EXPECT_EQUAL( elem->age, judith->age );
		}

		MySortedListEntry::SortOrder so = MySortedListEntry::soNAME;
		myList.sort( &so );

		{
			ListCursor	crs;
			MySortedListEntry *elem = (MySortedListEntry *)myList.getFirst( &crs );
			UT_EXPECT_EQUAL( elem->name, judith->name );
			UT_EXPECT_EQUAL( elem->age, judith->age );

			elem = (MySortedListEntry *)myList.getNext( &crs );
			UT_EXPECT_EQUAL( elem->name, martin->name );
			UT_EXPECT_EQUAL( elem->age, martin->age );

			elem = (MySortedListEntry *)myList.getNext( &crs );
			UT_EXPECT_EQUAL( elem->name, nadja->name );
			UT_EXPECT_EQUAL( elem->age, nadja->age );
		}

		so = MySortedListEntry::soAGE;
		myList.sort( &so );
		{
			ListCursor	crs;
			MySortedListEntry *elem = (MySortedListEntry *)myList.getFirst( &crs );
			UT_EXPECT_EQUAL( elem->name, nadja->name );
			UT_EXPECT_EQUAL( elem->age, nadja->age );

			elem = (MySortedListEntry *)myList.getNext( &crs );
			UT_EXPECT_EQUAL( elem->name, judith->name );
			UT_EXPECT_EQUAL( elem->age, judith->age );

			elem = (MySortedListEntry *)myList.getNext( &crs );
			UT_EXPECT_EQUAL( elem->name, martin->name );
			UT_EXPECT_EQUAL( elem->age, martin->age );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static SortedListTest mySortedListTest;

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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
