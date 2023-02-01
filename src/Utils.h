#pragma once

#include <list>



namespace Core
{
	namespace Utils
	{
		template < typename T >
		std::set< T > GetUniqueIntersection( std::list< T > const & list1, std::list< T > const & list2 );
	}
}



template < typename T >
std::set< T > Core::Utils::GetUniqueIntersection( std::list< T > const & list1, std::list< T > const & list2 )
{
	std::set< T > result;

	std::set< T > temp;
	std::for_each( list1.begin(), list1.end(), 
	              [ &temp ] ( T const & item ) { temp.insert( item ); } );
	std::for_each( list2.begin(), list2.end(),
	              [ &temp, &result ] ( T const & item ) 
	              {
	                  auto it = temp.find( item );
	                  if ( it != temp.end() )
	                  {
	                      result.insert( item );
	                  }
	              } );

	return result;
}