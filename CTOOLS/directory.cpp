/*
		Project:		GAKLIB
		Module:			directory.cpp
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

#ifdef _Windows
	#ifndef STRICT
		#define STRICT 1
	#endif
	#include <windows.h>
#endif

#ifdef __MACH__
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/mount.h>
#endif

#include <gak/string.h>
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

#ifdef _Windows
static STRING getUserShellFolder( const char *name )
{
	HKEY	shellFolderKey;
	STRING	result;
	char	buffer[1024];
	DWORD	type, size;

	if( RegOpenKey(
		HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
		&shellFolderKey
	) == ERROR_SUCCESS )
	{
		size = sizeof( buffer );
		if( RegQueryValueEx(
			shellFolderKey, name,
			NULL,
			&type,
			(LPBYTE)buffer,
			&size ) == ERROR_SUCCESS )
		{
			result = buffer;
		}


		RegCloseKey( shellFolderKey );
	}

	if( result.endsWith( DIRECTORY_DELIMITER ) )
		result.cut( result.strlen()-1 );

	return result;
}
#endif

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

F_STRING getTempPath( void )
{
	STRING	temp = getenv( "TEMP" );
	if( temp.isEmpty() )
	{
		temp = getenv( "TMP" );
	}
	if( temp.isEmpty() )
#if defined( _Windows )
		temp = "C:\\TEMP";
#elif defined( __ELF__ )
		temp = "/tmp";
#elif defined( __MACH__ )
		temp = "/private/tmp";
#else
#error "Don't know temp"
#endif

	if( temp.endsWith( DIRECTORY_DELIMITER ) )
		temp.cut( temp.strlen()-1 );

	return temp;
}

F_STRING getPersonalHome( void )
{
#if defined( __MACH__ ) || defined( __ELF__ )
	STRING	home = getenv( "HOME" );
#endif
#if defined( _Windows )
	STRING	home = getenv( "USERPROFILE" );
#endif

	if( home.endsWith( DIRECTORY_DELIMITER ) )
		home.cut( home.strlen()-1 );

	return home;
}

#if defined( _Windows )
F_STRING getPersonalDocs( void )
{
	return getUserShellFolder( "Personal");
}
#endif

#if defined( _Windows )
F_STRING getPersonalConfig( void )
{
	return getUserShellFolder( "AppData");
}
#endif

void getUSBdrives( ArrayOfStrings *result )
{
	result->clear();
#if defined( _Windows )
	DWORD	driveMap = GetLogicalDrives();
	char	path[] = "A:\\";
	while( driveMap )
	{
		if( driveMap & 1 )
		{
			UINT type = GetDriveType( path );
			if( type == DRIVE_REMOVABLE )
			{
				*result += STRING( path );
			}
		}
		driveMap >>= 1;
		++(*path);
	}
#elif defined( __MACH__ )
    struct statfs   *fsInfo;
    int				fsCount = getmntinfo( &fsInfo, MNT_NOWAIT );
    for( int i=0; i<fsCount; i++ )
    {
        if( !strcmp( fsInfo[i].f_fstypename, "msdos" ) )
        {
			result->addElement( fsInfo[i].f_mntonname );
        }
    }
#endif
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

