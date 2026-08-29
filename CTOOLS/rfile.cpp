/*
		Project:		GAKLIB
		Module:			rfile.cpp
		Description:	Input File Stream
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

/* --------------------------------------------------------------------- */
/* ----- includes ------------------------------------------------------ */
/* --------------------------------------------------------------------- */

#include <stdlib.h>

#include <gak/io.h>
#include <gak/gaklib.h>
#include <gak/exception.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{


// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

inline void RFILE::skipCharacter()
{
	m_nextRead++;
}

inline void RFILE::ignoreCharacter( int c )
{
	int next = pollCharacter();
	if( next == c )
		skipCharacter();
}

int RFILE::pollCharacter()
{
	int	c;

	if( m_nextRead >= m_firstClear )
	{
		m_firstClear =
			Fread( m_handle, BUFF_SIZE, m_buffer );
		if( m_firstClear > BUFF_SIZE )
			throw ReadError().addCerror();
		else if( m_firstClear > 0 )
			m_nextRead = 0;
	}

	if( m_nextRead < m_firstClear )
		c = m_buffer[m_nextRead];
	else
	{
		c = EOF;
		m_eof = true;
	}

	return c;
}

inline int RFILE::popCharacter()
{
	int	c = pollCharacter();
	skipCharacter();

	return c;
}


// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void RFILE::open( const STRING &fileName )
{
	close();
	m_handle = Fopen( fileName, OM_READ );
	if( m_handle >= 0 )
	{
		m_eof		= false;
		m_nextRead	= 0;
		m_firstClear= 0;
		m_lineEnd	= RL_UNKONW;
	}
	else
		throw OpenReadError( fileName ).addCerror();
}

STRING RFILE::gets()
{
	int				c;
	STRING			string;

	while( !m_eof )
	{
		c = popCharacter();
		if( c == '\n' )
		{
			if( m_lineEnd == RL_UNKONW )
			{
				c = pollCharacter();
				if( c == '\r' )
				{
					m_lineEnd = RL_END_LFCR;
					skipCharacter();
				}
				else
					m_lineEnd = RL_END_LF;

				break;
			}
			else if( m_lineEnd == RL_END_LF )
			{
				break;
			}
			else if( m_lineEnd == RL_END_LFCR )
			{
				ignoreCharacter('\r');
				break;
			}
		}
		else if( c == '\r' )
		{
			if( m_lineEnd == RL_UNKONW )
			{
				c = pollCharacter();
				if( c == '\n' )
				{
					m_lineEnd = RL_END_CRLF;
					skipCharacter();
				}
				else
					m_lineEnd = RL_END_CR;

				break;
			}
			else if( m_lineEnd == RL_END_CR )
			{
				break;
			}
			else if( m_lineEnd == RL_END_CRLF )
			{
				ignoreCharacter('\n');
				break;
			}
		}
		else if( !m_eof )
		{
			string += c;
		}
	}

	if( m_eof && !string.isEmpty() )
		m_eof = false;

	return string;
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

}	// namespace gak
