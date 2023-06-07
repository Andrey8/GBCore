#include "../../src/Geometry.h"
#include "../Utils.h"

#include <gtest/gtest.h>

using Core::Geometry::Point;
using Core::Geometry::Rect;



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
	TEST( RectTest1, General1 )
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
			
			EXPECT_EQ( rect.GetTopY(), std::max( p1.Y(), p2.Y() ) );
			EXPECT_EQ( rect.GetBottomY(), std::min( p1.Y(), p2.Y() ) );
			EXPECT_EQ( rect.GetLeftX(), std::min( p1.X(), p2.X() ) );
			EXPECT_EQ( rect.GetRightX(), std::max( p1.X(), p2.X() ) );
		}
	};
	
	TEST_F( RectTest, RectBoundaries )
	{
		Point const p1 = CoreTests::Utils::GetRandomPointInRect( -10, 10, -10, 10 );
		Point const p2 = CoreTests::Utils::GetRandomPointInRect( -10, 10, -10, 10 );
		
		for ( int i = 0; i < 10; ++i )
		{
			TestRectBoundaries( p1, p2 );
		}		
	}
}
