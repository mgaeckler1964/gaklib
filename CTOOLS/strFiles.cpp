/*
		Project:		GAKLIB
		Module:			strFiles.cpp
		Description:	check for UTF-8 characters in file names prior some
						standard calls.
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

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable: 4986 4820 4668 )
#endif

#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>

#ifdef _MSC_VER
#	pragma warning( pop )
#endif

#include <gak/directory.h>
#include <gak/strFiles.h>
#include <gak/wideString.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

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

int strCreat( const STRING &filename, int mode )
{
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
		result = _wcreat( uSTRING().decodeUTF8( filename ), mode );

	}
	else
	{
		result = creat( filename, mode );
	}

	return result;
}

int strOpen( const STRING &filename, int flags )
{
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
		result = _wopen( uSTRING().decodeUTF8( filename ), flags );

	}
	else
	{
		result = open( filename, flags );
	}

	return result;
}

int strStat( const STRING &filename, struct stat *statbuff )
{
#if defined _MSC_VER
	struct _stat64	vs2010Buff;
#endif
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
#if defined( __BORLANDC__ )
	#if __BORLANDC__ > 0x0520
		result = _wstat( uSTRING().decodeUTF8( filename ), (struct _stat *)statbuff );
	#else
		result = _wstat( uSTRING().decodeUTF8( filename ), statbuff );
	#endif
#elif defined _MSC_VER
		result = _wstat64( uSTRING().decodeUTF8( filename ), &vs2010Buff );
#endif
	}
	else
	{
#if defined( __BORLANDC__ )
		result = stat( filename, statbuff );
#elif defined _MSC_VER
		result = _stat64( filename, &vs2010Buff );
#endif
	}

#ifdef _MSC_VER
	if( !result )
	{
		statbuff->st_atime = vs2010Buff.st_atime;
		statbuff->st_ctime = vs2010Buff.st_ctime;
		statbuff->st_dev = vs2010Buff.st_dev;
		statbuff->st_gid = vs2010Buff.st_gid;
		statbuff->st_ino = vs2010Buff.st_ino;
		statbuff->st_mode = vs2010Buff.st_mode;
		statbuff->st_mtime = vs2010Buff.st_mtime;
		statbuff->st_nlink = vs2010Buff.st_nlink;
		statbuff->st_rdev = vs2010Buff.st_rdev;
		statbuff->st_size = _off_t(vs2010Buff.st_size);
		statbuff->st_uid = vs2010Buff.st_uid;

		/*
			fix bug with wrong time, if date is in an different time zone
		*/
		DateTime	now;
		DateTime	modTime( statbuff->st_mtime );
		long		nowOffset = now.getTZoffset();
		long		modOffset = modTime.getTZoffset();

		if( nowOffset != modOffset )
			statbuff->st_mtime += (modOffset - nowOffset);
	}
#endif
	return result;
}

int strAccess( const STRING &filename, int amode )
{
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
		result = _waccess( uSTRING().decodeUTF8( filename ), amode );

	}
	else
	{
		result = access( filename, amode );
	}

	return result;
}


int strUtime( const STRING &filename, struct utimbuf *times )
{
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
		result = gak::_wutime( uSTRING().decodeUTF8( filename ), times );

	}
	else
	{
		result = gak::utime( filename, times );
	}

	return result;
#if 0
	int	result;

#if defined( __BORLANDC__ ) || defined( _MSC_VER )
	result = _tutime( path, times );
#else
#if defined( _MSC_VER )
	/*
		fix bug with wrong time, if date is in an different time zone
	*/
	DateTime	now;
	DateTime	modTime( times->modtime );
	long		nowOffset = now.getTZoffset();
	long		modOffset = modTime.getTZoffset();

	if( nowOffset != modOffset )
		times->modtime -= (modOffset - nowOffset);
#endif
	result = utime( path, times );
#endif

	return result;
#endif
}

FILE *strFopen( const STRING &filename, const STRING &mode )
{
	FILE	*fp;

	if( filename.getCharSet() == STR_UTF8 )
	{
		fp = _wfopen( uSTRING().decodeUTF8( filename ), uSTRING().decodeUTF8( mode ) );

	}
	else
	{
		fp = fopen( filename, mode );
	}

	return fp;
}

int strRemove( const STRING &filename )
{
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
		result = _wremove( uSTRING().decodeUTF8( filename ) );

	}
	else
	{
		result = remove( filename );
	}

	return result;
}

int strRmdir( const STRING &filename )
{
	int	result;

	if( filename.getCharSet() == STR_UTF8 )
	{
		result = _wrmdir( uSTRING().decodeUTF8( filename ) );

	}
	else
	{
		result = rmdir( filename );
	}

	return result;
}

int strRename(const STRING &oldname, const STRING &newname)
{
	int	result;

	if( oldname.getCharSet() == STR_UTF8 || newname.getCharSet() == STR_UTF8 )
	{
		result = _wrename( uSTRING(oldname), uSTRING(newname) );

	}
	else
	{
		result = rename( oldname, newname );
	}

	return result;
}

}	// namespace gak

