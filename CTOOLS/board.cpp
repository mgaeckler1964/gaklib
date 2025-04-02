/*
		Project:		Gaklib
		Module:			board.cpp
		Description:	the chess board
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

#include <gak/chess.h>

#include <gak/logfile.h>

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
namespace chess
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const gakLogging::LogLevel logLegel = gakLogging::llDetail;

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

Figure *Board::checkEnPassant(const PlayerPos &src, const Position &dest) const
{
	int rowMove = dest.row - src.pos.row;
	int colMove = dest.col - src.pos.col;

	if( !rowMove || rowMove < -1 ||  rowMove > 1 )
	{
		return NULL;
	}
	if( !colMove || colMove < -1 ||  colMove > 1 )
	{
		return NULL;
	}

	size_t srcIndex = src.index;
	Figure *fig = m_board[srcIndex];
	
	if( !fig->getType() == Figure::ftPawn )
	{
		return NULL;
	}

	size_t destIndex = getIndex( dest );
	Figure *destFig = m_board[destIndex];
	if( destFig )
	{
		return NULL;
	}

	Position enPasantPos(dest.col, src.pos.row);
	destIndex = getIndex( enPasantPos );
	destFig = m_board[destIndex];
	if( destFig && destFig->getType() == Figure::ftPawn && destFig->m_color != fig->m_color )
	{
		size_t size = m_moves.size();
		if( size )
		{
			const Movement &last = m_moves[size-1];
			if( last.dest != enPasantPos && last.src.col != last.dest.col )
			{
				return NULL;
			}
			int rowCount = last.src.row - last.dest.row;
			if( rowCount == - 2 || rowCount == 2 )
			{
				return destFig;
			}
		}
	}

	return NULL;
}

Figure *Board::uncheckedMove( const PlayerPos &src, const Position &dest )
{
	size_t srcIndex = src.index;
	size_t destIndex = getIndex( dest );
	Figure *fig = src.fig;
	Figure *toCapture = m_board[destIndex];

	m_board[destIndex] = fig;
	fig->moveTo(dest);
	m_board[srcIndex] = NULL;

	return toCapture;
}

Figure *Board::passantMove( const PlayerPos &src, const Position &dest )
{
	Figure *enPassantDel = checkEnPassant(src,dest);
	Figure *toCapture = uncheckedMove(src,dest);

	if( enPassantDel )
	{
		assert(!toCapture);

		size_t destIndex = getIndex( enPassantDel->getPos() );
		m_board[destIndex] = NULL;

		return enPassantDel;
	}

	return toCapture;
}

void Board::reset(Figure::Color color)
{
	char row = (color == Figure::White) ? 2 : 7;
	for( char col='A'; col <='H'; ++col )
	{
		create(color,Figure::ftPawn, Position(col,row));
	}

	char col = 'A';
	row = (color == Figure::White) ? 1 : NUM_ROWS;

	create(color, Figure::ftRook, Position(col,row));

	++col;
	create(color, Figure::ftKnight, Position(col,row));

	++col;
	create(color,Figure::ftBishop, Position(col,row));

	++col;
	create(color, Figure::ftQueen, Position(col,row));

	++col;
	create(color, Figure::ftKing, Position(col,row));

	++col;
	create(color,Figure::ftBishop, Position(col,row));

	++col;
	create(color, Figure::ftKnight, Position(col,row));

	++col;
	create(color, Figure::ftRook, Position(col,row));
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

const Figure *Board::getAttacker( const Figure *fig ) const
{
	const Figure::Color color = fig->m_color;
	const Position &pos = fig->getPos();

	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		const Figure *fig = m_board[i];
		if( fig && fig->m_color != color )
		{
			const TargetPositions &targets = fig->getPossible();
			for( size_t j=0; j<targets.numCaptures; ++j )
			{
				if( targets.captures[j] == pos )
				{
					return fig;
				}
			}
		}
	}

	return NULL;
}

size_t Board::getAttackers( const Figure *fig, FigurePtr *attackers ) const
{
	const Figure::Color color = fig->m_color;
	const Position &pos = fig->getPos();
	size_t numAttackers = 0;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		FigurePtr fig = m_board[i];
		if( fig && fig->m_color != color )
		{
			const TargetPositions &targets = fig->getPossible();
			for( size_t j=0; j<targets.numCaptures; ++j )
			{
				if( targets.captures[j] == pos )
				{
					attackers[numAttackers++] = fig;
				}
			}
		}
	}

	return numAttackers;
}

const Figure *Board::getThread( Figure::Color color, const Position &pos, bool checkEnPassant, bool check4King ) const
{
	if( check4King )
	{
		King *king = getOponentKing(color);
		if( math::abs(king->getPos().row - pos.row) <2 && math::abs(king->getPos().col - pos.col) <2 )
		{
			return king;
		}
	}
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		const Figure *fig = m_board[i];
		if( fig && fig->m_color != color && fig->getPos() != pos )
		{
			const TargetPositions &targets = fig->getPossible();
			for( size_t j=0; j<targets.numThreads; ++j )
			{
				if( targets.threads[j] == pos )
				{
					return fig;
				}
			}
		}
	}

	return NULL;
}

size_t Board::getThreads( Figure::Color color, const Position &pos, FigurePtr *threads, bool checkEnPassant ) const
{
	size_t numThreads = 0;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig->m_color != color )
		{
			const TargetPositions &targets = fig->getPossible();
			for( size_t j=0; j<targets.numThreads; ++j )
			{
				if( targets.threads[j] == pos )
				{
					threads[numThreads++] = fig;
				}
			}
		}
	}

	return numThreads;
}

bool Board::checkMoveTo( const PlayerPos &src, const Position &dest, Figure::Type newFig ) const
{
	if( !canPlay() )
	{
		return true;
	}
	const Figure *fig = src.fig;
	if( !fig )
	{
		return true;
	}
	if( fig->m_color !=	m_nextColor )
	{
		return true;
	}

	if( newFig > Figure::ftNone )
	{
		if( newFig <= Figure::ftPawn 
		|| newFig >= Figure::ftKing 
		|| fig->getType() != Figure::ftPawn
		|| (dest.row != 1 && dest.row != NUM_ROWS)	)
		{
			return true;
		}
	}
	else if(fig->getType() == Figure::ftPawn && (dest.row == 1 || dest.row == NUM_ROWS))
	{
		return true;
	}
	const TargetPositions &targets = fig->getPossible();
	for( size_t i=0; i<targets.numTargets; ++i )
	{
		if( targets.targets[i] == dest )
		{
			return false;
		}
	}

	// final we have to check whether our king is now checked
	return getCurKing()->isAttacked(src.pos,dest);
}

void Board::moveTo( const PlayerPos &src, const Position &dest )
{
	assert(!checkMoveTo(src, dest) );

	Figure *toCapture = passantMove(src, dest);

	Movement &move = m_moves.createElement();
	move.fig = src.fig;
	move.src = src.pos;
	move.dest = dest;

	if( toCapture )
	{
		move.captured = toCapture;
		move.capturePos = toCapture->getPos();
		toCapture->capture();
	}

	flipTurn();
	refresh();
}

void Board::rochade( const PlayerPos &king, const PlayerPos &rook, const Position &kingDest, const Position &rookDest )
{
	assert(!checkMoveTo(king, kingDest) );
	assert(!checkMoveTo(rook, rookDest) );

	Figure *toCapture = uncheckedMove(king, kingDest );
	assert(!toCapture);
	toCapture = uncheckedMove(rook, rookDest );
	assert(!toCapture);

	Movement &move = m_moves.createElement();
	move.fig = king.fig;
	move.src = king.pos;
	move.dest = kingDest;
	move.rook = rook.fig;
	move.rookSrc = rook.pos;
	move.rookDest = rookDest;

	flipTurn();
	refresh();
}

Figure *Board::create( Figure::Color color, Figure::Type newFig, const Position &dest )
{
	Figure *newFigure;
	size_t destIndex = getIndex( dest );

	switch(newFig)
	{
	case Figure::ftPawn:
		newFigure = new Pawn(color, dest, *this);
		break;
	case Figure::ftBishop:
		newFigure = new Bishop(color, dest, *this);
		break;
	case Figure::ftKnight:
		newFigure = new Knight(color, dest, *this);
		break;
	case Figure::ftRook:
		newFigure = new Rook(color, dest, *this);
		break;
	case Figure::ftQueen:
		newFigure = new Queen(color, dest, *this);
		break;
	case Figure::ftKing:
		newFigure = new King(color, dest, *this);
		break;
	}
	m_board[destIndex] = newFigure;
	m_all.addElement(newFigure);

	return newFigure;
}

void Board::promote( const PlayerPos &pawn, Figure::Type newFig, const Position &dest )
{
	assert(!checkMoveTo(pawn, dest, newFig) );

	size_t srcIndex = pawn.index;
	size_t destIndex = getIndex( dest );

	m_board[srcIndex] = NULL;

	Figure *toCapture = m_board[destIndex];
	Figure *newFigure = create( pawn.fig->m_color, newFig, dest );

	pawn.fig->capture();

	Movement &move = m_moves.createElement();
	move.fig = pawn.fig;
	move.src = pawn.pos;
	move.dest = dest;
	move.promotion = newFigure;
	if( toCapture )
	{
		move.captured = toCapture;
		move.capturePos = toCapture->getPos();
		toCapture->capture();
	}

	flipTurn();
	refresh();
}

void Board::reset()
{
	clear();
	reset(Figure::White);
	reset(Figure::Black);

	m_whiteK = dynamic_cast<King*>(getFigure('E', 1));
	m_blackK = dynamic_cast<King*>(getFigure('E', NUM_ROWS));

	m_nextColor = Figure::White;
	m_state = csPlaying;
	refresh();
}

void Board::refresh()
{
	if( canPlay() )
	{
		m_state = csPlaying;
	}
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig->getType() != Figure::ftKing)
		{
			fig->refresh();
		}
	}

	// since king must not move to attacked fields all other figures must be refreshed befor 
	m_whiteK->refresh();
	m_blackK->refresh();

	if( canPlay() )
	{
		checkCheck();
	}
}

void Board::checkCheck()
{
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig->getType() != Figure::ftKing )
		{
			const TargetPositions &targets = fig->getPossible();
			for( size_t i=0; i<targets.numCaptures; ++i )
			{
				if( m_whiteK->getPos() == targets.captures[i] )
				{
					m_state = csWhiteCheck;
/*@*/				return;
				}
				else if( m_blackK->getPos() == targets.captures[i] )
				{
					m_state = csBlackCheck;
/*@*/				return;
				}
			}
		}
	}
}

