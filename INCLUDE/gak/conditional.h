/*
		Project:		GAKLIB
		Module:			conditional.h
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

#ifndef GAK_CONDITIONAL_H
#define GAK_CONDITIONAL_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#if defined( _Windows )
#include <gak/win_tools.h>
#elif defined( __MACH__ ) || defined( __unix__ )
#include <pthread.h>
#else
#error "Unknown operating system"
#endif

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

///	@brief this class is used to notify another Thread and to wait for a notification
class Conditional
{
#if defined( _Windows )
	windows::WinHandle	m_eventHandle;
#elif defined( __MACH__ ) || defined( __unix__ )
	pthread_cond_t	m_conditional;
	pthread_mutex_t	m_mutex;
	bool			m_notified;
#endif

	public:
#if defined( _Windows )
	/// Creates a new condition variable
	Conditional() : m_eventHandle( CreateEvent(NULL, FALSE, FALSE, NULL) ) {}
#elif defined( __MACH__ ) || defined( __unix__ )
	Conditional()
	{
		//static pthread_cond_t	conditional = PTHREAD_COND_INITIALIZER;
		//static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

		//m_conditional = conditional;
		pthread_cond_init(&m_conditional, NULL);

		//m_mutex = mutex;
		pthread_mutex_init(&m_mutex, NULL);

		m_notified = false;
	}
#endif

	/// constant value to specify to wait for a notification forever
	static const unsigned long WAIT_FOREVER = static_cast<unsigned long>(-1);

	/**
		@brief waits for a notification

		if the condition variable was notified since the last wait it return immediately

		@param [in] timeOut the max time in ms to wait for the notification
		@return true if the object was notified
	*/
	bool wait( unsigned long timeOut = WAIT_FOREVER )
	{
#if defined( _Windows )
		return WaitForSingleObject( m_eventHandle.get(), timeOut ) == WAIT_OBJECT_0;
#elif defined( __MACH__ ) || defined( __unix__ )
		bool result;
		if( m_notified )
		{
			result = true;
		}
		else if( timeOut != WAIT_FOREVER )
		{
			struct timespec	absTime;
			struct timeval	now;
			
			gettimeofday(&now,NULL);

			absTime.tv_sec = now.tv_sec + timeOut / 1000;
			absTime.tv_nsec = (now.tv_usec + 1000UL*(timeOut % 1000UL)) * 1000UL;
			pthread_mutex_lock( &m_mutex );
			result = pthread_cond_timedwait( &m_conditional, &m_mutex, &absTime ) == 0;
			pthread_mutex_unlock( &m_mutex );
		}
		else
		{
			pthread_mutex_lock( &m_mutex );
			result = pthread_cond_wait( &m_conditional, &m_mutex ) == 0;
			pthread_mutex_unlock( &m_mutex );
		}
		m_notified = false;
		return result;
#endif
	}

	/// notifies the object (i.e. the waiting thread)
	void notify( void )
	{
#if defined( _Windows )
		SetEvent( m_eventHandle.get() );
#elif defined( __MACH__ ) || defined( __unix__ )
		m_notified = true;
		pthread_cond_signal(&m_conditional);  
#endif
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

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_CONDITIONAL_H
