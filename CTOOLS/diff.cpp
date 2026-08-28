/*
		Project:		GAKLIB
		Module:			DIFF.CPP
		Description:	Create difference information
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <gak/gaklib.h>
#include <gak/io.h>
#include <gak/string.h>

#include <gak/stdlib.h>
#include <gak/fmtNumber.h>

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
	uint32	chksum;
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
	helper functions
*/
/// connect two lines
static void connect( LineInfo *line1, LineInfo *line2 )
{
	assert(line1->chksum == line2->chksum);
	assert(line1->comparedLine == no_line);
	assert(line2->comparedLine == no_line);

	line1->comparedLine = line2->lineNo;
	line2->comparedLine = line1->lineNo;
}

static size_t findMatchingLine( LineInfo *curLine1, LineInfo *info2 )
{
	doEnterFunctionEx( gakLogging::llInfo, "findMatchingLine" );

	/*
		find the last line in file 2 that matches before the actual line in file 1
			and
		find the first line in file 2 that matches after the actual line in file 1
	*/
	size_t	lowerBound = 0;
	size_t	upperBound = std::numeric_limits<std::size_t>::max();
	size_t idx2=0;
	LineInfo *curLine2 = info2;
	for(; curLine2->lineNo != no_line; ++idx2, ++curLine2 )
	{
		if( curLine2->comparedLine != no_line )				// it is a match
		{
			if(  curLine2->lineNo > lowerBound					// it is behind the last match
			&&  curLine2->comparedLine < curLine1->lineNo )		// it matches before the current
				lowerBound = curLine2->lineNo;

			if(  curLine2->lineNo < upperBound					// it is behind the last match
			&&  curLine2->comparedLine > curLine1->lineNo )		// it matches after the current
				upperBound = curLine2->lineNo;
		}
	}

	/*
		the first line in file 2 between lower and upper bound that matches the actual line in file 1
	*/
	idx2=0;
	curLine2 = info2;
	for(; curLine2->lineNo != no_line; ++idx2, ++curLine2 )
	{
		if( curLine2->comparedLine == no_line
		&&  curLine2->lineNo > lowerBound
		&&  curLine2->lineNo < upperBound
		&&  curLine2->chksum == curLine1->chksum )
/*@*/		return idx2;
	}

	return no_line;
}

/*
	count lines with the same checkum (the lines are sorted by checksum)
*/
static void countChksum( LineInfo *info )
{
	doEnterFunctionEx( gakLogging::llInfo, "countChksum" );
	size_t	count = 0;
	uint32	chksum = info->chksum;

	const LineInfo *curLine1 = info;	// used to count
	LineInfo *curLine2 = info;			// store the count
	while( curLine2->lineNo != no_line )
	{
		if( curLine1->lineNo != no_line && curLine1->chksum == chksum )
		{
			curLine1++;
			count++;
		}
		else
		{
			for( ;curLine2->lineNo != no_line && curLine2->chksum == chksum; ++curLine2 )
				curLine2->chksumCnt = count;

			count = 0;
			chksum = curLine2->chksum;
		}
	}
}

/*
	compare functions
*/
/**
	First phase: compare the lines at the begining of the files
	untile the lines differ
*/
static void checkStart( LineInfo *info1, LineInfo *info2 )
{
	doEnterFunctionEx( gakLogging::llInfo, "checkStart" );

	LineInfo	*curLine1 = info1;
	LineInfo	*curLine2 = info2;
	while( 1 )
	{
		if( curLine1->lineNo == no_line )
/*v*/		break;
		if( curLine2->lineNo == no_line )
/*v*/		break;
		if( curLine1->chksum == curLine2->chksum )
		{
			connect(curLine1, curLine2);
		}
		else
/*v*/		break;

		++curLine1;
		++curLine2;
	}
}

/*
	sceond phase: compare the lines at end of the files
	untile the lines differ
*/
static void checkEnd( LineInfo *info1, size_t nLines1, LineInfo *info2, size_t nLines2 )
{
	doEnterFunctionEx( gakLogging::llInfo, "checkEnd" );

	size_t	idx1 = nLines1,
			idx2 = nLines2;
	LineInfo *curLine1 = info1+idx1;
	LineInfo *curLine2 = info2+idx2;
	while( 1 )
	{
		if( --idx1 == no_line )
/*v*/		break;
		if( --idx2 == no_line )
/*v*/		break;

		--curLine1;
		--curLine2;

		if( curLine1->chksum == curLine2->chksum )
		{
			connect( curLine1, curLine2 );
		}
		else
/*v*/		break;
	}
}

/**
	third phase: compare the lines where the count did not change
*/
static void checkCount( LineInfo *info1, LineInfo *info2 )
{
	doEnterFunctionEx( gakLogging::llInfo, "checkCount" );
	for( LineInfo *curLine1 = info1; curLine1->lineNo != no_line; ++curLine1 )
	{
		if( curLine1->comparedLine == no_line )
		{
			size_t	idx2 = findMatchingLine( curLine1, info2 );
			if( idx2 != no_line )
			{
				LineInfo *curLine2 = info2 + idx2;
				if( curLine1->chksumCnt == curLine2->chksumCnt )
				{
					connect(curLine1, curLine2);
				}
			}
		}
	}
}

