/*
		Project:		GAKLIB
		Module:			rfile.cpp
		Description:	Input File Stream
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

int RFILE::getCharacter( void )
{
	int	c;

	if( nextRead >= firstClear )
	{
		firstClear =
			Fread( handle, buffSize, buffer );
		if( firstClear > buffSize )
			throw ReadError().addCerror();
		else if( firstClear > 0 )
			nextRead = 0;
	}

	if( nextRead < firstClear )
		c = buffer[nextRead];
	else
	{
		c = EOF;
		eof = true;
	}

	return c;
}

inline int RFILE::getNextCharacter( void )
{
	int	c = getCharacter();

	nextRead++;

	return c;
}


// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void RFILE::open( const STRING &fileName )
{
	close();
	handle = Fopen( fileName, OM_READ );
	if( handle >= 0 )
	{
		eof			= false;
		nextRead	= 0;
		firstClear	= 0;
		lineEnd	= RL_UNKONW;
	}
	else
		throw OpenReadError( fileName ).addCerror();
}

STRING RFILE::gets(  void )
{
	int				c;
	STRING			string;

	while( !eof )
	{
		c = getNextCharacter();
		if( c == '\n' )
		{
			if( lineEnd == -1 )
			{
				c = getCharacter();
				if( c == '\r' )
				{
					lineEnd = RL_END_LFCR;
					nextRead++;
				}
				else
					lineEnd = RL_END_LF;

				break;
			}
			else if( lineEnd == RL_END_LF )
			{
				break;
			}
			else if( lineEnd == RL_END_LFCR )
			{
				c = getCharacter();
				if( c == '\r' )
					nextRead++;

				break;
			}
		}
		else if( c == '\r' )
		{
			if( lineEnd == -1 )
			{
				c = getCharacter();
				if( c == '\n' )
				{
					lineEnd = RL_END_CRLF;
					nextRead++;
				}
				else
					lineEnd = RL_END_CR;

				break;
			}
			else if( lineEnd == RL_END_CR )
			{
				break;
			}
			else if( lineEnd == RL_END_CRLF )
			{
				c = getCharacter();
				if( c == '\n' )
					nextRead++;

				break;
			}
		}
		else if( !eof )
		{
			string += c;
		}
	}

	if( eof && !string.isEmpty() )
		eof = false;

	return string;
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

}	// namespace gak
