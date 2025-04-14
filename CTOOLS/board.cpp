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

#include <gak/chess.h>

#include <gak/logfile.h>
#include <gak/thread.h>

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
	m_board[srcIndex] = NULL;
	fig->setPosition(dest, true);

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

void Board::undoMove(const Movement &move)
{
	m_moves.removeElementAt(m_moves.size()-1);

	size_t srcIndex = getIndex(move.src);
	size_t destIndex = getIndex(move.dest);

	// re pos figure
	move.fig->setPosition(move.src, false);
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
		move.captured->setPosition(move.capturePos, false);
	}

	if( move.rook )
	{
		// restore rook, if it was a rochade
		size_t srcIndex = getIndex(move.rookSrc);
		size_t destIndex = getIndex(move.rookDest);
		move.rook->setPosition(move.rookSrc, false);
		m_board[srcIndex] = move.rook;
		m_board[destIndex] = NULL;
	}

	m_state = csPlaying;
}

void Board::redoMove(Movement &move)
{
	size_t srcIndex = getIndex(move.src);
	size_t destIndex = getIndex(move.dest);

	// save captured
	if(m_board[destIndex])
	{
		move.captured = m_board[destIndex];
	}
	// position figure
	if( move.promotionType && !move.promotion )
	{
		move.promotion = create( move.fig->m_color, move.promotionType, move.dest );
	}
	if( move.promotion )
	{
		move.fig->capture();
		move.promotion->setPosition(move.dest, false);
		m_board[destIndex] = move.promotion;
	}
	else
	{
		move.fig->setPosition(move.dest, false);
		m_board[destIndex] = move.fig;
	}
	m_board[srcIndex] = NULL;

	Figure *cap = move.captured;
	if( cap )
	{
		const Position &cPos = cap->getPos();
		if( cPos )
		{
			move.capturePos = cPos;
		}
		cap->capture();
		assert(move.capturePos);
	}

	Figure *rook = move.rook;
	if( rook )
	{
		// restore rook, if it was a rochade
		size_t srcIndex = getIndex(move.rookSrc);
		size_t destIndex = getIndex(move.rookDest);
		rook->setPosition(move.rookDest, false);
		m_board[destIndex] = rook;
		m_board[srcIndex] = NULL;
	}

	m_moves.addElement(move);
}

Movements Board::collectMoves() const
{
	doEnterFunctionEx(gakLogging::llInfo, "Board::collectMoves");
	Movements moves;
	for( size_t i1=0; i1<NUM_FIELDS; ++i1 )
	{
		Figure *fig = m_board[i1];
		if( fig && fig->m_color == m_nextColor )
		{
			const Position &src = fig->getPos();
			const PotentialDestinations &curMoves = fig->getPossible();
			for( size_t i2=0; i2<curMoves.numTargets; ++i2 )
			{
				const Position &dest = curMoves.targets[i2].getTarget();
				Figure *cap = m_board[getIndex(dest)];
				if(fig->mustPromote(dest) )
				{
					Movement move;
					move.fig = fig;
					move.src = src;
					move.dest = dest;
					move.captured = cap;
					if( cap )
					{
						move.capturePos = cap->getPos();
					}
					addPromoteMoves(moves,move);
				}
				else
				{
					Movement &move = moves.createElement();
					move.fig = fig;
					move.src = src;
					move.dest = dest;
					move.captured = cap;
					if( cap )
					{
						move.capturePos = cap->getPos();
					}
					if(fig->getType() == Figure::ftKing)
					{
						const King *king = static_cast<const King*>(fig);
						const gak::chess::King::Rochade *rochade = king->getRochade(dest);
						if( rochade )
						{
							move.rook = rochade->rook;
							move.rookSrc = rochade->rook->getPos();
							move.rookDest = rochade->rookTarget;
						}
					}
				}
			}
		}
	}
	return moves;
}

bool Board::canMove() const
{
	doEnterFunctionEx(logLegel, "Board::canMove");
	
	size_t numAttackers;
	Movements moves = Board::findCheckDefend(&numAttackers);
	if( numAttackers )
	{
		return moves.size() ? true : false;
	}

	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig->m_color == m_nextColor )
		{
			const PotentialDestinations &curMoves = fig->getPossible();
			if( curMoves.numTargets )
			{
				return true;
			}
		}
	}

	return false;
}

