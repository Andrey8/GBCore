#include "Geometry.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>



using Point = Core::Geometry::Point;



double Core::Geometry::GetAbsoluteAngle( Vector const & v )
{
	double const x = v.X();
	double const y = v.Y();

	if ( x == 0 )
	{
		if ( y >= 0 )
		{
			return PI / 2;
		}
		else
		{
			return ( 3 * PI / 2 );
		}
	}

	double const arctang = atan( y / x );

	if ( x > 0 )
	{
		if ( y >= 0 )
		{
			return arctang;
		}
		else
		{
			return ( 2 * PI + arctang );
		}
	}
	else
	{
		return ( PI + arctang );
	}
}

bool Core::Geometry::LineSeparatesPoints( Line const & line, Point const & p1, Point const & p2 )
{
	Vector const v( line.P1(), line.P2() );
	Vector const v1( line.P1(), p1 );
	Vector const v2( line.P2(), p2 );

	return ( VectorIsBetweenTwoOthers( v, v1, v2 ) || VectorIsBetweenTwoOthers( v, -v1, -v2 ) );
}

bool Core::Geometry::VectorIsBetweenTwoOthers( Vector const & v, Vector const & v1, Vector const & v2 )
{
	double const a = GetAbsoluteAngle( v );
	double const a1 = GetAbsoluteAngle( v1 );
	double const a2 = GetAbsoluteAngle( v2 );

	double const smallerAngle = std::min( a1, a2 );
	double const biggerAngle = std::max( a1, a2 );
	if ( biggerAngle - smallerAngle <= PI )
	{
		if ( a > smallerAngle && a < biggerAngle )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ( a < smallerAngle || a > biggerAngle )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

Point Core::Geometry::GetMidpoint( Point const & p1, Point const & p2 )
{
    return Point( ( p1.X() + p2.X() ) / 2, ( p1.Y() + p2.Y() ) / 2 );
}

Point Core::Geometry::GetPerpendicularBase( Point const & source, Point const & p1, Point const & p2 )
{
    if ( p1 == p2 )
    {
        throw std::invalid_argument( "Line is defined by two equivalent points!\n" );
    }

    double const x = source.X();
    double const y = source.Y();
    double const x1 = p1.X();
    double const y1 = p1.Y();
    double const x2 = p2.X();
    double const y2 = p2.Y();

    double const v12x = x2 - x1;
    double const v12y = y2 - y1;
    double const denum = v12x * v12x + v12y * v12y;

    double const x0 = ( v12x * v12x * x + v12y * v12y * x1 + v12x * v12y * ( y - y1 ) ) / denum;
    double const y0 = ( v12x * v12x * y1 + v12y * v12y * y + v12x * v12y * ( x - x1 ) ) / denum;

    return Point( x0, y0 );
}

Point Core::Geometry::GetPointOnLineSegment( Point const & p1, Point const & p2, double ratio )
{
	if ( ratio <= 0 )
    {
		throw std::invalid_argument( "ERROR : ratio <= 0.\n" );
    }

	return GetPointOnLine( p1, p2, ratio );
}

std::vector< Point > Core::Geometry::GetIntersectionPoints( const Core::Geometry::LineSegment & ls1, const Core::Geometry::LineSegment & ls2 )
{
	if ( !LineSeparatesPoints( Line( ls1 ), ls2.P1(), ls2.P2() ) ||
		 !LineSeparatesPoints( Line( ls2 ), ls1.P1(), ls1.P2() ) )
	{
		return std::vector< Point >();
	}

	return GetIntersectionPoints( Line( ls1 ), Line( ls2 ), true );
}

std::vector<Core::Geometry::Point> Core::Geometry::GetIntersectionPoints( const Core::Geometry::Circle & circle, const Core::Geometry::LineSegment & ls )
{
	std::vector< Point > const intersectionPoints = GetIntersectionPoints( circle, Line( ls ) );

	switch ( intersectionPoints.size() )
	{
	case 2 :
	{
		Point const p1 = intersectionPoints[ 0 ];
		Point const p2 = intersectionPoints[ 1 ];

		bool const p1IsInside = PointIsBetweenTwoOthers( p1, ls.P1(), ls.P2() );
		bool const p2IsInside = PointIsBetweenTwoOthers( p2, ls.P1(), ls.P2() );
		if ( p1IsInside && p2IsInside )
		{
			return intersectionPoints;
		}
		else if ( p1IsInside )
		{
			return std::vector< Point >{ p1 };
		}
		else if ( p2IsInside )
		{
			return std::vector< Point >{ p2 };
		}

		return std::vector< Point >();
	}
	case 1 :
	{
		Point const p = intersectionPoints[ 0 ];

		if ( PointIsBetweenTwoOthers( p, ls.P1(), ls.P2() ) )
		{
			return intersectionPoints;
		}

		return std::vector< Point >();
	}
	case 0 :
	{
		return std::vector< Point >();
	}
	default :
	{
		throw std::logic_error( "ERROR : invalid number of intersection points.\n" );
	}
	}
}

Core::Geometry::Triple<double> Core::Geometry::GetGeneralCoefficients( const Core::Geometry::Line & line )
{
	Point const p1 = line.P1();
	Point const p2 = line.P2();

	double const x1 = p1.X();
	double const y1 = p1.Y();
	double const x2 = p2.X();
	double const y2 = p2.Y();

	if ( y1 != 0 && y2 != 0 )
	{
		if ( x1 / y1 != x2 / y2 )
		{
			double const d = x1 * y2 - x2 * y1;

			return Triple< double >( ( y1 - y2 ) / d, ( x2 - x1 ) / d, 1 );
		}
		else
		{
			double const k = x1 / y1;

			return Triple< double >( 1, -k, 0 );
		}
	}
	else if ( y1 == 0 && y2 != 0 )
	{
		if ( x1 != 0 )
		{
			return Triple< double >( -1 / x1, ( x2 - x1 ) / ( x1 * y2 ), 1 );
		}
		else
		{
			return Triple< double >( 1, -x2 / y2, 0 );
		}
	}
	else if ( y2 == 0 && y1 != 0 )
	{
		if ( x2 != 0 )
		{
			return Triple< double >( -1 / x2, ( x1 - x2 ) / ( x2 * y1 ), 1 );
		}
		else
		{
			return Triple< double >( 1, -x1 / y1, 0 );
		}
	}
	else
	{
		return Triple< double >( 0, 1, 0 );
	}
}

std::vector<Core::Geometry::Point> Core::Geometry::GetIntersectionPoints(const Core::Geometry::Circle & circle, const Core::Geometry::Line & line )
{
	Point const p1 = line.P1();
	Point const p2 = line.P2();
	Point const circleCenter = circle.GetCenter();
	double const r = circle.GetRadius();

	double const x1 = p1.X();
	double const y1 = p1.Y();
	double const x2 = p2.X();
	double const y2 = p2.Y();
	double const x0 = circleCenter.X();
	double const y0 = circleCenter.Y();

	Triple< double > const generalCoefficients = GetGeneralCoefficients( line );
	double const a = generalCoefficients.GetValue1();
	double const b = generalCoefficients.GetValue2();
	double const c = generalCoefficients.GetValue3();

	Point const perpBase( ( b * ( b * x0 - a * y0 ) - a * c ) / ( a * a + b * b ),
							( a * ( a * y0 - b * x0 ) - b * c ) / ( a * a + b * b ) );
	double const h = std::abs( ( y2 - y1 ) * x0 - ( x2 - x1 ) * y0 + x2 * y1 - x1 * y2 ) /
					 std::sqrt( ( y2 - y1 ) * ( y2 - y1 ) + ( x2 - x1 ) * ( x2 - x1 ) );
	if ( h < r )
	{
		double const m = std::sqrt( r * r - h * h );
		Vector const v( b * m / std::sqrt( a * a + b * b ), -a * m / std::sqrt( a * a + b * b ) );

		return std::vector< Point >{ perpBase + v, perpBase - v };
	}
	else if ( h == r )
	{
		return std::vector< Point >{ perpBase };
	}
	else
	{
		return std::vector< Point >();
	}
}

Core::Geometry::Point Core::Geometry::operator+(const Core::Geometry::Point & p, const Core::Geometry::Vector & v )
{
	return Point( p.X() + v.X(), p.Y() + v.Y() );
}

Core::Geometry::Point Core::Geometry::operator-(const Core::Geometry::Point & p, const Core::Geometry::Vector & v )
{
	return Point( p.X() - v.X(), p.Y() - v.Y() );
}

bool Core::Geometry::PointIsBetweenTwoOthers(const Core::Geometry::Point & p, const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
{
	if ( p1.X() < p2.X() )
	{
		return ( p1.X() <= p.X() && p.X() <= p2.X() );
	}
	else if ( p1.X() > p2.X() )
	{
		return ( p1.X() >= p.X() && p.X() >= p2.X() );
	}

	if ( p1.Y() < p2.Y() )
	{
		return ( p1.Y() <= p.Y() && p.Y() <= p2.Y() );
	}
	else if ( p1.Y() > p2.Y() )
	{
		return ( p1.Y() >= p.Y() && p.Y() >= p2.Y() );
	}

	return p == p1;
}

std::vector<Core::Geometry::Point> Core::Geometry::GetIntersectionPoints(const Core::Geometry::Line & line1, const Core::Geometry::Line & line2, const Core::Geometry::Rect & boundingBox)
{
	LineSegment const ls1 = GetIntersectionLineSegment( line1, boundingBox ).front();
	LineSegment const ls2 = GetIntersectionLineSegment( line2, boundingBox ).front();

	return GetIntersectionPoints( ls1, ls2 );
}

std::vector< Core::Geometry::LineSegment > Core::Geometry::GetIntersectionLineSegment(const Core::Geometry::Line & line, const Core::Geometry::Rect & rect)
{
	if ( !line.IsHorizontal() && !line.IsVertical() )
	{
		std::vector< Point > const points1 = GetPointOnLineByOrdinate( line, rect.GetBottomY() );
		std::vector< Point > const points2 = GetPointOnLineByOrdinate( line, rect.GetTopY() );

		std::vector< Point > const points3 = GetPointOnLineByAbscissa( line, rect.GetLeftX() );
		std::vector< Point > const points4 = GetPointOnLineByAbscissa( line, rect.GetRightX() );

		if ( points1.size() != 1 || points2.size() != 1 || points3.size() != 1 || points4.size() != 1 )
		{
			throw std::logic_error( "ERROR : a non-horizontal and non-vertical line has !=1 point with the given ordinate or abscissa.\n" );
		}

		Point const p1 = std::min( points1[ 0 ], points2[ 0 ] );
		Point const p2 = std::max( points1[ 0 ], points2[ 0 ] );
		Point const p3 = std::min( points3[ 0 ], points4[ 0 ] );
		Point const p4 = std::max( points3[ 0 ], points4[ 0 ] );

		if ( p1 <= p3 )
		{
			if ( p2 < p3 )
			{
				return std::vector< LineSegment >();
			}
			else
			{
				return std::vector< LineSegment >{ LineSegment( p3, std::min( p2, p4 ) ) };
			}
		}
		else
		{
			if ( p4 < p1 )
			{
				return std::vector< LineSegment >();
			}
			else
			{
				return std::vector< LineSegment >{ LineSegment( p1, std::min( p4, p2 ) ) };
			}
		}
	}
	else if ( line.IsHorizontal() )
	{
		double const y = line.P1().Y();

		if ( y < rect.GetBottomY() || y > rect.GetTopY() )
		{
			return std::vector< LineSegment >();
		}

		return std::vector< LineSegment >{ LineSegment( Point( rect.GetLeftX(), y ), Point( rect.GetRightX(), y ) ) };
	}
	else if ( line.IsVertical() )
	{
		double const x = line.P1().X();

		if ( x < rect.GetLeftX() || x > rect.GetRightX() )
		{
			return std::vector< LineSegment >();
		}

		return std::vector< LineSegment >{ LineSegment( Point( x, rect.GetBottomY() ), Point( x, rect.GetTopY() ) ) };
	}

	throw std::logic_error( "ERROR : a line is non-horizontal, non-vertical and even non-sloping.\n" );
}

std::vector<Core::Geometry::Point> Core::Geometry::GetPointOnLineByOrdinate(const Core::Geometry::Line & line, double y)
{
	if ( line.IsHorizontal() )
	{
		if ( line.P1().Y() == y )
		{
			throw std::logic_error( "ERROR : horizontal line ordinate = the argument ordinate.\n" );
		}

		return std::vector< Point >();
	}

	double const x1 = line.P1().X();
	double const y1 = line.P1().Y();
	double const x2 = line.P2().X();
	double const y2 = line.P2().Y();

	double const x0 = x1 - ( y1 - y ) * ( x1 - x2 )/( y1 - y2 );

	return std::vector< Point >{ Point( x0, y ) };
}

std::vector<Core::Geometry::Point> Core::Geometry::GetPointOnLineByAbscissa(const Core::Geometry::Line & line, double x)
{
	if ( line.IsVertical() )
	{
		if ( line.P1().X() == x )
		{
			throw std::logic_error( "ERROR : vertical line abscissa = the argument abscissa.\n" );
		}

		return std::vector< Point >();
	}

	double const x1 = line.P1().X();
	double const y1 = line.P1().Y();
	double const x2 = line.P2().X();
	double const y2 = line.P2().Y();

	double const y0 = y1 - ( x1 - x ) * ( y1 - y2 )/( x1 - x2 );

	return std::vector< Point >{ Point( x, y0 ) };
}

bool Core::Geometry::operator<(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
{
	if ( p1.X() == p2.X() )
	{
		return p1.Y() < p2.Y();
	}

	return p1.X() < p2.X();
}

bool Core::Geometry::operator>(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
{
	if ( p1.X() == p2.X() )
	{
		return p1.Y() > p2.Y();
	}

	return p1.X() > p2.X();
}

bool Core::Geometry::operator<=(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
{
	return !( p1 > p2 );
}

bool Core::Geometry::operator>=(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
{
	return !( p1 < p2 );
}

Core::Geometry::Rect::Rect(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
	: m_center( GetMidpoint( p1, p2 ) ),
	  m_width( std::abs( p1.X() - p2.X() ) ),
	  m_height( std::abs( p1.Y() - p2.Y() ) )
{
	if ( p1.X() == p2.X() || p1.Y() == p2.Y() )
	{
		throw std::invalid_argument( "ERROR : \n." );
	}
}

std::vector<Core::Geometry::Point> Core::Geometry::GetIntersectionPoints(const Core::Geometry::Line & line, const Core::Geometry::LineSegment & ls)
{
	if ( !LineSeparatesPoints( line, ls.P1(), ls.P2() ) )
	{
		return std::vector< Point >();
	}

	return GetIntersectionPoints( line, Line( ls ), true );
}

std::vector<Core::Geometry::Point> Core::Geometry::GetIntersectionPoints(const Core::Geometry::Line & line1, const Core::Geometry::Line & line2, bool forced)
{
	if ( !forced )
	{
		throw std::invalid_argument( "ERROR : \n" );
	}

	double const x1 = line1.P1().X();
	double const y1 = line1.P1().Y();
	double const x2 = line1.P2().X();
	double const y2 = line1.P2().Y();

	double const x3 = line2.P1().X();
	double const y3 = line2.P1().Y();
	double const x4 = line2.P2().X();
	double const y4 = line2.P2().Y();

	if ( x1 == x2 )
	{
		double const y = y4 - ( ( y4 - y3 ) / ( x4 - x3 ) ) * ( x4 - x1 );

		return std::vector< Point > { Point( x1, y ) };
	}

	if ( x3 == x4 )
	{
		double const y = y2 - ( ( y2 - y1 ) / ( x2 - x1 ) ) * ( x2 - x3 );

		return std::vector< Point > { Point( x3, y ) };
	}

	double const x = ( ( x1 * y2 - x2 * y1 ) / ( x2 - x1 ) - ( x3 * y4 - x4 * y3 ) / ( x4 - x3 ) ) /
					 ( ( y2 - y1 ) / ( x2 - x1 ) - ( y4 - y3 ) / ( x4 - x3 ) );

	double const y = x * ( y2 - y1 ) / ( x2 - x1 ) - ( x1 * y2 - x2 * y1 ) / ( x2 - x1 );

	return std::vector< Point > { Point( x, y ) };
}

std::vector<Core::Geometry::Point> Core::Geometry::GetIntersectionPoints(const Core::Geometry::Circle & c1, const Core::Geometry::Circle & c2)
{
	if ( c1 == c2 )
	{
		throw std::invalid_argument( "ERROR: intersection of two identic circles\n" );
	}

	Point const center1 = c1.GetCenter();
	Point const center2 = c2.GetCenter();
	double const r1 = c1.GetRadius();
	double const r2 = c2.GetRadius();
	double const d = GetDistance( center1, center2 );

	if ( d < std::abs( r1 - r2 ) || d > r1 + r2 )
	{
		return std::vector< Point >();
	}
	else if ( r1 + r2 == d )
	{
		return std::vector< Point >{ GetPointOnLineSegment( center1, center2, r1 / r2 ) };
	}
	else if ( std::abs( r1 - r2 ) == d )
	{
		if ( r1 > r2 )
		{
			return std::vector< Point >{ GetPointOnLine( center1, center2, -r1 / r2 ) };
		}
		else if ( r2 > r1 )
		{
			return std::vector< Point >{ GetPointOnLine( center2, center1, -r2 / r1 ) };
		}

		throw std::logic_error( "ERROR: intersection of two identic circles\n" );
	}

	double const d1 = ( ( r1 * r1 - r2 * r2 ) / d + d ) / 2;
	Point const p0 = GetPointOnLine( center1, center2, d1 / ( d - d1 ) );
	Vector const n = GetNormal( Vector( center1, center2 ) );
	double const h1 = sqrt( r1 * r1 - d1 * d1 );

	return std::vector< Point >{ p0 + n * h1, p0 - n * h1 };
}

double Core::Geometry::GetDistance(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2)
{
	double const dx = p1.X() - p2.X();
	double const dy = p1.Y() - p2.Y();

	return std::sqrt( dx * dx + dy * dy );
}

Core::Geometry::Vector Core::Geometry::GetNormal(const Core::Geometry::Vector & v)
{
	double const norm = v.GetNorm();

	return Vector( -v.Y() / norm, v.X() / norm );
}

double Core::Geometry::Vector::GetNorm() const
{
	return std::sqrt( m_x * m_x + m_y * m_y );
}

Core::Geometry::Vector Core::Geometry::operator*(const Core::Geometry::Vector & v, double d)
{
	return Vector( d * v.X(), d * v.Y() );
}

Core::Geometry::Vector Core::Geometry::operator*(double d, const Core::Geometry::Vector & v)
{
	return v * d;
}

Core::Geometry::Point Core::Geometry::GetPointOnLine(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2, double ratio)
{
	if ( ratio == -1 )
	{
		throw std::invalid_argument( "ERROR : ratio == -1.\n" );
	}

	double const x1 = p1.X();
	double const y1 = p1.Y();
	double const x2 = p2.X();
	double const y2 = p2.Y();

	double const x = ( x1 + x2 * ratio ) / ( ratio + 1 );
	double const y = ( y1 + y2 * ratio ) / ( ratio + 1 );

	return Point( x, y );
}

bool Core::Geometry::Circle::operator==(const Core::Geometry::Circle & rhs) const
{
	return m_center == rhs.m_center && m_radius == rhs.m_radius;
}

double Core::Geometry::GetDistance(const Core::Geometry::Point & p, const Core::Geometry::Line & line)
{
	Point const p1 = line.P1();
	Point const p2 = line.P2();

	double const angle = GetAngleBetweenVectors( Vector( p1, p ), Vector( p1, p2 ) );

	return GetDistance( p1, p ) * std::sin( angle );
}

double Core::Geometry::GetAngleBetweenVectors(const Core::Geometry::Vector & v1, const Core::Geometry::Vector & v2)
{
	double const scalarProduct = v1.X() * v2.X() + v1.Y() * v2.Y();

	return std::acos( scalarProduct / ( v1.GetNorm() * v2.GetNorm() ) );
}

Core::Geometry::Point Core::Geometry::GetCircumcircleCenter(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2, const Core::Geometry::Point & p3)
{
	// throw

	double const x1 = p1.X();
	double const y1 = p1.Y();
	double const x2 = p2.X();
	double const y2 = p2.Y();
	double const x3 = p3.X();
	double const y3 = p3.Y();

	double const expr12 = ( x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2 ) / 2;
	double const expr13 = ( x1 * x1 + y1 * y1 - x3 * x3 - y3 * y3 ) / 2;

	double const x = ( expr12 * ( y1 - y3 ) - expr13 * ( y1 - y2 ) ) / ( ( x1 - x2 ) * ( y1 - y3 ) - ( x1 - x3 ) * ( y1 - y2 ) );
	double const y = ( expr12 * ( x1 - x3 ) - expr13 * ( x1 - x2 ) ) / ( ( y1 - y2 ) * ( x1 - x3 ) - ( y1 - y3 ) * ( x1 - x2 ) );

	return Point( x, y );
}

double Core::Geometry::GetCircumcircleRadius(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2, const Core::Geometry::Point & p3)
{
	// throw

	double const x1 = p1.X();
	double const y1 = p1.Y();
	double const x2 = p2.X();
	double const y2 = p2.Y();
	double const x3 = p3.X();
	double const y3 = p3.Y();

	double const a = sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
	double const b = sqrt( ( x2 - x3 ) * ( x2 - x3 ) + ( y2 - y3 ) * ( y2 - y3 ) );
	double const c = sqrt( ( x3 - x1 ) * ( x3 - x1 ) + ( y3 - y1 ) * ( y3 - y1 ) );
	double const p = ( a + b + c ) / 2;
	double const s = sqrt( p * ( p - a ) * ( p - b ) * ( p - c ) );

	return ( a * b * c / ( 4 * s ) );
}

Core::Geometry::Circle Core::Geometry::GetCircumcircle(const Core::Geometry::Point & p1, const Core::Geometry::Point & p2, const Core::Geometry::Point & p3)
{
	Point const center = GetCircumcircleCenter( p1, p2, p3 );
	double const r = GetCircumcircleRadius( p1, p2, p3 );

	return Circle( center, r );
}
