#pragma once

#include "../src/Geometry.h"

using Core::Geometry::Point;



namespace Tests
{
	namespace CoreTests
	{
		namespace Utils
		{
			double GetRandomNumberInRange( double a, double b );
			int GetRandomIntegerInRange( int a, int b );
			
			Point GetRandomPointInRect( double leftBoundX, double rightBoundX, double bottomBoundY, double topBoundY );
		}		
	}
}
