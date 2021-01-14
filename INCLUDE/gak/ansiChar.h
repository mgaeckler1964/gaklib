/*
		Project:		GAKLIB
		Module:			ansiChar.h
		Description:	
		Author:			Martin G‰ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G‰ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G‰ckler, Germany, Munich ``AS IS''
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

#ifndef GAK_ANSI_CHAR_H
#define GAK_ANSI_CHAR_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#define CALCULATE	0

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <ctype.h>

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

#ifdef __cplusplus
namespace gak
{
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#define LOWER_UMLAUT "öúû‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ¯˘˙˚¸˝˛ˇﬂ"
#define UPPER_UMLAUT "äåé¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷ÿŸ⁄€‹›ﬁüﬂ"

#define NULL_CHAR					0
#define START_OF_HEADING			1
#define START_OF_TEXT				2
#define END_OF_TEXT					3
#define END_OF_TRANSMISSION			4
#define ENQUIRY						5
#define ACKNOWLEDGEMENT				6
#define BELL						'\a'
#define BACKSPACE					'\b'
#define HORZIZONTAL_TAB				'\t'
#define LINE_FEED					'\n'
#define VERTICAL_TAB				'\v'
#define FORM_FEED					'\f'
#define CARRIAGE_RETURN				'\r'
#define SHIFT_OUT					14
#define SHIFT_IN					15
#define DATA_LINK_ESCAPE			16
#define XON							17
#define DEVICE_CONTROL_1			17
#define DEVICE_CONTROL_2			18
#define XOFF						19
#define DEVICE_CONTROL_3			19
#define DEVICE_CONTROL_4			20
#define NEGATIVE_ACKNOWLEDGEMENT	21
#define SYNCHRONOUS_IDLE			22
#define END_OF_TRANSMISSION_BLOCK	23
#define CANCEL						24
#define END_OF_MEDIUM				25
#define SUBSTITUTE					26
#define ESCAPE						27
#define FILE_SEPARATOR				28
#define GROUP_SEPARATOR				29
#define RECORD_SEPARATOR			30
#define UNIT_SEPARATOR				31
#define SPACE						32
#define DELETE_CHAR					127
#define NON_BREAKING_SPACE			0xA0
#define SOFT_HYPHEN					0xAD

#define DIGIT_CHAR			0x01
#define UPPER_CHAR			0x02
#define LOWER_CHAR			0x04
#define PUNCTUATION_CHAR	0x08
#define SYMBOL_CHAR			0x10
#define SPACE_CHAR			0x20
#define CONTROL_CHAR		0x40

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __cplusplus
	#ifdef __BORLANDC__
		#pragma warn -ccc
	#endif
		template<typename CharT>
		inline bool isAscii( CharT c )
		{
			return c>=0 && c<=127;
		}
	#ifdef __BORLANDC__
		#pragma warn +ccc
	#endif
#else
	#define isAscii( c )	((c)>=0 && (c)<=127)
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

#ifdef __cplusplus
extern "C"
{
#endif
	extern unsigned char const ansiFlags[];

	extern unsigned char ansiUpperChars[];
	extern unsigned char ansiLowerChars[];
	extern unsigned char ansiOrder[];
	extern unsigned char ansiIgnoreCaseOrder[];
#ifdef __cplusplus
}
#endif
// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

int checkData();

// replacement for strcmp
int ansiCompare( const char *s1, const char *s2 );
// replacement for strncmp
int ansiCompareN( const char *s1, const char *s2, size_t len );
// replacement for strcmpi
int ansiCompareI( const char *s1, const char *s2 );
// replacement for strncmpi
int ansiCompareNI( const char *s1, const char *s2, size_t len );

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

#ifdef __cplusplus
	inline unsigned char ansiToUpper( unsigned char c )
	{
		#if CALCULATE
			return toupper( c );
		#else
			return ansiUpperChars[c];
		#endif
	};
	inline unsigned char ansiToLower( unsigned char c )
	{
		#if CALCULATE
			return tolower( c );
		#else
			return ansiLowerChars[c];
		#endif
	};

	inline bool isUpperLetter( unsigned char c )
	{
		return (ansiFlags[c] & UPPER_CHAR) != 0;
	}
	inline bool isLowerLetter( unsigned char c )
	{
		return (ansiFlags[c] & LOWER_CHAR) != 0;
	}
	inline bool isLetter( unsigned char c )
	{
		return (ansiFlags[c] & (UPPER_CHAR|LOWER_CHAR)) != 0;
	}
	inline bool isDigit( unsigned char c )
	{
		return (ansiFlags[c] & DIGIT_CHAR) != 0;
	}
	inline bool isAlphaNum( unsigned char c )
	{
		return (ansiFlags[c] & (UPPER_CHAR|LOWER_CHAR|DIGIT_CHAR)) != 0;
	}
	inline bool isPunctuation( unsigned char c )
	{
		return (ansiFlags[c] & PUNCTUATION_CHAR) != 0;
	}
	inline bool isSymbol( unsigned char c )
	{
		return (ansiFlags[c] & SYMBOL_CHAR) != 0;
	}
	inline bool isSpace( unsigned char c )
	{
		return (ansiFlags[c] & SPACE_CHAR) != 0;
	}
	inline bool isControl( unsigned char c )
	{
		return (ansiFlags[c] & CONTROL_CHAR) != 0;
	}
	inline bool isControlOrSpace( unsigned char c )
	{
		return (ansiFlags[c] & (SPACE_CHAR|CONTROL_CHAR)) != 0;
	}
#else
	#if CALCULATE
		#define ansiToUpper( c )	toupper(c)
		#define ansiToLower( c )	tolower(c)
	#else
		#define ansiToUpper( c )	ansiUpperChars[(unsigned char)c]
		#define ansiToLower( c )	ansiLowerChars[(unsigned char)c]
	#endif
#endif

#ifdef __cplusplus
}	// namespace gak
#endif

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_ANSI_CHAR_H