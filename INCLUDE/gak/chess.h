/*
		Project:		gaklib
		Module:			chess.h
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

#ifndef GAK_CHESS_H
#define GAK_CHESS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <memory>
#include <assert.h>

#include <gak/array.h>

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

class Figure;
class Board;

struct Position
{
	char	col;
	char	row;
	char	enPassant;

	Position(const Position &src) : col(src.col),row(src.row), enPassant(src.enPassant) {}
	Position(char col=0, char row=0) : col(col),row(row), enPassant(0) {}

	bool operator == ( const Position &o ) const
	{
		return col == o.col && row == o.row;
	}
	bool operator != ( const Position &o ) const
	{
		return col != o.col || row != o.row;
	}
	operator bool () const
	{
		return col && row;
	}
	Position move(int cDir, int rDir ) const
	{
		char newCol = char(col + cDir);
		char newRow = char(row + rDir);
		if( newCol <= 'H' && newCol >= 'A' && newRow >= 1 && newRow <= 8 )
		{
			return Position(newCol,newRow);
		}
		else
		{
			return Position(0,0);
		}
	}
	Position moveNorth()
	{
		return move(0,1);
	}
	Position moveNorthEast()
	{
		return move(1,1);
	}
	Position moveEast()
	{
		return move(1,0);
	}
	Position moveSouthEast()
	{
		return move(1,-1);
	}
	Position moveSouth()
	{
		return move(0,-1);
	}
	Position moveSouthWest()
	{
		return move(-1,-1);
	}
	Position moveWest()
	{
		return move(-1,0);
	}
	Position moveNorthWest()
	{
		return move(-1,1);
	}
	// springer zuege
	Position moveSNorthEast()
	{
		return move(1,2);
	}
	Position moveSEastNorth()
	{
		return move(2,1);
	}
	Position moveSEastSouth()
	{
		return move(2,-1);
	}
	Position moveSSouthEast()
	{
		return move(1,-2);
	}
	Position moveSSouthWest()
	{
		return move(-1,-2);
	}
	Position moveSWestSouth()
	{
		return move(-2,-1);
	}
	Position moveSWestNorth()
	{
		return move(-2,1);
	}
	Position moveSNorthWest()
	{
		return move(-1,2);
	}
};

struct Movement
{
	bool		isRochade;
	Position	src;
	Position	dest;

	Movement() : isRochade(false) {}
};

struct TargetPositions
{
	Position	targets[32];
	Position	beats[32];
	size_t		numTargets;
	size_t		numBeats;
	
	TargetPositions()
	{
		std::memset( this, 0, sizeof( *this ) );
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class Figure
{
	public:
	enum Color
	{
		White, Black
	};
	struct Attack
	{
		const Figure *figure;
		size_t steps;
		Attack()
		{
			figure = NULL;
			steps = 0;
		}
	};

	private:
	bool				m_moved;
	Position			m_pos;
	TargetPositions		m_targets;

	protected:
	Board				&m_board;

	public:
	const enum Color	m_color;

	private:
	Figure( const Figure & );
	Figure &operator=(const Figure &src );

	virtual TargetPositions calcPossible() = 0;

	protected:
	size_t checkDirection(TargetPositions *pos, Position (Position::*movement )(), size_t maxCount, bool allowSacrifice) const;

	public:
	Figure( Color color, Position pos, Board &board ) : m_color(color), m_pos(pos), m_board(board), m_moved(false) {}

	void refresh()
	{
		m_targets = calcPossible();
	}
	void moveTo( const Position &pos );
	void rochade( const Position &pos );
	const Position &getPos() const
	{
		return m_pos;
	}
	bool getMoved() const
	{
		return m_moved;
	}
	bool canBeat(const Position &pos) const
	{
		for( size_t i=0; i<m_targets.numBeats; ++i )
		{
			if( pos == m_targets.beats[i] )
			{
				return true;
			}
		}
		return false;
	}
	bool isOK( const Attack &attack ) const
	{
		return !attack.figure						// no attacker
			|| attack.figure->m_color == m_color	// no enemy
			|| !attack.figure->canBeat(m_pos) ;		// too weak?
	}
	const TargetPositions &getPossible() const
	{
		return m_targets;
	}

	size_t checkDirection(TargetPositions *pos, Position (Position::*movement )()) const
	{
		return checkDirection(pos, movement, 8, true);
	}

	Attack searchAttack(const Position &pos, Position (Position::*movement )()) const;

	Attack searchAttack(Position (Position::*movement )()) const
	{
		return searchAttack(m_pos, movement );
	}

	virtual int getValue() const = 0;
	virtual char getFigure() const = 0;
};

class Pawn : public Figure
{
	public:
	Pawn( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions calcPossible();
	virtual int getValue() const
	{
		return 1;
	}
	virtual char getFigure() const
	{
		return 'B';
	}
};

class Knight : public Figure
{
	public:
	Knight( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	size_t checkDirection(TargetPositions *pos, Position (Position::*movement )()) const
	{
		return Figure::checkDirection(pos, movement, 1, true);
	}
	virtual TargetPositions calcPossible();
	virtual int getValue() const
	{
		return 3;
	}
	virtual char getFigure() const
	{
		return 'S';
	}
};

class Bishop : public Figure
{
	public:
	Bishop( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions calcPossible();
	virtual int getValue() const
	{
		return 3;
	}
	virtual char getFigure() const
	{
		return 'L';
	}
};

class Rook : public Figure
{
	public:
	Rook( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions calcPossible();
	virtual int getValue() const
	{
		return 5;
	}
	virtual char getFigure() const
	{
		return 'T';
	}
};

class Queen : public Figure
{
	public:
	Queen( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions calcPossible();
	virtual int getValue() const
	{
		return 8;
	}
	virtual char getFigure() const
	{
		return 'D';
	}
};

class King : public Figure
{
	public:
	struct Rochade
	{
		Position	myTarget,
					rookTarget;
		Rook		*rook;

		Rochade() : rook(NULL) {}
	};
	
	private:
	Rochade m_rochadeWest, m_rochadeEast;

	public:
	King( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	size_t checkDirection(TargetPositions *pos, Position (Position::*movement )(), size_t maxCount) const
	{
		return Figure::checkDirection(pos, movement, maxCount, false);
	}
	virtual TargetPositions calcPossible();

	const Rochade &getEastRochade() const
	{
		return m_rochadeEast;
	}
	const Rochade &getWestRochade() const
	{
		return m_rochadeWest;
	}


	virtual int getValue() const
	{
		return 1000;
	}
	virtual char getFigure() const
	{
		return 'K';
	}
};

class Board
{
	Array<Movement>	m_moves;

	Figure *m_board[64];

	King	*m_whiteK;
	King	*m_blackK;

	Figure::Color	m_nextColor;

	void forget()
	{
		std::memset(m_board, 0, sizeof(m_board) );
	}

	void clear();
	void empty()
	{
		clear();
		forget();
	}

	void uncheckedMove(const Position &src, const Position &dest);

	public:
	Board()
	{
		m_nextColor = Figure::White;
		forget();
	}
	~Board()
	{
		clear();
	}
	void refresh();
	void reset();

	/* count figures */
	void evaluate1(int &whitePower, int &blackPower);
	int evaluate1()
	{
		int whitePower, blackPower;
		evaluate1(whitePower, blackPower);
		return whitePower - blackPower;
	}

	/* count targets and beats */
	void evaluate2(int &whiteTargets, int &blackTargets, int &whiteBeats, int &blackBeats);
	void evaluate2(int &targets, int &beats)
	{
		int whiteTargets, blackTargets, whiteBeats, blackBeats;
		evaluate2(whiteTargets, blackTargets, whiteBeats, blackBeats);
		targets = whiteTargets - blackTargets;
		beats = whiteBeats - blackBeats;
	}

	const King *getWhiteK() const
	{
		return m_whiteK;
	}
	const King *getBlackK() const
	{
		return m_blackK;
	}

	const Figure *getFigure(size_t index) const
	{
		return m_board[index];
	}
	Figure *getFigure(size_t index)
	{
		return m_board[index];
	}
	const Figure *getFigure(char col, char row) const
	{
		return getFigure(getIndex(col, row));
	}
	Figure *getFigure(char col, char row)
	{
		return getFigure(getIndex(col, row));
	}
	const Figure *getFigure(const Position &pos) const
	{
		return getFigure(getIndex(pos));
	}
	Figure *getFigure(const Position &pos)
	{
		return getFigure(getIndex(pos));
	}

	static size_t getIndex( char col, char row )
	{
		assert(col >= 'A' && col <= 'H' );
		assert(row >= 1 && row <= 8 );

		return (row-1)*8+col-'A';
	}
	static size_t getIndex( const Position &pos )
	{
		return getIndex(pos.col, pos.row);
	}
	static Position getPosition( size_t index )
	{
		char row = char(index/8 +1);
		char col = char(index%8 +'A');
		return Position(col, row);
	}

	const Figure *getAttacker( Figure::Color color, const Position &pos ) const;


	bool checkMoveTo( const Position &src, const Position &dest ); 
	void moveTo( const Position &src, const Position &dest ); 
	void moveTo( const Figure *src, const Position &dest )
	{
		return moveTo( src->getPos(), dest );
	}
	void rochade( const Figure *king, const Figure *rook, const Position &kingDest, const Position &rookDest );

	Position checkBoard() const
	{
		for( size_t i=0; i<64; ++i )
		{
			Position pos = getPosition(i);
			if( m_board[i] && m_board[i]->getPos() != pos )
			{
				return pos;
			}
		}

		return Position(0,0);
	}

	void print() const
	{
		std::cout << "  ";
		for( char c='A';c<='H'; ++c )
		{
			std::cout << ' ' << c;
		}
		std::cout << std::endl;

		bool whiteField = true;
		for( char row=8; row >= 1; --row )
		{
			std::cout << int(row) << ' ';
			for( char col='A'; col <= 'H'; ++col )
			{
				const Figure *fig = getFigure(col, row );
				if( !fig )
				{
					if( whiteField )
						std::cout << "  ";
					else
						std::cout << "XX";
				}
				else
				{
					char sym = fig->getFigure();
					if( fig->m_color == Figure::Black )
						sym = char(tolower(sym));
					std::cout << ' ' << sym;
				}
				whiteField = !whiteField;
			}
			std::cout << std::endl;
		}
	}
	const Array<Movement> &getMoves() const
	{
		return m_moves;
	}
};

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

}	// namespace chess
}	//namespace gak


#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_CHESS_H
