/*
		Project:		GAKLIB
		Module:			logFile.cpp
		Description:	do some debug logging and profiling
		Author:			Martin Gäckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
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

/*
#undef NDEBUG
#ifndef _DEBUG
#	define _DEBUG
#endif

#undef DEBUG_LOG
#define DEBUG_LOG 1
*/

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#ifdef __BORLANDC__
#include <iostream>
#endif

#include <memory>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <queue>
#include <map>

#include <gak/thread.h>
#include <gak/map.h>
#include <gak/logfile.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

/* --------------------------------------------------------------------- */
/* ----- module switches ----------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -a2
#		pragma option -po
#	endif
#endif

namespace gakLogging
{

/* --------------------------------------------------------------------- */
/* ----- macros -------------------------------------------------------- */
/* --------------------------------------------------------------------- */

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

static const char *s_logLevels[] =
{
	"llDetail", "llInfo", "llWarn", "llError", "llFatal", "llNolog"
};

/*
---------------------------------------------------------------------------
	Profiling
---------------------------------------------------------------------------
*/

typedef std::map<const char	*, int> FunctionMap;

struct ProfileLogEntry
{
	LogLevel			logLevel;
	const char 			*file;
	int					line;
	const char			*functionName;
	gak::ThreadID		threadId;
	int					count;
	std::clock_t		startTimeCPU;
	std::clock_t		startTimeReal;
	FunctionMap			functionMap;
	const char			*callerFunc;
	bool				recursiveCall;
};

typedef std::vector<ProfileLogEntry>		Summaries;
typedef std::vector<ProfileLogEntry>		CallStack;
typedef std::map<gak::ThreadID, CallStack>	CallStacks;

/*
---------------------------------------------------------------------------
	Logging
---------------------------------------------------------------------------
*/

class LogLine
{
	std::string		m_line;
	std::size_t		m_indent;
	std::clock_t	m_ticks;

	public:
	LogLine( LogLevel lvl, const std::string &line, std::size_t indent )
	{
		m_line = line + '|' + s_logLevels[lvl];
		m_indent = indent;
		m_ticks = gak::UserTimeClock::clock();
	}
	std::size_t getIndent() const
	{
		return m_indent;
	}
	std::clock_t getTicks() const
	{
		return m_ticks;
	}
	const std::string &getLine() const
	{
		return m_line;
	}
};

class LoggingThread : public gak::Thread
{
	std::ofstream		m_out;
	std::string			m_fileName;
	std::queue<LogLine>	m_logLines;
	gak::Locker			m_locker;

	virtual void ExecuteThread( void );

	public:
	LoggingThread( const std::string &fileName )
	{
		m_fileName = fileName;
		StartThread();
	}

	void logLine( const LogLine &line );
	void pushLine( const LogLine &line )
	{
		gak::LockGuard	guard( m_locker );
		return m_logLines.push( line );
	}
	bool hasOpenLines() const
	{
		return m_logLines.size() > 0;
	}
	void flush()
	{
		while( hasOpenLines() )
		{
			Sleep( 1 );
		}
	}
};

typedef gak::SharedObjectPointer<LoggingThread>		LoggingThreadPtr;
typedef gak::PairMap<std::string, LoggingThreadPtr>	LoggingThreads;

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

LogLevel		g_minLevel			= llNolog;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static bool	s_ignoreThread		= false;
static bool	s_shutdownProfile	= false;
static bool	s_shutdownLogging	= false;

static const bool s_flushDebug	= false;
static const bool s_asyncLog	= true;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static gak::Locker &getLocker()
{
	static gak::Locker *locker = new gak::Locker();

	return *locker;
}

/*
---------------------------------------------------------------------------
	Profiling
---------------------------------------------------------------------------
*/

static CallStacks &getCallStacks()
{
	static CallStacks	*logEntries = new CallStacks();

	return *logEntries;
}

static inline CallStack &getCallStack()
{
	CallStacks	&logEntries = getCallStacks();

	return logEntries[gak::Locker::GetCurrentThreadID()];
}

