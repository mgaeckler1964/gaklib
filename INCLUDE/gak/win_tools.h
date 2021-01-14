/*
		Project:		GAKLIB
		Module:			win_tools.h
		Description:	some useful stuff for windows development
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

/// @cond

#if !defined( GAK_WIN_TOOLS_h ) && defined( _Windows )
#define GAK_WIN_TOOLS_h

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef STRICT
#	define STRICT 1
#endif

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <Windows.h>

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
namespace windows
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

template <class ComObject> class ComPointer
{
	ComObject	*myPointer;


	ComPointer<ComObject> & operator = ( ComPointer<ComObject> &newPointer )
	{
		return *this;
	}
	ComPointer( ComPointer<ComObject> &newPointer )
	{
		myPointer = NULL;
	}
	public:
	ComPointer()
	{
		myPointer = NULL;
	}
	~ComPointer()
	{
		if( myPointer )
			myPointer->Release();
	}
	ComObject ** operator & ( void )
	{
		return &myPointer;
	}
	ComObject * operator -> ( void )
	{
		return myPointer;
	}
	ComObject & operator * ( void )
	{
		return *myPointer;
	}
	operator ComObject * ()
	{
		return myPointer;
	}

	ComPointer<ComObject> & operator = ( ComObject * newPointer )
	{
		if( myPointer && myPointer != newPointer )
			myPointer->Release();
		myPointer = newPointer;

		return *this;
	}

	ULONG Release( void )
	{
		ULONG result = myPointer->Release();
		myPointer = NULL;

		return result;
	}
};

class WinHandle
{
	HANDLE	m_handle;

	WinHandle( const WinHandle &src );
	WinHandle & operator = ( const WinHandle &src );

	public:
	WinHandle()
	{
		m_handle = NULL;
	}
	WinHandle( HANDLE handle )
	{
		m_handle = handle;
	}
	~WinHandle()
	{
		if( m_handle )
		{
			::CloseHandle( m_handle );
		}
	}
	WinHandle & operator = ( HANDLE handle )
	{
		if( m_handle )
		{
			::CloseHandle( m_handle );
		}
		m_handle = handle;
		return *this;
	}
	HANDLE get() const
	{
		return m_handle;
	}
	operator bool () const
	{
		return m_handle != 0;
	}
};

class SystemEvent
{
	WinHandle	theEventHandle;

	public:
	SystemEvent() : theEventHandle( CreateEvent(NULL, FALSE, FALSE, NULL) ) {}
	bool WaitFor( unsigned long timeOut )
	{
		return WaitForSingleObject( theEventHandle.get(), timeOut ) == WAIT_OBJECT_0;
	}
	bool SignalEvent( void ) const
	{
		return SetEvent( theEventHandle.get() ) ? true : false;
	}
	HANDLE get( void ) const
	{
		return theEventHandle.get();
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

}	// namespace windows
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif

/// @endcond
