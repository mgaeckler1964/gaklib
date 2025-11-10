/*
		Project:		GAKLIB
		Module:			socketServer.h
		Description:	Base for any TCP/IP server
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/socketbuf.h>
#include <gak/threadPool.h>

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
namespace net
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
	@brief abstract base for all socket processors

	the objects are created by the listener's thread pool.
	to implement your own server derive a new class from this
	base and implement the virtual function runServerThread. See 
	HTTPserverBase for an example.

	@see SocketServer, HTTPserverBase
*/
class ServerProcessorBase : public SocketStreambuf
{
	SOCKADDR_IN			m_client;
	unsigned short		m_port;

	virtual void runServerThread( void ) = 0;
	public:
	/// this type is used by PoolThread
	typedef ClientConnection object_type;

	/// this type is called by PoolThread
	void process( const ClientConnection &connection, void *threadPool, void *mainData )
	{
		accept( connection.m_socket, 10240 );
		m_client = connection.m_client;
		m_port = connection.m_port;
		runServerThread();
		disconnect();
	}
	/// returns the client with this worker is connected to
	const SOCKADDR_IN &getClient( void ) const
	{
		return m_client;
	}
	/// returns the IP port number of the corresponding listener
	unsigned short getPort() const
	{
		return m_port;
	}
};

/**
	@brief a server that listens on IP ports

	The passed class type must implement a function void process( const ClientConnection &connection )
	that is called for each incomming connection. The class also must define a typedef for the ClientConnection:

	@code
	typedef ClientConnection object_type;
	@endcode

	A good idea is to use ServerProcessorBase as a base for your own implementation

	@tparam ServerProcessorT the class that can handle the incomming connections
	@see ServerProcessorBase, ClientConnection, HTTPserverBase
*/
template <typename ServerProcessorT>
class SocketServer
{
	class ListenerThread : public Thread
	{
		unsigned short		m_port;
		size_t				m_numWorker;
		SocketStreambuf		m_socket;

		virtual void ExecuteThread( void );
		public:
		ListenerThread( unsigned short port, size_t numWorker ) 
		: Thread(), m_port(port), m_numWorker(numWorker)
		{
			StartThread();
		}
		STRING	getSocketError( void ) const
		{
			return m_socket.getSocketError();
		}
	};

	unsigned short						m_port;
	SharedObjectPointer<ListenerThread>	m_listener;

	public:
	/// returns true, if the server is enabled and running
	bool isRunning( void ) const
	{
		return bool(m_listener) && m_listener->isRunning;
	}
	/**
		@brief starts the server
		@param [in] port the tcp/ip port this server should listen to
		@param [in] numWorker the number of worker threads, the listener should start
	*/
	void startServer( unsigned short port, size_t numWorker )
	{
		if( m_listener )
		{
			stopServer( false );
		} 
		m_listener = new ListenerThread( port, numWorker );
	}

	/// stops this server
	void stopServer( bool killed )
	{
		if( m_listener )
		{
			m_listener->StopThread();
			if( !killed )
			{
				m_listener->join();
				m_listener = NULL;
			}
		}
	}

	/// returns the port number this server is listening to
	unsigned short getPort( void ) const
	{
		return m_port;
	}

	/// returns the last socket error if available
	STRING	getSocketError( void ) const
	{
		return m_listener ? m_listener->getSocketError() : NULL_STRING;
	}
};

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

template <typename ServerProcessorT>   
void SocketServer<ServerProcessorT>::ListenerThread::ExecuteThread( void )
{
	doEnterFunction("SocketServer<ServerProcessorT>::ListenerThread::ExecuteThread");

	ThreadPool<ClientConnection, PoolThread<ServerProcessorT> >	threadPool( m_numWorker, "ServerPool", nullptr );

	if( !m_socket.listen( m_port ) )
	{
		threadPool.start();
		terminated = false;
		while( !terminated )
		{
			ClientConnection	connection = m_socket.accept();

			if( connection.m_socket != INVALID_SOCKET )
			{
				connection.m_port = m_port;
				threadPool.process(connection);
			}
		}
		threadPool.shutdown();
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

#endif
