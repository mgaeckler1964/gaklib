/*
		Project:		GAKLIB
		Module:			StringBuffer.h
		Description:	A small buffer on stack used to construct strings
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

#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/exception.h>
#include <gak/fmtNumber.h>

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

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4996 )	// 'strcpy': This function or variable may be unsafe.
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

template <size_t BUFFER_SIZE=128>
class StringBuffer : public BaseBuffer<BUFFER_SIZE>
{
	typedef BaseBuffer<BUFFER_SIZE>	Super;

	public:
	// adding a character
	StringBuffer &addDigit( char digit )
	{
		if( this->m_len >= BUFFER_SIZE )
			throw IndexError();
		Super::addDigit( digit );
		return *this;
	}
	StringBuffer &addDigit( char digit,  size_t count )
	{
		if( this->m_len+count >= BUFFER_SIZE )
			throw IndexError();
		Super::addDigit( digit, count );
		return *this;
	}

	StringBuffer &insDigit( char digit, size_t startPos, size_t count )
	{
		if( this->m_len+count >= BUFFER_SIZE )
			throw IndexError();
		if( startPos >= this->m_len )
			throw IndexError();
		Super::insDigit( digit, startPos, count );
		return *this;
	}

	StringBuffer &operator += ( char digit )
	{
		addDigit( digit );
		return *this;
	}

	// adding a C string
	private:
	StringBuffer &addCP( const char *cp, size_t len )
	{
		if( this->m_len+len > BUFFER_SIZE )
			throw IndexError();
		Super::addCP( cp, len );
		return *this;
	}
	public:
	StringBuffer &addSTR( const STRING &cp )
	{
		addCP( cp.c_str(), cp.strlen() );
		return *this;
	}
	StringBuffer &addCP( const char *cp )
	{
		addCP( cp, strlen(cp) );
		return *this;
	}
	template <typename T, size_t N>
	StringBuffer &add( const T (&arr)[N] )
	{
		addCP( arr, N-1 );
		return *this;
	}
	template <typename T, size_t N>
	StringBuffer &operator += (const T (&arr)[N])
	{
		addCP( arr, N-1 );
		return *this;
	}
	StringBuffer &addBB ( const StringBuffer &bb )
	{
		return addCP( bb.m_buffer, bb.m_len );
	}

#ifdef __BORLANDC__
	StringBuffer &add( const char *arr )
	{
		addCP( arr, strlen(arr) );
		return *this;
	}
	StringBuffer &operator += (const char *arr)
	{
		addCP( arr, strlen(arr) );
		return *this;
	}
#endif
	// add a number
	template <typename NUMBER_T>
	StringBuffer &addNumber(
		NUMBER_T value, int fieldLength=0, char filler=0, char thousand=0
	)
	{
		appendNumberFast( this, value, fieldLength, filler, thousand );
		return *this;
	}
	template <typename NUMBER_T>
	StringBuffer &addFloat(
		NUMBER_T value, int fieldLength=0, int prec=0, char thousand=0, char decimal='.'
	)
	{
		appendFloatFast( this, value, fieldLength, prec, thousand, decimal );
		return *this;
	}

	void assertAdd( size_t additional )
	{
		if( additional > this->remain() )
			throw IndexError();
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

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// #define STRING_BUFFER_H
