/*
		Project:		GAKLIB
		Module:			QuantitiesTest.h
		Description:	
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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/quantities.h>

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

class QuantitiesTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "QuantitiesTest";
	}

	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "QuantitiesTest::PerformTest");
		TestScope scope( "PerformTest" );

		const double baseValue = 9.1234567890123456789;

		TeraMeter tera = baseValue;
		UT_ASSERT_EQUAL( baseValue, tera.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e12, tera.getBaseValue() );
		GigaMeter giga = baseValue;
		UT_ASSERT_EQUAL( baseValue, giga.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e9, giga.getBaseValue() );
		MegaMeter mega = baseValue;
		UT_ASSERT_EQUAL( baseValue, mega.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e6, mega.getBaseValue() );
		KiloMeter kilo = baseValue;
		UT_ASSERT_EQUAL( baseValue, kilo.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e3, kilo.getBaseValue() );
		Meter meter = baseValue;
		UT_ASSERT_EQUAL( baseValue, meter.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e0, meter.getBaseValue() );
		MilliMeter milli = baseValue;
		UT_ASSERT_EQUAL( baseValue, milli.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e-3, milli.getBaseValue() );
		MicroMeter micro = baseValue;
		UT_ASSERT_EQUAL( baseValue, micro.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e-6, micro.getBaseValue() );
		NanoMeter nano = baseValue;
		UT_ASSERT_EQUAL( baseValue, nano.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e-9, nano.getBaseValue() );
		PicoMeter pico = baseValue;
		UT_ASSERT_EQUAL( baseValue, pico.m_value );
		UT_ASSERT_EQUAL( baseValue*1.0e-12, pico.getBaseValue() );

		kilo = 126.0;
		meter = 3.6;
		milli = kilo + meter;
		UT_ASSERT_EQUAL( (126e3+3.6)*1000., milli.m_value );

		milli = kilo - meter;
		UT_ASSERT_EQUAL( (126e3-3.6)*1000., milli.m_value );

		kilo += 4.;
		UT_ASSERT_EQUAL( 130., kilo.m_value );
		kilo -= 15.;
		UT_ASSERT_EQUAL( 115., kilo.m_value );
		kilo += meter;
		UT_ASSERT_EQUAL( 115.0036, kilo.m_value );
		kilo -= meter;
		UT_ASSERT_EQUAL( 115., kilo.m_value );

		kilo = kilo + 3.;
		UT_ASSERT_EQUAL( 118., kilo.m_value );
		kilo = kilo - 8.;
		UT_ASSERT_EQUAL( 110., kilo.m_value );

		Inch inch = 1.0;
		CentiMeter centi = inch;
		UT_ASSERT_EQUAL( 2.54, centi.m_value );
		meter = inch;
		UT_ASSERT_EQUAL( 0.0254, meter.m_value );

		Feet feet = 1.0;
		meter = feet;
		UT_ASSERT_EQUAL( 0.3048, meter.m_value );

		Yard yard = 1.0;
		meter = yard;
		UT_ASSERT_EQUAL( 0.9144, meter.m_value );

		inch = feet;
		UT_ASSERT_EQUAL( 12.0, inch.m_value );
		inch = yard;
		UT_ASSERT_EQUAL( 36.0, inch.m_value );
		feet = yard;
		UT_ASSERT_EQUAL( 3.0, feet.m_value );

		InterMile	iMile = 1.;
		meter = iMile;
		UT_ASSERT_EQUAL( 1609.344, meter.m_value );

		Show();
	}
	void Show()
	{
		compareUnit<TeraMeter, Meter>();
		compareUnit<GigaMeter, Meter>();
		compareUnit<MegaMeter, Meter>();
		compareUnit<KiloMeter, Meter>();
		compareUnit<HectoMeter, Meter>();
		compareUnit<DecaMeter, Meter>();
		compareUnit<Meter, Meter>();
		compareUnit<DeciMeter, Meter>();
		compareUnit<CentiMeter, Meter>();
		compareUnit<MilliMeter, Meter>();
		compareUnit<MicroMeter, Meter>();
		compareUnit<NanoMeter, Meter>();
		compareUnit<PicoMeter, Meter>();

		KiloMeter	myKilo = 126.0;
		Meter		myMeter = 3.6;

		MilliMeter	myMilli = myKilo + myMeter;

		printUnit( myKilo );
		printUnit( myMeter );
		printUnit( myMilli );

		myKilo += 4.;
		printUnit( myKilo );
		myKilo -= 15.;
		printUnit( myKilo );
		myKilo += myMeter;
		printUnit( myKilo );
		myKilo -= myMeter;
		printUnit( myKilo );

		compareUnits<Inch, CentiMeter>();
		compareUnits<InterMile, Meter>();
		compareUnits<Feet, Meter>();
		compareUnits<Feet, Inch>();
		compareUnits<Yard, Meter>();
		compareUnits<Yard, Feet>();

		double val = 9999999.0;
		for( int i=0; i<12; ++i )
		{
			double divisor = getDivisor( val );
			std::cout	<< formatFloat( val, 19, 12 ) << "m = " 
						<< formatFloat(val/divisor, 16, 12 ) << getMatchingPrefixText( val ) << "m "
						<< formatFloat(divisor, 0, 12 ) << std::endl;
			val /= 10;
		}
	}

	template<typename UnitT>
	void printValue( const UnitT &value )
	{
		std::cout << formatFloat( value.m_value ) << value.getPrefix() << value.getUnit();
	}
	template<typename UnitT>
	void printBaseValue( const UnitT &value )
	{
		std::cout << formatFloat( value.getBaseValue() ) << value.getSIunit();
	}
	template<typename UnitT1>
	void printUnit( const UnitT1 &theUnit )
	{
		printValue( theUnit );
		std::cout << " = ";
		printBaseValue( theUnit );
		std::cout << std::endl;
	}

	template<typename UnitT1, typename UnitT2>
	void compareUnits()
	{
		compareUnit<UnitT1, UnitT2>();
		compareUnit<UnitT2, UnitT1>();
	}
	template<typename UnitT1, typename UnitT2>
	void compareUnit()
	{
		UnitT1	theUnit = 1.0;
		UnitT2	theOther = theUnit;

		printValue( theUnit );
		std::cout << " = ";
		printValue( theOther );
		std::cout << std::endl;
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static QuantitiesTest myQuantitiesTest;

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

