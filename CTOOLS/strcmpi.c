/*
		Project:		GAKLIB
		Module:			strcmpi.c
		Description:
		Author:			Martin G�ckler
		Address:		Hopfengasse 15, A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2021 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Germany, Munich ``AS IS''
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

#include <ctype.h>

#include <gak/gaklib.h>
#include <gak/ansiChar.h>

int strcmpi( const char *s1, const char *s2 )
{
	int	c1, c2, result;

	do
	{
		c1 = ansiToLower( *s1++ );
		c2 = ansiToLower( *s2++ );
		result = c1-c2;
	} while( (*s1 || *s2) && !result );

	return result;
}

int strncmpi( const char *s1, const char *s2, size_t len )
{
	int	c1, c2, result = 0;

	while( len>0 && (*s1 || *s2) && !result)
	{
		c1 = ansiToLower( *s1++ );
		c2 = ansiToLower( *s2++ );
		result = c1-c2;
		len--;
	}

	return result;
}


