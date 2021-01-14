/*
		Project:		GAKLIB
		Module:			IostreamTest.h
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

#include <gak/iostream.h>

#include <gak/datetime.h>
#include <gak/map.h>
#include <gak/strFiles.h>

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

class IOstreamTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "IOstreamTest";
	}
	template <typename OBJ>
	void doTest( const OBJ &source )
	{
		const uint32	magic = 0x12345678;
		const F_STRING	fileName = "testFile.dmp";
		OBJ				target;

		writeToFile( fileName, source, magic );

		readFromFile( fileName, &target, magic );
		UT_ASSERT_EQUAL( source, target );

		readFromFile( fileName, &target, magic );
		UT_ASSERT_EQUAL( source, target );

		UT_ASSERT_EXCEPTION( readFromFile( fileName, &target, magic+1 ), BadHeaderError );

		{
			ofstream	str( fileName, owmRenameOld );
			toBinaryStream( str, source );
			str.flush();
			str.rollback();

			readFromFile( fileName, &target, magic );
			UT_ASSERT_EQUAL( source, target );
		}
		{
			ofstream	str( fileName, std::ios_base::binary, owmRenameOld );
			toBinaryStream( str, source );
			str.flush();
			str.rollback();

			readFromFile( fileName, &target, magic );
			UT_ASSERT_EQUAL( source, target );
		}
		{
			ofstream	str;
			str.open( fileName, owmRenameOld );
			toBinaryStream( str, source );
			str.flush();
		}
		{
			ofstream	str;
			str.open( fileName, std::ios_base::binary, owmRenameOld );
			toBinaryStream( str, source );
			str.flush();
		}
		readFromFile( fileName, &target, magic );
		UT_ASSERT_EQUAL( source, target );

		strRemove( fileName );
	}

	virtual void PerformTest( void )
	{
		doEnableLog();
		{
			TestScope("TreeMap<int, STRING>");
			TreeMap<int, STRING>	map;

			map[1234] = STRING("1234");
			map[0x815] = STRING("0x815");
			map[666] = STRING("666");
			map[4711] = STRING("4711");

			doTest( map );
		}
		{
			TestScope("PairMap<int, STRING>");
			PairMap<int, STRING>	map;

			map[1234] = STRING("1234");
			map[0x815] = STRING("0x815");
			map[666] = STRING("666");
			map[4711] = STRING("4711");

			doTest( map );
		}
		{
			TestScope("TreeMap<int, DateTime>");
			TreeMap<int, DateTime>	map;

			map[1234] = DateTime( time_t(1234) );
			map[0x815] = DateTime( time_t(0x815) );
			map[666] = DateTime( time_t(666) );
			map[4711] = DateTime( time_t(4711) );

			doTest( map );
		}
		{
			TestScope("PairMap<int, DateTime>");
			PairMap<int, DateTime>	map;

			map[1234] = DateTime( time_t(1234) );
			map[0x815] = DateTime( time_t(0x815) );
			map[666] = DateTime( time_t(666) );
			map[4711] = DateTime( time_t(4711) );

			doTest( map );
		}
		{
			TestScope("TreeMap<bool, DateTime>");
			TreeMap<bool, DateTime>	map;

			map[true] = DateTime( time_t(0x815) );
			map[false] = DateTime( time_t(4711) );

			doTest( map );
		}
		{
			TestScope("PairMap<bool, DateTime>");
			PairMap<bool, DateTime>	map;

			map[true] = DateTime( time_t(0x815) );
			map[false] = DateTime( time_t(4711) );

			doTest( map );
		}

		{
			TestScope("DirectoryList");
			DirectoryList	array;

			array.findFiles( ALL_FILES_PATTERN );

			doTest( array );
		}
		{
			TestScope("FixedArray<double, 2>");
			FixedArray<double, 2>	array;
			array[0] = 3.14;
			array[1] = 2.71;

			doTest( array );
		}
		{
			TestScope("FixedArray<STRING, 2>");
			FixedArray<STRING, 2>	array;
			array[0] = "3.14";
			array[1] = "2.71";

			doTest( array );
		}
		{
			TestScope("ArrayOfFloats");
			ArrayOfFloats	array;
			array[0] = 3.14;
			array[1] = 2.71;

			doTest( array );
		}
		{
			TestScope("Array<STRING>");
			Array<STRING>	array;
			array[0] = "3.14";
			array[1] = "2.71";

			doTest( array );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static IOstreamTest myIOstreamTest;

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

