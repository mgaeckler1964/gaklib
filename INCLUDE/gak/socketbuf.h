/*
		Project:		GAKLIB
		Module:			socketbuf.h
		Description:	stream buffer to read/write from/to a socket
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

#ifndef SOCKET_BUF_H
#define SOCKET_BUF_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>

#include <time.h>
#if defined( __BORLANDC__ ) || defined( _MSC_VER )
#include <winsock.h>
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <arpa/inet.h> 
#endif

#include <gak/string.h>

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
namespace net
{
// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

const int SocketPutbackSize = 8;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

#ifndef _Windows
typedef int					SOCKET;
typedef struct sockaddr_in	SOCKADDR_IN;
typedef struct sockaddr		SOCKADDR;

#define INVALID_SOCKET		-1
#define SOCKET_ERROR		-1
#define WSAENOTCONN			ENOTCONN
#define WSAGetLastError()	(errno)
#endif

/// contains all data to describe a client connection @see SocketStreambuf::accept()
struct ClientConnection
{
	/// the address of the client connected to this server
	SOCKADDR_IN		m_client;
	/// the socket number that can be used to read and write data from/to the client @see SocketStreambuf::accept( SOCKET socket, int bufferSize )
	SOCKET 			m_socket;
	/// the port number the server is listening to
	unsigned short	m_port;
};

/// streambuf used to read from an ip socket
class SocketStreambuf : public std::streambuf
{
#ifdef _Windows
	static bool		s_first;
	static WSADATA	s_wsaData;
#endif

	SOCKET		m_socket;
	bool		m_connected;
	int			m_bufferSize;
	char		*m_dataBuffer;

	protected:

	/// an error code of the last error
	int			m_socketError;
	/// an error text of the last error
	STRING		m_errorText;

	/// @name Some time metrics:
	///@{
	clock_t		m_totalTime,	//!< total execution time
				m_connectTime,	//!< time to connect
				m_sendTime,		//!< time to send data
				m_receiveTime;	//!< time to receive data
	///@}

	/**
		@brief the number of bytes read
		@todo make private and documentation clear
	*/
	size_t		m_bytesRead;

	private:
	// do nothing, just forbid copying
	SocketStreambuf( const SocketStreambuf &aStream );
	SocketStreambuf &operator = ( const SocketStreambuf & );

	void		makeBuffer( int bufferSize );
	virtual int overflow( int c );
	virtual int pbackfail( int c );
	int			sync( void );

	protected:
	/// fill the buffer from the socket stream
	virtual int underflow( void );

	/// clears all data from the I/O buffer
	void flush( void );

	/// returns the OS id of the socket
	int getSocket( void ) const
	{
		return int(m_socket);
	}

	/// returns the size of the currently used I/O buffer
	int getBufferSize( void ) const
	{
		return m_bufferSize;
	}

	public:
	/// creates a new socket buffer
	SocketStreambuf()
	{
		m_totalTime = m_connectTime = m_sendTime = m_receiveTime = 0;

		m_dataBuffer = NULL;
		m_bytesRead = m_bufferSize = 0;
		m_socket = 0;
		m_connected = false;

#ifdef _Windows
		if( s_first )
		{
//			m_socketError = WSAStartup( (MAKEWORD( (2),(0) )), &s_wsaData );
			m_socketError = WSAStartup( 0x0002, &s_wsaData );
			s_first = false;
		}
		else
#endif
		{
			m_socketError = 0;
		}
	}
	virtual ~SocketStreambuf();

	/**
		@brief opens a new client connection to a server
		@param [in] server the hostname of the server
		@param [in] port the IP port number
		@param [in] buffersize the size of the I/O buffer
		@return 0 on success otherwise an error code
	*/
	virtual int connect( const char *server, int port, int buffersize=10240 );

	/**
	*/
	/**
		@brief opens a listener connection
		@return 0 on success otherwise an error code
	*/
	int listen(unsigned short port);

	/**
		@brief opens a worker connection
		@return a client connection for use with accept( SOCKET socket, int bufferSize ) from a worker
		@see accept( SOCKET socket, int bufferSize )
	*/
	ClientConnection accept();

	/**
		@brief retrieves an opened ClientConnection for a worker provided by a listener
		@see accept()
	*/
	void accept( SOCKET socket, int bufferSize )
	{
		disconnect();
		makeBuffer( bufferSize );
		m_socket = socket;
		m_connected = socket != 0L;
	}

	/**
		@brief send data to the connection
		@param [in] data the input buffer
		@param [in] numData the number of bytes to send
		@return 0 on success otherwise an error code
	*/
	virtual int sendData( const char *data, size_t numData );

	/// returns the next byte from the connection
	int getNextByte( void )
	{
		m_bytesRead++;
		return sbumpc();
	}
	/// returns the next text line from the connection
	STRING getNextLine( void );

	/// closes the connection
	virtual void disconnect( void );

	/// returns a text describing the last error
	virtual STRING	getSocketError( void ) const;

	/// returns the time to connect
	clock_t getConnectTime( void ) const
	{
		return m_connectTime;
	}
	/// returns the time to send data
	clock_t getSendTime( void ) const
	{
		return m_sendTime;
	}
	/// returns the time to receive data
	clock_t getReceiveTime( void ) const
	{
		return m_receiveTime;
	}
	/// returns the total time 
	clock_t getTotalTime( void ) const
	{
		return m_totalTime;
	}

	/// returns the number of bytes read
	size_t getBytesRead( void ) const
	{
		return m_bytesRead;
	}

	/// returns true if this buffer is connected to a socket
	bool isConnected( void ) const
	{
		return m_connected;
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
   
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -p.
#	pragma option -a.
#endif

#endif
