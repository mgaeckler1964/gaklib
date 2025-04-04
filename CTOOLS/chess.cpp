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

Movements Board::collectMoves() const
{
	doEnterFunctionEx(gakLogging::llInfo, "Board::collectMoves");
	static Figure::Type promoTypes[] =
	{
		Figure::ftQueen, Figure::ftRook, Figure::ftKnight, Figure::ftBishop
	};
	Movements moves;
	for( size_t i1=0; i1<NUM_FIELDS; ++i1 )
	{
		Figure *fig = m_board[i1];
		if( fig && fig->m_color == m_nextColor )
		{
			const Position &src = fig->getPos();
			const PotentialDestinations &curMoves = fig->getPossible();
			for( int i2=0; i2<curMoves.numTargets; ++i2 )
			{
				const Position &dest = curMoves.targets[i2].target;
				Figure *cap = m_board[getIndex(dest)];
				if(fig->getType() == Figure::ftPawn && (dest.row == 8 || dest.row == 1) )
				{
					FOR_EACH(i3,promoTypes)
					{
						Movement &move = moves.createElement();
						move.fig = fig;
						move.src = src;
						move.dest = dest;
						move.promotionType = promoTypes[i3];
						move.captured = cap;
						if( cap )
						{
							move.capturePos = cap->getPos();
						}
					}
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
				}
			}
		}
	}
	return moves;
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

/*
	for some reasons
	MSVC++ cannot index this module with this member. Making it more effort to edit this file.
	I have copied all other functions to figure.cpp and board.cpp for that reason
*/
Figure::Attack Figure::searchAttack(const Position &ignore, const Position &stop ) const
{
	static struct 
	{
		Position::MoveFunc	move;
		int					maxDistance;
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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

std::ostream &operator << (std::ostream &stream, Position::MoveFunc func )
{
	char *name="unknown";
	static struct 
	{
		Position::MoveFunc	move;
		char				*name;
	} moves[] =
	{
		{ &Position::moveNorth,			"Position::moveNorth" },
		{ &Position::moveNorthEast,		"Position::moveNorthEast" },
		{ &Position::moveEast,			"Position::moveEast" },
		{ &Position::moveSouthEast,		"Position::moveSouthEast" },
		{ &Position::moveSouth,			"Position::moveSouth" },
		{ &Position::moveSouthWest,		"Position::moveSouthWest" },
		{ &Position::moveWest,			"Position::moveWest" },
		{ &Position::moveNorthWest,		"Position::moveNorthWest" },
		{ &Position::moveSNorthEast,	"Position::moveSNorthEast" },
		{ &Position::moveSEastNorth,	"Position::moveSEastNorth" },
		{ &Position::moveSEastSouth,	"Position::moveSEastSouth" },
		{ &Position::moveSSouthEast,	"Position::moveSSouthEast" },
		{ &Position::moveSSouthWest,	"Position::moveSSouthWest" },
		{ &Position::moveSWestSouth,	"Position::moveSWestSouth" },
		{ &Position::moveSWestNorth,	"Position::moveSWestNorth" },
		{ &Position::moveSNorthWest,	"Position::moveSNorthWest" }
	};
	FOR_EACH(i,moves)
	{
		if(moves[i].move == func)
		{
			name = moves[i].name;
			break;
		}
	}

	return stream << name;
}

}	// namespace chess
}	//namespace gak

#ifdef __BORLANDC__
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

