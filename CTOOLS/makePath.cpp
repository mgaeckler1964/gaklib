/*
		Project:		GAKLIB
		Module:			makepath.cpp
		Description:	build a complete path
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

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <string.h>

#if defined( _MSC_VER )
	#include <direct.h>
#endif

#ifdef __BORLANDC__
	#include <dir.h>
#endif

#include <gak/gaklib.h>
#include <gak/directory.h>
#include <gak/wideString.h>
#include <gak/exception.h>

namespace gak
{

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */
static inline int gakMkdir( const char *dirName )
{
	#ifdef _Windows
		return ::mkdir( dirName );
	#else
		return ::mkdir( dirName, 0777 );
	#endif
}

static void makePath( const char *path )
{
	char	*cp;

	cp = const_cast<char *>(path);
	while( 1 )
	{
		cp = strchr( cp, DIRECTORY_DELIMITER );
		if( !cp )
/*v*/		break;

		*cp = 0;
		gakMkdir( path );
		*cp = DIRECTORY_DELIMITER;
		cp++;
	}
}

#ifdef _Windows
static void makePath( const wchar_t *path )
{
	wchar_t	*cp;

	cp = (wchar_t *)path;
	while( 1 )
	{
		cp = wcschr( cp, DIRECTORY_DELIMITER );
		if( !cp )
/*v*/		break;

		*cp = 0;
		_wmkdir( path );

		*cp = DIRECTORY_DELIMITER;
		cp++;
	}
}
#endif

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

void makePath( const STRING &path )
{
#ifdef _Windows
	if( path.getCharSet() == STR_UTF8 )
	{
		makePath( uSTRING().decodeUTF8( path ) );
	}
	else
		makePath( static_cast<const char *>(path) );
#else
	makePath( (const char *)path.convertToCharset( STR_UTF8 ) );
#endif
}

void makeDirectory( const STRING &dirName )
{
	int	error;
#ifdef _Windows
	if( dirName.getCharSet() == STR_UTF8 )
	{
		error = _wmkdir( uSTRING().decodeUTF8( dirName ) );
	}
	else
		error = gakMkdir( dirName );
#else
	error = gakMkdir( dirName.convertToCharset( STR_UTF8 ) );
#endif
	if( error )
		throw MakeDirectoryError( dirName );
}

}	// namespace gak

