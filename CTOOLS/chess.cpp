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

void Board::clear()
{
	for( std::size_t i=0; i<64; ++i)
	{
		if( m_board[i] )
		{
			delete m_board[i];
		}
	}
}

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
	Figure *enPassantDel = checkEnPassant(src,dest);
		
	size_t srcIndex = src.index;
	size_t destIndex = getIndex( dest );
	Figure *fig = src.fig;

	if( m_board[destIndex] )
	{
		delete m_board[destIndex];
	}
	m_board[destIndex] = fig;
	fig->moveTo(dest);
	m_board[srcIndex] = NULL;

	if( enPassantDel )
	{
		destIndex = getIndex( enPassantDel->getPos() );
		m_board[destIndex] = NULL;
		delete enPassantDel;
	}

	return fig;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

size_t Figure::checkDirection(TargetPositions *pos, Position (Position::*movement )(), size_t maxCount, bool allowSacrifice) const
{
	const size_t	begin = pos->numTargets;

	Position targetPos = m_pos;
	while( 1 )
	{
		targetPos = (targetPos.*movement)();
		if( !targetPos )
			break;

		if( !allowSacrifice && m_board.getAttacker(m_color,targetPos) )
		{
			break;		// king must not move over an attacked field
		}
		const Figure *figure = m_board.getFigure( targetPos );
		if( !figure ) 
		{
			pos->targets[pos->numTargets++] = targetPos;
		}
		else 
		{
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
		const Figure *figure = m_board.getFigure( targetPos );
		if( figure && figure->m_color != m_color )
		{
			result.targets[result.numTargets++] = targetPos;
			result.captures[result.numCaptures++] = targetPos;
		}
	}

	// en-passant
	const Array<Movement>	&moves = m_board.getMoves();
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

	checkDirection(&result, &Position::moveNorth);
	checkDirection(&result, &Position::moveEast);
	checkDirection(&result, &Position::moveSouth);
	checkDirection(&result, &Position::moveWest);

	return result;
}

TargetPositions  Knight::calcPossible()
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveSNorthEast);
	checkDirection(&result, &Position::moveSEastNorth);
	checkDirection(&result, &Position::moveSEastSouth);
	checkDirection(&result, &Position::moveSSouthEast);
	checkDirection(&result, &Position::moveSSouthWest);
	checkDirection(&result, &Position::moveSWestSouth);
	checkDirection(&result, &Position::moveSWestNorth);
	checkDirection(&result, &Position::moveSNorthWest);

	return result;
}

TargetPositions  Bishop::calcPossible()
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveNorthEast);
	checkDirection(&result, &Position::moveSouthEast);
	checkDirection(&result, &Position::moveSouthWest);
	checkDirection(&result, &Position::moveNorthWest);

	return result;
}

TargetPositions Queen::calcPossible()
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveNorth);
	checkDirection(&result, &Position::moveNorthEast);
	checkDirection(&result, &Position::moveEast);
	checkDirection(&result, &Position::moveSouthEast);
	checkDirection(&result, &Position::moveSouth);
	checkDirection(&result, &Position::moveSouthWest);
	checkDirection(&result, &Position::moveWest);
	checkDirection(&result, &Position::moveNorthWest);

	return result;
}

