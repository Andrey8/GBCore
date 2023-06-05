#pragma once

#include <list>
#include <set>
#include <algorithm>



namespace Core
{
	namespace Utils
	{
		template < typename T >
		std::set< T > GetUniqueIntersection( std::list< T > const & list1, std::list< T > const & list2 );

		template < typename InputIterator1, typename InputIterator2 >
		std::set< typename InputIterator1::value_type > GetSetDifference(
				InputIterator1 first1, InputIterator1 last1,
				InputIterator2 first2, InputIterator2 last2 );
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

template < typename InputIterator1, typename InputIterator2 >
std::set< typename InputIterator1::value_type > Core::Utils::GetSetDifference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2 )
{
	using T = typename InputIterator1::value_type;

	std::set< T > result;

	std::for_each( first1, last1,
				   [ &first2, &last2, &result ] ( T const & item )
				   {
					   auto it = std::find( first2, last2, item );

					   if ( it == last2 )
					   {
						   result.insert( item );
					   }
				   } );

	return result;
}

















