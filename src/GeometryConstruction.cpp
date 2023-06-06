#include "GeometryConstruction.h"
#include "Utils.h"

#include <typeinfo>
#include <sstream>
#include <iomanip>

using Point = Core::Geometry::Point;



void Core::GeometryConstruction::Add( GCPoint const * gcpoint )
{
	//Point const * p = gcpoint->GetPoint();

	m_container.Add( gcpoint );
	m_points.push_back( gcpoint );

}

void Core::GeometryConstruction::Add( GCFigure const * gcfigure )
{
	// GCFigure const * new_gcfigure = gcfigure->CreateCopy();

	m_container.Add( gcfigure, gcfigure->GetBasePoints() );
	
}

Core::GCPoint const * Core::GeometryConstruction::CreatePointByProperty( PointProperty const * property )
{
	Point const p( property->CalculateAndGetPoint() );
	GCPoint const * gcpoint = new GCPoint( p );
	property->ModifyGCContainer( m_container, gcpoint );

	m_pointsToProperties.insert( std::make_pair( gcpoint, property ) );

	return gcpoint;
}

const Core::GCFigure *Core::GeometryConstruction::CreateFigureByCreator(const Core::AbstractFigureCreator * fcreator)
{
	GCFigure const * figure = fcreator->Create();

	m_container.Add( figure, fcreator->GetBasePoints(), fcreator->GetBaseFigures() );

	m_figuresToBuilders.insert( std::make_pair( figure, fcreator ) );

	// Hide { basePoints / all GC points }
	auto basePoints = fcreator->GetBasePoints();
	auto allPoints = m_container.GetAllUnits();

	auto pointsToHide = Core::Utils::GetSetDifference( basePoints.begin(), basePoints.end(),
													   allPoints.begin(), allPoints.end() );

	m_hiddenPoints.insert( pointsToHide.begin(), pointsToHide.end() );

	return figure;
}

std::ostream & Core::operator<<( std::ostream & os, Core::GeometryConstruction const & gc )
{
	os << "[GeometryConstruction]\n";

	os << "\nFigures :\n";
	for ( auto gcfigure : gc.m_container.GetAllBodies() )
	{
		auto it = gc.m_figuresToBuilders.find( gcfigure );
		if ( it != gc.m_figuresToBuilders.end() )
		{
			os << "\t" << it->second->GetFigureInfo() << "\n\n";

			continue;
		}

		os << "\t" << *gcfigure << '\n';

		os << "\t" << "- Slave points :\n";
		for ( GCPoint const * p : gc.m_container.GetSlaveUnits( gcfigure ) )
		{
			os << "\t\t" << p->GetPoint();

			auto iter = gc.m_pointsToProperties.find( p );
			if ( iter != gc.m_pointsToProperties.end() )
			{
				os << " - " << iter->second->GetInfo();
			}

			os << '\n';
		}

		os << '\n';
	}

	os << "Single points :\n";
	for ( GCPoint const * point : gc.m_container.GetIsolatedUnits() )
	{
		os << "\t" << point->GetPoint() << '\n';
	}
	if ( gc.m_container.GetIsolatedUnits().empty() )
	{
		os << "\tNone\n";
	}
	os << '\n';

	return os;
}



void Core::MidpointProperty::ModifyGCContainer( GCContainer & container, GCPoint const * point ) const
{
	std::set< GCFigure const * > const commonFigures = Utils::GetUniqueIntersection( container.GetAllBodies( m_p1 ), container.GetAllBodies( m_p2 ) );
	for ( GCFigure const * gcfigure : commonFigures )
	{
		if ( typeid( *gcfigure ) == typeid( GCLineSegment ) )
		{
			container.AddUnitToBody( point, gcfigure );

			return;
		}
	}

	container.Add( point );
}

std::string Core::MidpointProperty::GetInfo() const
{
	std::ostringstream oss;

	oss << "Midpoint for " << Core::ToString( m_p1->GetPoint() ) << " and " << Core::ToString( m_p2->GetPoint() );

	return oss.str();
}

void Core::PerpendicularBaseProperty::ModifyGCContainer( GCContainer & container, GCPoint const * point ) const
{
	std::set< GCFigure const * > const commonFigures = Utils::GetUniqueIntersection( container.GetAllBodies( m_p1 ), container.GetAllBodies( m_p2 ) );
	for ( GCFigure const * gcfigure : commonFigures )
	{
		if ( typeid( *gcfigure ) == typeid( GCLineSegment ) )
		{
			container.AddUnitToBody( point, gcfigure );

			return;
		}
	}

	container.Add( point );
}

