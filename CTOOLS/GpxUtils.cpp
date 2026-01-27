/*
		Project:		GAKLIB
		Module:			GpxUtils.cpp
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/date.h>
#include <gak/gps.h>
#include <gak/numericString.h>

#include <gak/GpxUtils.h>

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
void GPXtrack::LoadGpxTrack( xml::Element *theSegments )
{
	Date			startDate, endDate;
	long			startTime,
					endTime = 0,
					elapsedTime;
	int				year, month, day, hour, minute, second;
	GpxPosition		startPos, endPos;
	bool			first;
	double			heightDistance, sphereDistance, distance;
	double			speed, lastSpeed, acceleration;
	double			gradient;
	char			sign, lastSign=0;

	m_theSegments	= theSegments;

	acceleration = lastSpeed = 0.0;

	init();

	elapsedTime = 0;
	heightDistance = sphereDistance = distance = gradient = 0;

	m_minHeight = std::numeric_limits<int>::max();
	m_maxHeight = std::numeric_limits<int>::min();
	m_maxSpeed = 0;
	m_minAcc = std::numeric_limits<double>::max();
	m_maxAcc = std::numeric_limits<double>::min();
	m_minGradient = std::numeric_limits<double>::max();
	m_maxGradient = std::numeric_limits<double>::min();

	m_minLon = 180;
	m_maxLon = -180;
	m_minLat = 90;
	m_maxLat = -90;

	std::size_t	num = theSegments->getNumObjects();
	first = true;
	for( std::size_t i=0; i<num; i++ )
	{
		xml::Element *thePoint = theSegments->getElement( i );

		if( thePoint->getTag() == "trkpt" )
		{
			STRING	height, time;
			STRING	lon = thePoint->getAttribute( "lon" );
			STRING	lat = thePoint->getAttribute( "lat" );


			xml::Element *ele = thePoint->getElement( "ele" );
			if( ele )
				height = ele->getValue( xml::XML_MODE );
			else
				height = "";

			ele = thePoint->getElement( "time" );
			if( ele )
				time = ele->getValue( xml::XML_MODE );
			else
				time = "";

			year = month = day = hour = minute = second = 0;
			sscanf( time, "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second );


			if( !first )
			{
				startPos = endPos;
				startDate = endDate;
				startTime = endTime;
			}

			endPos.longitude = lon.getValueE<double>();
			endPos.latitude = lat.getValueE<double>();
			endPos.height = height.getValueE<double>();

			if( endPos.longitude < m_minLon )
			{
				m_minLon = endPos.longitude;
			}
			if( endPos.longitude > m_maxLon )
			{
				m_maxLon = endPos.longitude;
			}
			if( endPos.latitude < m_minLat )
			{
				m_minLat = endPos.latitude;
			}
			if( endPos.latitude > m_maxLat )
			{
				m_maxLat = endPos.latitude;
			}

			if( endPos.height < m_minHeight )
			{
				m_minHeight = int(endPos.height);
			}
			if( endPos.height > m_maxHeight )
			{
				m_maxHeight = int(endPos.height);
			}

			endDate.setDate(
				static_cast<unsigned char>(day),
				static_cast<Date::Month>(month),
				static_cast<unsigned short>(year)
			);
			endTime = hour*3600L + minute*60L + second;

			speed = 0;
			if( !first )
			{
				heightDistance = getHeightDistance( startPos, endPos );
				sphereDistance = getSphereDistance( startPos, endPos );

				distance = gak::math::getDistance( sphereDistance, heightDistance );

				m_totalDistance += distance;

				elapsedTime = (endDate-startDate)*24L*3600L + (endTime-startTime);
				m_totalTime1 += elapsedTime;

				if( elapsedTime > 0 )
				{
					speed = distance / (double)elapsedTime;

					if( speed > 0.5 )
					{
						m_totalTime2 += elapsedTime;
					}

					acceleration = (speed - lastSpeed)/elapsedTime;
					lastSpeed = speed;

					if( acceleration > m_maxAcc )
						m_maxAcc = acceleration;
					if( acceleration < m_minAcc )
						m_minAcc = acceleration;

					speed *= 3.6;

					if( speed > m_maxSpeed )
					{
						m_maxSpeed = speed;
					}
				}
				else
				{
					speed = 0;
					acceleration = 0;
				}

				sign = char(math::sign( heightDistance ));

				if( sphereDistance )
				{
					gradient = (heightDistance/sphereDistance)*100.0;
					if( sign == lastSign && abs(gradient) >= 1 )
					{
						if( heightDistance > 0 )
						{
							m_totalUp += heightDistance;
						}
						else
						{
							m_totalDown -= heightDistance;
						}

						if( gradient > m_maxGradient )
						{
							m_maxGradient = gradient;
						}
						if( gradient < m_minGradient )
						{
							m_minGradient = gradient;
						}
					}
					lastSign = sign;
				}
				else
				{
					gradient = 0;
				}
			}

			first = false;

			GPXpoint	&trackEntry = createElement();

			trackEntry.m_position = endPos;
			trackEntry.m_time = time;
			trackEntry.m_timestamp = DateTime(
				static_cast<unsigned char>(day), DateTime::Month(month),static_cast<unsigned short>(year),
				static_cast<unsigned char>(hour),static_cast<unsigned char>(minute),static_cast<unsigned char>(second)
			);

			trackEntry.m_longitude = lon;
			trackEntry.m_latitude = lat;
			trackEntry.m_heightDistance = heightDistance;
			trackEntry.m_sphereDistance = sphereDistance;
			trackEntry.m_distance = distance;
			trackEntry.m_totalDistance = int(m_totalDistance);
			trackEntry.m_height = int(endPos.height);
			trackEntry.m_speed = speed;
			trackEntry.m_acceleration = acceleration;
			trackEntry.m_elapsedTime = elapsedTime;
			trackEntry.m_totalTime1 = m_totalTime1;
			trackEntry.m_totalTime2 = m_totalTime2;
			trackEntry.m_totalUp = m_totalUp;
			trackEntry.m_totalDown = m_totalDown;
			trackEntry.m_gradient = gradient;
			trackEntry.m_heartRate = 0;
			trackEntry.m_temperature = 0;

			if( !m_ts_valid )
			{
				m_timestamp = trackEntry.m_timestamp;
				m_ts_valid = true;
			}
		}
	}
	if( m_minHeight > m_maxHeight )
	{
		m_minHeight = m_maxHeight = 0;
	}
	if( m_minAcc > m_maxAcc )
	{
		m_minAcc = m_maxAcc = 0;
	}
	if( m_minGradient > m_maxGradient )
	{
		m_minGradient = m_maxGradient = 0;
	}
}


// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void GPXtrack::LoadGpxTrack( xml::Document *theGpxFile, const STRING &trackName )
{
	init();

	if( theGpxFile )
	{
		xml::Element *theRoot = theGpxFile->getElement( "gpx" );
		if( theRoot )
		{
			STRING	name;

			std::size_t	num = theRoot->getNumObjects();
			for( std::size_t i=0; i<num; i++ )
			{
				xml::Element *theTrack = theRoot->getElement( i );
				if( theTrack && theTrack->getTag() == "trk" )
				{
					xml::Element	*theName = theTrack->getElement( "name" );
					if( theName )
					{
						name = theName->getValue( xml::PLAIN_MODE );
						if( name == trackName )
						{
							xml::Element	*theSegments = theTrack->getElement( "trkseg" );
							if( theSegments )
							{
								LoadGpxTrack( theSegments );
								break;
							}
						}
					}
				}
			}
		}
	}
}
/*
CRP files use a “CSV” type file format with tab-delimited fields. Line delimiter: CR/LF, also after the last line. As far as I have yet figured out, a CRP file with n data rows consists of the following lines:

	* line 1: “HRMProfilDatas”
	* line 2: 8 fields
		 1. CRP version (?)
		 2. max. height [m]
		 3. min. height [m]
		 4. height of graphical profile [px]
		 5. width of graphical profile [px]
		 6. X-axis grid [km]
		 7. Y-axis grid [m]
		 8. vertical scaling factor for graphical profile
	* line 3,...,n+2: actual data rows, 9 fields per row
		 1. heart rate
		 2. speed [0.1 km/h]
		 3. distance [0.01 km]
		 4. height [m]
		 5. color [0..4]
		 6. symbol [0..42]
		 7. temperature [°C], format "9,9"
		 8. time, format "HH:MM:SS"
		 9. comment
	* line n+3: “***”
	* line n+4: 16 fields
		 1. day's distance [0.01 km]
		 2. height difference (ascent)
		 3. height difference (descent)
		 4. max. speed [0.01 km/h]
		 5. gross average speed [0.01 km/h]
		 6. avg. heart rate [1/min]
		 7. max. heart rate [1/min]
		 8. number of data rows minus 2
		 9. max. heart rate [% HFmax]
		10. avg. heart rate [% HFmax]
		11. driver's personal max. heart rate [1/min]
		12. min. temperature [°C] [format 9,9]
		13. max. temperature [°C] [format 9,9]
		14. Y-axis minimum for graphical profile [m]
		15. net trip duration (HH:MM:SS)
		16. net average speed [km/h] (Format 99,99)
	* line n+5: 6 fields
		 1. date (DD.MM.YYYY)
		 2. trip start time (HH:MM:SS)
		 3. gross trip duration (HH:MM:SS)
		 4. category
		 5. difficulty
		 6. title
	* line n+6: 5 fields
		 1. gross trip duration [s] (= 20*(n-1))
		 2. ??
		 3. ??
		 4. ??
		 5. ??
	* line n+7: 2 fields
		 1. ??
		 2. empty
	* line n+8: 3 fields
		 1. measurement interval [s] (normally 20)
		 2. “CRP mode”
		 3. empty
	* line n+9, n+10: empty
	* line n+11, n+12: “***”
	* line n+13 ff: comment (optional)
*/

