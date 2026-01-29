/*
		Project:		GAKLIB
		Module:			geometry.h
		Description:	
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

#ifndef GAK_GEOMETRY_H
#define GAK_GEOMETRY_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

/**
	@brief returns the length of the hypotenuse  of a right-angled triangle
	@param [in] first the length of the first cathetus
	@param [in] second the length of the second cathetus
	@tparam ScalarT must be a numerical type
*/
template <typename ScalarT>
inline ScalarT getHypotenuse( ScalarT first, ScalarT second )
{
	return sqrt( first*first + second*second );
}
  
/**
	@brief converts an angle from degree to radians
	@param [in] x the angle in degrees
	@return the angle in radians
	@tparam ScalarT must be a numerical type
*/
template <typename ScalarT>
inline ScalarT degree2radians( ScalarT x )
{
	return ScalarT((x * 2.0 * M_PI) / 360.0);
}

/**
	@brief converts an angle from radians to degree
	@param [in] x the angle in radians
	@return the angle in degree 
	@tparam ScalarT must be a numerical type
*/
template <typename ScalarT>
inline ScalarT radians2degree( ScalarT x )
{
	return ScalarT((x * 360.0) / (2.0 * M_PI));
}

template <typename ScalarT>
inline bool inRange( ScalarT min, ScalarT x, ScalarT max )
{
	assert( min <= max );
	return min <= x && x <= max;
}

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename ScalarT>
struct PolarPoint;

template <typename ScalarT>
struct Point
{
	typedef ScalarT	scalar_t;

	scalar_t	x,
				y;

	Point()
	{
	}
	Point(scalar_t x, scalar_t y) : x(x), y(y)
	{
	}
	template <typename ScalarT2>
	Point( const Point<ScalarT2> &other )
	: x(scalar_t(other.x)), y(scalar_t(other.y))
	{
	}
	template <typename ScalarT2>
	Point &operator = ( const Point<ScalarT2> &other )
	{
		x = scalar_t(other.x);
		y = scalar_t(other.y);

		return *this;
	}

	template <typename ScalarT2>
	Point( const PolarPoint<ScalarT2> &other );
	template <typename ScalarT2>
	Point &operator = ( const PolarPoint<ScalarT2> &other );

	scalar_t getX() const
	{
		return x;
	}
	scalar_t getY() const
	{
		return y;
	}
	scalar_t getRadius() const
	{
		return getHypotenuse( x, y );
	}
	scalar_t getAngle( scalar_t radius ) const
	{
		scalar_t	angle;
		if( radius == 0 )
		{
			angle = 0;
		}
		else
		{
			angle = acos( x/radius );
			if( y < 0 )
			{
				angle = -angle;
			}
		}
		return angle;
	}
	int compare( const Point<scalar_t> &other ) const
	{
		int compareResult = gak::compare( this->x, other.x );
		if( !compareResult )
		{
			compareResult = gak::compare( this->y, other.y );
		}

		return compareResult;
	}
	template <typename ScalarT2>
	const Point<scalar_t> &operator += ( const Point<ScalarT2> &other )
	{
		x += other.x;
		y += other.y;

		return *this;
	}
	template <typename ScalarT2>
	Point<scalar_t> operator + ( const Point<ScalarT2> &other )
	{
		return Point<scalar_t>( x+other.x, y+other.y );
	}
};

template <typename ScalarT>
struct PolarPoint
{
	typedef ScalarT	scalar_t;

	scalar_t	radius, angle;

	PolarPoint()
	{
	}
	PolarPoint( scalar_t radius, scalar_t angle )
	: radius(radius), angle(angle)
	{
	} 
	template <typename ScalarT2>
	PolarPoint( const PolarPoint<ScalarT2> &other )
	: radius(scalar_t(other.radius)), angle(scalar_t(other.angle))
	{
	}
	template <typename ScalarT2>
	PolarPoint &operator = ( const PolarPoint<ScalarT2> &other )
	{
		radius = scalar_t(other.radius);
		angle = scalar_t(other.angle);

		return *this;
	}
	template <typename ScalarT2>
	PolarPoint( const Point<ScalarT2> &other );

	template <typename ScalarT2>
	PolarPoint &operator = ( const Point<ScalarT2> &other );

	scalar_t getX() const
	{
		return cos( angle ) * radius;
	}
	scalar_t getY() const
	{
		return sin( angle ) * radius;
	}
};

template <typename PointT>
struct Rectangle
{
	typedef PointT						point_t;
	typedef typename PointT::scalar_t	scalar_t;

	PointT	topLeft,
			bottomRight;

	Rectangle( const point_t &topLeft, const point_t &bottomRight )
	: topLeft( topLeft ), bottomRight( bottomRight )
	{
	}
	Rectangle( scalar_t left, scalar_t top, scalar_t right, scalar_t bottom )
	: topLeft( left, top ), bottomRight( right, bottom )
	{
	}
	Rectangle( const point_t &topLeft, scalar_t width, scalar_t height )
	: topLeft( topLeft ), bottomRight( topLeft.getX()+width, topLeft.getY()-height )
	{
	}

