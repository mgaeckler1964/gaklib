/*
		Project:		GAKLIB
		Module:			socketbuf.cpp
		Description:	stream buffer to read/write from/to a socket
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#include <gak/socketbuf.h>
#include <gak/hostResolver.h>
#include <gak/logfile.h>
#include <gak/htmlParser.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
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

#ifdef _Windows
typedef int socklen_t;
#endif

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

#ifdef _Windows
WSADATA	SocketStreambuf::s_wsaData;
bool SocketStreambuf::s_first = true;
#endif

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

SocketStreambuf::~SocketStreambuf()
{
	if( m_connected )
	{
		disconnect();
	}

	if( m_dataBuffer )
	{
		free( m_dataBuffer );
	}
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void SocketStreambuf::makeBuffer( int bufferSize )
{
	doEnterFunction("SocketStreambuf::makeBuffer");
	if( !m_dataBuffer || bufferSize != m_bufferSize )
	{
		if( m_dataBuffer )
		{
			free( m_dataBuffer );
			m_dataBuffer = NULL;
		}

		while( !m_dataBuffer && bufferSize )
		{
			m_dataBuffer = static_cast<char *>(malloc( bufferSize+1+SocketPutbackSize ));
			if( m_dataBuffer )
			{
				break;
			}
			bufferSize >>= 1;
		}
		m_bufferSize = bufferSize;
	}

	flush();
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

void SocketStreambuf::flush( void )
{
	if( m_dataBuffer )
	{
		char *begin = m_dataBuffer + SocketPutbackSize;
		char *end = m_dataBuffer + m_bufferSize + SocketPutbackSize;

		setg( begin, end, end );
		setp( begin, end );
	}
	else
	{
		setg( NULL, NULL, NULL );
		setp( NULL, NULL );
	}
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int SocketStreambuf::connect( const char *server, int port, int buffersize )
{
	doEnterFunction("SocketStreambuf::connect");

	m_totalTime = m_connectTime = clock();

	m_socketError = 0;

	/*
		init winsock library if not yet done
	*/
	makeBuffer( buffersize );

	XResolver	resolver(server,port);
	/*
		check host ip address
	*/
/*
	if( !m_socketError )
	{
		memset( &socketAddress, 0, sizeof(socketAddress) );
		hostp = gethostbyname( server );
		if( hostp )
			memcpy( &socketAddress.sin_addr, hostp->h_addr, sizeof(socketAddress.sin_addr) );
		else
			socketAddress.sin_addr.s_addr = inet_addr( server );

		socketAddress.sin_family = AF_INET;
		socketAddress.sin_port = htons( uint16(port) );
	}
*/
	/*
		create a socket
	*/
	if( !m_socketError )
	{
		m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if( m_socket == INVALID_SOCKET )
		{
			m_socketError = -1;
		}
	}

	/*
		connect
	*/
	if( !m_socketError )
	{
		m_socketError = ::connect( m_socket, resolver.get(), resolver.getLen() );
	}

	if( !m_socketError )
	{
		m_connected = true;
	}
	else
	{
		m_socketError = WSAGetLastError();
	}

	m_connectTime = clock() - m_connectTime;

	if( !m_connected )
	{
		m_errorText = "Connect failed";
	}
	return m_socketError;
}

int SocketStreambuf::sendData( const char *data, size_t numData )
{
	doEnterFunction("SocketStreambuf::sendData");

	clock_t sendTime = clock();

	m_socketError = 0;
	if( m_connected )
	{
		while( numData )
		{
			int blockSize = numData > 1024*1024 ? 1024*1024 : int(numData);

			int numSentData = ::send( m_socket, data, blockSize, 0 );

			if( numSentData == SOCKET_ERROR )
			{
				m_socketError = WSAGetLastError();
				break;
			}
			numData -= numSentData;
			data += numSentData;
		}
	}
	else
	{
		m_socketError = WSAENOTCONN;

/*@*/	return SOCKET_ERROR;
	}

	m_sendTime += clock()-sendTime;

	if( m_socketError )
	{
		m_errorText = "Send failed";
	}

	return m_socketError;
}

void SocketStreambuf::disconnect( void )
{
	doEnterFunction("SocketStreambuf::disconnect");
	if( m_connected )
	{
#ifdef _Windows
		closesocket( m_socket );
#else
		close( m_socket );
#endif
		m_connected = false;
	}

	m_totalTime = clock() - m_totalTime;
}

