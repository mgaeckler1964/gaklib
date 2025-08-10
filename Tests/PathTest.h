/*
		Project:		GAKLIB
		Module:			PathTest.h
		Description:	
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

#include <gak/directory.h>

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

class PathTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "PathTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "PathTest::PerformTest");
		TestScope scope( "PerformTest" );

#ifdef _Windows
		STRING	relPath = makeRelPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			"D:\\Dokumente\\user\\gak\\deppen.cpp"
		);
		STRING	fullPath = makeFullPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("D:\\Dokumente\\user\\gak\\deppen.cpp")  );
		UT_ASSERT_EQUAL( fullPath, STRING("D:\\Dokumente\\user\\gak\\deppen.cpp") );

		relPath = makeRelPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			"C:\\Dokumente\\user\\gak\\deppen.cpp"
		);
		fullPath = makeFullPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("deppen.cpp") );
		UT_ASSERT_EQUAL( fullPath, STRING("C:\\Dokumente\\user\\gak\\deppen.cpp") );

		relPath = makeRelPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			"C:\\Maja\\user\\gak\\deppen.cpp"
		);
		fullPath = makeFullPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("..\\..\\..\\Maja\\user\\gak\\deppen.cpp") );
		UT_ASSERT_EQUAL( fullPath, STRING("C:\\Maja\\user\\gak\\deppen.cpp") );

		relPath = makeRelPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			"C:\\Dokumente\\user\\gak\\doks\\deppen.cpp"
		);
		fullPath = makeFullPath(
			"C:\\Dokumente\\user\\gak\\deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("doks\\deppen.cpp") );
		UT_ASSERT_EQUAL( fullPath, STRING("C:\\Dokumente\\user\\gak\\doks\\deppen.cpp") );
#endif
#if defined( __MACH__ ) || defined( __unix__ )
		STRING	relPath = makeRelPath(
			"/Dokumente/user/gak/deppen.cpp",
			"/Dokumente/user/gak/deppen.cpp"
		);
		STRING	fullPath = makeFullPath(
			"/Dokumente/user/gak/deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("deppen.cpp") );
		UT_ASSERT_EQUAL( fullPath, STRING("/Dokumente/user/gak/deppen.cpp") );

		relPath = makeRelPath(
			"/Dokumente/user/gak/deppen.cpp",
			"/Maja/user/gak/deppen.cpp"
		);
		fullPath = makeFullPath(
			"/Dokumente/user/gak/deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("../../../Maja/user/gak/deppen.cpp") );
		UT_ASSERT_EQUAL( fullPath, STRING("/Maja/user/gak/deppen.cpp") );

		relPath = makeRelPath(
			"/Dokumente/user/gak/deppen.cpp",
			"/Dokumente/user/gak/doks/deppen.cpp"
		);
		fullPath = makeFullPath(
			"/Dokumente/user/gak/deppen.cpp",
			relPath
		);
		UT_ASSERT_EQUAL( relPath, STRING("doks/deppen.cpp") );
		UT_ASSERT_EQUAL( fullPath, STRING("/Dokumente/user/gak/doks/deppen.cpp") );
#endif
		F_STRING	fName = DIRECTORY_DELIMITER_STRING "path" DIRECTORY_DELIMITER_STRING "name.ext";

		F_STRING	path, name;
		STRING	extension = fsplit( fName, &path, &name );
		UT_ASSERT_EQUAL( path, F_STRING(DIRECTORY_DELIMITER_STRING "path" DIRECTORY_DELIMITER_STRING) );
		UT_ASSERT_EQUAL( name, F_STRING("name.ext") );
		UT_ASSERT_EQUAL( extension, STRING("ext") );

		F_STRING fullName = gak::fullPath( __FILE__ );

#ifdef _Windows
		UT_ASSERT_EQUAL( fullName, F_STRING("c:\\cresd\\source\\gaklib\\Tests/PathTest.h") );
#endif
#if defined( __MACH__ ) || defined( __unix__ )
		UT_ASSERT_EQUAL( fullName, F_STRING("/Source/GAKLIB/Tests/PathTest.h") );
#endif
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static PathTest myPathTest;

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