void Board::evaluateForce( int &whiteForce, int &blackForce) const
{
	whiteForce=0;
	blackForce=0;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		if( m_board[i] )
		{
			int force = m_board[i]->getValue();
			if( m_board[i]->m_color == Figure::White )
			{
				whiteForce += force;
			}
			else
			{
				blackForce += force;
			}
		}
	}
}

void Board::evaluateRange(int &whiteTargets, int &blackTargets, int &whiteCaptures, int &blackCaptures) const
{
	whiteTargets = blackTargets = whiteCaptures = blackCaptures = 0;
	for( int i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig )
		{
			const TargetPositions &pos = fig->getPossible();
			int captures=0;
			for( int j=0; j<pos.numCaptures; ++j )
			{
				captures += getFigure(pos.captures[j])->getValue();
			}

			if( fig->m_color == Figure::White )
			{
				whiteTargets += int(pos.numTargets);
				whiteCaptures += captures;
			}
			else
			{
				blackTargets += int(pos.numTargets);
				blackCaptures += captures;
			}
		}
	}
}

int Board::evaluate() const
{
	doEnterFunctionEx(logLegel, "Board::evaluate");
	int whiteForce, blackForce;
	int whiteTargets, blackTargets, whiteCaptures, blackCaptures;

	evaluateForce( whiteForce, blackForce);
	if( whiteForce < KING_VALUE )
	{
		return BLACK_WINS;
	}
	if( blackForce < KING_VALUE )
	{
		return WHITE_WINS;
	}

	if( m_state == csWhiteCheck )
	{
		if( isBlackTurn() )
		{
			return BLACK_WINS;
		}
		return WHITE_CHECK;
	}
	else if( m_state == csBlackCheck )
	{
		if( isWhiteTurn() )
		{
			return WHITE_WINS;
		}
		return BLACK_CHECK;
	}

	evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);

	int evaluation = whiteForce + whiteTargets + whiteCaptures - blackForce - blackTargets - blackCaptures;
	doLogValueEx(logLegel, evaluation);
	return whiteForce + whiteTargets + whiteCaptures - blackForce - blackTargets - blackCaptures;
}

