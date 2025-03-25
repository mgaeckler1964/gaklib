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
struct Position
{
	char			col;
	short			row;
	char			enPassant;
	const Figure	*second;		// for rochade

	Position(const Position &src) : col(src.col),row(src.row), enPassant(src.enPassant), second(src.second) {}
	Position(char col=0, char row=0) : col(col),row(row), enPassant(0), second(0) {}

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
	Position	src;
	Position	dest;
};

struct TargetPositions
{
	Position	targets[64];
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

/*
	const enum Type
	{
		Bauer, Laeufer, Springer, Turm, Dame, Koenig
	}		m_type;
*/
class Board;

class Figure
{
	bool		m_moved;
	Position	m_pos;

	private:
	Figure( const Figure & );
	Figure &operator=(const Figure &src );

	public:
	const enum Color
	{
		Weiss, Schwarz
	}		m_color;

	Board		&m_board;

	Figure( Color color, Position pos, Board &board ) : m_color(color), m_pos(pos), m_board(board), m_moved(false) {}

	void moveTo( const Position &pos )
	{
		m_moved = true;
		m_pos = pos;
	}
	const Position &getPos() const
	{
		return m_pos;
	}
	bool getMoved() const
	{
		return m_moved;
	}
	size_t checkDirection(TargetPositions *pos, Position (Position::*movement )(), size_t maxCount=8) const;
	virtual TargetPositions getPossible() const = 0;
	virtual int getValue() const = 0;
	virtual char getFigure() const = 0;
};

class Bauer : public Figure
{
	public:
	Bauer( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions getPossible() const;
	virtual int getValue() const
	{
		return 1;
	}
	virtual char getFigure() const
	{
		return 'B';
	}
};

class Springer : public Figure
{
	public:
	Springer( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions getPossible() const;
	virtual int getValue() const
	{
		return 3;
	}
	virtual char getFigure() const
	{
		return 'S';
	}
};

class Laeufer : public Figure
{
	public:
	Laeufer( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions getPossible() const;
	virtual int getValue() const
	{
		return 3;
	}
	virtual char getFigure() const
	{
		return 'L';
	}
};

class Turm : public Figure
{
	public:
	Turm( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions getPossible() const;
	virtual int getValue() const
	{
		return 5;
	}
	virtual char getFigure() const
	{
		return 'T';
	}
};

class Dame : public Figure
{
	public:
	Dame( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions getPossible() const;
	virtual int getValue() const
	{
		return 8;
	}
	virtual char getFigure() const
	{
		return 'D';
	}
};

class Koenig : public Figure
{
	public:
	Koenig( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual TargetPositions getPossible() const;
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

	Koenig	*weissK;
	Koenig	*schwarzK;

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
	public:
	Board()
	{
		forget();
	}
	~Board()
	{
		clear();
	}
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

	const Koenig *getWeissK() const
	{
		return weissK;
	}
	const Koenig *getSchwarzK() const
	{
		return schwarzK;
	}

	const Figure *getFigure(size_t index) const
	{
		return m_board[index];
	}
	const Figure *getFigure(char col, char row) const
	{
		return getFigure(getIndex(col, row));
	}
	const Figure *getFigure(const Position &pos) const
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
		char row = index/8 +1;
		char col = index%8 +'A';
		return Position(col, row);
	}

	bool checkMoveTo( const Position &src, const Position &dest ); 
	void moveTo( const Position &src, const Position &dest ); 

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
		for( int row=8; row >= 1; --row )
		{
			std::cout << row << ' ';
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
					if( fig->m_color == Figure::Color::Schwarz )
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
