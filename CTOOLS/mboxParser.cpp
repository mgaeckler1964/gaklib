/*
		Project:		GAKLIB
		Module:			mboxParser.cpp
		Description:	Parser for Linux mbox files
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/mboxParser.h>

#include <fstream>

#include <gak/string.h>
#include <gak/logfile.h>
#include <gak/textReader.h>
#include <gak/t_string.h>
#include <gak/stringStream.h>

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
namespace mail
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

static STRING appendMailHeader( std::istream &fp, MAIL *theMail )
{
	doEnterFunctionEx(gakLogging::llDetail,"appendMailHeader");

	STRING	line, nextLine, header;

	fp >> line;
	while( !fp.eof() )
	{
		header += line;
		header += '\n';

		if( !line.isEmpty() )
		{
			do
			{
				fp >> nextLine;
				if( !nextLine.isEmpty() && isspace( nextLine[0U] ) )
				{
					line += nextLine;
				}
				else
				{
					break;
				}
			} while( !fp.eof() );
		}

		if( line.beginsWithI( "From:" ) )
		{
			line += std::size_t(5);
			line.stripBlanks();
			theMail->from = decodeMessageHeader( line );
		}
		else if( line.beginsWithI( "To:" ) )
		{
			line += std::size_t(3);
			line.stripBlanks();
			theMail->to = decodeMessageHeader( line );
		}
		else if( line.beginsWithI( "CC:" ) )
		{
			line += std::size_t(3);
			line.stripBlanks();
			theMail->cc = decodeMessageHeader( line );
		}
		else if( line.beginsWithI( "Subject:" ) )
		{
			line += std::size_t(8);
			line.stripBlanks();
			theMail->subject = decodeMessageHeader( line );
		}
		else if( line.beginsWithI( "Message-ID:" ) )
		{
			line += std::size_t(11);
			line.stripBlanks();
			theMail->messageID = line;
		}
		else if( line.beginsWithI( "Date:" ) )
		{
			line += std::size_t(5);
			line.stripBlanks();
			try
			{
				theMail->date.setInetTime( line );
			}
			catch( ... )
			{
				theMail->date.setDate( 1, Date::JANUARY, 1901 );
				theMail->date.setTime( 0, 0, 0 );
			}
		}
		else if( line.beginsWithI( "Content-Type:" ) )
		{
			line += std::size_t(13);
			line.stripBlanks();
			theMail->contentType = line;
		}
		else if( line.beginsWithI( "Content-Disposition:" ) )
		{
			line += std::size_t(20);
			line.stripBlanks();
			theMail->contentDisposition = line;
		}

		else if( line.beginsWithI( "Content-Transfer-Encoding:" ) )
		{
			line += std::size_t(26);
			line.stripBlanks();
			theMail->contentTransferEncoding = line;
		}
		else if( line.beginsWithI( "Content-ID:" ) )
		{
			line += std::size_t(11);
			line.stripBlanks();
			theMail->contentID = line;
		}
		else if( line.beginsWithI( "X-Status:" ) )
		{
			line += std::size_t(9);
			line.stripBlanks();
			theMail->xStatus = line;
		}
		else if( line.isEmpty() )
		{
			break;
		}
		line = nextLine;
	}
	if( !theMail->contentType.isEmpty() )
	{
		T_STRING	contentType = theMail->contentType;
		STRING		type = contentType.getFirstToken( ";" );
		T_STRING	subType = contentType.getNextToken();
		while( !subType.isEmpty() )
		{
			CI_STRING name=subType.getFirstToken( "=" );
			if( name=="boundary" )
			{
				size_t	eqPos = subType.searchChar('=');
				subType += eqPos + 1;
				subType.stripBlanks();

				theMail->boundary = subType;
				if( theMail->boundary.strlen() > 2 
				&&  theMail->boundary[0U] == '\"' 
				&&  theMail->boundary[theMail->boundary.strlen()-1] == '\"' )
				{
					theMail->boundary.cut( theMail->boundary.strlen()-1 );
					theMail->boundary += std::size_t(1);
				}
			}
			else if( name=="charset" )
			{
				size_t	eqPos = subType.searchChar('=');
				subType += eqPos + 1;
				subType.stripBlanks();

				theMail->charset = subType;
				if( theMail->charset.strlen() > 2
				&&  theMail->charset[0U] == '\"'
				&&  theMail->charset[theMail->charset.strlen()-1] == '\"' )
				{
					theMail->charset.cut( theMail->charset.strlen()-1 );
					theMail->charset += std::size_t(1);
				}
			}
			else if( name=="name" )
			{
				size_t	eqPos = subType.searchChar('=');
				subType += eqPos + 1;
				subType.stripBlanks();

				theMail->attachFileName = subType;
			}
			subType = contentType.getNextToken();
		}
		theMail->contentType = type;
	}
	if( !theMail->contentDisposition.isEmpty() )
	{
		T_STRING	contentDisposition = theMail->contentDisposition;
		STRING		disposition = contentDisposition.getFirstToken( ";" );
		T_STRING	subDisposition = contentDisposition.getNextToken();
		while( !subDisposition.isNullPtr() )
		{
			CI_STRING name=subDisposition.getFirstToken( "=" );
			if( name=="filename" )
			{
				size_t	eqPos = subDisposition.searchChar('=');
				subDisposition += eqPos + 1;
				subDisposition.stripBlanks();

				theMail->attachFileName = subDisposition;
			}
			subDisposition = contentDisposition.getNextToken();
		}
		theMail->contentDisposition = disposition;
	}

	if( theMail->attachFileName.strlen() > 2
	&&  theMail->attachFileName[0U] == '\"'
	&&  theMail->attachFileName[theMail->attachFileName.strlen()-1] == '\"' )
	{
		theMail->attachFileName.cut( theMail->attachFileName.strlen()-1 );
		theMail->attachFileName += std::size_t(1);
	}
	theMail->attachFileName = decodeMessageHeader( theMail->attachFileName );

	return header;
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

STRING readMailHeader( std::istream &fp, MAIL *theMail )
{
	doEnterFunctionEx(gakLogging::llDetail,"readMailHeader");

	STRING	header = appendMailHeader( fp, theMail );

	return header;
}

STRING readMailBody( std::istream &fp, const CI_STRING &contentTransfer, const CI_STRING &encoding, const STRING boundary, bool *endFound )
{
	doEnterFunctionEx(gakLogging::llDetail, "readMailBody" );

	STRING	begBoundary = "--" + boundary;
	STRING	endBoundary = begBoundary + "--";

	STRING	line, body;
	bool	empty = false;
	std::istream::pos_type curPos = fp.tellg();
	while( !fp.eof() )
	{
		curPos = fp.tellg();
		fp >> line;
		if( line.isEmpty() )
		{
			empty = true;
		}
		else if( !boundary.isEmpty()
		&&  (line == begBoundary || line == endBoundary) )
		{
			if( endFound )
			{
				*endFound = (line == endBoundary);
			}
/*v*/		break;
		}
		else if( !empty || !line.beginsWith( "From " ) )
		{
			if( empty )
			{
				body += '\n';
				empty = false;
			}
			body += line;
			body += '\n';
		}
		else
		{
/*v*/		break;
		}
	}
	if( line.beginsWith( "From " ) )
	{
		fp.seekg(curPos);
	}
	if( contentTransfer == "quoted-printable" )
	{
		body = decodeQuotedPrintable( body );
	}
	if( encoding == "utf-8" )
	{
		body = body.decodeUTF8();
	}

	return body;
}