Movement Board::findBest( int maxLevel, int *quality, bool recalcState )
{
	doEnterFunctionEx(logLegel, "Board::findBest( int maxLevel, int *quality, bool recalcState )");
	assert( quality );

	const State			state = m_state;
	const Figure::Color	nextColor = m_nextColor;

	int			bestEval;
	Movement	best;
	size_t		numAttackers;
	Movements	movements = findCheckDefend(&numAttackers);

	if( !numAttackers )
	{
		movements = collectMoves();
	}

	if( movements.size() && maxLevel > 0)
	{
		bestEval = evaluateMovements(movements, maxLevel);

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
			}
		}
		movements.removeElementsAt(numMoves, movements.size());
	}
	if( movements.size() )
	{
		size_t index = randomNumber(int(movements.size()));
		best = movements[index];
	}
	*quality = int(movements.size());

	m_nextColor = nextColor;
	if( recalcState )
	{
		refresh();
		if( numAttackers && !movements.size() )
		{
			m_state = isWhiteTurn() ? csWhiteCheckMate : csBlackCheckMate;
		}
	}
	else
	{
		m_state = state;
	}

	return best;
}

Movements Board::findCheckDefend(size_t *numCheckers) const
{
	assert(numCheckers);
	Movements defends;
	Figure *king = getCurKing();
	FigurePtr attackers[NUM_TEAM_FIGURES];
	*numCheckers = getCheckers(king, attackers);

	if( *numCheckers == 1 )
	{
		// try to capture the attacker
		FigurePtr defenders[NUM_TEAM_FIGURES];
		size_t numDefenders = getAttackers(attackers[0], defenders);
		const Position &dest = attackers[0]->getPos();
		for( size_t i=0; i<numDefenders; ++i )
		{
			if(defenders[i]->mustPromote(dest) )
			{
				Movement defend;
				defend.fig = defenders[i];
				defend.src = defenders[i]->getPos();
				defend.capturePos = defend.dest = dest;
				defend.captured = attackers[0];
				addPromoteMoves( defends, defend );
			}
			else
			{
				Movement &defend = defends.createElement();
				defend.fig = defenders[i];
				defend.src = defenders[i]->getPos();
				defend.capturePos = defend.dest = dest;
				defend.captured = attackers[0];
			}
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
					size_t numDefenders = getShields(attackers[0]->m_color, targetPos, defenders);
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
	const PotentialDestinations &escapes = king->getPossible();
	const Position &src = king->getPos();
	for( size_t i=0; i<escapes.numTargets; ++i )
	{
		const Position &dest = escapes.targets[i].getTarget();
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
	SharedObjectPointer<Thread>	currentThread = Thread::FindCurrentThread();

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

#ifndef NDEBUG
		size_t srcIndex = getIndex(theMove.src);
		size_t destIndex = getIndex(theMove.dest);
		if(srcIndex == 27 && maxLevel<=1)
		{
			srcIndex++;
			doLogValue(srcIndex);
			doLogValue(destIndex);
			srcIndex--;
		}
#endif
		redoMove(theMove);
		refresh();
		theMove.evaluate = evaluate();
		if( maxLevel > 0 && (!currentThread || !currentThread->terminated) )
		{
			int quality;
			Movement nextMove = findBest( maxLevel, &quality, false );
			if( nextMove )
			{
				theMove.evaluate = nextMove.evaluate;
			}
			else
			{
				if( (m_state == csBlackCheck && isBlackTurn()) ||  m_state == csBlackCheckMate )
				{
					theMove.evaluate = WHITE_WINS;
				}
				else if( (m_state == csWhiteCheck && isWhiteTurn()) ||  m_state == csWhiteCheckMate )
				{
					theMove.evaluate = BLACK_WINS;
				}
				else if(m_state == csDraw)
				{
					theMove.evaluate = 0;
				}
			}
		}
		minVal = math::min(minVal, theMove.evaluate);
		maxVal = math::max(maxVal, theMove.evaluate);

		undoMove(*it);
	}
	flipTurn();
	return isWhiteTurn() ? maxVal : minVal;
}

void Board::reset(Figure::Color color)
{
	char row = (color == Figure::White) ? 2 : 7;
	for( char col=MIN_COL_LETTER; col <=MAX_COL_LETTER; ++col )
	{
		create(color,Figure::ftPawn, Position(col,row));
	}

	char col = MIN_COL_LETTER;
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
			const PotentialDestinations &targets = fig->getPossible();
			if( targets.hasCaptures )
			{
				for( size_t j=0; j<targets.numTargets; ++j )
				{
					if( targets.targets[j].getCapture() == pos )
					{
						return fig;
					}
				}
			}
		}
	}

	return NULL;
}

size_t Board::getAttackers( const Figure *myFig, FigurePtr *attackers ) const
{
	const Figure::Color color = myFig->m_color;
	const Position &pos = myFig->getPos();
	size_t numAttackers = 0;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		FigurePtr oponent = m_board[i];
		if( oponent && oponent->m_color != color )
		{
			const PotentialDestinations &targets = oponent->getPossible();
			if( targets.hasCaptures )
			{
				for( size_t j=0; j<targets.numTargets; ++j )
				{
					if( targets.targets[j].getCapture() == pos )
					{
						attackers[numAttackers++] = oponent;
					}
				}
			}
		}
	}

	return numAttackers;
}

