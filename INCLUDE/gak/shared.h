/*
		Project:		GAKLIB
		Module:			shared.h
		Description:	Template for smart pointer that can automaticaly 
						destroy objects no longer used
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

#ifndef SHARED_OBJECT_H
#define SHARED_OBJECT_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <assert.h>

#include <gak/locker.h>

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
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

/**
	@brief Base class for all objects that can be automaticaly deleted
	@see SharedObjectPointer
*/
class SharedObject
{
	Critical	m_critical;
	int			m_usageCount;
	bool		m_autoDelete;

	protected:
	/**
		@brief creates a new object
		@param [in] autoDelete if true, the item will be automaticaly deleted if there is no SharedObjectPointer pointing to this item
	*/
	SharedObject( bool autoDelete=true )
	{
		m_usageCount = 0;
		m_autoDelete = autoDelete;
	}
	~SharedObject()
	{
		assert( !m_usageCount );
	}

	public:
	/// Increments the usage counter
	void incUC()
	{
		CriticalScope scope( m_critical );
		m_usageCount++;
	}
	/// Decrements the usage counter
	int decUC()
	{
		CriticalScope scope( m_critical );
		--m_usageCount;
		return m_usageCount;
	}
	/// Disables the automatoc deletion
	void disableAutoDelete()
	{
		m_autoDelete = false;
	}
	/// Enables the automatoc deletion
	void enableAutoDelete()
	{
		m_autoDelete = true;
	}
	/// Returns true if the item can be automatically deleted
	bool canDelete() const
	{
		return m_autoDelete;
	}
	/// Returns the number of SharedObjectPointer objects pointing to this item
	int getUsageCount() const
	{
		return m_usageCount;
	}
};

/**
	@brief A shared pointer pointing to an element derived from SharedObject

	@code
		class MyType : public SharedObject
		{
			...
		};
		SharedObjectPointer<MyType>	myPointer = new MyType();
		...
		*myPointer = ...;
		myPointer->xxx = ...;
	@endcode
	@tparam A class derived from SharedObject
*/
template <typename ShObj>
class SharedObjectPointer
{
	private:
	ShObj	*m_addr;

	private:
	void clrPointer()
	{
		if( m_addr && !m_addr->decUC() && m_addr->SharedObject::canDelete() )
		{
			delete m_addr;
		}
	}
	void setPointer( ShObj *addr )
	{
		m_addr = addr;
		if( addr )
		{
			addr->incUC();
		}
	}

	protected:
	/// Returns the address of the item this pointer points to
	ShObj * get() const
	{
		assert( !m_addr || m_addr->getUsageCount() > 0 );
		return m_addr;
	}

	/**
		@brief Assigns a new pointer
		@param [in] addr the address of an item this pointer should point to
		@return the pointer itself
	*/
	void changePointer( ShObj *addr )
	{
		if( m_addr != addr )
		{
			clrPointer();
			setPointer( addr );
		}
	}
	public:
	/**
		@brief Creates a new pointer
		@param [in] addr the address of an item this pointer should point to
	*/
	SharedObjectPointer( const ShObj *addr = NULL )
	{
		setPointer( const_cast<ShObj *>(addr) );
	}
	/// A copy constructor
	SharedObjectPointer( const SharedObjectPointer &src )
	{
		setPointer( src.m_addr );
	}

	/**
		@brief Assigns a new pointer
		@param [in] addr The address of an item this pointer should point to
		@return The pointer itself
	*/
	SharedObjectPointer &operator = ( const ShObj *addr )
	{
		changePointer( const_cast<ShObj *>(addr) );
		return *this;
	}
	/**
		@brief Assigns a new pointer
		@param [in] src the SharedObjectPointer object with the address of an item this pointer should point to
		@return the pointer itself
	*/
	SharedObjectPointer &operator = ( const SharedObjectPointer &src )
	{
		changePointer( src.m_addr );
		return *this;
	}
	/// Destroys the pointer, if it is the last pointer the item will be deleted, too
	~SharedObjectPointer()
	{
		clrPointer();
	}

	/// Returns the address of the item this pointer points to
	operator ShObj * () const
	{
		return get();
	}
	/// Returns the address of the item this pointer points to
	ShObj *operator -> () const
	{
		return get();
	}
	/// Returns the reference of the item this pointer points to
	ShObj &operator  * () const
	{
		return *get();
	}

