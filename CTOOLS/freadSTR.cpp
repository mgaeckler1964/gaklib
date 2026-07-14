/*
		Project:		GAKLIB
		Module:			freadSTR.cpp
		Description:	Read a string from a file
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

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <fstream>

#include <gak/gaklib.h>
#include <gak/string.h>
#include <gak/wideChar.h>

#if defined( __BORLANDC__ ) && __BORLANDC__ == 0x621
// otherweise I get al lot of useless warnings about unused identifiers
#pragma option -w-use
#pragma option -w-pch
#pragma option -w-eff
#pragma option -w-8005
#endif

namespace gak
{

static const char UTF8_BOM[3] = {char(0xEFu), char(0xBBu), char(0xBFu)};

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

STR *freadSTR( FILE *fp )
{
	char	buffer[1024];
	size_t	len;
	bool	ready = false;
	STR		*line = newStr();

	while( fgets( buffer, 1024, fp ) )
	{
		len = strlen( buffer );
		if( !len )
/*v*/		break;

		len--;			
		if( buffer[len] == '\n' )
		{
			buffer[len] = 0;
			ready = true;
		}

		line = addStr( line, buffer );
		if( !line || ready )
		{
/*v*/		break;
		}
	}
	
	return line;
}

STR *readNfromFile( FILE *fp, size_t n )
{
	STR *str = resizeStr( NULL, n, cTrue );

	fread( str->string, n, 1, fp );
	str->string[n] = 0;
	str->actSize = strlen( str->string );

	return str;
}

STR *readFromFile( const char *fileName )
{
	STR *str = nullptr;

	std::ifstream fp( fileName, std::ios_base::binary|std::ios_base::in);
	if( fp )
	{
		bool	utf8IndicatorFnd = false;

		fp.seekg( 0, std::ios_base::end );
		size_t pos = fp.tellg();
		fp.seekg( 0, std::ios_base::beg );

		str = resizeStr( nullptr, size_t(pos+2), cTrue );
		char *bufferPos = str->string;
		str->actSize = size_t(pos);
		if( pos >= 3 )
		{
			fp.read( bufferPos, 3 );
			pos -= 3;
			if( !memcmp( bufferPos, UTF8_BOM, sizeof(UTF8_BOM) ) )
			{
				utf8IndicatorFnd = true;
				str->actSize -= 3;
			}
			else
			{
				bufferPos += 3;
			}
		}
		fp.read( bufferPos, size_t(pos) );
		bufferPos[pos]=0;
		bufferPos[pos+1]=0;		// maybe an unicode file

		if( utf8IndicatorFnd )
		{
			str->charset = STR_UTF8;
		}
		else
		{
			unsigned short uniCodeIndicator = *((short*)str->string);
			if( uniCodeIndicator == 0xFFFE || uniCodeIndicator == 0xFEFF )	// unicode
			{
				wchar_t	*uniCodeString = (wchar_t *)str->string;
				wchar_t	uniCodeChar;
				char	ansiChar;

				pos = 0;
				while( (uniCodeChar = *++uniCodeString) != 0 )
				{
					if( uniCodeIndicator == 0xFFFE )
						uniCodeChar = intimot(uniCodeChar);

					ansiChar = convertWChar( uniCodeChar );
					if( ansiChar )
						str->string[pos++] = ansiChar;
				}
				str->string[pos] = 0;
				str->actSize = size_t(pos);
				str->charset = STR_UNICODE;
			}
		}
	}

	return str;
}

bool writeToFile( const STR *str, const char *fileName, bool withBOM )
{
	bool	errorFlag = true;

	std::ofstream fp( fileName, std::ios_base::binary|std::ios_base::out );
	if( fp )
	{
		if( str && str->actSize )
		{
			if( str->charset == STR_UTF8 && withBOM )
			{
				fp.write( UTF8_BOM, sizeof(UTF8_BOM) );
			}
			fp.write( str->string, str->actSize );
			if( fp.good() )
			{
				errorFlag = false;
			}
		}
		else
		{
			errorFlag = false;
		}
	}

	return errorFlag;
}

}	// namespace gak
