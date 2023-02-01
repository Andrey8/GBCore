#include "Geometry.h"

#include <stdexcept>
#include <cmath>



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
    if ( ratio == -1 )
    {
        throw std::invalid_argument( "Ratio can't equal -1!\n" );
    }

    double const x1 = p1.X();
    double const y1 = p1.Y();
    double const x2 = p2.X();
    double const y2 = p2.Y();

    double const x0 = ( x1 + x2 * ratio ) / ( ratio + 1 );
    double const y0 = ( y1 + y2 * ratio ) / ( ratio + 1 );

    return Point( x0, y0 );
}

std::vector< Point > Core::Geometry::GetIntersectionPoints( const Core::Geometry::LineSegment & ls1, const Core::Geometry::LineSegment & ls2 )
{
	if ( !LineSeparatesPoints( Line( ls1 ), ls2.P1(), ls2.P2() ) ||
		 !LineSeparatesPoints( Line( ls2 ), ls1.P1(), ls1.P2() ) )
	{
		return std::vector< Point >();
	}

	double const x1 = ls1.P1().X();
	double const y1 = ls1.P1().Y();
	double const x2 = ls1.P2().X();
	double const y2 = ls1.P2().Y();

	double const x3 = ls2.P1().X();
	double const y3 = ls2.P1().Y();
	double const x4 = ls2.P2().X();
	double const y4 = ls2.P2().Y();

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

	}
	case 0 :
	{

	}
	default :
	{

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