void Board::undoMove(const Movement &move)
{
	size_t srcIndex = getIndex(move.src);
	size_t destIndex = getIndex(move.dest);

	// re pos figure
	move.fig->setPosition(move.src);
	m_board[srcIndex] = move.fig;

	// if it was a promotion, remove new figure
	if( move.promotion )
	{
		move.promotion->capture();
	}

	if( !move.captured )
	{
		// no caption -> the old destination is now empty
		m_board[destIndex] = NULL;
	}
	else
	{
		// restore captured
		size_t capIndex = getIndex(move.capturePos);
		m_board[capIndex] = move.captured;
		if( destIndex != capIndex )
		{
			// if it is en-passant -> the old destination is now empty
			m_board[destIndex] = NULL;
		}
	}

	if( move.rook )
	{
		// restore rook, if it was a rochade
		size_t srcIndex = getIndex(move.rookSrc);
		size_t destIndex = getIndex(move.rookDest);
		move.rook->setPosition(move.rookSrc);
		m_board[srcIndex] = move.rook;
		m_board[destIndex] = NULL;
	}
}

void Board::redoMove(const Movement &move)
{
	size_t srcIndex = getIndex(move.src);
	size_t destIndex = getIndex(move.dest);

	// position figure
	if( move.promotion )
	{
		move.fig->capture();
		move.promotion->setPosition(move.dest);
		m_board[destIndex] = move.promotion;
	}
	else
	{
		move.fig->setPosition(move.dest);
		m_board[destIndex] = move.fig;
	}
	m_board[srcIndex] = NULL;

	if( move.captured )
	{
		move.captured->capture();
	}

	if( move.rook )
	{
		// restore rook, if it was a rochade
		size_t srcIndex = getIndex(move.rookSrc);
		size_t destIndex = getIndex(move.rookDest);
		move.rook->setPosition(move.rookDest);
		m_board[destIndex] = move.rook;
		m_board[srcIndex] = NULL;
	}
}

