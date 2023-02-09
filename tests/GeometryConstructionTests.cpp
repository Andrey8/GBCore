#include "GeometryConstructionTests.h"
#include "../src/GeometryConstruction.h"

#include <iostream>

using Point = Core::Geometry::Point;
using Core::GCPoint;
using Core::GCLineSegment;



void Tests::CoreTests::TestGeometryConstruction_PappusConstruction()
{
	GCPoint a( 1, 4 );
	GCPoint b( 7, 6 );
	GCLineSegment ab( &a, &b );
	GCPoint a1( 1, 1 );
	GCPoint b1( 10, 2 );	
	GCLineSegment a1b1( &a1, &b1 );

	Core::GeometryConstruction gc;

	gc.Add( &ab );
	gc.Add( &a1b1 );

	auto prop1 = new Core::LineSegmentPointProperty( &a, &b, 1.0 / 2 );
	GCPoint const * c = gc.CreatePointByProperty( prop1 );

	auto prop2 = new Core::LineSegmentPointProperty( &a1, &b1, 3.0 / 2 );
	GCPoint const * c1 = gc.CreatePointByProperty( prop2 );

	GCLineSegment ac1( &a, c1 );
	GCLineSegment ca1( c, &a1 );
	gc.Add( &ac1 );
	gc.Add( &ca1 );
	auto intersectionProp1 = new Core::TwoLSIntersectionPorperty( &ac1, &ca1 );
	GCPoint const * i1 = gc.CreatePointByProperty( intersectionProp1 );

	GCLineSegment cb1( c, &b1 );
	GCLineSegment bc1( &b, c1 );
	gc.Add( &cb1 );
	gc.Add( &bc1 );
	auto intersectionProp2 = new Core::TwoLSIntersectionPorperty( &cb1, &bc1 );
	GCPoint const * i2 = gc.CreatePointByProperty( intersectionProp2 );

	GCLineSegment ab1( &a, &b1 );
	GCLineSegment ba1( &b, &a1 );
	gc.Add( &ab1 );
	gc.Add( &ba1 );
	auto intersectionProp3 = new Core::TwoLSIntersectionPorperty( &ab1, &ba1 );
	GCPoint const * i3 = gc.CreatePointByProperty( intersectionProp3 );

	GCLineSegment i1i2( i1, i2 );
	gc.Add( &i1i2 );

	std::cout << gc;
	std::cout << "Distance : " << Core::Geometry::GetDistance(
					 i3->GetPoint(),
					 Core::Geometry::Line( i1->GetPoint(), i2->GetPoint() ) );

	//gc.MovePointTo( &a, Point( 0, 3 ) );

	delete prop1;
	delete prop2;
}

void Tests::CoreTests::TestGeometryConstruction_Construction1()
{
	GCPoint const p1( 3, 4 );
	GCPoint const p2( 5, 2 );
	GCPoint const p3( 6, 5 );
	GCPoint const p4( 1, 1 );
	
	Core::GCLineSegment p1p2ls( &p1, &p2 );
	Core::GCLineSegment p1p3ls( &p1, &p3 );

	Core::GeometryConstruction gc;

	gc.Add( &p1p2ls );
	gc.Add( &p1p3ls );
	gc.Add( &p4 );

	Core::PointProperty * prop1 = new Core::MidpointProperty( &p1, &p2 );
	gc.CreatePointByProperty( prop1 );

	Core::PointProperty * prop2 = new Core::MidpointProperty( &p2, &p3 );
	gc.CreatePointByProperty( prop2 );

	Core::PointProperty * prop3 =  new Core::PerpendicularBaseProperty( &p2, &p1, &p3 );
	GCPoint const * h = gc.CreatePointByProperty( prop3 );

	Core::GCLineSegment p2hls( &p2, h );
	gc.Add( &p2hls );

	std::cout << gc;

	delete prop1;
	delete prop2;
	delete prop3;
}

void Tests::CoreTests::TestGeometryConstruction_Construction2()
{
	GCPoint a( 4, 3 );
	GCPoint b( 7, 7 );
	GCPoint c( 9, 3 );

	GCLineSegment ab( &a, &b );
	GCLineSegment bc( &b, &c );
	GCLineSegment ca( &c, &a );

	Core::GeometryConstruction gc;

	gc.Add( &ab );
	gc.Add( &bc );
	gc.Add( &ca );

	Core::PointProperty * prop1 = new Core::PerpendicularBaseProperty( &b, &a, &c );
	GCPoint const * b1 = gc.CreatePointByProperty( prop1 );
	GCLineSegment bb1( &b, b1 );
	gc.Add( &bb1 );

	Core::PointProperty * prop2 = new Core::PerpendicularBaseProperty( b1, &a, &b );
	GCPoint const * d = gc.CreatePointByProperty( prop2 );
	GCLineSegment b1d( b1, d );
	gc.Add( &b1d );

	Core::PointProperty * prop3 = new Core::PerpendicularBaseProperty( d, &b, &c );
	GCPoint const * d1 = gc.CreatePointByProperty( prop3 );
	GCLineSegment dd1( d, d1 );
	gc.Add( &dd1 );

	std::cout << gc;

	delete prop1;
	delete prop2;
	delete prop3;
}



// std::ostream & Tests::CoreTests::operator<<( std::ostream & os, Core::GCFigure const & gcfigure )
// {
// 	return gcfigure.operator<<( os );
// }

// std::ostream & Tests::CoreTests::operator<<( std::ostream & os, Core::Geometry::Point const & point )
// {
// 	os << "Point( " << point.X() << ", " << point.Y() << " )";

// 	return os;
// }

// std::ostream & Tests::CoreTests::operator<<( std::ostream & os, Core::GCLineSegment const & gcls )
// {
// 	os << "GCLineSegment[ " << *( gcls.P1() ) << ", " << *( gcls.P2() ) << " ]";

// 	return os;
// }














