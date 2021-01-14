/*
		Project:		GAKLIB
		Module:			fLink.cpp
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

#if defined( _Windows )
#	ifndef STRICT
#		define STRICT 1
#	endif
	#include <windows.h>
#elif defined( __MACH__ ) || defined( __unix__ )
#	include <unistd.h>
#else
#	error "Don't know how to create hard links"
#endif

#include <gak/string.h>
#include <gak/gaklib.h>
// #include <gak/wideString.h>
#include <gak/exception.h>

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

#if defined( __BORLANDC__ ) && __BORLANDC__ < 0x0550
//extern "C"
//{
// BOOL WINAPI CreateHardLinkA( LPCTSTR lpFileName, LPCTSTR	lpExistingFilename, LPSECURITY_ATTRIBUTES reserved );
// BOOL WINAPI CreateHardLinkW( LPCWTSTR lpFileName, LPCWTSTR	lpExistingFilename, LPSECURITY_ATTRIBUTES reserved );
//}
//#ifdef UNICODE
// #define CreateHardLink  CreateHardLinkW
//#else
// #define CreateHardLink  CreateHardLinkA
//#endif // !UNICODE

typedef BOOL (__stdcall *CreateHardLinkType)(LPCTSTR,LPCTSTR,LPSECURITY_ATTRIBUTES);

static BOOL CreateHardLink( const char *target, const char *fileName, void * )
{
	static HINSTANCE	kernel = 0;
	static CreateHardLinkType CreateHardLinkPtr = NULL;

	if( !kernel )
		kernel = LoadLibrary( "KERNEL32.DLL" );

	if( kernel && !CreateHardLinkPtr )
		CreateHardLinkPtr = (CreateHardLinkType)GetProcAddress( kernel, "CreateHardLinkA" );

	if( CreateHardLinkPtr )
		return CreateHardLinkPtr( target, fileName, NULL );

	return FALSE;
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

void flink( const STRING &source, const STRING &target )
{
#if defined( _Windows )
	if( !CreateHardLink( target, source, NULL ) )
		throw LinkError( source, target );
#elif defined( __MACH__ ) || defined( __unix__ )
	if( link( source.convertToCharset( STR_UTF8 ), target.convertToCharset( STR_UTF8 ) ) )
		throw LinkError( source, target );
#endif
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

