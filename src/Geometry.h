#pragma once

#include <vector>



namespace Core
{
    namespace Geometry
    {
        class Point
        {
        public:
            Point( double x, double y )
                : m_x( x ), m_y( y )
            {}

            double X() const { return m_x; }
            double Y() const { return m_y; }

            bool operator==( Point const & other ) const
            {
                return ( m_x == other.m_x && m_y == other.m_y );
            }

            bool operator!=( Point const & other ) const
            {
                return !( *this == other );
            }

        private:
            double m_x;
            double m_y;
        };

		bool operator<( Point const & p1, Point const & p2 );
		bool operator>( Point const & p1, Point const & p2 );
		bool operator<=( Point const & p1, Point const & p2 );
		bool operator>=( Point const & p1, Point const & p2 );

		class Vector
		{
		public:
			Vector( Point const & begin, Point const & end )
				: m_x( end.X() - begin.X() ), m_y( end.Y() - begin.Y() )
			{}

			Vector( double x, double y )
				: m_x( x ), m_y( y )
			{}

			Vector const operator-() const { return Vector( -m_x, -m_y ); }

			double X() const { return m_x; }
			double Y() const { return m_y; }
			double GetNorm() const;
		private:
			double const m_x;
			double const m_y;
		};

		Point operator+( Point const &, Vector const & );
		Point operator-( Point const &, Vector const & );
		Vector operator*( Vector const & v, double d );
		Vector operator*( double d, Vector const & v );

		class Rect
		{
		public:
			Rect( Point const & p1, Point const & p2 );

			double GetTopY() const { return m_center.Y() + m_height / 2; }
			double GetBottomY() const { return m_center.Y() - m_height / 2; }
			double GetLeftX() const { return m_center.X() - m_width / 2; }
			double GetRightX() const { return m_center.X() + m_width / 2; }

		private:
			Point const m_center;
			double const m_width;
			double const m_height;
		};

        class LineSegment
        {
        public:
			LineSegment( Point const & p1, Point const & p2 )
				: m_p1( p1 ), m_p2( p2 )
			{}

			Point P1() const { return m_p1; }
			Point P2() const { return m_p2; }

		private:
			Point m_p1;
			Point m_p2;
        };

		class Line
		{
		public:
			Line( Point const & p1, Point const & p2 )
				: m_p1( p1 ), m_p2( p2 )
			{}

			Line( LineSegment const & ls )
				: m_p1( ls.P1() ), m_p2( ls.P2() )
			{}

			Point P1() const { return m_p1; }
			Point P2() const { return m_p2; }

			bool IsHorizontal() const { return m_p1.Y() == m_p2.Y(); }
			bool IsVertical() const { return m_p1.X() == m_p2.X(); }

		private:
			Point m_p1;
			Point m_p2;
		};

        class Circle
        {
        public:
			Circle( Point const & center, double radius )
				: m_center( center ), m_radius( radius )
			{}

			Point GetCenter() const { return m_center; }
			double GetRadius() const { return m_radius; }

			bool operator==( Circle const & rhs ) const;
			bool operator!=( Circle const & rhs ) const { return !( *this == rhs ); }

		private:
			Point m_center;
			double m_radius;
		};

		template < typename T >
		class Triple
		{
		public:
			Triple( T const & v1, T const & v2, T const & v3 )
				: m_v1( v1 ),
				  m_v2( v2 ),
				  m_v3( v3 )
			{}

			T const & GetValue1() const { return m_v1; }
			T const & GetValue2() const { return m_v2; }
			T const & GetValue3() const { return m_v3; }

		private:
			T const m_v1;
			T const m_v2;
			T const m_v3;
		};



		static double const PI = 3.14159265;

		double GetAbsoluteAngle( Vector const & );
		Vector GetNormal( Vector const & v );
		double GetAngleBetweenVectors( Vector const & v1, Vector const & v2 );
		bool LineSeparatesPoints( Line const &, Point const &, Point const & );
		bool VectorIsBetweenTwoOthers( Vector const & v, Vector const & v1, Vector const & v2 );
		Triple< double > GetGeneralCoefficients( Line const & );
		bool PointIsBetweenTwoOthers( Point const & p, Point const & p1, Point const & p2 );

        Point GetMidpoint( Point const &, Point const & );
        Point GetPerpendicularBase( Point const & source, Point const & p1, Point const & p2 );
		Point GetPointOnLineSegment( Point const & p1, Point const & p2, double ratioOfDistancesToEnds );
		Point GetPointOnLine( Point const & p1, Point const & p2, double ratio );
		double GetDistance( Point const & p1, Point const & p2 );
		double GetDistance( Point const & p, Line const & line );
		Point GetCircumcircleCenter( Point const & p1, Point const & p2, Point const & p3 );
		double GetCircumcircleRadius( Point const & p1, Point const & p2, Point const & p3 );
		Circle GetCircumcircle( Point const & p1, Point const & p2, Point const & p3 );

		std::vector< Point > GetPointOnLineByOrdinate( Line const & line, double y );
		std::vector< Point > GetPointOnLineByAbscissa( Line const & line, double x );
		std::vector< LineSegment > GetIntersectionLineSegment( Line const & line, Rect const & rect );
		std::vector< Point > GetIntersectionPoints( const Line &, const Line &, bool forced );
		std::vector< Point > GetIntersectionPoints( LineSegment const &, LineSegment const & );
		std::vector< Point > GetIntersectionPoints( Line const &, Line const &, Rect const & boundingBox );
		std::vector< Point > GetIntersectionPoints( Line const & line, LineSegment const & ls );
		std::vector< Point > GetIntersectionPoints( Circle const &, Line const & );
		std::vector< Point > GetIntersectionPoints( Circle const &, LineSegment const & );
		std::vector< Point > GetIntersectionPoints( Circle const & c1, Circle const & c2 );

    }


}
