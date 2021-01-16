/*
		Project:		GAKLIB
		Module:			wideChar.cpp
		Description:
		Author:			Martin G�ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Germany, Munich ``AS IS''
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

#include <gak/wideChar.h>
#include <gak/gaklib.h>
#include <gak/ansiChar.h>

/* --------------------------------------------------------------------- */
/* ----- module switches ----------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

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

static struct uniMaps
{
	wchar_t			unicode;
	unsigned char	ansiCode;
} unicodeMapping[] =
{
	{ 0x20AC, 0x80 },	// �
	{ 0x0081, 0x81 },	// <--- unkown by windows
	{ 0x201A, 0x82 },	// �
	{ 0x0192, 0x83 },	// �
	{ 0x201E, 0x84 },	// �
	{ 0x2026, 0x85 },	// �
	{ 0x2020, 0x86 },	// �
	{ 0x2021, 0x87 },	// �
	{ 0x02C6, 0x88 },	//
	{ 0x2030, 0x89 },	// �
	{ 0x0160, 0x8A },	// �
	{ 0x2039, 0x8B },	// �
	{ 0x0152, 0x8C },	// �
	{ 0x008D, 0x8D },	// <--- unkown by windows
	{ 0x017D, 0x8E },	// �
	{ 0x008F, 0x8F },	// <--- unkown by windows
	{ 0x0090, 0x90 },	// <--- unkown by windows
	{ 0x2018, 0x91 },	// �
	{ 0x2019, 0x92 },	// �
	{ 0x201C, 0x93 },	// �
	{ 0x201D, 0x94 },	// �
	{ 0x2022, 0x95 },	// �
	{ 0x2013, 0x96 },	// � en dash &ndash; (Bindestrich)
	{ 0x2014, 0x97 },	// � em dash (Gedankenstrich)
	{ 0x02DC, 0x98 },	// �
	{ 0x2122, 0x99 },	// �
	{ 0x0161, 0x9A },	// �
	{ 0x203A, 0x9B },	// �
	{ 0x0153, 0x9C },	// �s
	{ 0x009D, 0x9D },	// <--- unkown by windows
	{ 0x017E, 0x9E },	// �
	{ 0x0178, 0x9F },	// �z
	{ 0x00A0, 0xA0 },	// NON_BREAKING_SPACE
	{ 0x00A1, 0xA1 },	// � (Inverted exclamation mark)
	{ 0x00A2, 0xA2 },	// � (cent)
	{ 0x00A3, 0xA3 },	// � (pound)
	{ 0x00A4, 0xA4 },	// �
	{ 0x00A5, 0xA5 },	// � (Yen)
	{ 0x00A6, 0xA6 },	// �
	{ 0x00A7, 0xA7 },	// �
	{ 0x00A8, 0xA8 },	// �
	{ 0x00A9, 0xA9 },	// �
	{ 0x00AA, 0xAA },	// � (Ordinal indicator feminine)
	{ 0x00AB, 0xAB },	// �
	{ 0x00AC, 0xAC },	// �
	{ 0x00AD, 0xAD },	// �
	{ 0x00AE, 0xAE },	// �
	{ 0x00AF, 0xAF },	// � (Macron)
	{ 0x00B0, 0xB0 },	// �
	{ 0x00B1, 0xB1 },	// �
	{ 0x00B2, 0xB2 },	// � (upper 2)
	{ 0x00B3, 0xB3 },	// � (upper 3)
	{ 0x00B4, 0xB4 },	// �
	{ 0x00B5, 0xB5 },	// �
	{ 0x00B6, 0xB6 },	// �
	{ 0x00B7, 0xB7 },	// �
	{ 0x00B8, 0xB8 },	// �
	{ 0x00B9, 0xB9 },	// � (upper 1)
	{ 0x00BA, 0xBA },	// � (Ordinal indicator male)
	{ 0x00BB, 0xBB },	// �
	{ 0x00BC, 0xBC },	// � (quarter)
	{ 0x00BD, 0xBD },	// � (half)
	{ 0x00BE, 0xBE },	// � (three quarter)
	{ 0x00BF, 0xBF },	// � (Inverted question mark)
	{ 0x00C0, 0xC0 },	// �
	{ 0x00C1, 0xC1 },	// �
	{ 0x00C2, 0xC2 },	// �
	{ 0x00C3, 0xC3 },	// �
	{ 0x00C4, 0xC4 },	// �
	{ 0x00C5, 0xC5 },	// �
	{ 0x00C6, 0xC6 },	// �
	{ 0x00C7, 0xC7 },	// �
	{ 0x00C8, 0xC8 },	// �
	{ 0x00C9, 0xC9 },	// �
	{ 0x00CA, 0xCA },	// �
	{ 0x00CB, 0xCB },	// �
	{ 0x00CC, 0xCC },	// �
	{ 0x00CD, 0xCD },	// �
	{ 0x00CE, 0xCE },	// �
	{ 0x00CF, 0xCF },	// �
	{ 0x00D0, 0xD0 },	// �
	{ 0x00D1, 0xD1 },	// �
	{ 0x00D2, 0xD2 },	// �
	{ 0x00D3, 0xD3 },	// �
	{ 0x00D4, 0xD4 },	// �
	{ 0x00D5, 0xD5 },	// �
	{ 0x00D6, 0xD6 },	// �
	{ 0x00D7, 0xD7 },	// �
	{ 0x00D8, 0xD8 },	// �
	{ 0x00D9, 0xD9 },	// �
	{ 0x00DA, 0xDA },	// �
	{ 0x00DB, 0xDB },	// �
	{ 0x00DC, 0xDC },	// �
	{ 0x00DD, 0xDD },	// �
	{ 0x00DE, 0xDE },	// �
	{ 0x00DF, 0xDF },	// �
	{ 0x00E0, 0xE0 },	// �
	{ 0x00E1, 0xE1 },	// �
	{ 0x00E2, 0xE2 },	// �
	{ 0x00E3, 0xE3 },	// �
	{ 0x00E4, 0xE4 },	// �
	{ 0x00E5, 0xE5 },	// �
	{ 0x00E6, 0xE6 },	// �
	{ 0x00E7, 0xE7 },	// �
	{ 0x00E8, 0xE8 },	// �
	{ 0x00E9, 0xE9 },	// �
	{ 0x00EA, 0xEA },	// �
	{ 0x00EB, 0xEB },	// �
	{ 0x00EC, 0xEC },	// �
	{ 0x00ED, 0xED },	// �
	{ 0x00EE, 0xEE },	// �
	{ 0x00EF, 0xEF },	// �
	{ 0x00F0, 0xF0 },	// �
	{ 0x00F1, 0xF1 },	// �
	{ 0x00F2, 0xF2 },	// �
	{ 0x00F3, 0xF3 },	// �
	{ 0x00F4, 0xF4 },	// �
	{ 0x00F5, 0xF5 },	// �
	{ 0x00F6, 0xF6 },	// �
	{ 0x00F7, 0xF7 },	// �
	{ 0x00F8, 0xF8 },	// �
	{ 0x00F9, 0xF9 },	// �
	{ 0x00FA, 0xFA },	// �
	{ 0x00FB, 0xFB },	// �
	{ 0x00FC, 0xFC },	// �
	{ 0x00FD, 0xFD },	// �
	{ 0x00FE, 0xFE },	// �
	{ 0x00FF, 0xFF }	// �
};

/* --------------------------------------------------------------------- */
/* ----- exported datas ------------------------------------------------ */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