	scalar_t getWidth() const
	{
		return abs(bottomRight.getX() - topLeft.getX());
	}
	scalar_t getHeight() const
	{
		return abs(bottomRight.getY() - topLeft.getY());
	}
	int compare( const Rectangle<scalar_t> &other ) const
	{
		int compareResult = gak::compare( this->topLeft, other.topLeft );
		if( !compareResult )
		{
			compareResult = gak::compare( this->bottomRight, other.bottomRight );
		}

		return compareResult;
	}

	template <typename PointT2>
	void move( const PointT2 &offset )
	{
		topLeft += offset;
		bottomRight += offset;
	}

	template <typename PointT2>
	bool contains( const PointT2 &point )
	{
		const typename PointT2::scalar_t	x = point.getX(),
											y = point.getY();
		scalar_t	topLeftX = topLeft.getX(),
					topLeftY = topLeft.getY(),
					bottomRightX = bottomRight.getX(),
					bottomRightY = bottomRight.getY();
		scalar_t	minX = min( topLeftX, bottomRightX ),
					maxX = max( topLeftX, bottomRightX ),
					minY = min( topLeftY, bottomRightY ),
					maxY = max( topLeftY, bottomRightY );

		return inRange( minX, x, maxX ) && inRange( minY, y, maxY );
	}
};

template <typename PointT>
struct Polygon
{
	typedef PointT								point_t;
	typedef Array<point_t>						points_t;
	typedef typename PointT::scalar_t			scalar_t;
	typedef typename points_t::const_iterator	const_iterator;

	points_t	points;

	const_iterator cbegin() const
	{
		return points.cbegin();
	}
	const_iterator cend() const
	{
		return points.cend();
	}
	void toBinaryStream( std::ostream &stream ) const
	{
		gak::toBinaryStream( stream, points );
	}
	void fromBinaryStream( std::istream &stream )
	{
		gak::fromBinaryStream( stream, &points );
	}
};

template <typename ScalarT>
struct Circle
{
	Point<ScalarT>	center;
	ScalarT			radius;

	Circle( const Point<ScalarT> &center, ScalarT radius )
	: center( center ), radius( radius )
	{
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

template <typename ScalarT>
template <typename ScalarT2>
Point<ScalarT>::Point( const PolarPoint<ScalarT2> &other )
{
	x = scalar_t(other.getX());
	y = scalar_t(other.getY());
}

template <typename ScalarT>
template <typename ScalarT2>
Point<ScalarT> &Point<ScalarT>::operator = ( const PolarPoint<ScalarT2> &other )
{
	x = scalar_t(other.getX());
	y = scalar_t(other.getY());

	return *this;
}

template <typename ScalarT>
template <typename ScalarT2>
PolarPoint<ScalarT>::PolarPoint( const Point<ScalarT2> &other )
{
	ScalarT2	otherRadius = other.getRadius();
	radius = scalar_t(otherRadius);
	angle = scalar_t(other.getAngle( otherRadius ));
}

template <typename ScalarT>
template <typename ScalarT2>
PolarPoint<ScalarT> &PolarPoint<ScalarT>::operator = ( const Point<ScalarT2> &other )
{
	ScalarT2	otherRadius = other.getRadius();
	radius = scalar_t(otherRadius);
	angle = scalar_t(other.getAngle( otherRadius ));

	return *this;
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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

template <typename ScalarT1, typename ScalarT2>
inline double getDistance( const Point<ScalarT1> &start, const Point<ScalarT2> &end )
{
	return getHypotenuse( abs( start.x - end.x ), abs( start.y - end.y ) );
}

template <typename RectangleT, typename PointT>
inline void moveRectangle( RectangleT *rect, const PointT &point )
{
	rect->move( point );
} 


template <typename ContainerT>
Rectangle<typename ContainerT::value_type> getBoundingBox( const ContainerT &points )
{
	typedef typename ContainerT::value_type	point_t;
	typedef typename point_t::scalar_t		scalar_t;

	scalar_t	left = std::numeric_limits<scalar_t>::max(),
				right = std::numeric_limits<scalar_t>::min() <= 0 ? std::numeric_limits<scalar_t>::min() : std::numeric_limits<scalar_t>::max(),
				top = std::numeric_limits<scalar_t>::min() <= 0 ? std::numeric_limits<scalar_t>::min() : std::numeric_limits<scalar_t>::max(),
				bottom = std::numeric_limits<scalar_t>::max();

	for(
		typename ContainerT::const_iterator it = points.cbegin(), endIT = points.cend();
		it != endIT;
		++it
	)
	{
		scalar_t	x = it->getX(), y = it->getY();
		if( left > x )
		{
			left = x;
		}
		if( right < x )
		{
			right = x;
		}
		if( top < y )
		{
			top = y;
		}
		if( bottom > y )
		{
			bottom = y;
		}
	}

	return Rectangle<point_t>(point_t(left,top),point_t(right,bottom));
}

}	// namespace math
}	// namespace gak

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// GAK_GEOMETRY_H
