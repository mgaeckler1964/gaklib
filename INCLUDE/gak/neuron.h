/*
		Project:		gaklib
		Module:			neuron.h
		Description:	one single neuron for a network
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

#ifndef GAK_NEURON_H
#define GAK_NEURON_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <limits>

#include <gak/array.h>

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
namespace ai
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

static const double RANDOM_WEIGHT_RANGE	= 0.1;
static const double RANDOM_BIAS_RANGE	= 1;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef float				base_t;
typedef PODarray<base_t>	BaseValues;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct TanhActivation {
    static base_t activate(base_t x) { return std::tanh(x); }
};

struct ReLUActivation {
    static base_t activate(base_t x) { return std::max(0.0f, x); }
};

inline double getRandomInRange( double range )
{
	int intRange = std::numeric_limits<int>::max()/2;
	int intRand = randomNumber( std::numeric_limits<int>::max() ) - intRange;

	return (range * intRand)/intRange;
};

template <typename ACTIVATION_T=TanhActivation>
class Neuron
{
	BaseValues	m_lastInputs;
	base_t		m_lastResult;
	BaseValues	m_weights;
	base_t		m_bias;
	double		m_delta;

	void fixInputSize(const BaseValues &inputs)
	{
		if( m_weights.size() < inputs.size() )
		{
			m_weights.setSize(inputs.size());
		}
	}
	public:
	Neuron( base_t bias=0 ) : m_lastResult(0), m_bias(bias), m_delta(0)
	{
	}
	base_t calculate( const BaseValues &inputs )
	{
		fixInputSize(inputs);
		m_lastInputs = inputs;

		base_t	result = 0;
		for( BaseValues::const_iterator inp = inputs.cbegin(),
			weight = m_weights.cbegin(),
			endIT = inputs.cend();
			inp != endIT;
			++inp, ++weight
		)
		{
			result += *inp * *weight;
		}
		m_lastResult = ACTIVATION_T::activate( result + m_bias );
		return m_lastResult;
	}
	base_t calculate( base_t input )
	{
		BaseValues	inputs(1);
		inputs[0] = input;
		return calculate(inputs);
	}

	base_t getLastOutput() const
	{
		return m_lastResult;
	}
	double getDelta() const
	{
		return m_delta;
	}
	void setDelta(double delta) 
	{
		m_delta = delta;
	}
	void calcDelta( base_t actual, base_t expected )
	{
		base_t	diff = actual - expected;
		setDelta(diff * (1.0- m_lastResult*m_lastResult));
	}

	void nextStep( double step )
	{
		double delta = getDelta();
		m_bias -= step * delta; 

		// Alle Gewichte korrigieren
		for(size_t i = 0; i < m_weights.size(); ++i) 
		{
			double gradient_w = delta * m_lastInputs[i]; // <--- Hier nutzen wir die Vergangenheit!
			m_weights[i] -= step * gradient_w;
		}
	}

	base_t getWeight( size_t index ) const
	{
		return m_weights[index];
	}
	void setWeights( const BaseValues &weights )
	{
		m_weights = weights;
	}
	void setWeight( base_t weight )
	{
		BaseValues	weights(1);
		weights[0] = weight;
		setWeights( weights );
	}
	void setWeight( std::size_t index, base_t weight )
	{
		m_weights[index] = weight;
	}

	base_t setBias( base_t bias )
	{
		base_t oldBias = m_bias;
		m_bias = bias;
		return oldBias;
	}
	void modifyBias( base_t change )
	{
		m_bias += change;
	}

	void initNeuron( std::size_t numWeights )
	{
		if( m_weights.size() < numWeights )
		{
			m_weights.setSize(numWeights);
		}

		m_bias = getRandomInRange(RANDOM_BIAS_RANGE);
		for(
			BaseValues::iterator it = m_weights.begin(), endIT = m_weights.end();
			it != endIT;
			++it
		)
		{
			*it = getRandomInRange(RANDOM_WEIGHT_RANGE);
		}
	}
	BaseValues::iterator begin()
	{
		return m_weights.begin();
	}
	BaseValues::iterator end()
	{
		return m_weights.end();
	}
};

template <typename ACTIVATION_T=TanhActivation>
class NeuronLayer : public Array<Neuron<ACTIVATION_T>> 
{
	public:
	typedef NeuronLayer<typename ACTIVATION_T>	SelfT;
	typedef Neuron<ACTIVATION_T>				MY_NEURON_T;

	NeuronLayer( std::size_t numNeurons=0 ) : Array<MY_NEURON_T>(numNeurons) {
	}
	void calculate( const BaseValues &input, BaseValues *output )
	{
		output->empty();
		output->setCapacity( size(), false );
		for(
			Array<MY_NEURON_T>::iterator it = begin(), endIT = end();
			it != endIT;
			++it
		)
		{
			output->push_back( it->calculate(input) );
		}
	}
	void initNeurons( std::size_t numWeights )
	{
		for(
			Array<MY_NEURON_T>::iterator it = begin(), endIT = end();
			it != endIT;
			++it
		)
		{
			it->initNeuron(numWeights);
		}
	}
	void nextStep( double step )
	{
		for(
			Array<MY_NEURON_T>::reverse_iterator it = rbegin(), endIT = rend();
			it != endIT;
			++it
		)
		{
			it->nextStep( step );
		}
	}
	/*
		this is for the last layer of the network
	*/
	void calcDeltas( const BaseValues &output, const BaseValues &expected )
	{
		assert( output.size() == expected.size() );
		assert( size() == expected.size() );

		BaseValues::const_iterator oIT = output.cbegin();
		BaseValues::const_iterator eIT = expected.cbegin();
		for(
			Array<MY_NEURON_T>::iterator it = begin(), endIT = end();
			it != endIT;
			++it, ++oIT, ++eIT
		)
		{
			it->calcDelta( *oIT, *eIT );
		}
	}
	/*
		this is for the hidden and the input layer of the network
	*/
	void calculateHiddenDeltas( const SelfT &nextLayer )
	{
		size_t	idx=0;
		for(
			Array<MY_NEURON_T>::iterator it = begin(), endIT = end();
			it != endIT;
			++it, ++idx
		)
		{
			double neuronErrorSum = 0;
			for(
				Array<MY_NEURON_T>::const_iterator itn = nextLayer.cbegin(), endITn = nextLayer.cend();
				itn != endITn;
				++itn
			)
			{
				neuronErrorSum += itn->getDelta() * itn->getWeight(idx);
			}
			double currentCur = it->getLastOutput();
			double delta = neuronErrorSum * (1.0 - currentCur * currentCur); // Für tanh	
			it->setDelta(delta);
		}
	}
};