static Summaries &getSummaryEntries()
{
	static Summaries *summaryEntries = new Summaries();

	return *summaryEntries;
}

static FILE *getCsvFp( void )
{
	char		tmpFile[10240];
	const char	*temp = getenv( "TEMP" );

	if( !temp )
	{
		temp = DIRECTORY_DELIMITER_STRING "tmp";
	}

	sprintf(
		tmpFile,
		"%s" DIRECTORY_DELIMITER_STRING "gaklib%d.csv",
		temp, int(GetCurrentProcessId())
	);
	return fopen( tmpFile, "a" );
}

static void createSummaryEntry( const ProfileLogEntry &logEntry )
{
	Summaries	&summaryEntries = getSummaryEntries();

	for( 
		Summaries::iterator it = summaryEntries.begin(), endIT = summaryEntries.end();
		it != endIT;
		++it
	)
	{
		ProfileLogEntry &summary = *it;
		if( summary.line	 == logEntry.line
		&&  summary.threadId == logEntry.threadId
		&&  summary.file	 == logEntry.file
		&&  summary.functionName == logEntry.functionName )
		{
			if( !logEntry.recursiveCall )
			{
				summary.startTimeCPU += logEntry.startTimeCPU;
				summary.startTimeReal += logEntry.startTimeReal;
				summary.count++;
			}
			if( logEntry.callerFunc )
			{
#ifndef NDEBUG
				size_t	oldNumCaller = summary.functionMap.size();
#endif
				summary.functionMap[logEntry.callerFunc]++;
#ifndef NDEBUG
				size_t	newNumCaller = summary.functionMap.size();
				assert( newNumCaller >= oldNumCaller && newNumCaller - oldNumCaller <= 1 );
#endif
			}
/***/		return;
		}
	}

	summaryEntries.push_back( logEntry );
}

static void exitProfile( void )
{
	s_shutdownProfile = true;

	Summaries	&summaryEntries = getSummaryEntries();
	if( !summaryEntries.size() )
	{
/*@*/	return;
	}

	gak::STDfile	fp( getCsvFp() );
	assert( fp );

	fprintf( fp, "%s", "file,line,function,thread,cpu time,total time,count\n" );
	for( 
		Summaries::iterator it = summaryEntries.begin(), endIT = summaryEntries.end();
		it != endIT;
		++it
	)
	{
		ProfileLogEntry		&theEntry = *it;
		clock_t				executionTimeCPU = theEntry.startTimeCPU;
		unsigned long		executionTimeReal = theEntry.startTimeReal;
		fprintf(
			fp,
			"%s,%d,\"%s\",%p,%lf,%lf,%d,",
			theEntry.file, theEntry.line,
			theEntry.functionName,
			(void*)theEntry.threadId,
			(double)executionTimeCPU/(double)CLOCKS_PER_SEC,
			(double)executionTimeReal/1000.0,
			theEntry.count
		);
		if( theEntry.callerFunc )
		{
			theEntry.functionMap[theEntry.callerFunc]++;
		}
		for( 
			FunctionMap::iterator it = theEntry.functionMap.begin(), endIT = theEntry.functionMap.end();
			it != endIT;
			++it
		)
		{
			fprintf(
				fp,
				"\"%s\",%d,",
				it->first,
				it->second
			);
		}
		fputc( '\n', fp );
	}

	disableLog();
}

/*
---------------------------------------------------------------------------
	Logging
---------------------------------------------------------------------------
*/

static void exitLogging();

static LoggingThreads &getLoggingThreads()
{
	static LoggingThreads	*loggingThreads = new LoggingThreads();
	static bool first = true;

	if( first )
	{
		atexit( exitLogging );
		first = false;
	}

	return *loggingThreads;
}

static void exitLogging()
{
	s_shutdownLogging = true;

	LoggingThreads &threads = getLoggingThreads();
	for( 
		LoggingThreads::iterator it = threads.begin(), endIT = threads.end();
		it != endIT;
		++it
	)
	{
		LoggingThreadPtr thread = it->getValue();
		thread->flush();
		thread->StopThread();
		thread->join();
	}

	threads.clear();
}