unsigned char convertWChar( wchar_t unicode )
{
	int i;

	if( isAscii(unicode) )
	{
		return (unsigned char)unicode;
	}

	for( i=0; i<arraySize( unicodeMapping ); i++ )
	{
		if( unicode == unicodeMapping[i].unicode )
		{
/***/		return unicodeMapping[i].ansiCode;
		}
	}

	return 0;
}

wchar_t convertChar( unsigned char ansiCode )
{
	int i;

	if( isAscii(ansiCode) )
	{
		return ansiCode;
	}

	for( i=0; i<arraySize( unicodeMapping ); i++ )
	{
		if( ansiCode == unicodeMapping[i].ansiCode )
		{
/***/		return unicodeMapping[i].unicode;
		}
	}

	return 0;
}

unsigned long encodeUTF8( wchar_t unicode )
{
	unsigned long	utf8Code = 0, tmpVal;
	size_t			byteCount = 0;

	while( unicode )
	{
		// get 6 lowest bits
		tmpVal = (wchar_t)(unicode & 0x3F);
		unicode >>= 6;
		if( unicode )
		{
			tmpVal |= 0x80;
		}
		else if( byteCount == 1 )
		{
			tmpVal |= 0xC0;
		}
		else if( byteCount == 2 )
		{
			tmpVal |= 0xE0;
		}
		else if( byteCount == 3 )
		{
			tmpVal |= 0xF0;
		}

		tmpVal <<= byteCount * 8;
		utf8Code |= tmpVal;
		byteCount++;
	}

	return utf8Code;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif


