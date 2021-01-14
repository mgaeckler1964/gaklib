/*
		Project:		GAKLIB
		Module:			logfile.h
		Description:	
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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class Profiler
{
	const char	*fileName;
	int			lineNumber;
	const char	*functionName;

	public:
	Profiler( const char *fileName, int lineNumber, const char *functionName );
	~Profiler();
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

void logFileLine( const std::string &line );
void logLine( const std::string &line );

void logError( const char *file, int line, unsigned long dw );
void enterFunction( const char *file, int line, const char *function, bool doLogProfile );
void exitFunction( const char *file, int line, bool doLogProfile );

void enableLog( void );
void disableLog( void );

void ignoreThreads( void );
void applyThreads( void );
void flushLogs( void );

#if DEBUG_LOG
#define doLogFile( val )		gakLogging::logFile( __FILE__, __LINE__, #val, val )
#define doLogValue( val )												\
{																		\
	static size_t callCount = 0;										\
	gakLogging::logValue( __FILE__, __LINE__, ++callCount, #val, val );	\
}

#define doLogPosition()													\
{																		\
	static size_t callCount = 0;										\
	gakLogging::logValue( __FILE__, __LINE__, ++callCount, "", "" );	\
}

#define doLogError( err )		gakLogging::logError( __FILE__, __LINE__, err )

#define doEnterFunction( x )	gakLogging::Profiler	_profiler( __FILE__, __LINE__, x )

#define doEnableLog()			gakLogging::enableLog()
#define doDisableLog()			gakLogging::disableLog()

#define doIgnoreThreads()		gakLogging::ignoreThreads()
#define doApplyThreads()		gakLogging::applyThreads()
#define doFlushLogs()			gakLogging::flushLogs()

#else
#define doLogFile( val )		/* nothing */

#define doLogValue( val )		/* nothing */
#define doLogBool( val )		/* nothing */
#define doLogPosition()			/* nothing */
#define doLogError( err )		/* nothing */

#if !PROFILER
#	define doEnterFunction( x )	/* nothing */
#	define doEnableLog()		/* nothing */
#	define doDisableLog()		/* nothing */
#else
#	define doEnterFunction( x )	gakLogging::Profiler	_profiler( __FILE__, __LINE__, x )
#	define doEnableLog()		gakLogging::enableLog()
#	define doDisableLog()		gakLogging::disableLog()
#endif


#define doIgnoreThreads()		/* nothing */
#define doApplyThreads()		/* nothing */
#define doFlushLogs()			/* nothing */
#endif

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

inline Profiler::Profiler( const char *fileName, int lineNumber, const char *functionName )
{
	this->fileName		= fileName;
	this->lineNumber	= lineNumber;
	this->functionName	= functionName;
	enterFunction( fileName, lineNumber, functionName, DO_LOG_PROFILE );
}

inline Profiler::~Profiler()
{
	exitFunction( fileName, lineNumber, DO_LOG_PROFILE );
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

template <typename ValueT>
void logValue( const char *fName, int lineNum, size_t callCount, const char *valName, const ValueT &val )
{
	std::stringstream	out;

	out << fName << ' ' << lineNum << " - " << callCount << ": " << valName << '=' << val;
	out.flush();
	logLine( out.str() );
}

template <>
inline void logValue( const char *fName, int lineNum, size_t callCount, const char *valName, const bool &val )
{
	gakLogging::logValue<const char *>(fName, lineNum, callCount, valName, val ? "true" : "false");
}

template <typename ValueT>
void logFile( const char *fName, int lineNum, const char *valName, const ValueT &val )
{
	std::stringstream	out;

	out << fName << ' ' << lineNum << ": " << valName << '=' << val;
	out.flush();
	logFileLine( out.str() );
	logLine( out.str() );
}

}	// namespace gakLogging

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

 #endif // GAK_LOGFILE_H
