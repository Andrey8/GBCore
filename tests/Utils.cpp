#include "Utils.h"

//#include <random>
#include <cstdlib>

using Core::Geometry::Point;



double Tests::CoreTests::Utils::GetRandomNumberInRange( double a, double b )
{	
	int const diff = static_cast< int >( std::abs( a * 1e8 - b * 1e8 ) );
	
	return ( std::rand() % diff ) / 1e8 + std::min( a, b );
}

Point Tests::CoreTests::Utils::GetRandomPointInRect(double leftBoundX, double rightBoundX, double bottomBoundY, double topBoundY)
{
	double const x = GetRandomNumberInRange( leftBoundX, rightBoundX );
	double const y = GetRandomNumberInRange( bottomBoundY, topBoundY );
	
	return Point{ x, y };
}

int Tests::CoreTests::Utils::GetRandomIntegerInRange(int a, int b)
{
	return std::rand() % std::abs( a - b ) + std::min( a, b );
}
