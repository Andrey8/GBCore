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

std::ostream & Core::operator<<( std::ostream & os, Core::GeometryConstruction const & gc )
{
	os << "[GeometryConstruction] :\n";

	os << '\t' << "Figures :\n";
	for ( auto gcfigure : gc.m_container.GetAllBodies() )
	{
		os << "\t\t" << *gcfigure << '\n';

		os << "\t\t" << "- Slave points :\n";
		for ( GCPoint const * p : gc.m_container.GetSlaveUnits( gcfigure ) )
		{
			os << "\t\t\t" << p->GetPoint();

			auto iter = gc.m_pointsToProperties.find( p );
			if ( iter != gc.m_pointsToProperties.end() )
			{
				os << " - " << iter->second->GetInfo();
			}

			os << '\n';
		}

		os << '\n';
	}

	os << '\t' << "Single points :\n";
	for ( GCPoint const * point : gc.m_container.GetIsolatedUnits() )
	{
		os << "\t\t" << point->GetPoint() << '\n';
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
	os << "Point(" << point.X() << ", " << point.Y() << ")";

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