template <typename ACTIVATION_T=TanhActivation>
class NeuronNetwork : public Array<NeuronLayer<ACTIVATION_T>>
{
	public:
	NeuronNetwork( const PODarray<std::size_t> &numNeurons ) : Array<NeuronLayer<ACTIVATION_T>>(numNeurons.size()) 
	{
		iterator myIT = begin();
		for(
			Array<std::size_t>::const_iterator it = numNeurons.cbegin(), endIT = numNeurons.cend();
			it != endIT;
			++it, ++myIT
		)
		{
			myIT->setSize(*it);
		}
	}
	double getLoss(const BaseValues &actual, const BaseValues &expected)
	{
		double loss = 0;
		BaseValues::const_iterator	aIt = actual.cbegin(),
									aEnd = actual.cend(),
									eIt = expected.cbegin(),
									eEnd = expected.cend();
		while(aIt != aEnd && eIt != eEnd)
		{
			double diff = *aIt - *eIt;
			double square = diff*diff;
			loss += square;
			++aIt;
			++eIt;
		}
		while(aIt != aEnd)
		{
			double diff = *aIt;
			double square = diff*diff;
			loss += square;
			++aIt;
		}
		while(eIt != eEnd)
		{
			double diff = *eIt;
			double square = diff*diff;
			loss += square;
			++eIt;
		}
		return loss;
	}
	void calculate( const BaseValues &input, BaseValues *output )
	{
		BaseValues tmp;

		*output = input;
		for(
			iterator it = begin(), endIT = end();
			it != endIT;
			++it
		)
		{
			tmp.moveFrom( *output );
			it->calculate( tmp, output );
		}
	}
	void setBias( std::size_t layer, std::size_t neuron, base_t bias )
	{
		getOrCreateElementAt(layer)[neuron].setBias( bias );
	}
	void setWeights( std::size_t layer, std::size_t neuron, const BaseValues &weights )
	{
		getOrCreateElementAt(layer)[neuron].setWeights( weights );
	}

