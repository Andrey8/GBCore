#include "../../src/Geometry.h"
#include "../Utils.h"

#include <gtest/gtest.h>
#include <cmath>

using Core::Geometry::Point;
using Core::Geometry::Rect;
using Core::Geometry::PI;
using Core::Geometry::Vector;



namespace Tests
{
	TEST( GetRandomNumberInRangeTest, IsInRange )
	{
		double const x1 = 14.85647393;
		double const x2 = -5.75849305;
				
		for ( int i = 0; i < 10; ++i )
		{
			double const randomNumber = CoreTests::Utils::GetRandomNumberInRange( x1, x2 );
			
			EXPECT_TRUE( randomNumber >= std::min( x1, x2 ) );
			EXPECT_TRUE( randomNumber <= std::max( x1, x2 ) );
		}
	}
}

namespace Tests
{
	static double const tolerance = 1e-6;
	
	
	
	TEST( SpecificRectTest_1, TestBoundaries )
	{
		Point const p1 = { 2, 3 };
		Point const p2 = { 6, 11 };
		
		Rect const rect = { p1, p2 };
		
		EXPECT_EQ( rect.GetTopY(), 11 );
		EXPECT_EQ( rect.GetBottomY(), 3 );
		EXPECT_EQ( rect.GetLeftX(), 2 );
		EXPECT_EQ( rect.GetRightX(), 6 );
	}
	
	class RectTest : public ::testing::Test
	{
	protected:
		void TestRectBoundaries( Point const & p1, Point const & p2 )
		{
			Rect const rect = { p1, p2 };
			
			EXPECT_TRUE( std::abs( rect.GetTopY() - std::max( p1.Y(), p2.Y() ) ) < tolerance );
			EXPECT_TRUE( std::abs( rect.GetBottomY() - std::min( p1.Y(), p2.Y() ) ) < tolerance );
			EXPECT_TRUE( std::abs( rect.GetLeftX() - std::min( p1.X(), p2.X() ) ) < tolerance );
			EXPECT_TRUE( std::abs( rect.GetRightX() - std::max( p1.X(), p2.X() ) ) < tolerance );
		}
	};
	
	TEST_F( RectTest, RectBoundaries )
	{		
		for ( int i = 0; i < 10; ++i )
		{
			Point const p1 = CoreTests::Utils::GetRandomPointInRect( -10, 10, -10, 10 );
			Point const p2 = CoreTests::Utils::GetRandomPointInRect( -10, 10, -10, 10 );
			
			TestRectBoundaries( p1, p2 );
		}		
	}
}

namespace Tests
{
	TEST( GetAbsoluteAngleOfVector, Test1 )
	{
		double const r = 5;
		int const n = CoreTests::Utils::GetRandomIntegerInRange( 10, 20 );
		for ( int i = 0; i < n; ++i )
		{
			double const angle = i * ( 2 * PI ) / n;
			Vector const v = { r * std::cos( angle ), r * std::sin( angle ) };
			
			EXPECT_TRUE( std::abs( Core::Geometry::GetAbsoluteAngle( v ) - angle ) < tolerance );
		}
	}
}
