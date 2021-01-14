/*
		Project:		GAKLIB
		Module:			hostResolver.cpp
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifdef _MSC_VER
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <gak/exception.h>
#include <gak/hostResolver.h>

#include <iostream>

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

ResolverError::ResolverError(const STRING &url) : NETerror("Cannot resolve host",url) {}

HostResolver::HostResolver(const char *server, int port)
{
	struct hostent	* const hostp = gethostbyname( server );
	if( hostp )
	{
		memcpy(
			&socketAddress.addr_in.sin_addr,
			hostp->h_addr,
			sizeof(socketAddress.addr_in.sin_addr)
		);
	}
	else
	{
		socketAddress.addr_in.sin_addr.s_addr = inet_addr( server );
		if(socketAddress.addr_in.sin_addr.s_addr == -1 ) 
		{
			throw ResolverError(server);
		}
	}

	socketAddress.addr_in.sin_family = AF_INET;
	socketAddress.addr_in.sin_port = htons( uint16(port) );
	setTmp();
}

#ifndef __BORLANDC__
namespace detail
{
struct AddrInfo
{
	struct addrinfo *m_info;

	AddrInfo() : m_info(NULL) {}
	~AddrInfo()
	{
		if(m_info) 
		{
			freeaddrinfo(m_info);
		}
	}
	
};

}

XResolver::XResolver(const char *server, int port)
{
	detail::AddrInfo	info;

#ifdef _MSC_VER
	if( !strcmp( server, "localhost" ) ) 
	{
		server = "127.0.0.1";
	}
#endif
	int errCode = ::getaddrinfo( server, NULL, NULL, &info.m_info );
	if( errCode )
	{
		throw ResolverError(server);
		//std::string text(gai_strerror(errCode));
		//std::cerr << errCode << text;
	}
	else
	{
		struct sockaddr_in  *sockaddr_ipv4;
		sockaddr_ipv4 = (struct sockaddr_in *) info.m_info->ai_addr;
		memcpy(
			&socketAddress.addr_in.sin_addr,
			(void*)&sockaddr_ipv4->sin_addr,
			sizeof(socketAddress.addr_in.sin_addr)
		);
	}

	socketAddress.addr_in.sin_family = AF_INET;
	socketAddress.addr_in.sin_port = htons( uint16(port) );
	setTmp();
}

#endif

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

}	// namespace net
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

