/*
		Project:		GAKLIB
		Module:			DirectoryTest.h
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

class DirectoryTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "DirectoryTest";
	}
	virtual void PerformTest( void )
	{
		F_STRING	tmp = getTempPath();
		std::cout << tmp << '\n';

		F_STRING home = getPersonalHome();
		std::cout << home << '\n';

		F_STRING docs = getPersonalDocs();
		std::cout << docs << '\n';

		F_STRING config = getPersonalConfig();
		std::cout << docs << '\n';

		F_STRING	curDir = getcwd();
		std::cout << curDir << '\n';

		setcwd( tmp );
		UT_ASSERT_EQUAL( getcwd(), tmp );

		setcwd( home );
		UT_ASSERT_EQUAL( getcwd(), home );

		setcwd( docs );
		UT_ASSERT_EQUAL( getcwd(), docs );

		setcwd( config );
		UT_ASSERT_EQUAL( getcwd(), config );

		setcwd( curDir );
		UT_ASSERT_EQUAL( getcwd(), curDir );

		ArrayOfStrings	usbDrives;
		getUSBdrives( &usbDrives );
		for( size_t i=0; i<usbDrives.size(); i++ )
		{
			std::cout << usbDrives[i] << std::endl;
		}

		STRING	myFile = tmp + DIRECTORY_DELIMITER_STRING "xxyyzäüöß.xxx";

		STDfile fp( myFile, "wb" );
		UT_ASSERT_NOT_NULL( fp );
		if( fp )
		{
			fp.close();
			int result = strAccess( myFile, 06 );
			UT_ASSERT_EQUAL( result, 0 );
			myFile = myFile.convertToCharset( STR_UTF8 );
			result = strAccess( myFile, 06 );
			UT_ASSERT_EQUAL( result, 0 );
		}
		strRemoveE( myFile );

		myFile = "LICENSE"; // "TCGAKLIB.PRJ";
		int result = strAccess( myFile, 06 );
		UT_ASSERT_NOT_EQUAL( result, 0 );
		if( result )
			UT_ASSERT_EQUAL( errno, EACCES );
		result = strAccess( myFile, 04 );
		UT_ASSERT_EQUAL( result, 0 );

		myFile = "GAKDLL32.DEF";
		DirectoryEntry	theEntry( myFile );
		UT_ASSERT_TRUE( theEntry.readOnly );
		funprotect( myFile );
		theEntry.findFile( myFile );
		UT_ASSERT_FALSE( theEntry.readOnly );
		fprotect( myFile );
		theEntry.findFile( myFile );
		UT_ASSERT_TRUE( theEntry.readOnly );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static DirectoryTest myDirectoryTest;

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

