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

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

TargetPositions Bauer::getPossible() const
{
	// init
	TargetPositions	result;

	int direction = ( m_color == Figure::Weiss ) ? 1 : -1;
	char startRow = ( m_color == Figure::Weiss ) ? 2 : 7;

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
			result.beats[result.numBeats++] = targetPos;
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
			result.beats[result.numBeats++] = targetPos;
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
				leftTarget.enPassant = leftPos.row;
				result.targets[result.numTargets++] = leftTarget;
				result.beats[result.numBeats++] = leftTarget;
			}
		}

		Position rightPos = getPos().move( 1, 0 );
		if( rightPos && lastMove.dest == rightPos )
		{
			Position rightStart = getPos().move( 1, 2*direction );
			if( lastMove.src == rightStart )
			{
				Position rightTarget = getPos().move( 1, direction );
				rightTarget.enPassant = rightPos.row;
				result.targets[result.numTargets++] = rightTarget;
				result.beats[result.numBeats++] = rightTarget;
			}
		}
	}

	return result;
}

TargetPositions  Turm::getPossible() const
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveNorth);
	checkDirection(&result, &Position::moveEast);
	checkDirection(&result, &Position::moveSouth);
	checkDirection(&result, &Position::moveWest);

	return result;
}

TargetPositions  Springer::getPossible() const
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveSNorthEast,1);
	checkDirection(&result, &Position::moveSEastNorth,1);
	checkDirection(&result, &Position::moveSEastSouth,1);
	checkDirection(&result, &Position::moveSSouthEast,1);
	checkDirection(&result, &Position::moveSSouthWest,1);
	checkDirection(&result, &Position::moveSWestSouth,1);
	checkDirection(&result, &Position::moveSWestNorth,1);
	checkDirection(&result, &Position::moveSNorthWest,1);

	return result;
}

TargetPositions  Laeufer::getPossible() const
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveNorthEast);
	checkDirection(&result, &Position::moveSouthEast);
	checkDirection(&result, &Position::moveSouthWest);
	checkDirection(&result, &Position::moveNorthWest);

	return result;
}

TargetPositions Dame::getPossible() const
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

TargetPositions Koenig::getPossible() const
{
	TargetPositions	result;

	checkDirection(&result, &Position::moveNorth, 1);
	checkDirection(&result, &Position::moveNorthEast, 1);
	size_t canEast = checkDirection(&result, &Position::moveEast, 1);
	checkDirection(&result, &Position::moveSouthEast, 1);
	checkDirection(&result, &Position::moveSouth, 1);
	checkDirection(&result, &Position::moveSouthWest, 1);
	size_t canWest = checkDirection(&result, &Position::moveWest, 1);
	checkDirection(&result, &Position::moveNorthWest, 1);

	if( !getMoved() )
	{
		if( canWest )
		{
			TargetPositions	result2;
			checkDirection(&result2, &Position::moveWest, 2);
			if( result2.numTargets == 2 && result2.numBeats == 0 )
			{
				const Figure *rock = m_board.getFigure( Position( 'A', getPos().row ) );
				if( !rock->getMoved() )
				{
					TargetPositions	result3;
					rock->checkDirection(&result3, &Position::moveEast, 3);
					if( result3.numTargets == 3 && result3.numBeats == 0 )
					{
						Position &targetPosition = result2.targets[1];
						targetPosition.second = rock;
						result.targets[result.numTargets++] = targetPosition;
					}
				}
			}
		}
		if( canEast )
		{
			TargetPositions	result2;
			checkDirection(&result2, &Position::moveEast, 2);
			if( result2.numTargets == 2 && result2.numBeats == 0 )
			{
				const Figure *rock = m_board.getFigure( Position( 'H', getPos().row ) );
				if( !rock->getMoved() )
				{
					TargetPositions	result3;
					rock->checkDirection(&result3, &Position::moveWest, 2);
					if( result3.numTargets == 2 && result3.numBeats == 0 )
					{
						Position &targetPosition = result2.targets[1];
						targetPosition.second = rock;
						result.targets[result.numTargets++] = targetPosition;
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

size_t Figure::checkDirection(TargetPositions *pos, Position (Position::*movement )(), size_t maxCount) const
{
	const size_t	begin = pos->numTargets;

	Position targetPos = m_pos;
	while( 1 )
	{
		targetPos = (targetPos.*movement)();
		if( !targetPos )
			break;

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
				pos->beats[pos->numBeats++] = targetPos;
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

bool Board::checkMoveTo( const Position &src, const Position &dest )
{
	size_t srcIndex = getIndex( src );
	Figure *fig = m_board[srcIndex];
	if( !fig )
	{
		return true;
	}
	TargetPositions targets = fig->getPossible();
	for( size_t i=0; i<targets.numTargets; ++i )
	{
		if( targets.targets[i] == dest )
		{
			return false;
		}
	}
	return true;
}

void Board::moveTo( const Position &src, const Position &dest )
{
	assert(!checkMoveTo(src, dest) );
	size_t srcIndex = getIndex( src );
	size_t destIndex = getIndex( dest );

	if( m_board[destIndex] )
	{
		delete m_board[destIndex];
	}
	m_board[destIndex] = m_board[srcIndex];
	m_board[destIndex]->moveTo(dest);
	m_board[srcIndex] = NULL;

	Movement &move = m_moves.createElement();
	move.src = src;
	move.dest = dest;
}

void Board::reset()
{
	empty();
	for( char col='A'; col <='H'; ++col )
	{
		char row=2;
		size_t index = getIndex(col, row);
		m_board[index] = new Bauer(Figure::Weiss,Position(col,row),*this);

		row=7;
		index = getIndex(col, row);
		m_board[index] = new Bauer(Figure::Schwarz,Position(col,row),*this);
	}

	char col = 'A';
	char row = 1;
	size_t index = getIndex(col, row);

	m_board[index] = new Turm(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Springer(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Laeufer(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Dame(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = weissK = new Koenig(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Laeufer(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Springer(Figure::Weiss,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Turm(Figure::Weiss,Position(col,row),*this);

	col = 'A';
	row = 8;
	index = getIndex(col, row);

	m_board[index] = new Turm(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Springer(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Laeufer(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Dame(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = schwarzK = new Koenig(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Laeufer(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Springer(Figure::Schwarz,Position(col,row),*this);
	++col;
	++index;
	m_board[index] = new Turm(Figure::Schwarz,Position(col,row),*this);
}

void Board::evaluate1( int &whitePower, int &blackPower)
{
	Position	board[64];

	whitePower=0;
	blackPower=0;
	for( size_t i=0; i<64; ++i )
	{
		if( m_board[i] )
		{
			int power = m_board[i]->getValue();
			if( m_board[i]->m_color == Figure::Weiss )
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

void Board::evaluate2(int &whiteTargets, int &blackTargets, int &whiteBeats, int &blackBeats)
{
	whiteTargets = blackTargets = whiteBeats = blackBeats = 0;
	for( size_t i1=0; i1<64; ++i1 )
	{
		if( m_board[i1] )
		{
			TargetPositions pos = m_board[i1]->getPossible();
			int beats=0;
			for( int i2=0; i2<pos.numBeats; ++i2 )
			{
				beats += getFigure(pos.beats[i2])->getValue();
			}

			if( m_board[i1]->m_color == Figure::Weiss )
			{
				whiteTargets += pos.numTargets;
				whiteBeats += beats;
			}
			else
			{
				blackTargets += pos.numTargets;
				blackBeats += beats;
			}
		}
	}
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

