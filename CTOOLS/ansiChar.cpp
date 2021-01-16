/*
		Project:		GAKLIB
		Module:			ansiChar.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <iomanip>

#include <gak/ansiChar.h>
#include <gak/set.h>

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

#define TEST_SIZE	0

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

extern "C" {

#if CALCULATE
unsigned char const ansiChars[] =
{
	NULL_CHAR,
	START_OF_HEADING,
	START_OF_TEXT,
	END_OF_TEXT,
	END_OF_TRANSMISSION,
	ENQUIRY,
	ACKNOWLEDGEMENT,
	BELL,
	BACKSPACE,
	HORZIZONTAL_TAB,
	LINE_FEED,
	VERTICAL_TAB,
	FORM_FEED,
	CARRIAGE_RETURN,
	SHIFT_OUT,
	SHIFT_IN,
	DATA_LINK_ESCAPE,
	DEVICE_CONTROL_1,
	DEVICE_CONTROL_2,
	DEVICE_CONTROL_3,
	DEVICE_CONTROL_4,
	NEGATIVE_ACKNOWLEDGEMENT,
	SYNCHRONOUS_IDLE,
	END_OF_TRANSMISSION_BLOCK,
	CANCEL,
	END_OF_MEDIUM,
	SUBSTITUTE,
	ESCAPE,
	FILE_SEPARATOR,
	GROUP_SEPARATOR,
	RECORD_SEPARATOR,
	UNIT_SEPARATOR,
	SPACE,
	'!',
	'\"',
	'#',
	'$',
	'%',
	'&',
	'\'',
	'(',
	')',
	'*',
	'+',
	',',
	'-',
	'.',
	'/',
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	':',
	';',
	'<',
	'=',
	'>',
	'?',
	'@',
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	'[',
	'\\',
	']',
	'^',
	'_',
	'`',
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	'{',
	'|',
	'}',
	'~',
	DELETE_CHAR,
	// Windows 1252 Start
	0x80,	// �
	0x81,	// <--- unkown by windows
	0x82,	// �
	0x83,	// �
	0x84,	// �
	0x85,	// �
	0x86,	// �
	0x87,	// �
	0x88,	// �
	0x89,	// � (Promille)
	0x8A,	// �
	0x8B,	// �
	0x8C,	// �
	0x8D,	// <--- unkown by windows
	0x8E,	// �
	0x8F,	// <--- unkown by windows
	0x90,	// <--- unkown by windows
	0x91,	// �
	0x92,	// �
	0x93,	// �
	0x94,	// �
	0x95,	// �
	0x96,	// � en dash &ndash; (Bindestrich)
	0x97,	// � em dash (Gedankenstrich)
	0x98,	// �
	0x99,	// � (Trademark)
	0x9A,	// �
	0x9B,	// �
	0x9C,	// �
	0x9D,	// <--- unkown by windows
	0x9E,	// �
	0x9F,	// �
	// Windows 1252 end
	NON_BREAKING_SPACE,
	0xA1,	// � (Inverted exclamation mark)
	0xA2,	// � (cent)
	0xA3,	// � (pound)
	0xA4,	// � (currenry)
	0xA5,	// � (Yen)
	0xA6,	// �
	0xA7,	// �
	0xA8,	// �
	0xA9,	// �
	0xAA,	// � (Ordinal indicator feminine)
	0xAB,	// �
	0xAC,	// �
	SOFT_HYPHEN,
	0xAE,	// �
	0xAF,	// � (Macron)
	0xB0,	// �
	0xB1,	// �
	0xB2,	// � (superscript 2)
	0xB3,	// � (superscript 3)
	0xB4,	// �
	0xB5,	// �
	0xB6,	// �
	0xB7,	// �
	0xB8,	// �
	0xB9,	// � (superscript 1)
	0xBA,	// � (Ordinal indicator male)
	0xBB,	// �
	0xBC,	// � (quarter)
	0xBD,	// � (half)
	0xBE,	// � (three quarter)
	0xBF,	// � (Inverted question mark)
	0xC0,	// �
	0xC1,	// �
	0xC2,	// �
	0xC3,	// �
	0xC4,	// �
	0xC5,	// �
	0xC6,	// �
	0xC7,	// �
	0xC8,	// �
	0xC9,	// �
	0xCA,	// �
	0xCB,	// �
	0xCC,	// �
	0xCD,	// �
	0xCE,	// �
	0xCF,	// �
	0xD0,	// �
	0xD1,	// �
	0xD2,	// �
	0xD3,	// �
	0xD4,	// �
	0xD5,	// �
	0xD6,	// �
	0xD7,	// �
	0xD8,	// �
	0xD9,	// �
	0xDA,	// �
	0xDB,	// �
	0xDC,	// �
	0xDD,	// �
	0xDE,	// � (upper thorn)
	0xDF,	// �
	0xE0,	// �
	0xE1,	// �
	0xE2,	// �
	0xE3,	// �
	0xE4,	// �
	0xE5,	// �
	0xE6,	// �
	0xE7,	// �
	0xE8,	// �
	0xE9,	// �
	0xEA,	// �
	0xEB,	// �
	0xEC,	// �
	0xED,	// �
	0xEE,	// �
	0xEF,	// �
	0xF0,	// �
	0xF1,	// �
	0xF2,	// �
	0xF3,	// �
	0xF4,	// �
	0xF5,	// �
	0xF6,	// �
	0xF7,	// �
	0xF8,	// �
	0xF9,	// �
	0xFA,	// �
	0xFB,	// �
	0xFC,	// �
	0xFD,	// �
	0xFE,	// � (lower thorn)
	0xFF,	// �
};

unsigned char const ansiOrdered[] =
{
	// the terminator
	NULL_CHAR,

	// digits
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',

	// letters
	'A',
	0xC0,	// �
	0xC1,	// �
	0xC2,	// �
	0xC3,	// �
	0xC4,	// �
	0xC5,	// �
	0xC6,	// �
	'B',
	'C',
	0xC7,	// �
	'D',
	0xD0,	// �
	'E',
	0xC8,	// �
	0xC9,	// �
	0xCA,	// �
	0xCB,	// �
	'F',
	'G',
	'H',
	'I',
	0xCC,	// �
	0xCD,	// �
	0xCE,	// �
	0xCF,	// �
	'J',
	'K',
	'L',
	'M',
	'N',
	0xD1,	// �
	'O',
	0x8C,	// �
	0xD2,	// �
	0xD3,	// �
	0xD4,	// �
	0xD5,	// �
	0xD6,	// �
	0xD8,	// �
	'P',
	'Q',
	'R',
	'S',
	0x8A,	// �
	'T',
	0xDE,	// � (upper thorn)
	'U',
	0xD9,	// �
	0xDA,	// �
	0xDB,	// �
	0xDC,	// �
	'V',
	'W',
	'X',
	'Y',
	0x9F,	// �
	0xDD,	// �
	'Z',
	0x8E,	// �
	'a',
	0xE0,	// �
	0xE1,	// �
	0xE2,	// �
	0xE3,	// �
	0xE4,	// �
	0xE5,	// �
	0xE6,	// �
	'b',
	'c',
	0xE7,	// �
	'd',
	'e',
	0xE8,	// �
	0xE9,	// �
	0xEA,	// �
	0xEB,	// �
	'f',
	0x83,	// �
	'g',
	'h',
	'i',
	0xEC,	// �
	0xED,	// �
	0xEE,	// �
	0xEF,	// �
	'j',
	'k',
	'l',
	'm',
	'n',
	0xF1,	// �
	'o',
	0x9C,	// �
	0xF0,	// �
	0xF2,	// �
	0xF3,	// �
	0xF4,	// �
	0xF5,	// �
	0xF6,	// �
	0xF8,	// �
	'p',
	'q',
	'r',
	's',
	0x9A,	// �
	0xDF,	// �
	't',
	0xFE,	// � (lower thorn)
	'u',
	0xF9,	// �
	0xFA,	// �
	0xFB,	// �
	0xFC,	// �
	'v',
	'w',
	'x',
	'y',
	0xFD,	// �
	0xFF,	// �
	'z',
	0x9E,	// �

	// punctuation
	'!',
	'\"',
	'\'',
	'(',
	')',
	',',
	'.',
	'/',
	':',
	';',
	'<',
	'>',
	'?',
	'[',
	']',
	'\\',
	'{',
	'}',
	0x82,	// �
	0x84,	// �
	0x85,	// �
	0x8B,	// �
	0x91,	// �
	0x92,	// �
	0x93,	// �
	0x94,	// �
	0x95,	// �
	0x96,	// � en dash &ndash; (Bindestrich)
	0x97,	// � em dash (Gedankenstrich)
	0x9B,	// �
	0xA1,	// � (Inverted exclamation mark)
	0xAB,	// �
	0xB4,	// �
	0xBB,	// �
	0xBF,	// � (Inverted question mark)
	SOFT_HYPHEN,

	// symbols
	'#',
	'$',
	'%',
	'&',
	'*',
	'+',
	'-',
	'=',
	'@',
	'^',
	'_',
	'`',
	'|',
	'~',
	0x80,	// �
	0x86,	// �
	0x87,	// �
	0x88,	// �
	0x89,	// � (Promille)
	0x98,	// �
	0x99,	// � (Trademark)
	// Windows 1252 end
	0xA2,	// � (cent)
	0xA3,	// � (pound)
	0xA4,	// � (currenry)
	0xA5,	// � (Yen)
	0xA6,	// �
	0xA7,	// �
	0xA8,	// �
	0xA9,	// �
	0xAA,	// � (Ordinal indicator feminine)
	0xAC,	// �
	0xAE,	// �
	0xAF,	// � (Macron)
	0xB0,	// �
	0xB1,	// �
	0xB2,	// � (superscript 2)
	0xB3,	// � (superscript 3)
	0xB5,	// �
	0xB6,	// �
	0xB7,	// �
	0xB8,	// �
	0xB9,	// � (superscript 1)
	0xBA,	// � (Ordinal indicator male)
	0xBC,	// � (quarter)
	0xBD,	// � (half)
	0xBE,	// � (three quarter)
	0xD7,	// �
	0xF7,	// �

	// spaces
	HORZIZONTAL_TAB,
	LINE_FEED,
	VERTICAL_TAB,
	FORM_FEED,
	CARRIAGE_RETURN,
	SPACE,
	NON_BREAKING_SPACE,

	// control
	START_OF_HEADING,
	START_OF_TEXT,
	END_OF_TEXT,
	END_OF_TRANSMISSION,
	ENQUIRY,
	ACKNOWLEDGEMENT,
	BELL,
	BACKSPACE,
	SHIFT_OUT,
	SHIFT_IN,
	DATA_LINK_ESCAPE,
	DEVICE_CONTROL_1,
	DEVICE_CONTROL_2,
	DEVICE_CONTROL_3,
	DEVICE_CONTROL_4,
	NEGATIVE_ACKNOWLEDGEMENT,
	SYNCHRONOUS_IDLE,
	END_OF_TRANSMISSION_BLOCK,
	CANCEL,
	END_OF_MEDIUM,
	SUBSTITUTE,
	ESCAPE,
	FILE_SEPARATOR,
	GROUP_SEPARATOR,
	RECORD_SEPARATOR,
	UNIT_SEPARATOR,
	DELETE_CHAR,
	0x81,	// <--- unkown by windows
	0x8D,	// <--- unkown by windows
	0x8F,	// <--- unkown by windows
	0x90,	// <--- unkown by windows
	0x9D,	// <--- unkown by windows
};
#endif // CALCULATE

unsigned char const ansiFlags[] =
{
	CONTROL_CHAR,		//NULL_CHAR,
	CONTROL_CHAR,		//START_OF_HEADING,
	CONTROL_CHAR,		//START_OF_TEXT,
	CONTROL_CHAR,		//END_OF_TEXT,
	CONTROL_CHAR,		//END_OF_TRANSMISSION,
	CONTROL_CHAR,		//ENQUIRY,
	CONTROL_CHAR,		//ACKNOWLEDGEMENT,
	CONTROL_CHAR,		//BELL,
	CONTROL_CHAR,		//BACKSPACE,
	SPACE_CHAR,			//HORZIZONTAL_TAB,
	SPACE_CHAR,			//LINE_FEED,
	SPACE_CHAR,			//VERTICAL_TAB,
	SPACE_CHAR,			//FORM_FEED,
	SPACE_CHAR,			//CARRIAGE_RETURN,
	CONTROL_CHAR,		//SHIFT_OUT,
	CONTROL_CHAR,		//SHIFT_IN,
	CONTROL_CHAR,		//DATA_LINK_ESCAPE,
	CONTROL_CHAR,		//DEVICE_CONTROL_1,
	CONTROL_CHAR,		//DEVICE_CONTROL_2,
	CONTROL_CHAR,		//DEVICE_CONTROL_3,
	CONTROL_CHAR,		//DEVICE_CONTROL_4,
	CONTROL_CHAR,		//NEGATIVE_ACKNOWLEDGEMENT,
	CONTROL_CHAR,		//SYNCHRONOUS_IDLE,
	CONTROL_CHAR,		//END_OF_TRANSMISSION_BLOCK,
	CONTROL_CHAR,		//CANCEL,
	CONTROL_CHAR,		//END_OF_MEDIUM,
	CONTROL_CHAR,		//SUBSTITUTE,
	CONTROL_CHAR,		//ESCAPE,
	CONTROL_CHAR,		//FILE_SEPARATOR,
	CONTROL_CHAR,		//GROUP_SEPARATOR,
	CONTROL_CHAR,		//RECORD_SEPARATOR,
	CONTROL_CHAR,		//UNIT_SEPARATOR,
	SPACE_CHAR,			//SPACE,
	PUNCTUATION_CHAR,	//'!',
	PUNCTUATION_CHAR,	//'\"',
	SYMBOL_CHAR,		//'#',
	SYMBOL_CHAR,		//'$',
	SYMBOL_CHAR,		//'%',
	SYMBOL_CHAR,		//'&',
	PUNCTUATION_CHAR,	//'\'',
	PUNCTUATION_CHAR,	//'(',
	PUNCTUATION_CHAR,	//')',
	SYMBOL_CHAR,		//'*',
	SYMBOL_CHAR,		//'+',
	PUNCTUATION_CHAR,	//',',
	SYMBOL_CHAR,		//'-',
	PUNCTUATION_CHAR,	//'.',
	PUNCTUATION_CHAR,	//'/',
	DIGIT_CHAR,			//'0',
	DIGIT_CHAR,			//'1',
	DIGIT_CHAR,			//'2',
	DIGIT_CHAR,			//'3',
	DIGIT_CHAR,			//'4',
	DIGIT_CHAR,			//'5',
	DIGIT_CHAR,			//'6',
	DIGIT_CHAR,			//'7',
	DIGIT_CHAR,			//'8',
	DIGIT_CHAR,			//'9',
	PUNCTUATION_CHAR,	//':',
	PUNCTUATION_CHAR,	//';',
	PUNCTUATION_CHAR,	//'<',
	SYMBOL_CHAR,		//'=',
	PUNCTUATION_CHAR,	//'>',
	PUNCTUATION_CHAR,	//'?',
	SYMBOL_CHAR,		//'@',
	UPPER_CHAR,			//'A',
	UPPER_CHAR,			//'B',
	UPPER_CHAR,			//'C',
	UPPER_CHAR,			//'D',
	UPPER_CHAR,			//'E',
	UPPER_CHAR,			//'F',
	UPPER_CHAR,			//'G',
	UPPER_CHAR,			//'H',
	UPPER_CHAR,			//'I',
	UPPER_CHAR,			//'J',
	UPPER_CHAR,			//'K',
	UPPER_CHAR,			//'L',
	UPPER_CHAR,			//'M',
	UPPER_CHAR,			//'N',
	UPPER_CHAR,			//'O',
	UPPER_CHAR,			//'P',
	UPPER_CHAR,			//'Q',
	UPPER_CHAR,			//'R',
	UPPER_CHAR,			//'S',
	UPPER_CHAR,			//'T',
	UPPER_CHAR,			//'U',
	UPPER_CHAR,			//'V',
	UPPER_CHAR,			//'W',
	UPPER_CHAR,			//'X',
	UPPER_CHAR,			//'Y',
	UPPER_CHAR,			//'Z',
	PUNCTUATION_CHAR,	//'[',
	PUNCTUATION_CHAR,	//'\\',
	PUNCTUATION_CHAR,	//']',
	SYMBOL_CHAR,		//'^',
	SYMBOL_CHAR,		//'_',
	SYMBOL_CHAR,		//'`',
	LOWER_CHAR,			//'a',
	LOWER_CHAR,			//'b',
	LOWER_CHAR,			//'c',
	LOWER_CHAR,			//'d',
	LOWER_CHAR,			//'e',
	LOWER_CHAR,			//'f',
	LOWER_CHAR,			//'g',
	LOWER_CHAR,			//'h',
	LOWER_CHAR,			//'i',
	LOWER_CHAR,			//'j',
	LOWER_CHAR,			//'k',
	LOWER_CHAR,			//'l',
	LOWER_CHAR,			//'m',
	LOWER_CHAR,			//'n',
	LOWER_CHAR,			//'o',
	LOWER_CHAR,			//'p',
	LOWER_CHAR,			//'q',
	LOWER_CHAR,			//'r',
	LOWER_CHAR,			//'s',
	LOWER_CHAR,			//'t',
	LOWER_CHAR,			//'u',
	LOWER_CHAR,			//'v',
	LOWER_CHAR,			//'w',
	LOWER_CHAR,			//'x',
	LOWER_CHAR,			//'y',
	LOWER_CHAR,			//'z',
	PUNCTUATION_CHAR,	//'{',
	SYMBOL_CHAR,		//'|',
	PUNCTUATION_CHAR,	//'}',
	SYMBOL_CHAR,		//'~',
	CONTROL_CHAR,		//DELETE_CHAR,
	//// Windows 1252 Start
	SYMBOL_CHAR,		//0x80,	// �
	CONTROL_CHAR,		//0x81,	// <--- unkown by windows
	PUNCTUATION_CHAR,	//0x82,	// �
	LOWER_CHAR,			//0x83,	// �
	PUNCTUATION_CHAR,	//0x84,	// �
	PUNCTUATION_CHAR,	//0x85,	// �
	SYMBOL_CHAR,		//0x86,	// �
	SYMBOL_CHAR,		//0x87,	// �
	SYMBOL_CHAR,		//0x88,	// �
	SYMBOL_CHAR,		//0x89,	// � (Promille)
	UPPER_CHAR,			//0x8A,	// �
	PUNCTUATION_CHAR,	//0x8B,	// �
	UPPER_CHAR,			//0x8C,	// �
	CONTROL_CHAR,		//0x8D,	// <--- unkown by windows
	UPPER_CHAR,			//0x8E,	// �
	CONTROL_CHAR,		//0x8F,	// <--- unkown by windows
	CONTROL_CHAR,		//0x90,	// <--- unkown by windows
	PUNCTUATION_CHAR,	//0x91,	// �
	PUNCTUATION_CHAR,	//0x92,	// �
	PUNCTUATION_CHAR,	//0x93,	// �
	PUNCTUATION_CHAR,	//0x94,	// �
	PUNCTUATION_CHAR,	//0x95,	// �
	PUNCTUATION_CHAR,	//0x96,	// � en dash &ndash; (Bindestrich)
	PUNCTUATION_CHAR,	//0x97,	// � em dash (Gedankenstrich)
	SYMBOL_CHAR,		//0x98,	// �
	SYMBOL_CHAR,		//0x99,	// � (Trademark)
	LOWER_CHAR,			//0x9A,	// �
	PUNCTUATION_CHAR,	//0x9B,	// �
	LOWER_CHAR,			//0x9C,	// �
	CONTROL_CHAR,		//0x9D,	// <--- unkown by windows
	LOWER_CHAR,			//0x9E,	// �
	UPPER_CHAR,			//0x9F,	// �
	//// Windows 1252 end
	SPACE_CHAR,			//NON_BREAKING_SPACE,
	PUNCTUATION_CHAR,	//0xA1,	// � (Inverted exclamation mark)
	SYMBOL_CHAR,		//0xA2,	// � (cent)
	SYMBOL_CHAR,		//0xA3,	// � (pound)
	SYMBOL_CHAR,		//0xA4,	// � (currenry)
	SYMBOL_CHAR,		//0xA5,	// � (Yen)
	SYMBOL_CHAR,		//0xA6,	// �
	SYMBOL_CHAR,		//0xA7,	// �
	SYMBOL_CHAR,		//0xA8,	// �
	SYMBOL_CHAR,		//0xA9,	// �
	SYMBOL_CHAR,		//0xAA,	// � (Ordinal indicator feminine)
	PUNCTUATION_CHAR,	//0xAB,	// �
	SYMBOL_CHAR,		//0xAC,	// �
	PUNCTUATION_CHAR,	//SOFT_HYPHEN,
	SYMBOL_CHAR,		//0xAE,	// �
	SYMBOL_CHAR,		//0xAF,	// � (Macron)
	SYMBOL_CHAR,		//0xB0,	// �
	SYMBOL_CHAR,		//0xB1,	// �
	SYMBOL_CHAR,		//0xB2,	// � (superscript 2)
	SYMBOL_CHAR,		//0xB3,	// � (superscript 3)
	PUNCTUATION_CHAR,	//0xB4,	// �
	SYMBOL_CHAR,		//0xB5,	// �
	SYMBOL_CHAR,		//0xB6,	// �
	SYMBOL_CHAR,		//0xB7,	// �
	SYMBOL_CHAR,		//0xB8,	// �
	SYMBOL_CHAR,		//0xB9,	// � (superscript 1)
	SYMBOL_CHAR,		//0xBA,	// � (Ordinal indicator male)
	PUNCTUATION_CHAR,	//0xBB,	// �
	SYMBOL_CHAR,		//0xBC,	// � (quarter)
	SYMBOL_CHAR,		//0xBD,	// � (half)
	SYMBOL_CHAR,		//0xBE,	// � (three quarter)
	PUNCTUATION_CHAR,	//0xBF,	// � (Inverted question mark)
	UPPER_CHAR,			//0xC0,	// �
	UPPER_CHAR,			//0xC1,	// �
	UPPER_CHAR,			//0xC2,	// �
	UPPER_CHAR,			//0xC3,	// �
	UPPER_CHAR,			//0xC4,	// �
	UPPER_CHAR,			//0xC5,	// �
	UPPER_CHAR,			//0xC6,	// �
	UPPER_CHAR,			//0xC7,	// �
	UPPER_CHAR,			//0xC8,	// �
	UPPER_CHAR,			//0xC9,	// �
	UPPER_CHAR,			//0xCA,	// �
	UPPER_CHAR,			//0xCB,	// �
	UPPER_CHAR,			//0xCC,	// �
	UPPER_CHAR,			//0xCD,	// �
	UPPER_CHAR,			//0xCE,	// �
	UPPER_CHAR,			//0xCF,	// �
	UPPER_CHAR,			//0xD0,	// �
	UPPER_CHAR,			//0xD1,	// �
	UPPER_CHAR,			//0xD2,	// �
	UPPER_CHAR,			//0xD3,	// �
	UPPER_CHAR,			//0xD4,	// �
	UPPER_CHAR,			//0xD5,	// �
	UPPER_CHAR,			//0xD6,	// �
	SYMBOL_CHAR,		//0xD7,	// �
	UPPER_CHAR,			//0xD8,	// �
	UPPER_CHAR,			//0xD9,	// �
	UPPER_CHAR,			//0xDA,	// �
	UPPER_CHAR,			//0xDB,	// �
	UPPER_CHAR,			//0xDC,	// �
	UPPER_CHAR,			//0xDD,	// �
	UPPER_CHAR,			//0xDE,	// � (upper thorn)
	LOWER_CHAR,			//0xDF,	// �
	LOWER_CHAR,			//0xE0,	// �
	LOWER_CHAR,			//0xE1,	// �
	LOWER_CHAR,			//0xE2,	// �
	LOWER_CHAR,			//0xE3,	// �
	LOWER_CHAR,			//0xE4,	// �
	LOWER_CHAR,			//0xE5,	// �
	LOWER_CHAR,			//0xE6,	// �
	LOWER_CHAR,			//0xE7,	// �
	LOWER_CHAR,			//0xE8,	// �
	LOWER_CHAR,			//0xE9,	// �
	LOWER_CHAR,			//0xEA,	// �
	LOWER_CHAR,			//0xEB,	// �
	LOWER_CHAR,			//0xEC,	// �
	LOWER_CHAR,			//0xED,	// �
	LOWER_CHAR,			//0xEE,	// �
	LOWER_CHAR,			//0xEF,	// �
	LOWER_CHAR,			//0xF0,	// �
	LOWER_CHAR,			//0xF1,	// �
	LOWER_CHAR,			//0xF2,	// �
	LOWER_CHAR,			//0xF3,	// �
	LOWER_CHAR,			//0xF4,	// �
	LOWER_CHAR,			//0xF5,	// �
	LOWER_CHAR,			//0xF6,	// �
	SYMBOL_CHAR,		//0xF7,	// �
	LOWER_CHAR,			//0xF8,	// �
	LOWER_CHAR,			//0xF9,	// �
	LOWER_CHAR,			//0xFA,	// �
	LOWER_CHAR,			//0xFB,	// �
	LOWER_CHAR,			//0xFC,	// �
	LOWER_CHAR,			//0xFD,	// �
	LOWER_CHAR,			//0xFE,	// � (lower thorn)
	LOWER_CHAR,			//0xFF	// �
};

#if CALCULATE
unsigned char ansiUpperChars[256];
unsigned char ansiLowerChars[256];
unsigned char ansiOrder[256];
unsigned char ansiIgnoreCaseOrder[256];
#else
unsigned char ansiUpperChars[256] = {
	  0,	// = 0x00 index 0x00=000
	  1,	// = 0x01 index 0x01=001
	  2,	// = 0x02 index 0x02=002
	  3,	// = 0x03 index 0x03=003
	  4,	// = 0x04 index 0x04=004
	  5,	// = 0x05 index 0x05=005
	  6,	// = 0x06 index 0x06=006
	  7,	// = 0x07 index 0x07=007
	  8,	// = 0x08 index 0x08=008
	  9,	// = 0x09 index 0x09=009
	 10,	// = 0x0a index 0x0a=010
	 11,	// = 0x0b index 0x0b=011
	 12,	// = 0x0c index 0x0c=012
	 13,	// = 0x0d index 0x0d=013
	 14,	// = 0x0e index 0x0e=014
	 15,	// = 0x0f index 0x0f=015
	 16,	// = 0x10 index 0x10=016
	 17,	// = 0x11 index 0x11=017
	 18,	// = 0x12 index 0x12=018
	 19,	// = 0x13 index 0x13=019
	 20,	// = 0x14 index 0x14=020
	 21,	// = 0x15 index 0x15=021
	 22,	// = 0x16 index 0x16=022
	 23,	// = 0x17 index 0x17=023
	 24,	// = 0x18 index 0x18=024
	 25,	// = 0x19 index 0x19=025
	 26,	// = 0x1a index 0x1a=026
	 27,	// = 0x1b index 0x1b=027
	 28,	// = 0x1c index 0x1c=028
	 29,	// = 0x1d index 0x1d=029
	 30,	// = 0x1e index 0x1e=030
	 31,	// = 0x1f index 0x1f=031
	 32,	// = 0x20 index 0x20=032
	 33,	// = 0x21 index 0x21=033 for character '!'
	 34,	// = 0x22 index 0x22=034 for character '"'
	 35,	// = 0x23 index 0x23=035 for character '#'
	 36,	// = 0x24 index 0x24=036 for character '$'
	 37,	// = 0x25 index 0x25=037 for character '%'
	 38,	// = 0x26 index 0x26=038 for character '&'
	 39,	// = 0x27 index 0x27=039 for character '''
	 40,	// = 0x28 index 0x28=040 for character '('
	 41,	// = 0x29 index 0x29=041 for character ')'
	 42,	// = 0x2a index 0x2a=042 for character '*'
	 43,	// = 0x2b index 0x2b=043 for character '+'
	 44,	// = 0x2c index 0x2c=044 for character ','
	 45,	// = 0x2d index 0x2d=045 for character '-'
	 46,	// = 0x2e index 0x2e=046 for character '.'
	 47,	// = 0x2f index 0x2f=047 for character '/'
	 48,	// = 0x30 index 0x30=048 for character '0'
	 49,	// = 0x31 index 0x31=049 for character '1'
	 50,	// = 0x32 index 0x32=050 for character '2'
	 51,	// = 0x33 index 0x33=051 for character '3'
	 52,	// = 0x34 index 0x34=052 for character '4'
	 53,	// = 0x35 index 0x35=053 for character '5'
	 54,	// = 0x36 index 0x36=054 for character '6'
	 55,	// = 0x37 index 0x37=055 for character '7'
	 56,	// = 0x38 index 0x38=056 for character '8'
	 57,	// = 0x39 index 0x39=057 for character '9'
	 58,	// = 0x3a index 0x3a=058 for character ':'
	 59,	// = 0x3b index 0x3b=059 for character ';'
	 60,	// = 0x3c index 0x3c=060 for character '<'
	 61,	// = 0x3d index 0x3d=061 for character '='
	 62,	// = 0x3e index 0x3e=062 for character '>'
	 63,	// = 0x3f index 0x3f=063 for character '?'
	 64,	// = 0x40 index 0x40=064 for character '@'
	 65,	// = 0x41 index 0x41=065 for character 'A'
	 66,	// = 0x42 index 0x42=066 for character 'B'
	 67,	// = 0x43 index 0x43=067 for character 'C'
	 68,	// = 0x44 index 0x44=068 for character 'D'
	 69,	// = 0x45 index 0x45=069 for character 'E'
	 70,	// = 0x46 index 0x46=070 for character 'F'
	 71,	// = 0x47 index 0x47=071 for character 'G'
	 72,	// = 0x48 index 0x48=072 for character 'H'
	 73,	// = 0x49 index 0x49=073 for character 'I'
	 74,	// = 0x4a index 0x4a=074 for character 'J'
	 75,	// = 0x4b index 0x4b=075 for character 'K'
	 76,	// = 0x4c index 0x4c=076 for character 'L'
	 77,	// = 0x4d index 0x4d=077 for character 'M'
	 78,	// = 0x4e index 0x4e=078 for character 'N'
	 79,	// = 0x4f index 0x4f=079 for character 'O'
	 80,	// = 0x50 index 0x50=080 for character 'P'
	 81,	// = 0x51 index 0x51=081 for character 'Q'
	 82,	// = 0x52 index 0x52=082 for character 'R'
	 83,	// = 0x53 index 0x53=083 for character 'S'
	 84,	// = 0x54 index 0x54=084 for character 'T'
	 85,	// = 0x55 index 0x55=085 for character 'U'
	 86,	// = 0x56 index 0x56=086 for character 'V'
	 87,	// = 0x57 index 0x57=087 for character 'W'
	 88,	// = 0x58 index 0x58=088 for character 'X'
	 89,	// = 0x59 index 0x59=089 for character 'Y'
	 90,	// = 0x5a index 0x5a=090 for character 'Z'
	 91,	// = 0x5b index 0x5b=091 for character '['
	 92,	// = 0x5c index 0x5c=092 for character '\'
	 93,	// = 0x5d index 0x5d=093 for character ']'
	 94,	// = 0x5e index 0x5e=094 for character '^'
	 95,	// = 0x5f index 0x5f=095 for character '_'
	 96,	// = 0x60 index 0x60=096 for character '`'
	 65,	// = 0x41 index 0x61=097 for character 'a'
	 66,	// = 0x42 index 0x62=098 for character 'b'
	 67,	// = 0x43 index 0x63=099 for character 'c'
	 68,	// = 0x44 index 0x64=100 for character 'd'
	 69,	// = 0x45 index 0x65=101 for character 'e'
	 70,	// = 0x46 index 0x66=102 for character 'f'
	 71,	// = 0x47 index 0x67=103 for character 'g'
	 72,	// = 0x48 index 0x68=104 for character 'h'
	 73,	// = 0x49 index 0x69=105 for character 'i'
	 74,	// = 0x4a index 0x6a=106 for character 'j'
	 75,	// = 0x4b index 0x6b=107 for character 'k'
	 76,	// = 0x4c index 0x6c=108 for character 'l'
	 77,	// = 0x4d index 0x6d=109 for character 'm'
	 78,	// = 0x4e index 0x6e=110 for character 'n'
	 79,	// = 0x4f index 0x6f=111 for character 'o'
	 80,	// = 0x50 index 0x70=112 for character 'p'
	 81,	// = 0x51 index 0x71=113 for character 'q'
	 82,	// = 0x52 index 0x72=114 for character 'r'
	 83,	// = 0x53 index 0x73=115 for character 's'
	 84,	// = 0x54 index 0x74=116 for character 't'
	 85,	// = 0x55 index 0x75=117 for character 'u'
	 86,	// = 0x56 index 0x76=118 for character 'v'
	 87,	// = 0x57 index 0x77=119 for character 'w'
	 88,	// = 0x58 index 0x78=120 for character 'x'
	 89,	// = 0x59 index 0x79=121 for character 'y'
	 90,	// = 0x5a index 0x7a=122 for character 'z'
	123,	// = 0x7b index 0x7b=123 for character '{'
	124,	// = 0x7c index 0x7c=124 for character '|'
	125,	// = 0x7d index 0x7d=125 for character '}'
	126,	// = 0x7e index 0x7e=126 for character '~'
	127,	// = 0x7f index 0x7f=127
	128,	// = 0x80 index 0x80=128 for character '�'
	129,	// = 0x81 index 0x81=129
	130,	// = 0x82 index 0x82=130 for character '�'
	131,	// = 0x83 index 0x83=131 for character '�'
	132,	// = 0x84 index 0x84=132 for character '�'
	133,	// = 0x85 index 0x85=133 for character '�'
	134,	// = 0x86 index 0x86=134 for character '�'
	135,	// = 0x87 index 0x87=135 for character '�'
	136,	// = 0x88 index 0x88=136 for character '�'
	137,	// = 0x89 index 0x89=137 for character '�'
	138,	// = 0x8a index 0x8a=138 for character '�'
	139,	// = 0x8b index 0x8b=139 for character '�'
	140,	// = 0x8c index 0x8c=140 for character '�'
	141,	// = 0x8d index 0x8d=141
	142,	// = 0x8e index 0x8e=142 for character '�'
	143,	// = 0x8f index 0x8f=143
	144,	// = 0x90 index 0x90=144
	145,	// = 0x91 index 0x91=145 for character '�'
	146,	// = 0x92 index 0x92=146 for character '�'
	147,	// = 0x93 index 0x93=147 for character '�'
	148,	// = 0x94 index 0x94=148 for character '�'
	149,	// = 0x95 index 0x95=149 for character '�'
	150,	// = 0x96 index 0x96=150 for character '�'
	151,	// = 0x97 index 0x97=151 for character '�'
	152,	// = 0x98 index 0x98=152 for character '�'
	153,	// = 0x99 index 0x99=153 for character '�'
	138,	// = 0x8a index 0x9a=154 for character '�'
	155,	// = 0x9b index 0x9b=155 for character '�'
	140,	// = 0x8c index 0x9c=156 for character '�'
	157,	// = 0x9d index 0x9d=157
	142,	// = 0x8e index 0x9e=158 for character '�'
	159,	// = 0x9f index 0x9f=159 for character '�'
	160,	// = 0xa0 index 0xa0=160
	161,	// = 0xa1 index 0xa1=161 for character '�'
	162,	// = 0xa2 index 0xa2=162 for character '�'
	163,	// = 0xa3 index 0xa3=163 for character '�'
	164,	// = 0xa4 index 0xa4=164 for character '�'
	165,	// = 0xa5 index 0xa5=165 for character '�'
	166,	// = 0xa6 index 0xa6=166 for character '�'
	167,	// = 0xa7 index 0xa7=167 for character '�'
	168,	// = 0xa8 index 0xa8=168 for character '�'
	169,	// = 0xa9 index 0xa9=169 for character '�'
	170,	// = 0xaa index 0xaa=170 for character '�'
	171,	// = 0xab index 0xab=171 for character '�'
	172,	// = 0xac index 0xac=172 for character '�'
	173,	// = 0xad index 0xad=173 for character '�'
	174,	// = 0xae index 0xae=174 for character '�'
	175,	// = 0xaf index 0xaf=175 for character '�'
	176,	// = 0xb0 index 0xb0=176 for character '�'
	177,	// = 0xb1 index 0xb1=177 for character '�'
	178,	// = 0xb2 index 0xb2=178 for character '�'
	179,	// = 0xb3 index 0xb3=179 for character '�'
	180,	// = 0xb4 index 0xb4=180 for character '�'
	181,	// = 0xb5 index 0xb5=181 for character '�'
	182,	// = 0xb6 index 0xb6=182 for character '�'
	183,	// = 0xb7 index 0xb7=183 for character '�'
	184,	// = 0xb8 index 0xb8=184 for character '�'
	185,	// = 0xb9 index 0xb9=185 for character '�'
	186,	// = 0xba index 0xba=186 for character '�'
	187,	// = 0xbb index 0xbb=187 for character '�'
	188,	// = 0xbc index 0xbc=188 for character '�'
	189,	// = 0xbd index 0xbd=189 for character '�'
	190,	// = 0xbe index 0xbe=190 for character '�'
	191,	// = 0xbf index 0xbf=191 for character '�'
	192,	// = 0xc0 index 0xc0=192 for character '�'
	193,	// = 0xc1 index 0xc1=193 for character '�'
	194,	// = 0xc2 index 0xc2=194 for character '�'
	195,	// = 0xc3 index 0xc3=195 for character '�'
	196,	// = 0xc4 index 0xc4=196 for character '�'
	197,	// = 0xc5 index 0xc5=197 for character '�'
	198,	// = 0xc6 index 0xc6=198 for character '�'
	199,	// = 0xc7 index 0xc7=199 for character '�'
	200,	// = 0xc8 index 0xc8=200 for character '�'
	201,	// = 0xc9 index 0xc9=201 for character '�'
	202,	// = 0xca index 0xca=202 for character '�'
	203,	// = 0xcb index 0xcb=203 for character '�'
	204,	// = 0xcc index 0xcc=204 for character '�'
	205,	// = 0xcd index 0xcd=205 for character '�'
	206,	// = 0xce index 0xce=206 for character '�'
	207,	// = 0xcf index 0xcf=207 for character '�'
	208,	// = 0xd0 index 0xd0=208 for character '�'
	209,	// = 0xd1 index 0xd1=209 for character '�'
	210,	// = 0xd2 index 0xd2=210 for character '�'
	211,	// = 0xd3 index 0xd3=211 for character '�'
	212,	// = 0xd4 index 0xd4=212 for character '�'
	213,	// = 0xd5 index 0xd5=213 for character '�'
	214,	// = 0xd6 index 0xd6=214 for character '�'
	215,	// = 0xd7 index 0xd7=215 for character '�'
	216,	// = 0xd8 index 0xd8=216 for character '�'
	217,	// = 0xd9 index 0xd9=217 for character '�'
	218,	// = 0xda index 0xda=218 for character '�'
	219,	// = 0xdb index 0xdb=219 for character '�'
	220,	// = 0xdc index 0xdc=220 for character '�'
	221,	// = 0xdd index 0xdd=221 for character '�'
	222,	// = 0xde index 0xde=222 for character '�'
	223,	// = 0xdf index 0xdf=223 for character '�'
	192,	// = 0xc0 index 0xe0=224 for character '�'
	193,	// = 0xc1 index 0xe1=225 for character '�'
	194,	// = 0xc2 index 0xe2=226 for character '�'
	195,	// = 0xc3 index 0xe3=227 for character '�'
	196,	// = 0xc4 index 0xe4=228 for character '�'
	197,	// = 0xc5 index 0xe5=229 for character '�'
	198,	// = 0xc6 index 0xe6=230 for character '�'
	199,	// = 0xc7 index 0xe7=231 for character '�'
	200,	// = 0xc8 index 0xe8=232 for character '�'
	201,	// = 0xc9 index 0xe9=233 for character '�'
	202,	// = 0xca index 0xea=234 for character '�'
	203,	// = 0xcb index 0xeb=235 for character '�'
	204,	// = 0xcc index 0xec=236 for character '�'
	205,	// = 0xcd index 0xed=237 for character '�'
	206,	// = 0xce index 0xee=238 for character '�'
	207,	// = 0xcf index 0xef=239 for character '�'
	208,	// = 0xd0 index 0xf0=240 for character '�'
	209,	// = 0xd1 index 0xf1=241 for character '�'
	210,	// = 0xd2 index 0xf2=242 for character '�'
	211,	// = 0xd3 index 0xf3=243 for character '�'
	212,	// = 0xd4 index 0xf4=244 for character '�'
	213,	// = 0xd5 index 0xf5=245 for character '�'
	214,	// = 0xd6 index 0xf6=246 for character '�'
	247,	// = 0xf7 index 0xf7=247 for character '�'
	216,	// = 0xd8 index 0xf8=248 for character '�'
	217,	// = 0xd9 index 0xf9=249 for character '�'
	218,	// = 0xda index 0xfa=250 for character '�'
	219,	// = 0xdb index 0xfb=251 for character '�'
	220,	// = 0xdc index 0xfc=252 for character '�'
	221,	// = 0xdd index 0xfd=253 for character '�'
	222,	// = 0xde index 0xfe=254 for character '�'
	159,	// = 0x9f index 0xff=255 for character '�'
};

unsigned char ansiLowerChars[256] = {
	  0,	// = 0x00 index 0x00=000
	  1,	// = 0x01 index 0x01=001
	  2,	// = 0x02 index 0x02=002
	  3,	// = 0x03 index 0x03=003
	  4,	// = 0x04 index 0x04=004
	  5,	// = 0x05 index 0x05=005
	  6,	// = 0x06 index 0x06=006
	  7,	// = 0x07 index 0x07=007
	  8,	// = 0x08 index 0x08=008
	  9,	// = 0x09 index 0x09=009
	 10,	// = 0x0a index 0x0a=010
	 11,	// = 0x0b index 0x0b=011
	 12,	// = 0x0c index 0x0c=012
	 13,	// = 0x0d index 0x0d=013
	 14,	// = 0x0e index 0x0e=014
	 15,	// = 0x0f index 0x0f=015
	 16,	// = 0x10 index 0x10=016
	 17,	// = 0x11 index 0x11=017
	 18,	// = 0x12 index 0x12=018
	 19,	// = 0x13 index 0x13=019
	 20,	// = 0x14 index 0x14=020
	 21,	// = 0x15 index 0x15=021
	 22,	// = 0x16 index 0x16=022
	 23,	// = 0x17 index 0x17=023
	 24,	// = 0x18 index 0x18=024
	 25,	// = 0x19 index 0x19=025
	 26,	// = 0x1a index 0x1a=026
	 27,	// = 0x1b index 0x1b=027
	 28,	// = 0x1c index 0x1c=028
	 29,	// = 0x1d index 0x1d=029
	 30,	// = 0x1e index 0x1e=030
	 31,	// = 0x1f index 0x1f=031
	 32,	// = 0x20 index 0x20=032
	 33,	// = 0x21 index 0x21=033 for character '!'
	 34,	// = 0x22 index 0x22=034 for character '"'
	 35,	// = 0x23 index 0x23=035 for character '#'
	 36,	// = 0x24 index 0x24=036 for character '$'
	 37,	// = 0x25 index 0x25=037 for character '%'
	 38,	// = 0x26 index 0x26=038 for character '&'
	 39,	// = 0x27 index 0x27=039 for character '''
	 40,	// = 0x28 index 0x28=040 for character '('
	 41,	// = 0x29 index 0x29=041 for character ')'
	 42,	// = 0x2a index 0x2a=042 for character '*'
	 43,	// = 0x2b index 0x2b=043 for character '+'
	 44,	// = 0x2c index 0x2c=044 for character ','
	 45,	// = 0x2d index 0x2d=045 for character '-'
	 46,	// = 0x2e index 0x2e=046 for character '.'
	 47,	// = 0x2f index 0x2f=047 for character '/'
	 48,	// = 0x30 index 0x30=048 for character '0'
	 49,	// = 0x31 index 0x31=049 for character '1'
	 50,	// = 0x32 index 0x32=050 for character '2'
	 51,	// = 0x33 index 0x33=051 for character '3'
	 52,	// = 0x34 index 0x34=052 for character '4'
	 53,	// = 0x35 index 0x35=053 for character '5'
	 54,	// = 0x36 index 0x36=054 for character '6'
	 55,	// = 0x37 index 0x37=055 for character '7'
	 56,	// = 0x38 index 0x38=056 for character '8'
	 57,	// = 0x39 index 0x39=057 for character '9'
	 58,	// = 0x3a index 0x3a=058 for character ':'
	 59,	// = 0x3b index 0x3b=059 for character ';'
	 60,	// = 0x3c index 0x3c=060 for character '<'
	 61,	// = 0x3d index 0x3d=061 for character '='
	 62,	// = 0x3e index 0x3e=062 for character '>'
	 63,	// = 0x3f index 0x3f=063 for character '?'
	 64,	// = 0x40 index 0x40=064 for character '@'
	 97,	// = 0x61 index 0x41=065 for character 'A'
	 98,	// = 0x62 index 0x42=066 for character 'B'
	 99,	// = 0x63 index 0x43=067 for character 'C'
	100,	// = 0x64 index 0x44=068 for character 'D'
	101,	// = 0x65 index 0x45=069 for character 'E'
	102,	// = 0x66 index 0x46=070 for character 'F'
	103,	// = 0x67 index 0x47=071 for character 'G'
	104,	// = 0x68 index 0x48=072 for character 'H'
	105,	// = 0x69 index 0x49=073 for character 'I'
	106,	// = 0x6a index 0x4a=074 for character 'J'
	107,	// = 0x6b index 0x4b=075 for character 'K'
	108,	// = 0x6c index 0x4c=076 for character 'L'
	109,	// = 0x6d index 0x4d=077 for character 'M'
	110,	// = 0x6e index 0x4e=078 for character 'N'
	111,	// = 0x6f index 0x4f=079 for character 'O'
	112,	// = 0x70 index 0x50=080 for character 'P'
	113,	// = 0x71 index 0x51=081 for character 'Q'
	114,	// = 0x72 index 0x52=082 for character 'R'
	115,	// = 0x73 index 0x53=083 for character 'S'
	116,	// = 0x74 index 0x54=084 for character 'T'
	117,	// = 0x75 index 0x55=085 for character 'U'
	118,	// = 0x76 index 0x56=086 for character 'V'
	119,	// = 0x77 index 0x57=087 for character 'W'
	120,	// = 0x78 index 0x58=088 for character 'X'
	121,	// = 0x79 index 0x59=089 for character 'Y'
	122,	// = 0x7a index 0x5a=090 for character 'Z'
	 91,	// = 0x5b index 0x5b=091 for character '['
	 92,	// = 0x5c index 0x5c=092 for character '\'
	 93,	// = 0x5d index 0x5d=093 for character ']'
	 94,	// = 0x5e index 0x5e=094 for character '^'
	 95,	// = 0x5f index 0x5f=095 for character '_'
	 96,	// = 0x60 index 0x60=096 for character '`'
	 97,	// = 0x61 index 0x61=097 for character 'a'
	 98,	// = 0x62 index 0x62=098 for character 'b'
	 99,	// = 0x63 index 0x63=099 for character 'c'
	100,	// = 0x64 index 0x64=100 for character 'd'
	101,	// = 0x65 index 0x65=101 for character 'e'
	102,	// = 0x66 index 0x66=102 for character 'f'
	103,	// = 0x67 index 0x67=103 for character 'g'
	104,	// = 0x68 index 0x68=104 for character 'h'
	105,	// = 0x69 index 0x69=105 for character 'i'
	106,	// = 0x6a index 0x6a=106 for character 'j'
	107,	// = 0x6b index 0x6b=107 for character 'k'
	108,	// = 0x6c index 0x6c=108 for character 'l'
	109,	// = 0x6d index 0x6d=109 for character 'm'
	110,	// = 0x6e index 0x6e=110 for character 'n'
	111,	// = 0x6f index 0x6f=111 for character 'o'
	112,	// = 0x70 index 0x70=112 for character 'p'
	113,	// = 0x71 index 0x71=113 for character 'q'
	114,	// = 0x72 index 0x72=114 for character 'r'
	115,	// = 0x73 index 0x73=115 for character 's'
	116,	// = 0x74 index 0x74=116 for character 't'
	117,	// = 0x75 index 0x75=117 for character 'u'
	118,	// = 0x76 index 0x76=118 for character 'v'
	119,	// = 0x77 index 0x77=119 for character 'w'
	120,	// = 0x78 index 0x78=120 for character 'x'
	121,	// = 0x79 index 0x79=121 for character 'y'
	122,	// = 0x7a index 0x7a=122 for character 'z'
	123,	// = 0x7b index 0x7b=123 for character '{'
	124,	// = 0x7c index 0x7c=124 for character '|'
	125,	// = 0x7d index 0x7d=125 for character '}'
	126,	// = 0x7e index 0x7e=126 for character '~'
	127,	// = 0x7f index 0x7f=127
	128,	// = 0x80 index 0x80=128 for character '�'
	129,	// = 0x81 index 0x81=129
	130,	// = 0x82 index 0x82=130 for character '�'
	131,	// = 0x83 index 0x83=131 for character '�'
	132,	// = 0x84 index 0x84=132 for character '�'
	133,	// = 0x85 index 0x85=133 for character '�'
	134,	// = 0x86 index 0x86=134 for character '�'
	135,	// = 0x87 index 0x87=135 for character '�'
	136,	// = 0x88 index 0x88=136 for character '�'
	137,	// = 0x89 index 0x89=137 for character '�'
	154,	// = 0x9a index 0x8a=138 for character '�'
	139,	// = 0x8b index 0x8b=139 for character '�'
	156,	// = 0x9c index 0x8c=140 for character '�'
	141,	// = 0x8d index 0x8d=141
	158,	// = 0x9e index 0x8e=142 for character '�'
	143,	// = 0x8f index 0x8f=143
	144,	// = 0x90 index 0x90=144
	145,	// = 0x91 index 0x91=145 for character '�'
	146,	// = 0x92 index 0x92=146 for character '�'
	147,	// = 0x93 index 0x93=147 for character '�'
	148,	// = 0x94 index 0x94=148 for character '�'
	149,	// = 0x95 index 0x95=149 for character '�'
	150,	// = 0x96 index 0x96=150 for character '�'
	151,	// = 0x97 index 0x97=151 for character '�'
	152,	// = 0x98 index 0x98=152 for character '�'
	153,	// = 0x99 index 0x99=153 for character '�'
	154,	// = 0x9a index 0x9a=154 for character '�'
	155,	// = 0x9b index 0x9b=155 for character '�'
	156,	// = 0x9c index 0x9c=156 for character '�'
	157,	// = 0x9d index 0x9d=157
	158,	// = 0x9e index 0x9e=158 for character '�'
	255,	// = 0xff index 0x9f=159 for character '�'
	160,	// = 0xa0 index 0xa0=160
	161,	// = 0xa1 index 0xa1=161 for character '�'
	162,	// = 0xa2 index 0xa2=162 for character '�'
	163,	// = 0xa3 index 0xa3=163 for character '�'
	164,	// = 0xa4 index 0xa4=164 for character '�'
	165,	// = 0xa5 index 0xa5=165 for character '�'
	166,	// = 0xa6 index 0xa6=166 for character '�'
	167,	// = 0xa7 index 0xa7=167 for character '�'
	168,	// = 0xa8 index 0xa8=168 for character '�'
	169,	// = 0xa9 index 0xa9=169 for character '�'
	170,	// = 0xaa index 0xaa=170 for character '�'
	171,	// = 0xab index 0xab=171 for character '�'
	172,	// = 0xac index 0xac=172 for character '�'
	173,	// = 0xad index 0xad=173 for character '�'
	174,	// = 0xae index 0xae=174 for character '�'
	175,	// = 0xaf index 0xaf=175 for character '�'
	176,	// = 0xb0 index 0xb0=176 for character '�'
	177,	// = 0xb1 index 0xb1=177 for character '�'
	178,	// = 0xb2 index 0xb2=178 for character '�'
	179,	// = 0xb3 index 0xb3=179 for character '�'
	180,	// = 0xb4 index 0xb4=180 for character '�'
	181,	// = 0xb5 index 0xb5=181 for character '�'
	182,	// = 0xb6 index 0xb6=182 for character '�'
	183,	// = 0xb7 index 0xb7=183 for character '�'
	184,	// = 0xb8 index 0xb8=184 for character '�'
	185,	// = 0xb9 index 0xb9=185 for character '�'
	186,	// = 0xba index 0xba=186 for character '�'
	187,	// = 0xbb index 0xbb=187 for character '�'
	188,	// = 0xbc index 0xbc=188 for character '�'
	189,	// = 0xbd index 0xbd=189 for character '�'
	190,	// = 0xbe index 0xbe=190 for character '�'
	191,	// = 0xbf index 0xbf=191 for character '�'
	224,	// = 0xe0 index 0xc0=192 for character '�'
	225,	// = 0xe1 index 0xc1=193 for character '�'
	226,	// = 0xe2 index 0xc2=194 for character '�'
	227,	// = 0xe3 index 0xc3=195 for character '�'
	228,	// = 0xe4 index 0xc4=196 for character '�'
	229,	// = 0xe5 index 0xc5=197 for character '�'
	230,	// = 0xe6 index 0xc6=198 for character '�'
	231,	// = 0xe7 index 0xc7=199 for character '�'
	232,	// = 0xe8 index 0xc8=200 for character '�'
	233,	// = 0xe9 index 0xc9=201 for character '�'
	234,	// = 0xea index 0xca=202 for character '�'
	235,	// = 0xeb index 0xcb=203 for character '�'
	236,	// = 0xec index 0xcc=204 for character '�'
	237,	// = 0xed index 0xcd=205 for character '�'
	238,	// = 0xee index 0xce=206 for character '�'
	239,	// = 0xef index 0xcf=207 for character '�'
	240,	// = 0xf0 index 0xd0=208 for character '�'
	241,	// = 0xf1 index 0xd1=209 for character '�'
	242,	// = 0xf2 index 0xd2=210 for character '�'
	243,	// = 0xf3 index 0xd3=211 for character '�'
	244,	// = 0xf4 index 0xd4=212 for character '�'
	245,	// = 0xf5 index 0xd5=213 for character '�'
	246,	// = 0xf6 index 0xd6=214 for character '�'
	215,	// = 0xd7 index 0xd7=215 for character '�'
	248,	// = 0xf8 index 0xd8=216 for character '�'
	249,	// = 0xf9 index 0xd9=217 for character '�'
	250,	// = 0xfa index 0xda=218 for character '�'
	251,	// = 0xfb index 0xdb=219 for character '�'
	252,	// = 0xfc index 0xdc=220 for character '�'
	253,	// = 0xfd index 0xdd=221 for character '�'
	254,	// = 0xfe index 0xde=222 for character '�'
	223,	// = 0xdf index 0xdf=223 for character '�'
	224,	// = 0xe0 index 0xe0=224 for character '�'
	225,	// = 0xe1 index 0xe1=225 for character '�'
	226,	// = 0xe2 index 0xe2=226 for character '�'
	227,	// = 0xe3 index 0xe3=227 for character '�'
	228,	// = 0xe4 index 0xe4=228 for character '�'
	229,	// = 0xe5 index 0xe5=229 for character '�'
	230,	// = 0xe6 index 0xe6=230 for character '�'
	231,	// = 0xe7 index 0xe7=231 for character '�'
	232,	// = 0xe8 index 0xe8=232 for character '�'
	233,	// = 0xe9 index 0xe9=233 for character '�'
	234,	// = 0xea index 0xea=234 for character '�'
	235,	// = 0xeb index 0xeb=235 for character '�'
	236,	// = 0xec index 0xec=236 for character '�'
	237,	// = 0xed index 0xed=237 for character '�'
	238,	// = 0xee index 0xee=238 for character '�'
	239,	// = 0xef index 0xef=239 for character '�'
	240,	// = 0xf0 index 0xf0=240 for character '�'
	241,	// = 0xf1 index 0xf1=241 for character '�'
	242,	// = 0xf2 index 0xf2=242 for character '�'
	243,	// = 0xf3 index 0xf3=243 for character '�'
	244,	// = 0xf4 index 0xf4=244 for character '�'
	245,	// = 0xf5 index 0xf5=245 for character '�'
	246,	// = 0xf6 index 0xf6=246 for character '�'
	247,	// = 0xf7 index 0xf7=247 for character '�'
	248,	// = 0xf8 index 0xf8=248 for character '�'
	249,	// = 0xf9 index 0xf9=249 for character '�'
	250,	// = 0xfa index 0xfa=250 for character '�'
	251,	// = 0xfb index 0xfb=251 for character '�'
	252,	// = 0xfc index 0xfc=252 for character '�'
	253,	// = 0xfd index 0xfd=253 for character '�'
	254,	// = 0xfe index 0xfe=254 for character '�'
	255,	// = 0xff index 0xff=255 for character '�'
};

unsigned char ansiOrder[256] = {
	  0,	// = 0x00 index 0x00=000
	224,	// = 0xe0 index 0x01=001
	225,	// = 0xe1 index 0x02=002
	226,	// = 0xe2 index 0x03=003
	227,	// = 0xe3 index 0x04=004
	228,	// = 0xe4 index 0x05=005
	229,	// = 0xe5 index 0x06=006
	230,	// = 0xe6 index 0x07=007
	231,	// = 0xe7 index 0x08=008
	217,	// = 0xd9 index 0x09=009
	218,	// = 0xda index 0x0a=010
	219,	// = 0xdb index 0x0b=011
	220,	// = 0xdc index 0x0c=012
	221,	// = 0xdd index 0x0d=013
	232,	// = 0xe8 index 0x0e=014
	233,	// = 0xe9 index 0x0f=015
	234,	// = 0xea index 0x10=016
	235,	// = 0xeb index 0x11=017
	236,	// = 0xec index 0x12=018
	237,	// = 0xed index 0x13=019
	238,	// = 0xee index 0x14=020
	239,	// = 0xef index 0x15=021
	240,	// = 0xf0 index 0x16=022
	241,	// = 0xf1 index 0x17=023
	242,	// = 0xf2 index 0x18=024
	243,	// = 0xf3 index 0x19=025
	244,	// = 0xf4 index 0x1a=026
	245,	// = 0xf5 index 0x1b=027
	246,	// = 0xf6 index 0x1c=028
	247,	// = 0xf7 index 0x1d=029
	248,	// = 0xf8 index 0x1e=030
	249,	// = 0xf9 index 0x1f=031
	222,	// = 0xde index 0x20=032
	133,	// = 0x85 index 0x21=033 for character '!'
	134,	// = 0x86 index 0x22=034 for character '"'
	169,	// = 0xa9 index 0x23=035 for character '#'
	170,	// = 0xaa index 0x24=036 for character '$'
	171,	// = 0xab index 0x25=037 for character '%'
	172,	// = 0xac index 0x26=038 for character '&'
	135,	// = 0x87 index 0x27=039 for character '''
	136,	// = 0x88 index 0x28=040 for character '('
	137,	// = 0x89 index 0x29=041 for character ')'
	173,	// = 0xad index 0x2a=042 for character '*'
	174,	// = 0xae index 0x2b=043 for character '+'
	138,	// = 0x8a index 0x2c=044 for character ','
	175,	// = 0xaf index 0x2d=045 for character '-'
	139,	// = 0x8b index 0x2e=046 for character '.'
	140,	// = 0x8c index 0x2f=047 for character '/'
	  1,	// = 0x01 index 0x30=048 for character '0'
	  2,	// = 0x02 index 0x31=049 for character '1'
	  3,	// = 0x03 index 0x32=050 for character '2'
	  4,	// = 0x04 index 0x33=051 for character '3'
	  5,	// = 0x05 index 0x34=052 for character '4'
	  6,	// = 0x06 index 0x35=053 for character '5'
	  7,	// = 0x07 index 0x36=054 for character '6'
	  8,	// = 0x08 index 0x37=055 for character '7'
	  9,	// = 0x09 index 0x38=056 for character '8'
	 10,	// = 0x0a index 0x39=057 for character '9'
	141,	// = 0x8d index 0x3a=058 for character ':'
	142,	// = 0x8e index 0x3b=059 for character ';'
	143,	// = 0x8f index 0x3c=060 for character '<'
	176,	// = 0xb0 index 0x3d=061 for character '='
	144,	// = 0x90 index 0x3e=062 for character '>'
	145,	// = 0x91 index 0x3f=063 for character '?'
	177,	// = 0xb1 index 0x40=064 for character '@'
	 11,	// = 0x0b index 0x41=065 for character 'A'
	 19,	// = 0x13 index 0x42=066 for character 'B'
	 20,	// = 0x14 index 0x43=067 for character 'C'
	 22,	// = 0x16 index 0x44=068 for character 'D'
	 24,	// = 0x18 index 0x45=069 for character 'E'
	 29,	// = 0x1d index 0x46=070 for character 'F'
	 30,	// = 0x1e index 0x47=071 for character 'G'
	 31,	// = 0x1f index 0x48=072 for character 'H'
	 32,	// = 0x20 index 0x49=073 for character 'I'
	 37,	// = 0x25 index 0x4a=074 for character 'J'
	 38,	// = 0x26 index 0x4b=075 for character 'K'
	 39,	// = 0x27 index 0x4c=076 for character 'L'
	 40,	// = 0x28 index 0x4d=077 for character 'M'
	 41,	// = 0x29 index 0x4e=078 for character 'N'
	 43,	// = 0x2b index 0x4f=079 for character 'O'
	 51,	// = 0x33 index 0x50=080 for character 'P'
	 52,	// = 0x34 index 0x51=081 for character 'Q'
	 53,	// = 0x35 index 0x52=082 for character 'R'
	 54,	// = 0x36 index 0x53=083 for character 'S'
	 56,	// = 0x38 index 0x54=084 for character 'T'
	 58,	// = 0x3a index 0x55=085 for character 'U'
	 63,	// = 0x3f index 0x56=086 for character 'V'
	 64,	// = 0x40 index 0x57=087 for character 'W'
	 65,	// = 0x41 index 0x58=088 for character 'X'
	 66,	// = 0x42 index 0x59=089 for character 'Y'
	 69,	// = 0x45 index 0x5a=090 for character 'Z'
	146,	// = 0x92 index 0x5b=091 for character '['
	148,	// = 0x94 index 0x5c=092 for character '\'
	147,	// = 0x93 index 0x5d=093 for character ']'
	178,	// = 0xb2 index 0x5e=094 for character '^'
	179,	// = 0xb3 index 0x5f=095 for character '_'
	180,	// = 0xb4 index 0x60=096 for character '`'
	 71,	// = 0x47 index 0x61=097 for character 'a'
	 79,	// = 0x4f index 0x62=098 for character 'b'
	 80,	// = 0x50 index 0x63=099 for character 'c'
	 82,	// = 0x52 index 0x64=100 for character 'd'
	 83,	// = 0x53 index 0x65=101 for character 'e'
	 88,	// = 0x58 index 0x66=102 for character 'f'
	 90,	// = 0x5a index 0x67=103 for character 'g'
	 91,	// = 0x5b index 0x68=104 for character 'h'
	 92,	// = 0x5c index 0x69=105 for character 'i'
	 97,	// = 0x61 index 0x6a=106 for character 'j'
	 98,	// = 0x62 index 0x6b=107 for character 'k'
	 99,	// = 0x63 index 0x6c=108 for character 'l'
	100,	// = 0x64 index 0x6d=109 for character 'm'
	101,	// = 0x65 index 0x6e=110 for character 'n'
	103,	// = 0x67 index 0x6f=111 for character 'o'
	112,	// = 0x70 index 0x70=112 for character 'p'
	113,	// = 0x71 index 0x71=113 for character 'q'
	114,	// = 0x72 index 0x72=114 for character 'r'
	115,	// = 0x73 index 0x73=115 for character 's'
	118,	// = 0x76 index 0x74=116 for character 't'
	120,	// = 0x78 index 0x75=117 for character 'u'
	125,	// = 0x7d index 0x76=118 for character 'v'
	126,	// = 0x7e index 0x77=119 for character 'w'
	127,	// = 0x7f index 0x78=120 for character 'x'
	128,	// = 0x80 index 0x79=121 for character 'y'
	131,	// = 0x83 index 0x7a=122 for character 'z'
	149,	// = 0x95 index 0x7b=123 for character '{'
	181,	// = 0xb5 index 0x7c=124 for character '|'
	150,	// = 0x96 index 0x7d=125 for character '}'
	182,	// = 0xb6 index 0x7e=126 for character '~'
	250,	// = 0xfa index 0x7f=127
	183,	// = 0xb7 index 0x80=128 for character '�'
	251,	// = 0xfb index 0x81=129
	151,	// = 0x97 index 0x82=130 for character '�'
	 89,	// = 0x59 index 0x83=131 for character '�'
	152,	// = 0x98 index 0x84=132 for character '�'
	153,	// = 0x99 index 0x85=133 for character '�'
	184,	// = 0xb8 index 0x86=134 for character '�'
	185,	// = 0xb9 index 0x87=135 for character '�'
	186,	// = 0xba index 0x88=136 for character '�'
	187,	// = 0xbb index 0x89=137 for character '�'
	 55,	// = 0x37 index 0x8a=138 for character '�'
	154,	// = 0x9a index 0x8b=139 for character '�'
	 44,	// = 0x2c index 0x8c=140 for character '�'
	252,	// = 0xfc index 0x8d=141
	 70,	// = 0x46 index 0x8e=142 for character '�'
	253,	// = 0xfd index 0x8f=143
	254,	// = 0xfe index 0x90=144
	155,	// = 0x9b index 0x91=145 for character '�'
	156,	// = 0x9c index 0x92=146 for character '�'
	157,	// = 0x9d index 0x93=147 for character '�'
	158,	// = 0x9e index 0x94=148 for character '�'
	159,	// = 0x9f index 0x95=149 for character '�'
	160,	// = 0xa0 index 0x96=150 for character '�'
	161,	// = 0xa1 index 0x97=151 for character '�'
	188,	// = 0xbc index 0x98=152 for character '�'
	189,	// = 0xbd index 0x99=153 for character '�'
	116,	// = 0x74 index 0x9a=154 for character '�'
	162,	// = 0xa2 index 0x9b=155 for character '�'
	104,	// = 0x68 index 0x9c=156 for character '�'
	255,	// = 0xff index 0x9d=157
	132,	// = 0x84 index 0x9e=158 for character '�'
	 67,	// = 0x43 index 0x9f=159 for character '�'
	223,	// = 0xdf index 0xa0=160
	163,	// = 0xa3 index 0xa1=161 for character '�'
	190,	// = 0xbe index 0xa2=162 for character '�'
	191,	// = 0xbf index 0xa3=163 for character '�'
	192,	// = 0xc0 index 0xa4=164 for character '�'
	193,	// = 0xc1 index 0xa5=165 for character '�'
	194,	// = 0xc2 index 0xa6=166 for character '�'
	195,	// = 0xc3 index 0xa7=167 for character '�'
	196,	// = 0xc4 index 0xa8=168 for character '�'
	197,	// = 0xc5 index 0xa9=169 for character '�'
	198,	// = 0xc6 index 0xaa=170 for character '�'
	164,	// = 0xa4 index 0xab=171 for character '�'
	199,	// = 0xc7 index 0xac=172 for character '�'
	168,	// = 0xa8 index 0xad=173 for character '�'
	200,	// = 0xc8 index 0xae=174 for character '�'
	201,	// = 0xc9 index 0xaf=175 for character '�'
	202,	// = 0xca index 0xb0=176 for character '�'
	203,	// = 0xcb index 0xb1=177 for character '�'
	204,	// = 0xcc index 0xb2=178 for character '�'
	205,	// = 0xcd index 0xb3=179 for character '�'
	165,	// = 0xa5 index 0xb4=180 for character '�'
	206,	// = 0xce index 0xb5=181 for character '�'
	207,	// = 0xcf index 0xb6=182 for character '�'
	208,	// = 0xd0 index 0xb7=183 for character '�'
	209,	// = 0xd1 index 0xb8=184 for character '�'
	210,	// = 0xd2 index 0xb9=185 for character '�'
	211,	// = 0xd3 index 0xba=186 for character '�'
	166,	// = 0xa6 index 0xbb=187 for character '�'
	212,	// = 0xd4 index 0xbc=188 for character '�'
	213,	// = 0xd5 index 0xbd=189 for character '�'
	214,	// = 0xd6 index 0xbe=190 for character '�'
	167,	// = 0xa7 index 0xbf=191 for character '�'
	 12,	// = 0x0c index 0xc0=192 for character '�'
	 13,	// = 0x0d index 0xc1=193 for character '�'
	 14,	// = 0x0e index 0xc2=194 for character '�'
	 15,	// = 0x0f index 0xc3=195 for character '�'
	 16,	// = 0x10 index 0xc4=196 for character '�'
	 17,	// = 0x11 index 0xc5=197 for character '�'
	 18,	// = 0x12 index 0xc6=198 for character '�'
	 21,	// = 0x15 index 0xc7=199 for character '�'
	 25,	// = 0x19 index 0xc8=200 for character '�'
	 26,	// = 0x1a index 0xc9=201 for character '�'
	 27,	// = 0x1b index 0xca=202 for character '�'
	 28,	// = 0x1c index 0xcb=203 for character '�'
	 33,	// = 0x21 index 0xcc=204 for character '�'
	 34,	// = 0x22 index 0xcd=205 for character '�'
	 35,	// = 0x23 index 0xce=206 for character '�'
	 36,	// = 0x24 index 0xcf=207 for character '�'
	 23,	// = 0x17 index 0xd0=208 for character '�'
	 42,	// = 0x2a index 0xd1=209 for character '�'
	 45,	// = 0x2d index 0xd2=210 for character '�'
	 46,	// = 0x2e index 0xd3=211 for character '�'
	 47,	// = 0x2f index 0xd4=212 for character '�'
	 48,	// = 0x30 index 0xd5=213 for character '�'
	 49,	// = 0x31 index 0xd6=214 for character '�'
	215,	// = 0xd7 index 0xd7=215 for character '�'
	 50,	// = 0x32 index 0xd8=216 for character '�'
	 59,	// = 0x3b index 0xd9=217 for character '�'
	 60,	// = 0x3c index 0xda=218 for character '�'
	 61,	// = 0x3d index 0xdb=219 for character '�'
	 62,	// = 0x3e index 0xdc=220 for character '�'
	 68,	// = 0x44 index 0xdd=221 for character '�'
	 57,	// = 0x39 index 0xde=222 for character '�'
	117,	// = 0x75 index 0xdf=223 for character '�'
	 72,	// = 0x48 index 0xe0=224 for character '�'
	 73,	// = 0x49 index 0xe1=225 for character '�'
	 74,	// = 0x4a index 0xe2=226 for character '�'
	 75,	// = 0x4b index 0xe3=227 for character '�'
	 76,	// = 0x4c index 0xe4=228 for character '�'
	 77,	// = 0x4d index 0xe5=229 for character '�'
	 78,	// = 0x4e index 0xe6=230 for character '�'
	 81,	// = 0x51 index 0xe7=231 for character '�'
	 84,	// = 0x54 index 0xe8=232 for character '�'
	 85,	// = 0x55 index 0xe9=233 for character '�'
	 86,	// = 0x56 index 0xea=234 for character '�'
	 87,	// = 0x57 index 0xeb=235 for character '�'
	 93,	// = 0x5d index 0xec=236 for character '�'
	 94,	// = 0x5e index 0xed=237 for character '�'
	 95,	// = 0x5f index 0xee=238 for character '�'
	 96,	// = 0x60 index 0xef=239 for character '�'
	105,	// = 0x69 index 0xf0=240 for character '�'
	102,	// = 0x66 index 0xf1=241 for character '�'
	106,	// = 0x6a index 0xf2=242 for character '�'
	107,	// = 0x6b index 0xf3=243 for character '�'
	108,	// = 0x6c index 0xf4=244 for character '�'
	109,	// = 0x6d index 0xf5=245 for character '�'
	110,	// = 0x6e index 0xf6=246 for character '�'
	216,	// = 0xd8 index 0xf7=247 for character '�'
	111,	// = 0x6f index 0xf8=248 for character '�'
	121,	// = 0x79 index 0xf9=249 for character '�'
	122,	// = 0x7a index 0xfa=250 for character '�'
	123,	// = 0x7b index 0xfb=251 for character '�'
	124,	// = 0x7c index 0xfc=252 for character '�'
	129,	// = 0x81 index 0xfd=253 for character '�'
	119,	// = 0x77 index 0xfe=254 for character '�'
	130,	// = 0x82 index 0xff=255 for character '�'
};

unsigned char ansiIgnoreCaseOrder[256] = {
	  0,	// = 0x00 index 0x00=000
	224,	// = 0xe0 index 0x01=001
	225,	// = 0xe1 index 0x02=002
	226,	// = 0xe2 index 0x03=003
	227,	// = 0xe3 index 0x04=004
	228,	// = 0xe4 index 0x05=005
	229,	// = 0xe5 index 0x06=006
	230,	// = 0xe6 index 0x07=007
	231,	// = 0xe7 index 0x08=008
	217,	// = 0xd9 index 0x09=009
	218,	// = 0xda index 0x0a=010
	219,	// = 0xdb index 0x0b=011
	220,	// = 0xdc index 0x0c=012
	221,	// = 0xdd index 0x0d=013
	232,	// = 0xe8 index 0x0e=014
	233,	// = 0xe9 index 0x0f=015
	234,	// = 0xea index 0x10=016
	235,	// = 0xeb index 0x11=017
	236,	// = 0xec index 0x12=018
	237,	// = 0xed index 0x13=019
	238,	// = 0xee index 0x14=020
	239,	// = 0xef index 0x15=021
	240,	// = 0xf0 index 0x16=022
	241,	// = 0xf1 index 0x17=023
	242,	// = 0xf2 index 0x18=024
	243,	// = 0xf3 index 0x19=025
	244,	// = 0xf4 index 0x1a=026
	245,	// = 0xf5 index 0x1b=027
	246,	// = 0xf6 index 0x1c=028
	247,	// = 0xf7 index 0x1d=029
	248,	// = 0xf8 index 0x1e=030
	249,	// = 0xf9 index 0x1f=031
	222,	// = 0xde index 0x20=032
	133,	// = 0x85 index 0x21=033 for character '!'
	134,	// = 0x86 index 0x22=034 for character '"'
	169,	// = 0xa9 index 0x23=035 for character '#'
	170,	// = 0xaa index 0x24=036 for character '$'
	171,	// = 0xab index 0x25=037 for character '%'
	172,	// = 0xac index 0x26=038 for character '&'
	135,	// = 0x87 index 0x27=039 for character '''
	136,	// = 0x88 index 0x28=040 for character '('
	137,	// = 0x89 index 0x29=041 for character ')'
	173,	// = 0xad index 0x2a=042 for character '*'
	174,	// = 0xae index 0x2b=043 for character '+'
	138,	// = 0x8a index 0x2c=044 for character ','
	175,	// = 0xaf index 0x2d=045 for character '-'
	139,	// = 0x8b index 0x2e=046 for character '.'
	140,	// = 0x8c index 0x2f=047 for character '/'
	  1,	// = 0x01 index 0x30=048 for character '0'
	  2,	// = 0x02 index 0x31=049 for character '1'
	  3,	// = 0x03 index 0x32=050 for character '2'
	  4,	// = 0x04 index 0x33=051 for character '3'
	  5,	// = 0x05 index 0x34=052 for character '4'
	  6,	// = 0x06 index 0x35=053 for character '5'
	  7,	// = 0x07 index 0x36=054 for character '6'
	  8,	// = 0x08 index 0x37=055 for character '7'
	  9,	// = 0x09 index 0x38=056 for character '8'
	 10,	// = 0x0a index 0x39=057 for character '9'
	141,	// = 0x8d index 0x3a=058 for character ':'
	142,	// = 0x8e index 0x3b=059 for character ';'
	143,	// = 0x8f index 0x3c=060 for character '<'
	176,	// = 0xb0 index 0x3d=061 for character '='
	144,	// = 0x90 index 0x3e=062 for character '>'
	145,	// = 0x91 index 0x3f=063 for character '?'
	177,	// = 0xb1 index 0x40=064 for character '@'
	 11,	// = 0x0b index 0x41=065 for character 'A'
	 19,	// = 0x13 index 0x42=066 for character 'B'
	 20,	// = 0x14 index 0x43=067 for character 'C'
	 22,	// = 0x16 index 0x44=068 for character 'D'
	 24,	// = 0x18 index 0x45=069 for character 'E'
	 29,	// = 0x1d index 0x46=070 for character 'F'
	 30,	// = 0x1e index 0x47=071 for character 'G'
	 31,	// = 0x1f index 0x48=072 for character 'H'
	 32,	// = 0x20 index 0x49=073 for character 'I'
	 37,	// = 0x25 index 0x4a=074 for character 'J'
	 38,	// = 0x26 index 0x4b=075 for character 'K'
	 39,	// = 0x27 index 0x4c=076 for character 'L'
	 40,	// = 0x28 index 0x4d=077 for character 'M'
	 41,	// = 0x29 index 0x4e=078 for character 'N'
	 43,	// = 0x2b index 0x4f=079 for character 'O'
	 51,	// = 0x33 index 0x50=080 for character 'P'
	 52,	// = 0x34 index 0x51=081 for character 'Q'
	 53,	// = 0x35 index 0x52=082 for character 'R'
	 54,	// = 0x36 index 0x53=083 for character 'S'
	 56,	// = 0x38 index 0x54=084 for character 'T'
	 58,	// = 0x3a index 0x55=085 for character 'U'
	 63,	// = 0x3f index 0x56=086 for character 'V'
	 64,	// = 0x40 index 0x57=087 for character 'W'
	 65,	// = 0x41 index 0x58=088 for character 'X'
	 66,	// = 0x42 index 0x59=089 for character 'Y'
	 69,	// = 0x45 index 0x5a=090 for character 'Z'
	146,	// = 0x92 index 0x5b=091 for character '['
	148,	// = 0x94 index 0x5c=092 for character '\'
	147,	// = 0x93 index 0x5d=093 for character ']'
	178,	// = 0xb2 index 0x5e=094 for character '^'
	179,	// = 0xb3 index 0x5f=095 for character '_'
	180,	// = 0xb4 index 0x60=096 for character '`'
	 11,	// = 0x0b index 0x61=097 for character 'a'
	 19,	// = 0x13 index 0x62=098 for character 'b'
	 20,	// = 0x14 index 0x63=099 for character 'c'
	 22,	// = 0x16 index 0x64=100 for character 'd'
	 24,	// = 0x18 index 0x65=101 for character 'e'
	 29,	// = 0x1d index 0x66=102 for character 'f'
	 30,	// = 0x1e index 0x67=103 for character 'g'
	 31,	// = 0x1f index 0x68=104 for character 'h'
	 32,	// = 0x20 index 0x69=105 for character 'i'
	 37,	// = 0x25 index 0x6a=106 for character 'j'
	 38,	// = 0x26 index 0x6b=107 for character 'k'
	 39,	// = 0x27 index 0x6c=108 for character 'l'
	 40,	// = 0x28 index 0x6d=109 for character 'm'
	 41,	// = 0x29 index 0x6e=110 for character 'n'
	 43,	// = 0x2b index 0x6f=111 for character 'o'
	 51,	// = 0x33 index 0x70=112 for character 'p'
	 52,	// = 0x34 index 0x71=113 for character 'q'
	 53,	// = 0x35 index 0x72=114 for character 'r'
	 54,	// = 0x36 index 0x73=115 for character 's'
	 56,	// = 0x38 index 0x74=116 for character 't'
	 58,	// = 0x3a index 0x75=117 for character 'u'
	 63,	// = 0x3f index 0x76=118 for character 'v'
	 64,	// = 0x40 index 0x77=119 for character 'w'
	 65,	// = 0x41 index 0x78=120 for character 'x'
	 66,	// = 0x42 index 0x79=121 for character 'y'
	 69,	// = 0x45 index 0x7a=122 for character 'z'
	149,	// = 0x95 index 0x7b=123 for character '{'
	181,	// = 0xb5 index 0x7c=124 for character '|'
	150,	// = 0x96 index 0x7d=125 for character '}'
	182,	// = 0xb6 index 0x7e=126 for character '~'
	250,	// = 0xfa index 0x7f=127
	183,	// = 0xb7 index 0x80=128 for character '�'
	251,	// = 0xfb index 0x81=129
	151,	// = 0x97 index 0x82=130 for character '�'
	 89,	// = 0x59 index 0x83=131 for character '�'
	152,	// = 0x98 index 0x84=132 for character '�'
	153,	// = 0x99 index 0x85=133 for character '�'
	184,	// = 0xb8 index 0x86=134 for character '�'
	185,	// = 0xb9 index 0x87=135 for character '�'
	186,	// = 0xba index 0x88=136 for character '�'
	187,	// = 0xbb index 0x89=137 for character '�'
	 55,	// = 0x37 index 0x8a=138 for character '�'
	154,	// = 0x9a index 0x8b=139 for character '�'
	 44,	// = 0x2c index 0x8c=140 for character '�'
	252,	// = 0xfc index 0x8d=141
	 70,	// = 0x46 index 0x8e=142 for character '�'
	253,	// = 0xfd index 0x8f=143
	254,	// = 0xfe index 0x90=144
	155,	// = 0x9b index 0x91=145 for character '�'
	156,	// = 0x9c index 0x92=146 for character '�'
	157,	// = 0x9d index 0x93=147 for character '�'
	158,	// = 0x9e index 0x94=148 for character '�'
	159,	// = 0x9f index 0x95=149 for character '�'
	160,	// = 0xa0 index 0x96=150 for character '�'
	161,	// = 0xa1 index 0x97=151 for character '�'
	188,	// = 0xbc index 0x98=152 for character '�'
	189,	// = 0xbd index 0x99=153 for character '�'
	 55,	// = 0x37 index 0x9a=154 for character '�'
	162,	// = 0xa2 index 0x9b=155 for character '�'
	 44,	// = 0x2c index 0x9c=156 for character '�'
	255,	// = 0xff index 0x9d=157
	 70,	// = 0x46 index 0x9e=158 for character '�'
	 67,	// = 0x43 index 0x9f=159 for character '�'
	223,	// = 0xdf index 0xa0=160
	163,	// = 0xa3 index 0xa1=161 for character '�'
	190,	// = 0xbe index 0xa2=162 for character '�'
	191,	// = 0xbf index 0xa3=163 for character '�'
	192,	// = 0xc0 index 0xa4=164 for character '�'
	193,	// = 0xc1 index 0xa5=165 for character '�'
	194,	// = 0xc2 index 0xa6=166 for character '�'
	195,	// = 0xc3 index 0xa7=167 for character '�'
	196,	// = 0xc4 index 0xa8=168 for character '�'
	197,	// = 0xc5 index 0xa9=169 for character '�'
	198,	// = 0xc6 index 0xaa=170 for character '�'
	164,	// = 0xa4 index 0xab=171 for character '�'
	199,	// = 0xc7 index 0xac=172 for character '�'
	168,	// = 0xa8 index 0xad=173 for character '�'
	200,	// = 0xc8 index 0xae=174 for character '�'
	201,	// = 0xc9 index 0xaf=175 for character '�'
	202,	// = 0xca index 0xb0=176 for character '�'
	203,	// = 0xcb index 0xb1=177 for character '�'
	204,	// = 0xcc index 0xb2=178 for character '�'
	205,	// = 0xcd index 0xb3=179 for character '�'
	165,	// = 0xa5 index 0xb4=180 for character '�'
	206,	// = 0xce index 0xb5=181 for character '�'
	207,	// = 0xcf index 0xb6=182 for character '�'
	208,	// = 0xd0 index 0xb7=183 for character '�'
	209,	// = 0xd1 index 0xb8=184 for character '�'
	210,	// = 0xd2 index 0xb9=185 for character '�'
	211,	// = 0xd3 index 0xba=186 for character '�'
	166,	// = 0xa6 index 0xbb=187 for character '�'
	212,	// = 0xd4 index 0xbc=188 for character '�'
	213,	// = 0xd5 index 0xbd=189 for character '�'
	214,	// = 0xd6 index 0xbe=190 for character '�'
	167,	// = 0xa7 index 0xbf=191 for character '�'
	 12,	// = 0x0c index 0xc0=192 for character '�'
	 13,	// = 0x0d index 0xc1=193 for character '�'
	 14,	// = 0x0e index 0xc2=194 for character '�'
	 15,	// = 0x0f index 0xc3=195 for character '�'
	 16,	// = 0x10 index 0xc4=196 for character '�'
	 17,	// = 0x11 index 0xc5=197 for character '�'
	 18,	// = 0x12 index 0xc6=198 for character '�'
	 21,	// = 0x15 index 0xc7=199 for character '�'
	 25,	// = 0x19 index 0xc8=200 for character '�'
	 26,	// = 0x1a index 0xc9=201 for character '�'
	 27,	// = 0x1b index 0xca=202 for character '�'
	 28,	// = 0x1c index 0xcb=203 for character '�'
	 33,	// = 0x21 index 0xcc=204 for character '�'
	 34,	// = 0x22 index 0xcd=205 for character '�'
	 35,	// = 0x23 index 0xce=206 for character '�'
	 36,	// = 0x24 index 0xcf=207 for character '�'
	 23,	// = 0x17 index 0xd0=208 for character '�'
	 42,	// = 0x2a index 0xd1=209 for character '�'
	 45,	// = 0x2d index 0xd2=210 for character '�'
	 46,	// = 0x2e index 0xd3=211 for character '�'
	 47,	// = 0x2f index 0xd4=212 for character '�'
	 48,	// = 0x30 index 0xd5=213 for character '�'
	 49,	// = 0x31 index 0xd6=214 for character '�'
	215,	// = 0xd7 index 0xd7=215 for character '�'
	 50,	// = 0x32 index 0xd8=216 for character '�'
	 59,	// = 0x3b index 0xd9=217 for character '�'
	 60,	// = 0x3c index 0xda=218 for character '�'
	 61,	// = 0x3d index 0xdb=219 for character '�'
	 62,	// = 0x3e index 0xdc=220 for character '�'
	 68,	// = 0x44 index 0xdd=221 for character '�'
	 57,	// = 0x39 index 0xde=222 for character '�'
	117,	// = 0x75 index 0xdf=223 for character '�'
	 12,	// = 0x0c index 0xe0=224 for character '�'
	 13,	// = 0x0d index 0xe1=225 for character '�'
	 14,	// = 0x0e index 0xe2=226 for character '�'
	 15,	// = 0x0f index 0xe3=227 for character '�'
	 16,	// = 0x10 index 0xe4=228 for character '�'
	 17,	// = 0x11 index 0xe5=229 for character '�'
	 18,	// = 0x12 index 0xe6=230 for character '�'
	 21,	// = 0x15 index 0xe7=231 for character '�'
	 25,	// = 0x19 index 0xe8=232 for character '�'
	 26,	// = 0x1a index 0xe9=233 for character '�'
	 27,	// = 0x1b index 0xea=234 for character '�'
	 28,	// = 0x1c index 0xeb=235 for character '�'
	 33,	// = 0x21 index 0xec=236 for character '�'
	 34,	// = 0x22 index 0xed=237 for character '�'
	 35,	// = 0x23 index 0xee=238 for character '�'
	 36,	// = 0x24 index 0xef=239 for character '�'
	 23,	// = 0x17 index 0xf0=240 for character '�'
	 42,	// = 0x2a index 0xf1=241 for character '�'
	 45,	// = 0x2d index 0xf2=242 for character '�'
	 46,	// = 0x2e index 0xf3=243 for character '�'
	 47,	// = 0x2f index 0xf4=244 for character '�'
	 48,	// = 0x30 index 0xf5=245 for character '�'
	 49,	// = 0x31 index 0xf6=246 for character '�'
	216,	// = 0xd8 index 0xf7=247 for character '�'
	 50,	// = 0x32 index 0xf8=248 for character '�'
	 59,	// = 0x3b index 0xf9=249 for character '�'
	 60,	// = 0x3c index 0xfa=250 for character '�'
	 61,	// = 0x3d index 0xfb=251 for character '�'
	 62,	// = 0x3e index 0xfc=252 for character '�'
	 68,	// = 0x44 index 0xfd=253 for character '�'
	 57,	// = 0x39 index 0xfe=254 for character '�'
	 67,	// = 0x43 index 0xff=255 for character '�'
};
#endif

}	// extern "C"

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

static unsigned char calcUpper( unsigned char c )
{
	if( c >= 'a' && c <= 'z' )
	{
		return static_cast<unsigned char>(c-32);
	}
	if( c >= (unsigned char)'�' && c <= (unsigned char)'�' && c != (unsigned char)'�' && c != (unsigned char)'�' )
	{
		return static_cast<unsigned char>(c-32);
	}
	if( c == (unsigned char)'�' )
	{
		return (unsigned char)'�';
	}
	if( c == (unsigned char)'�' || c == (unsigned char)'�' || c == (unsigned char)'�' )
	{
		return static_cast<unsigned char>(c-16);
	}
	
	return c;
}

static unsigned char calcLower( unsigned char c )
{
	if( c >= 'A' && c <= 'Z' )
	{
		return static_cast<unsigned char>(c+32);
	}
	if( c >= (unsigned char)'�' && c <= (unsigned char)'�' && c != (unsigned char)'�' && c != (unsigned char)'�' )
	{
		return static_cast<unsigned char>(c+32);
	}
	if( c == (unsigned char)'�' )
	{
		return (unsigned char)'�';
	}
	if( c == (unsigned char)'�' || c == (unsigned char)'�' || c == (unsigned char)'�' )
	{
		return static_cast<unsigned char>(c+16);
	}
	
	return c;
}
#if CALCULATE
static void writeTable( std::ostream &stream, unsigned char const table[256], const char *name )
{
	stream << "unsigned char "<< name <<"[256] = {\n";
	for( short c=0; c<=255; ++c )
	{
		stream	<< '\t' << std::dec << std::setw(3) << std::setfill(' ') << short(table[c]) 
				<< ",\t// = 0x" << std::setw(2) << std::setfill('0') << std::hex << short(table[c])
				<< " index 0x" << std::setw(2) << c << '=' << std::dec << std::setw(3) << c;

		if( !(ansiFlags[c] & (CONTROL_CHAR|SPACE_CHAR)) )
		{
			stream << " for character '" << char(c) << '\'';
		}
		stream << '\n';
	}
	stream << "};\n\n";
}
#endif // CALCULATE

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

int ansiCompare( const char *s1, const char *s2 )
{
#if CALCULATE
	return strcmp( s1, s2 );
#else
	unsigned char	c1, c2;
	short			o1, o2;
	int				result;

	do
	{
		c1 = *s1++;
		c2 = *s2++;
		o1 = ansiOrder[c1];
		o2 = ansiOrder[c2];
		result = o1-o2;
	} while( (*s1 || *s2) && !result );

	return result;
#endif
}

int ansiCompareN( const char *s1, const char *s2, size_t len )
{
#if CALCULATE
	return strncmp( s1, s2, len );
#else
	unsigned char	c1, c2;
	short			o1, o2;
	int				result = 0;

	while( len>0 && (*s1 || *s2) && !result)
	{
		c1 = *s1++;
		c2 = *s2++;
		o1 = ansiOrder[c1];
		o2 = ansiOrder[c2];
		result = o1-o2;
		len--;
	}

	return result;
#endif
}

int ansiCompareI( const char *s1, const char *s2 )
{
	unsigned char	c1, c2;
	short			o1, o2;
	int				result;

	do
	{
		c1 = *s1++;
		c2 = *s2++;
		o1 = ansiIgnoreCaseOrder[c1];
		o2 = ansiIgnoreCaseOrder[c2];
		result = o1-o2;
	} while( (*s1 || *s2) && !result );

	return result;
}

int ansiCompareNI( const char *s1, const char *s2, size_t len )
{
	unsigned char	c1, c2;
	short			o1, o2;
	int				result = 0;

	while( len>0 && (*s1 || *s2) && !result)
	{
		c1 = *s1++;
		c2 = *s2++;
		o1 = ansiIgnoreCaseOrder[c1];
		o2 = ansiIgnoreCaseOrder[c2];
		result = o1-o2;
		len--;
	}

	return result;
}



int checkData()
{
#if TEST_SIZE
	/*
		edited tables
	*/
