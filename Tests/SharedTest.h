/*
		Project:		GAKLIB
		Module:			SharedTest.h
		Description:	
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
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/shared.h>

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

class SharedTest : public UnitTest
{
	virtual const char *GetClassName( void ) const
	{
		return "SharedTest";
	}
	virtual void PerformTest( void )
	{
		SharedObjectPointerTest();
		SharedPointerTest();
	}
	void SharedObjectPointerTest( void )
	{
		{
			SharedObjectPointer<MySharedObject>	first = new MySharedObject( 666 );
			UT_ASSERT_EQUAL( 1, MySharedObject::s_objectCount );
			UT_ASSERT_TRUE( first && first->m_value == 666 );
			UT_ASSERT_FALSE( !first );
			UT_ASSERT_EQUAL( 666, first->m_value );
			UT_ASSERT_EQUAL( 666, (*first).m_value );
			
			SharedObjectPointer<MySharedObject>	second = new MySharedObject( 123 );
			UT_ASSERT_EQUAL( 2, MySharedObject::s_objectCount );
			UT_ASSERT_EQUAL( 123, second->m_value );
			UT_ASSERT_EQUAL( 123, (*second).m_value );

			second = first;
			UT_ASSERT_EQUAL( 1, MySharedObject::s_objectCount );
			UT_ASSERT_EQUAL( 666, second->m_value );
			UT_ASSERT_EQUAL( 666, (*second).m_value );
			
			second = NULL;
			UT_ASSERT_EQUAL( 1, MySharedObject::s_objectCount );
			
			first = NULL;
			UT_ASSERT_EQUAL( 0, MySharedObject::s_objectCount );

			UT_ASSERT_FALSE( bool( first ) );
			UT_ASSERT_TRUE( !first );
		}
		{
			const SharedObjectPointer<MySharedObject>	first = new MySharedObject( 666 );
			UT_ASSERT_TRUE( bool( first ) );
			UT_ASSERT_FALSE( !first );
			UT_ASSERT_EQUAL( 666, first->m_value );
			UT_ASSERT_EQUAL( 666, (*first).m_value );
		}
		UT_ASSERT_EQUAL( 0, MySharedObject::s_objectCount );
	}
	void SharedPointerTest( void )
	{
		{
			SharedPointer<MyObject>	first = SharedPointer<MyObject>::makeShared( MyObject( 666 ) );
			UT_ASSERT_EQUAL( 1, MyObject::s_objectCount );
			UT_ASSERT_TRUE( bool(first) && first->m_value == 666 );
			UT_ASSERT_FALSE( !first );
			UT_ASSERT_EQUAL( 666, first->m_value );
			UT_ASSERT_EQUAL( 666, (*first).m_value );
			
			SharedPointer<MyObject>	second = SharedPointer<MyObject>::makeShared( 123 );
			UT_ASSERT_EQUAL( 2, MyObject::s_objectCount );
			UT_ASSERT_EQUAL( 123, second->m_value );
			UT_ASSERT_EQUAL( 123, (*second).m_value );

			second = first;
			UT_ASSERT_EQUAL( 1, MyObject::s_objectCount );
			UT_ASSERT_EQUAL( 666, second->m_value );
			UT_ASSERT_EQUAL( 666, (*second).m_value );
			
			second = NULL;
			UT_ASSERT_EQUAL( 1, MyObject::s_objectCount );
			
			first = NULL;
			UT_ASSERT_EQUAL( 0, MyObject::s_objectCount );

			UT_ASSERT_FALSE( first );
			UT_ASSERT_TRUE( !first );
		}
		{
			const SharedPointer<MyObject>	first = SharedPointer<MyObject>::makeShared( MyObject( 666 ) );
			UT_ASSERT_EQUAL( 1, MyObject::s_objectCount );
			UT_ASSERT_TRUE( first );
			UT_ASSERT_FALSE( !first );
			UT_ASSERT_EQUAL( 666, first->m_value );
			UT_ASSERT_EQUAL( 666, (*first).m_value );
		}
		UT_ASSERT_EQUAL( 0, MySharedObject::s_objectCount );
	}

	struct MySharedObject : public SharedObject
	{
		static int	s_objectCount;
		int			m_value;

		MySharedObject( int value ) : m_value( value )
		{
			++s_objectCount;
		}
		MySharedObject( const MySharedObject &value ) : m_value( value.m_value )
		{
			++s_objectCount;
		}
		~MySharedObject()
		{
			--s_objectCount;
			assert( s_objectCount >= 0 );
		}
	};
	struct MyObject
	{
		static int	s_objectCount;
		int			m_value;

		MyObject( int value ) : m_value( value )
		{
			++s_objectCount;
		}
		MyObject( const MyObject &value ) : m_value( value.m_value )
		{
			++s_objectCount;
		}
		~MyObject()
		{
			--s_objectCount;
		}
	};
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static SharedTest mySharedTest;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

int	SharedTest::MySharedObject::s_objectCount = 0;
int	SharedTest::MyObject::s_objectCount = 0;

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

