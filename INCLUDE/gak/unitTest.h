/*
		Project:		GAKLIB
		Module:			unitTest.h
		Description:	Base class for unit tests
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

#ifndef GAK_UNIT_TESTS_H
#define GAK_UNIT_TESTS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/stopWatch.h>
#include <gak/array.h>
#include <gak/sortedArray.h>
#include <gak/fmtNumber.h>
#include <gak/stringStream.h>
#include <gak/locker.h>
#include <gak/math.h>
#include <gak/logfile.h>
#include <gak/thread.h>
#include <gak/exception.h>

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

#define DISABLED_TEST_PREFIX	"Disabled_"

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

#define UT_EXPECT_TRUE( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem, "false", bool(testItem), false )
#define UT_EXPECT_FALSE( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, "!" #testItem, "true", !bool(testItem), false )
#define UT_EXPECT_NULL( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem " == NULL", "!NULL", bool(testItem == NULL), false )
#define UT_EXPECT_NOT_NULL( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem " != NULL", "NULL", bool(testItem != NULL), false )

#define UT_EXPECT_RANGE( min, val, max )		\
	gak::testRange( GetClassName(), __FILE__, __LINE__, #min "<=" #val "<=" #max, min, val, max, false )

#define UT_EXPECT_EQUAL( i1, i2 )		\
	gak::testEqual( GetClassName(), __FILE__, __LINE__, #i1 "==" #i2, i1, i2, false )
#define UT_EXPECT_NOT_EQUAL( i1, i2 )		\
	gak::testNotEqual( GetClassName(), __FILE__, __LINE__, #i1 "!=" #i2, i1, i2, false )
#define UT_EXPECT_LESS( i1, i2 )		\
	gak::testLess( GetClassName(), __FILE__, __LINE__, #i1 "<" #i2, i1, i2, false )
#define UT_EXPECT_LESSEQ( i1, i2 )		\
	gak::testLessEqual( GetClassName(), __FILE__, __LINE__, #i1 "<=" #i2, i1, i2, false )
#define UT_EXPECT_GREATER( i1, i2 )		\
	gak::testGreater( GetClassName(), __FILE__, __LINE__, #i1 ">" #i2, i1, i2, false )
#define UT_EXPECT_GREATEREQ( i1, i2 )		\
	gak::testGreaterEqual( GetClassName(), __FILE__, __LINE__, #i1 ">=" #i2, i1, i2, false )

#define UT_EXPECT_EQUAL_FLT( i1, i2, maxDev )		\
	gak::testEqualFloat( GetClassName(), __FILE__, __LINE__, #i1 "==" #i2, i1, i2, maxDev, false )

#define UT_EXPECT_EXCEPTION( expr, Except )	\
{ \
	bool	exFound = false; \
	try \
	{ \
		(expr); \
	} \
	catch( Except & ) \
	{ \
		exFound = true; \
	} \
	catch( ... ) \
	{ \
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #expr " throws " #Except, #Except " not thrown, bad exception", false, false ); \
	} \
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #expr " throws " #Except, #Except " not thrown", exFound, false ); \
}


#define UT_ASSERT_TRUE( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem, "false", bool(testItem), true )
#define UT_ASSERT_FALSE( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, "!" #testItem, "true", !bool(testItem), true )
#define UT_ASSERT_NULL( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem " == NULL", "!NULL", bool(testItem == NULL), true )
#define UT_ASSERT_NOT_NULL( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem " != NULL", "NULL", bool(testItem != NULL), true )
#define UT_ASSERT_RANGE( min, val, max )		\
	gak::testRange( GetClassName(), __FILE__, __LINE__, #min "<=" #val "<=" #max, min, val, max, true )
#define UT_ASSERT_EQUAL( i1, i2 )		\
	gak::testEqual( GetClassName(), __FILE__, __LINE__, #i1 "==" #i2, i1, i2, true )

#define UT_ASSERT_NOT_EQUAL( i1, i2 )		\
	gak::testNotEqual( GetClassName(), __FILE__, __LINE__, #i1 "!=" #i2, i1, i2, true )
#define UT_ASSERT_LESS( i1, i2 )		\
	gak::testLess( GetClassName(), __FILE__, __LINE__, #i1 "<" #i2, i1, i2, true )
#define UT_ASSERT_LESSEQ( i1, i2 )		\
	gak::testLessEqual( GetClassName(), __FILE__, __LINE__, #i1 "<=" #i2, i1, i2, true )
#define UT_ASSERT_GREATER( i1, i2 )		\
	gak::testGreater( GetClassName(), __FILE__, __LINE__, #i1 ">" #i2, i1, i2, true )
#define UT_ASSERT_GREATEREQ( i1, i2 )		\
	gak::testGreaterEqual( GetClassName(), __FILE__, __LINE__, #i1 ">=" #i2, i1, i2, true )
#define UT_ASSERT_EQUAL_FLT( i1, i2, maxDev )		\
	gak::testEqualFloat( GetClassName(), __FILE__, __LINE__, #i1 "==" #i2, i1, i2, maxDev, true )
	

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

struct TestResult
{
	const char	*m_srcFileName;
	int			m_srcFileLine;
	const char	*m_className;
	const char	*m_testItem;
	const char	*m_scope;
	STRING		m_actualValue;
	bool		m_success;

	std::ostream &toFmtStream( std::ostream &out ) const
	{
		doEnterFunctionEx(gakLogging::llDetail, "TestResult::toFmtStream");
		doLogValueEx(gakLogging::llDetail, m_srcFileName);
		doLogValueEx(gakLogging::llDetail, m_srcFileLine);
		doLogValueEx(gakLogging::llDetail, m_className);
		doLogValueEx(gakLogging::llDetail, m_testItem);

		out << m_srcFileName << ' ' << m_srcFileLine << ' ' << m_className;
		if( m_scope && *m_scope )
		{
			out << " Scope: " << m_scope;
		}
		out << ' ' << (m_success ? "OK" : "FAILED") << ": " << m_testItem
			<< " found: " << m_actualValue.convertToTerminal()
		;
		return out;
	}
};

struct StressResult
{
	STRING	testName;
	size_t	goodCount, badCount;
	clock_t	goodTime, badTime;
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class UnitTestException : public LibraryException
{
	public:
	UnitTestException( const char *testItem ) : LibraryException(STRING("Unit Test Assertion Failed: ").add(testItem) )
	{}
};

class UnitTest
{
	enum TestMode { tmTest, tmStress, tmThread };
	static Array<TestResult>	s_theTestResults;
	static Array<StressResult>	s_theStressResults;
	static Stack<const char *>	s_scopes;				/// TODO make this thread local
	static std::size_t			s_errorCount;

	protected:
	static Locker				s_testLocker;

	private:
	Hours<clock_t>				m_ellapsedTime;
	bool						m_tested;

	private:
	static Array<UnitTest*> &getTheTestItems();
	static bool PerformTest( UnitTest *theTest, bool catchCout );
	static bool StressTest( UnitTest *theTest );
	static bool ThreadTest( UnitTest *theTest, void *pool );

	static void PerformTests( SortedArray<const char*> &testsToPerform, bool catchCout, bool checkTested );
	static void StressTests( SortedArray<const char*> &testsToPerform, bool checkTested );
	static void ThreadTest( SortedArray<const char*> &testsToPerform, bool checkTested );

	static void ShowNotFound( const SortedArray<const char*> &testsToPerform );

	virtual void StressTest( size_t /* factor */ )
	{
	}
	virtual bool canStressTest()
	{
		return false;
	}
	virtual bool canThreadTest()
	{
		return false;
	}
	virtual UnitTest *duplicate()
	{
		return nullptr;
	}
	virtual void PerformTest() = 0;
	virtual const char *GetClassName() const = 0;
	bool isDisabled() const
	{
		return !strncmp(GetClassName(), DISABLED_TEST_PREFIX, sizeof(DISABLED_TEST_PREFIX)-1 );
	}

	friend class TestScope;
	static void AddScope( const char *scope )
	{
		s_scopes.push( scope );
	}
	static void EndScope()
	{
		s_scopes.pop();
	}

	public:
	UnitTest( bool isStatic = true )
	{
		m_ellapsedTime = 0;
		m_tested = false;

		if( isStatic )
		{
			getTheTestItems() += this;
		}
	}

	static void PerformTests( const char *argv[] );
	static void PrintResult();
	virtual void PerformThreadTest();
	static void AddResult(
		const char		*className,
		const char		*srcFileName,
		int				srcFileLine,
		const char		*testItem,
		const STRING	&actualValue,
		bool			success,
		bool			throwException
	)
	{
		LockGuard	lock( s_testLocker );

		TestResult	&newResult = s_theTestResults.createElement();
		newResult.m_className = className;
		newResult.m_srcFileName = srcFileName;
		newResult.m_srcFileLine = srcFileLine;
		newResult.m_testItem = testItem;
		newResult.m_actualValue = actualValue;
		newResult.m_success = success;
		
		if( Thread::isMainThread() )
		{
			newResult.m_scope = s_scopes.size() ? s_scopes.top() : "";
		}
		else
		{
			newResult.m_scope = "Thread";
		}
		if( !success )
		{
			++s_errorCount;
			//std::cerr << actualValue << std::endl;
			if( throwException )
			{
				throw UnitTestException(testItem);
			}
		}
	}
};