#if CALCULATE
	if( sizeof( ansiChars ) != 256 )
	{
		std::cout << "Bad size of ansiChars	" << sizeof( ansiChars ) << std::endl;
		return 0;
	}
	if( sizeof( ansiOrdered ) != 256 )
	{
		std::cout << "Bad size of ansiOrdered	" << sizeof( ansiOrdered ) << std::endl;
		return 0;
	}
#endif // CALCULATE
	if( sizeof( ansiFlags) != 256 )
	{
		std::cout << "Bad size of ansiFlags	" << sizeof( ansiFlags ) << std::endl;
		return 0;
	}

	/*
		calculated tables
	*/
	if( sizeof( ansiLowerChars ) != 256 )
	{
		std::cout << "Bad size of ansiLowerChars	" << sizeof( ansiLowerChars ) << std::endl;
		return 0;
	}
	if( sizeof( ansiUpperChars ) != 256 )
	{
		std::cout << "Bad size of ansiUpperChars	" << sizeof( ansiUpperChars ) << std::endl;
		return 0;
	}
	if( sizeof( ansiOrder ) != 256 )
	{
		std::cout << "Bad size of ansiOrder	" << sizeof( ansiOrder ) << std::endl;
		return 0;
	}
	if( sizeof( ansiIgnoreCaseOrder ) != 256 )
	{
		std::cout << "Bad size of ansiIgnoreCaseOrder	" << sizeof( ansiIgnoreCaseOrder ) << std::endl;
		return 0;
	}
