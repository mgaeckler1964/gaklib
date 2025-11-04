/*
		Project:		GAKLIB
		Module:			THREAD.H
		Description:	The Threads
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

#ifndef GAK_THREAD_H
#define GAK_THREAD_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if defined( _Windows )
#include <gak/win_tools.h>
#endif

#include <gak/gaklib.h>
#include <gak/array.h>
#include <gak/shared.h>
#include <gak/locker.h>
#include <gak/conditional.h>

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

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief abstract base class for a program thread

	overload virtual function void ExecuteThread(); 
*/
class Thread : public SharedObject
{
	public:
#if defined( _Windows )
	typedef DWORD		T_RETURN;
	typedef LPVOID		T_PARAM;
	typedef HANDLE		P_HANDLE;
#elif defined( __MACH__ ) || defined( __unix__ )
	typedef void *		T_RETURN;
	typedef void *		T_PARAM;
	typedef pthread_t	P_HANDLE;
	#define WINAPI		/* */
#endif

	private:
	ThreadID	m_threadID, m_ownerThreadID;
	Conditional	m_theConditional;
	STRING		m_name;

#if defined( _Windows )
	windows::WinHandle	theThreadHandle;
#endif

	static Array< SharedObjectPointer<Thread> >	theThreadList;
	static Locker								theThreadListLocker;

	private:
	void RunThread();

	static T_RETURN WINAPI RunThreadCallback( T_PARAM data );

	virtual void ExecuteThread() = 0;

	protected:
	/**
		@brief stop current execution until another thread terminates
		@param [in] other the other thread's object to wait for
	*/ 
	void join( const Thread *other )
	{
		isWaiting = true;
		if( other->isRunning )
		{
			other->join();
		}

		isWaiting = false;
	}
	/**
		@brief stop current execution until a Locker is locked
		@param [in] locker the Locker to wait for
		@param [in] timeOut the max time in ms to wait for the locker
		@return true if locker is locked
	*/
	bool waitForLocker( Locker &locker, unsigned long timeOut=Locker::WAIT_FOREVER )
	{
		isWaiting = true;
		bool result = locker.lock( timeOut );
		isWaiting = false;

		return result;
	}
	/**
		@brief stop current execution until the thread is notified
		@param [in] timeOut the max time in ms to wait for the locker
		@return true if the thread should terminate
	*/
	bool Sleep( unsigned long timeOut )
	{
		isWaiting = true;
		m_theConditional.wait( timeOut );
		isWaiting = false;
		return terminated;
	}
	/**
		@brief stop current execution until the thread is notified
		@param [in] timeOut the max time in ms to wait for the locker
		@return true if the thread was notified
	*/
	bool wait( unsigned long timeOut=Conditional::WAIT_FOREVER )
	{
		isWaiting = true;
		bool result = m_theConditional.wait(timeOut);
		isWaiting = false;
		return result;
	}
	public:

	/// notifies the thread
	void notify()
	{
		m_theConditional.notify();
	}

	public:
	/// true if the thread should terminate
	bool terminated;
	/// true if the thread is executing
	bool isRunning;
	void clrRunning()
	{
		isRunning = false;
	}
	void setRunning()
	{
		isRunning = true;
	}
	/// true if the thread is waiting for a notification, Locker or another Thread
	bool isWaiting;

	private:
	// no copy
	Thread( const Thread &src );
	const Thread & operator = ( const Thread &src );

	public:
	Thread( bool autoDelete = true );
	virtual ~Thread()
	{
		assert( !isRunning );
	}

	/// Starts execution of the thread
	void StartThread( const STRING &name=STRING(), bool hideOwner=false );

	/**
		@brief stops execution of the thread.
		@param [in] stopImmediately if set to true, the thread will be killed immediately, otherwise it's up to the thread to terminate
	*/
	void StopThread( bool stopImmediately = false );

	/// return the number of currently existing threads
	static size_t GetThreadCount()
	{
		return theThreadList.size();
	}

	/**
		@brief check all thread whether they are running

		all threads that are terminated will be destroyed

		@param [in] ownThreads set to true, if list should retrieve the thread handles started by the current thread
		@param [out] list an array containg all running threads
		@return the number of currently running threads
	*/
	static size_t CheckThreadCount(
		bool ownThreads = false, Array<P_HANDLE> *list = NULL
	);

	/// @copydoc join( const Thread *other )
	static void joinOtherThread( const Thread *other )
	{
		SharedObjectPointer<Thread>	myThread=FindCurrentThread();
		if( myThread )
		{
			myThread->join( other );
		}
		else
		{
			other->join();
		}
	}
	/**
		@brief stops execution until all other threads terminate
		@param [in] timeOut the maximum time to wait for the other thread
		@param [in] ownThreads if true, wait for the threads started by this thread only

		@return true, if all threads are terminated
	*/
	static bool waitForThreads( unsigned long timeOut=-1, bool ownThreads = true );
#ifdef _Windows
	/// stops execution until onhe of the threads retrieves a windows message (Windows, only)
	static bool waitForMsgThreads( unsigned long timeOut=-1, bool ownThreads=true );
#endif

	/**
		@brief return a thread object
		@param [in] pos index of the required thread object
	*/
	static SharedObjectPointer<Thread> &GetThread( size_t pos )
	{
		return theThreadList[pos];
	}
	/**
		@brief return a thread object
		@param [in] threadID the OS handle of the thread to search for
	*/
	static SharedObjectPointer<Thread> FindThread( ThreadID threadID );
	/**
		@brief return a thread of the current thread
	*/
	static SharedObjectPointer<Thread> FindCurrentThread()
	{
		return FindThread( Locker::GetCurrentThreadID() );
	}

	/// returns the OS handle of the thread
	ThreadID getThreadID() const
	{
		return m_threadID;
	}

	/// returns the name of the thread
	const STRING getName() const
	{
		return m_name;
	}

	/// stops execution until the thread terminates
	void join() const
	{
		if( isRunning )
		{
#ifdef _Windows
			WaitForSingleObject( theThreadHandle.get(), INFINITE );
#else
			pthread_join( m_threadID, NULL );
#endif
		}
		// otherwise ~SharedObject crashes for not auto deleting thread objects 
		CheckThreadCount();
	}

#ifdef _Windows
	/// returns the time of the last user input (Windows, only)
	static unsigned long GetLastInputTime()
	{
		LASTINPUTINFO	lastInputInfo;
		unsigned long	lastPeriod;

		lastInputInfo.cbSize = sizeof( lastInputInfo );
		GetLastInputInfo( &lastInputInfo );
		lastPeriod = GetTickCount()-lastInputInfo.dwTime;

		return lastPeriod;
	}
	/// stops execution, until there is nhew user input waiting (Windows, only)
	bool waitForUserSleep( unsigned long timeOut );
#endif

	/// @todo unix port
	static unsigned getNumberOfCores()
	{
#ifdef _Windows
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;
#else
		return 1;
#endif
	}

	static bool isMainThread()
	{
		SharedObjectPointer<Thread> cur = FindCurrentThread();
		return !cur;
	}
};

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

#endif	// GAK_THREAD_H
