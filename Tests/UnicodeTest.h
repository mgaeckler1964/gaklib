/*
		Project:		GAKLIB
		Module:			UnicodeTest.h
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

#include <iostream>
#include <gak/unitTest.h>

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

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class UnicodeTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "UnicodeTest";
	}
	virtual void PerformTest( void )
	{
		STRING	gak="Martin Gäckler";

		UT_ASSERT_EQUAL( gak.strlen(), size_t(14) );
		gak = gak.encodeUTF8();
		UT_ASSERT_EQUAL( gak.strlen(), size_t(15) );
		UT_ASSERT_EQUAL( gak[8U], char(-61) );
		UT_ASSERT_EQUAL( gak[9U], char(-92) );

		gak += 'Ä';
		UT_ASSERT_EQUAL( gak.strlen(), size_t(17) );

		gak += "Gäckler";
		UT_ASSERT_EQUAL( gak.strlen(), size_t(25) );
		gak = gak.decodeUTF8();
		UT_ASSERT_EQUAL( gak.strlen(), size_t(22) );
		UT_ASSERT_EQUAL( gak, STRING("Martin GäcklerÄGäckler") );
		std::cout << gak << '\n';

		STRING	ansi, utf8, ansiResult;
		for( unsigned i= 128; i<256; i++ )
		{
			ansi = formatNumber( i );
			ansi += (char)i;

			utf8 = ansi.encodeUTF8();
			ansiResult = utf8.decodeUTF8();

			UT_ASSERT_EQUAL( ansi, ansiResult );
		}

		STRING	ansiText = "Martin Gäckler";
		STRING	utf8Source = ansiText.encodeUTF8();
		UT_ASSERT_EQUAL( ansiText, utf8Source );

		{
			uSTRING tmp( ansiText );
			STRING	result = tmp.encodeUTF8();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
			result = tmp.toString();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
		}

		{
			uSTRING tmp( utf8Source );
			STRING	result = tmp.encodeUTF8();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
			result = tmp.toString();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
		}

		{
			uSTRING tmp;
			tmp.decodeUTF8( utf8Source );
			STRING	result = tmp.encodeUTF8();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
			result = tmp.toString();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
		}
		{
			uSTRING tmp = uSTRING().decodeUTF8( utf8Source );
			STRING	result = tmp.encodeUTF8();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
			result = tmp.toString();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
		}
		{
			STRING	result = uSTRING().decodeUTF8( utf8Source ).encodeUTF8();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
			result = uSTRING().decodeUTF8( utf8Source ).toString();
			UT_ASSERT_EQUAL( ansiText, result );
			UT_ASSERT_EQUAL( utf8Source, result );
		}
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static UnicodeTest myUnicodeTest;

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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

