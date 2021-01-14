/*
		Project:		GAKLIB
		Module:			DIFF.CPP
		Description:	Create difference information
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <gak/gaklib.h>
#include <gak/io.h>
#include <gak/string.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

namespace gak
{

/* --------------------------------------------------------------------- */
/* ----- type definitions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

struct LineInfo
{
	size_t	lineNo;
	size_t	comparedLine;
	long	chksum;
	size_t	chksumCnt;
	off_t	position;
};

const std::size_t no_line = std::size_t(-1);

/* --------------------------------------------------------------------- */
/* ----- module functions ---------------------------------------------- */
/* --------------------------------------------------------------------- */

/*
	Sorting functions
*/
static int lineSort( const void *ptr1, const void *ptr2 )
{
	LineInfo *info1 = (LineInfo *)ptr1;
	LineInfo *info2 = (LineInfo *)ptr2;

	if( info1->lineNo == info2->lineNo )
		return 0;
	if( info1->lineNo == no_line )
/***/	return 1;
	else if( info2->lineNo == no_line )
/***/	return -1;

	return info1->lineNo > info2->lineNo ? 1 : -1;
}

static int chksumSort( const void *ptr1, const void *ptr2 )
{
	LineInfo *info1 = (LineInfo *)ptr1;
	LineInfo *info2 = (LineInfo *)ptr2;

	if( info1->lineNo == no_line )
/***/	return 1;
	else if( info2->lineNo == no_line )
/***/	return -1;
	if( info1->chksum == info2->chksum )
/***/	return lineSort( ptr1, ptr2 );
	else
/***/	return info1->chksum > info2->chksum ? 1 : -1;
}

static int chksumCountSort( const void *ptr1, const void *ptr2 )
{
	LineInfo *info1 = (LineInfo *)ptr1;
	LineInfo *info2 = (LineInfo *)ptr2;

	if( info1->lineNo == no_line )
/***/	return 1;
	else if( info2->lineNo == no_line )
/***/	return -1;
	if( info1->chksumCnt == info2->chksumCnt )
/***/	return chksumSort( ptr1, ptr2 );
	else
/***/	return info1->chksumCnt > info2->chksumCnt ? 1 : -1;
}

/*
	compare functions
*/
static void checkStart( LineInfo *info1, LineInfo *info2 )
{
	size_t	idx;

	idx = 0;
	while( 1 )
	{
		if( info1[idx].lineNo == no_line )
/*v*/		break;
		if( info2[idx].lineNo == no_line )
/*v*/		break;
		if( info1[idx].chksum == info2[idx].chksum )
		{
			info1[idx].comparedLine = idx;
			info2[idx].comparedLine = idx;
		}
		else
/*v*/		break;
		idx++;
	}
}

static void checkEnd( LineInfo *info1, LineInfo *info2 )
{
	size_t	idx1, idx2;

	for( idx1 = 0; info1[idx1].lineNo != no_line; idx1++ )
		;
	for( idx2 = 0; info2[idx2].lineNo != no_line; idx2++ )
		;

	while( 1 )
	{
		idx1--;
		idx2--;

		if( (int)idx1 == -1 )
/*v*/		break;
		if( (int)idx2 == -1 )
/*v*/		break;
		if( info1[idx1].chksum == info2[idx2].chksum )
		{
			info1[idx1].comparedLine = idx2;
			info2[idx2].comparedLine = idx1;
		}
		else
/*v*/		break;
	}
}

static size_t findMatchingLine( LineInfo *info1, size_t idx1, LineInfo *info2 )
{
	size_t	idx2;
	size_t	maxLine;

	maxLine = 0;
	for( idx2=0; info2[idx2].lineNo != no_line; idx2++ )
	{
		if( info2[idx2].comparedLine != no_line
		&&  info2[idx2].lineNo > maxLine
		&&  info2[idx2].comparedLine < info1[idx1].lineNo )
			maxLine = info2[idx2].lineNo;
	}

	for( idx2=0; info2[idx2].lineNo != no_line; idx2++ )
	{
		if( info2[idx2].comparedLine == no_line
		&&  info2[idx2].lineNo >= maxLine
		&&  info2[idx2].chksum == info1[idx1].chksum )
/*@*/		return idx2;
	}

	return no_line;
}

static void checkCount( LineInfo *info1, LineInfo *info2 )
{
	size_t	idx1, idx2;

	for( idx1 = 0; info1[idx1].lineNo != no_line; idx1++ )
	{
		if( info1[idx1].comparedLine == no_line )
		{
			idx2 = findMatchingLine( info1, idx1, info2 );
			if( idx2 != no_line
			&& info1[idx1].chksumCnt == info2[idx2].chksumCnt )
			{
				info2[idx2].comparedLine = info1[idx1].lineNo;
				info1[idx1].comparedLine = info2[idx2].lineNo;
			}
		}
	}
}

static void checkFiles( LineInfo *info1, LineInfo *info2 )
{
	size_t	idx1, idx2;

	for( idx1=0; info1[idx1].lineNo != no_line; idx1++ )
	{
		if( info1[idx1].comparedLine == no_line )
		{
			idx2 = findMatchingLine( info1, idx1, info2 );
			if( idx2 != no_line )
			{
				info2[idx2].comparedLine = info1[idx1].lineNo;
				info1[idx1].comparedLine = info2[idx2].lineNo;
			}
		}
	}
}