#endif	// TEST_SIZE
	/*
		edited tables
	*/
#if CALCULATE
	for( short c=0; c<=255; ++c )
	{
		if( ansiChars[c] != c )
		{
			std::cout << "Bad ansiChar " << short(ansiChars[c]) << ' ' << c << std::endl;
		}
	}

	short lastFlag = 0;
	short lastChar = 0;
	for( short c=1; c<=255; ++c )	// do not test 0 byte
	{
		short	currentChar = ansiOrdered[c];
		short	currentFlag = ansiFlags[currentChar];
		if( currentFlag < lastFlag )
		{
			std::cout << "Out of order group" << short(c) << ":" << currentChar << std::endl;

			return 0;
		}
		if( currentFlag > lastFlag )
		{
			lastFlag = currentFlag;
			lastChar = currentChar;
		}
		else if( !(currentFlag&(UPPER_CHAR|LOWER_CHAR)) )
		{
			if( lastChar >= currentChar )
			{
				std::cout << "Out of order char" << short(c) << ":" << currentChar << std::endl;

				return 0;
			}
		}
	}
#endif // CALCULATE

	/*
		calculate tables
	*/
#if CALCULATE
	for( short c=0; c<=255; ++c )
	{
		ansiUpperChars[c] = calcUpper( c );
		ansiLowerChars[c] = calcLower( c );
		ansiOrder[ansiOrdered[c]] = c;
	}
	for( short c=0; c<=255; ++c )
	{
		ansiIgnoreCaseOrder[c] = ansiOrder[ansiUpperChars[c]];
	}

	std::ofstream stream( "ansiChar.txt" );
	writeTable( stream, ansiUpperChars, "ansiUpperChars" );
	writeTable( stream, ansiLowerChars, "ansiLowerChars" );
	writeTable( stream, ansiOrder, "ansiOrder" );
	writeTable( stream, ansiIgnoreCaseOrder, "ansiIgnoreCaseOrder" );
