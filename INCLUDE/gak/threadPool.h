/*
		Project:		GAKLIB
		Module:			threadPool.h
		Description:	
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

#ifndef GAK_THREAD_POOL_H
#define GAK_THREAD_POOL_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/thread.h>
#include <gak/blockedQueue.h>
#include <gak/fixedHeapArray.h>
#include <gak/exception.h>

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum PoolState
{
	psIdle,
	psWaiting,
	psProssesing,
	psStopping
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class ThreadPoolError : public LibraryException
{
	public:
	ThreadPoolError() : LibraryException("ThreadPoolError") {}
};

/**
	@brief helper class that processes an item by calling its operator ()
	@tparam ObjectT the type of the items that can be processed
	@see ThreadPool, PoolThread
*/
template <typename ObjectT>
class ProcessorType
{
	public:
	typedef ObjectT	object_type;
	/**
		@brief processes one item
		@param [in] objectToProcess the item to process
	*/
	static void process( const object_type &objectToProcess )
	{
		objectToProcess();
	}
};

/**
	@brief Base class for all threads managed by ThreadPool

	This class starts a new worker thread that processes the items. 
	
	@tparam ProcessorT the type of the real item processor

	@see ThreadPool, ProcessorType
*/
template <typename ProcessorT>
class PoolThread : public Thread
{
	typedef ProcessorT							processor_type;
	typedef typename ProcessorT::object_type	object_type;

	processor_type	m_objectProcessor;
	object_type		m_objectToProcess;
	enum ThreadMode { tmIdle, tmAquired, tmProcessing } m_mode;
	Thread			*m_dispatcher;

	virtual void ExecuteThread();

	public:
	PoolThread() : Thread(false)
	{
		m_dispatcher = nullptr;
		m_mode = tmIdle;
	}

	/**
		@brief notifies the thread to process the next item (called by the dispatcher thread).
		@param [in] objectToProcess the item to process
		@param [in] dispatcher the dispatcher thread which is notified, after the item was processed
	*/
	void process( const object_type &objectToProcess, Thread *dispatcher )
	{
		assert(m_mode == tmAquired);
		m_objectToProcess = objectToProcess;
		m_dispatcher = dispatcher;
		notify();
	}

	void process( const object_type &objectToProces )
	{
		assert(m_mode != tmProcessing);
		m_mode = tmProcessing;
		m_objectToProcess = objectToProces;
		m_dispatcher = nullptr;
		try
		{
			m_objectProcessor.process( m_objectToProcess );
		}
		catch( ... )
		{
			/// TODO better error handling
		}
		m_mode = tmIdle;
	}

	/// returns true, if this thread is currently waiting for the next item
	bool isIdle() const
	{
		return m_mode == tmIdle ;
	}

	/// returns true, if this thread is currently waiting for the next item and changes mode to aquired (called by the dispatcher thread).
	bool aquire()
	{
		if( isIdle() )
		{
			m_mode = tmAquired;
			return true;
		}
		return false;
	}

	void setObjectProcessor( const processor_type &objectProcessor )
	{
		m_objectProcessor = objectProcessor;
	}
};

/**
	@brief a pool of threads processing input data of a type simultaniously and asynchroniously

	this object creates a number of threads processing the items, a queue with items waiting for processing
	and a dispatcher thread passing these items to a free thread.
	 
	@tparam ObjectT the type of objects that can be processed
	@tparam ThreadT the type of thread that process the objects, default PoolThread<ProcessorType<ObjectT>>

	@see PoolThread, ProcessorType
*/
template <typename ObjectT, typename ThreadT = PoolThread<ProcessorType<ObjectT> > > 
class ThreadPool
{
	typedef FixedHeapArray<ThreadT>	PoolArray;

	class PoolDispatcher : public Thread
	{
		private:
		PoolArray				&m_pool;
		BlockedQueue<ObjectT>	&m_queue;

		public:
		PoolDispatcher( PoolArray &pool, BlockedQueue<ObjectT> &queue ) : Thread(false), m_pool(pool), m_queue(queue)
		{
		}
		virtual void ExecuteThread();

		void StopThread()
		{
			terminated = true;
			m_queue.push( ObjectT() );	// this is a dummy to notify the dispatcher loop, to have a chance to terminate
										// otherwise the dispatcher could wait for a new object and never terminates
										// since terminate is true, the dispatcher loop will not forward that dummy to
										// an object processor
			Thread::StopThread();
		}
	};

	bool					m_singleThreadMode;
	PoolArray				m_pool;
	BlockedQueue<ObjectT>	m_queue;
	PoolDispatcher			m_dispatcher;
	STRING					m_threadNames;
	bool					m_stopping, m_stopped;

	public:
	/**
		@brief creates a new thread pool
		@param count the numnber of worker threads to create
	*/
	ThreadPool( size_t count, const STRING &threadNames )
		: m_singleThreadMode(count==0), m_pool(m_singleThreadMode?1:count),
		  m_dispatcher(m_pool, m_queue), m_threadNames(threadNames), m_stopping(false), m_stopped(true)
	{}
	~ThreadPool()
	{
		shutdown();
	}