STRING GPXtrack::LoadCrpTrack( const STRING &crpFilename )
{
	FILE		*fp;
	STRING		line, trackTitle;
	int			numBytes;
	char		time[128];

	int			day, month, year;

	int			heartRate,
				speedI,
				totalDistance,
				height,
				dummy,
				temperatureI,
				tempDeg,
				hour,
				minute,
				second;

	double		speedKm, speedM, lastSpeed, acceleration;;
	int			lastDistance, distance, heightDistance;
	double		sphereDistance, gradient;
	double		temperature;
	int			lastHeight;
	long		startTime, endTime, elapsedTime;
	bool		first = true;

	init();

	lastHeight = 0;
	startTime = 0;
	lastDistance = 0;
	acceleration = lastSpeed = 0.0;
	elapsedTime = 0;
	heightDistance = distance = 0;
	sphereDistance = gradient = 0;

	m_minHeight = std::numeric_limits<int>::max();
	m_maxHeight = std::numeric_limits<int>::min();
	m_maxSpeed = 0;
	m_minAcc = std::numeric_limits<double>::max();
	m_maxAcc = std::numeric_limits<double>::min();
	m_minGradient = std::numeric_limits<double>::max();
	m_maxGradient = std::numeric_limits<double>::min();
	m_minHeartRate = std::numeric_limits<int>::max();
	m_maxHeartRate = std::numeric_limits<int>::min();
	m_minTemperature = std::numeric_limits<double>::max();
	m_maxTemperature = std::numeric_limits<double>::min();

	fp = fopen( crpFilename, "r" );
	if( fp )
	{
		line << fp;
		if( line == "HRMProfilDatas" )
		{
			// search the date file
			while( !feof( fp ) && line != "***" )
				line << fp;

			line << fp;		// ignore statistics
			line << fp;

			sscanf(
				line,
				"%d.%d.%d %d:%d:%d %d:%d:%d %d %d %n",
				&day, &month, &year,
				&dummy, &dummy, &dummy,
				&dummy, &dummy, &dummy,
				&dummy, &dummy,
				&numBytes
			);

			line += (size_t)numBytes;
			trackTitle = line;


			fseek( fp, 0, SEEK_SET );

			line << fp;		// HRMProfilDatas
			line << fp;		// ignore header
			line << fp;		// first line

			while( !feof( fp ) && line != "***" )
			{
				sscanf( line, "%d %d %d %d %d %d %d,%d %d:%d:%d",
					&heartRate,
					&speedI,
					&totalDistance,
					&height,
					&dummy,		// color
					&dummy,		// symbol
					&temperatureI,
					&tempDeg,
					&hour,
					&minute,
					&second
				);

				totalDistance *= 10;
				speedKm = speedI / 10.0;
				speedM = speedKm / 3.6;
				sprintf( time, "%04d-%02d-%02dT%02d:%02d:%02dZ", year, month, day, hour, minute, second );

				if( height < m_minHeight )
					m_minHeight = height;
				if( height > m_maxHeight )
					m_maxHeight = height;

				if( temperatureI >= 0 )
					temperature = (double)temperatureI + (double)tempDeg/10.0;
				else
					temperature = (double)temperatureI - (double)tempDeg/10.0;

				if( temperature > m_maxTemperature )
					m_maxTemperature = temperature;
				if( temperature < m_minTemperature )
					m_minTemperature = temperature;

				if( heartRate > m_maxHeartRate )
					m_maxHeartRate = heartRate;
				if( heartRate < m_minHeartRate )
					m_minHeartRate = heartRate;

				endTime = hour*3600L+minute*60L+second;
				if( !first )
				{
					distance = totalDistance - lastDistance;

					heightDistance = height - lastHeight;
					if( heightDistance > 0 )
					{
						m_totalUp += heightDistance;
					}
					else
					{
						m_totalDown -= heightDistance;
					}

					double tmpValue =	double(distance)*double(distance) -
								double(heightDistance)*double(heightDistance);
					if( tmpValue > 0 )
						sphereDistance = sqrt( tmpValue );
					else
						sphereDistance = 0;

					if( sphereDistance )
					{
						gradient = double(heightDistance)/double(sphereDistance)*100.0;
						if( gradient > m_maxGradient )
							m_maxGradient = gradient;
						if( gradient < m_minGradient )
							m_minGradient = gradient;
					}
					else
						gradient = 0;

					elapsedTime = endTime - startTime;
					if( elapsedTime < 0 )
					{
						elapsedTime += 24L * 3600L;
					}
					m_totalTime1 += elapsedTime;
					if( speedKm )
					{
						m_totalTime2 += elapsedTime;
					}

					if( elapsedTime )
						acceleration = (speedM - lastSpeed)/elapsedTime;
					else
						acceleration = 0;


					if( acceleration > m_maxAcc )
						m_maxAcc = acceleration;
					if( acceleration < m_minAcc )
						m_minAcc = acceleration;
				}

				lastDistance = totalDistance;
				lastHeight = height;
				lastSpeed = speedM;

				if( speedKm > m_maxSpeed )
				{
					m_maxSpeed = speedKm;
				}

				GPXpoint	&trackEntry = createElement();
				trackEntry.m_time = time;
				trackEntry.m_timestamp = DateTime( 
					static_cast<unsigned char>(day), DateTime::Month(month), static_cast<unsigned short>(year), 
					static_cast<unsigned char>(hour), static_cast<unsigned char>(minute), static_cast<unsigned char>(second) 
				);
				trackEntry.m_longitude = "";
				trackEntry.m_latitude = "";
				trackEntry.m_heightDistance = heightDistance;
				trackEntry.m_sphereDistance = sphereDistance;
				trackEntry.m_distance = distance;
				trackEntry.m_totalDistance = totalDistance;
				trackEntry.m_height = height;
				trackEntry.m_speed = speedKm;
				trackEntry.m_acceleration = acceleration;
				trackEntry.m_elapsedTime = elapsedTime;
				trackEntry.m_totalTime1 = m_totalTime1;
				trackEntry.m_totalTime2 = m_totalTime2;
				trackEntry.m_totalUp = m_totalUp;
				trackEntry.m_totalDown = m_totalDown;
				trackEntry.m_gradient = gradient;
				trackEntry.m_heartRate = heartRate;
				trackEntry.m_temperature = temperature;

				if( !m_ts_valid )
				{
					m_timestamp = trackEntry.m_timestamp;
					m_ts_valid = true;
				}

				line << fp;		// next line

				startTime = endTime;
				first = false;
			}

			m_totalDistance = totalDistance;
		}

		fclose( fp );
	}

	if( m_minHeight > m_maxHeight )
	{
		m_minHeight = m_maxHeight = 0;
	}
	if( m_minAcc > m_maxAcc )
	{
		m_minAcc = m_maxAcc = 0;
	}
	if( m_minGradient > m_maxGradient )
	{
		m_minGradient = m_maxGradient = 0;
	}
	if( m_minHeartRate > m_maxHeartRate )
	{
		m_minHeartRate = m_maxHeartRate = 0;
	}
	if( m_minTemperature > m_maxTemperature )
	{
		m_minTemperature = m_maxTemperature = 0.0;
	}
	return trackTitle;
}

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

