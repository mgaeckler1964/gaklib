/*
		Project:		GAKLIB
		Module:			figure.cpp
		Description:	chess figures
		Author:			Martin G�ckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Linz, Austria ``AS IS''
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
char Figure::getLetter( Type type )
{
	switch(type)
	{
	case ftPawn:	return PAWN_LETTER;
	case ftRook:	return ROOK_LETTER;
	case ftKnight:	return KNIGHT_LETTER;
	case ftBishop:	return BISHOP_LETTER;
	case ftQueen:	return QUEEN_LETTER;
	case ftKing:	return KING_LETTER;
	}

	return 0;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

size_t Figure::checkRange(PotentialDestinations *result, Position::MoveFunc movement, size_t maxCount, bool allowSacrifice) const
{
	if( m_toKing && m_fromKing && movement != m_toKing && movement != m_fromKing)
	{
		return 0;
	}
	const size_t	begin = result->numTargets;

	Position targetPos = m_pos;
	while( 1 )
	{
		targetPos = (targetPos.*movement)();
		if( !targetPos )
			break;

		if( !allowSacrifice && m_board.getThread(m_color,targetPos,true) )
		{
			break;		// king must not move over an attacked field
		}
		const Figure *figure = m_board.getFigure( targetPos );
		if( !figure ) 
		{
			result->targets[result->numTargets++] = Destination(targetPos);
			result->threads[result->numThreads++] = targetPos;
		}
		else 
		{
			result->threads[result->numThreads++] = targetPos;
			if( figure->m_color != m_color && figure->getType() != ftKing )		// it is not allowed to capture the king
			{
				result->targets[result->numTargets++] = Destination(targetPos, targetPos );
				result->hasCaptures = true;
			}
			break;
		}
		if( !--maxCount )
		{
			break;
		}
	}

	return result->numTargets - begin;
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
PotentialDestinations Pawn::calcPossible()
{
	// init
	PotentialDestinations	result;

	int direction = ( m_color == Figure::White ) ? 1 : -1;
	char startRow = ( m_color == Figure::White ) ? 2 : 7;

	// one step no beat
	Position targetPos = getPos().move( 0, direction );
	if( targetPos && !m_board.getFigure( targetPos ) )
	{
		result.targets[result.numTargets++] = Destination(targetPos);
		// two step no beat
		if( getPos().row == startRow )
		{
			targetPos = targetPos.move( 0, direction );
			if( !m_board.getFigure( targetPos ) )
			{
				result.targets[result.numTargets++] = Destination(targetPos);
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
			result.targets[result.numTargets++] = Destination(targetPos, targetPos);
			result.hasCaptures = true;
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
			result.targets[result.numTargets++] = Destination(targetPos, targetPos);
			result.hasCaptures = true;
		}
	}

	// en-passant
	if( shouldCheckPassant() )
	{
		const Movements	&moves = m_board.getHistory();
		if( size_t size = moves.size() )
		{
			const Movement &lastMove = moves[size-1];
			if( lastMove.fig->getType() == Figure::ftPawn )
			{
				Position leftPos = getPos().move( -1, 0 );
				if( leftPos && lastMove.dest == leftPos )
				{
					Position leftStart = getPos().move( -1, 2*direction );
					if( lastMove.src == leftStart )
					{
						Position leftTarget = getPos().move( -1, direction );
						result.targets[result.numTargets++] = Destination(leftTarget,leftPos);
						result.hasCaptures = true;
					}
				}

				Position rightPos = getPos().move( 1, 0 );
				if( rightPos && lastMove.dest == rightPos )
				{
					Position rightStart = getPos().move( 1, 2*direction );
					if( lastMove.src == rightStart )
					{
						Position rightTarget = getPos().move( 1, direction );
						result.targets[result.numTargets++] = Destination(rightTarget,rightPos);
						result.hasCaptures = true;
					}
				}
			}
		}
	}

	return result;
}

PotentialDestinations  Rook::calcPossible()
{
	PotentialDestinations	result;

	checkRange(&result, &Position::moveNorth);
	checkRange(&result, &Position::moveEast);
	checkRange(&result, &Position::moveSouth);
	checkRange(&result, &Position::moveWest);

	return result;
}

PotentialDestinations  Knight::calcPossible()
{
	PotentialDestinations	result;

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

PotentialDestinations  Bishop::calcPossible()
{
	PotentialDestinations	result;

	checkRange(&result, &Position::moveNorthEast);
	checkRange(&result, &Position::moveSouthEast);
	checkRange(&result, &Position::moveSouthWest);
	checkRange(&result, &Position::moveNorthWest);

	return result;
}

PotentialDestinations Queen::calcPossible()
{
	PotentialDestinations	result;

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

PotentialDestinations King::calcPossible()
{
	PotentialDestinations	result;
	size_t canEast = 0;
	size_t canWest = 0;

	{
		Attack moveNorthAttack = searchAttack(&Position::moveNorth);
		Attack moveSouthAttack = searchAttack(&Position::moveSouth);
		if( isOK( moveNorthAttack, 2 ) && isOK( moveSouthAttack, 0 ) )
		{
			checkRange(&result, &Position::moveNorth, 1);
		}
		if( isOK( moveNorthAttack, 0 ) && isOK( moveSouthAttack, 2 ) )
		{
			checkRange(&result, &Position::moveSouth, 1);
		}
	}
	{
		Attack moveEastAttack = searchAttack(&Position::moveEast);
		Attack moveWestAttack = searchAttack(&Position::moveWest);
		if( isOK( moveEastAttack, 2 ) && isOK( moveWestAttack, 0 ) )
		{
			canEast = checkRange(&result, &Position::moveEast, 1);
		}
		if( isOK( moveEastAttack, 0 ) && isOK( moveWestAttack, 2 ) )
		{
			canWest = checkRange(&result, &Position::moveWest, 1);
		}
	}
	{
		Attack moveNorthEastAtack = searchAttack(&Position::moveNorthEast);
		Attack moveSouthWestAtack = searchAttack(&Position::moveSouthWest);
		if( isOK( moveNorthEastAtack, 2 ) && isOK( moveSouthWestAtack, 0 ))
		{
			checkRange(&result, &Position::moveNorthEast, 1);
		}
		if( isOK( moveNorthEastAtack, 0 ) && isOK( moveSouthWestAtack, 2 ))
		{
			checkRange(&result, &Position::moveSouthWest, 1);
		}
	}
	{
		Attack moveNorthWestAtack = searchAttack(&Position::moveNorthWest);
		Attack moveSouthEastAtack = searchAttack(&Position::moveSouthEast);
		if( isOK( moveNorthWestAtack, 2 ) && isOK( moveSouthEastAtack, 0 ))
		{
			checkRange(&result, &Position::moveNorthWest, 1);
		}
		if( isOK( moveNorthWestAtack, 0 ) && isOK( moveSouthEastAtack, 2 ))
		{
			checkRange(&result, &Position::moveSouthEast, 1);
		}
	}

	if( !hasMoved() )
	{
		if( canWest )
		{
			PotentialDestinations	result2;
			checkRange(&result2, &Position::moveWest, 2);
			if( result2.numTargets == 2 && !result2.hasCaptures )
			{
				Figure *rook = m_board.getFigure( Position( MIN_COL_LETTER, getPos().row ) );
				if( rook && !rook->hasMoved() )
				{
					PotentialDestinations	result3;
					rook->checkRange(&result3, &Position::moveEast);
					if( result3.numTargets >= 3 && !result3.hasCaptures )
					{
						Destination &targetPosition = result2.targets[1];
						result.targets[result.numTargets++] = targetPosition;
						m_rochadeWest.myTarget = targetPosition.getTarget();
						m_rochadeWest.rookTarget = result3.targets[2].getTarget();
						m_rochadeWest.rook = dynamic_cast<Rook*>(rook);
					}
				}
			}
		}
		if( canEast )
		{
			PotentialDestinations	result2;
			checkRange(&result2, &Position::moveEast, 2);
			if( result2.numTargets == 2 && !result2.hasCaptures )
			{
				Figure *rook = m_board.getFigure( Position( MAX_COL_LETTER, getPos().row ) );
				if( rook && !rook->hasMoved() )
				{
					PotentialDestinations	result3;
					rook->checkRange(&result3, &Position::moveWest);
					if( result3.numTargets >= 2 && !result3.hasCaptures )
					{
						Destination &targetPosition = result2.targets[1];
						result.targets[result.numTargets++] = targetPosition;
						m_rochadeEast.myTarget = targetPosition.getTarget();
						m_rochadeEast.rookTarget = result3.targets[1].getTarget();
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

bool Figure::isOK( const Attack &attack, unsigned maxSteps ) const
{
	return !attack.figure						// no attacker
		|| attack.figure->m_color == m_color	// no enemy
		|| (
			attack.steps < maxSteps &&			// king can self defend
			!m_board.getThread(m_color, attack.figure->getPos(), false )
		)
		|| !attack.figure->isThread(m_pos) ;	// too weak?
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
				if( figure->m_color != m_color && figure->isThread(pos) )
				{
					attack.figure = figure;
				}
				break;
			}
		}
	}

	return attack;
}

King *Figure::getKing() const
{
	return m_board.getKing(m_color);
}

void Figure::checkInterPos()
{
	King * king = getKing();
	m_toKing = Position::findMoveFunc( m_pos, king->getPos() );
	m_fromKing = Position::findMoveFunc( king->getPos(), m_pos );
	if( m_fromKing )
	{
		Attack attack = searchAttack(m_fromKing);
		if( isOK( attack, 0 ) )
		{
			m_toKing = NULL;
			m_fromKing = NULL;
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