void getBodyParts( const STRING &body, Array<MAIL> *theMails, const STRING &boundary )
{
	doEnterFunctionEx(gakLogging::llDetail, "getBodyParts( const STRING &body, Array<MAIL> *theMails, const STRING &boundary )" );

	theMails->clear();
	iSTRINGstream fp( body );

	STRING	begBoundary = "--" + boundary;
	STRING	line;
	MAIL	theMail;
	bool	endFound = false;

	// search boundary
	while( !fp.eof() )
	{
		fp >> line;
		if( line == begBoundary )
		{
			break;
		}
	}

	if( line == begBoundary )
	{
		while( !fp.eof() && !endFound )
		{
			readMailHeader( fp, &theMail );

			theMail.body = readMailBody( fp, theMail.contentTransferEncoding, theMail.charset, boundary, &endFound );

			theMails->addElement( theMail );
		}
	}
}


void loadMail(
	const STRING &mboxFile, const STRING &messageID, MAIL *theMail
)
{
	doEnterFunctionEx(gakLogging::llDetail, "loadMail(const STRING &mboxFile, const STRING &messageID, MAIL *theMail)" );
	STRING	line;

	std::ifstream fp( mboxFile );
	while( !fp.eof() )
	{
		// search begin of mail
		while( !fp.eof() )
		{
			fp >> line;
			if( line.beginsWith( "From MAILER-DAEMON" ) )
			{
/*^*/			continue;
			}

			if( line.beginsWith( "From " ) )
			{
/*v*/			break;
			}
		}

		if( !fp.eof() )
		{
			readMailHeader( fp, theMail );
		}

		if( theMail->messageID == messageID )
		{
/*v*/		break;
		}
	}
	theMail->body = readMailBody(
		fp, theMail->contentTransferEncoding, theMail->charset
	);
}

void loadMboxFile( const STRING &mboxFile, Array<MAIL> &theMails )
{
	doEnterFunctionEx(gakLogging::llDetail, "void loadMboxFile( const STRING &mboxFile, Array<MAIL> &theMails )" );
	STRING	line;
	std::ifstream fp( mboxFile, std::ifstream::binary );
	while( !fp.eof() )
	{
		// search begin of mail
		while( !fp.eof() )
		{
			fp >> line;
			if( line.beginsWith( "From MAILER-DAEMON" ) )
			{
/*^*/			continue;
			}

			if( line.beginsWith( "From " ) )
			{
/*v*/			break;
			}
		}

		if( fp.eof() )
		{
			break;
		}

		MAIL	&theMail = theMails.createElement();
		theMail.mboxFile = mboxFile;

		readMailHeader( fp, &theMail );

		theMail.body = readMailBody(
			fp, theMail.contentTransferEncoding, theMail.charset
		);
	}
}

}	// namespace mail
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