static LoggingThreadPtr &getLoggingThread( const std::string &logFileName )
{
	LoggingThreads		&threads = getLoggingThreads();
	LoggingThreadPtr	&thread = threads[logFileName];
	if( !thread )
	{
		thread = new LoggingThread( logFileName );
	}

	return thread;
}

static inline std::string getLogFilename( gak::ThreadID curThread )
{
	std::stringstream	stream;
	const char			*temp = getenv( "TEMP" );
	std::string			fName = temp ? temp : "";

	if( !fName.size() )
	{
		stream << DIRECTORY_DELIMITER_STRING "tmp";
	}
	else
	{
		stream << fName;
	}

	stream << DIRECTORY_DELIMITER_STRING "gaklib" << GetCurrentProcessId() << '_' << (s_ignoreThread ? 0 :curThread) << ".log";
	return stream.str();
}

static inline std::string getGlobalLogFilename()
{
	std::string fName = getenv( "TEMP" );
	if( !fName.size() )
	{
		fName = DIRECTORY_DELIMITER_STRING "tmp";
	}

	fName += DIRECTORY_DELIMITER_STRING "gaklib.log";

	return fName;
}

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

void LoggingThread::ExecuteThread( void )
{
	while( !terminated )
	{
		if( !m_logLines.size() )
		{
			Sleep( 100 );
		}
		else
		{
			gak::LockGuard	guard( m_locker );

			const LogLine &line = m_logLines.front();
			logLine( line );
			m_logLines.pop();
		}
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void LoggingThread::logLine( const LogLine &line )
{
	if( !m_out.is_open() )
	{
		m_out.open(
			m_fileName.c_str(), std::ios_base::app | std::ios_base::out
		);
	}
	if( m_out.is_open() )
	{
		for( unsigned i=0; i<line.getIndent(); ++i )
		{
			m_out << "    ";
		}
		m_out << line.getLine() << ' ' << line.getTicks() << '\n';

		if( s_flushDebug )
		{
			m_out.flush();
		}
	}
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*
---------------------------------------------------------------------------
	Profiling
---------------------------------------------------------------------------
*/
bool enterFunction( LogLevel level, const char *file, int line, const char *function, bool doLog )
{
	if( level < g_minLevel || s_shutdownProfile )
	{
		return false;
	}

	static bool	first = true;
	if( first )
	{
		atexit( exitProfile );
		first = false;
	}

	gak::LockGuard		lock( getLocker() );
	CallStack			&logEntries = getCallStack();
	std::size_t			curIndent = logEntries.size();
	gak::ThreadID		curThread = gak::Locker::GetCurrentThreadID();
	std::string			fileName = getLogFilename( curThread );
	bool				recursiveCall = false;

	// find out this is a recursive call
	for( 
		CallStack::iterator it = logEntries.begin(), endIT = logEntries.end();
		it != endIT;
		++it
	)
	{
		const	ProfileLogEntry &logEntry = *it;
		if( logEntry.functionName == function
		&&  logEntry.file == file
		&&  logEntry.line == line )
		{
			recursiveCall = true;
			break;
		}
	}

	ProfileLogEntry theEntry;
	theEntry.logLevel = level;
	theEntry.file = file;
	theEntry.line = line;
	theEntry.functionName = function;
	theEntry.threadId = curThread;
	theEntry.startTimeCPU = gak::CpuTimeClock::clock();
	theEntry.startTimeReal = gak::UserTimeClock::clock();
	theEntry.callerFunc =
		logEntries.size()
		? logEntries.rbegin()->functionName
		: NULL;
	theEntry.recursiveCall = recursiveCall;
	logEntries.push_back(theEntry);

	if( doLog )
	{
		LoggingThreadPtr	&thread = getLoggingThread( fileName );
		std::stringstream	out;

		out << ">>>Enter " << function << " at " << file << ' ' << line << ":  time =" << theEntry.startTimeReal;
		out.flush();

		thread->pushLine( LogLine( level, out.str(), curIndent ) );
	}

	return true;
}

void exitFunction( const char *file, int line, bool doLog )
{
	if( s_shutdownProfile )
	{
		return;
	}

	gak::LockGuard		lock( getLocker() );
	CallStack			&logEntries = getCallStack();
	std::size_t			curIndent = logEntries.size();
	gak::ThreadID		curThread = gak::Locker::GetCurrentThreadID();
	std::string			fileName = getLogFilename( curThread );

	if( curIndent > 0 )
	{
		--curIndent;
		ProfileLogEntry		&theEntry = logEntries[curIndent];
		clock_t				executionTimeCPU = gak::CpuTimeClock::clock()-theEntry.startTimeCPU;
		clock_t				executionTimeReal = gak::UserTimeClock::clock()-theEntry.startTimeReal;

		if( doLog )
		{
			std::stringstream	out;
			LoggingThreadPtr	&thread = getLoggingThread( fileName );

			out << "<<<Exit " << theEntry.functionName << " at " << file << ' ' << line << ":  time =" << executionTimeCPU << '/' << executionTimeReal;
			out.flush();

			thread->pushLine( LogLine( theEntry.logLevel, out.str(), curIndent ) );
		}

		theEntry.startTimeCPU = executionTimeCPU;
		theEntry.startTimeReal = executionTimeReal;
		theEntry.count = 1;
		createSummaryEntry( theEntry );

		// mark entry as finished
		logEntries.pop_back();
	}
	else if( doLog )
	{
		std::stringstream	out;
		LoggingThreadPtr	&thread = getLoggingThread( fileName );

		out << "!!!unknown exit from " << file << ' ' << line << " disabled at start?";
		out.flush();

		thread->pushLine( LogLine( llNolog, out.str(), 0 ) );
	}
}

/*
---------------------------------------------------------------------------
	Logging
---------------------------------------------------------------------------
*/

void logFileLine( const std::string &line )
{
	if( llInfo < g_minLevel || s_shutdownProfile )
	{
		return;
	}

	gak::LockGuard		lock( getLocker() );
	std::string			fileName = getGlobalLogFilename();
	LoggingThreadPtr	&thread = getLoggingThread( fileName );
	thread->pushLine( LogLine( llInfo, line, 0 ) );
/*

	STDfile	fp( getGlobalLogFp() );

	if( fp )
	{
		fprintf( fp, "%s time=%ld\n", line.c_str(), long(clock()) );
	}
*/
}

void logLine( LogLevel level, const std::string &line )
{
	if( g_minLevel > level || s_shutdownLogging )
	{
		return;
	}

	gak::LockGuard		lock( getLocker() );
	CallStack			&logEntries = getCallStack();
	std::size_t			curIndent = logEntries.size();
	gak::ThreadID		curThread = gak::Locker::GetCurrentThreadID();
	std::string			fileName = getLogFilename( curThread );
	LoggingThreadPtr	&thread = getLoggingThread( fileName );

	thread->pushLine( LogLine( level, line, curIndent ) );
	if( !s_asyncLog )
	{
		flushLogs();
	}
}

#ifdef __WINDOWS__
void logError( const char *file, int line, DWORD dw )
{ 
    // Retrieve the system error message for the last-error code

    char *lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

	std::stringstream	out;

	out << file << ' ' << line << ": Error=" << lpMsgBuf;
	out.flush();

	logLine( llError,out.str() );

	LocalFree( LPVOID(lpMsgBuf) );
}
#endif

void flushLogs( void )
{
	LoggingThreads &threads = getLoggingThreads();
	for( 
		LoggingThreads::iterator it = threads.begin(), endIT = threads.end();
		it != endIT;
		++it
	)
	{
		LoggingThreadPtr thread = it->getValue();
		thread->flush();
	}
}


/*
---------------------------------------------------------------------------
	Configuration
---------------------------------------------------------------------------
*/
void disableLog( void )
{
	g_minLevel = llNolog;
}

void enableLog( LogLevel minLevel )
{
	g_minLevel = minLevel;
}

void ignoreThreads( void )
{
	s_ignoreThread = true;
}

void applyThreads( void )
{
	s_ignoreThread = false;
}

}	// namespace gakLogging

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