	/// Returns true, if this pointer points to no item (NULL)
	bool operator ! () const
	{
		assert( !m_addr || m_addr->getUsageCount() > 0 );
		return m_addr == NULL;
	}

	/**
		@brief Compares this pointer with another pointer
		@return The compare result
	*/
	int compare( const SharedObjectPointer &oper ) const
	{
		if( m_addr < oper.m_addr )
			return -1;
		if( m_addr > oper.m_addr )
			return 1;
		return 0;

//		return gak::compare<void*>( addr, oper.addr );
	}
};

///@cond
namespace internal
{
	template<class OBJ>
	class ShObjContainer : public SharedObject
	{
		OBJ	m_item;

		public:
		ShObjContainer() : m_item()
		{
		}
		ShObjContainer( const OBJ &item ) : m_item( item )
		{
		}
		template<typename ArgT1>
		ShObjContainer( const ArgT1 &arg1 ) : m_item( arg1 )
		{
		}
		template<typename ArgT1, typename ArgT2>
		ShObjContainer( const ArgT1 &arg1, const ArgT2 &arg2 ) : m_item( arg1, arg2 )
		{
		}
		template<typename ArgT1, typename ArgT2, typename ArgT3>
		ShObjContainer( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3 ) : m_item( arg1, arg2, arg3 )
		{
		}
		template<typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4>
		ShObjContainer( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3, const ArgT4 &arg4 ) : m_item( arg1, arg2, arg3, arg4 )
		{
		}
		template<typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5>
		ShObjContainer( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3, const ArgT4 &arg4, const ArgT5 &arg5 ) 
		: m_item( arg1, arg2, arg3, arg4, arg5 )
		{
		}
		template<typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6>
		ShObjContainer( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3, const ArgT4 &arg4, const ArgT5 &arg5, const ArgT6 &arg6 ) 
		: m_item( arg1, arg2, arg3, arg4, arg5, arg6 )
		{
		}

		const OBJ &get() const
		{
			return m_item;
		}
		OBJ &get()
		{
			return m_item;
		}
	};
}	// namespace internal
///@endcond

/**
	@brief A shared pointer that can (unlike SharedObjectPointer) point to any type

	@code
		class MyType
		{
			...
		};
		SharedPointer<MyType>	myPointer = SharedPointer<MyType>::makeShared();
		...
		*myPointer = ...;
		myPointer->xxx = ...;
	@endcode

	@tparam OBJ the type this pointer points to
*/
template<class OBJ>
class SharedPointer
{
	typedef internal::ShObjContainer<OBJ>	ShObj;
	typedef SharedObjectPointer<ShObj>		MySharedObjectPointer;

	MySharedObjectPointer					m_shPointer;

	SharedPointer( ShObj *addr ) : m_shPointer( addr )
	{
	}

	public:
	SharedPointer()
	{
	}

	/**
		@brief Assigns a new address to this pointer
		@param [in] addr The address of the container item (typically this can only be NULL)
		@return The pointer itself
	*/
	SharedPointer &operator = ( const ShObj *addr )
	{
		m_shPointer = addr;
		return *this;
	}

	/// Returns the address of the item this pointer points to
	operator OBJ * () const
	{
		return m_shPointer ? &(m_shPointer->get()) : NULL;
	}
	/// Returns the address of the item this pointer points to
	OBJ *operator -> () const
	{
		return m_shPointer ? &(m_shPointer->get()) : NULL;
	}

	/// Returns the reference of the item this pointer points to
	OBJ &operator  * () const
	{
		return m_shPointer->get();
	}

	/// Returns true, if this pointer points to no item (NULL)
	bool operator ! () const
	{
		return !m_shPointer;
	}

