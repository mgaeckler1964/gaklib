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
#include <gak/math.h>
#include <gak/map.h>

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

static const size_t NUM_ROWS = 8;
static const size_t NUM_COLS = 8;
static const size_t MAX_DISTANCE = 8;
static const size_t NUM_FIELDS = NUM_ROWS*NUM_COLS;
static const size_t NUM_TEAM_PAWNS = NUM_COLS;
static const size_t NUM_TEAM_FIGURES = 2*NUM_COLS;
static const size_t NUM_TOT_FIGURES = 2*NUM_TEAM_FIGURES;
static const char MIN_COL_LETTER = 'a';
static const char MAX_COL_LETTER = MIN_COL_LETTER + NUM_COLS -1;	//  'h'

static const int PAWN_VALUE = 1;
static const int BISHOP_VALUE = 3;
static const int KNIGHT_VALUE = 3;
static const int ROOK_VALUE = 5;
static const int QUEEN_VALUE = 8;

static const int BASE_VALUE = (PAWN_VALUE * NUM_COLS + BISHOP_VALUE*2 + KNIGHT_VALUE*2 + ROOK_VALUE * 2 + QUEEN_VALUE);
static const int PROMOTED_VALUE = (QUEEN_VALUE * NUM_COLS + BISHOP_VALUE*2 + KNIGHT_VALUE*2 + ROOK_VALUE * 2 + QUEEN_VALUE);

static const int KING_VALUE = PROMOTED_VALUE *2;

static const int INIT_VALUE = BASE_VALUE + KING_VALUE;

static const  int PLAYER_WINS = KING_VALUE*4;
static const  int WHITE_WINS = PLAYER_WINS;
static const  int BLACK_WINS = -PLAYER_WINS;
static const  int WHITE_CHECK = -KING_VALUE;
static const  int BLACK_CHECK = KING_VALUE;

#if 1
// for german chess. 
static const char PAWN_LETTER = 'B';		// Bauer
static const char ROOK_LETTER = 'T';		// Turm
static const char KNIGHT_LETTER = 'S';		// Springer
static const char BISHOP_LETTER = 'L';		// Läufer
static const char QUEEN_LETTER = 'D';		// Dame
static const char KING_LETTER = 'K';		// König

#define CHESS_WHITE			"Wei" OEM_sz
#define CHESS_BLACK			"Schwarz"
#define CHESS_DRAW			"Patt/Remis"
#define CHESS_CHECK_MATE	"Schwachmatt"
#define CHESS_CHECK_NEXT	"N" OEM_ae "chster"
#define CHESS_EVAL			"Bewertung"
#define CHESS_WINS			"(Nadja) gewinnt durch"
#else
// for english chess. warning; unit test works with german, only
static const char PAWN_LETTER = 'P';
static const char ROOK_LETTER = 'R';
static const char KNIGHT_LETTER = 'N';
static const char BISHOP_LETTER = 'B';
static const char QUEEN_LETTER = 'Q';
static const char KING_LETTER = 'K';

#define CHESS_WHITE			"White"
#define CHESS_BLACK			"Black"
#define CHESS_DRAW			"Draw"
#define CHESS_CHECK_MATE	"Check Mate"
#define CHESS_CHECK_NEXT	"Next"
#define CHESS_EVAL			"Evaluation"
#define CHESS_WINS			"(Nadja) wins by"
#endif

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

class Board;
class Figure;

typedef Figure *FigurePtr;

struct Position
{
	typedef Position (Position::*MoveFunc )();

	char	col;
	char	row;

	Position(const Position &src) : col(src.col),row(src.row) {}
	Position(char col=0, char row=0) : col(col),row(row) {}

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
		if( newCol <= MAX_COL_LETTER && newCol >= MIN_COL_LETTER && newRow >= 1 && newRow <= NUM_ROWS )
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

