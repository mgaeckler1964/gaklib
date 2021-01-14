/*
		Project:		GAKLIB
		Module:			setcwd.cpp
		Description:	Set current working directory for DOS-systems
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

#include <ctype.h>

#if defined( _Windows )
	#include <direct.h>
#elif defined( __unix__ ) || defined( __MACH__ )
	#include <unistd.h>
#else
	#error "Don't know system"
#endif

#include <gak/string.h>
#if defined( _Windows )
#include <gak/wideString.h>
#endif
#include <gak/gaklib.h>
#include <gak/directory.h>

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */
namespace gak
{

int setcwd( const STRING &newPath )
{
#if defined( _Windows )
	if( newPath[1U] == ':' )
		_chdrive( ansiToUpper( newPath[0U] ) - 'A' + 1 );

	int	result;

	if( newPath.getCharSet() == STR_UTF8 )
	{
		result = _wchdir( uSTRING().decodeUTF8( newPath ) );

	}
	else
		result = chdir( newPath );

	return result;
#else
	return chdir( newPath );
#endif
}

}	// namespace gak