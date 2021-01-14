/*
		Project:		GAKLIB
		Module:			fProtect.cpp
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

#include <gak/directory.h>
#include <gak/wideString.h>

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
#if defined( _Windows ) && !defined( INVALID_FILE_ATTRIBUTES )
const DWORD INVALID_FILE_ATTRIBUTES=0xFFFFFFFF;
#endif

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

#if defined( _Windows )

static inline unsigned long GetAttributes( const STRING &fileName )
{
	unsigned long attr = GetFileAttributes( fileName );
	if( attr == INVALID_FILE_ATTRIBUTES )
		throw AttributeReadError( fileName );

	return attr;
}

static inline unsigned long GetAttributes( const uSTRING &wPath, const STRING &fileName )
{
	unsigned long attr = GetFileAttributesW( wPath );
	if( attr == INVALID_FILE_ATTRIBUTES )
		throw AttributeReadError( fileName );

	return attr;
}

static inline void SetAttributes( const STRING &fileName, unsigned long attr )
{
	if( !SetFileAttributesA( fileName, attr ) )
	{
		throw AttributeWriteError( fileName );
	}
}

static inline void SetAttributes( 
	const uSTRING &wPath, const STRING &fileName, unsigned long attr 
)
{
	if( !SetFileAttributesW( wPath, attr ) )
	{
		throw AttributeWriteError( fileName );
	}
}
#elif defined( __MACH__ ) || defined( __unix__ )
static inline mode_t GetAttributes( const STRING &utfName )
{
	struct stat	statBuf;
	if( stat( utfName, &statBuf ) )
	{
		throw AttributeReadError( utfName );
	}

	return statBuf.st_mode;
}

static inline mode_t SetAttributes( const STRING &utfName, mode_t attr )
{
	if( chmod( utfName, attr ) )
	{
		throw AttributeWriteError( utfName );
	}
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

void fprotect( const STRING &fileName )
{
#if defined( _Windows )
	if( fileName.getCharSet() == STR_UTF8 )
	{
		uSTRING	wPath;

		wPath.decodeUTF8( fileName );
		unsigned long attr = GetAttributes( wPath, fileName );
		if( !(attr & FILE_ATTRIBUTE_READONLY) )
		{
			SetAttributes( wPath, fileName, attr );
		}
	}
	else
	{
		unsigned long attr = GetAttributes( fileName );
		if( !(attr & FILE_ATTRIBUTE_READONLY) )
		{
			attr |= FILE_ATTRIBUTE_READONLY;
			SetAttributes( fileName, attr );
		}
	}
#elif defined( __MACH__ ) || defined( __unix__ )
	STRING		utfName = fileName.convertToCharset( STR_UTF8 );
	mode_t attr = GetAttributes( utfName );
	if( attr & (S_IWUSR|S_IWGRP|S_IWOTH) )
		SetAttributes( utfName, attr & ~(S_IWUSR|S_IWGRP|S_IWOTH) );
#endif
}

void funprotect( const STRING &fileName )
{
#if defined( _Windows )
	if( fileName.getCharSet() == STR_UTF8 )
	{
		uSTRING	wPath;

		wPath.decodeUTF8( fileName );
		unsigned long attr = GetAttributes( wPath, fileName );
		if( attr & FILE_ATTRIBUTE_READONLY )
		{
			attr &= ~FILE_ATTRIBUTE_READONLY;
			SetAttributes( wPath, fileName, attr );
		}
	}
	else
	{
		unsigned long attr = GetAttributes( fileName );
		if( attr & FILE_ATTRIBUTE_READONLY )
		{
			attr &= ~FILE_ATTRIBUTE_READONLY;
			SetAttributes( fileName, attr );
		}
	}
#elif defined( __MACH__ ) || defined( __unix__ )
	STRING		utfName = fileName.convertToCharset( STR_UTF8 );
	mode_t attr = GetAttributes( utfName );
	if( !(attr & S_IWUSR))
		SetAttributes( utfName, attr | S_IWUSR );
#endif
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

