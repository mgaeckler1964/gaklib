/*
		Project:		gaklib
		Module:			chess.h
		Description:	The chess figures and board
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
#include <gak/stopWatch.h>
#include <gak/shared.h>

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
static const int KING_VALUE = PAWN_VALUE *2;

static const int BASE_VALUE		= PAWN_VALUE * NUM_COLS + BISHOP_VALUE*2 + KNIGHT_VALUE*2 + ROOK_VALUE * 2 + QUEEN_VALUE;
static const int PROMOTED_VALUE	= QUEEN_VALUE * NUM_COLS + BISHOP_VALUE*2 + KNIGHT_VALUE*2 + ROOK_VALUE * 2 + QUEEN_VALUE;

static const int INIT_VALUE = BASE_VALUE + KING_VALUE;

static const  int PLAYER_WINS = PROMOTED_VALUE*4;
static const  int WHITE_WINS = PLAYER_WINS;
static const  int BLACK_WINS = -PLAYER_WINS;

#if 1
// for german chess. 
static const char PAWN_LETTER = 'B';		// Bauer
static const char ROOK_LETTER = 'T';		// Turm
static const char KNIGHT_LETTER = 'S';		// Springer
static const char BISHOP_LETTER = 'L';		// L�ufer
static const char QUEEN_LETTER = 'D';		// Dame
static const char KING_LETTER = 'K';		// K�nig

static const char S_START[] = 
	"TSLDKLST"
	"BBBBBBBB"
	"        "
	"        "
	"        "
	"        "
	"bbbbbbbb"
	"tsldklst";

#define CHESS_WHITE			"Wei" OEM_sz
#define CHESS_BLACK			"Schwarz"
#define CHESS_BLANK			"Leer"
#define CHESS_DRAW			"Patt/Remis"
#define CHESS_CHECK_MATE	"Schachmatt"
#define CHESS_CHECK			"Schach"
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

static const char S_START[] = 
	"RNBQKBNR"
	"PPPPPPPP"
	"        "
	"        "
	"        "
	"        "
	"pppppppp"
	"rnbqkbnr";

#define CHESS_WHITE			"White"
#define CHESS_BLACK			"Black"
#define CHESS_BLANK			"Empty"
#define CHESS_DRAW			"Draw"
#define CHESS_CHECK_MATE	"Check Mate"
#define CHESS_CHECK			"Check"
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

enum State
{
	csBlank, 
	csPlaying, csWhiteCheck, csBlackCheck, 
	csEnd, csWhiteCheckMate, csBlackCheckMate, csDraw
};

struct Position
{
	typedef Position (Position::*MoveFunc)();

	char	col;
	char	row;
#ifndef NDEBUG
	size_t	index;

	void setIndex()
	{
		index = (row-1)*NUM_COLS+col-MIN_COL_LETTER;
	}
#endif

	Position(const Position &src) : col(src.col),row(src.row) {
#ifndef NDEBUG
		setIndex();
#endif
	}
	Position(char col=0, char row=0) : col(col),row(row) 
	{
#ifndef NDEBUG
		setIndex();
#endif
	}

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

class Destination
{
	Position	m_target;			// where I can move
	Position	m_capture;			// what I capture
	public:
	Destination( Position target=Position(), Position capture=Position() ) 
		: m_target(target), m_capture(capture) {}
	Destination( const Destination &src) 
		: m_target(src.m_target), m_capture(src.m_capture) {}

	const Position &getTarget() const
	{
		return m_target;
	}
	const Position &getCapture() const
	{
		return m_capture;
	}
};

struct PotentialDestinations
{
	Destination	targets[32];
	Position	threads[32];		// where I can capture, if a oponent is comming
	size_t		numTargets;
	bool		hasCaptures;
	size_t		numThreads;
	
	PotentialDestinations() : numTargets(0), hasCaptures(false), numThreads(0)
	{
		std::memset( threads, 0, sizeof( threads ) );
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
	Figure( Color color, Position pos, bool moved, Board &board ) : m_color(color), m_pos(pos), m_board(board), m_moved(moved), m_toKing(NULL), m_fromKing(NULL) {}

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
		assert( getType() != ftKing );
		m_pos = Position();
	}
	bool isActive() const
	{
		return m_pos;
	}
	
	void setPosition( const Position &pos, bool mark )
	{
		if( mark )
		{
			m_moved = true;
		}
		m_pos = pos;
	}
	const Position &getPos() const
	{
		return m_pos;
	}
	bool hasMoved() const
	{
		return m_moved;
	}
	bool isThread(const Position &pos) const
	{
		for( size_t i=0; i<m_targets.numThreads; ++i )
		{
			if( pos == m_targets.threads[i] )
			{
				return true;
			}
		}
		return false;
	}
	bool canCapture(const Position &pos) const
	{
		for( size_t i=0; i<m_targets.numTargets; ++i )
		{
			if( pos == m_targets.targets[i].getCapture() )
			{
				return true;
			}
		}
		return false;
	}
	bool canPromote() const
	{
		if( getType() != ftPawn )
			return false;
		if( m_color == White && m_pos.row == 7 )
			return true;
		if( m_color == Black && m_pos.row == 2 )
			return true;

		return false;
	}
	bool isOK( const Attack &attack, unsigned maxSteps ) const;
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
	Attack searchAttack(unsigned maxStep, const Position &ignore=Position(), const Position &stop=Position() ) const;
	bool isAttacked(const Position &ignore, const Position &stop, unsigned maxSteps=0 ) const
	{
		return !isOK(searchAttack(maxSteps, ignore,stop), maxSteps);
	}

	bool mustPromote(const Position &dest) const
	{
		return getType() == ftPawn && (dest.row == 8 || dest.row == 1);
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
	int maxDistance() const
	{
		return m_color == White ? NUM_ROWS-getPos().row : getPos().row-1;
	}
	int movedRows() const
	{
		return m_color == White ? getPos().row-2 : (NUM_ROWS-1)-getPos().row;
	}
	char enPassantRow() const
	{
		return m_color == White ? 5 : 4;
	}
	bool shouldCheckPassant() const
	{
		return getPos().row == enPassantRow();
	}
	public:
	Pawn( Color color, Position pos, bool moved, Board &board ) : Figure( color, pos, moved, board ) {}

	virtual PotentialDestinations calcPossible();
	virtual Type getType() const
	{
		return ftPawn;
	}
	virtual int getValue() const
	{
		int moved = movedRows() - 3;
		if( moved < 0 )
		{
			moved = 0;
		}
		return  PAWN_VALUE + moved;
	}
};

class Knight : public Figure
{
	public:
	Knight( Color color, Position pos, bool moved, Board &board ) : Figure( color, pos, moved, board ) {}

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
	Bishop( Color color, Position pos, bool moved, Board &board ) : Figure( color, pos, moved, board ) {}

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
	Rook( Color color, Position pos, bool moved, Board &board ) : Figure( color, pos, moved, board ) {}

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
	Queen( Color color, Position pos, bool moved, Board &board ) : Figure( color, pos, moved, board ) {}

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
	King( Color color, Position pos, bool moved, Board &board ) : Figure( color, pos, moved, board ) {}

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
	const Rochade *getRochade( const Position &dest ) const
	{
		if( dest == m_rochadeWest.myTarget )
		{
			return &m_rochadeWest;
		}
		if( dest == m_rochadeEast.myTarget )
		{
			return &m_rochadeEast;
		}
		return NULL;
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
	State		state;

	Movement() : fig(NULL), promotion(NULL), promotionType(Figure::ftNone), captured(NULL), rook(NULL), evaluate(0) {}
	operator bool ()
	{
		return fig != NULL && src && dest;
	}
	STRING toString() const;
};

typedef Array<Movement>	Movements;

#if 0
// preserve for future usage
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
#endif

class Board
{
	private:
	Figure					*m_board[NUM_FIELDS];
	ArrayOfPointer<Figure>	m_all;
	King					*m_whiteK;
	King					*m_blackK;
	State					m_state;
	Figure::Color			m_nextColor;
	Movements				m_history;
	StopWatch				m_whiteClock, m_blackClock;

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
	Figure::Color getNextColor() const
	{
		return m_nextColor;
	}
	const StopWatch &getWhiteClock() const
	{
		return m_whiteClock;
	}
	const StopWatch &getBlackClock() const
	{
		return m_blackClock;
	}

	private:
	void flipTurn()
	{
		m_nextColor = isWhiteTurn() ? Figure::Black : Figure::White;
	}
	void flipWatch()
	{
		if( isWhiteTurn() )
		{
			m_nextColor = Figure::Black;
			m_whiteClock.pause();
			m_blackClock.start();
		}
		else
		{
			m_nextColor = Figure::White;
			m_whiteClock.start();
			m_blackClock.pause();
		}
	}

	void clear()
	{
		m_history.clear();
		m_all.clear();
		std::memset(m_board, 0, sizeof(m_board) );
		m_whiteK = m_blackK = NULL;
		m_state = csBlank;
		m_whiteClock.stop();
		m_blackClock.stop();
	}

	Figure *checkEnPassant(const PlayerPos &src, const Position &dest) const;
	Figure *uncheckedMove(const PlayerPos &src, const Position &dest);
	Figure *passantMove(const PlayerPos &src, const Position &dest);

	void undoTmpMove(const Movement &move);
	void tmpMove(Movement &move);

	Movement findBest(int maxLevel, int *quality, bool fromPublic);
	static size_t findMove(const Movements &moves, const Position &src, const Position &dest );

	void addPromoteMoves(Movements &moves, const Movement &moveTemplate)  const;
	Movements collectMoves()  const;

	bool canMove() const;
	Movements findCheckDefend( size_t *numCheckers) const;

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
	bool canPlay() const
	{
		return m_state > csBlank && m_state < csEnd;
	}

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

	const Movements &getHistory() const
	{
		return m_history;
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
	size_t getShields( Figure::Color color, const Position &pos, FigurePtr *threads ) const;

	const Figure *getAttacker( const Figure *fig ) const;
	size_t getAttackers( const Figure *fig, FigurePtr *attackers ) const;
	size_t getCheckers( const Figure *king, FigurePtr *checkers ) const;

	bool checkMoveTo( const PlayerPos &src, const Position &dest, Figure::Type newFig=Figure::ftNone ) const;

	Figure *create( Figure::Color color, Figure::Type newFig, const Position &dest, bool moved );
	void moveTo( const PlayerPos &src, const Position &dest ); 
	void rochade( const PlayerPos &king, const PlayerPos &rook, const Position &kingDest, const Position &rookDest );
	void promote( const PlayerPos &pawn, Figure::Type newFig, const Position &dest );

	Movement findBest(int maxLevel, int *quality)
	{
		return findBest(maxLevel, quality, true );
	}
	void performMove(const Movement& move);

	Position checkBoard() const;
	State getState() const
	{
		return m_state;
	}
	STRING getStateString() const;
	void print() const;
	STRING generateString() const;
	void generateFromString(const STRING &string );

	void clone( const Board &src );
	SharedPointer<Board> clone();
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
