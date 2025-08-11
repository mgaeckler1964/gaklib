/*
		Project:		GAKLIB
		Module:			threadDirScannerTest.h
		Description:	unit test for ParalelDirScanner
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/threadDirScanner.h>

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


namespace gak
{

static Locker	g_locker;
static size_t	g_count = 0;

static ArrayOfStrings	g_filesFound;

template <>
struct ProcessorType<STRING>
{
	typedef STRING object_type;

	bool increment( const STRING &file )
	{
		LockGuard guard( g_locker );

		if( guard )
		{
			g_count++;
			g_filesFound.push_back( file );
			return true;
		}
		else
		{
			doLogMessageEx( gakLogging::llError, "Cannot lock" );
			return false;
		}
	}
	void process( const STRING &fileName )
	{
		doEnterFunctionEx(gakLogging::llDetail,"ProcessorType<IndexSourcePtr>::process");
		doLogValueEx( gakLogging::llDetail, fileName );
		if( !increment(fileName) )
		{
			std::cout << "Failed " <<  fileName << std::endl;
		}
	}
};


class ParalelDirScannerTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "ParalelDirScannerTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "ParalelDirScannerTest::PerformTest");
		TestScope scope( "PerformTest" );

		ParalelDirScanner	myScanner("ParalelDirScannerTest", CommandLine(), 5);

		myScanner("Java");

		// We expect to find these 4 files in this sort order (it is sorted by name):
		UT_ASSERT_EQUAL(g_count,4);
		UT_ASSERT_EQUAL(g_filesFound.size(),4);
		UT_ASSERT_EQUAL(g_filesFound[0],"Java\\com\\gaklib\\Lock.java");
		UT_ASSERT_EQUAL(g_filesFound[1],"Java\\com\\gaklib\\MessageBox.java");
		UT_ASSERT_EQUAL(g_filesFound[2],"Java\\gaklib.jpr");
		UT_ASSERT_EQUAL(g_filesFound[3],"Java\\gaklib.jpr.local");
	}
};

static ParalelDirScannerTest	myParalelDirScannerTest;

}	//namespace gak

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

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

