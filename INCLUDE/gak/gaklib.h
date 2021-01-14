/*
		Project:		GAKLIB
		Module:			GAKLIB.H
		Description:	Interface to gaklib
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

/*
--------------------
*/
#ifndef __GAKLIB__
#define __GAKLIB__

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef STRICT
#define STRICT 1
#endif

#if defined( _WIN32 ) && !defined( _WIN64 ) && !defined( __WIN32__ )
#	define __WIN32__
#endif

#if defined( _WIN64 ) && !defined( __WIN64__ )
#	define __WIN64__
#endif

#if (defined( __WIN32__ ) || defined( __WIN64__ )) && !defined( __WINDOWS__ )
#define __WINDOWS__
#endif

#if defined( __WINDOWS__ ) && !defined( _Windows )
#define	_Windows
#endif

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
	#include <direct.h>
#endif

#if defined( __MACH__ ) || defined( __unix__ )
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

#include <gak/types.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

#ifdef _MSC_VER
//#	pragma warning( disable: 996 4514 4710 4711 4820  )
//#	pragma warning( disable: 4355 4365  4625 4626 )

#	pragma warning( disable: 4996 ) // 'xxx': This function or variable may be unsafe. Consider using xxx instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#	pragma warning( disable: 4800 ) // 'xxx': Variable wird auf booleschen Wert ('True' oder 'False') gesetzt (Auswirkungen auf Leistungsverhalten möglich)	
#	pragma warning( disable: 4512 )	// 'xxx': Zuweisungsoperator konnte nicht generiert werden
#	pragma warning( disable: 4127 )	// bedingter Ausdruck ist konstant
#	pragma warning( disable: 4505 ) // Nichtreferenzierte lokale Funktion wurde entfernt
#	pragma warning( disable: 4701 ) // Die möglicherweise nicht initialisierte lokale Variable "xxx" wurde verwendet.

#	pragma warning( 1: 4061 4063 4263 )
#	pragma comment( lib, "gaklib.lib" )
#endif

/* --------------------------------------------------------------------- */
/* ----- constants ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined( __MSDOS__ ) || defined ( _MSDOS )|| defined( __TOS__ ) || defined ( _Windows )
	#define DIRECTORY_DELIMITER			'\\'
	#define DIRECTORY_DELIMITER_STRING	"\\"
#elif defined( __UNIX__ ) || defined( UNIX ) || defined( __MACH__ ) || defined( __unix__ )
	#define DIRECTORY_DELIMITER			'/'
	#define DIRECTORY_DELIMITER_STRING	"/"
#else
	#error "Don't know DIRECTORY_DELIMITER"
#endif

// #define VERSION "1.71"

#define MOTOROLA	0
#define INTEL		1

#ifdef __TOS__
#define INT_FORMAT	MOTOROLA
#else
#define INT_FORMAT	INTEL
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#ifndef M_E
#define M_E         2.71828182845904523536
#endif

/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- macros -------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if !defined( __cplusplus )
#	ifndef min
#		define min(x,y) (x) < (y) ? (x) : (y)
#	endif

#	ifndef max
#		define max(x,y) (x) > (y) ? (x) : (y)
#	endif
#endif

#define arraySize( array )		(sizeof( array ) / sizeof( array[0] ))

#if defined( _WIN32 ) && defined( _MSC_VER )
#define _pascal /* dummy */
#define _export /* dummy */
#endif

#if defined( _MSC_VER )
#	define STRINGIFY(x) #x
#	define TOSTRING(x) STRINGIFY(x)
#	define WARNING( txt ) __FILE__"(" TOSTRING(__LINE__) ") : warning: " txt
// #pragma message( WARNING( "Example" ) )
#endif

/* --------------------------------------------------------------------- */
/* ----- prototypes ---------------------------------------------------- */
/* --------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/******** micelaneous ****************************************************/

unsigned long   swaplong( unsigned long value );
unsigned long   intlmot( unsigned long value );

#ifdef __cplusplus
inline unsigned short intimot( unsigned short x )
{
	return  (unsigned short)((( x & 0xFF00 )>>8) | (( x & 0xFF ) << 8));
}
#else
	#define intimot(x)	(unsigned short)((((unsigned short)(x)&0xFF00)>>8) | (((unsigned short)(x)&0xFF)<<8))
#endif

#define swapByte( ch )	(((unsigned)(ch&0xF0)>>4) |(((unsigned)(ch)&0xF)<<4))


int     append( char *destination, const char *source, int start);
#ifdef __BORLANDC__
#pragma obsolete append
#endif
/*
	tokenize is obsolete. Use ARRAY_OF_STRINGS::createElements instead
*/
int     tokenize( char *string, const char *pointerlist[], int numPtr );
#ifdef __BORLANDC__
#pragma obsolete tokenize
#endif

int     tokenSearch(const char *key, const char *list);

#if !defined( __BORLANDC__ ) && !defined( _MSC_VER )
int		strcmpi( const char *s1, const char *s2 );
#endif
#if !defined( __BORLANDC__ )
int		strncmpi( const char *s1, const char *s2, size_t len );
#endif


#define mkTimeStamp( date, time ) \
					( ((unsigned long)(date)<<16)|(unsigned long)(time) )


#ifdef __BORLANDC__
#define randomNumber( max ) random( max )
#else
int randomNumber( int max );
#endif


/******** Profile support *************************************************/

cBool LocateProfileSection( FILE *file, char *section );
cBool LocateProfileEntry( FILE *file, char *entry, char *value );
void AddProfileEntry( const char *iniFile, const char *section, const char *entry );
void RemoveProfileEntry( const char *iniFile, const char *section, const char *entry );

#ifdef __cplusplus
}	// extern "C"
#endif

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif


