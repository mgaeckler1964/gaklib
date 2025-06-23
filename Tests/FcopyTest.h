/*
		Project:		GAKLIB
		Module:			FcopyTest.h
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

#include <gak/acls.h>
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

#	pragma warn -inl
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

class FcopyTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "FcopyTest";
	}
	void copyFileTest( const STRING &source, const STRING &targetFile, bool noSecTest )
	{
		TestScope	scope(source + "->" + targetFile);
		int		errorCount = 0;
		STRING	sourceOwner, sourceGroup;
		STRING	targetOwner, targetGroup;
		STRING	dates;

		std::cout << "Preparing " << source << " to " << targetFile << std::endl;
		try
		{
			getOwner( source, &sourceOwner, &sourceGroup );
		}
		catch( std::exception &e )
		{
			std::cout << __LINE__ << ": Got exception " << e.what() << '\n';
			++errorCount;
		}

		makePath( targetFile );
		remove( targetFile );
		std::cout << "Copy " << source << " to " << targetFile << std::endl;
		fcopy( source, targetFile );

		if( !noSecTest )
		{
			try
			{
				copyACLs( source, targetFile );
			}
			catch( std::exception &e )
			{
				std::cout << __LINE__ << ": Got exception " << e.what() << '\n';
				++errorCount;
			}

			try
			{
				getOwner( targetFile, &targetOwner, &targetGroup );
			}
			catch( std::exception &e )
			{
				std::cout << __LINE__ << ": Got exception " << e.what() << '\n';
				++errorCount;
			}

			UT_ASSERT_EQUAL( sourceOwner, targetOwner );
			UT_ASSERT_EQUAL( sourceGroup, targetGroup );

			UT_ASSERT_EQUAL( 0, errorCount );
		}

		DirectoryEntry sourceEntry( source ), targetEntry( targetFile );

		UT_ASSERT_EQUAL( sourceEntry.fileSize, targetEntry.fileSize );
		UT_ASSERT_EQUAL( sourceEntry.modifiedDate, targetEntry.modifiedDate );
		UT_ASSERT_EQUAL( sourceEntry.accessDate, targetEntry.accessDate );

		// remove( targetFile );
	}
	virtual void PerformTest( void )
	{
		ArrayOfStrings	usbDrives;

		getUSBdrives( &usbDrives );
		for(
			ArrayOfStrings::iterator it = usbDrives.begin(), endIT = usbDrives.end();
			it != endIT;
			++it
		)
		{
			STRING	targetFile = *it;
			targetFile += "FcopyTest.tmp";
			copyFileTest( __FILE__, targetFile, true );
			copyFileTest( "LICENSE", targetFile, true );
			copyFileTest( "GAKDLL32.DEF", targetFile, true );
		}

		STRING targetFile = STRING(getTempPath()) + DIRECTORY_DELIMITER_STRING "FcopyTest" DIRECTORY_DELIMITER_STRING "copy.txt";

		copyFileTest( __FILE__, targetFile, false );

		/* this file was changed in summer time */
		copyFileTest( "mac.txt", targetFile, false );

		/* this file was changed in winter time */
		copyFileTest( "GAKDLL32.DEF", targetFile, false );

		STRING	tmpFile = getTempPath() + DIRECTORY_DELIMITER_STRING "test.dat";
		if( !exists( tmpFile ) )
		{
			std::cout << "Creating " << tmpFile << std::endl;
			Buffer<char>	buffer( malloc( 5*1024*1024 ) );
			STDfile fp( tmpFile, "wb" );
			for( size_t i=1; i<1024; i++ )
				fwrite( buffer, 1024*1024, 5, fp );
		}

		const DateTime	date( Date::getMothersDay(2009), Time(0UL) );
		const std::time_t	modTime1 = date.getUtcUnixSeconds();
		setModTime( tmpFile, modTime1 );
		struct stat	x;
		strStatE( tmpFile, &x );
		std::time_t modTime2 = x.st_mtime;
		std::cout << modTime1 <<'-' << modTime2 << '=' << (modTime2-modTime1) << std::endl;
		UT_ASSERT_EQUAL( modTime1, modTime2 );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FcopyTest myFcopyTest;

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

#	pragma warn +inl
#endif

