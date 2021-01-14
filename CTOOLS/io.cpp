/*
		Project:		GAKLIB
		Module:			io.cpp
		Description:	Tos´ Fread-function for DOS
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
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits.h>

#include <io.h>

#include <gak/io.h>

namespace gak
{

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef _Windows
size_t Fread( int handle, size_t count, void *buffer )
{
	size_t	numRead;
	int		errCode;

	numRead = 0;
	while( count )
	{
		errCode = read(
			handle,
			 buffer,
			count>INT_MAX ? INT_MAX : unsigned(count) );

		if( !errCode )
			break;
		if( errCode < 0 )
		{
			numRead = size_t(-1);
			break;
		}
		numRead        += errCode;
		count          -= errCode;

		buffer = (char *)buffer + errCode;
	}

	return numRead;
}

size_t Fwrite( int handle, size_t count, void *buffer )
{
	size_t	numWritten;
	int		errCode;

	numWritten = 0;
	while( count )
	{
		errCode = write(
			handle,
			 buffer,
			count>INT_MAX ? INT_MAX : unsigned(count) );

		if( !errCode )
			break;
		if( errCode < 0 )
		{
			numWritten = size_t(-1);
			break;
		}
		numWritten     += errCode;
		count          -= errCode;

		buffer = (char *)buffer + errCode;
	}

	return numWritten;
}
#endif

}	// namespace gak