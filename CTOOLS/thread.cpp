/*
		Project:		GAKLIB
		Module:			THREAD.CPP
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <time.h>

#include <gak/thread.h>

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
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
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

Array<Thread::Ptr>	Thread::s_ThreadList;
Locker				Thread::s_ThreadListLocker;

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

Thread::Thread( bool autoDelete ) : SharedObject( autoDelete )
{
	CheckThreadCount();		// remove lo longer used threads
#ifndef _Windows
	m_detached = true;
#endif
	terminated = false;
	isWaiting = false;
	isRunning = false;
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

Thread::T_RETURN WINAPI Thread::RunThreadCallback( T_PARAM data )
{
	Thread	*theThread = static_cast<Thread*>(data);

	theThread->RunThread();

	doLogPositionEx( gakLogging::llInfo );
	return 0;
}

size_t Thread::CheckThreadCount( bool ownThreads, Array<P_HANDLE> *list )
{
	size_t		i, otherThreads;
	ThreadID	threadID = ownThreads ? Locker::GetCurrentThreadID() : 0;

	otherThreads = i = 0;

	{
		LockGuard	lock( s_ThreadListLocker );

		if( lock )
		{
			while( i<s_ThreadList.size() )
			{
				Ptr &theThread = GetThread( i );
				if( !theThread->isRunning )
				{
					theThread = NULL;
					s_ThreadList.removeElementAt( i );
				}
				else
				{
					if( threadID && theThread->m_ownerThreadID != threadID )
						otherThreads++;
					else if( list )
#ifdef _Windows
						*list += theThread->theThreadHandle.get();
#else
						*list += theThread->m_threadID;
#endif
					i++;
				}
			}
		}
	}

	return s_ThreadList.size() - otherThreads;
}

Thread::Ptr Thread::FindThread( ThreadID threadID, size_t *idx )
{
	Ptr		result;
	size_t	numThreads;
	bool	found = false;

	if( idx ) *idx = s_ThreadList.no_index;
	{
		LockGuard	lock( s_ThreadListLocker, 10000 );

		if( lock )
		{
			numThreads = s_ThreadList.size();

			for( size_t i=0; i<numThreads; i++ )
			{
				result = GetThread( i );
				if( result->getThreadID() == threadID )
				{
					if( idx ) *idx = i;
					found = true;
/*v*/				break;
				}
			}
		}
	}

	if( !found )
		result = NULL;

	return result;
}

bool Thread::waitForThreads( unsigned long timeOut, bool ownThreads )
{
	bool			result;
	Array<P_HANDLE>	myThreads;

	size_t			numThreads = CheckThreadCount( ownThreads, &myThreads );

	result = (numThreads == 0);
	if( !result )
	{
#ifdef _Windows
		unsigned long status = WaitForMultipleObjects(
			DWORD(numThreads), myThreads.getDataBuffer(), true, timeOut
		);

		result = (status == WAIT_OBJECT_0);
#else
		for( size_t i=0; i<myThreads.size(); i++ )
		{
			result = pthread_join( myThreads[i], NULL );
			if( result )
/*v*/			break;
		}			
#endif
	}

	return result;
}

#ifdef _Windows
bool Thread::waitForMsgThreads( unsigned long timeOut, bool ownThreads )
{
	Array<HANDLE>	myThreads;

	size_t			numThreads = CheckThreadCount( ownThreads, &myThreads );

	if( numThreads )
	{
		MsgWaitForMultipleObjects(
			DWORD(numThreads), (LPHANDLE)myThreads.getDataBuffer(), false, timeOut, QS_ALLINPUT
		);
	}


	myThreads.clear();
	numThreads = CheckThreadCount( ownThreads, &myThreads );
	return numThreads == 0;
}
#endif

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void Thread::RunThread()
{
	setRunning();
	try
	{
		ExecuteThread();
	}
	catch( ... )
	{
		// ignore any exceptions
		std::cerr << "Thread Crash " << FindCurrentThreadIdx() << std::endl;
		doLogMessageEx(gakLogging::llFatal,"Thread Crash");
	}
	clrRunning();
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

void Thread::StartThread( const STRING &name, bool hideOwner )
{
	CheckThreadCount();		// remove lo longer used threads

	if( isRunning )
	{
		return;				// do not start a thread twice
	}

	terminated = false;
	isWaiting = false;
	setRunning();

	m_ownerThreadID = hideOwner ? -1 : Locker::GetCurrentThreadID();
	{
		LockGuard lock( s_ThreadListLocker, 10000 );
		if( lock )
		{
			m_name = name;
			s_ThreadList += Ptr(this);
		}
	}

	#if (defined( __BORLANDC__ ) &&!defined( __MT__ )) || defined( NOTHREADS )
		RunThread();
	#else

		#if defined( _Windows )
			theThreadHandle = CreateThread(
				NULL, 1024,
				RunThreadCallback,
				(LPVOID)this,
				0,
				&m_threadID
			);
		#elif defined( __MACH__ ) || defined( __unix__ )
			pthread_create( &m_threadID, NULL, RunThreadCallback, this );
			m_detached = false;
		#endif
	#endif
}

void Thread::StopThread( bool stopImmediately )
{
	CheckThreadCount();		// remove lo longer used threads

	terminated = true;

	m_theConditional.notify();

	if( stopImmediately )
	{
#if defined( _Windows )
		::TerminateThread(theThreadHandle.get(), 0 );
#elif defined( __MACH__ ) || defined( __unix__ )
		pthread_cancel( m_threadID );
#endif
		clrRunning();
	}
}

#ifdef _Windows
bool Thread::waitForUserSleep( unsigned long timeOut )
{
	bool			ok;
	LASTINPUTINFO	lastInputInfo;
	unsigned long	lastPeriod;

	lastInputInfo.cbSize = sizeof( lastInputInfo );
	while( !terminated )
	{
		ok = GetLastInputInfo( &lastInputInfo );
		if( ok )
		{
			lastPeriod = GetTickCount()-lastInputInfo.dwTime;
			if( lastPeriod > timeOut )
/*v*/			break;
			Sleep( timeOut - lastPeriod );
		}
		else
			Sleep( 1000 );
	}
	return terminated;
}
#endif
// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

