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
		doDisableLog();
		//TestScope scope( "PerformTest" );
		chess::Board	chess;
		int quality;

		// find move for king
		{
			chess.generateFromString(
				"     K  "
				"     d  "
				"        "
				"     k  "
				"        "
				"        "
				"        "
				"        "
				"W"
			);
			chess::King *king = chess.getCurKing();
			const chess::PotentialDestinations &targets = king->getPossible();
			UT_ASSERT_EQUAL(targets.numTargets, 1);
			UT_ASSERT_EQUAL(targets.hasCaptures, 1);

			chess.findBest(2, &quality);
			UT_ASSERT_EQUAL(quality, 1);
		}

		// find mate
		{
#if 0
			chess.generateFromString(
				"    K   "
				"        "
				"        "
				"        "
				"        "
				"   D    "
				"   B    "
				"   k    "
				"W"
			);
#else
			chess.generateFromString(
				"     K  "
				"        "
				"     k  "
				"        "
				"        "
				"        "
				"d       "
				"        "
				"S"
			);
#endif
			chess.findBest(2, &quality);
			UT_ASSERT_EQUAL(quality, 1);
			chess.findBest(1, &quality);
			UT_ASSERT_EQUAL(quality, 1);
		}

		const STRING start = "TSLDKLST"
							 "BBBBBBBB"
							 "        "
							 "        "
							 "        "
							 "        "
							 "bbbbbbbb"
							 "tsldklst" ;

		chess.reset();
		UT_ASSERT_EQUAL( 0, chess.evaluateForce() );

		int whiteForce, blackForce;
		chess.evaluateForce(whiteForce, blackForce);
		UT_ASSERT_EQUAL( whiteForce, blackForce );
		UT_ASSERT_EQUAL( whiteForce, chess::INIT_VALUE );

		int targets, captures;
		chess.evaluateRange(targets, captures);
		UT_ASSERT_EQUAL( 0, targets );
		UT_ASSERT_EQUAL( 0, captures );

		int whiteTargets, blackTargets, whiteCaptures, blackCaptures;
		chess.evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);

		UT_ASSERT_EQUAL( 20, whiteTargets );
		UT_ASSERT_EQUAL( 20, blackTargets );

		UT_ASSERT_EQUAL( 0, whiteCaptures );
		UT_ASSERT_EQUAL( 0, blackCaptures );

		chess::Position badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		const STRING initString = chess.generateString(); 
		UT_ASSERT_EQUAL( start, initString );

		chess.moveTo( chess::PlayerPos( 'e', 2, chess ), chess::Position( 'e', 4 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'f', 7, chess ), chess::Position( 'f', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'e', 4, chess ), chess::Position( 'f', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'g', 7, chess ), chess::Position( 'g', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'f', 5, chess ), chess::Position( 'g', 6 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );
		const chess::Figure *fig = chess.getFigure('g', 5);
		UT_ASSERT_TRUE( fig==NULL );

		chess.evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);
		UT_ASSERT_EQUAL( 1, whiteCaptures );
		UT_ASSERT_EQUAL( 1, blackCaptures );

		// Pawn, Queen, King, Knight
		UT_ASSERT_EQUAL( 16 + 4 + 1 + 5 + 5, whiteTargets );

		// try rochade
		chess.moveTo(chess::PlayerPos( 'b', 8, chess ), chess::Position( 'a', 6 ));
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'f', 1, chess ), chess::Position( 'e', 2 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo(chess::PlayerPos( 'd', 7, chess ), chess::Position( 'd', 6 ));
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'g', 1, chess ), chess::Position( 'f', 3 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'd', 6, chess), chess::Position( 'd', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess::King *king = chess.getWhiteK();
		const chess::PotentialDestinations	&result = king->getPossible();
		UT_ASSERT_EQUAL( result.numTargets, 2 );

		const chess::King::Rochade &eastRochade = king->getEastRochade();

		chess::Figure		*rook1 = eastRochade.rook;
		chess::PlayerPos	rook2('h', 1, chess);
		UT_ASSERT_EQUAL( rook1, rook2.fig );
		
		chess.rochade(king, rook1, eastRochade.myTarget, eastRochade.rookTarget );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );
		UT_ASSERT_EQUAL( king->getPos().col, 'g' );
		UT_ASSERT_EQUAL( king->getPos().row, 1 );
		UT_ASSERT_EQUAL( rook1->getPos().col, 'f' );
		UT_ASSERT_EQUAL( rook1->getPos().row, 1 );

		chess.moveTo( chess::PlayerPos( 'h', 7, chess ), chess::Position( 'h', 5 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'g', 6, chess ), chess::Position( 'g', 7 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.moveTo( chess::PlayerPos( 'e', 7, chess ), chess::Position( 'e', 6 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		chess.promote( chess.getFigure(chess::Position( 'g', 7 )), 
			chess::Figure::ftQueen, chess::Position( 'h', 8 ) );
		badPos = chess.checkBoard();
		UT_ASSERT_EQUAL( chess::Position(), badPos );

		const STRING expected =
			"TSLD TK "
			"BBBBLBBB"
			"     S  "
			"        "
			"   b   b"
			"s   b   "
			"bbb     "
			"t ldklsD" ;

		const STRING finalString = chess.generateString(); 
		UT_ASSERT_EQUAL( expected, finalString );

//		UT_ASSERT_TRUE( false );
		chess.print();

		// from upper right to lower left
		chess::Position::MoveFunc func = chess::Position::findMoveFunc(chess::Position( 'h', 8 ), chess::Position( 'a', 1 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveSouthWest );

		// from right to left
		func = chess::Position::findMoveFunc(chess::Position( 'h', 8 ), chess::Position( 'a', 8 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveWest );

		// from lower right to upper left
		func = chess::Position::findMoveFunc(chess::Position( 'h', 1 ), chess::Position( 'a', 8 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveNorthWest );

		// from lower to upper
		func = chess::Position::findMoveFunc(chess::Position( 'h', 1 ), chess::Position( 'h', 8 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveNorth );

		// from lower left to upper right
		func = chess::Position::findMoveFunc(chess::Position( 'a', 1 ), chess::Position( 'h', 8 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveNorthEast );

		// from left to right
		func = chess::Position::findMoveFunc(chess::Position( 'a', 1 ), chess::Position( 'h', 1 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveEast );

		// from upper left to lower right
		func = chess::Position::findMoveFunc(chess::Position( 'a', 8 ), chess::Position( 'h', 1 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveSouthEast );

		// from upper to lower
		func = chess::Position::findMoveFunc(chess::Position( 'h', 8 ), chess::Position( 'h', 1 ));
		UT_ASSERT_EQUAL( func, &chess::Position::moveSouth );

		// bad
		func = chess::Position::findMoveFunc(chess::Position( 'c', 8 ), chess::Position( 'h', 1 ));
		UT_ASSERT_EQUAL( func, chess::Position::MoveFunc(NULL) );

		// equal
		func = chess::Position::findMoveFunc(chess::Position( 'c', 5 ), chess::Position( 'c', 5 ));
		UT_ASSERT_EQUAL( func, chess::Position::MoveFunc(NULL) );
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

