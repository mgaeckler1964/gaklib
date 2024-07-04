/*
		Project:		GAKLIB
		Module:			gps.h
		Description:	GPS support
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

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

#ifndef GAK_GPS_H
#define GAK_GPS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/fmtNumber.h>
#include <gak/geometry.h>

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
namespace math
{

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

const double MIN_EARTH_RADIUS	= 6356752.0;
const double MAX_EARTH_RADIUS	= 6378137.0;
const double EARTH_RADIUS		= (MIN_EARTH_RADIUS + MAX_EARTH_RADIUS)/2;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief returns the distance of two points with known horizontal and vertical distance
	@param [in] horizontalDistance the horizontal distance of the two points
	@param [in] verticalDistance the vertical distance of the two points
*/
inline double getDistance( double horizontalDistance, double verticalDistance )
{
	double distance;

	// we use phytagoras, which is OK for small distances
	distance = getHypotenuse( horizontalDistance, verticalDistance );

	return distance;
}

/**
	@brief Calculates an angle in deggrees from its components
	@param [in] degree the angle in degrees
	@param [in] minutes the minutes part of the angle
	@param [in] seconds the seconds part of the angle
	@return the angle in degrees
*/
inline double ConvertDegree( double degree, double minutes, double seconds=0 )
{
	return degree + minutes/60.0 + seconds/3600.0;
}

inline void ConvertDegree( double angle, int *degree, unsigned *minutes, double *seconds )
{
	*degree = int(angle);
	if( angle < 0 )
	{
		angle = -angle+int(angle);
	}
	else
	{
		angle -= *degree;
	}
	angle *= 60;
	*minutes = unsigned(angle);
	angle -= *minutes;
	*seconds = angle * 60;
}

inline STRING ConvertDegree( double angle )
{
	int			degree;
	unsigned	minutes;
	double		seconds;

	ConvertDegree( angle, &degree, &minutes, &seconds );

	return formatNumber( degree ) + '°' + formatNumber( minutes ) + '\'' + formatFloat( seconds, 0, 6 ) + '\"'; 
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

typedef unsigned long tileid_t;

static const double degreePerTile = 0.5;
static const tileid_t maxTileIdPerLine = 360/degreePerTile+0.5;
static const double longOffset = 180;
static const double latOffset = 90;

template <typename ScalarT>
struct GeoPosition
{
	typedef ScalarT	scalar_t;

	/// the longitude in degrees
	ScalarT	longitude,
	/// the latitude in degrees
			latitude;

	static tileid_t getTileID( scalar_t longitude, scalar_t lattitude )
	{
		tileid_t longIdx = (longitude+longOffset)/degreePerTile;
		tileid_t latIdx = (lattitude+latOffset)/degreePerTile;
		tileid_t tileIdx = latIdx * maxTileIdPerLine + longIdx;

		return tileIdx;
	}
	static void getTile( tileid_t tileid, GeoPosition &lowerLeft, GeoPosition &upperRight )
	{
		tileid_t latIdx = tileid/maxTileIdPerLine;
		tileid_t longIdx = tileid%maxTileIdPerLine;
		lowerLeft.longitude = longIdx*degreePerTile - longOffset;
		lowerLeft.latitude = latIdx*degreePerTile - latOffset;

		upperRight.longitude = lowerLeft.longitude + degreePerTile;
		upperRight.latitude = lowerLeft.latitude + degreePerTile;
	}

	GeoPosition()
	{
	}

	GeoPosition( ScalarT longitude, ScalarT latitude) 
	: longitude(longitude), latitude(latitude)
	{
//		assert( longitude >= -180 && longitude <= 180 ) ;
//		assert( latitude >= -90 && latitude <= 90 ) ;
	}

	STRING toString() const
	{
		return ConvertDegree( longitude ) + '/' + ConvertDegree( latitude );
	}

	scalar_t getX() const
	{
		return longitude;
	}
	scalar_t getY() const
	{
		return latitude;
	}
	template <typename ScalarT2>
	const GeoPosition<scalar_t> &operator += ( const GeoPosition<ScalarT2> &other )
	{
		longitude += other.longitude;
		latitude += other.latitude;

		return *this;
	}
	template <typename ScalarT2>
	GeoPosition<scalar_t> operator + ( const GeoPosition<ScalarT2> &other )
	{
		return GeoPosition<scalar_t>( longitude+other.longitude, latitude+other.latitude );
	}

	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, longitude );
		gak::toBinaryStream( stream, latitude );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &longitude );
		gak::fromBinaryStream( stream, &latitude );
	}
};
	