size_t Board::getCheckers( const Figure *king, FigurePtr *checkers ) const
{
	const Figure::Color color = king->m_color;
	const Position &pos = king->getPos();
	size_t numCheckers = 0;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		FigurePtr oponent = m_board[i];
		if( oponent && oponent->m_color != color )
		{
			const PotentialDestinations &targets = oponent->getPossible();
			for( size_t j=0; j<targets.numThreads; ++j )
			{
				if( targets.threads[j] == pos )
				{
					checkers[numCheckers++] = oponent;
				}
			}
		}
	}

	return numCheckers;
}

const Figure *Board::getThread( Figure::Color color, const Position &pos, bool check4King ) const
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
			const PotentialDestinations &targets = fig->getPossible();
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

size_t Board::getShields( Figure::Color color, const Position &pos, FigurePtr *shields ) const
{
	size_t numShields = 0;
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig->m_color != color )
		{
			const PotentialDestinations &targets = fig->getPossible();

			for( size_t j=0; j<targets.numTargets; ++j )
			{
				if( targets.targets[j].getTarget() == pos )
				{
					shields[numShields++] = fig;
				}
			}
		}
	}

	return numShields;
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
	const PotentialDestinations &targets = fig->getPossible();
	for( size_t i=0; i<targets.numTargets; ++i )
	{
		if( targets.targets[i].getTarget() == dest )
		{
			if( fig->getType() != Figure::ftKing )
			{
				// final we have to check whether our king is now checked
				return getCurKing()->isAttacked(src.pos,dest);
			}
			return false;
		}
	}
	return true;
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

	flipWatch();
	refresh();
	move.state = m_state;
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

	flipWatch();
	refresh();
	move.state = m_state;
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
	move.promotionType = newFig;
	if( toCapture )
	{
		move.captured = toCapture;
		move.capturePos = toCapture->getPos();
		toCapture->capture();
	}

	flipWatch();
	refresh();
	move.state = m_state;
}

void Board::reset()
{
	clear();
	reset(Figure::White);
	reset(Figure::Black);

	m_whiteK = dynamic_cast<King*>(getFigure('e', 1));
	m_blackK = dynamic_cast<King*>(getFigure('e', NUM_ROWS));

	m_nextColor = Figure::White;
	m_whiteClock.start();
	refresh();
	m_state = csPlaying;
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
		checkMate();
	}
}

void Board::checkMate()
{
	checkCheck();
	if( m_state == csWhiteCheck )
	{
		if( m_nextColor == Figure::Black || !canMove() )
		{
			m_state = csWhiteCheckMate;
		}
	}
	else if( m_state == csBlackCheck )
	{
		if( m_nextColor == Figure::White || !canMove() )
		{
			m_state = csBlackCheckMate;
		}
	}
	else if( !canMove() )
	{
		m_state = csDraw;
	}
}