STRING SocketStreambuf::getSocketError( void ) const
{
	STRING	errorString;
	if( m_socketError )
	{
#ifdef _Windows
		LPVOID lpMsgBuf;
		errorString = m_errorText;
		errorString += ' ';

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			m_socketError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
		);

		errorString += (const char *)lpMsgBuf;

		LocalFree( lpMsgBuf );
#else
		errorString += formatNumber(m_socketError);
#endif
	}

	return errorString;
}


int SocketStreambuf::underflow( void )
{
	doEnterFunction("SocketStreambuf::underflow");

	char	*base = eback();
	int		count = 0;

	clock_t	receiveTime = clock();

	m_socketError = 0;
	if( m_connected )
	{
		if( m_dataBuffer )
		{
			timeval	timeout;
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;
			fd_set	sockets;

#ifdef __GNUC__
			memset( &sockets, 0, sizeof( sockets ) );
#else
			sockets.fd_count = 0;
#endif
			FD_SET( m_socket, &sockets );

			select( int(m_socket)+1, &sockets, NULL, NULL, &timeout );
			if( FD_ISSET( m_socket, &sockets ) )
			{
				count = ::recv( m_socket, base, m_bufferSize, 0 );
			}
			else
			{
				count = 0;
			}

			if( count > 0 )
			{
				setg( base, base, base+count );
				setp( base, base+count );
			}
			else if( count < 0 )
			{
				m_socketError = WSAGetLastError();;
			}
		}
	}
	else
	{
		m_socketError = WSAENOTCONN;
	}

	if( m_socketError )
	{
		m_errorText = "Receive failed";
	}

	m_receiveTime += clock() - receiveTime;

	return count ? *(unsigned char*)base : EOF;
}

int SocketStreambuf::pbackfail( int c )
{
	char	*ptr = gptr();
	if( ptr > m_dataBuffer )
	{
		--ptr;
		*ptr = char(c);
		setg( ptr, ptr, egptr() );

		return c;
	}

	return EOF;
}

int SocketStreambuf::overflow( int c )
{
	sync();

	if( c != EOF )
	{
		sputc( char(c) );
		gbump(1);       // pptr and gptr must be the same
	}

	return c;
}

int SocketStreambuf::sync( void )
{
	size_t	count;
	int		result = 0;

	count = pptr()-pbase();
	if( count > 0 )
	{
		result = sendData( pbase(), count );
	}

	flush();

	return result;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

int SocketStreambuf::listen( unsigned short port )
{
	SOCKADDR_IN		serverAddress;

	/*
		create a socket
	*/
	if( !m_socketError )
	{
		m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		if( m_socket == INVALID_SOCKET )
		{
			m_socketError = -1;
		}
	}

	if( !m_socketError )
	{
		memset( &serverAddress, 0, sizeof(serverAddress) );
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons( port );

		m_socketError = bind( m_socket, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress) );
	}

	if( !m_socketError )
	{
		m_socketError = ::listen( m_socket, SOMAXCONN );
	}

	if( !m_socketError )
	{
		m_connected = true;
	}

	if( m_socketError )
	{
		m_socketError = WSAGetLastError();
		m_errorText = "Listen failed";
	}

	return m_socketError;
}

ClientConnection SocketStreambuf::accept( void )
{
	ClientConnection	connection;

	timeval				timeout = { 10, 0 };
	fd_set				sockets;

#ifdef __GNUC__
	memset( &sockets, 0, sizeof( sockets ) );
#else
	sockets.fd_count = 0;
#endif
	FD_SET( m_socket, &sockets );
	select( int(m_socket)+1, &sockets, NULL, NULL, &timeout );
	if( FD_ISSET( m_socket, &sockets ) )
	{
		socklen_t		clientSize = sizeof( connection.m_client );
		connection.m_socket = ::accept(
			m_socket, reinterpret_cast<SOCKADDR*>(&connection.m_client), &clientSize
		);
	}
	else
	{
		connection.m_socket = INVALID_SOCKET;
	}

	return connection;
}

STRING SocketStreambuf::getNextLine( void )
{
	int		c;
	STRING	theLine;

	while( (c=getNextByte()) != -1 )
	{
		if( c == '\r' )
		{
			// ignore CR
		}
		else if( c == '\n' )
		{
			break;
		}
		else
		{
			theLine += char(c);
		}
	}

	return theLine;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


}	// namespace net
}	// namespace gak

