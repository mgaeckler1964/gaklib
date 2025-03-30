/*
		Project:		gaklib
		Module:			chess.cpp
		Description:	The chess figures and board
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

#include <memory>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
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

size_t Figure::checkRange(TargetPositions *pos, Position::MoveFunc movement, size_t maxCount, bool allowSacrifice) const
{
	const size_t	begin = pos->numTargets;

	Position targetPos = m_pos;
	while( 1 )
	{
		targetPos = (targetPos.*movement)();
		if( !targetPos )
			break;

		if( !allowSacrifice && m_board.getThread(m_color,targetPos,false) )
		{
			break;		// king must not move over an attacked field
		}
		const Figure *figure = m_board.getFigure( targetPos );
		if( !figure ) 
		{
			pos->targets[pos->numTargets++] = targetPos;
			pos->threads[pos->numThreads++] = targetPos;
		}
		else 
		{
			pos->threads[pos->numThreads++] = targetPos;
			if( figure->m_color != m_color )
			{
				pos->targets[pos->numTargets++] = targetPos;
				pos->captures[pos->numCaptures++] = targetPos;
			}
			break;
		}
		if( !--maxCount )
		{
			break;
		}
	}

	return pos->numTargets - begin;
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

TargetPositions Pawn::calcPossible()
{
	// init
	TargetPositions	result;

	int direction = ( m_color == Figure::White ) ? 1 : -1;
	char startRow = ( m_color == Figure::White ) ? 2 : 7;

	// one step no beat
	Position targetPos = getPos().move( 0, direction );
	if( targetPos && !m_board.getFigure( targetPos ) )
	{
		result.targets[result.numTargets++] = targetPos;
		// two step no beat
		if( getPos().row == startRow )
		{
			targetPos = targetPos.move( 0, direction );
			if( !m_board.getFigure( targetPos ) )
			{
				result.targets[result.numTargets++] = targetPos;
			}
		}
	}

	// beat step right
	targetPos = getPos().move( 1, direction );
	if( targetPos )
	{
		result.threads[result.numThreads++] = targetPos;

		const Figure *figure = m_board.getFigure( targetPos );
		if( figure && figure->m_color != m_color )
		{
			result.targets[result.numTargets++] = targetPos;
			result.captures[result.numCaptures++] = targetPos;
		}
	}

	// beat step left
	targetPos = getPos().move( -1, direction );
	if( targetPos )
	{
		result.threads[result.numThreads++] = targetPos;

		const Figure *figure = m_board.getFigure( targetPos );
		if( figure && figure->m_color != m_color )
		{
			result.targets[result.numTargets++] = targetPos;
			result.captures[result.numCaptures++] = targetPos;
		}
	}

	// en-passant
	const Movements	&moves = m_board.getMoves();
	if( size_t size = moves.size() )
	{
		const Movement &lastMove = moves[size-1];

		Position leftPos = getPos().move( -1, 0 );
		if( leftPos && lastMove.dest == leftPos )
		{
			Position leftStart = getPos().move( -1, 2*direction );
			if( lastMove.src == leftStart )
			{
				Position leftTarget = getPos().move( -1, direction );
				result.targets[result.numTargets++] = leftTarget;
				result.captures[result.numCaptures++] = leftTarget;
			}
		}

		Position rightPos = getPos().move( 1, 0 );
		if( rightPos && lastMove.dest == rightPos )
		{
			Position rightStart = getPos().move( 1, 2*direction );
			if( lastMove.src == rightStart )
			{
				Position rightTarget = getPos().move( 1, direction );
				result.targets[result.numTargets++] = rightTarget;
				result.captures[result.numCaptures++] = rightTarget;
			}
		}
	}

	return result;
}

TargetPositions  Rook::calcPossible()
{
	TargetPositions	result;

	checkRange(&result, &Position::moveNorth);
	checkRange(&result, &Position::moveEast);
	checkRange(&result, &Position::moveSouth);
	checkRange(&result, &Position::moveWest);

	return result;
}

TargetPositions  Knight::calcPossible()
{
	TargetPositions	result;

	checkRange(&result, &Position::moveSNorthEast);
	checkRange(&result, &Position::moveSEastNorth);
	checkRange(&result, &Position::moveSEastSouth);
	checkRange(&result, &Position::moveSSouthEast);
	checkRange(&result, &Position::moveSSouthWest);
	checkRange(&result, &Position::moveSWestSouth);
	checkRange(&result, &Position::moveSWestNorth);
	checkRange(&result, &Position::moveSNorthWest);

	return result;
}

TargetPositions  Bishop::calcPossible()
{
	TargetPositions	result;

	checkRange(&result, &Position::moveNorthEast);
	checkRange(&result, &Position::moveSouthEast);
	checkRange(&result, &Position::moveSouthWest);
	checkRange(&result, &Position::moveNorthWest);

	return result;
}

TargetPositions Queen::calcPossible()
{
	TargetPositions	result;

	checkRange(&result, &Position::moveNorth);
	checkRange(&result, &Position::moveNorthEast);
	checkRange(&result, &Position::moveEast);
	checkRange(&result, &Position::moveSouthEast);
	checkRange(&result, &Position::moveSouth);
	checkRange(&result, &Position::moveSouthWest);
	checkRange(&result, &Position::moveWest);
	checkRange(&result, &Position::moveNorthWest);

	return result;
}

TargetPositions King::calcPossible()
{
	TargetPositions	result;
	Attack attack = searchAttack(&Position::moveNorth);
	if( isOK( attack ) )
	{
		checkRange(&result, &Position::moveNorth, 1);
	}

	attack = searchAttack(&Position::moveNorthEast);
	if( isOK( attack ) )
	{
		checkRange(&result, &Position::moveNorthEast, 1);
	}

	attack = searchAttack(&Position::moveEast);
	size_t canEast = isOK( attack ) ? checkRange(&result, &Position::moveEast, 1) : 0;

	attack = searchAttack(&Position::moveSouthEast);
	if( isOK( attack ) )
	{
		checkRange(&result, &Position::moveSouthEast, 1);
	}

	attack = searchAttack(&Position::moveSouth);
	if( isOK( attack ) )
	{
		checkRange(&result, &Position::moveSouth, 1);
	}

	attack = searchAttack(&Position::moveSouthWest);
	if( isOK( attack ) )
	{
		checkRange(&result, &Position::moveSouthWest, 1);
	}

	attack = searchAttack(&Position::moveWest);
	size_t canWest = isOK( attack ) ? checkRange(&result, &Position::moveWest, 1) : 0;

	attack = searchAttack(&Position::moveNorthWest);
	if( isOK( attack ) )
	{
		checkRange(&result, &Position::moveNorthWest, 1);
	}

	if( !hasMoved() )
	{
		if( canWest )
		{
			TargetPositions	result2;
			checkRange(&result2, &Position::moveWest, 2);
			if( result2.numTargets == 2 && result2.numCaptures == 0 )
			{
				Figure *rook = m_board.getFigure( Position( 'A', getPos().row ) );
				if( !rook->hasMoved() )
				{
					TargetPositions	result3;
					rook->checkRange(&result3, &Position::moveEast);
					if( result3.numTargets >= 3 && result3.numCaptures == 0 )
					{
						Position &targetPosition = result2.targets[1];
						result.targets[result.numTargets++] = targetPosition;
						m_rochadeWest.myTarget = targetPosition;
						m_rochadeWest.rookTarget = result3.targets[2];
						m_rochadeWest.rook = dynamic_cast<Rook*>(rook);
					}
				}
			}
		}
		if( canEast )
		{
			TargetPositions	result2;
			checkRange(&result2, &Position::moveEast, 2);
			if( result2.numTargets == 2 && result2.numCaptures == 0 )
			{
				Figure *rook = m_board.getFigure( Position( 'H', getPos().row ) );
				if( !rook->hasMoved() )
				{
					TargetPositions	result3;
					rook->checkRange(&result3, &Position::moveWest);
					if( result3.numTargets >= 2 && result3.numCaptures == 0 )
					{
						Position &targetPosition = result2.targets[1];
						result.targets[result.numTargets++] = targetPosition;
						m_rochadeEast.myTarget = targetPosition;
						m_rochadeEast.rookTarget = result3.targets[1];
						m_rochadeEast.rook = dynamic_cast<Rook*>(rook);
					}
				}
			}
		}
	}


	return result;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void Figure::moveTo( const Position &pos )
{
	setPosition(pos);
	m_board.refresh();
}

Figure::Attack Figure::searchAttack(const Position &pos, Position::MoveFunc movement, const Position &ignore, const Position &stop, int maxCount ) const
{
	Attack attack;
	Position targetPos = pos;
	while( maxCount-- )
	{
		targetPos = (targetPos.*movement)();
		if( !targetPos || targetPos == stop)
			break;
		++attack.steps;
		if(ignore != targetPos)
		{
			const Figure *figure = m_board.getFigure( targetPos );
			if( figure ) 
			{
				if( figure->m_color != m_color )
				{
					attack.figure = figure;
				}
				break;
			}
		}
	}

	return attack;
}

Figure::Attack Figure::searchAttack(const Position &ignore, const Position &stop ) const
{
	static struct 
	{
		Position::MoveFunc	move;
		int				maxDistance;
	} moves[] =
	{
		{ &Position::moveNorth,			MAX_DISTANCE },
		{ &Position::moveNorthEast,		MAX_DISTANCE },
		{ &Position::moveEast,			MAX_DISTANCE },
		{ &Position::moveSouthEast,		MAX_DISTANCE },
		{ &Position::moveSouth,			MAX_DISTANCE },
		{ &Position::moveSouthWest,		MAX_DISTANCE },
		{ &Position::moveWest,			MAX_DISTANCE },
		{ &Position::moveNorthWest,		MAX_DISTANCE },
		{ &Position::moveSNorthEast,	1 },
		{ &Position::moveSEastNorth,	1 },
		{ &Position::moveSEastSouth,	1 },
		{ &Position::moveSSouthEast,	1 },
		{ &Position::moveSSouthWest,	1 },
		{ &Position::moveSWestSouth,	1 },
		{ &Position::moveSWestNorth,	1 },
		{ &Position::moveSNorthWest,	1 }
	};
	FOR_EACH(i,moves)
	{
		Attack attack = searchAttack(m_pos, moves[i].move, ignore, stop, moves[i].maxDistance );
		if( !isOK(attack) )
		{
			return attack;
		}
	}

	return Attack();
}

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

const Figure *Board::getThread( Figure::Color color, const Position &pos, bool checkEnPassant ) const
{
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		const Figure *fig = m_board[i];
		if( fig && fig->m_color != color )
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
	for( size_t i=0; i<NUM_FIELDS; ++i )
	{
		if( m_board[i] )
		{
			m_board[i]->refresh();
		}
	}
	checkCheck();
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
	int whiteForce, blackForce;
	int whiteTargets, blackTargets, whiteCaptures, blackCaptures;

	evaluateForce( whiteForce, blackForce);
	if( whiteForce < KING_VALUE )
	{
		return -PLAYER_WINS;
	}
	if( blackForce < KING_VALUE )
	{
		return PLAYER_WINS;
	}

	if( m_state == csWhiteCheck )
	{
		if( isBlackTurn() )
		{
			return -PLAYER_WINS;
		}
		return -CHECK;
	}
	else if( m_state == csBlackCheck )
	{
		if( isWhiteTurn() )
		{
			return PLAYER_WINS;
		}
		return CHECK;
	}

	evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);

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
	for( int i=0; i<escapes.numTargets; ++i )
	{
		Movement &defend = defends.createElement();
		defend.fig = king;
		defend.src = king->getPos();
		defend.dest = escapes.targets[i];
	}
	return defends;
}

Movement Board::findBest() const
{
	Movement	best;
	size_t		numAttackers;
	Movements	movements = findCheckDefend(&numAttackers);

	if( !numAttackers )
	{
		movements = collectMoves();
	}

	if( movements.size() )
	{
		size_t index = randomNumber(movements.size());
		best = movements[index];
	}
	return best;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace chess
}	//namespace gak

#ifdef __BORLANDC__
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

