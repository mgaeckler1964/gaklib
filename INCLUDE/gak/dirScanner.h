/*
		Project:		GAKLIB
		Module:			dirScanner.h
		Description:	directory (tree) scanner
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

#ifndef GAK_DIRECTORY_SCANNER_INCLUDED
#define GAK_DIRECTORY_SCANNER_INCLUDED

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/string.h>
#include <gak/directory.h>
#include <gak/cmdlineParser.h>

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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
class FileProcessor
{
public:
	const CommandLine &m_cmdLine;

	FileProcessor(const CommandLine &cmdLine) : m_cmdLine(cmdLine) {}
	void start( const STRING &path )
	{
	}
	void process( const DirectoryEntry &entry, const STRING &file )
	{
	}
	void end( const STRING &path )
	{
	}
};

static const unsigned FOLLOW_REPARSE = 0x01U;
static const unsigned IGNORE_ERROR   = 0x02U;

template <class PROCESSORt>
class DirectoryScanner
{
	PROCESSORt	m_prozessor;

	void processEntry( const DirectoryEntry	&entry, const STRING &path, const STRING &filePattern, unsigned flags )
	{
		STRING newPath = path;
		newPath.condAppend( DIRECTORY_DELIMITER );
		newPath += entry.fileName;

		if( entry.directory )
		{
			scanDirectory( newPath, filePattern, flags );
		}
		else
		{
			m_prozessor.process(entry, newPath);
		}
	}
	void scanDirectory( const STRING &path, const STRING &filePattern, unsigned flags )
	{
		DirectoryList		content;

		m_prozessor.start( path );
		try
		{
			content.dirlist( path, filePattern );
		}
		catch( ... )
		{
			if( !(flags&IGNORE_ERROR) )
				throw;
		}
		for(
			DirectoryList::const_iterator it = content.cbegin(), endIT = content.cend();
			it != endIT;
			++it
		)
		{
			const DirectoryEntry	&entry = *it;
#if defined( __WINDOWS__ )
			if( !(flags&FOLLOW_REPARSE) && entry.reparsePoint )
/*^*/			continue;
#endif
			if( entry.fileName != "." && entry.fileName != ".." )
			{
				processEntry(entry, path, filePattern, flags);
			}
		}
		m_prozessor.end( path );
	}
	public:
	template <class InitT>
	DirectoryScanner(const InitT &initData) : m_prozessor(initData) {}

	void operator () ( const STRING &path, const STRING &filePattern = NULL_STRING, unsigned flags = FOLLOW_REPARSE  )
	{
		DirectoryEntry	entry( path );

		if( entry.directory )
		{
			scanDirectory( path, filePattern, flags );
		}
		else 
		{
			m_prozessor.process(entry, path);
		}
	}
	const PROCESSORt &processor() const
	{
		return m_prozessor;
	}
	PROCESSORt &processor()
	{
		return m_prozessor;
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

#endif	// GAK_DIRECTORY_SCANNER_INCLUDED