Movements Board::collectMoves() const
{
	doEnterFunctionEx(logLegel, "Board::collectMoves");

	Movements moves;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig->m_color == m_nextColor )
		{
			const TargetPositions &curMoves = fig->getPossible();
			for( int i=0; i<curMoves.numTargets; ++i )
			{
				Movement &move = moves.createElement();
				move.fig = fig;
				move.src = fig->getPos();
				move.dest = curMoves.targets[i];
			}
		}
	}
	return moves;
}

size_t Board::findMove(const Movements &moves, const Position &src, const Position &dest )
{
	for( size_t i=0; i<moves.size(); ++i )
	{
		const Movement &move = moves[i];
		if( move.src == src && move.dest == dest )
		{
			return i;
		}
	}
	return Movements::no_index;
}

Movements Board::findCheckDefend(size_t *numAttackers) const
{
	assert(numAttackers);
	Movements defends;
	Figure *king = getCurKing();
	FigurePtr attackers[NUM_TEAM_FIGURES];
	*numAttackers = getAttackers(king, attackers);

	if( *numAttackers == 1 )
	{
		// try to capture the attacker
		FigurePtr defenders[NUM_TEAM_FIGURES];
		size_t numDefenders = getAttackers(attackers[0], defenders);
		for( size_t i=0; i<numDefenders; ++i )
		{
			Movement &defend = defends.createElement();
			defend.fig = defenders[i];
			defend.src = defenders[i]->getPos();
			defend.capturePos = defend.dest = attackers[0]->getPos();
			defend.captured = attackers[0];
		}

		// try to shield the king 
		if( attackers[0]->getType() != Figure::ftKnight )
		{
			Position targetPos = king->getPos();
			const Position &attackerPos = attackers[0]->getPos();
			Position::MoveFunc	move = Position::findMoveFunc(targetPos, attackerPos);
			int dist = Position::getDistance(king->getPos(), attackers[0]->getPos());
			assert(move);

			if(dist>1)
			{
				while(1)
				{
					targetPos = (targetPos.*move)();
					if( !targetPos || targetPos == attackerPos)
						break;

					FigurePtr defenders[NUM_TEAM_FIGURES];
					size_t numDefenders = getThreads(attackers[0]->m_color, targetPos, defenders, false);
					for( size_t i=0; i<numDefenders; ++i )
					{
						Movement &defend = defends.createElement();
						defend.fig = defenders[i];
						defend.src = defenders[i]->getPos();
						defend.dest = targetPos;
					}
				}
			}
		}
	}

	// final we add the escape options this is possible regardless the number of attackers
	const TargetPositions &escapes = king->getPossible();
	const Position &src = king->getPos();
	for( int i=0; i<escapes.numTargets; ++i )
	{
		const Position &dest = escapes.targets[i];
		if( findMove( defends, src, dest ) == defends.no_index )
		{
			Movement &defend = defends.createElement();
			defend.fig = king;
			defend.src = src;
			defend.dest = dest;
		}
	}
	return defends;
}

