/*
		Project:		GAKLIB
		Module:			aiBrainTest.h
		Description:	
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

#include <gak/aiBrain.h>

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

static const char BRAINFILE[] = "aiBrain.bin";
static const uint32 BRAINMAGIC = 0x12345678;
static const uint16 BRAINVERSION = 1;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

using ai::Brain;
using ai::StringTokens;
using ai::tokenString;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class AiBrainTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "AiBrainTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "AiBrainTest::PerformTest");
		TestScope scope( "PerformTest" );

		Brain		theBrain;
		STRING		word1 = "Martin",
					word2 = "Gäckler",
					word3 = "Linz",
					word4 = "München",
					word5 = "Austria",
					word6 = "Baiern";

		// check empty brain
		size_t count = theBrain.getPairCount(word1, word2);
		UT_ASSERT_EQUAL(count, 0);
		UT_ASSERT_EQUAL(theBrain.size(), 0UL);

		// check brain with first item
		theBrain.addPair(word1, word2);
		count = theBrain.getPairCount(word1, word2);
		UT_ASSERT_EQUAL(count, 1);
		UT_ASSERT_EQUAL(theBrain.size(), 1UL);

		// check brain with second item
		theBrain.addPair(word1, word2);
		count = theBrain.getPairCount(word1, word2);
		UT_ASSERT_EQUAL(count, 2);
		UT_ASSERT_EQUAL(theBrain.size(), 1UL);

		// check words can be swaped
		count = theBrain.getPairCount(word2, word1);
		UT_ASSERT_EQUAL(count, 2);

		// check if one word is not yet known
		count = theBrain.getPairCount(word2, word3);
		UT_ASSERT_EQUAL(count, 0);
		UT_ASSERT_EQUAL(theBrain.size(), 1UL);

		// check equal word
		theBrain.addPair(word2, word2);
		count = theBrain.getPairCount(word2, word2);
		UT_ASSERT_EQUAL(count, 0);
		UT_ASSERT_EQUAL(theBrain.size(), 1UL);

		writeToBinaryFile(BRAINFILE, theBrain, BRAINMAGIC, BRAINVERSION, owmOverwrite);

		Brain		cloneBrain;
		UT_ASSERT_EQUAL(cloneBrain.size(), 0UL);
		readFromBinaryFile(BRAINFILE, &cloneBrain, BRAINMAGIC, BRAINVERSION, false);
		UT_ASSERT_EQUAL(cloneBrain.size(), 1UL);

		// check saved and loaded brain
		count = cloneBrain.getPairCount(word2, word1);
		UT_ASSERT_EQUAL(count, 2UL);

		const STRING	testText1 = "the quick brown fox jumps over the lazy dog the quick brown fox jumps over the lazy dog the quick brown fox jumps over the lazy dog handball fussball fcbaiern Martin";

		StringTokens	tokens;
		tokenString( testText1, Array<STRING>(), ai::IS_ANY, &tokens );
		UT_ASSERT_EQUAL(tokens.size(), 31UL);
		UT_ASSERT_EQUAL(cloneBrain.size(), 1UL);
		cloneBrain.learnFromTokens(testText1, tokens, 5);
		UT_ASSERT_EQUAL(cloneBrain.size(), 49UL);
		count = cloneBrain.getPairCount("the", "fcbaiern");
		UT_ASSERT_GREATEREQ(count, size_t(1UL));

		const STRING	testText2 = "Martin Gäckler lebt in Linz";
		StringIndex positions;
		indexString( testText2, Array<STRING>(), ai::IS_ANY, &positions );
		cloneBrain.learnFromIndex(positions, 10);
		UT_ASSERT_EQUAL(cloneBrain.size(), 58UL);
		count = cloneBrain.getPairCount("the", "fcbaiern");
		UT_ASSERT_GREATEREQ(count, size_t(1UL));

		Set<STRING> partners = cloneBrain.getPartners("Martin");
		UT_ASSERT_EQUAL(partners.size(), 9UL);
		partners = cloneBrain.getPartners("Gäckler");
		UT_ASSERT_EQUAL(partners.size(), 4UL);

		strRemoveE(BRAINFILE);
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static AiBrainTest	myAiBrainTest;

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

