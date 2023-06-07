#include "Utils.h"

//#include <random>
#include <cstdlib>

using Core::Geometry::Point;



double Tests::CoreTests::Utils::GetRandomNumberInRange( double a, double b )
{	
	int const diff = static_cast< int >( std::abs( a * 1e8 - b * 1e8 ) );
	
	//int const x = static_cast< int >( * ( 10 ^ 8 ) );
	
	return ( std::rand() % diff ) / 1e8 + std::min( a, b );
}

Point Tests::CoreTests::Utils::GetRandomPointInRect(double leftBoundX, double rightBoundX, double bottomBoundY, double topBoundY)
{
	double const x = GetRandomNumberInRange( leftBoundX, rightBoundX );
	double const y = GetRandomNumberInRange( bottomBoundY, topBoundY );
	
	return Point{ x, y };
}
