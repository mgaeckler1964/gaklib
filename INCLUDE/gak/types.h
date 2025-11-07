/*
		Project:		GAKLIB
		Module:			types.h
		Description:	common types
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

#ifndef GAK_TYPES_H
#define GAK_TYPES_H

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

#if defined( __BORLANDC__  )
#define nullptr (void*)(NULL)
#endif
#if defined( __GNUC__ )
#define nullptr (NULL)
#endif

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef enum { cFalse, cTrue } cBool;
#ifndef __cplusplus
	#define false	cFalse
	#define true	cTrue
#endif

#ifdef __cplusplus
namespace gak
{
#endif

typedef unsigned char			uint8;
typedef signed char				int8;

typedef unsigned short			uint16;
typedef signed short			int16;

#if defined( __GNUC__ ) || defined( _MSC_VER )
typedef unsigned int			uint32;
typedef signed int				int32;
typedef signed long long		int64;
typedef unsigned long long		uint64;
#elif defined( __WIN32__ )
typedef unsigned long			uint32;
typedef signed long				int32;
	#if !defined(__BORLANDC__ ) || !defined( SERVICES_WSYSINC_H )
/* OWL has its own definition */
typedef unsigned __int64		uint64;
typedef signed __int64			int64;
#	endif
#else
#error "Unkown Compiler"
#endif

#ifndef __GNUC__
#	ifdef __WIN32__
		typedef int32					ssize_t;
#	else
		typedef int64					ssize_t;
#	endif
#endif


#if 0
/*
typedef struct uint64
{
#ifdef __cplusplus
	private:
#endif
	uint32	lowWord,
			highWord;
#ifdef __cplusplus
	public:
	uint64()
	{
		lowWord = highWord = 0;
	}
	uint64( uint32 src )
	{
		highWord = 0;
		lowWord = src;
	}
	bool operator == ( const uint64 &oper ) const
	{
		return highWord == oper.highWord && lowWord == oper.lowWord;
	}
	bool operator < ( const uint64 &oper ) const
	{
		return highWord < oper.highWord
		||  (highWord == oper.highWord && lowWord < oper.lowWord);
	}
	bool operator > ( const uint64 &oper ) const
	{
		return highWord > oper.highWord
		||  (highWord == oper.highWord && lowWord > oper.lowWord);
	}
	bool operator >= ( const uint64 &oper ) const
	{
		return !(*this < oper);
	}
	bool operator <= ( const uint64 &oper ) const
	{
		return !(*this > oper);
	}
	bool operator != ( const uint64 &oper ) const
	{
		return !(*this == oper);
	}

	void setValue( uint32 lowWord, uint32 highWord )
	{
		this->lowWord = lowWord;
		this->highWord = highWord;
	}
	operator uint32 ( void ) const
	{
		return lowWord;
	}
	operator double ( void ) const
	{
		return double(uint32(-1))*double(highWord)+double(lowWord);
	}
#endif
} uint64;
*/
#endif

#ifdef __cplusplus
}	// namespace gak
#endif


// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

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

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#if defined( __cplusplus ) && defined( __BORLANDC__ ) && __BORLANDC__ < 0x590
#include <limits>
namespace std
{
	template <>
	struct numeric_limits<gak::int64>
	{
		static const bool is_integer = true;
		static const bool is_signed = true;
		static const int digits = CHAR_BIT*sizeof(gak::int64)-1;
		static const int max_exponent10 = 0;
		static const int min_exponent10 = 0;
		static gak::int64 max()
		{
			return gak::int64( gak::uint64(-1) >> 1 );
		}
		static gak::int64 min()
		{
			return -max()-1;
		}
	};

	template <>
	struct numeric_limits<gak::uint64>
	{
		static const bool is_integer = true;
		static const bool is_signed = false;
		static const int digits = CHAR_BIT*sizeof(gak::uint64);
		static const int max_exponent10 = 0;
		static const int min_exponent10 = 0;
		static gak::uint64 max()
		{
			return gak::uint64(-1);
		}
		static gak::uint64 min()
		{
			return 0;
		}
	};
}	// namespace std
#endif

#endif