/**
	forth phase: try to connect all lines not yet connected
*/
static void checkFiles( LineInfo *info1, LineInfo *info2 )
{
	doEnterFunctionEx( gakLogging::llInfo, "checkFiles" );

	for( LineInfo *curLine1 = info1; curLine1->lineNo != no_line; ++curLine1 )
	{
		if( curLine1->comparedLine == no_line )
		{
			size_t		idx2 = findMatchingLine( curLine1, info2 );
			if( idx2 != no_line )
			{
				connect( curLine1, info2 + idx2 );
			}
		}
	}
}

/*
	read files
*/
static LineInfo *readFile( const STRING &fileName, size_t *nLines )
{
	doEnterFunctionEx( gakLogging::llInfo, "readFile" );
	bool				ignoreSpaces = true;
	bool				space = false;
	RFILE				fp;
	Buffer<LineInfo>	fileInfo;
	size_t				lines = 0;
	size_t				maxLines = 0;
	STRING				lineBuffer;
	size_t				idx, lastChar;

	fp.open( fileName );
	while( true )
	{
		if( lines >= maxLines )
		{
			maxLines += 32;
			fileInfo.resize( maxLines * sizeof( LineInfo ) );
			if( !fileInfo )
/*v*/			break;
		}
		LineInfo	&curLine = fileInfo[lines];
		curLine.chksum = 0;
		curLine.lineNo = lines;

		curLine.comparedLine = no_line;
		curLine.position = fp.getpos();
		lineBuffer = fp.gets();
		if( fp.isEOF() )
		{
			curLine.lineNo = no_line;
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
					curLine.chksum++;
				space = true;
			}
			else
			{
				uint8 left = (uint8(idx&3)<<3);
				curLine.chksum += uint32(lineBuffer[idx]) << left;
				space = false;
			}
		}
		lines++;
	}
	fp.close();

	*nLines = lines +1;
	return fileInfo.release();
}

/*
	construct the difference information
*/
static STRING buildDiffFile(
	const char *file1, const char *file2,
	LineInfo *info1, LineInfo *info2
)
{
	doEnterFunctionEx( gakLogging::llInfo, "buildDiffFile" );
	STRING	diff;
	RFILE	fp1, fp2;
	STRING	lineBuff;

	// open the files
	fp1.open( file1 );
	fp2.open( file2 );

	size_t idx1 = 0;
	size_t idx2 = 0;

	while( 1 )
	{
		const LineInfo	&curLine1 = info1[idx1];
		const LineInfo	&curLine2 = info2[idx2];
		if( curLine1.lineNo == no_line && curLine2.lineNo == no_line )
			break;

		if( curLine1.lineNo != no_line
		&&  curLine1.comparedLine == no_line )
		{
			fp1.setpos( curLine1.position );
			lineBuff = fp1.gets();

			diff += formatNumber( curLine1.lineNo+1, 5 );

			diff += "       -<";
			diff += lineBuff;
			diff += '\n';
			idx1++;
		}
		else if( curLine2.lineNo != no_line
				&&  curLine2.comparedLine == no_line )
		{
			fp2.setpos( curLine2.position );
			lineBuff = fp2.gets();

			diff += "      ";
			diff += formatNumber( curLine2.lineNo+1, 5 );
			diff += " +>";
			diff += lineBuff;
			diff += '\n';
			idx2++;
		}
		else
		{
			if( curLine1.lineNo != no_line )
			{
				fp1.setpos( curLine1.position );
				lineBuff = fp1.gets();

				diff += formatNumber( curLine1.lineNo+1, 5 );
				diff += ' ';
				diff += formatNumber( curLine2.lineNo+1, 5 );
				diff += ' ';
#if 0			// for better debugging
				diff += formatNumber( curLine1.comparedLine+1, 5 );
				diff += ' ';
				diff += formatNumber( curLine2.comparedLine+1, 5 );
				diff += ' ';
				diff += formatNumber( curLine1.chksum() );
				diff += ' ';
				diff += formatNumber( curLine2.chksum() );
				diff += ' ';

				if(curLine1.lineNo != curLine2.comparedLine || curLine2.lineNo != curLine1.comparedLine )
					diff += "=========================================";
#else			// for testing
				assert(curLine1.lineNo == curLine2.comparedLine);
				assert(curLine2.lineNo == curLine1.comparedLine);
				assert(curLine1.chksum == curLine2.chksum);
#endif
				diff += "   ";

				diff += lineBuff;
				diff += '\n';
				idx1++;
			}
			if( curLine2.lineNo != no_line )
				idx2++;
		}
	}

	return diff;
}

/* --------------------------------------------------------------------- */
/* ----- entry points -------------------------------------------------- */
/* --------------------------------------------------------------------- */

const STRING &diff( const STRING &file1, const STRING &file2 )
{
	doEnterFunctionEx( gakLogging::llInfo, "diff" );

	static STRING	diffInfo;
	size_t			nLines1, nLines2;

	Buffer<LineInfo> info1( true, readFile( file1, &nLines1 ) );
	if( info1 )
	{
		Buffer<LineInfo> info2( true, readFile( file2, &nLines2 ) );
		if( info2 )
		{
			// check from beginig of file
			checkStart( info1, info2 );

			// check from end of file
			checkEnd( info1, nLines1, info2, nLines2 );

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
		}
	}

	return diffInfo;
}

}	// namespace gak