std::string Core::PerpendicularBaseProperty::GetInfo() const
{
	std::ostringstream oss;

	oss << "Base of perpendicular from " << Core::ToString( m_source->GetPoint() ) << " to line of " << Core::ToString( m_p1->GetPoint() ) << " and " << Core::ToString( m_p2->GetPoint() );

	return oss.str();
}

void Core::LineSegmentPointProperty::ModifyGCContainer( GCContainer & container, GCPoint const * point ) const
{
	std::set< GCFigure const * > const commonFigures = Utils::GetUniqueIntersection( container.GetAllBodies( m_p1 ), container.GetAllBodies( m_p2 ) );
	for ( GCFigure const * gcfigure : commonFigures )
	{
		if ( typeid( *gcfigure ) == typeid( GCLineSegment ) )
		{
			container.AddUnitToBody( point, gcfigure );

			return;
		}
	}

	container.Add( point );
}

std::string Core::LineSegmentPointProperty::GetInfo() const
{
	std::ostringstream oss;

	oss << "Line segment point between " << Core::ToString( m_p1->GetPoint() ) << " and " << Core::ToString( m_p2->GetPoint() );

	return oss.str();
}





std::list< Core::GCPoint const * > Core::GCLineSegment::GetBasePoints() const
{
	std::list< GCPoint const * > result;

	result.push_back( m_p1 );
	result.push_back( m_p2 );

	return result;
}

std::ostream & Core::GCLineSegment::operator<<( std::ostream & os ) const
{
	os << "GCLineSegment[ " << m_p1->GetPoint() << ", " << m_p2->GetPoint() << " ]";

	return os;
}



std::ostream & Core::operator<<( std::ostream & os, Core::Printable const & printable )
{
	return printable.operator<<( os );
}

std::ostream & Core::operator<<( std::ostream & os, Core::Geometry::Point const & point )
{
	os << "P(" << point.X() << ", " << point.Y() << ")";

	return os;
}

std::string Core::ToString( Point const & point )
{
	std::stringstream ss;

	ss << std::fixed << std::setprecision( 1 ) << point.X();
	std::string const x_str = ss.str();

	ss.str( "" );

	ss << point.Y();
	std::string const y_str = ss.str();

	return "Point(" + x_str + ", " + y_str + ")";
}



void Core::TwoLSIntersectionPorperty::ModifyGCContainer(Core::GCContainer & container, const Core::GCPoint * gcpoint ) const
{
	container.AddUnitToBody( gcpoint, m_ls1 );
	container.AddUnitToBody( gcpoint, m_ls2 );
}

std::string Core::TwoLSIntersectionPorperty::GetInfo() const
{
	std::ostringstream oss;

	oss << "Intersection point of " << *m_ls1 << " and " << *m_ls2;

	return oss.str();
}

Core::Point Core::TwoLSIntersectionPorperty::CalculateAndGetPoint() const
{
	auto ipoints = Geometry::GetIntersectionPoints( m_ls1->GetLS(), m_ls2->GetLS() );

	if ( ipoints.empty() )
	{
		throw std::logic_error( "ERROR : line segments don't intersect.\n" );
	}

	return ipoints.front();
}

std::list<const Core::GCPoint *> Core::GCLine::GetBasePoints() const
{
	std::list< GCPoint const * > result;

	result.push_back( m_p1 );
	result.push_back( m_p2 );

	return result;
}

std::ostream &Core::GCLine::operator<<(std::ostream & os) const
{
	os << "GCLine[ " << m_p1->GetPoint() << ", " << m_p2->GetPoint() << " ]";

	return os;
}

const Core::GCFigure *Core::CreateParallelLine(const Core::GCPoint * p, const Core::GCLine * line)
{
	Geometry::Vector const v( line->P1()->GetPoint(), line->P2()->GetPoint() );
	GCPoint * p3 = new GCPoint( p->GetPoint() + v );

	return new GCLine( p, p3 );
}

const Core::GCFigure *Core::CreatePerpendicularLine(const Core::GCPoint * p, const Core::GCLine * line)
{
	Point const perpBase = Geometry::GetPerpendicularBase( p->GetPoint(), line->P1()->GetPoint(), line->P2()->GetPoint() );
	GCPoint * p3 = new GCPoint( perpBase );

	return new GCLine( p, p3 );
}

