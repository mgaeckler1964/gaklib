/*
		Project:		GAKLIB
		Module:			DateTimeTest.h
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

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

class DateTimeTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "DateTimeTest";
	}
	virtual void PerformTest( void )
	{
		DateTime	now;

		UT_ASSERT_EQUAL( now.getTZoffset(), 3600L );
		DateTime	epochBegin( time_t( 0 ) );
		UT_ASSERT_EQUAL( epochBegin.getYear(), (unsigned short)1970 );
		UT_ASSERT_EQUAL( epochBegin.getMonth(), Date::JANUARY );
		UT_ASSERT_EQUAL( int(epochBegin.getDay()), 1 );
		UT_ASSERT_EQUAL( int(epochBegin.getHour()), 0 );
		UT_ASSERT_EQUAL( int(epochBegin.getMinute()), 0 );
		UT_ASSERT_EQUAL( int(epochBegin.getSecond()), 0 );

		const time_t daySeconds = 24 * 3600;
		const time_t yearSeconds = 365 * daySeconds;
		const time_t leapYearSeconds = yearSeconds + daySeconds;

		DateTime	theDate1( time_t( yearSeconds*2 + leapYearSeconds + yearSeconds ) );
		UT_ASSERT_EQUAL( theDate1.getYear(), (unsigned short)1974 );
		UT_ASSERT_EQUAL( theDate1.getMonth(), Date::JANUARY );
		UT_ASSERT_EQUAL( int(theDate1.getDay()), 1 );
		UT_ASSERT_EQUAL( int(theDate1.getHour()), 0 );
		UT_ASSERT_EQUAL( int(theDate1.getMinute()), 0 );
		UT_ASSERT_EQUAL( int(theDate1.getSecond()), 0 );

		DateTime	theDate2( time_t( yearSeconds*2 + 31*daySeconds + 28*daySeconds + 14*3600 + 30 * 60 + 25 ) );
		UT_ASSERT_EQUAL( theDate2.getYear(), (unsigned short)1972 );
		UT_ASSERT_EQUAL( theDate2.getMonth(), Date::FEBRUARY );
		UT_ASSERT_EQUAL( int(theDate2.getDay()), 29 );
		UT_ASSERT_EQUAL( int(theDate2.getHour()), 14 );
		UT_ASSERT_EQUAL( int(theDate2.getMinute()), 30 );
		UT_ASSERT_EQUAL( int(theDate2.getSecond()), 25 );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static DateTimeTest	myDateTimeTest;

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