#endif

	/*
		check the tables
	*/
	for( short c=0; c<=255; ++c )
	{
		if( ansiLowerChars[c] != calcLower( (unsigned char)c ) )
		{
			std::cout	<< "Bad lower: " << c << ' '
						<< short(ansiLowerChars[c]) << " != " << short(calcLower( (unsigned char)c )) << std::endl;
			return 0;
		}
	}
	Set<unsigned char>	order;
	order.addElements( ansiOrder, 256 );
	if( order.size() != 256 )
	{
		std::cout << "Bad size of order	" << order.size() << std::endl;
		for( short o=0; o<256; ++o )
		{
			if( !order.hasElement( static_cast<unsigned char>(o) ) )
			{
				std::cout << "Missing order " << o << std::endl;
			}
		}
		order.clear();
		for( short c=0; c<256; ++c )
		{
			if( order.hasElement( ansiOrder[c] ) )
			{
				std::cout << "Duplicate order " << c << ' ' << short(ansiOrder[c]) << std::endl;
			}
			else
			{
				order.addElement( ansiOrder[c] );
			}
		}
		return 0;
	}

	for( short c=0; c<256; ++c )
	{
		if( ansiIgnoreCaseOrder[c] != ansiIgnoreCaseOrder[ansiToLower((unsigned char)c)] )
		{
			std::cout	<< "1: " << c << ' '<< short(ansiToLower((unsigned char)c)) << ' ' 
						<< ansiIgnoreCaseOrder[c] << " != " << ansiIgnoreCaseOrder[ansiToLower((unsigned char)c)] << std::endl;
			return 0;
		}
		if( ansiIgnoreCaseOrder[c] != ansiIgnoreCaseOrder[ansiToUpper((unsigned char)c)] )
		{
			std::cout	<< "2: " << c << ' '<< short(ansiToUpper((unsigned char)c))
						<< ansiIgnoreCaseOrder[c] << " != " << ansiIgnoreCaseOrder[ansiToUpper((unsigned char)c)] << std::endl;
			return 0;
		}
		if( c == ansiLowerChars[c] && c == ansiUpperChars[c] )
		{
			if( ansiIgnoreCaseOrder[c] != ansiOrder[c] )
			{
				std::cout << "3: " << c << ' '<< short(ansiIgnoreCaseOrder[c]) << " != " << short(ansiOrder[c]) << std::endl;
				return 0;
			}
		}
	}

	return 1;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

