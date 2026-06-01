/*
		Project:		GAKLIB
		Module:			MachineLearningTest.h
		Description:	Test for MachineLearning
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
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

#include <gak/MachineLearning.h>

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

static const double s_minVal = -5;
static const double s_maxVal = 5;
static const double s_increment = 0.1;

static const double s_searchM = -2.6;
static const double s_searchD = 1.8;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef Duo<double,double>	MyVariableType;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

namespace ai
{

template <>
class MLprocessor<double, MyVariableType, double>
{
	MyVariableType m_curVal;

	public:
	MyVariableType getFirstValue()
	{
		m_curVal = MyVariableType(s_minVal,s_minVal);
		return m_curVal;
	}
	MyVariableType getNextValue()
	{
		m_curVal.val2 += s_increment;
		if( m_curVal.val2 > s_maxVal )
		{
			m_curVal.val1 += s_increment;
			m_curVal.val2 = s_minVal;
		}
		return m_curVal;
	}
	MyVariableType getLastValue()
	{
		return MyVariableType(s_maxVal,s_maxVal);
	}
	double loss(double expected, double actual )
	{
		return gak::math::abs(expected - actual);
	}
	double process(double input, const MyVariableType &var )
	{
		return input * var.val1 + var.val2;
	}
};

}	// namespace ai

class MachineLearningTest : public UnitTest
{
	double	m_m, m_d;
	double linearFunc( double x )
	{
		return x*m_m + m_d;
	}
	virtual const char *GetClassName() const
	{
		return "MachineLearningTest";
	}
	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "MachineLearningTest::PerformTest");
		TestScope scope( "PerformTest" );

		m_m = s_searchM;
		m_d = s_searchD;

		ai::SupervisedLearning<double, MyVariableType, double>		mlSchool;

		size_t expectedLessonCount = 0;
		double inp = -60;
		double out = linearFunc( inp );
		mlSchool.learnLesson( inp, out );
		++expectedLessonCount;

		inp = -50;
		out = linearFunc( inp );
		mlSchool.learnLesson( inp, out );
		++expectedLessonCount;

		inp = 100;
		out = linearFunc( inp );
		mlSchool.learnLesson( inp, out );
		++expectedLessonCount;

		inp = 10;
		out = linearFunc( inp );
		mlSchool.learnLesson( inp, out );
		++expectedLessonCount;

		size_t	expectedVariableCount = size_t((s_maxVal - s_minVal)/s_increment + 1);
		size_t	lessonsCount, variableCount;
		expectedVariableCount *= expectedVariableCount;
		MyVariableType	best = mlSchool.getBest( &lessonsCount, &variableCount );

		UT_ASSERT_EQUAL( best.val1, m_m );
		UT_ASSERT_EQUAL( best.val2, m_d );
		UT_ASSERT_EQUAL( lessonsCount, expectedLessonCount );
		UT_ASSERT_EQUAL( variableCount, expectedVariableCount );
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static MachineLearningTest myMachineLearningTest;

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
