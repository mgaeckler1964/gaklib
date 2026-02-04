/*
		Project:		GAKLIB
		Module:			CmdlineTest.h
		Description:	the command line and parameter handling
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/cmdlineParser.h>

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

#	pragma warn -inl
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

class CmdlineTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "CmdlineTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "CmdlineTest::PerformTest");
		TestScope scope( "PerformTest" );

		static const int flag1Present	= 0x10;
		static const int flag2Present	= 0x20;
		static const int flag3Present	= 0x40;
		static const int optionPresent	= 0x80;
		static gak::CommandLine::Options options[] =
		{
			{ 'D', "option",		0, 99, optionPresent|CommandLine::needArg, "<option>" },
			{ 'F', "flag1",			0, 1, flag1Present },
			{ 'X', "flag2",			0, 1, flag2Present },
			{ 'Y', "flag3",			0, 1, flag3Present },
			{ 0 },
		};

		const char *scopes[] =
		{
			"no args",
			"command line",
			"config file",
			"command & config"
		};

		for( int x=0; x<=3; ++x )
		{
			TestScope scope( scopes[x] );

			const char *argv[] =
			{
				"test.EXE",
				"otherParam1",
				"-D=optCharWithParam",
				"otherParam2",
				"-D:optCharWithParam",
				"otherParam3",
				"-D", "optCharWithParam",
				"otherParam4",
				"--option=optStringWithParam",
				"otherParam5",
				"--option:optStringWithParam",
				"otherParam6",
				"--option", "optStringWithParam",
				"otherParam7",
				"-F",
				"otherParam8",
				"--flag2",
				"otherParam9",
				NULL
			};
			const char *noargv[] =
			{
				"test.EXE",
				NULL
			};

			TempFileName	cfg( STRING(".test.cfg") );
			if( x&2 )
			{
				std::ofstream out( cfg.c_str() );
				out << "-D=fromConfig\n";
			}
			else
				strRemove(STRING(".test.cfg"));

			CommandLine	cmdLine( options, x & 1 ? argv : noargv );

			if( x )
			{
				UT_ASSERT_EQUAL( cmdLine.flags & optionPresent, optionPresent );
				if( x&1 )
				{
					UT_ASSERT_EQUAL( cmdLine.flags & flag1Present, flag1Present );
					UT_ASSERT_EQUAL( cmdLine.flags & flag2Present, flag2Present );
				}
				else
				{
					UT_ASSERT_EQUAL( cmdLine.flags & flag1Present, 0 );
					UT_ASSERT_EQUAL( cmdLine.flags & flag2Present, 0 );
				}
				UT_ASSERT_EQUAL( cmdLine.flags & flag3Present, 0 );
			}
			else
				UT_ASSERT_EQUAL( cmdLine.flags, 0 );

			int index = 0;
			size_t expectedCount = 0;
			if( x&1 )
				expectedCount += 6;
			if( x&2 )
				expectedCount += 1;

			UT_ASSERT_EQUAL( cmdLine.parameter['D'].size(), expectedCount );
			if( x&2 )
			{
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("fromConfig") );
			}
			if( x&1 )
			{
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("optCharWithParam") );
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("optCharWithParam") );
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("optCharWithParam") );
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("optStringWithParam") );
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("optStringWithParam") );
				UT_ASSERT_EQUAL( cmdLine.parameter['D'][index++], STRING("optStringWithParam") );
			}

			if( x&1 )
			{
				UT_ASSERT_EQUAL( cmdLine.argc, 10 );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[0], (const char *)"test.EXE" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[1], (const char *)"otherParam1" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[2], (const char *)"otherParam2" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[3], (const char *)"otherParam3" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[4], (const char *)"otherParam4" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[5], (const char *)"otherParam5" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[6], (const char *)"otherParam6" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[7], (const char *)"otherParam7" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[8], (const char *)"otherParam8" );
				UT_ASSERT_EQUAL( (const char *)cmdLine.argv[9], (const char *)"otherParam9" );
				UT_ASSERT_EQUAL( (void *)cmdLine.argv[10], (void *)NULL );
			}
			else
			{
				UT_ASSERT_EQUAL( cmdLine.argc, 1 );
			}
		}
	}
};
// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static CmdlineTest myCmdlineTest;

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

#	pragma warn +inl
#endif

