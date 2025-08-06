/*
		Project:		GAKLIB
		Module:			locker.h
		Description:	Locking
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

#ifndef GAK_LOCKER_H
#define GAK_LOCKER_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <time.h>
#include <assert.h>

#if defined( __BORLANDC__ ) || defined( _MSC_VER )
#	ifndef STRICT
#		define STRICT 1
#	endif
#	include <windows.h>
#elif defined( __MACH__ ) || defined( __unix__ )
#	include <pthread.h>
#	include <sys/types.h>
#	include <unistd.h>
inline int GetCurrentProcessId( void )
{
	return (int)getpid();
}
inline void Sleep( unsigned long to )
{
	usleep(to *1000);
}
#else
	#error "Don't know thread implementation"
#endif

#include <gak/stopWatch.h>
#include <gak/conditional.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -a4
#	pragma option -pc
#endif

namespace gak
{

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

#if defined( _Windows )
	/// type to store the OS specific handle of a Thread
	typedef DWORD		ThreadID;
#elif defined( __MACH__ ) || defined( __unix__ )
	typedef pthread_t	ThreadID;
#else
#	error "Unkown operating system"
#endif

#ifdef __MACH__
	/// const value to specify an unkown Thread
#	define UndefinedThread		ThreadID(-1)
#endif

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/// this class can be used as an mutex for multiple threads in the application.
class Locker
{
	private:
	Conditional	m_conditional;
	ThreadID	m_lockedBy;
	int			m_lockCount;
#if defined( __MACH__ ) || defined( __unix__ )
//	static ThreadID		mainThread;
#endif

	bool lock( const StopWatch &startTime, unsigned long timeOut );
	bool isAvail( ThreadID threadId ) const
	{
		return m_lockedBy == UndefinedThread || m_lockedBy == threadId;
	}
	public:
	#ifndef __MACH__
	/// const value to specify an unkown Thread
	static const ThreadID UndefinedThread = ThreadID(-1);
	#endif

	/// creates a new unlocked mutex
	Locker()
	{
		m_lockedBy = UndefinedThread;
		m_lockCount = 0;
	}
	/// return the OS specific handle of the current Thread
	static ThreadID GetCurrentThreadID( void )
	{
#if defined( _Windows )
		return ::GetCurrentThreadId();
#elif defined( __MACH__ ) || defined( __unix__ )
		ThreadID	myID = pthread_self();
//		if( mainThread == LOCKERundefinedThread )
//			mainThread = myID;
//		else if( pthread_equal( mainThread, myID ) )
//			myID = mainThread;

		return myID;
#endif
	}
	/// returns true if the mutex can be locked by the current Thread
	bool isAvail( void ) const
	{
		return isAvail( GetCurrentThreadID() );
	}
	/**
		@brief tries to unlock this mutex
		
		if it has not been locked by the current Thread it does nothing.
		otherwise it decrements the lock counter and if it is 0, the mutex
		will be unlocked. 
		
		@see lock
	*/
	void unlock( void )
	{
		if( m_lockedBy == GetCurrentThreadID() )
		{
			m_lockCount--;
			assert( m_lockCount >= 0 );
			if( !m_lockCount )
			{
				m_lockedBy = UndefinedThread;
				m_conditional.notify();
			}
		}
	}
	/// constant value to specify to wait forever
	static const unsigned long WAIT_FOREVER = static_cast<unsigned long>(-1);
	/**
		@brief tries to lock this mutex

		if it has been locked by another Thread, it waits for the mutex beeing freed.
		on success the lock counter will be incremented.

		@param timeOut the max time in ms to wait for the lock beeing available
		@return true on success
	*/
	bool lock( unsigned long timeOut = WAIT_FOREVER );
	/// return the OS specific handle of the Thread that locked this mutex
	ThreadID getLockedBy( void ) const
	{
		return m_lockedBy;
	}
	/// returns the lock count
	int getLockCount( void ) const
	{
		return m_lockCount;
	}
};

/**
	@brief Helper class for locking and unlocking a Locker

	@code
		Locker	locker;

		...
		{
			LockGuard	lock( locker );

			if( lock )
			{
				...
			}
		}
	@endcode
*/
class LockGuard
{
	Locker	&m_locker;
	bool	m_locked;

	public:
	/**
		@brief creates a new guard

		@param [in] locker the Locker to lock (and unlock)
		@param [in] timeout the max time in ms to wait for the Locker
	*/
	LockGuard( Locker &locker, unsigned long timeout=Locker::WAIT_FOREVER ) : m_locker( locker )
	{
		m_locked = locker.lock( timeout );
	}
	/// return true if the Locker is locked by the owner
	bool isLocked( void ) const
	{
		return m_locked;
	}
	/// return true if the Locker is locked by the owner
	operator bool ( void ) const
	{
		return m_locked;
	}
	/// unlocks the Locker
	void unlock( void )
	{
		if( m_locked )
		{
			m_locker.unlock();
			m_locked = false;
		}
	}
	/// unlocks the Locker
	~LockGuard()
	{
		unlock();
	}
};

#if defined( _Windows )
class Critical
{
	friend class CriticalScope;
	CRITICAL_SECTION	m_cs;

public:
	Critical()
	{
		InitializeCriticalSection(&m_cs);
	}
};


class CriticalScope
{
	Critical	&m_cs;

public:
	CriticalScope(Critical	&cs) : m_cs(cs)
	{
		EnterCriticalSection(&m_cs.m_cs);
	}
	~CriticalScope()
	{
		LeaveCriticalSection(&m_cs.m_cs);
	}
};
#endif

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

#endif	// GAK_LOCKER_H
