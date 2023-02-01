#pragma once

#include "../src/ConstructionContainer.h"

#include <string>



namespace Tests
{
	namespace CoreTests
	{
		void TestConstructionContainer_AddMethods();
		void TestConstructionContainer_RemoveUnit();
		void TestConstructionContainer_RemoveBody();



		void TestConstructionContainer_IntTriangle();

		class IntTriangle
		{
		public:
			IntTriangle( int p1, int p2, int p3 );

			void SetP1( int );

			Core::ConstructionContainer< int, std::string > const & GetContainer() const { return m_container; }

			~IntTriangle();
		private:
			int * m_p1;
			int * m_p2;
			int * m_p3;
			std::string * m_line12;
			std::string * m_line23;
			std::string * m_line31;

			Core::ConstructionContainer< int, std::string > m_container;
		};
	}
}