static void countChksum( LineInfo *info )
{
	size_t	idx1 = 0, idx2 = 0, count = 0;
	long	chksum = -1;

	while( info[idx2].lineNo != no_line )
	{
		if( info[idx1].lineNo != no_line && info[idx1].chksum == chksum )
		{
			idx1++;
			count++;
		}
		else
		{
			for( ;info[idx2].lineNo != no_line && info[idx2].chksum == chksum; idx2++ )
				info[idx2].chksumCnt = count;

			count = 0;
			chksum = info[idx2].chksum;
		}
	}
}

/*
	read files, create diff files
*/

static LineInfo *readFile( const STRING &fileName, size_t *nLines )
{
	bool		ignoreSpaces = true;
	bool		space;
	RFILE		fp;
	LineInfo	*fileInfo;
	size_t		lines;
	size_t		maxLines;
	STRING		lineBuffer;
	size_t		idx, lastChar;

	fileInfo = NULL;
	lines = 0;
	maxLines = 0;
	
	fp.open( fileName );
	while( true )
	{
		if( lines >= maxLines )
		{
			maxLines += 32;
			fileInfo = (LineInfo*)realloc( fileInfo, maxLines * sizeof( *fileInfo ) );
			if( !fileInfo )
/*v*/			break;
		}
		fileInfo[lines].chksum = 0;
		fileInfo[lines].lineNo = lines;
		fileInfo[lines].comparedLine = -1;
		fileInfo[lines].position = fp.getpos();
		lineBuffer = fp.gets();
		if( fp.isEOF() )
		{
			fileInfo[lines].lineNo = no_line;
/*v*/		break;
		}
		if( ignoreSpaces )
		{
			for( idx=0; lineBuffer[idx] && isspace(static_cast<unsigned char>(lineBuffer[idx])); idx++ )
				;
			for(
				lastChar = lineBuffer.strlen()-1;
				lastChar != size_t(-1) && isspace(static_cast<unsigned char>(lineBuffer[lastChar]));
				lastChar--
			)
				;


			space = false;
		}
		else
		{
			idx = 0;
			lastChar = size_t(-1);
		}
		for( ; lineBuffer[idx] && lineBuffer[idx] != '\n' && idx <= lastChar; idx++ )
		{
			if( ignoreSpaces && isspace(static_cast<unsigned char>(lineBuffer[idx])) )
			{
				if( !space )
					fileInfo[lines].chksum++;
				space = true;
			}
			else
			{
				fileInfo[lines].chksum += long(lineBuffer[idx]) * long(idx);
				space = false;
			}
		}
		lines++;
	}
	fp.close();

	*nLines = lines +1;
	return fileInfo;
}

static STRING buildDiffFile(
	const char *file1, const char *file2,
	LineInfo *info1, LineInfo *info2
)
{
	STRING	diff;
	RFILE	fp1, fp2;
	size_t	idx1, idx2;
	STRING	lineBuff;

	// open the files
	fp1.open( file1 );
	fp2.open( file2 );

	idx1 = 0;
	idx2 = 0;

	while( 1 )
	{
		if( info1[idx1].lineNo != no_line
		&&  info1[idx1].comparedLine == no_line )
		{
			fp1.setpos( info1[idx1].position );
			lineBuff = fp1.gets();

			diff += "-<";
			diff += lineBuff;
			diff += '\n';
			idx1++;
		}
		else if( info2[idx2].lineNo != no_line
				&&  info2[idx2].comparedLine == no_line )
		{
			fp2.setpos( info2[idx2].position );
			lineBuff = fp2.gets();

			diff += "+>";
			diff += lineBuff;
			diff += '\n';
			idx2++;
		}
		else
		{
			if( info1[idx1].lineNo != no_line )
			{
				fp1.setpos( info1[idx1].position );
				lineBuff = fp1.gets();

				diff += "  ";
				diff += lineBuff;
				diff += '\n';
				idx1++;
			}
			if( info2[idx2].lineNo != no_line )
				idx2++;
		}
		if( info1[idx1].lineNo == no_line && info2[idx2].lineNo == no_line )
			break;
	}

	return diff;
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

const STRING &diff( const STRING &file1, const STRING &file2 )
{
	static STRING	diffInfo;
	LineInfo		*info1, *info2;
	size_t			nLines1, nLines2;

	info1 = readFile( file1, &nLines1 );
	if( info1 )
	{
		info2 = readFile( file2, &nLines2 );
		if( info2 )
		{
			// check from beginig of file
			checkStart( info1, info2 );

			// check from end of file
			checkEnd( info1, info2 );

			qsort( info1, nLines1, sizeof( LineInfo ), chksumSort );
			qsort( info2, nLines2, sizeof( LineInfo ), chksumSort );

			countChksum( info1 );
			countChksum( info2 );

			qsort( info1, nLines1, sizeof( LineInfo ), chksumCountSort );
			qsort( info2, nLines2, sizeof( LineInfo ), chksumCountSort );

			// check complete files
			checkCount( info1, info2 );
			checkFiles( info1, info2 );

			qsort( info1, nLines1, sizeof( LineInfo ), lineSort );
			qsort( info2, nLines2, sizeof( LineInfo ), lineSort );

			diffInfo = buildDiffFile( file1, file2, info1, info2 );

			free( info2 );
		}
		free( info1 );
	}

	return diffInfo;
}

}	// namespace gak
