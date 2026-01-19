/*
		Project:		GAKLIB
		Module:			GpxUtils.h
		Description:	Utilities for GPX files
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
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

#ifndef GPX_UTILS_H
#define GPX_UTILS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/array.h>
#include <gak/xml.h>
#include <gak/gps.h>

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

namespace gak
{
namespace math
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

typedef GpsPosition<double>	GpxPosition;

struct GPXpoint
{
	GpxPosition	m_position;

	STRING		m_time,
				m_longitude,
				m_latitude;
	int			m_totalDistance,
				m_height,
				m_heartRate;
	long		m_elapsedTime,
				m_totalTime1,
				m_totalTime2;
	double		m_speed,
				m_acceleration,
				m_heightDistance,
				m_sphereDistance,
				m_distance,
				m_totalUp,
				m_totalDown,
				m_gradient,
				m_temperature;
	DateTime	m_timestamp;
};

class GPXtrack : public gak::Array<GPXpoint>
{
	xml::Element	*m_theSegments;
	int				m_minHeight, m_maxHeight;
	double			m_maxSpeed;
	double			m_minGradient, m_maxGradient;
	double			m_maxAcc, m_minAcc;
	int				m_minHeartRate, m_maxHeartRate;
	double			m_minTemperature, m_maxTemperature;

	double			m_minLon, m_maxLon;
	double			m_minLat, m_maxLat;

	double			m_totalDistance;
	double			m_totalUp, m_totalDown;
	long			m_totalTime1, m_totalTime2;
	DateTime		m_timestamp;
	bool			m_ts_valid;

	void LoadGpxTrack( gak::xml::Element *theSegments );

	void init( void )
	{
		clear();
		m_totalDistance	= 0.0;
		m_totalUp = m_totalDown = 0;
		m_totalTime1 = m_totalTime2 = 0;

		m_minHeight = m_maxHeight = 0;
		m_maxSpeed = 0;
		m_maxAcc = m_minAcc = m_minGradient = m_maxGradient = 0.0;
		m_minTemperature = m_maxTemperature = 0;
		m_minHeartRate = m_maxHeartRate = 0;

		m_minLon = m_maxLon = 0.0;
		m_minLat = m_maxLat = 0.0;

		m_timestamp = 0;
		m_ts_valid = false;
	}

	public:
	void LoadGpxTrack( xml::Document *theGpxFile, const STRING &trackName );
	STRING LoadCrpTrack( const STRING &crpFileName );
	int getMinHeight( void ) const
	{
		return m_minHeight;
	}
	int getMaxHeight( void ) const
	{
		return m_maxHeight;
	}
	double getMaxSpeed( void ) const
	{
		return m_maxSpeed;
	}
	double getDistance( void ) const
	{
		return m_totalDistance;
	}
	double getUp( void ) const
	{
		return m_totalUp;
	}
	double getDown( void ) const
	{
		return m_totalDown;
	}
	long getTotalTime1( void ) const
	{
		return m_totalTime1;
	}
	long getTotalTime2( void ) const
	{
		return m_totalTime2;
	}
	DateTime getStartTime( void ) const
	{
		return m_timestamp;
	}
	double getMinGradient( void ) const
	{
		return m_minGradient;
	}
	double getMaxGradient( void ) const
	{
		return m_maxGradient;
	}
	double getMinAcceleration( void ) const
	{
		return m_minAcc;
	}
	double getMaxAcceleration( void ) const
	{
		return m_maxAcc;
	}
	int getMinHeartRate( void ) const
	{
		return m_minHeartRate;
	}
	int getMaxHeartRate( void ) const
	{
		return m_maxHeartRate;
	}
	double getMinTemperature( void ) const
	{
		return m_minTemperature;
	}
	double getMaxTemperature( void ) const
	{
		return m_maxTemperature;
	}
	double getMinLongitude( void ) const
	{
		return m_minLon;
	}
	double getMaxLongitude( void ) const
	{
		return m_maxLon;
	}
	double getMinLatitude( void ) const
	{
		return m_minLat;
	}
	double getMaxLatitude( void ) const
	{
		return m_maxLat;
	}
	template <typename ScalarT>
	gak::math::Rectangle< gak::math::GeoPosition<ScalarT> > getBoundingBox() const
	{
		return gak::math::Rectangle< gak::math::GeoPosition<ScalarT> >(
			m_minLon, m_maxLat,
			m_maxLon, m_minLat
		);
	}

};

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
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

}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
