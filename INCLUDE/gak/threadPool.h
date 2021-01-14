/*
		Project:		GAKLIB
		Module:			threadPool.h
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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

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
	bool			m_processing;
	Thread			*m_dispatcher;

	virtual void ExecuteThread( void );

	public:
	PoolThread() : Thread(false)
	{
		m_dispatcher = NULL;
		m_processing = false;
	}

	/**
		@brief notifies the thread to process the next item (called by the dispatcher).
		@param [in] objectToProcess the item to process
		@param [in] dispatcher the dispatcher thread which is notified, after the item was processed
	*/
	void process( const object_type &objectToProcess, Thread *dispatcher )
	{
		m_processing = true;
		m_objectToProcess = objectToProcess;
		m_dispatcher = dispatcher;
		notify();
	}

	/// returns true, if this thread is currently waiting for the next item (called by the dispatcher).
	bool isFree() const
	{
		return !m_processing;
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
		PoolDispatcher( PoolArray &pool, BlockedQueue<ObjectT> &queue ) : Thread(false), m_pool( pool ), m_queue( queue )
		{
		}
		virtual void ExecuteThread( void );
		void StopThread()
		{
			terminated = true;
			m_queue.push( ObjectT() );
			Thread::StopThread();
		}
	};


	PoolArray				m_pool;
	BlockedQueue<ObjectT>	m_queue;
	PoolDispatcher			m_dispatcher;

	public:
	/**
		@brief creates a new thread pool
		@param count the numnber of worker threads to create
	*/
	ThreadPool( size_t count ) : m_pool(count), m_dispatcher( m_pool, m_queue )
	{
	}
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
		m_queue.push( objectToProcess );
	}
	/// waits for all objects in the queue to be processed returns after the last object is processed
	void flush();
	/// starts all threads incl. the dispatcher
	void start();
	/// terminates all threads incl. the dispatcher
	void shutdown();

	template<typename ProcessorT>
	void setObjectProcessor( const ProcessorT &objectProcessor );
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
void PoolThread<ProcessorT>::ExecuteThread( void )
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
			m_processing = false;
			m_dispatcher->notify();
		}
	}
}

template <typename ObjectT, typename ThreadT>
void ThreadPool<ObjectT, ThreadT>::PoolDispatcher::ExecuteThread( void )
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
				if( it->isFree() )
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
	if( !m_dispatcher.isRunning )
	{
		m_queue.clear();
		for( 
			typename PoolArray::iterator it = m_pool.begin(), endIT = m_pool.end();
			it != endIT;
			++it
		)
		{
			it->StartThread();
		}
		m_dispatcher.StartThread();
	}
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
		while( !it->isFree() )
		{
			Sleep( 100 );
		}
	}
}

template <typename ObjectT, typename ThreadT>
void ThreadPool<ObjectT, ThreadT>::shutdown()
{
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
