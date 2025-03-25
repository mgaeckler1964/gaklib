/*
		Project:		GAKLIB
		Module:			ChessTest.h
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

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
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

#include <gak/chess.h>

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

class ChessTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "ChessTest";
	}
	virtual void PerformTest( void )
	{
		//TestScope scope( "PerformTest" );
		chess::Board	chess;

		chess.reset();
		UT_ASSERT_EQUAL( 0, chess.evaluate1() );

		int whitePower, blackPower;
		chess.evaluate1(whitePower, blackPower);
		UT_ASSERT_EQUAL( whitePower, blackPower );
		UT_ASSERT_EQUAL( whitePower, 1038 );

		int targets, beats;
		chess.evaluate2(targets, beats);
		UT_ASSERT_EQUAL( 0, targets );
		UT_ASSERT_EQUAL( 0, beats );

		int whiteTargets, blackTargets, whiteBeats, blackBeats;
		chess.evaluate2(whiteTargets, blackTargets, whiteBeats, blackBeats);

		UT_ASSERT_EQUAL( 20, whiteTargets );
		UT_ASSERT_EQUAL( 20, blackTargets );

		UT_ASSERT_EQUAL( 0, whiteBeats );
		UT_ASSERT_EQUAL( 0, blackBeats );

		chess::Position badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'E', 2 ), chess::Position( 'E', 4 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'F', 7 ), chess::Position( 'F', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'E', 4 ), chess::Position( 'F', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'G', 7 ), chess::Position( 'G', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'F', 5 ), chess::Position( 'G', 6 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.evaluate2(whiteTargets, blackTargets, whiteBeats, blackBeats);
		UT_ASSERT_EQUAL( 1, whiteBeats );
		UT_ASSERT_EQUAL( 1, blackBeats );

		// Pawn, Queen, King, Knight
		UT_ASSERT_EQUAL( 16 + 4 + 1 + 5 + 5, whiteTargets );

		// try rochade
		chess.moveTo(chess::Position( 'B', 8 ), chess::Position( 'A', 6 ));
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'F', 1 ), chess::Position( 'E', 2 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo(chess::Position( 'D', 7 ), chess::Position( 'D', 6 ));
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'G', 1 ), chess::Position( 'F', 3 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::Position( 'D', 6 ), chess::Position( 'D', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		const chess::King *king = chess.getWhiteK();
		const chess::TargetPositions	&result = king->getPossible();
		UT_ASSERT_EQUAL( result.numTargets, 2 );

		const chess::King::Rochade &eastRochade = king->getEastRochade();

		const chess::Figure * rook1 = eastRochade.rook;
		chess::Position	pos('H', 1);
		const chess::Figure * rook2 = chess.getFigure( pos );
		UT_ASSERT_EQUAL( rook1, rook2 );
		
		chess.rochade(king, rook1, eastRochade.myTarget, eastRochade.rookTarget );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );
		UT_ASSERT_EQUAL( king->getPos().col, 'G' );
		UT_ASSERT_EQUAL( king->getPos().row, 1 );
		UT_ASSERT_EQUAL( rook1->getPos().col, 'F' );
		UT_ASSERT_EQUAL( rook1->getPos().row, 1 );

		chess.print();
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static ChessTest myChessTest;

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

