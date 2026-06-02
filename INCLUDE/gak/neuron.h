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

template <typename ACTIVATION_T=TanhActivation>
class Neuron
{
	BaseValues	m_weights;
	base_t		m_bias;

	void fixInputSize(const BaseValues &inputs)
	{
		if( m_weights.size() < inputs.size() )
		{
			m_weights.setSize(inputs.size());
		}
	}
	public:
	Neuron( base_t bias=0 ) : m_bias(bias)
	{
	}
	base_t calculate( const BaseValues &inputs )
	{
		fixInputSize(inputs);

		base_t	result = 0;
		for( BaseValues::const_iterator inp = inputs.cbegin(),
			wheight = m_weights.cbegin(),
			endIT = inputs.cend();
			inp != endIT;
			++inp, ++wheight
		)
		{
			result += *inp * *wheight;
		}
		return ACTIVATION_T::activate( result + m_bias );
	}
	base_t calculate( base_t input )
	{
		BaseValues	inputs(1);
		inputs[0] = input;
		return calculate(inputs);
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
};

template <typename ACTIVATION_T=TanhActivation>
class NeuronLayer : public Array<Neuron<ACTIVATION_T>> 
{
	public:
	typedef Neuron<ACTIVATION_T>	MY_NYRON_T;

	NeuronLayer( std::size_t numNeurons=0 ) : Array<MY_NYRON_T>(numNeurons) {
	}
	void calculate( const BaseValues &input, BaseValues *output )
	{
		output->empty();
		output->setCapacity( size(), false );
		for(
			Array<MY_NYRON_T>::iterator it = begin(), endIT = end();
			it != endIT;
			++it
		)
		{
			output->push_back( it->calculate(input) );
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
