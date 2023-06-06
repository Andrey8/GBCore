#pragma once

#include "ConstructionContainer.h"
#include "Geometry.h"

#include <list>
#include <map>



namespace Core
{
    class Printable
    {
    public:
        virtual std::ostream & operator<<( std::ostream & os ) const = 0;
    };

    std::ostream & operator<<( std::ostream & os, Printable const & );

    using Point = Geometry::Point;

    std::ostream & operator<<( std::ostream & os, Point const & );
    std::string ToString( Point const & );

    // enum class GCItemType
    // {
    // 	Point, LineSegment, Circle
    // };

    class GCPoint
    {
    public:
        GCPoint( Point const & p )
            : m_point( p )
        {}
        GCPoint( double x, double y )
            : m_point( Point( x, y ) )
        {}

        Point const & GetPoint() const { return m_point; }
    private:
        Point const m_point;
    };

    class GCFigure : public Printable
    {
    public:
        virtual std::list< GCPoint const * > GetBasePoints() const = 0;

        // template < typename T >
        // GCFigure const * CreateCopy() const
        // {
        // 	return new T( *this );
        // }
    };

    class GCLineSegment : public GCFigure
    {
    public:
        GCLineSegment( GCPoint const * p1, GCPoint const * p2 )
            : m_p1( p1 ), m_p2( p2 )
        {}

        std::list< GCPoint const * > GetBasePoints() const override;

        GCPoint const * P1() const { return m_p1; }
        GCPoint const * P2() const { return m_p2; }
		Geometry::LineSegment GetLS() const { return Geometry::LineSegment( m_p1->GetPoint(), m_p2->GetPoint() ); }

		// Test
		std::ostream & operator<<( std::ostream & os ) const override;
    private:
        GCPoint const * m_p1;
        GCPoint const * m_p2;
    };

	class GCLine : public GCFigure
	{
	public:
		GCLine( GCPoint const * p1, GCPoint const * p2 )
			: m_p1( p1 ), m_p2( p2 )
		{}

		std::list< GCPoint const * > GetBasePoints() const override;

		GCPoint const * P1() const { return m_p1; }
		GCPoint const * P2() const { return m_p2; }
		Geometry::Line GetLine() const { return Geometry::Line( m_p1->GetPoint(), m_p2->GetPoint() ); }

		// Test
		std::ostream & operator<<( std::ostream & os ) const override;
	private:
		GCPoint const * m_p1;
		GCPoint const * m_p2;
	};

	 class GCCircle : public GCFigure
	 {
	 public:
		GCCircle( GCPoint const * center, double radius )
			: m_center( center ), m_r( radius )
		{}

		virtual std::list<const GCPoint *> GetBasePoints() const override { return std::list< GCPoint const * >{ m_center }; }

		// Test
		std::ostream & operator<<( std::ostream & os ) const override;
	 private:
		GCPoint const * m_center;
		double const m_r;		
	 };



    using GCContainer = ConstructionContainer< GCPoint, GCFigure >;



    class GeometryProperty
    {
    public:
        virtual void ModifyGCContainer( GCContainer &, GCPoint const * ) const {}
        virtual ~GeometryProperty() {}

        // Test
        virtual std::string GetInfo() const = 0;
    };

	class FigureProperty
    {
    public:
		virtual void ModifyGCContainer( GCContainer & ) const;

		//std::list< GCItem > GetBaseItems() const;

		// Test
		virtual std::string GetInfo();
    };

	class ParallelLineThroughPointProperty : public FigureProperty
	{
	public:
		ParallelLineThroughPointProperty( GCPoint * p, GCLine * line );

	};

    // template < typename BaseType >
    // class CopyCreator
    // {
    // public:
    // 	BaseType const * CreateCopy() const = 0;
    // };

    class PointProperty : public GeometryProperty
    {
    public:
        //virtual PointProperty const * CreateCopy() const = 0;
        virtual Point CalculateAndGetPoint() const = 0;

        virtual ~PointProperty() {}
    };

    class MidpointProperty : public PointProperty
    {
    public:
        MidpointProperty( GCPoint const * p1, GCPoint const * p2 )
            : m_p1( p1 ), m_p2( p2 )
        {}

        //PointProperty const * CreateCopy() const override { return new MidpointProperty( *this ); }
        Point CalculateAndGetPoint() const override { return Geometry::GetMidpoint( m_p1->GetPoint(), m_p2->GetPoint() ); }
        void ModifyGCContainer( GCContainer &, GCPoint const * ) const override;

        // Test
        std::string GetInfo() const override;

    private:
        GCPoint const * m_p1;
        GCPoint const * m_p2;
    };

    class PerpendicularBaseProperty : public PointProperty
    {
    public:
        PerpendicularBaseProperty( GCPoint const * source, GCPoint const * p1, GCPoint const * p2 )
            : m_source( source ), m_p1( p1 ), m_p2( p2 )
        {}

        //PointProperty const * CreateCopy() const override { return new PerpendicularBaseProperty( *this ); }
        Point CalculateAndGetPoint() const override { return Geometry::GetPerpendicularBase( m_source->GetPoint(), m_p1->GetPoint(), m_p2->GetPoint() ); }
        void ModifyGCContainer( GCContainer &, GCPoint const * ) const override;

        // Test
        std::string GetInfo() const override;

    private:
        GCPoint const * m_source;
        GCPoint const * m_p1;
        GCPoint const * m_p2;
    };

    class LineSegmentPointProperty : public PointProperty
    {
    public:
        LineSegmentPointProperty( GCPoint const * p1, GCPoint const * p2, double distancesRatio )
            : m_p1( p1 ), m_p2( p2 ), m_distancesRatio( distancesRatio )
        {}