int Board::evaluateMovements(Movements &movements, int maxLevel)
{
	doEnterFunctionEx(logLegel, "Board::evaluateMovements");

	--maxLevel;
	int minVal = std::numeric_limits<int>::max();
	int maxVal = std::numeric_limits<int>::min();;
	flipTurn();
	for(
		Movements::iterator it = movements.begin(), endIT = movements.end();
		it != endIT;
		++it
	)
	{
		Movement &theMove = *it;

		redoMove(theMove);
		refresh();
		theMove.evaluate = evaluate();
		minVal = math::min(minVal, theMove.evaluate);
		maxVal = math::max(maxVal, theMove.evaluate);
		if( maxLevel )
		{
			int quality;
			Movement nextMove = findBest( maxLevel, &quality );
			if( nextMove )
			{
				theMove.nextValue = nextMove.nextValue;
			}
			else
			{
				if( isWhiteTurn() )
				{
					theMove.evaluate = BLACK_WINS;
					theMove.nextValue = BLACK_WINS;
				}
				else
				{
					theMove.evaluate = WHITE_WINS;
					theMove.nextValue = WHITE_WINS;
				}
				minVal = math::min(minVal, theMove.evaluate);
				maxVal = math::max(maxVal, theMove.evaluate);
			}
		}

		undoMove(*it);
	}
	flipTurn();
	return isWhiteTurn() ? maxVal : minVal;
}

Movement Board::findBest( int maxLevel, int *quality )
{
	doEnterFunctionEx(logLegel, "Board::findBest");
	assert( quality );
	if( maxLevel <= 0 )
	{
		return Movement();
	}

	int			bestEval;
	Movement	best;
	size_t		numAttackers;
	Movements	movements = findCheckDefend(&numAttackers);

	if( !numAttackers )
	{
		movements = collectMoves();
	}

	if( movements.size() )
	{
		bestEval = evaluateMovements(movements, maxLevel);

		int minNextEval = std::numeric_limits<int>::max();
		int maxNextEval = std::numeric_limits<int>::min();;

		size_t	numMoves=0;
		for( size_t i=0; i<movements.size(); ++i )
		{
			const Movement &move = movements[i];
			if( move.evaluate == bestEval )
			{
				if(i != numMoves)
				{
					movements[numMoves] = move;
				}
				numMoves++;
				maxNextEval = math::max(maxNextEval, move.nextValue);
				minNextEval = math::min(minNextEval, move.nextValue);
			}
		}
		movements.removeElementsAt(numMoves, movements.size());

		int nextValue = isWhiteTurn() ? minNextEval : maxNextEval;
		numMoves=0;
		for( size_t i=0; i<movements.size(); ++i )
		{
			if( movements[i].nextValue == nextValue )
			{
				if(i != numMoves)
				{
					movements[numMoves] = movements[i];
				}
				numMoves++;
			}
		}
		movements.removeElementsAt(numMoves, movements.size());
	}
	if( movements.size() )
	{
		size_t index = randomNumber(movements.size());
		best = movements[index];
	}
	*quality = int(movements.size());
	refresh();
	return best;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace chess
}	//namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