const Core::GCFigure *Core::CreatePerpendicularBisector(const Core::GCPoint * p1, const Core::GCPoint * p2)
{
	Point const midpoint = Geometry::GetMidpoint( p1->GetPoint(), p2->GetPoint() );
	Geometry::Vector const v( p1->GetPoint(), p2->GetPoint() );

	GCPoint * p3 = new GCPoint( midpoint );
	GCPoint * p4 = new GCPoint( midpoint + Geometry::GetNormal( v ) );

	return new GCLine( p3, p4 );
}

//const Core::GCFigure *Core::CreateCircumcircle(const Core::GCPoint * p1, const Core::GCPoint * p2, const Core::GCPoint * p3)
//{

//}

const Core::GCFigure *Core::CircumcircleCreator::Create() const
{
	Geometry::Point const p1 = m_p1->GetPoint();
	Geometry::Point const p2 = m_p2->GetPoint();
	Geometry::Point const p3 = m_p3->GetPoint();

	GCPoint const * center = new GCPoint( Geometry::GetCircumcircleCenter( p1, p2, p3 ) );
	double const r = Geometry::GetCircumcircleRadius( p1, p2, p3 );

	return new GCCircle( center, r );
}

std::string Core::CircumcircleCreator::GetFigureInfo() const
{
	std::ostringstream oss;

	oss << "Circumcircle of points " << m_p1->GetPoint() << ", " << m_p2->GetPoint() << ", " << m_p3->GetPoint();

	return oss.str();
}

std::ostream &Core::GCCircle::operator<<(std::ostream & os) const
{
	os << "GCCircle[ center : " << m_center->GetPoint() << ", radius : " << m_r << " ]";

	return os;
}

std::string Core::ParallelLineCreator::GetFigureInfo() const
{
	std::ostringstream oss;

	oss << "Parallel line to " << *m_line << " and through " << m_p->GetPoint();

	return oss.str();
}


Core::PerpendicularBisectorCreator::PerpendicularBisectorCreator(const Core::GCLineSegment * ls)
	: m_ls( ls ),
	  m_p1( nullptr ),
	  m_p2( nullptr ),
	  m_mode( Mode::FromLineSegment )
{}

Core::PerpendicularBisectorCreator::PerpendicularBisectorCreator(const Core::GCPoint * p1, const Core::GCPoint * p2)
	: m_ls( nullptr ),
	  m_p1( p1 ),
	  m_p2( p2 ),
	  m_mode( Mode::FromTwoPoints )
{}

const Core::GCFigure *Core::PerpendicularBisectorCreator::Create() const
{
	Point p1( 0, 0 );
	Point p2( 0, 0 );

	switch ( m_mode )
	{
	case Mode::FromLineSegment :
	{
		p1 = m_ls->P1()->GetPoint();
		p2 = m_ls->P2()->GetPoint();

		break;
	}
	case Mode::FromTwoPoints :
	{
		p1 = m_p1->GetPoint();
		p2 = m_p2->GetPoint();

		break;
	}
	default :
	{
		throw std::logic_error( "" );
	}
	}

	Point const midpoint = Geometry::GetMidpoint( p1, p2 );
	Geometry::Vector const normal = Geometry::GetNormal( Geometry::Vector( p1, p2 ) );
	Point const p = midpoint + normal;

	return new GCLine( new GCPoint( midpoint ), new GCPoint( p ) );
}

std::vector<const Core::GCFigure *> Core::PerpendicularBisectorCreator::GetBaseFigures() const
{
	switch ( m_mode )
	{
	case Mode::FromLineSegment :
	{
		return std::vector< GCFigure const * >{ m_ls };
	}
	case Mode::FromTwoPoints :
	{
		return std::vector< GCFigure const * >();
	}
	default :
	{
		throw std::logic_error( "" );
	}
	}
}

std::string Core::PerpendicularBisectorCreator::GetFigureInfo() const
{
	std::ostringstream oss;

	switch ( m_mode )
	{
	case Mode::FromLineSegment :
	{
		oss << "Perpendicular bisector for " << *m_ls;

		break;
	}
	case Mode::FromTwoPoints :
	{
		oss << "Perpendicular bisector between " << m_p1->GetPoint() << " and " << m_p2->GetPoint();

		break;
	}
	default :
	{
		throw std::logic_error( "" );
	}
	}

	return oss.str();
}


std::vector<const Core::GCPoint *> Core::PerpendicularBisectorCreator::GetBasePoints() const
{
	switch ( m_mode )
	{
	case Mode::FromLineSegment :
	{
		return std::vector< GCPoint const * >();
	}
	case Mode::FromTwoPoints :
	{
		return std::vector< GCPoint const * >{ m_p1, m_p2 };
	}
	default :
	{
		throw std::logic_error( "" );
	}
	}
}