	/**
		@brief add a new item to the processing queue
		@param objectToProcess the object to be processed
	*/
	void process( const ObjectT &objectToProcess )
	{
		if( m_stopped )
		{
			throw ThreadPoolError();
		}
		if(m_singleThreadMode)
		{
			m_pool[0].process(objectToProcess);
		}
		else
		{
			m_queue.push( objectToProcess );
		}
	}
	/// waits for all objects in the queue to be processed returns after the last object is processed
	void flush();
	/// starts all threads incl. the dispatcher
	void start();
	/// terminates all threads incl. the dispatcher
	void shutdown();
	/// returns thr total number of items pushed in this queue
	size_t total() const
	{
		return m_queue.total();
	}
	/// returns thr number of items in process
	size_t inProgress() const;

	/// returns thr number of items still waiting in this queue
	size_t size() const
	{
		return m_queue.size() + inProgress();
	}

	template<typename ProcessorT>
	void setObjectProcessor( const ProcessorT &objectProcessor );

	PoolState getCurrentState() const;
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

   
template <typename ProcessorT>
void PoolThread<ProcessorT>::ExecuteThread()
{
	while( !terminated )
	{
		if( wait() && !terminated && m_dispatcher )
		{
			try
			{
				m_objectProcessor.process( m_objectToProcess );
			}
			catch( ... )
			{
				// ignore all errors
			}
			m_mode = tmIdle;
			m_dispatcher->notify();
		}
	}
}

template <typename ObjectT, typename ThreadT>
void ThreadPool<ObjectT, ThreadT>::PoolDispatcher::ExecuteThread()
{
	while( !terminated )
	{
		ObjectT objectToProcess = m_queue.pop();

		bool	processing = false;

		while( !terminated )
		{
			for( 
				typename PoolArray::iterator it = m_pool.begin(), endIT = m_pool.end();
				it != endIT;
				++it
			)
			{
				if( it->aquire() )
				{
					it->process( objectToProcess, this );
					processing = true;
					break;
				}
			}

			if( !processing && !terminated )
			{
				wait( 100 );
			}
			else
			{
				break;
			}
		}
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename ObjectT, typename ThreadT>
void ThreadPool<ObjectT, ThreadT>::start()
{
	if( !m_singleThreadMode && !m_dispatcher.isRunning )
	{
		m_queue.clear();
		for( 
			typename PoolArray::iterator it = m_pool.begin(), endIT = m_pool.end();
			it != endIT;
			++it
		)
		{
			it->StartThread(m_threadNames);
		}
		m_dispatcher.StartThread("PoolDispatcher");
	}
	m_stopped = false;
}

template <typename ObjectT, typename ThreadT>
void ThreadPool<ObjectT, ThreadT>::flush()
{
	while( m_queue.size() )
	{
		Sleep( 10 );
	}
	for( 
		typename PoolArray::iterator it = m_pool.begin(), endIT = m_pool.end();
		it != endIT;
		++it
	)
	{
		while( !it->isIdle() )
		{
			Sleep( 100 );
		}
	}
}

template <typename ObjectT, typename ThreadT>
size_t ThreadPool<ObjectT, ThreadT>::inProgress() const
{
	size_t inProgress = 0;

	for( 
		typename PoolArray::const_iterator it = m_pool.cbegin(), endIT = m_pool.cend();
		it != endIT;
		++it
	)
	{
		if( !it->isIdle() )
		{
			++inProgress;
		}
	}

	return inProgress;
}

template <typename ObjectT, typename ThreadT>
void ThreadPool<ObjectT, ThreadT>::shutdown()
{
	m_stopped = true;
	m_stopping = true;
	flush();
	for(
		typename PoolArray::iterator it = m_pool.begin(), endIT = m_pool.end();
		it != endIT;
		++it
	)
	{
		it->StopThread();
		it->join();
	}
	m_dispatcher.StopThread();
	m_dispatcher.join();
	m_stopping = false;
}

template <typename ObjectT, typename ThreadT>
template<typename ProcessorT>
void ThreadPool<ObjectT, ThreadT>::setObjectProcessor( const ProcessorT &objectProcessor )
{
	for(
		typename PoolArray::iterator it = m_pool.begin(), endIT = m_pool.end();
		it != endIT;
		++it
	)
	{
		it->setObjectProcessor( objectProcessor );
	}
}

template <typename ObjectT, typename ThreadT>
PoolState ThreadPool<ObjectT, ThreadT>::getCurrentState() const
{
	if( m_stopping )
	{
		return psStopping;
	}
	if( m_queue.size() > 0 || inProgress() > 0 )
	{
		return psProssesing;
	}

	for(
		typename PoolArray::const_iterator it = m_pool.cbegin(), endIT = m_pool.cend();
		it != endIT;
		++it
	)
	{
		if( it->isRunning )
		{
			return psWaiting;
		}
	}
	return psIdle;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_THREAD_POOL_H
