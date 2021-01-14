/*
		Project:		GAKLIB
		Module:			streams.h
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

#ifndef GAK_STREAMS_H
#define GAK_STREAMS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <fstream>
#include <memory>

#include <gak/blockedQueue.h>
#include <gak/thread.h>
#include <gak/shared.h>

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
namespace streams
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

class StreamThread : public Thread
{
	private:
	bool		m_started;
	const char	*m_name;

	public:
	StreamThread() : m_started( false ), m_name(NULL)
	{
	}
	void StartThread()
	{
		if( !m_started )
		{
			Thread::StartThread();
			m_started = true;
		}
	}
	const char *getName() const
	{
		return m_name;
	}
	void setName( const char *name )
	{
		m_name = name;
	}
};

template <typename OBJ>
class Stream;
template <typename OBJ>
class StreamBuffer;

template <typename OBJ>
class StreamBufferImpl : public SharedObject
{
	public:
	typedef Stream<OBJ>			MyStream;
	typedef StreamBuffer<OBJ>	MyStreamBuffer;

	private:
	std::size_t							m_numItems;
	BlockedQueue<OBJ>					m_ioQueue;
	SharedObjectPointer<StreamThread>	m_originatingThread;

	public:
	StreamBufferImpl() : m_numItems(0)
	{
		doEnterFunction("StreamBufferImpl");
		doLogValue( this );
	}
	virtual ~StreamBufferImpl()
	{
		doEnterFunction("~StreamBufferImpl");
		doLogValue( this );
	}
	void setOriginatingThread( const SharedObjectPointer<StreamThread> &thread )
	{
		m_originatingThread = thread;
	}
	virtual void push( const OBJ &item )
	{
		m_ioQueue.push( item );
	}
	virtual OBJ pop()
	{
		while( !eof() )
		{
			try
			{
				++m_numItems;
				return m_ioQueue.pop( 1000 );
			}
			catch( TimeoutException & )
			{
				--m_numItems;
			}
		}
		throw TimeoutException();
	}

	virtual bool eof()
	{
		doEnterFunction("StreamBufferImpl::eof");

		bool threadRunning = m_originatingThread ? m_originatingThread->isRunning : false;
		if( !threadRunning )
		{
			m_originatingThread = NULL;
		}

#ifndef NDEBUG
		size_t	numElements = m_ioQueue.size();
		bool	result = !threadRunning && numElements == 0;

		doLogValue( threadRunning );
		doLogValue( numElements );
		doLogValue( result );

		return result;
#else
		return !threadRunning && m_ioQueue.size() == 0;
#endif
	}
	virtual void setName( const char *name )
	{
	}
	virtual void start()
	{
		if( m_originatingThread )
		{
			m_originatingThread->StartThread();
		}
	}
	virtual void stop()
	{
		if( m_originatingThread )
		{
			m_originatingThread->StopThread();
			m_originatingThread->join();
		}
	}
	virtual void wait()
	{
		if( m_originatingThread )
		{
			m_originatingThread->join();
		}
	}
	size_t getNumItemsProcessed() const
	{
		return m_numItems;
	}
	size_t getNumItemsWaiting() const
	{
		return m_ioQueue.size();
	}
	size_t getNumItems() const
	{
		return getNumItemsProcessed() + getNumItemsWaiting();
	}
};

template <typename OBJ>
class  StreamBuffer : public SharedObjectPointer< StreamBufferImpl<OBJ> >
{
	typedef SharedObjectPointer< StreamBufferImpl<OBJ> >	Super;

	public:
	StreamBuffer( StreamBufferImpl<OBJ> *src=NULL ) : Super( src )
	{
	}
};

template <typename SourceT, typename TargetT>
class DoubleBufferImpl : public StreamBufferImpl<TargetT>
{
	public:
	typedef StreamBuffer<TargetT>	TargetBuffer;
	typedef StreamBuffer<SourceT>	SourceBuffer;

	typedef Stream<TargetT>			TargetStream;
	typedef Stream<SourceT>			SourceStream;

	protected:
	class ConversionThread : public StreamThread
	{
		protected:
		SourceStream	m_source;
		SourceBuffer	m_inputBuffer;
		TargetBuffer	m_outputBuffer;

		ConversionThread() 
		{
			doEnterFunction("DoubleBufferImpl::DoubleBufferImpl::ConversionThread::ConversionThread");
			doLogValue( this );
		}

		bool eof()
		{
#ifndef NDEBUG
			doEnterFunction("DoubleBufferImpl::ConversionThread::eof");

			bool	sourceEOF = m_source->eof();
			bool	bufferEOF = m_inputBuffer->eof();
			bool	result = sourceEOF && bufferEOF;

			doLogValue( sourceEOF );
			doLogValue( bufferEOF );
			doLogValue( result );

			return result;
#else
			return m_source->eof() && m_inputBuffer->eof();
#endif
		}
		virtual void doConversion() = 0;
		void ExecuteThread()
		{
			doEnterFunction("DoubleBufferImpl::ConversionThread::ExecuteThread");
			doLogValue( this );
			if( getName() )
			{
				doLogValue( getName() );
			}

			doConversion();
			m_source = NULL;
			m_inputBuffer = NULL;
			m_outputBuffer = NULL;
		}
		public:
		void setInputBuffer( const SourceBuffer &sourceBuffer )
		{
			assert( sourceBuffer );
			m_inputBuffer = sourceBuffer;
		}
		void setOutputBuffer( const TargetBuffer &outputBuffer )
		{
			assert( outputBuffer );
			m_outputBuffer = outputBuffer;
		}
		void setSource( const SourceStream &source )
		{
			assert( source );
			m_source = source;
		}
	};

	private:
	SharedObjectPointer<ConversionThread>	m_conversionThread;
	SourceStream							m_source;
	SourceBuffer							m_inputBuffer;

	private:
	static SourceBuffer createSourceBuffer()
	{
		doEnterFunction("DoubleBufferImpl::createSourceBuffer");
		return new StreamBufferImpl<SourceT>();
	} 
	protected:
	DoubleBufferImpl( ConversionThread *thread, const SourceStream &source ) : m_conversionThread( thread ), m_source( source )
	{
		m_inputBuffer = createSourceBuffer();
		m_source->registerDestination( m_inputBuffer );
		m_conversionThread->setInputBuffer( m_inputBuffer );
		m_conversionThread->setSource( source );
	}

	public:
	virtual void setName( const char *name )
	{
		m_conversionThread->setName( name );
	}
	virtual void start()
	{
		m_conversionThread->setOutputBuffer( this );

		m_source->start();
		m_conversionThread->StartThread();
	}
	virtual void stop()
	{
		m_source->stop();
		m_conversionThread->StopThread();
		m_conversionThread->join();
	}
	virtual void wait()
	{
		m_source->wait();
		m_conversionThread->join();
	}
	virtual bool eof()
	{
#ifndef NDEBUG
		doEnterFunction("DoubleBufferImpl::eof");

		bool threadRunning = m_conversionThread->isRunning;
		bool superEOF = StreamBufferImpl<TargetT>::eof();
		bool sourceEOF = m_source->eof();
		bool result = !threadRunning && superEOF && sourceEOF;

		doLogValue( threadRunning );
		doLogValue( superEOF );
		doLogValue( sourceEOF );
		doLogValue( result );

		return result;
#else
		return !m_conversionThread->isRunning && StreamBufferImpl<TargetT>::eof() && m_source->eof();
#endif
	}
	SourceBuffer &getInputBuffer()
	{
		return this->m_thread->getInputBuffer();
	}
};

template <typename OBJ>
class SourceBaseImpl : public StreamBufferImpl<OBJ>
{
	typedef typename StreamBufferImpl<OBJ>::MyStreamBuffer	MyStreamBuffer;

	public:
	class SourceThread : public StreamThread
	{
		protected:
		MyStreamBuffer	m_buffer;

		SourceThread()
		{
			doEnterFunction("SourceBaseImpl::SourceThread::SourceThread");
			doLogValue( this );
		}
		virtual void fillBuffer() = 0;
		virtual void ExecuteThread()
		{
			doEnterFunction("SourceBaseImpl::SourceThread::ExecuteThread");
			doLogValue( this );
			if( getName() )
			{
				doLogValue( getName() );
			}

			fillBuffer();
			m_buffer = NULL;
		}

		public:
		void setStreamBuffer( const MyStreamBuffer &buffer )
		{
			m_buffer = buffer;
		}
	};
	private:
	SharedObjectPointer<SourceThread>	m_sourceThread;

	protected:
	SourceBaseImpl( SourceThread *thread ) : m_sourceThread( thread )
	{
	}
	public:
	virtual void setName( const char *name )
	{
		m_sourceThread->setName( name );
	}
	virtual void start()
	{
		m_sourceThread->setStreamBuffer( this );
		m_sourceThread->StartThread();
	}
	virtual void stop()
	{
		m_sourceThread->StopThread();
		m_sourceThread->join();
	}
	virtual void wait()
	{
		m_sourceThread->join();
	}
	virtual bool eof()
	{
#ifndef NDEBUG
		doEnterFunction("SourceBaseImpl::eof");

		bool	sourceRunning = m_sourceThread->isRunning;
		bool	superEOF = StreamBufferImpl<OBJ>::eof();
		bool	result = !sourceRunning && superEOF;

		doLogValue( sourceRunning );
		doLogValue( superEOF );
		doLogValue( result );

		return result;
#else
		return !m_sourceThread->isRunning && StreamBufferImpl<OBJ>::eof();
#endif
	}
};

template <typename ContainerT>
struct ContainerSourceFunction
{
	typedef typename ContainerT::value_type		OBJ;
	typedef typename ContainerT::const_iterator	Iterator;
	
	const ContainerT	&m_container;
	Iterator			m_it, m_endIT;

	ContainerSourceFunction( const ContainerT &container ) : m_container( container )
	{
		m_it = container.cbegin();
		m_endIT = container.cend();
	}
	bool eof() const
	{
		doEnterFunction("ContainerSourceFunction::eof");
		bool result = m_it == m_endIT;
		doLogValue( result );
		return result;
	}
	OBJ getNext()
	{
		return *m_it++;
	}
};

template <typename OBJ>
class FileSourceFunction
{
	SharedPointer<std::ifstream>	m_stream;

	public:
	FileSourceFunction( const STRING &fileName ) : m_stream( SharedPointer<std::ifstream>::makeShared(fileName, std::ios_base::binary) )
	{
	}
	bool eof() const
	{
		doEnterFunction("FileSourceFunction::eof");
		bool result = m_stream->eof() || m_stream->fail();
		doLogValue( result );
		return result;
	}

	OBJ getNext()
	{
		OBJ	item;
		item.fromBinaryStream( *m_stream );
		return item;
	}
};


template <typename OBJ, typename FunctionT>
class FunctionSourceImpl : public SourceBaseImpl<OBJ>
{
	private:
	typedef SourceBaseImpl<OBJ>				Super;
	typedef typename Super::SourceThread	SourceThread;

	class FunctionThread : public SourceThread
	{
		FunctionT	m_sourceFunction;

		virtual void fillBuffer()
		{
			doEnterFunction("FunctionThread::fillBuffer");
			while( !m_sourceFunction.eof() && !this->terminated )
			{
				try
				{
					OBJ	item = m_sourceFunction.getNext();
					doLogValue( item );
					this->m_buffer->push( item );
				}
				catch( ... )
				{
					// ignore
				}
			}
		}
		public:
		FunctionThread( const FunctionT &sourceFunction ) : m_sourceFunction( sourceFunction )
		{
		}
	};

	public:
	FunctionSourceImpl( const FunctionT &sourceFunction ) : Super( new FunctionThread( sourceFunction ) )
	{
	}
};

template <typename OBJ>
class UnionSourceImpl : public StreamBufferImpl<OBJ>
{
	typedef StreamBufferImpl<OBJ>		Super;
	typedef typename Super::MyStream	MyStream;

	MyStream	m_first, m_second;

	public:
	UnionSourceImpl( const MyStream &first, const MyStream &second ) : m_first( first ), m_second( second )
	{
	}
	virtual bool eof()
	{
#ifndef NDEBUG
		doEnterFunction("UnionSourceImpl::eof");

		bool	superEOF = Super::eof();
		bool	firstEOF = m_first->eof();
		bool	secondEOF = m_second->eof();
		bool	result = superEOF && firstEOF && secondEOF;

		doLogValue( superEOF );
		doLogValue( firstEOF );
		doLogValue( secondEOF );
		doLogValue( result );

		return result;
#else
		return Super::eof() && m_first->eof() && m_second->eof();
#endif
	}
	virtual void start()
	{
		m_first->start();
		m_second->start();
	}
	virtual void stop()
	{
		m_first->stop();
		m_second->stop();
	}
	virtual void wait()
	{
		m_first->wait();
		m_second->wait();
	}
};

template <typename SourceT, typename TargetT, typename MapFunctionT >
class MapSourceImpl : public DoubleBufferImpl<SourceT, TargetT>
{
	private:
	typedef DoubleBufferImpl<SourceT, TargetT>	Super;
	typedef typename Super::ConversionThread	ConversionThread;
	typedef typename Super::SourceStream		SourceStream;

	class MapThread : public ConversionThread
	{
		MapFunctionT	m_mapFunction;

		virtual void doConversion()
		{
			doEnterFunction("MapThread::doConversion");
			assert( this->m_source );

			while( !this->eof() && !this->terminated )
			{
				try
				{
					SourceT	item1 = this->m_inputBuffer->pop();
					doLogValue( item1 );
					TargetT	item2 = m_mapFunction( item1 );
					doLogValue( item2 );
					this->m_outputBuffer->push( item2 );
				}
				catch( TimeoutException & )
				{
					doLogPosition();
				}
			}
		}
		public:
		MapThread( const MapFunctionT &mapFunction ) : m_mapFunction( mapFunction )
		{
		}
	};

	static MapThread *createMapThread( const MapFunctionT &mapFunction )
	{
		doEnterFunction("createMapThread");
		return new MapThread( mapFunction );
	}
	public:
	MapSourceImpl( const SourceStream &source, const MapFunctionT &mapFunction )
	: Super( createMapThread( mapFunction ), source )
	{
		doEnterFunction("MapSourceImpl");
		doLogValue( this );
	}
};

template <typename SourceT, typename TargetT, typename FlatMapFunctionT >
class FlatMapSourceImpl : public DoubleBufferImpl<SourceT, TargetT>
{
	private:
	typedef DoubleBufferImpl<SourceT, TargetT>	Super;
	typedef typename Super::ConversionThread	ConversionThread;
	typedef typename Super::SourceStream		SourceStream;

	class FlatMapThread : public ConversionThread
	{
		FlatMapFunctionT	m_flatMapFunction;

		virtual void doConversion()
		{
			assert( this->m_source );
			doEnterFunction("FlatMapThread::doConversion");
			while( !this->eof() && !this->terminated )
			{
				try
				{
					SourceT	item = this->m_inputBuffer->pop();
					doLogValue( item );
					m_flatMapFunction( item, this->m_outputBuffer );
				}
				catch( TimeoutException & )
				{
					doLogPosition();
				}
			}
		}
		public:
		FlatMapThread( const FlatMapFunctionT &flatMapFunction ) : m_flatMapFunction( flatMapFunction )
		{
		}
	};

	static FlatMapThread *createFlatMapThread( const FlatMapFunctionT &flatMapFunction )
	{
		doEnterFunction("createFlatMapThread");
		return new FlatMapThread( flatMapFunction );
	}
	public:
	FlatMapSourceImpl( const SourceStream &source, const FlatMapFunctionT &flatMapFunction ) 
	: Super( createFlatMapThread( flatMapFunction ), source )
	{
		doEnterFunction("FlatMapSourceImpl");
		doLogValue( this );
	}
};

template <typename OBJ, typename FilterFunctionT >
class FilterSourceImpl : public DoubleBufferImpl<OBJ, OBJ>
{
	private:
	typedef DoubleBufferImpl<OBJ, OBJ>			Super;
	typedef typename Super::ConversionThread	ConversionThread;
	typedef typename Super::SourceStream		SourceStream;

	class FilterThread : public ConversionThread
	{
		FilterFunctionT	m_filterFunction;

		virtual void doConversion()
		{
			doEnterFunction("FilterThread::doConversion");
			while( !this->eof() && !this->terminated )
			{
				try
				{
					OBJ	item = this->m_inputBuffer->pop();
					doLogValue( item );
					if( m_filterFunction( item ) )
					{
						doLogValue( item );
						this->m_outputBuffer->push( item );
					}
				}
				catch( TimeoutException & )
				{
					doLogPosition();
				}
			}
		}
		public:
		FilterThread( const FilterFunctionT &filterFunction ) : m_filterFunction( filterFunction )
		{
		}
	};

	static FilterThread *createFilterThread( const FilterFunctionT &filterFunction )
	{
		doEnterFunction("createFilterThread");
		return new FilterThread( filterFunction );
	}
	public:
	FilterSourceImpl( const SourceStream &source, const FilterFunctionT &filterFunction ) 
	: Super( createFilterThread( filterFunction ), source )
	{
		doEnterFunction("FilterSourceImpl");
		doLogValue( this );
	}
};

template <typename BackInserterIteratorT, typename OBJ>
class ContainerSinkFunction
{
	BackInserterIteratorT	m_iterator;

	public:
	ContainerSinkFunction( const BackInserterIteratorT &iterator ) : m_iterator(iterator)
	{
	}

	void operator () ( const OBJ &item )
	{
		m_iterator = item;
	}
};

template <typename OBJ>
class FileSinkFunction
{
	SharedPointer<std::ofstream>	m_stream;

	public:
	FileSinkFunction( const STRING &fileName ) : m_stream( SharedPointer<std::ofstream>::makeShared(fileName, std::ios_base::binary) )
	{
	}

	void operator () ( const OBJ &item )
	{
		item.toBinaryStream( *m_stream );
	}
};

template <typename OBJ, typename FunctionT>
class StreamSinkImpl : public SharedObject
{
	public:
	typedef StreamBuffer<OBJ>		MyStreamBuffer;
	typedef Stream<OBJ>				MyStream;

	class SinkThread : public StreamThread
	{
		FunctionT		m_sinkFunction;
		MyStreamBuffer	m_buffer;

		public:
		SinkThread( const FunctionT &sinkFunction ) : m_sinkFunction(sinkFunction)
		{
			doEnterFunction("StreamSinkImpl::SinkThread::SinkThread");
			doLogValue( this );
		}
		void ExecuteThread()
		{
			doEnterFunction("StreamSinkImpl::SinkThread::ExecuteThread");
			doLogValue( this );
			if( getName() )
			{
				doLogValue( getName() );
			}

			while( !m_buffer->eof() && !terminated )
			{
				doLogPosition();
				OBJ	item = m_buffer->pop();
				doLogValue( item );
				m_sinkFunction( item );
			}
			m_buffer = NULL;
		}
		void setBuffer( const MyStreamBuffer &buffer )
		{
			m_buffer = buffer;
		}
	};

	private:
	MyStream						m_sourceStream;
	MyStreamBuffer					m_streamBuffer;
	SharedObjectPointer<SinkThread>	m_sinkThread;

	public:
	StreamSinkImpl( const MyStream &sourceStream, const MyStreamBuffer &buffer, const FunctionT &sinkFunction) 
		: m_sourceStream(sourceStream), m_streamBuffer(buffer), m_sinkThread( new SinkThread( sinkFunction ) )
	{
	}
	virtual ~StreamSinkImpl()
	{
		assert( !m_sinkThread->isRunning );
	}

	virtual void setName( const char *name )
	{
		m_sinkThread->setName( name );
	}
	virtual void start()
	{
		m_sourceStream->start();
		m_sinkThread->setBuffer( m_streamBuffer );
		m_sinkThread->StartThread();
	}
	virtual void stop()
	{
		m_sourceStream->stop();
		m_sinkThread->StopThread();
		m_sinkThread->join();
	}
	virtual void wait()
	{
		m_sourceStream->wait();
		m_sinkThread->join();
	}
	virtual bool eof()
	{
#ifndef NDEBUG
		doEnterFunction("StreamSinkImpl::eof");

		bool	sinkIsRunning = m_sinkThread->isRunning;
		bool	streamEOF = m_streamBuffer->eof();
		bool	result = !sinkIsRunning && streamEOF;

		doLogValue( sinkIsRunning );
		doLogValue( streamEOF );
		doLogValue( result );

		return result;
#else
		return !m_sinkThread->isRunning && m_streamBuffer->eof();
#endif
	}

	static void printItem( const OBJ &item )
	{
		doEnterFunction( "StreamSinkImpl::SinkThread::printItem" );
		std::cout << item << std::endl;
	}
};

template <typename OBJ, typename FunctionT>
class Sink : public SharedObjectPointer< StreamSinkImpl<OBJ, FunctionT> >
{
	public:
	Sink( StreamSinkImpl<OBJ, FunctionT> *str=NULL ) : SharedObjectPointer< StreamSinkImpl<OBJ, FunctionT> >( str )
	{
	}
};

template <typename OBJ>
class StreamImpl : public SharedObject
{
	public:
	typedef StreamBuffer<OBJ>		MyStreamBuffer;
	typedef Stream<OBJ>				MyStream;
	typedef void (*SinkFunction)(const OBJ &);
	typedef Sink<OBJ, SinkFunction>	SimpleSink;
 
	private:
	typedef Array<MyStreamBuffer>	Destinations;

	class DispatcherThread : public StreamThread
	{
		std::size_t		m_numItems;
		Destinations	&m_destinations;
		MyStreamBuffer	m_buffer;

		public:
		DispatcherThread( Destinations &destinations ) : m_destinations(destinations), m_numItems(0)
		{
			doEnterFunction("StreamImpl::DispatcherThread::DispatcherThread");
			doLogValue( this );
		}
		void ExecuteThread()
		{
			doEnterFunction("StreamImpl::DispatcherThread::ExecuteThread");
			doLogValue( this );
			if( getName() )
			{
				doLogValue( getName() );
			}

			typename Destinations::iterator	startIT = m_destinations.begin(),
											endIT = m_destinations.end();

			if( startIT != endIT && m_buffer )
			{
				while( !m_buffer->eof() && !terminated )
				{
					doLogPosition();
					try
					{
						OBJ	item = m_buffer->pop();
						doLogValue( item );
						++m_numItems;
						for(
							typename Destinations::iterator it = startIT; it != endIT; ++it
						)
						{
							MyStreamBuffer &dest = *it;
							doLogPosition();
							(*it)->push( item );
							doLogPosition();
						}
					}
					catch( TimeoutException & )
					{
						doLogPosition();
					}
				}
				m_destinations.clear();
				m_buffer = NULL;
			}
		}
		void setBuffer( const MyStreamBuffer &buffer )
		{
			doEnterFunction("StreamImpl::DispatcherThread::buffer");
			doLogValue( this );
			doLogValue( buffer );
			m_buffer = buffer;
		}
		std::size_t getNumItems() const
		{
			return m_numItems;
		}
	};

	private:
	MyStreamBuffer							m_streamBuffer;
	SharedObjectPointer<DispatcherThread>	m_dispatcherThread;
	Destinations							m_destinations;

	public:
	StreamImpl( const MyStreamBuffer &buffer ) : m_streamBuffer(buffer), m_dispatcherThread( new DispatcherThread( m_destinations ) )
	{
		doEnterFunction("StreamImpl::StreamImpl");
		doLogValue( this );
	}
	virtual ~StreamImpl()
	{
		assert( !m_dispatcherThread->isRunning );
	}

	void registerDestination( MyStreamBuffer &destination )
	{
		m_destinations.addElement( destination );
		destination->setOriginatingThread( static_cast<StreamThread*>(m_dispatcherThread) );
	}

	void setName( const char *name )
	{
		m_dispatcherThread->setName( name );
		m_streamBuffer->setName( name );
	}
	virtual void start()
	{
		doEnterFunction("StreamImpl::start");
		doLogValue( this );
		m_streamBuffer->start();
		m_dispatcherThread->setBuffer( m_streamBuffer );
		m_dispatcherThread->StartThread();
	}
	virtual void stop()
	{
		doEnterFunction("StreamImpl::stop");
		doLogValue( this );
		m_streamBuffer->stop();
		m_dispatcherThread->StopThread();
		m_dispatcherThread->join();
	}
	virtual void wait()
	{
		doEnterFunction("StreamImpl::wait");
		doLogValue( this );
		m_streamBuffer->wait();
		m_dispatcherThread->join();
	}
	virtual bool eof()
	{
#ifndef NDEBUG
		doEnterFunction("StreamImpl::eof");

		bool	dispatcherIsRunning = m_dispatcherThread->isRunning;
		bool	streamEOF = m_streamBuffer->eof();
		bool	result = !dispatcherIsRunning && streamEOF;

		doLogValue( dispatcherIsRunning );
		doLogValue( streamEOF );
		doLogValue( result );

		return result;
#else
		return !m_dispatcherThread->isRunning && m_streamBuffer->eof();
#endif
	}

	std::size_t getNumItemsProcessed() const
	{
		return m_streamBuffer->getNumItemsProcessed();
	}
	std::size_t getNumItemsWaiting() const
	{
		return m_streamBuffer->getNumItemsWaiting();
	}
	std::size_t getNumItems() const
	{
		return getNumItemsProcessed() + getNumItemsWaiting();
	}
	std::size_t getNumItemsDispatched() const
	{
		return m_dispatcherThread->getNumItems();
	}

	MyStream unionStream( MyStream &other );

	template <typename TargetT, typename MapFunctionT>
	Stream<TargetT> map( const MapFunctionT &mapFunction );

	template <typename TargetT, typename FlatMapFunctionT>
	Stream<TargetT> flatMap( const FlatMapFunctionT &flatMapFunction );

	template <typename FilterFunctionT>
	MyStream filter( const FilterFunctionT &filterFunction );

	template <typename SinkFunctionT>
	Sink<OBJ, SinkFunctionT> addSink( const SinkFunctionT &sinkFunction );

	SimpleSink addConsoleSink();

	template <typename BackInserterIteratorT>
	Sink<OBJ, ContainerSinkFunction<BackInserterIteratorT, OBJ> > addContainerSink( const BackInserterIteratorT &iterator );

	Sink<OBJ, FileSinkFunction<OBJ> > addFileSink( const STRING &fileName );
};

template <typename OBJ>
class Stream : public SharedObjectPointer< StreamImpl<OBJ> >
{
	public:
	typedef typename StreamImpl<OBJ>::SinkFunction	SinkFunction;
	typedef typename StreamImpl<OBJ>::SimpleSink	SimpleSink;

	Stream( StreamImpl<OBJ> *str=NULL ) : SharedObjectPointer< StreamImpl<OBJ> >( str )
	{
	}
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
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename OBJ>
inline Stream<OBJ> StreamImpl<OBJ>::unionStream( MyStream &other )
{
	MyStreamBuffer	newSource = new UnionSourceImpl<OBJ>( MyStream(this), other );
	registerDestination( newSource );
	other->registerDestination( newSource );

	return MyStream( new StreamImpl<OBJ>( newSource ) );
}

template <typename OBJ>
template <typename TargetT, typename MapFunctionT>
inline Stream<TargetT> StreamImpl<OBJ>::map( const MapFunctionT &mapFunction )
{
	doEnterFunction("StreamImpl<SourceT>::map");
	doLogValue("Creating MapSourceImpl");

	doLogValue("Creating Stream");
	return Stream<TargetT>(
		new StreamImpl<TargetT>(
			StreamBuffer<TargetT>(
				new MapSourceImpl<OBJ, TargetT, MapFunctionT>( 
					MyStream(this), mapFunction 
				)
			)
		)
	);
}

template <typename SourceT>
template <typename TargetT, typename FlatMapFunctionT>
inline Stream<TargetT> StreamImpl<SourceT>::flatMap( const FlatMapFunctionT &flatMapFunction )
{
	return Stream<TargetT>(
		new StreamImpl<TargetT>(
			StreamBuffer<TargetT>(
				new FlatMapSourceImpl<SourceT, TargetT, FlatMapFunctionT>( 
					MyStream(this), flatMapFunction 
				)
			)
		)
	);
}

template <typename OBJ>
template <typename FilterFunctionT>
inline Stream<OBJ> StreamImpl<OBJ>::filter( const FilterFunctionT &filterFunction )
{
	return MyStream(
		new StreamImpl<OBJ>(
			MyStreamBuffer(
				new FilterSourceImpl<OBJ, FilterFunctionT>( 
					MyStream(this), filterFunction 
				)
			)
		)
	);
}

template <typename OBJ>
template <typename SinkFunctionT>
inline Sink<OBJ, SinkFunctionT> StreamImpl<OBJ>::addSink( const SinkFunctionT &sinkFunction )
{
	MyStreamBuffer	buffer( new StreamBufferImpl<OBJ>() );
	registerDestination( buffer );

	return Sink<OBJ, SinkFunctionT>(
		new StreamSinkImpl<OBJ, SinkFunctionT>(
			buffer, sinkFunction
		)
	);
}

template <typename OBJ>
inline Sink<OBJ, void (*)(const OBJ &)> StreamImpl<OBJ>::addConsoleSink()
{
	MyStreamBuffer	buffer( new StreamBufferImpl<OBJ>() );
	registerDestination( buffer );

	return SimpleSink(
		new StreamSinkImpl<OBJ, SinkFunction>(
			MyStream(this), buffer, &StreamSinkImpl<OBJ, SinkFunction>::printItem
		)
	);
}

template <typename OBJ>
template <typename BackInserterIteratorT>
inline Sink<OBJ, ContainerSinkFunction<BackInserterIteratorT, OBJ> > StreamImpl<OBJ>::addContainerSink( const BackInserterIteratorT &iterator )
{
	MyStreamBuffer	buffer( new StreamBufferImpl<OBJ>() );
	registerDestination( buffer );

	return Sink<OBJ, ContainerSinkFunction<BackInserterIteratorT, OBJ> >(
		new StreamSinkImpl<OBJ, ContainerSinkFunction<BackInserterIteratorT, OBJ> >(
			MyStream(this), buffer, ContainerSinkFunction<BackInserterIteratorT, OBJ>(
				iterator
			)
		)
	);
}


template <typename OBJ>
inline Sink<OBJ, FileSinkFunction<OBJ> > StreamImpl<OBJ>::addFileSink( const STRING &fileName )
{
	MyStreamBuffer	buffer( new StreamBufferImpl<OBJ>() );
	registerDestination( buffer );

	return Sink<OBJ, FileSinkFunction<OBJ> >(
		new StreamSinkImpl<OBJ, FileSinkFunction<OBJ> >(
			MyStream(this), buffer, FileSinkFunction<OBJ>(
				fileName
			)
		)
	);
}


// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename OBJ, typename FunctionT>
inline Stream<OBJ> makeFunctionStream( const FunctionT &function )
{
	return Stream<OBJ>( 
		new StreamImpl<OBJ>(
			StreamBuffer<OBJ>( 
				new FunctionSourceImpl<OBJ, FunctionT>( 
					function
				) 
			)
		) 
	);
}

template <typename ContainerT>
inline Stream<typename ContainerT::value_type> makeContainerStream( const ContainerT &container )
{
	typedef ContainerSourceFunction<ContainerT>	ContainerFunction;
	typedef typename ContainerT::value_type		OBJ;

	return makeFunctionStream<OBJ, ContainerFunction>(
		ContainerFunction(container)
	);
}

template <typename OBJ>
inline Stream<OBJ> makeFileStream( const STRING &fileName )
{
	typedef FileSourceFunction<OBJ>				FileFunction;

	return makeFunctionStream<OBJ, FileFunction>(
		FileFunction(fileName)
	);
}

}	// namespace streams
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_STREAMS_H