class TestScope
{
	public:
	TestScope( const char *scope )
	{
		doLogValue( scope );
		if( Thread::isMainThread() )
			UnitTest::AddScope( scope );
	}
	~TestScope()
	{
		if( Thread::isMainThread() )
			UnitTest::EndScope();
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

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

inline std::ostream &operator << ( std::ostream &out, const TestResult &theResult )
{
	theResult.toFmtStream( out );
	return out;
}

template <class ITEM>
void testRange(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &minVal, const ITEM &val, const ITEM &maxVal,
	bool throwException = false
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = minVal <= val && val <= maxVal;;

	logStream << val << " not in [" << minVal << ',' << maxVal << ']';
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <class ITEM>
void testEqualFloat(
	const char *className, const char *fileName, int line,
	const char *testItem,
	ITEM i1, ITEM i2,
	double iMaxDelata,
	bool throwException=false
)
{
	const double	myDeltaFactor = 50;
	int				exponent = math::getExponent(i1);
	int				maxExp = exponent - std::numeric_limits<ITEM>::digits10;
	double			maxDelta = iMaxDelata ? iMaxDelata : myDeltaFactor * pow( 10.0, maxExp );

	STRING			log;
	oSTRINGstream	logStream( log );
	ITEM			diff = fabs(i1 - i2);
	bool			success = (diff < maxDelta);

	logStream << formatNumber( i1 ) << " != " << formatNumber( i2 ) << " (diff=" << diff << " maxDelta=" << maxDelta << ')';
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <class ITEM1, class ITEM2> 
void testEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM1 &i1, const ITEM2 &i2,
	bool throwException = false
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 == i2;

	logStream << i1 << " != " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <>
inline void testEqual<double>(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const double &i1, const double &i2,
	bool throwException
)
{
	testEqualFloat( className, fileName, line, testItem, i1, i2, 0, throwException );
}

template <>
void testEqual<const char *>(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const char * const &i1, const char * const &i2,
	bool throwException
);

template <class ITEM> 
void testNotEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2,
	bool throwException
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 != i2;

	logStream << i1 << " == " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <> 
void testNotEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const char * const &i1, const char * const &i2,
	bool throwException
);

template <class ITEM> 
void testLess(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2,
	bool throwException
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 < i2;

	logStream << i1 << " >= " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <class ITEM> 
void testLessEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2,
	bool throwException
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 <= i2;

	logStream << i1 << " > " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <class ITEM> 
void testGreater(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2,
	bool throwException
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 > i2;

	logStream << i1 << " <= " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

template <class ITEM> 
void testGreaterEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2,
	bool throwException
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = (i1 >= i2);

	logStream << i1 << " < " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success, throwException );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
