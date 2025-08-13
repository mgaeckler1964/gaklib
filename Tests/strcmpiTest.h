/*
		Project:		GAKLIB
		Module:			strcmpiTest.h
		Description:	Test strcmpi and strncmpi
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/gaklib.h>

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

class strcmpiTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "strcmpiTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "strcmpiTest::PerformTest");
		TestScope scope( "PerformTest" );
		UT_ASSERT_EQUAL( 0,
			strcmpi(
				"abcdefghijklmnopqrstuvwxyzäöüß",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"
			)
		);
		UT_ASSERT_GREATER( 0,
			strcmpi(
				"abcdefghijklmnopqrstuvwxyzäöü",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß"
			)
		);
		UT_ASSERT_LESS( 0,
			strcmpi(
				"abcdefghijklmnopqrstuvwxyzäöüß",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ"
			)
		);
		UT_ASSERT_GREATER( 0,
			strcmpi(
				"",
				"ä"
			)
		);
		UT_ASSERT_GREATER( 0,
			strcmpi(
				"",
				"a"
			)
		);
		UT_ASSERT_LESS( 0,
			strcmpi(
				"ä",
				""
			)
		);
		UT_ASSERT_LESS( 0,
			strcmpi(
				"a",
				""
			)
		);

		UT_ASSERT_EQUAL( 0,
			strncmpi(
				"abcdefghijklmnopqrstuvwxyzäöüß",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß",
				16
			)
		);
		UT_ASSERT_EQUAL( 0,
			strncmpi(
				"abcdefghijklmnopqrstuvwxyzäöüß",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß",
				128
			)
		);
		UT_ASSERT_EQUAL( 0,
			strncmpi(
				"abcdefghijklmnopqrstuvwxyzäöü",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß",
				29
			)
		);
		UT_ASSERT_GREATER( 0,
			strncmpi(
				"abcdefghijklmnopqrstuvwxyzäöü",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜß",
				30
			)
		);
		UT_ASSERT_EQUAL( 0,
			strncmpi(
				"abcdefghijklmnopqrstuvwxyzäöüß",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ",
				29
			)
		);
		UT_ASSERT_LESS( 0,
			strncmpi(
				"abcdefghijklmnopqrstuvwxyzäöüß",
				"ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ",
				30
			)
		);
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static strcmpiTest myStrcmpiTest;

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