	void initNetwork(std::size_t numInputData)
	{
		for(
			iterator it = begin(), endIT = end();
			it != endIT;
			++it
		)
		{
			it->initNeurons(numInputData);
			numInputData = it->size();
		}
	}

	/*
		Here are sone learning algorythms
	*/
	double HillClimping(const BaseValues &input, const BaseValues &expected, double step)
	{
		BaseValues cur, tmpResult;
		calculate( input, &cur );
		double loss = getLoss(cur, expected);
		
		while( loss )
		{
			for(
				iterator layer = begin(), endLayer = end();
				layer != endLayer && loss;
				++layer
			)
			{
				for(
					NeuronLayer<ACTIVATION_T>::iterator neuronIT = layer->begin(), endNeuron = layer->end();
					neuronIT != endNeuron && loss;
					++neuronIT
				)
				{
					Neuron<ACTIVATION_T>	&neuron = *neuronIT;
					while( loss )
					{
						neuron.modifyBias(step);
						calculate( input, &tmpResult );
						double tmpLoss = getLoss(tmpResult, expected);
						if( tmpLoss > loss )		// worse
						{
							if( step > 0 )
							{
								step = -step;
								neuron.modifyBias(step);
/*^*/							continue;
							}
							else
/*v*/							break;
						}
						else if(tmpLoss == loss)	// no change
/*v*/						break;
						loss = tmpLoss;
					}
					for(
						BaseValues::iterator weight = neuron.begin(), endIT = neuron.end();
						weight != endIT && loss;
						++weight
					)
					{
						step = abs(step);
						while( loss )
						{
							*weight += step;
							calculate( input, &tmpResult );
							double tmpLoss = getLoss(tmpResult, expected);
							if( tmpLoss > loss )		// worse
							{
								if( step > 0 )
								{
									step = -step;
									*weight += step;
/*^*/								continue;
								}
								else
/*v*/								break;
							}
							else if(tmpLoss == loss)	// no change
/*v*/							break;
							loss = tmpLoss;
						}
					}
				}
			}
		}

		return loss;
	}
	double StartHillClimping(const BaseValues &input, const BaseValues &expected, double step )
	{
		initNetwork(input.size());
		return HillClimping(input, expected, step);
	}
	/*
		this is the most common
	*/
	void GradientDescent(const BaseValues &input, const BaseValues &expected, double step )
	{
		reverse_iterator layer = rbegin(), endLayer = rend();
		if( layer == endLayer )
			return;

		BaseValues cur, tmpResult;
		calculate( input, &cur );

		layer->calcDeltas( cur, expected );
		reverse_iterator	previous = layer;
		++layer;
		for( ; layer != endLayer; ++layer )
		{
			layer->calculateHiddenDeltas( *previous );
		}

		for(
			reverse_iterator layer = rbegin(), endLayer = rend();
			layer != endLayer;
			++layer
		)
		{
			layer->nextStep( step );
		}
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
}	// namespace ai
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_NEURON_H
