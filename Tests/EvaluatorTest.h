/*
		Project:		GAKLIB
		Module:			EvaluatorTest.h
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

#include <gak/expressionEvaluator.h>

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

class EvaluatorTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "EvaluatorTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "EvaluatorTest::PerformTest");
		TestScope scope( "PerformTest" );

		MathExpression	evaluator;

		double	result = evaluator.evaluate( "2 * 3 + 5" );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 11.0 );

		result = evaluator.evaluate( "5 - 3 - 2" );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 0.0 );

		result = evaluator.evaluate( "( 3 + 5 ) * 2" );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 16.0 );

		result = evaluator.evaluate( "( 3 + x ) * 2", 5 );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 16.0 );

		result = evaluator.evaluate( "(+x) * (-x)", 5 );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, -25.0 );

		result = evaluator.evaluate( "sin(pi():2)" );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 1.0 );

		result = evaluator.evaluate( "cos(pi)" );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, -1.0 );

		result = evaluator.evaluate( "x^2 + x*x", 5 );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 50.0 );

		result = evaluator.evaluate( "x! + 5*x", 5 );
		std::cout << result << std::endl;
		UT_ASSERT_EQUAL( result, 145.0 );

		bool exceptionFound = false;

		try
		{
			result = evaluator.evaluate( "1:x", 0 );
			std::cout << result << std::endl;
		}
		catch( DivisionByZeroError & )
		{
			exceptionFound = true;
		}
		UT_ASSERT_EQUAL( exceptionFound, true );

		exceptionFound = false;
		try
		{
			result = evaluator.evaluate( "sqrt(x)", -5 );
			std::cout << result << std::endl;
		}
		catch( InvalidResultError & )
		{
			exceptionFound = true;
		}
		UT_ASSERT_EQUAL( exceptionFound, true );
		exceptionFound = false;
		try
		{
			result = evaluator.evaluate( "x!", 1000 );
			std::cout << result << std::endl;
		}
		catch( InvalidResultError & )
		{
			exceptionFound = true;
		}
		UT_ASSERT_EQUAL( exceptionFound, true );

	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static EvaluatorTest myEvaluatorTest;

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