void Board::checkCheck()
{
	for( size_t i1=0; i1<NUM_FIELDS; ++i1 )
	{
		Figure *fig = m_board[i1];
		if( fig && fig->getType() != Figure::ftKing )
		{
			const PotentialDestinations &targets = fig->getPossible();
			for( size_t i2=0; i2<targets.numThreads; ++i2 )
			{
				if( fig->m_color == Figure::Black && m_whiteK->getPos() == targets.threads[i2] )
				{
					m_state = csWhiteCheck;
/*@*/				return;
				}
				if( fig->m_color == Figure::White && m_blackK->getPos() == targets.threads[i2] )
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
			const PotentialDestinations &pot = fig->getPossible();
			int captures=0;
			for( size_t j=0; j<pot.numTargets; ++j )
			{
				const Position &pos = pot.targets[j].getCapture();
				if( pos )
				{
					captures += getFigure(pos)->getValue();
				}
			}

			if( fig->m_color == Figure::White )
			{
				whiteTargets += int(pot.numTargets);
				whiteCaptures += captures;
			}
			else
			{
				blackTargets += int(pot.numTargets);
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

	if( m_state == csDraw )
	{
		return 0;
	}
	else if( m_state == csWhiteCheckMate )
	{
		return BLACK_WINS;
	}
	else if( m_state == csBlackCheckMate )
	{
		return WHITE_WINS;
	}
	else if( m_state == csWhiteCheck )
	{
		if( isBlackTurn() )
		{
			return BLACK_WINS;
		}
	}
	else if( m_state == csBlackCheck )
	{
		if( isWhiteTurn() )
		{
			return WHITE_WINS;
		}
	}


	evaluateForce( whiteForce, blackForce);

	evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);

	int evaluation = whiteForce + whiteTargets + whiteCaptures - blackForce - blackTargets - blackCaptures;
	doLogValueEx(logLegel, evaluation);
	return evaluation;
}

Position Board::checkBoard() const
{
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		Position pos = getPosition(i);
		if( m_board[i] && m_board[i]->getPos() != pos )
		{
			return pos;
		}
	}

	return Position(0,0);
}

STRING Board::getStateString() const
{
	STRING	result;
	if( m_state == csBlank )
	{
		result = CHESS_DRAW;
	}
	else if( m_state == csDraw )
	{
		result = CHESS_DRAW;
	}
	else if( m_state == csWhiteCheckMate )
	{
		result = STRING(CHESS_BLACK " " CHESS_WINS " " CHESS_CHECK_MATE);
	}
	else if( m_state == csBlackCheckMate )
	{
		result = STRING(CHESS_WHITE " " CHESS_WINS " " CHESS_CHECK_MATE);
	}
	else if( m_state == csWhiteCheck || m_state == csBlackCheck )
	{
		result = CHESS_CHECK;
	}
	else
	{
		result = STRING(CHESS_CHECK_NEXT ": ") + (isWhiteTurn() ? CHESS_WHITE : CHESS_BLACK);
	}
	result.setCharSet( STR_OEM );

	return result;
}

void Board::print() const
{
	std::cout << "+-+-+-+-+-+-+-+-+-+\n";
	std::cout << "| ";
	for( char c=MIN_COL_LETTER;c<=MAX_COL_LETTER; ++c )
	{
		std::cout << '|' << c;
	}
	std::cout << '|' << std::endl;
	std::cout << "+-+-+-+-+-+-+-+-+-+\n";

	bool whiteField = true;
	for( char row=NUM_ROWS; row >= 1; --row )
	{
		std::cout << '|' << int(row) << '|';
		for( char col=MIN_COL_LETTER; col <= MAX_COL_LETTER; ++col )
		{
			const Figure *fig = getFigure(col, row );
			if( !fig )
			{
				if( whiteField )
					std::cout << " ";
				else
					std::cout << "X";
			}
			else
			{
				char sym = fig->getLetter();
				if( fig->m_color == Figure::Black )
					sym = char(tolower(sym));
				std::cout << sym;
			}
			std::cout << '|';
			whiteField = !whiteField;
		}
		whiteField = !whiteField;
		std::cout << std::endl;
		std::cout << "+-+-+-+-+-+-+-+-+-+\n";
	}

	std::cout << getStateString() << std::endl;
	std::cout << CHESS_EVAL ": " << evaluate() << std::endl;
}

STRING Board::generateString() const
{
	STRING result;
	for( int i=0; i<NUM_FIELDS; ++i )
	{
		char sym = ' ';
		const Figure *fig = m_board[i];
		if( fig )
		{
			sym = fig->getLetter();
			if( fig->m_color == Figure::Black )
			{
				sym = char(tolower(sym));
			}
		}

		result += sym;
	}

	return result;
}

void Board::generateFromString(const STRING &string )
{
	if(string.size() < NUM_FIELDS )
	{
		return;
	}
	clear();
	const char *src = string;
	Figure::Color	newColor;
	Figure::Type	newType;
	for( int i=0; i<NUM_FIELDS; ++i )
	{
		char sym = *src++;
		char upperLetter = char(toupper(sym));
		newColor = upperLetter == sym ? Figure::White : Figure::Black;
		switch(upperLetter)
		{
		case PAWN_LETTER:
			newType = Figure::ftPawn;
			break;
		case ROOK_LETTER:
			newType = Figure::ftRook;
			break;
		case KNIGHT_LETTER:
			newType = Figure::ftKnight;
			break;
		case BISHOP_LETTER:
			newType = Figure::ftBishop;
			break;
		case QUEEN_LETTER:
			newType = Figure::ftQueen;
			break;
		case KING_LETTER:
			newType = Figure::ftKing;
			break;
		default:
			newType = Figure::ftNone;
			break;
		}
		if(newType != Figure::ftNone)
		{
			Position pos = getPosition(i);
			Figure *fig = create(newColor,newType, pos);
			m_board[i] = fig;
			if(newType==Figure::ftKing)
			{
				if(isWhiteTurn(newColor))
					m_whiteK = static_cast<King*>(fig);
				else
					m_blackK = static_cast<King*>(fig);;
			}
		}
	}
	if(*src == 'W')
	{
		m_nextColor =  Figure::White;
		m_whiteClock.start();
	}
	else
	{
		m_nextColor =  Figure::Black;
		m_blackClock.start();
	}
	m_state = csPlaying;
	refresh();
	return;
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