	static MoveFunc findMoveFunc( const Position &src, const Position &dest )
	{
		MoveFunc	result = NULL;

		if( src.col == dest.col )
		{
			if( src.row < dest.row )
			{
				return &moveNorth;
			}
			else if( src.row > dest.row )
			{
				return &moveSouth;
			}
		}
		else if( src.row == dest.row )
		{
			if( src.col < dest.col )
			{
				return &moveEast;
			}
			else if( src.col > dest.col )
			{
				return &moveWest;
			}
		}
		else
		{
			int horizDistance = dest.col - src.col;
			int vertDistance = dest.row - src.row;

			if( horizDistance == vertDistance )
			{
				if( vertDistance > 0 )
					return &moveNorthEast;
				if( vertDistance < 0 )
					return &moveSouthWest;
			}
			else if( horizDistance == -vertDistance )
			{
				if( vertDistance > 0 )
					return &moveNorthWest;
				if( vertDistance < 0 )
					return &moveSouthEast;
			}
		}
		return result;
	}
	static int getDistance( const Position &src, const Position &dest )
	{
		return math::max(math::abs(src.col-dest.col),math::abs(src.row-dest.row));
	}
};

struct PlayerPos
{
	Figure		*fig;
	Position	pos;
	size_t		index;

	PlayerPos(Figure *fig);
	PlayerPos(char col, char row, Board &board);
};

struct Destination
{
	Position	target;			// where I can move
	Position	captures;		// what I capture
};

struct PotentialDestinations
{
	Destination	targets[32];
	Position	threads[32];		// where I can capture, if a oponent is comming
	int			numTargets;
	bool		hasCaptures;
	int			numThreads;
	
	PotentialDestinations()
	{
		std::memset( this, 0, sizeof( *this ) );
	}
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //
class King;

class Figure
{
	public:
	enum Color
	{
		White, Black
	};
	enum Type
	{
		ftNone,
		ftPawn,
		ftKnight,
		ftBishop,
		ftRook,
		ftQueen,
		ftKing
	};
	struct Attack
	{
		const Figure *figure;
		size_t steps;
		Attack() : figure(NULL), steps(0)
		{
		}
	};

	private:
	bool					m_moved;
	Position				m_pos;
	PotentialDestinations	m_targets;
	Position::MoveFunc		m_toKing, m_fromKing;

	protected:
	Board					&m_board;

	public:
	const enum Color		m_color;

	private:
	Figure( const Figure & );
	Figure &operator=(const Figure &src );

	virtual PotentialDestinations calcPossible() = 0;

	protected:
	size_t checkRange(PotentialDestinations *result, Position::MoveFunc movement, size_t maxCount, bool allowSacrifice) const;

	public:
	Figure( Color color, Position pos, Board &board ) : m_color(color), m_pos(pos), m_board(board), m_moved(false), m_toKing(NULL), m_fromKing(NULL) {}

	King *getKing() const;
	void checkInterPos();

	void refresh()
	{
		// check whether figure is interposed between our king and an oponent
		checkInterPos();
		m_targets = calcPossible();
	}
	void capture()
	{
		m_pos = Position();
	}
	bool isActive() const
	{
		return m_pos;
	}
	
	void setPosition( const Position &pos )
	{
		m_moved = true;
		m_pos = pos;
	}
	void moveTo( const Position &pos );

	const Position &getPos() const
	{
		return m_pos;
	}
	bool hasMoved() const
	{
		return m_moved;
	}
	bool canCapture(const Position &pos) const
	{
		for( int i=0; i<m_targets.numTargets; ++i )
		{
			if( pos == m_targets.targets[i].captures )
			{
				return true;
			}
		}
		return false;
	}
	bool isOK( const Attack &attack ) const;
	const PotentialDestinations &getPossible() const
	{
		return m_targets;
	}

	size_t checkRange(PotentialDestinations *pos, Position::MoveFunc movement ) const
	{
		// good for Rook, Bishop and Queen, they may go the longest distance and it is allowed to be sacrified
		return checkRange(pos, movement, MAX_DISTANCE, true);
	}

	Attack searchAttack(const Position &pos, Position::MoveFunc movement, const Position &ignore, const Position &stop, int maxCount=MAX_DISTANCE ) const;

	Attack searchAttack(Position::MoveFunc movement, const Position &ignore=Position(), const Position &stop=Position() ) const
	{
		return searchAttack(m_pos, movement, ignore, stop  );
	}
	Attack searchAttack(const Position &ignore=Position(), const Position &stop=Position() ) const;
	bool isAttacked(const Position &ignore, const Position &stop ) const
	{
		Attack attack = searchAttack(ignore,stop);
		return isOK(attack);
	}