TargetPositions King::calcPossible()
{
	TargetPositions	result;
	Attack attack = searchAttack(&Position::moveNorth);
	if( isOK( attack ) )
	{
		checkDirection(&result, &Position::moveNorth, 1);
	}

	attack = searchAttack(&Position::moveNorthEast);
	if( isOK( attack ) )
	{
		checkDirection(&result, &Position::moveNorthEast, 1);
	}

	attack = searchAttack(&Position::moveEast);
	size_t canEast = isOK( attack ) ? checkDirection(&result, &Position::moveEast, 1) : 0;

	attack = searchAttack(&Position::moveSouthEast);
	if( isOK( attack ) )
	{
		checkDirection(&result, &Position::moveSouthEast, 1);
	}

	attack = searchAttack(&Position::moveSouth);
	if( isOK( attack ) )
	{
		checkDirection(&result, &Position::moveSouth, 1);
	}

	attack = searchAttack(&Position::moveSouthWest);
	if( isOK( attack ) )
	{
		checkDirection(&result, &Position::moveSouthWest, 1);
	}

	attack = searchAttack(&Position::moveWest);
	size_t canWest = isOK( attack ) ? checkDirection(&result, &Position::moveWest, 1) : 0;

	attack = searchAttack(&Position::moveNorthWest);
	if( isOK( attack ) )
	{
		checkDirection(&result, &Position::moveNorthWest, 1);
	}

	if( !getMoved() )
	{
		if( canWest )
		{
			TargetPositions	result2;
			checkDirection(&result2, &Position::moveWest, 2);
			if( result2.numTargets == 2 && result2.numCaptures == 0 )
			{
				Figure *rook = m_board.getFigure( Position( 'A', getPos().row ) );
				if( !rook->getMoved() )
				{
					TargetPositions	result3;
					rook->checkDirection(&result3, &Position::moveEast);
					if( result3.numTargets >= 3 && result3.numCaptures == 0 )
					{
						Position &targetPosition = result2.targets[1];
						result.targets[result.numTargets++] = targetPosition;
						m_rochadeWest.myTarget = targetPosition;
						m_rochadeWest.rookTarget = result3.targets[2];
#ifdef __BORLANDC__
						m_rochadeWest.rook = static_cast<Rook*>(rook);
#else
						m_rochadeWest.rook = dynamic_cast<Rook*>(rook);
#endif
					}
				}
			}
		}
		if( canEast )
		{
			TargetPositions	result2;
			checkDirection(&result2, &Position::moveEast, 2);
			if( result2.numTargets == 2 && result2.numCaptures == 0 )
			{
				Figure *rook = m_board.getFigure( Position( 'H', getPos().row ) );
				if( !rook->getMoved() )
				{
					TargetPositions	result3;
					rook->checkDirection(&result3, &Position::moveWest);
					if( result3.numTargets >= 2 && result3.numCaptures == 0 )
					{
						Position &targetPosition = result2.targets[1];
						result.targets[result.numTargets++] = targetPosition;
						m_rochadeEast.myTarget = targetPosition;
						m_rochadeEast.rookTarget = result3.targets[1];
#ifdef __BORLANDC__
						m_rochadeEast.rook = static_cast<Rook*>(rook);
#else
						m_rochadeEast.rook = dynamic_cast<Rook*>(rook);
#endif
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
	m_moved = true;
	m_pos = pos;

	m_board.refresh();
}

Figure::Attack Figure::searchAttack(const Position &pos, Position (Position::*movement )()) const
{
	Attack attack;
	Position targetPos = pos;
	while( 1 )
	{
		targetPos = (targetPos.*movement)();
		if( !targetPos )
			break;
		++attack.steps;
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

	return attack;
}

const Figure *Board::getAttacker( Figure::Color color, const Position &pos ) const
{
	for( size_t i=0; i<64; ++i )
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

size_t Board::getAttackers( Figure::Color color, const Position &pos, const Figure **attackers ) const
{
	size_t numAttackers = 0;
	for( size_t i=0; i<64; ++i )
	{
		const Figure *fig = m_board[i];
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

bool Board::checkMoveTo( const PlayerPos &src, const Position &dest, Figure::Type newFig ) const
{
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
		|| (dest.row != 1 && dest.row != 8)	)
		{
			return true;
		}
	}
	else if(fig->getType() == Figure::ftPawn && (dest.row == 1 || dest.row == 8))
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
	return true;
}

void Board::moveTo( const PlayerPos &src, const Position &dest )
{
	assert(!checkMoveTo(src, dest) );

	uncheckedMove(src, dest);

	Movement &move = m_moves.createElement();
	move.src = src.pos;
	move.dest = dest;

	m_nextColor = m_nextColor == Figure::White ? Figure::Black : Figure::White;
	refresh();
}

void Board::rochade( const PlayerPos &king, const PlayerPos &rook, const Position &kingDest, const Position &rookDest )
{
	assert(!checkMoveTo(king, kingDest) );
	assert(!checkMoveTo(rook, rookDest) );

	uncheckedMove(king, kingDest );
	uncheckedMove(rook, rookDest );

	Movement &move = m_moves.createElement();
	move.src = king.pos;
	move.dest = kingDest;
	move.isRochade = true;

	m_nextColor = m_nextColor == Figure::White ? Figure::Black : Figure::White;
	refresh();
}

void Board::promote( const PlayerPos &pawn, Figure::Type newFig, const Position &dest )
{
	assert(!checkMoveTo(pawn, dest, newFig) );

	size_t srcIndex = pawn.index;
	size_t destIndex = getIndex( dest );

	if( m_board[destIndex] )
	{
		delete m_board[destIndex];
	}
	Figure *newFigure;
	switch(newFig)
	{
	case Figure::ftBishop:
		newFigure = new Bishop(pawn.fig->m_color, dest, *this);
		break;
	case Figure::ftKnight:
		newFigure = new Knight(pawn.fig->m_color, dest, *this);
		break;
	case Figure::ftRook:
		newFigure = new Rook(pawn.fig->m_color, dest, *this);
		break;
	case Figure::ftQueen:
		newFigure = new Queen(pawn.fig->m_color, dest, *this);
		break;
	}
	m_board[destIndex] = newFigure;

	m_board[srcIndex] = NULL;
	delete pawn.fig;

	Movement &move = m_moves.createElement();
	move.src = pawn.pos;
	move.dest = dest;
	move.promotion = newFig;

	m_nextColor = m_nextColor == Figure::White ? Figure::Black : Figure::White;
	refresh();
}

void Board::reset()
{
	empty();
	for( char col='A'; col <='H'; ++col )
	{
		char row=2;
		size_t index = getIndex(col, row);
		m_board[index] = new Pawn(Figure::White,Position(col,row),*this);

		row=7;
		index = getIndex(col, row);
		m_board[index] = new Pawn(Figure::Black,Position(col,row),*this);
	}

	char col = 'A';
	char row = 1;
	size_t index = getIndex(col, row);

	m_board[index] = new Rook(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Knight(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Bishop(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Queen(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = m_whiteK = new King(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Bishop(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Knight(Figure::White,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Rook(Figure::White,Position(col,row),*this);

	col = 'A';
	row = 8;
	index = getIndex(col, row);

	m_board[index] = new Rook(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Knight(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Bishop(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Queen(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = m_blackK = new King(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Bishop(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Knight(Figure::Black,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Rook(Figure::Black,Position(col,row),*this);

	refresh();
}


void Board::refresh()
{
	for( size_t i=0; i<64; ++i )
	{
		if( m_board[i] )
		{
			m_board[i]->refresh();
		}
	}
}

void Board::evaluatePower( int &whitePower, int &blackPower) const
{
	Position	board[64];

	whitePower=0;
	blackPower=0;
	for( size_t i=0; i<64; ++i )
	{
		if( m_board[i] )
		{
			int power = m_board[i]->getValue();
			if( m_board[i]->m_color == Figure::White )
			{
				whitePower += power;
			}
			else
			{
				blackPower += power;
			}
		}
	}
}

void Board::evaluateRange(int &whiteTargets, int &blackTargets, int &whiteCaptures, int &blackCaptures) const
{
	whiteTargets = blackTargets = whiteCaptures = blackCaptures = 0;
	for( size_t i=0; i<64; ++i )
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
	int whitePower, blackPower;
	int whiteTargets, blackTargets, whiteCaptures, blackCaptures;

	evaluatePower( whitePower, blackPower);
	if( whitePower < KING_VALUE )
	{
		return -PLAYER_WINS;
	}
	if( blackPower < KING_VALUE )
	{
		return PLAYER_WINS;
	}

	enum Check {
		cNONE, cWHITE, cBLACK
	} check = cNONE;

	for( size_t i=0; i<64; ++i )
	{
		Figure *fig = m_board[i];
		if( fig && fig!=m_whiteK && fig!=m_blackK )
		{
			const TargetPositions &targets = fig->getPossible();
			for( size_t i=0; i<targets.numCaptures; ++i )
			{
				if( m_whiteK->getPos() == targets.captures[i] )
				{
					check = cWHITE;
/*v*/				break;
				}
				if( m_blackK->getPos() == targets.captures[i] )
				{
					check = cBLACK;
/*v*/				break;
				}
			}
			if( check )
			{
/*v*/			break;
			}
		}
	}

	if( check == cWHITE )
	{
		if( m_nextColor == Figure::Black )
		{
			return -PLAYER_WINS;
		}
		return -CHECK;
	}
	else if( check == cBLACK )
	{
		if( m_nextColor == Figure::White )
		{
			return PLAYER_WINS;
		}
		return CHECK;
	}

	evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);

	return whitePower + whiteTargets + whiteCaptures - blackPower - blackTargets - blackCaptures;
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

