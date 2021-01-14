/*
		Project:		GAKLIB
		Module:			textReader.h
		Description:	Text Reader for XML- and HTML-Parser
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

#ifndef TEXT_READER_H
#define TEXT_READER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <fstream>

#include <gak/array.h>
#include <gak/fmtNumber.h>
#include <gak/directoryEntry.h>
#include <gak/ci_string.h>


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

// Known encodings
extern const char ISO_8859_1[];
extern const char UTF_8[];

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct TextPosition
{
	F_STRING	m_fileName;
	size_t		m_lineNo;
	size_t		m_column;

	TextPosition( const STRING &fileName=NULL_STRING )  
		: m_fileName( fileName )
	{
#if '\\' != DIRECTORY_DELIMITER
		m_fileName.replaceChar( '\\', DIRECTORY_DELIMITER );
#elif  '/' != DIRECTORY_DELIMITER
		m_fileName.replaceChar( '/', DIRECTORY_DELIMITER );
#endif
		m_lineNo = 1;
		m_column = 0;
	}
	int compare( const TextPosition &other ) const
	{
		int	compareResult = gak::compare( m_fileName, other.m_fileName );
		if( !compareResult )
		{
			compareResult = gak::compare( m_lineNo, other.m_lineNo );
		}
		if( !compareResult )
		{
			compareResult = gak::compare( m_column, other.m_column );
		}

		return compareResult;
	}
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, m_fileName );
		gak::toBinaryStream( stream, m_lineNo );
		gak::toBinaryStream( stream, m_column );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &m_fileName );
		gak::fromBinaryStream( stream, &m_lineNo );
		gak::fromBinaryStream( stream, &m_column );
	}
};

class TextReader
{
	private:
	CI_STRING			m_encoding;
	bool				m_iStreamCreated, m_utf8failure;
	std::istream		*m_inputStream;
	ArrayOfStrings		m_errors;
	TextPosition		m_position;

	public:
	TextReader( const STRING &theFileName ) : m_position( theFileName )
	{
		m_inputStream = new std::ifstream(m_position.m_fileName);
		if( !*m_inputStream )
		{
			delete m_inputStream;
			m_inputStream = NULL;
			throw OpenReadError( theFileName );
		}
		m_iStreamCreated = true;
		m_utf8failure = false;
	}
	TextReader( std::istream *theInput, const STRING &theFileName=NULL_STRING )
	: m_position( theFileName )
	{
		m_inputStream = theInput;
		m_iStreamCreated = false;
		m_utf8failure = false;
	}
	~TextReader()
	{
		closeStream();
	}

	public:
	STRING readString( char stringDelimiter );
	char getNextWithBlank( void );
	char getNextNonBlank( void );
	bool eof( void ) const
	{
		return m_inputStream->eof() || m_inputStream->fail();
	}
	void putback( char c )
	{
		if( c == '\n' )
		{
			m_position.m_lineNo--;
		}
		else
		{
			m_position.m_column--;
		}
		m_inputStream->putback( c );
	}
	void addError( const STRING &theError )
	{
		addError( theError, m_position.m_lineNo, m_position.m_column );
	}
	void addError( const STRING &theError, size_t lineNo, size_t column )
	{
		STRING	newError = formatNumber( lineNo ) + '.' + formatNumber( column );

		newError += ": ";
		newError += theError;

		m_errors += newError;
	}

	public:
	STRING getErrors( void );
	void setEncoding( const STRING &encoding )
	{
		m_encoding = encoding;
	}
	const CI_STRING &getEncoding() const
	{
		return m_encoding;
	}
	const TextPosition &getPosition( void ) const
	{
		return m_position;
	}

	void closeStream( void )
	{
		if( m_iStreamCreated )
		{
			delete m_inputStream;
			m_inputStream = NULL;
			m_iStreamCreated = false;
		}
	}
	bool hasUtf8failure( void ) const
	{
		return m_utf8failure;
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

STRING encodeBase64( const STRING &src );
STRING encodeBase64( ArrayOfData &src );
void decodeBase64( const STRING &src, ArrayOfData &dest );
STRING decodeQuotedPrintable( const STRING &src );
STRING decodeMessageHeader( const STRING &src );

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

#endif