	virtual Type getType() const = 0;
	virtual int getValue() const = 0;
	char getLetter() const
	{
		return getLetter( getType() );
	}
	static char getLetter( Type type );
};

class Pawn : public Figure
{
	public:
	Pawn( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual PotentialDestinations calcPossible();
	virtual Type getType() const
	{
		return ftPawn;
	}
	virtual int getValue() const
	{
		return PAWN_VALUE;
	}
};

class Knight : public Figure
{
	public:
	Knight( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	size_t checkRange(PotentialDestinations *pos, Position::MoveFunc movement) const
	{
		// knight can gon one step, only, and it is allowed to be sacrified
		return Figure::checkRange(pos, movement, 1, true);
	}
	virtual PotentialDestinations calcPossible();
	virtual Type getType() const
	{
		return ftKnight;
	}
	virtual int getValue() const
	{
		return KNIGHT_VALUE;
	}
};

class Bishop : public Figure
{
	public:
	Bishop( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual PotentialDestinations calcPossible();
	virtual Type getType() const
	{
		return ftBishop;
	}
	virtual int getValue() const
	{
		return BISHOP_VALUE;
	}
};

class Rook : public Figure
{
	public:
	Rook( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual PotentialDestinations calcPossible();
	virtual Type getType() const
	{
		return ftRook;
	}
	virtual int getValue() const
	{
		return ROOK_VALUE;
	}
};

class Queen : public Figure
{
	public:
	Queen( Color color, Position pos, Board &board ) : Figure( color, pos, board ) {}

	virtual PotentialDestinations calcPossible();
	virtual Type getType() const
	{
		return ftQueen;
	}
	virtual int getValue() const
	{
		return QUEEN_VALUE;
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

	size_t checkRange(PotentialDestinations *pos, Position::MoveFunc movement, size_t maxCount) const
	{
		// knight can go one,  only, or two while rochade, and it is NOT allowed to be sacrified
		return Figure::checkRange(pos, movement, maxCount, false);
	}
	virtual PotentialDestinations calcPossible();

	const Rochade &getEastRochade() const
	{
		return m_rochadeEast;
	}
	const Rochade &getWestRochade() const
	{
		return m_rochadeWest;
	}


	virtual Type getType() const
	{
		return ftKing;
	}
	virtual int getValue() const
	{
		return KING_VALUE;
	}
};

struct Movement
{
	Figure			*fig;
	Position		src;
	Position		dest;
	Figure			*promotion;
	Figure::Type	promotionType;

	Figure			*captured;
	Position		capturePos;

	// rochade
	Figure		*rook;
	Position	rookSrc;
	Position	rookDest;

	int			evaluate;

	Movement() : fig(NULL), promotion(NULL), promotionType(Figure::ftNone), captured(NULL), rook(NULL), evaluate(0) {}
	operator bool ()
	{
		return fig != NULL && src && dest;
	}
	STRING toString() const;
};

typedef Array<Movement>	Movements;

struct PortableMove
{
	char	figure;
	char	fromCol;
	char	fromRow;
	char	toCol;
	char	toCRow;
	int		evaluation;
};
typedef Array<PortableMove>	PortableMoves;
typedef PairMap<STRING, PortableMoves> ChessLibrary;

class Board
{
	public:
	enum State
	{
		csBlank, 
		csPlaying, csWhiteCheck, csBlackCheck, 
		csEnd, csWhiteCheckMate, csBlackCheckMate, csDraw
	};

	private:

	Figure					*m_board[NUM_FIELDS];
	ArrayOfPointer<Figure>	m_all;
	King					*m_whiteK;
	King					*m_blackK;
	State					m_state;
	Figure::Color			m_nextColor;
	Movements				m_moves;

	public:
	static bool isWhiteTurn(Figure::Color nextColor)
	{
		return nextColor == Figure::White;
	}
	bool isWhiteTurn() const
	{
		return isWhiteTurn(m_nextColor);
	}
	static bool isBlackTurn(Figure::Color nextColor)
	{
		return nextColor == Figure::Black;
	}
	bool isBlackTurn() const
	{
		return isBlackTurn(m_nextColor);
	}
	private:
	void flipTurn()
	{
		m_nextColor = isWhiteTurn() ? Figure::Black : Figure::White;
	}

	bool canPlay() const
	{
		return m_state > csBlank && m_state < csEnd;
	}
	void clear()
	{
		m_moves.clear();
		m_all.clear();
		std::memset(m_board, 0, sizeof(m_board) );
		m_whiteK = m_blackK = NULL;
		m_state = csBlank;
	}

	Figure *checkEnPassant(const PlayerPos &src, const Position &dest) const;
	Figure *uncheckedMove(const PlayerPos &src, const Position &dest);
	Figure *passantMove(const PlayerPos &src, const Position &dest);

