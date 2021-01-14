/*
		Project:		GAKLIB
		Module:			pipeline.h
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

#ifndef GAK_PIPELINE_H
#define GAK_PIPELINE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/streamRingBuffer.h>
#include <gak/thread.h>

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

template <class OBJ>
class BasicPipe
{
	StreamRingBuffer<OBJ>		buffer;

	BasicIRingStream<OBJ>		iStream;
	BasicORingStream<OBJ>		oStream;

	public:
	BasicPipe( size_t size ) : buffer( size ), iStream( buffer ), oStream( buffer )
	{
	}

	BasicORingStream<OBJ> &getOutStream( void )
	{
		return oStream;
	}
	BasicIRingStream<OBJ> &getInStream( void )
	{
		return iStream;
	}
};

template <class OBJ>
class BasicProducerThread : public Thread
{
	BasicORingStream<OBJ> &cout;
	public:
	BasicProducerThread( BasicORingStream<OBJ> &cout ) : cout(cout)
	{
	}
	~BasicProducerThread()
	{
		cout.close();
	}
	virtual void ExecuteThread( BasicORingStream<OBJ> &cout ) = 0;
	virtual void ExecuteThread( void )
	{
		ExecuteThread( cout );
		cout.close();
	}
};

template <class OBJ>
class BasicConsumerThread : public Thread
{
	BasicIRingStream<OBJ> &cin;
	public:
	BasicConsumerThread( BasicIRingStream<OBJ> &cin ) : cin(cin)
	{
	}
	~BasicConsumerThread()
	{
		cin.close();
	}
	virtual void ExecuteThread( BasicIRingStream<OBJ> &cin ) = 0;
	virtual void ExecuteThread( void )
	{
		ExecuteThread( cin );
		cin.close();
	}
};

template <class OBJ>
class BasicFilterThread : public Thread
{
	protected:
	BasicIRingStream<OBJ> &cin;
	BasicORingStream<OBJ> &cout;
	public:
	BasicFilterThread( BasicIRingStream<OBJ> &cin, BasicORingStream<OBJ> &cout ) : cin(cin), cout(cout)
	{
	}
	~BasicFilterThread()
	{
		cin.close();
		cout.close();
	}
	virtual void ExecuteThread( BasicIRingStream<OBJ> &cin, BasicORingStream<OBJ> &cout ) = 0;
	virtual void ExecuteThread( void )
	{
		ExecuteThread( cin, cout );
		cin.close();
		cout.close();
	}
};

typedef BasicPipe<char>				Pipe;
typedef BasicProducerThread<char>	ProducerThread;
typedef BasicFilterThread<char>		FilterThread;
typedef BasicConsumerThread<char>	ConsumerThread;

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

#endif	// GAK_PIPELINE_H