/// @brief struct storing a GPS position
template <typename ScalarT>
struct GpsPosition : public GeoPosition<ScalarT>
{
	typedef GeoPosition<ScalarT>	Super;

	/// the height above sea level in meters
	ScalarT	height;

	GpsPosition()
	{
	}
	/**
		@brief creates a new GpsPosition
		@param [in] longitude the longitude in degrees
		@param [in] latitude the latitude in degrees
		@param [in] height the height above sea level in meters
	*/
	GpsPosition( double longitude, double latitude, double height=0 )
	: Super(longitude, latitude), height(height)
	{
	}
};

template< typename ViewPointT = Point<int> >
class GpsProjector
{
	public:
	typedef GeoPosition<double>		geo_point_t;
	typedef Rectangle<geo_point_t>	bounding_box_t;
	typedef ViewPointT				view_point_t;
	typedef Array<view_point_t>		view_points_t;
	typedef Rectangle<view_point_t>	view_port_t;

	private:
	geo_point_t		m_gpsTopLeft;
	view_point_t	m_viewPortTopLeft;

	double			m_angleWidth,
					m_angleHeight;
	double			m_geoWidth;
	int				m_viewPortWidth,
					m_viewPortHeight;

	double getLongitude( int x ) const
	{
		return m_gpsTopLeft.longitude + (x - m_viewPortTopLeft.x)*m_angleWidth / m_viewPortWidth;
	}
	double getLatitude( int y ) const
	{
		return m_gpsTopLeft.latitude + (y - m_viewPortTopLeft.y)*m_angleHeight / m_viewPortHeight;
	}
	int getX( double lon ) const
	{
		return int(m_viewPortTopLeft.x + (lon - m_gpsTopLeft.longitude)*m_viewPortWidth / m_angleWidth +0.5);
	}
	int getY( double lat ) const
	{
		return int(m_viewPortTopLeft.y + (lat - m_gpsTopLeft.latitude)*m_viewPortHeight / m_angleHeight +0.5);
	}

	public:
	GpsProjector() : m_gpsTopLeft( 0, 0 ), m_viewPortTopLeft( 0, 0 )
	{
	}
	GpsProjector( 
		const bounding_box_t	&boundingBox,
		const view_port_t		&viewPort 
	);

	geo_point_t getGeoPoint( const view_point_t &viewPosition ) const
	{
		return geo_point_t(
			getLongitude( viewPosition.x ),
			getLatitude( viewPosition.y )
		);
	}

	template <typename ScalarT>
	view_point_t getViewPoint( const GeoPosition<ScalarT> &geoPosition ) const
	{
		return view_point_t(
			getX( geoPosition.longitude ),
			getY( geoPosition.latitude )
		);
	}
	template <typename ScalarT>
	void getViewPoints(
		const Array< GeoPosition<ScalarT> > &geoPositions, view_points_t *result
	) const;

