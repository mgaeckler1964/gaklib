/*
		Project:		GAKLIB
		Module:			StringStreamTest.h
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

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

#include <gak/stringStream.h>

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

class StrStreamTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "StrStreamTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "StrStreamTest::PerformTest");
		TestScope scope( "PerformTest" );
		{
			STRING			theResult;
			oSTRINGstream	output( theResult );

			int		hello = 1;
			char	c;
			size_t	i;

			output << hello << '\n';
			output.flush();

			UT_ASSERT_EQUAL( theResult, STRING("1\n") );

			output << "The Quick Brown Fox Jumps Over The Lazy Dog\n";
			output.flush();

			UT_ASSERT_EQUAL( theResult, STRING("1\nThe Quick Brown Fox Jumps Over The Lazy Dog\n") );

			iSTRINGstream	input( theResult );
//			istrstream		input( theResult );

			input >> hello;

			UT_ASSERT_EQUAL( hello, 1 );

			input.unsetf( std::ios_base::skipws );


			i=1;
			while( true )
			{
				input >> c;
				if( input.eof() )
					break;
				std::cout << c;
				UT_ASSERT_EQUAL( c, theResult[i] );
				i++;
			}
			input.clear();

			input.seekg( 6 );
			input >> c;
			UT_ASSERT_EQUAL( c, 'Q' );

			input.seekg( 5, input.cur );
			input >> c;
			UT_ASSERT_EQUAL( c, 'B' );
#if 0
			input.clear();
			input.seekg( 0, ios::beg );

			cout << '\n';
			while( true )
			{
				input >> c;
				if( input.eof() )
					break;

				input.seekg( 1, ios::cur );
				cout << c;

			}
#endif
		}
		test<std::vector<char>,		oVectorStream,	iVectorStream>	("<std::vector<char>, oVectorStream, iVectorStream>");
		test<std::vector<wchar_t>,	woVectorStream,	wiVectorStream>	("<std::vector<wchar_t>, woVectorStream, wiVectorStream>");
		test<Array<char>,			oArrayStream,	iArrayStream>	("<Array<char>, oArrayStream, iArrayStream>");
		test<Array<wchar_t>,		woArrayStream,	wiArrayStream>	("<Array<wchar_t>, woArrayStream, wiArrayStream>");
		test<STRING,				oSTRINGstream,	iSTRINGstream>	("<STRING, oSTRINGstream, iSTRINGstream>");
		test<uSTRING,				woSTRINGstream,	wiSTRINGstream>	("<uSTRING, woSTRINGstream, wiSTRINGstream>");
	}
	template <class ContainerT, class oStreamT, class iStreamT>
	void test( const char *iScope ) const
	{
		TestScope scope( iScope );

		int			myTmp;
		ContainerT	buffer;
		oStreamT	output( buffer );
		iStreamT	input( buffer );

		output	<< 9 << std::endl;
		typename ContainerT::value_type expected( '9' );
		UT_ASSERT_EQUAL( buffer[0U], expected );
		UT_ASSERT_EQUAL( size_t(buffer.size()), size_t(2) ); 

		typename iStreamT::int_type c = input.get();
		typename iStreamT::int_type x = '9';
		UT_ASSERT_EQUAL( c, x );
		input.putback( c );

		input >> myTmp;
		UT_ASSERT_EQUAL( myTmp, 9 );

		input.putback( 'a' );
		c = input.get();
		x = 'a';
		UT_ASSERT_EQUAL( c, x );

		c = input.get();
		x = '\n';
		UT_ASSERT_EQUAL( c, x );

		c = input.get();
		UT_ASSERT_EQUAL( c, iStreamT::traits_type::eof() );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static StrStreamTest myStrStreamTest;

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
