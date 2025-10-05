/*
		Project:		GAKLIB
		Module:			freadSTR.cpp
		Description:	Read a string from a file
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

static const char UTF8_BOM[3] = {0xEFu, 0xBBu, 0xBFu};

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
	STR 			*str = NULL;
	long			pos;
	unsigned short	uniCodeIndicator;
	bool			utf8IndicatorFnd = false;

	FILE *fp = fopen( fileName, "rb" );
	if( fp )
	{
		fseek( fp, 0, SEEK_END );
		//fgetpos( fp, &pos );
		pos = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		str = resizeStr( NULL, size_t(pos+2), cTrue );
		unsigned char *bufferPos = (unsigned char *)str->string;
		str->actSize = size_t(pos);
		if( pos >= 3 )
		{
			fread( bufferPos, 3, 1, fp );
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
		fread( bufferPos, size_t(pos), 1, fp );
		bufferPos[pos]=0;
		bufferPos[pos+1]=0;		// maybe an unicode file

		fclose( fp );
		
		if( utf8IndicatorFnd )
		{
			str->charset = STR_UTF8;
		}
		else
		{
			uniCodeIndicator = *((short*)str->string);
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
	size_t	numWritten;
	bool	errorFlag = true;

	FILE *fp = fopen( fileName, "wb" );
	if( fp )
	{
		if( str && str->actSize )
		{
			if( str->charset == STR_UTF8 && withBOM )
			{
				fwrite( UTF8_BOM, sizeof(UTF8_BOM), 1, fp  );
			}
			numWritten = fwrite( str->string, 1, str->actSize, fp );
			if( numWritten == str->actSize )
			{
				errorFlag = false;
			}
		}
		else
		{
			errorFlag = false;
		}

		fclose( fp );
	}

	return errorFlag;
}

}	// namespace gak
