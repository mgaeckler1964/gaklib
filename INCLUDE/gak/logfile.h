/*
		Project:		GAKLIB
		Module:			logfile.h
		Description:	some helper function for a logfile
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
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

#ifndef GAK_LOGFILE_H
#define GAK_LOGFILE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef DEBUG_LOG
#	if !defined( NDEBUG ) || defined( _DEBUG )
#		define DEBUG_LOG 1
#	else
#		define DEBUG_LOG 0
#	endif
#endif

#ifndef PROFILER
#	if !defined( NDEBUG ) || defined( _DEBUG )
#		define PROFILER 1
#	else
#		define PROFILER 0
#	endif
#endif

#if DEBUG_LOG
#	define DO_LOG_PROFILE true
#else
#	define DO_LOG_PROFILE false
#endif


// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <sstream>

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

namespace gakLogging
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

enum LogLevel
{
	llDetail, llInfo, llWarn, llError, llFatal, llNolog
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class Profiler
{
	bool		m_active;
	const char	*fileName;
	int			lineNumber;
	const char	*functionName;
	LogLevel	level;

	public:
	Profiler( LogLevel level, const char *fileName, int lineNumber, const char *functionName );
	~Profiler();
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

extern LogLevel g_minLevel;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	file logging (depreceated???)
*/
void logFileLine( const std::string &line );

template <typename ValueT>
void logFile( const char *fName, int lineNum, const char *valName, const ValueT &val )
{
	std::stringstream	out;

	out << fName << ' ' << lineNum << ": " << valName << '=' << val;
	out.flush();
	logFileLine( out.str() );
	logLine( out.str() );
}

#if DEBUG_LOG
#define doLogFile( val )		gakLogging::logFile( __FILE__, __LINE__, #val, val )
#else
#define doLogFile( val )		/* nothing */
#endif


/*
	values
*/

void logLine( LogLevel level, const std::string &line );

template <typename ValueT>
void logValue( LogLevel level, const char *fName, int lineNum, size_t callCount, const char *valName, const ValueT &val )
{
	if( g_minLevel > level )
	{
		return;
	}

	std::stringstream	out;

	out << fName << ' ' << lineNum << " - " << callCount << ": " << valName << '=' << val;
	out.flush();
	logLine( level, out.str() );
}

template <>
inline void logValue( LogLevel level, const char *fName, int lineNum, size_t callCount, const char *valName, const bool &val )
{
	if( g_minLevel > level )
	{
		return;
	}

	gakLogging::logValue<const char *>(level, fName, lineNum, callCount, valName, val ? "true" : "false");
}

#if DEBUG_LOG
#define doLogValueEx(lvl,  val)														\
{																					\
	if( gakLogging::g_minLevel <= lvl )											\
	{																				\
		static size_t callCount = 0;												\
		gakLogging::logValue( lvl, __FILE__, __LINE__, ++callCount, #val, val );	\
	}																				\
}
#else
#define doLogValueEx(lvl, val)		/* nothing */
#endif

#define doLogValue(val)			doLogValueEx( gakLogging::llFatal,  val )

/*
	messages
*/
#if DEBUG_LOG
#define doLogMessageEx(lvl,msg)														\
{																					\
	if( gakLogging::g_minLevel <= lvl )												\
	{																				\
		static size_t callCount = 0;												\
		gakLogging::logValue( lvl, __FILE__, __LINE__, ++callCount, "message", msg );	\
	}																				\
}
#else
#define doLogMessageEx(lvl,msg)		/* nothing */
#endif

#define doLogMessage(msg)		doLogMessageEx(gakLogging::llFatal,msg)

/*
	error
*/
void logError( const char *file, int line, unsigned long dw );

#if DEBUG_LOG
#define doLogError( err )		gakLogging::logError( __FILE__, __LINE__, err )
#else
#define doLogError( err )		/* nothing */
#endif


/*
	positions
*/
#if DEBUG_LOG
#define doLogPositionEx(lvl)													\
{																		\
	static size_t callCount = 0;										\
	gakLogging::logValue( lvl, __FILE__, __LINE__, ++callCount, "", "" );	\
}
#else
#define doLogPositionEx(lvl)		/* nothing */
#endif

#define doLogPosition()			doLogPositionEx(gakLogging::llFatal)

/*
	profiling
*/

bool enterFunction( LogLevel level, const char *file, int line, const char *function, bool doLogProfile );
void exitFunction( const char *file, int line, bool doLogProfile );

#if DEBUG_LOG || PROFILER
#	define doEnterFunctionEx( lvl,x )	gakLogging::Profiler	_profiler( lvl, __FILE__, __LINE__, x )
#else
#	define doEnterFunctionEx( lvl,x )	/* nothing */
#endif

#define doEnterFunction( x )	doEnterFunctionEx( gakLogging::llFatal, x )

/*
	disable log levels
*/
void enableLog( LogLevel minLevel );
void disableLog( void );
#if DEBUG_LOG || PROFILER
#	define doEnableLogEx(lvl)	gakLogging::enableLog(lvl)
#	define doDisableLog()		gakLogging::disableLog()
#else
#	define doEnableLogEx(lvl)	/* nothing */
#	define doDisableLog()		/* nothing */
#endif
#define doEnableLog()		doEnableLogEx(gakLogging::llDetail)


/*
	threads and strams
*/

#if DEBUG_LOG
#define doIgnoreThreads()		gakLogging::ignoreThreads()
#define doApplyThreads()		gakLogging::applyThreads()
#define doFlushLogs()			gakLogging::flushLogs()
#define doImmediateLog()		gakLogging::immediateLog()
#define doFastLog()				gakLogging::fastLog()
#else
#define doIgnoreThreads()		/* nothing */
#define doApplyThreads()		/* nothing */
#define doFlushLogs()			/* nothing */
#define doImmediateLog()		/* nothing */
#define doFastLog()				/* nothing */
#endif

void ignoreThreads( void );
void applyThreads( void );
void flushLogs( void );
void immediateLog();
void fastLog();

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

inline Profiler::Profiler( LogLevel level, const char *fileName, int lineNumber, const char *functionName )
{
	this->fileName		= fileName;
	this->lineNumber	= lineNumber;
	this->functionName	= functionName;
	this->level			= level;
	this->m_active = enterFunction( level, fileName, lineNumber, functionName, DO_LOG_PROFILE );
}

inline Profiler::~Profiler()
{
	if( m_active )
	{
		exitFunction( fileName, lineNumber, DO_LOG_PROFILE );
	}
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

}	// namespace gakLogging

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

 #endif // GAK_LOGFILE_H