	/**
		@brief Creates a new item and a new pointer that points to the new item
		@return the new SharedPointer pointer
	*/
	static SharedPointer makeShared()
	{
		return SharedPointer<OBJ>( new ShObj() );
	}

	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param item The source of the new item
		@return the new SharedPointer pointer
	*/
	static SharedPointer makeShared( const OBJ &item )
	{
		return SharedPointer<OBJ>( new ShObj( item ) );
	}

	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param arg1 The first argument for the item's constructor
		@return The new SharedPointer pointer
		@tparam ArgT1 The type of the first constructor parameter
	*/
	template<typename ArgT1>
	static SharedPointer makeShared( const ArgT1 &arg1 )
	{
		return SharedPointer<OBJ>( new ShObj( arg1 ) );
	}
	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param arg1 The first argument for the item's constructor
		@param arg2 The second argument for the item's constructor
		@return The new SharedPointer pointer
		@tparam ArgT1 The type of the first constructor parameter
		@tparam ArgT2 The type of the second constructor parameter
	*/
	template<typename ArgT1, typename ArgT2>
	static SharedPointer makeShared( const ArgT1 &arg1, const ArgT2 &arg2 )
	{
		return SharedPointer<OBJ>( new ShObj( arg1, arg2 ) );
	}
	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param arg1 The first argument for the item's constructor
		@param arg2 The second argument for the item's constructor
		@param arg3 The third argument for the item's constructor
		@return The new SharedPointer pointer
		@tparam ArgT1 The type of the first constructor parameter
		@tparam ArgT2 The type of the second constructor parameter
		@tparam ArgT3 The type of the third constructor parameter
	*/
	template<typename ArgT1, typename ArgT2, typename ArgT3>
	static SharedPointer makeShared( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3 )
	{
		return SharedPointer<OBJ>( new ShObj( arg1, arg2, arg3 ) );
	}
	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param arg1 The first argument for the item's constructor
		@param arg2 The second argument for the item's constructor
		@param arg3 The third argument for the item's constructor
		@param arg4 The forth argument for the item's constructor
		@return The new SharedPointer pointer
		@tparam ArgT1 The type of the first constructor parameter
		@tparam ArgT2 The type of the second constructor parameter
		@tparam ArgT3 The type of the third constructor parameter
		@tparam ArgT4 The type of the forth constructor parameter
	*/
	template<typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4>
	static SharedPointer makeShared( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3, const ArgT4 &arg4 )
	{
		return SharedPointer<OBJ>( new ShObj( arg1, arg2, arg3, arg4 ) );
	}
	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param arg1 The first argument for the item's constructor
		@param arg2 The second argument for the item's constructor
		@param arg3 The third argument for the item's constructor
		@param arg4 The forth argument for the item's constructor
		@param arg5 The fifth argument for the item's constructor
		@return The new SharedPointer pointer
		@tparam ArgT1 The type of the first constructor parameter
		@tparam ArgT2 The type of the second constructor parameter
		@tparam ArgT3 The type of the third constructor parameter
		@tparam ArgT4 The type of the forth constructor parameter
		@tparam ArgT5 The type of the fifth constructor parameter
	*/
	template<typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5>
	static SharedPointer makeShared( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3, const ArgT4 &arg4, const ArgT5 &arg5 )
	{
		return SharedPointer<OBJ>( new ShObj( arg1, arg2, arg3, arg4, arg5 ) );
	}
	/**
		@brief Creates a new item and a new pointer that points to the new item
		@param arg1 The first argument for the item's constructor
		@param arg2 The second argument for the item's constructor
		@param arg3 The third argument for the item's constructor
		@param arg4 The forth argument for the item's constructor
		@param arg5 The fifth argument for the item's constructor
		@param arg6 The sixth argument for the item's constructor
		@return The new SharedPointer pointer
		@tparam ArgT1 The type of the first constructor parameter
		@tparam ArgT2 The type of the second constructor parameter
		@tparam ArgT3 The type of the third constructor parameter
		@tparam ArgT4 The type of the forth constructor parameter
		@tparam ArgT5 The type of the fifth constructor parameter
		@tparam ArgT6 The type of the sixth constructor parameter
	*/
	template<typename ArgT1, typename ArgT2, typename ArgT3, typename ArgT4, typename ArgT5, typename ArgT6>
	static SharedPointer makeShared( const ArgT1 &arg1, const ArgT2 &arg2, const ArgT3 &arg3, const ArgT4 &arg4, const ArgT5 &arg5, const ArgT6 &arg6 )
	{
		return SharedPointer<OBJ>( new ShObj( arg1, arg2, arg3, arg4, arg5, arg6 ) );
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

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
