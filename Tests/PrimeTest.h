/*
		Project:		GAKLIB
		Module:			PrimeTest.h
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

class PrimeTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "PrimeTest";
	}
	void show( const math::PrimeFactorBuffer &pf )
	{
		for( math::PrimeFactorBuffer::const_iterator it = pf.cbegin(); *it; ++it )
		{
			std::cout << *it++ << ' ';
		}
		std::cout << '\n';
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "PrimeTest::PerformTest");
		TestScope scope( "PerformTest" );

		math::PrimeFactorBuffer	pfs;
		uint32 					n = uint32(-1);

		std::cout << "Prime Factors 0 ";
		pfs = math::getPrimeFactors( 0 );
		show( pfs );

		std::cout << "Prime Factors 1 ";
		pfs = math::getPrimeFactors( 1 );
		show( pfs );

		std::cout << "Prime Factors 2 ";
		pfs = math::getPrimeFactors( 2 );
		show( pfs );

		std::cout << "Prime Factors 3 ";
		pfs = math::getPrimeFactors( 3 );
		show( pfs );

		std::cout << "Prime Factors 10 ";
		pfs = math::getPrimeFactors( 10 );
		show( pfs );

		std::cout << "Prime Factors 17 ";
		pfs = math::getPrimeFactors( 17 );
		show( pfs );

		std::cout << "Prime Factors 99 ";
		pfs = math::getPrimeFactors( 99 );
		show( pfs );

		std::cout << "Prime Factors 1<<31 ";
		pfs = math::getPrimeFactors( 1UL<<31UL );
		show( pfs );

		std::cout << "Prime Factors 1<<5 ";
		pfs = math::getPrimeFactors( 1UL<<5UL );
		show( pfs );

		std::cout << "Prime Factors (unsigned long)-1 ";
		pfs = math::getPrimeFactors( (unsigned long)-1 );
		show( pfs );

		while( n )
		{
			if( math::isPrime( n ) )
			{
				std::cout << n << '\n';
				UT_ASSERT_EQUAL( n, uint32(4294967291U) );
				n=1;
			}
			n--;
		}

		UT_ASSERT_EQUAL( math::ggT( 64, 100 ), 4UL );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static PrimeTest myPrimeTest;

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
#endif

