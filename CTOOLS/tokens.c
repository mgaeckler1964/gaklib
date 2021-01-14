/*
		Project:		GAKLIB
		Module:			TOKENS.C
		Description:	tokenSearch, tokenize
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

#include <gak/gaklib.h>

int tokenize( char *string, const char *pointerList[], int numPtr )
{
	register int number;
	register char c;

	if( !*string || !numPtr )
		return 0;
	*pointerList++ = string;
	number = 1;

	while( (c=*string) != 0 && c != '\n' )
	{
		if(c == ' ' || c == ',' || c == ';' )
		{
			*string = '\0';
			if( number >= numPtr )
				break;
			*pointerList++ = string+1;
			number++;
		}
		string++;
	}
	return( number );
}

int tokenSearch( const char *key, const char *list )
{
	const char	*cp;
	char		c1, c2;
	int			found;
	
	if( !*key )
		return -1;

	found = 0;
	while( 1 )
	{
		cp = key;

		while( (c1 = *cp++) == (c2 = *list++) && c1 );
		
		if( c1 == c2 )				/* are thy equal 					*/
			return found;			/* yes return the number 			*/

		found++;					/* count the keywords in the list 	*/
		if( c2 )
			while( *list++ ); 		/* look for thew next key word 		*/
		
		if( !*list )				/* end of list ? 					*/
			return -1;
	}
}
