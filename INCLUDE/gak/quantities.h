/*
		Project:		GAKLIB
		Module:			quantities.h
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

#ifndef GAK_QUANTITIES_H
#define GAK_QUANTITIES_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <math.h>		// otherwise i will loose isnan on Mac

#include <gak/types.h>
#include <gak/math.h>

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

#define SI_UNIT	int64(1)

#define DECA	(SI_UNIT*int64(10))
#define HECTO	(DECA*int64(10))
#define KILO	(HECTO*int64(10))
#define MEGA	(KILO*int64(1000))
#define GIGA	(MEGA*int64(1000))
#define TERA	(GIGA*int64(1000))

#define DECI	(SI_UNIT*int64(10))
#define CENTI	(DECI*int64(10))
#define MILLI	(CENTI*int64(10))
#define MICRO	(MILLI*int64(1000))
#define NANO	(MICRO*int64(1000))
#define PICO	(NANO*int64(1000))

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

enum SIprefix
{
	siPICO, siNANO, siMICRO, siMILLI, siCENTI, siDECI, 
	siBASE, 
	siDECA, siHECTO, siKILO, siMEGA, siGIGA, siTERA
};

enum SIquantitiy
{
	siMETER
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

extern const char *siPrefixe[];
extern const char *siUnits[];

template <int64 FACTOR, int64 DIVISOR, SIprefix PREFIX, typename ValueT=double>
struct SIprefixTraits
{
	static const char *getPrefix()
	{
		return siPrefixe[PREFIX];
	}
	static ValueT getFactor()
	{
		ValueT	factor = FACTOR;
		ValueT	divisor = DIVISOR;
		ValueT	result = factor / divisor;
		return result;
	}
	static ValueT getBaseBalue( ValueT value )
	{
		ValueT factor = getFactor();
		ValueT result = value * factor;

		return result;
	}
	static ValueT getUnitValue( ValueT baseValue )
	{
		ValueT factor = getFactor();
		ValueT result = baseValue / factor;

		return result;
	}
};

typedef SIprefixTraits<TERA,SI_UNIT,siTERA>		TeraUnit;
typedef SIprefixTraits<GIGA,SI_UNIT,siGIGA>		GigaUnit;
typedef SIprefixTraits<MEGA,SI_UNIT,siMEGA>		MegaUnit;
typedef SIprefixTraits<KILO,SI_UNIT,siKILO>		KiloUnit;
typedef SIprefixTraits<HECTO,SI_UNIT,siHECTO>	HektoUnit;
typedef SIprefixTraits<DECA,SI_UNIT,siDECA>		DekaUnit;
typedef SIprefixTraits<SI_UNIT,SI_UNIT,siBASE>	BaseUnit;
typedef SIprefixTraits<SI_UNIT,DECI,siDECI>		DeziUnit;
typedef SIprefixTraits<SI_UNIT,CENTI,siCENTI>	ZentiUnit;
typedef SIprefixTraits<SI_UNIT,MILLI,siMILLI>	MilliUnit;
typedef SIprefixTraits<SI_UNIT,MICRO,siMICRO>	MicroUnit;
typedef SIprefixTraits<SI_UNIT,NANO,siNANO>		NanoUnit;
typedef SIprefixTraits<SI_UNIT,PICO,siPICO>		PikoUnit;

template <SIquantitiy THE_QUANTITY>
struct QuantityTraits
{
	static const char *getSIunit()
	{
		return siUnits[THE_QUANTITY];
	}
	static const char *getUnit()
	{
		return getSIunit();
	}
	static const SIquantitiy si_quantity = THE_QUANTITY;
};

typedef QuantityTraits<siMETER>	Distance;

struct InchUnit : public Distance
{
	static const char *getUnit()
	{
		return "in";
	}
};

struct FeetUnit : public Distance
{
	static const char *getUnit()
	{
		return "ft";
	}
};

struct YardUnit : public Distance
{
	static const char *getUnit()
	{
		return "yd";
	}
};

struct MilesUnit : public Distance
{
	static const char *getUnit()
	{
		return "miles";
	}
};

template <typename QuantityTraitsT, typename PrefixTraitsT, typename ValueT=double>
struct UnitValue
{
	typedef UnitValue<QuantityTraitsT, PrefixTraitsT, ValueT>	MyUnitType;

	ValueT	m_value;

	UnitValue( ValueT value=0 ) : m_value( value )
	{
	}
	template <typename UnitT>
	UnitValue( const UnitT &value )
	{
		setBaseValue( value.getBaseValue() );
	}

	const UnitValue &operator = ( const ValueT &value )
	{
		m_value = value;
		return *this;
	}
	template <typename UnitT>
	const UnitValue &operator = ( const UnitT &value )
	{
		setBaseValue( value.getBaseValue() );
		return *this;
	}

	MyUnitType operator + ( ValueT value ) const
	{
		return MyUnitType( m_value + value );
	}
	template <typename UnitT>
	MyUnitType operator + ( const UnitT &value ) const
	{
		return MyUnitType(
			PrefixTraitsT::getUnitValue( getBaseValue() + value.getBaseValue())
		);
	}
	const UnitValue &operator += ( ValueT value )
	{
		m_value += value;
		return *this;
	}
	template <typename UnitT>
	const UnitValue &operator += ( UnitT value )
	{
		m_value += PrefixTraitsT::getUnitValue( value.getBaseValue() );
		return *this;
	}

	MyUnitType operator - ( ValueT value ) const
	{
		return MyUnitType( m_value - value );
	}
	template <typename UnitT>
	MyUnitType operator - ( const UnitT &value ) const
	{
		return MyUnitType(
			PrefixTraitsT::getUnitValue( getBaseValue() - value.getBaseValue())
		);
	}
	const UnitValue &operator -= ( ValueT value )
	{
		m_value -= value;
		return *this;
	}
	template <typename UnitT>
	const UnitValue &operator -= ( UnitT value )
	{
		m_value -= PrefixTraitsT::getUnitValue( value.getBaseValue() );
		return *this;
	}

	ValueT getBaseValue() const
	{
		return PrefixTraitsT::getBaseBalue( m_value );
	}
	void setBaseValue( ValueT baseValue )
	{
		m_value = PrefixTraitsT::getUnitValue( baseValue );
	}

	static ValueT getFactor()
	{
		return PrefixTraitsT::getPrefix();
	}
	static const char *getPrefix()
	{
		return PrefixTraitsT::getPrefix();
	}
	static const char *getSIunit()
	{
		return QuantityTraitsT::getSIunit();
	}
	static const char *getUnit()
	{
		return QuantityTraitsT::getUnit();
	}
	static const SIquantitiy si_quantity = QuantityTraitsT::si_quantity;
};

typedef UnitValue<Distance,TeraUnit>	TeraMeter;
typedef UnitValue<Distance,GigaUnit>	GigaMeter;
typedef UnitValue<Distance,MegaUnit>	MegaMeter;
typedef UnitValue<Distance,KiloUnit>	KiloMeter;
typedef UnitValue<Distance,HektoUnit>	HectoMeter;
typedef UnitValue<Distance,DekaUnit>	DecaMeter;
typedef UnitValue<Distance,BaseUnit>	Meter;
typedef UnitValue<Distance,DeziUnit>	DeciMeter;
typedef UnitValue<Distance,ZentiUnit>	CentiMeter;
typedef UnitValue<Distance,MilliUnit>	MilliMeter;
typedef UnitValue<Distance,MicroUnit>	MicroMeter;
typedef UnitValue<Distance,NanoUnit>	NanoMeter;
typedef UnitValue<Distance,PikoUnit>	PicoMeter;

typedef SIprefixTraits<254,100 * CENTI, siBASE>	InchPrefix;
typedef UnitValue<InchUnit, InchPrefix>			Inch;

typedef SIprefixTraits<3048,10000, siBASE>		FeetPrefix;
typedef UnitValue<FeetUnit, FeetPrefix>			Feet;

typedef SIprefixTraits<9144,10000, siBASE>		YardPrefix;
typedef UnitValue<YardUnit, YardPrefix>			Yard;

typedef SIprefixTraits<1609344,1000, siBASE>	InterMilePrefix;
typedef UnitValue<MilesUnit,InterMilePrefix>	InterMile;

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

template <typename ValueT>
SIprefix getMatchingPrefix( ValueT value )
{
	int			exponent = math::getExponent( value );
	bool		isMicro = exponent < -3;
	bool		isMacro = exponent > 3;
	bool		isSmall = !isMicro && !isMacro;
	int			type = isSmall ? exponent : exponent/3;
	SIprefix	result = isSmall ? static_cast<SIprefix>(type + siBASE)
								 : isMicro ? static_cast<SIprefix>(type + siCENTI)
										   : static_cast<SIprefix>(type + siHECTO)
	;

	return result;
}

template <typename ValueT>
inline const char *getMatchingPrefixText( ValueT value )
{
	return siPrefixe[ getMatchingPrefix( value ) ];
}

template <>
inline const char *getMatchingPrefixText( SIprefix prefix )
{
	return siPrefixe[ prefix ];
}

template <typename ValueT>
ValueT getDivisor( SIprefix prefix )
{
	bool	isMicro = prefix < siMILLI;
	bool	isMacro = prefix > siKILO;
	bool	isSmall = !isMicro && !isMacro;

	double	exponent = isSmall ? prefix - siBASE
							   : isMacro ? (prefix-siHECTO)*3
										 : (prefix-siCENTI)*3
	;
	ValueT	result = pow( 10., exponent );

	return result;
}

template <typename ValueT>
ValueT getDivisor( ValueT value )
{
	return getDivisor<ValueT>( getMatchingPrefix( value ) );
}

}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_QUANTITIES_H