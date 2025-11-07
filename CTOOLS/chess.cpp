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

#include <memory>

#include <gak/fmtNumber.h>

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

void Board::addPromoteMoves( Movements &moves, const Movement &moveTemplate ) const
{
	static Figure::Type promoTypes[] =
	{
		Figure::ftQueen, Figure::ftRook, Figure::ftKnight, Figure::ftBishop
	};
	FOR_EACH(i,promoTypes)
	{
		Movement &move = moves.addElement(moveTemplate);
		move.promotionType = promoTypes[i];
	}
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
Figure::Attack Figure::searchAttack(unsigned maxStep, const Position &ignore, const Position &stop ) const
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
		if( !isOK(attack, maxStep) )
		{
			return attack;
		}
	}

	return Attack();
}

STRING Movement::toString() const
{
	STRING result;

	if( fig )
	{
		// check rochade
		if( rook )
		{
			assert(fig->getType() == Figure::ftKing);
			assert(rook->getType() == Figure::ftRook);
			if( rookSrc.col == MAX_COL_LETTER )
			{
				result = "O-O";
			}
			else
			{
				result = "O-O-O";
			}
		}
		else
		{
			if( fig->getType() != Figure::ftPawn )
			{
				result = fig->getLetter();
			}
			result += src.col + formatNumber(unsigned(src.row));
#ifndef NDEBUG
			result += STRING('(')+formatNumber(unsigned(src.index))+')';
#endif
			result += captured ? 'x' : '-';
			result += dest.col + formatNumber(unsigned(dest.row));
#ifndef NDEBUG
			result += STRING('(')+formatNumber(unsigned(dest.index))+')';
#endif
			if( promotionType )
			{
				result += Figure::getLetter(promotionType);
			}
			if( state == csBlackCheck || state == csWhiteCheck )
			{
				result += '+';
			}
			else if( state == csBlackCheckMate || state == csWhiteCheckMate )
			{
				result += '#';
			}
		}
	}
	return result;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

std::ostream &operator << (std::ostream &stream, Position::MoveFunc func )
{
	const char *name="unknown";
	static struct 
	{
		Position::MoveFunc	move;
		const char			*name;
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