        //PointProperty const * CreateCopy() const override { return new LineSegmentPointProperty( *this ); }
        Point CalculateAndGetPoint() const override { return Geometry::GetPointOnLineSegment( m_p1->GetPoint(), m_p2->GetPoint(), m_distancesRatio ); }
        void ModifyGCContainer( GCContainer &, GCPoint const * ) const override;

        // Test
        std::string GetInfo() const override;

    private:
        GCPoint const * m_p1;
        GCPoint const * m_p2;
        double const m_distancesRatio;
    };

	class TwoLSIntersectionPorperty : public PointProperty
	{
	public:
		TwoLSIntersectionPorperty( GCLineSegment * ls1, GCLineSegment * ls2 )
			: m_ls1( ls1 ), m_ls2( ls2 )
		{}

		virtual void ModifyGCContainer(GCContainer &, const GCPoint *) const override;
		virtual Point CalculateAndGetPoint() const override;

		// Test
		virtual std::string GetInfo() const override;
	private:
		GCLineSegment * m_ls1;
		GCLineSegment * m_ls2;
	};

    // class IntersectionPointProperty : public PointProperty
    // {
    // public:

    // };

    class IntersectionData
    {
    public:
        std::list< GCPoint const * > const & GetPoints() const;

    };





	// Used by GC
	class AbstractFigureCreator
	{
	public:
		virtual GCFigure const * Create() const = 0;

		virtual std::vector< GCPoint const * > GetBasePoints() const { return std::vector< GCPoint const * >(); }
		virtual std::vector< GCFigure const * > GetBaseFigures() const { return std::vector< GCFigure const * >(); }

		// Test
		virtual std::string GetFigureInfo() const = 0;
	};

	class ParallelLineCreator : public AbstractFigureCreator
	{
	public:
		ParallelLineCreator( GCPoint const * p, GCLine const * line )
			: m_p( p ), m_line( line )
		{}

		virtual GCFigure const * Create() const override
		{
			Geometry::Vector const v( m_line->P1()->GetPoint(), m_line->P2()->GetPoint() );
			GCPoint * p3 = new GCPoint( m_p->GetPoint() + v );

			return new GCLine( m_p, p3 );
		}

		virtual std::vector< GCPoint const * > GetBasePoints() const override { return std::vector< GCPoint const * >{ m_p }; }
		virtual std::vector< GCFigure const * > GetBaseFigures() const override { return std::vector< GCFigure const * >{ m_line }; }

		// Test
		virtual std::string GetFigureInfo() const;
	private:
		GCPoint const * m_p;
		GCLine const * m_line;
	};

	class CircumcircleCreator : public AbstractFigureCreator
	{
	public:
		CircumcircleCreator( GCPoint const * p1, GCPoint const * p2, GCPoint const * p3 )
			: m_p1( p1 ), m_p2( p2 ), m_p3( p3 )
		{}

		virtual const GCFigure *Create() const override;

		virtual std::vector<const GCPoint *> GetBasePoints() const override { return std::vector< GCPoint const * >{ m_p1, m_p2, m_p3 }; }

		// Test
		virtual std::string GetFigureInfo() const;
	private:
		GCPoint const * m_p1;
		GCPoint const * m_p2;
		GCPoint const * m_p3;
	};

	class PerpendicularBisectorCreator : public AbstractFigureCreator
	{
	private:
		enum class Mode { FromTwoPoints, FromLineSegment };
	public:
		PerpendicularBisectorCreator( GCLineSegment const * ls );
		PerpendicularBisectorCreator( GCPoint const * p1, GCPoint const * p2 );

		virtual const GCFigure *Create() const override;
		virtual std::vector<const GCPoint *> GetBasePoints() const override;
		virtual std::vector<const GCFigure *> GetBaseFigures() const override;
		virtual std::string GetFigureInfo() const override;

	private:
		GCLineSegment const * m_ls;
		GCPoint const * m_p1;
		GCPoint const * m_p2;
		Mode const m_mode;
	};



    class GeometryConstruction
	{
    public:
        void Add( GCPoint const * );
        void Add( GCFigure const * );
        GCPoint const * CreatePointByProperty( PointProperty const * );
		//GCFigure const * CreateFigureByProperty( FigureProperty const * );
		GCFigure const * CreateFigureByCreator( AbstractFigureCreator const * fcreator );
        //IntersectionData CreateIntersectionPoints( GCFigure const *, GCFigure const * );

        // void Attach( GCPoint const *, GCFigure const * );

        void Remove( GCPoint const * );
        void Remove( GCFigure const * );

        friend std::ostream & operator<<( std::ostream & os, GeometryConstruction const & );

        //~GeometryConstruction() {}
    private:
        ConstructionContainer< GCPoint, GCFigure > m_container;
        std::list< GCPoint const * > m_points;
        std::map< GCPoint const *, PointProperty const * > m_pointsToProperties;
		std::map< GCFigure const *, AbstractFigureCreator const * > m_figuresToBuilders;
		std::set< GCPoint const * > m_hiddenPoints;
    };
	
	std::ostream & operator<<( std::ostream & os, GeometryConstruction const & );




	GCFigure const * CreateParallelLine( GCPoint const * p, GCLine const * line );
	GCFigure const * CreatePerpendicularLine( GCPoint const * p, GCLine const * line );
	GCFigure const * CreatePerpendicularBisector( GCPoint const * p1, GCPoint const * p2 );
	GCFigure const * CreateCircumcircle( GCPoint const * p1, GCPoint const * p2, GCPoint const * p3 );

}
