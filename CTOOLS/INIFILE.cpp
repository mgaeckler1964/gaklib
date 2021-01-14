/*
		Project:		GAKLIB
		Module:			INIFILE.CPP
		Description:	Inifile support for non-windows apps
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
/* ----- switches ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <cstdio>
#include <cstring>

#include <gak/gaklib.h>
#include <gak/t_string.h>
#include <gak/stdlib.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace gak;

/* --------------------------------------------------------------------- */
/* ----- constants ----------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- macros -------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- prototypes ---------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- imported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module statics ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- exported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

static bool isSection( T_STRING &line, const char *section )
{
	if( line[0U] != '[' )
	{
		return false;
	}

	line += std::size_t(1);
	line = line.getFirstToken( "]" );

	if( line == section )
	{
		return true;
	}

	return false;
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

extern "C" cBool LocateProfileSection( FILE *file, char *section )
{
	T_STRING	line;

	while( 1 )
	{
		line << file;
		if( feof( file ) )
/*v*/		break;

		line.stripBlanks();

		if( !line[0U] || line[0U] == ';' || line[0U] != '[' )
/*^*/      	continue;

		if( !isSection( line, section ) && *section  )
/*^*/      	continue;

		if( !*section )
			strcpy( section, line );

/***/	return (cBool)true;

	}
	return (cBool)false;
}

extern "C" cBool LocateProfileEntry( FILE *file, char *entry, char *value )
{
	T_STRING	line;
	STRING		entryStr, valueStr;

	while( 1 )
	{
		line << file;
		if( feof( file ) )
/*v*/		break;

		line.stripBlanks();

		if( !line[0U] || line[0U] == ';' )
/*^*/      	continue;

		if( line[0U] == '[' )
/*v*/		break;

		line = line.getFirstToken( ";" );

		entryStr = line.getFirstToken( "=" );

		if( *entry && (STRING)entry != entryStr )
/*^*/      	continue;

		valueStr = line.getNextToken();

		if( !*entry )
			strcpy( entry, entryStr );
		strcpy( value, valueStr );

/***/	return (cBool)true;
	}
	return (cBool)false;
}

extern "C" void AddProfileEntry( const char *iniFile, const char *section, const char *entry )
{
	char		bakFile[256], *slash, *ext;
	T_STRING	line;
	bool		written = false;

	strcpy( bakFile, iniFile );

	slash = strrchr( bakFile, DIRECTORY_DELIMITER );
	ext   = strrchr( bakFile, '.' );
	if( ext && ext > slash )
	{
		strcpy( ext, ".$$$" );
	}
	else
		strcat( bakFile, ".$$$" );

	remove( bakFile );
	rename( iniFile, bakFile );
	STDfile iniFp( iniFile, "w" );
	if( iniFp )
	{
		STDfile	oldFp( bakFile, "r" );
		if( oldFp )
		{
			while( 1 )
			{
				line << oldFp;
				if( feof( oldFp ) )
/*v*/              	break;

				line >> iniFp;
				line.stripBlanks();
				if( !written && isSection( line, section ) )
				{
					fprintf( iniFp, "%s\n", entry );
					written = true;
				}
			}
		}
	}
	remove( bakFile );
}

extern "C" void RemoveProfileEntry( const char *iniFile, const char *section, const char *entry )
{
	char		bakFile[256], *slash, *ext;
	T_STRING	line, iniLine;
	bool		written = false;
	bool		sectionFound = false;

	char		lineBuf[256];

	strcpy( bakFile, iniFile );

	slash = strrchr( bakFile, DIRECTORY_DELIMITER );
	ext   = strrchr( bakFile, '.' );
	if( ext && ext > slash )
	{
		strcpy( ext, ".$$$" );
	}
	else
		strcat( bakFile, ".$$$" );

	remove( bakFile );
	rename( iniFile, bakFile );
	STDfile	iniFp( iniFile, "w" );
	if( iniFp )
	{
		STDfile	oldFp( bakFile, "r" );
		if( oldFp )
		{
			while( 1 )
			{
				line << oldFp;
				if( feof( oldFp ) )
/*v*/              	break;

				iniLine = line;

				line.stripBlanks();

				strcpy( lineBuf, line );

				if( !written && isSection( line, section ) )
					sectionFound = true;
				if( !written && sectionFound && line == entry )
				{
					written = true;
/*^*/				continue;
				}

				iniLine >> iniFp;
			}
		}
	}
	remove( bakFile );
}


