#include "ConstructionContainerTests.h"
#include "../../src/ConstructionContainer.h"

#include <string>
#include <iostream>



void Tests::CoreTests::TestConstructionContainer_AddMethods()
{
	Core::ConstructionContainer< int, std::string > container;

	std::cout << container << '\n';

	int const p1 = 1;
	int const p2 = 2;
	container.Add( &p1 );
	container.Add( &p2 );

	std::cout << container << '\n';

	std::string const line12( "line12" );
	container.Add( &line12, { &p1, &p2 } );

	std::cout << container << '\n';

	int const p3 = 3;
	container.AddUnitToBody( &p3, &line12 );

	std::cout << container << '\n';

	int const p4 = 4;
	container.Add( &p4 );
	std::string const circle134( "circle134" );
	container.Add( &circle134, { &p1, &p3, &p4 } );

	std::cout << container << '\n';

	//container.Add( &p2 );

	int const p5 = 5;
	std::string const line15( "line15" );
	container.Add( &line15, { &p5, &p1 } );

	std::cout << container << '\n';

	//container.Add( &line12, { &p2, &p1 } );

	//container.AddUnitToBody( &p3, &line15 );

	//container.AddUnitToBody( &p1, &circle134 );

	//container.AddUnitToBody( &p5, &circle134 );

	//std::cout << container << '\n';

	container.AddUnitToBody( &p2, &circle134 );

	std::cout << container << '\n';


}

void Tests::CoreTests::TestConstructionContainer_RemoveUnit()
{
	Core::ConstructionContainer< int, std::string > container;

	int const p1 = 1;
	int const p2 = 2;
	container.Add( &p1 );
	container.Add( &p2 );
	std::string const line12( "line12" );
	container.Add( &line12, { &p1, &p2 } );
	int const p3 = 3;
	container.AddUnitToBody( &p3, &line12 );
	int const p4 = 4;
	container.Add( &p4 );
	std::string const circle134( "circle134" );
	container.Add( &circle134, { &p1, &p3, &p4 } );
	int const p5 = 5;
	std::string const line15( "line15" );
	container.Add( &line15, { &p5, &p1 } );
	//container.AddUnitToBody( &p5, &circle134 );

	std::cout << container << '\n';

	container.Remove( &p4 );
	//container.Remove( &p3 );
	//container.Remove( &p2 );
	//container.Remove( &p1 );
	//container.Remove( &p5 );

	std::cout << container << '\n';
}

void Tests::CoreTests::TestConstructionContainer_RemoveBody()
{
	Core::ConstructionContainer< int, std::string > container;

	int const p1 = 1;
	int const p2 = 2;
	std::string const line12( "line12" );
	int const p3 = 3;
	int const p4 = 4;
	int const p7 = 7;
	int const p8 = 8;
	std::string const line37( "line37" );
	std::string const line48( "line48" );
	int const p5 = 5;
	int const p6 = 6;
	std::string const line56( "line56" );
	int const p9 = 9;
	std::string const line19( "line19" );

	container.Add( &p1 );
	container.Add( &p2 );
	container.Add( &line12, { &p1, &p2 } );
	container.AddUnitToBody( &p3, &line12 );
	container.AddUnitToBody( &p4, &line12 );
	container.Add( &p7 );
	container.Add( &p8 );
	container.Add( &line37, { &p3, &p7 } );
	container.Add( &line48, { &p4, &p8 } );
	container.AddUnitToBody( &p5, &line37 );
	container.AddUnitToBody( &p6, &line48 );
	container.Add( &line56, { &p5, &p6 } );
	container.AddUnitToBody( &p9, &line37 );
	container.Add( &line19, { &p1, &p9 } );

	std::cout << container << '\n';

	//container.Remove( &line12 );
	container.Remove( &line48 );

	std::cout << container << '\n';

}



void Tests::CoreTests::TestConstructionContainer_IntTriangle()
{
	IntTriangle triangle( 4, 5, 6 );

	std::cout << triangle.GetContainer() << '\n';

	triangle.SetP1( 9 );

	std::cout << triangle.GetContainer() << '\n';
}

Tests::CoreTests::IntTriangle::IntTriangle( int p1, int p2, int p3 )
	: m_p1( new int( p1 ) ),
	  m_p2( new int( p2 ) ),
	  m_p3( new int( p3 ) )
{
	// m_container.Add( m_p1 );
	// m_container.Add( m_p2 );
	// m_container.Add( m_p3 );

	m_line12 = new std::string( "line" + std::to_string( p1 ) + std::to_string( p2 ) );
	m_line23 = new std::string( "line" + std::to_string( p2 ) + std::to_string( p3 ) );
	m_line31 = new std::string( "line" + std::to_string( p3 ) + std::to_string( p1 ) );

	m_container.Add( m_line12, { m_p1, m_p2 } );
	m_container.Add( m_line23, { m_p2, m_p3 } );
	m_container.Add( m_line31, { m_p3, m_p1 } );
}

void Tests::CoreTests::IntTriangle::SetP1( int p1 )
{
	m_container.Remove( m_p1 );

	delete m_p1;
	delete m_line12;
	delete m_line31;

	m_p1 = new int( p1 );
	m_line12 = new std::string( "line" + std::to_string( p1 ) + std::to_string( *m_p2 ) );
	m_line31 = new std::string( "line" + std::to_string( *m_p3 ) + std::to_string( p1 ) );

	m_container.Add( m_p1 );
	m_container.Add( m_line12, { m_p1, m_p2 } );
	m_container.Add( m_line31, { m_p3, m_p1 } );
}

Tests::CoreTests::IntTriangle::~IntTriangle()
{
	delete m_p1;
	delete m_p2;
	delete m_p3;
	delete m_line12;
	delete m_line23;
	delete m_line31;
}

















