/*
		Project:		GAKLIB
		Module:			type_traits.h
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

#ifndef GAK_TYPE_TRAITS_H
#define GAK_TYPE_TRAITS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

/// @cond
namespace internal
{
	template <bool v>
	struct integral_constant
	{
		static const bool value = v;
	};

	template <typename TYPE>
	struct is_integral : integral_constant<false>
	{
	};

	template <>
	struct is_integral<bool> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<char> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<signed char> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<unsigned char> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<signed short> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<unsigned short> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<signed int> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<unsigned int> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<signed long> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<unsigned long> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<int64> : integral_constant<true>
	{
	};

	template <>
	struct is_integral<uint64> : integral_constant<true>
	{
	};

	template <typename TYPE>
	struct is_float : integral_constant<false>
	{
	};

	template <>
	struct is_float<float> : integral_constant<true>
	{
	};

	template <>
	struct is_float<double> : integral_constant<true>
	{
	};

	template <>
	struct is_float<long double> : integral_constant<true>
	{
	};

	template <typename TYPE>
	struct is_arithmetic
	{
		static const bool value = is_integral<TYPE>::value||is_float<TYPE>::value;
	};

	template <typename T>
	struct remove_const
	{
		typedef T type;
	};

	#if !defined( __BORLANDC__ ) || __BORLANDC__ > 0x551
	template <typename T>
	struct remove_const<const T>
	{
		typedef T type;
	};
	#endif

}	// namespace internal
/// @endcond

template <typename TYPE>
struct is_integral : internal::is_integral< typename internal::remove_const<TYPE>::type >
{
};

template <typename TYPE>
struct is_float : internal::is_float< typename internal::remove_const<TYPE>::type >
{
};

template <typename TYPE>
struct is_arithmetic : internal::is_arithmetic< typename internal::remove_const<TYPE>::type >
{
};

template <typename TYPE>
struct is_binary : internal::is_arithmetic< typename internal::remove_const<TYPE>::type >
{
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
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_TYPE_TRAITS_H