/*
		Project:		GAKLIB
		Module:			sslSocket.cpp
		Description:
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

#include <gak/sslSocket.h>
#include <gak/gaklib.h>
#include <gak/fmtNumber.h>
#include <gak/strFiles.h>
#include <gak/directory.h>

#if USE_SSL

#include <openssl/err.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{
namespace net
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const char CA_LIST[]			= "root.pem";
static const char DEF_KEYFILE[]		= "client.pem";
static const char DEF_PASSWORD[]	= "password";

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

static char *pass = NULL;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

/*The password code is not thread safe*/
static int password_cb(char *buf,int num, int rwflag,void *userdata)
{
	int len;

	if( !pass || num < (len=int(strlen(pass)))+1 )
		return 0;
	strcpy( buf, pass );

	return len;
}
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


SSL_LIB_ERROR SSLsocketStreambuf::initialize_ctx( void )
{
	char		progPath[10240];
	const char	*kf;
	STRING		theKeyfile, caFile;
	static bool first = true;

#ifdef _Windows
	GetModuleFileName( NULL, progPath, 10240 );	
#else
	strcpy( progPath, "." );
#endif
	m_sslLibraryError = SSL_NO_ERROR;	
	/* Global system initialization*/	

	if( first )
	{
		SSL_library_init();
		SSL_load_error_strings();
		first = false;
	}
	/* Create our context*/
	m_ctx=SSL_CTX_new(SSLv23_method());

	/* Load our keys and certificates*/
	if( !m_keyfile[0U] )
		kf = DEF_KEYFILE;
	else
		kf = m_keyfile;

	if( strAccess( kf, 00 ) )
		theKeyfile = makeFullPath( progPath, kf );
	else
		theKeyfile = kf;
	if( strAccess( CA_LIST, 00 ) )
		caFile = makeFullPath( progPath, CA_LIST );
	else
		caFile = CA_LIST;

	if( SSL_CTX_use_certificate_chain_file(m_ctx, theKeyfile) )
	{
		pass=m_password;

		SSL_CTX_set_default_passwd_cb( m_ctx, password_cb );
		if( SSL_CTX_use_PrivateKey_file( m_ctx, theKeyfile, SSL_FILETYPE_PEM ) )
		{
			/* Load the CAs we trust*/
			if( SSL_CTX_load_verify_locations(m_ctx, caFile,0) )
			{
#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
				SSL_CTX_set_verify_depth(ctx,1);
#endif
			}
			else
				m_sslLibraryError = BAD_CA_LIST;
		}
		else
			m_sslLibraryError = BAD_KEY_FILE;
	}
	else
		m_sslLibraryError = BAD_CERT_FILE;

	return m_sslLibraryError;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int SSLsocketStreambuf::connect( const char *server, int port, int bufferSize )
{
	STRING	theServer = server;
	int		thePort = port;
	bool	useProxy = false;
	STRING	proxyRequest;
	int		sslStatus;

	if( m_proxy[0U] && strcmpi( theServer, "localhost" ) )
	{
		theServer = m_proxy;
		thePort = m_proxyPort;
		useProxy = true;
		proxyRequest = "CONNECT ";
		proxyRequest += server;
		proxyRequest += ':';
		proxyRequest += formatNumber( port );
		proxyRequest += " HTTP/1.1\r\n\r\n";
	}

	m_sslLibraryError = initialize_ctx();

#ifndef NDEBUG
	std::cout << __FILE__ << __LINE__ << ' ' << m_sslLibraryError << "==" << SSL_NO_ERROR << " SSL-Version: " << std::hex << OPENSSL_VERSION_NUMBER << std::dec << '?' << std::endl;
#endif

	if( m_sslLibraryError == SSL_NO_ERROR )
	{
		if( !SocketStreambuf::connect( theServer, thePort, bufferSize ) )
		{
			if( useProxy )
			{
				STRING	result;

				SocketStreambuf::sendData( proxyRequest, proxyRequest.strlen() );
				SocketStreambuf::getNextLine();
				flush();
			}
			m_ssl=SSL_new( m_ctx );
			m_sbio=BIO_new_socket( getSocket(), BIO_NOCLOSE );
			SSL_set_bio( m_ssl, m_sbio, m_sbio );
			if( (sslStatus = SSL_connect( m_ssl )) > 0 )
			{
//				if( require_server_auth )
//				  check_cert( ssl, server );
			}
			else
			{
				m_sslLibraryError = SSL_LAYER_ERROR;
				m_sslLayerError = SSL_get_error(m_ssl,sslStatus);
			}
		}
		else
		{
			m_sslLibraryError = SSL_SOCKET_ERROR;
		}
	}
#ifndef NDEBUG
	if( m_sslLibraryError )
	{
		const char *file;
		int line;
		const char *data;
		int flags;
		unsigned long err = ERR_get_error_line_data(&file, &line, &data, &flags );
        while( err )
		{
			std::cout << __FILE__ << __LINE__ << " Status: " << sslStatus << " LayerError: " << m_sslLayerError << " queue: " << std::hex << err << std::dec << 
				' ' << file << ' ' << line << std::endl;
			err = ERR_get_error_line_data(&file, &line, &data, &flags );
		}
		m_errorText = "Connect failed";
	}
#endif

	return m_sslLibraryError;
}

int SSLsocketStreambuf::sendData( const char *data, size_t numData )
{
	if( m_ssl )
	{
		clock_t			sendTime = clock();

		m_sslLibraryError = SSL_NO_ERROR;

		while( numData )
		{
			int blockSize = numData > 1024*1024 ? 1024*1024 : int(numData);

			int numSentData = SSL_write( m_ssl, data, blockSize );

			if( numSentData <= 0 )
			{
				m_sslLayerError = SSL_get_error( m_ssl, numSentData );
				m_sslLibraryError = SSL_LAYER_ERROR;
				break;
			}
			numData -= numSentData;
			data += numSentData;
		}

		m_sendTime += clock()-sendTime;
		if( m_sslLibraryError )
		{
			m_errorText = "Send failed";
		}

		return m_sslLibraryError;
	}
	else
	{
		return SocketStreambuf::sendData( data, numData );
	}
}

int SSLsocketStreambuf::underflow( void )
{
	if( !m_ssl )
	{
/***/	return SocketStreambuf::underflow();
	}

	int		count = 0;
	char	*base = eback();

	clock_t receiveTime = clock();

	if( isConnected() && base )
	{
		count=SSL_read( m_ssl, base, getBufferSize() );
		if( count >= 0 )
		{
			setg( base, base, base+count );
			setp( base, base + count );
		}
		else if( count < 0 )
		{
			m_errorText = "Receive failed";
		}
	}

	m_receiveTime += clock() - receiveTime;

	return count ? *base : EOF;
}

void SSLsocketStreambuf::disconnect( void )
{
	if( m_ssl )
	{
		SSL_shutdown(m_ssl);
		SSL_free(m_ssl);
		m_ssl = nullptr;
	}

	if( m_ctx )
	{
		SSL_CTX_free(m_ctx);
		m_ctx = nullptr;
	}

	SocketStreambuf::disconnect();
}

STRING SSLsocketStreambuf::getSocketError( void ) const
{
	STRING	error = m_errorText;
	if( m_sslLibraryError )
	{
		char errorBuffer[10240];

		error += " Lib Error ";
		error += formatNumber( m_sslLibraryError );
		error += ' ';
		error += ERR_error_string( m_sslLayerError, errorBuffer );
		error += ' ';
		error += SocketStreambuf::getSocketError();
	}

	return error;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //


}	// namespace net
}	// namespace gak


#endif	// USE_SSL
