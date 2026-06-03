/*
		Project:		GAKLIB
		Module:			NeuronTest.h
		Description:	a learning neuron network 
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

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <iostream>
#include <gak/unitTest.h>

#include <gak/neuron.h>

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

class NeuronTest : public UnitTest
{
	virtual const char *GetClassName() const
	{
		return "NeuronTest";
	}
	void neuronTest()
	{
		ai::Neuron<>	neuron;

		float	result = neuron.calculate( 0.5 );
		UT_ASSERT_EQUAL( result, 0 );				// by default this neuron is off

		neuron.setWeight( 1 );

		result = neuron.calculate( 0.5 );
		UT_ASSERT_EQUAL( result, tanh(0.5f) );

		neuron.setBias( 1 );
		result = neuron.calculate( 0.5 );
		UT_ASSERT_EQUAL( result, tanh(1.5f) );
	}
	void layerTest()
	{
		ai::BaseValues		input, output;
		ai::NeuronLayer<>	layer(2);

		input[0] = 0.5;

		layer.calculate( input, &output );
		UT_ASSERT_EQUAL(layer.size(), output.size());
		UT_ASSERT_EQUAL( output[0], 0 );				// by default the neurons are off
		UT_ASSERT_EQUAL( output[1], 0 );

		layer[0].setWeight( 1 );
		layer.calculate( input, &output );
		UT_ASSERT_EQUAL( output[0], tanh(0.5f) );
		UT_ASSERT_EQUAL( output[1], 0 );

		layer[1].setBias( 1 );
		layer.calculate( input, &output );
		UT_ASSERT_EQUAL( output[0], tanh(0.5f) );
		UT_ASSERT_EQUAL( output[1], tanh(1.0f) );
	}
	ai::NeuronNetwork<> createNetwork()
	{
		PODarray<std::size_t>	numNeuronsPerLayer;
		numNeuronsPerLayer[0] = 1;
		numNeuronsPerLayer[1] = 2;
		numNeuronsPerLayer[2] = 1;

		ai::NeuronNetwork<>	netWork(numNeuronsPerLayer);

		return netWork;
	}

	void networkTest()
	{
		ai::BaseValues	input, output;
		input[0] = 0.5;

		PODarray<std::size_t>	numNeuronsPerLayer;
		numNeuronsPerLayer[0] = 1;
		numNeuronsPerLayer[1] = 2;
		numNeuronsPerLayer[2] = 1;

		ai::NeuronNetwork<>	netWork(numNeuronsPerLayer);

		netWork.calculate( input, &output );
		UT_ASSERT_EQUAL( 1, output.size() ); 
		UT_ASSERT_EQUAL( output[0], 0 );				// by default the neurons are off

		ai::BaseValues	weights1, weights2;
		weights1[0] = 1;

		weights2[0] = 2;
		weights2[1] = 0;

		netWork.setWeights( 0, 0, weights1 );
		netWork.setWeights( 1, 0, weights1 );
		netWork.setWeights( 1, 1, weights1 );
		netWork.setWeights( 2, 0, weights2 );

		netWork.calculate( input, &output );
		UT_ASSERT_EQUAL( 1, output.size() ); 
		UT_ASSERT_EQUAL( output[0], tanh(weights2[0]*tanh(tanh(0.5f))) );
	}

	void GradientTest()
	{
		ai::NeuronNetwork<>	netWork = createNetwork();
		netWork.initNetwork(1);

		ai::BaseValues	input, output, expected;
		input[0] = 0.5;
		expected[0] = 0.9;

		netWork.calculate( input, &output );
		double loss1 = netWork.getLoss(output, expected);

		netWork.GradientDescent(output, expected, 0.1);

		netWork.calculate( input, &output );
		double loss2 = netWork.getLoss(output, expected);

		UT_ASSERT_LESS(loss2, loss1);

		for( int i=0; i<10; ++i )
			netWork.GradientDescent(input, expected, 0.1);

		netWork.calculate( input, &output );
		double loss3 = netWork.getLoss(output, expected);
		UT_ASSERT_LESS(loss3, loss1);
	}

	virtual void PerformTest()
	{
		doEnterFunctionEx(gakLogging::llInfo, "NeuronTest::PerformTest");
		TestScope scope( "PerformTest" );
		neuronTest();
		layerTest();
		networkTest();
		GradientTest();
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static NeuronTest myNeuronTest;

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