	double getGeoWidth() const
	{
		return m_geoWidth;
	}
	double getScale() const
	{
		return m_geoWidth / m_viewPortWidth;
	}
	double getAngleScale() const
	{
		return m_angleWidth / m_viewPortWidth;
	}
	bounding_box_t getBoundingBox() const
	{
		return bounding_box_t( m_gpsTopLeft, m_angleWidth, m_angleHeight );
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

/**
	@brief Returns the vertical distance of two GPS positions
	@param [in] start the first position
	@param [in] end the second position
	@return the vertical distance in meters
	@see GpsPosition
*/
template <typename ScalarT>
inline double getHeightDistance(
	const GpsPosition<ScalarT> &start,
	const GpsPosition<ScalarT> &end
)
{
	double	verticalDistance;

	verticalDistance = end.height-start.height;

	return verticalDistance;
}

/**
	@brief Returns the distance of two GPS positions
	@param [in] start the first position
	@param [in] end the second position
	@param [in] theRadius the earth radius in meters to use
	@return the distance in meters
	@see GpsPosition
*/
template <typename ScalarT1, typename ScalarT2>
double getDistance(
	const GeoPosition<ScalarT1> &start, const GeoPosition<ScalarT2> &end,
	const double theRadius = EARTH_RADIUS
);

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

template<typename ViewPointT>
GpsProjector<ViewPointT>::GpsProjector( 
	const bounding_box_t	&boundingBox, 
	const view_port_t		&viewPort 
)
: m_gpsTopLeft(boundingBox.topLeft), m_viewPortTopLeft(viewPort.topLeft)
{
	m_viewPortWidth = viewPort.bottomRight.x - viewPort.topLeft.x;
	m_viewPortHeight = viewPort.topLeft.y - viewPort.bottomRight.y;
	m_angleWidth = boundingBox.bottomRight.longitude - boundingBox.topLeft.longitude;
	m_angleHeight = boundingBox.topLeft.latitude - boundingBox.bottomRight.latitude;

	if( boundingBox.bottomRight.latitude < 0 && boundingBox.topLeft.latitude > 0 )
	{
		// equator crosses our bounding box
		geo_point_t	left( boundingBox.topLeft.longitude, 0 ),
					right( boundingBox.bottomRight.longitude, 0 ); 
		m_geoWidth = getDistance( left, right );
	}
	else if( boundingBox.bottomRight.latitude < 0 )
	{
		// south side of earth
		geo_point_t	left( boundingBox.topLeft.longitude, boundingBox.topLeft.latitude ),
					right( boundingBox.bottomRight.longitude, boundingBox.topLeft.latitude ); 
		m_geoWidth = getDistance( left, right );
	}
	else
	{
		// north side of earth
		geo_point_t	left( boundingBox.topLeft.longitude, boundingBox.bottomRight.latitude ),
					right( boundingBox.bottomRight.longitude, boundingBox.bottomRight.latitude ); 
		m_geoWidth = getDistance( left, right );
	}

	geo_point_t	top( boundingBox.topLeft.longitude, boundingBox.topLeft.latitude ),
				bottom(boundingBox.topLeft.longitude, boundingBox.bottomRight.latitude );
	double		geoHeight = getDistance( top, bottom );


	double geoRatio = m_geoWidth/geoHeight;
	double viewPortRatio = double(m_viewPortWidth)/double(m_viewPortHeight);
	if( abs(viewPortRatio) > abs(geoRatio) )
	{
		int newViewPortWidth = int(double(m_viewPortWidth)/abs(viewPortRatio) * geoRatio);
		int	inset = abs(m_viewPortWidth - newViewPortWidth) / 2;
		m_viewPortTopLeft.x += inset;

		m_viewPortWidth = newViewPortWidth;

	}
	else if( viewPortRatio < geoRatio )
	{
		int newViewPortHeight = int(double(m_viewPortHeight)*abs(viewPortRatio) / geoRatio);
		int inset = abs(m_viewPortHeight - newViewPortHeight) / 2;
		m_viewPortTopLeft.y += inset;
		m_viewPortHeight = newViewPortHeight;
	}
}

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

template<typename ViewPointT>
template <typename ScalarT>
void GpsProjector<ViewPointT>::getViewPoints(
	const Array< GeoPosition<ScalarT> > &geoPositions, view_points_t *result
) const
{
	result->setCapacity( geoPositions.size(), true );
	for(
		typename Array< GeoPosition<ScalarT> >::const_iterator it = geoPositions.cbegin(),
			endIT = geoPositions.cend();
		it != endIT;
		++it
	)
	{
		result->addElement( getViewPoint( *it ) );
	}
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief Returns the distance of two GPS positions
	@param [in] start the first position
	@param [in] end the second position
	@param [in] theRadius the earth radius in meters to use
	@return the distance in meters
	@see GpsPosition
*/
template <typename ScalarT1, typename ScalarT2>
double getDistance(
	const GeoPosition<ScalarT1> &start, const GeoPosition<ScalarT2> &end,
	const double theRadius
)
{
	// calculate the distance of one degree of the latidute circles
	const double distancePerDegree = theRadius*(M_PI/180.0);

	// calculate the difference of the two longitude values. We must check
	// whether the line crosses the 180 degree longitude
	double longitudeDifference = abs(start.longitude-end.longitude);
	if( longitudeDifference > 180 )
		longitudeDifference = 360 - longitudeDifference;

	// calculate the east<->west distance and the north<->south distance
	const double latiduteDistance = abs(start.latitude-end.latitude)*distancePerDegree;
	const double longitudeDistance = longitudeDifference*distancePerDegree*cos(degree2radians((start.latitude+end.latitude)/2));

	// let's use phytagoras, this is OK for small distances
	const double distance = getHypotenuse( latiduteDistance, longitudeDistance );

	return distance;
}

/**
	@brief Returns the distance of two GPS positions on the earth sphere
	@param [in] start the first position
	@param [in] end the second position
	@return the horizontal distance in meters
	@see GpsPosition
*/
template <typename ScalarT>
double getSphereDistance( 
	const GpsPosition<ScalarT> &start, const GpsPosition<ScalarT> &end 
)
{
	// find the radius of the erath to use. Note: does not return a correct value,
	// if line crosses the equator.
	const double startRadius = MIN_EARTH_RADIUS + cos( degree2radians(start.latitude) ) * (MAX_EARTH_RADIUS-MIN_EARTH_RADIUS) + start.height;
	const double endRadius   = MIN_EARTH_RADIUS + cos( degree2radians(  end.latitude) ) * (MAX_EARTH_RADIUS-MIN_EARTH_RADIUS) +   end.height;
	const double theRadius = medium(startRadius, endRadius);

	return getDistance( start, end, theRadius );
}


/**
	@brief Returns the distance of two GPS positions
	@param [in] start the first position
	@param [in] end the second position
	@return the distance in meters
	@see GpsPosition
*/
template <typename ScalarT>
double getDistance( 
	const GpsPosition<ScalarT> &start, 
	const GpsPosition<ScalarT> &end 
)
{
	const double sphereDistance = getSphereDistance( start, end );

	// the height is important too
	const double heightDistance = getHeightDistance( start, end );

	const double distance = getDistance( sphereDistance, heightDistance );

	return distance;
}

template <typename ScalarT>
double getBearing( const GeoPosition<ScalarT> &start, const GeoPosition<ScalarT> &end )
{
    double  lon1 = degree2radians(start.longitude);
    double  lon2 = degree2radians(end.longitude);
    double  lat1 = degree2radians(start.latitude);
    double  lat2 = degree2radians(end.latitude);
    double  deltaLon = lon2-lon1;
	double  deltaLat = lat2-lat1;
	double  cosLat2 = cos(lat2);

    double  bearing = (abs(deltaLon) < 1e-6 && abs(deltaLat) < 1e-6)
		? 0.0
		: atan2(
			sin(deltaLon)*cosLat2,
			cos(lat1)*sin(lat2)-sin(lat1)*cosLat2*cos(deltaLon)
		)
	;

    return bearing;
}

template <typename ScalarT>
inline double getBearingDegree( const GeoPosition<ScalarT> &start, const GeoPosition<ScalarT> &end )
{
	double bearing = getBearing( start, end );

	return radians2degree( bearing < 0 ? (M_PI - bearing) : bearing );
}

template <typename ScalarT>
GeoPosition<ScalarT> movePoint(
    const GeoPosition<ScalarT> &startPoint, double bearing, double distance
)
{
	double  lon1 = degree2radians(startPoint.longitude);
    double  lat1 = degree2radians(startPoint.latitude);

    double  angularDistance = distance/EARTH_RADIUS;
    double  cosAngularDistance = cos(angularDistance);
    double  sinAngularDistance = sin(angularDistance);
    double  cosLat1 = cos(lat1);
    double  sinLat1 = sin(lat1);

    double  lat2 = asin(
        sinLat1 * cosAngularDistance +
        cosLat1*sinAngularDistance * cos(bearing)
    );
    double  lon2 = lon1 + atan2(
        sin(bearing)*sinAngularDistance*cosLat1,
        cosAngularDistance-sinLat1*sin(lat2)
    );

    return GeoPosition<ScalarT>(radians2degree(lon2), radians2degree(lat2));
}

template <typename RectangleT, typename PointT>
inline void moveGeoRectangle( RectangleT *rect, const PointT &point )
{
	rect->move( point );

	if( rect->topLeft.latitude > 90 )
	{
		rect->move( PointT( 0, 90-rect->topLeft.latitude ) );
	}
	else if( rect->bottomRight.latitude < -90 )
	{
		rect->move( PointT( 0, -90-rect->bottomRight.latitude ) );
	}
} 

template <typename ScalarT, typename PointT>
inline void moveRectangle( Rectangle< GeoPosition<ScalarT> > *rect, const PointT &point )
{
	moveGeoRectangle( rect, point );
}

template <typename ScalarT, typename PointT>
inline void moveRectangle( Rectangle< GpsPosition<ScalarT> > *rect, const PointT &point )
{
	moveGeoRectangle( rect, point );
}


}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
