/*
		Project:		GAKLIB
		Module:			bitfield.h
		Description:	A bitfield with up to 32 bits
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#ifndef GAK_BITFIELD_H
#define GAK_BITFIELD_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>

#include <gak/types.h>

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

class Bitfield
{
	uint32	m_bits;

	public:
	Bitfield( uint32 bits = 0 )
	{
		m_bits = bits;
	}
	Bitfield( const Bitfield &source )
	{
		m_bits = source.m_bits;
	}

	void set( int bit )
	{
		m_bits |= 1<<bit;
	}
	void clear( int bit )
	{
		m_bits &= ~(1<<bit);
	}
	uint32 test( int bit ) const
	{
		return m_bits & (1<<bit);
	}

	Bitfield operator << ( int bit ) const
	{
		Bitfield result( *this );

		result <<= bit;

		return result;
	}
	const Bitfield &operator <<= ( int bit )
	{
		set( bit );

		return *this;
	}
	Bitfield operator >> ( int bit ) const
	{
		Bitfield result( *this );

		result >>= bit;

		return result;
	}
	const Bitfield &operator >>= ( int bit )
	{
		clear( bit );

		return *this;
	}

	Bitfield operator & ( const Bitfield &source ) const
	{
		Bitfield result( *this );
		result &= source;

		return result;
	}
	const Bitfield &operator &= ( const Bitfield &source )
	{
		m_bits &= source.m_bits;

		return *this;
	}

	Bitfield operator | ( const Bitfield &source ) const
	{
		Bitfield result( *this );
		result |= source;

		return result;
	}
	const Bitfield &operator |= ( const Bitfield &source )
	{
		m_bits |= source.m_bits;

		return *this;
	}

	Bitfield operator ^ ( const Bitfield &source ) const
	{
		Bitfield result( *this );
		result ^= source;

		return result;
	}
	const Bitfield &operator ^= ( const Bitfield &source )
	{
		m_bits ^= source.m_bits;

		return *this;
	}

	operator uint32 () const
	{
		return m_bits;
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

inline std::ostream & operator << ( std::ostream &out, const Bitfield &in )
{
	const uint32	flags = in;
	uint32			flag = 0X80000000;

	while( flag )
	{
		out << ((flags & flag) ? '1' : '0');
		flag >>= 1;
	}

	return out;
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
