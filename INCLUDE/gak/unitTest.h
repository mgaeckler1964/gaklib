/*
		Project:		GAKLIB
		Module:			unitTest.h
		Description:	Base class for unit tests
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

#define UT_ASSERT_TRUE( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem, "false", bool(testItem) )
#define UT_ASSERT_FALSE( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, "!" #testItem, "true", !bool(testItem) )
#define UT_ASSERT_NULL( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem " == NULL", "!NULL", bool(testItem == NULL) )
#define UT_ASSERT_NOT_NULL( testItem )	\
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #testItem " != NULL", "NULL", bool(testItem != NULL) )

#define UT_ASSERT_RANGE( min, val, max )		\
	gak::assertRange( GetClassName(), __FILE__, __LINE__, #min "<=" #val "<=" #max, min, val, max )

#define UT_ASSERT_EQUAL( i1, i2 )		\
	gak::assertEqual( GetClassName(), __FILE__, __LINE__, #i1 "==" #i2, i1, i2 )
#define UT_ASSERT_NOT_EQUAL( i1, i2 )		\
	gak::assertNotEqual( GetClassName(), __FILE__, __LINE__, #i1 "!=" #i2, i1, i2 )
#define UT_ASSERT_LESS( i1, i2 )		\
	gak::assertLess( GetClassName(), __FILE__, __LINE__, #i1 "<" #i2, i1, i2 )
#define UT_ASSERT_LESSEQ( i1, i2 )		\
	gak::assertLessEqual( GetClassName(), __FILE__, __LINE__, #i1 "<=" #i2, i1, i2 )
#define UT_ASSERT_GREATER( i1, i2 )		\
	gak::assertGreater( GetClassName(), __FILE__, __LINE__, #i1 ">" #i2, i1, i2 )
#define UT_ASSERT_GREATEREQ( i1, i2 )		\
	gak::assertGreaterEqual( GetClassName(), __FILE__, __LINE__, #i1 ">=" #i2, i1, i2 )

#define UT_ASSERT_EXCEPTION( expr, Except )	\
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
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #expr " throws " #Except, #Except " not thrown, bad exception", false ); \
	} \
	UnitTest::AddResult( GetClassName(), __FILE__, __LINE__, #expr " throws " #Except, #Except " not thrown", exFound ); \
}

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
	static void PerformTest( UnitTest *theTest, bool catchCout );
	static void StressTest( UnitTest *theTest );
	static void ThreadTest( UnitTest *theTest, void *pool );

	static void PerformTests( SortedArray<const char*> &testsToPerform, bool catchCout );
	static void StressTests( SortedArray<const char*> &testsToPerform );
	static void ThreadTest( SortedArray<const char*> &testsToPerform );

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
		bool			success
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
void assertRange(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &minVal, const ITEM &val, const ITEM &maxVal
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = minVal <= val && val <= maxVal;;

	logStream << val << " not in [" << minVal << ',' << maxVal << ']';
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM>
void assertEqualFloat(
	const char *className, const char *fileName, int line,
	const char *testItem,
	ITEM i1, ITEM i2
)
{
	const double	myDeltaFactor = 50;
	int				exponent = math::getExponent(i1);
	int				maxExp = exponent - std::numeric_limits<ITEM>::digits10;
	double			maxDelta = myDeltaFactor * pow( 10.0, maxExp );

	STRING			log;
	oSTRINGstream	logStream( log );
	ITEM			diff = fabs(i1 - i2);
	bool			success = (diff < maxDelta);

	logStream << formatNumber( i1 ) << " != " << formatNumber( i2 ) << " (diff=" << diff << " maxDelta=" << maxDelta << ')';
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM1, class ITEM2> 
void assertEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM1 &i1, const ITEM2 &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 == i2;

	logStream << i1 << " != " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <>
inline void assertEqual<double>(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const double &i1, const double &i2
)
{
	assertEqualFloat( className, fileName, line, testItem, i1, i2 );
}

template <>
inline void assertEqual<const char *>(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const char * const &i1, const char * const &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 == i2 || !strcmp(i1 ? i1 : "NULL", i2 ? i2 : "NULL");

	logStream << (i1 ? i1 : "NULL") << " != " << (i2 ? i2 : "NULL");
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM> 
void assertNotEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 != i2;

	logStream << i1 << " == " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM> 
void assertLess(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 < i2;

	logStream << i1 << " >= " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM> 
void assertLessEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 <= i2;

	logStream << i1 << " > " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM> 
void assertGreater(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = i1 > i2;

	logStream << i1 << " <= " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

template <class ITEM> 
void assertGreaterEqual(
	const char *className, const char *fileName, int line,
	const char *testItem,
	const ITEM &i1, const ITEM &i2
)
{
	STRING			log;
	oSTRINGstream	logStream( log );
	bool			success = (i1 >= i2);

	logStream << i1 << " < " << i2;
	logStream.flush();

	UnitTest::AddResult( className, fileName, line, testItem, log, success );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