	void undoMove(const Movement &move);
	void redoMove(Movement &move);

	static size_t findMove(const Movements &moves, const Position &src, const Position &dest );

	bool canMove(Figure::Color color) const;
	Movements collectMoves()  const;
	Movements findCheckDefend( size_t *numAttackers) const;

	int evaluateMovements(Movements &movements, int maxLevel);

	void reset( Figure::Color color );

	// do nothing, just forbid
	Board( const Board &source );
	Board &operator = ( const Board &source );
	
	public:
	Board()
	{
		clear();
	}
	~Board()
	{
		clear();
	}
	void refresh();
	void checkMate();
	void checkCheck();
	void reset();

	/* count figures */
	void evaluateForce(int &whiteForce, int &blackForce) const;
	int evaluateForce() const
	{
		int whiteForce, blackForce;
		evaluateForce(whiteForce, blackForce);
		return whiteForce - blackForce;
	}

	/* count targets and captures */
	void evaluateRange(int &whiteTargets, int &blackTargets, int &whiteCaptures, int &blackCaptures) const;
	void evaluateRange(int &targets, int &captures) const
	{
		int whiteTargets, blackTargets, whiteCaptures, blackCaptures;
		evaluateRange(whiteTargets, blackTargets, whiteCaptures, blackCaptures);
		targets = whiteTargets - blackTargets;
		captures = whiteCaptures - blackCaptures;
	}

	int evaluate() const;

	King *getKing( Figure::Color color ) const
	{
		return isWhiteTurn(color) ? getWhiteK() : getBlackK();
	}
	King *getOponentKing( Figure::Color color ) const
	{
		return isWhiteTurn(color) ? getBlackK() : getWhiteK();
	}
	King *getCurKing() const
	{
		return isWhiteTurn() ? getWhiteK() : getBlackK();
	}
	King *getWhiteK() const
	{
		return m_whiteK;
	}
	King *getBlackK() const
	{
		return m_blackK;
	}

	Figure *getFigure(size_t index) const
	{
		return m_board[index];
	}
	Figure *getFigure(char col, char row) const
	{
		return getFigure(getIndex(col, row));
	}
	Figure *getFigure(const Position &pos) const
	{
		return getFigure(getIndex(pos));
	}

	const Movements &getMoves() const
	{
		return m_moves;
	}

	static size_t getIndex( char col, char row )
	{
		assert(col >= MIN_COL_LETTER && col <= MAX_COL_LETTER );
		assert(row >= 1 && row <= NUM_ROWS );

		return (row-1)*NUM_COLS+col-MIN_COL_LETTER;
	}
	static size_t getIndex( const Position &pos )
	{
		return getIndex(pos.col, pos.row);
	}
	static Position getPosition( size_t index )
	{
		char row = char(index/NUM_COLS +1);
		char col = char(index%NUM_COLS +MIN_COL_LETTER);
		return Position(col, row);
	}

	const Figure *getThread( Figure::Color color, const Position &pos, bool check4King  ) const;
	size_t getThreads( Figure::Color color, const Position &pos, FigurePtr *threads ) const;

	const Figure *getAttacker( const Figure *fig ) const;
	size_t getAttackers( const Figure *fig, FigurePtr *attackers ) const;

	bool checkMoveTo( const PlayerPos &src, const Position &dest, Figure::Type newFig=Figure::ftNone ) const;

	void moveTo( const PlayerPos &src, const Position &dest ); 
	void rochade( const PlayerPos &king, const PlayerPos &rook, const Position &kingDest, const Position &rookDest );
	Figure *create( Figure::Color color, Figure::Type newFig, const Position &dest );
	void promote( const PlayerPos &pawn, Figure::Type newFig, const Position &dest );

	Movement findBest(int maxLevel, int *quality);

	Position checkBoard() const;
	void print() const;
	STRING generateString() const;
	void generateFromString(const STRING &string );
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

std::ostream &operator << (std::ostream &stream, Position::MoveFunc func );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

inline PlayerPos::PlayerPos(Figure	*fig) : fig(fig), pos(fig->getPos()), index(Board::getIndex(pos))
{
}

inline PlayerPos::PlayerPos(char col, char row, Board &board) : pos(col,row), index(Board::getIndex(pos))
{
	fig = board.getFigure(pos);
}

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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_CHESS_H
