/*
		Project:		GAKLIB
		Module:			impwsdl.cpp
		Description:	the WSDL-importer tool.
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

#include <iostream>

#include <gak/cmdlineParser.h>
#include <gak/wsdlImporter.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const int proxyPresent			= 0x0100;
static const int portPresent			= 0x0200;
static const int keyfilePresent			= 0x0400;
static const int sslPasswordPresent		= 0x0800;
static const int userNamePresent		= 0x1000;
static const int userPasswordPresent	= 0x2000;
static const int destinationPresent		= 0x4000;

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

static gak::CommandLine::Options options[] =
{
	{ 'C', "proxy",			0, 1, proxyPresent|CommandLine::needArg,		"<proxy server>" },
	{ 'O', "port",			0, 1, portPresent|CommandLine::needArg,			"<proxy port>" },
	{ 'K', "keyfile",		0, 1, keyfilePresent|CommandLine::needArg,		"<ssl key file>" },
	{ 'S', "sslPassword",	0, 1, sslPasswordPresent|CommandLine::needArg,	"<ssl password>" },
	{ 'U', "userName",		0, 1, userNamePresent|CommandLine::needArg,		"<user>" },
	{ 'P', "userPassword",	0, 1, userPasswordPresent|CommandLine::needArg, "<password>" },
	{ 'D', "destination",	0, 1, destinationPresent|CommandLine::needArg,	"<local parth>" },
	{ 0 },
};

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static int impwsdl( const CommandLine &cmdLine )
{
	STRING		wsdl, keyfile, sslPassword, proxy,
				userName, userPassword, destination;
	unsigned	proxyPort=0;

	if( cmdLine.flags & proxyPresent )
	{
		proxy = cmdLine.parameter['C'][0];
	}
	if( cmdLine.flags & portPresent )
	{
		proxyPort = cmdLine.parameter['O'][0].getValueE<unsigned>();
	}
	if( cmdLine.flags & keyfilePresent )
	{
		keyfile = cmdLine.parameter['K'][0];
	}
	if( cmdLine.flags & sslPasswordPresent )
	{
		sslPassword = cmdLine.parameter['S'][0];
	}
	if( cmdLine.flags & userNamePresent )
	{
		userName = cmdLine.parameter['U'][0];
	}
	if( cmdLine.flags & userPasswordPresent )
	{
		userPassword = cmdLine.parameter['P'][0];
	}
	if( cmdLine.flags & destinationPresent )
	{
		destination = cmdLine.parameter['D'][0];
	}
	for( int i=1; i<cmdLine.argc; i++ )
	{
		wsdl = cmdLine.argv[i];

		std::cout << proxy << ' ' << proxyPort << ' ' << keyfile << ' ' << sslPassword << ' ' << wsdl << std::endl;

		net::HTTPrequest::setProxy( proxy, proxyPort );
		net::HTTPrequest::setSSLkeyfile( keyfile, sslPassword );
		net::WSDLimporter	theRequest( wsdl, userName, userPassword );

		theRequest.writeCppClass(destination);
	}

	if( wsdl.isEmpty() )
	{
		throw gak::CmdlineError();
	}

	return EXIT_SUCCESS;
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

int main( int, const char *argv[] )
{
	int result = EXIT_FAILURE;

	try
	{
		CommandLine cmdLine( options, argv );
		result = impwsdl( cmdLine );
	}
	catch( CmdlineError &e )
	{
		std::cerr << argv[0] << ": " << e.what() << std::endl;
		std::cerr << "Usage: " << argv[0] << " <options>... <URL to WSDL>\n" << options;
	}
	catch( std::exception &e )
	{
		std::cerr << argv[0] << ": " << e.what() << std::endl;
	}
	catch( ... )
	{
		std::cerr << argv[0] << ": Unkown error" << std::endl;
	}

	return result;
}
