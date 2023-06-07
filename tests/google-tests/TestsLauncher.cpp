#include "../Utils.h"
#include "../../src/Geometry.h"

#include <iostream>
#include <ctime>

#include <gtest/gtest.h>



namespace Tests
{
	void Test1()
	{
		for ( int i = 0; i < 10; ++i )
		{
			double const x = CoreTests::Utils::GetRandomNumberInRange( 13.4758, 21.0483 );
			
			std::cout << "Random number " << x << '\n';
		}		
	}
	
	void Test2()
	{
		for ( int i = 0; i < 10; ++i )
		{
			Core::Geometry::Point p = CoreTests::Utils::GetRandomPointInRect( -7, 6, -2, 11 );
			
			std::cout << "Random point " << p.X() << " " << p.Y() << '\n';
		}
	}
}

int main( int argc, char *argv[] )
{
	srand( time( 0 ) );
	
	//Tests::Test1();
	Tests::Test2();
	
	
	
	::testing::InitGoogleTest( &argc, argv );
	
	return RUN_ALL_TESTS();
}
