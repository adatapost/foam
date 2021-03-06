
/*
 *
 * (C) Copyright Sarfaraz Nawaz 2013-14, sir_nawaz959@yahoo.com
 *
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt Or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#pragma once

#include <type_traits>
#include <memory>
#include <cmath>

#include <foam/composition/processor_pipe.h>
#include <foam/composition/generator_range.h>
#include <foam/composition/builder.h>
#include <foam/composition/functor.h>
#include <foam/composition/expression.h>

namespace foam
{
	namespace composition
	{
		//from 
		template<typename Iterable>
		auto from(Iterable & c) -> from_range<Iterable>
		{
			return from_range<Iterable>(&c);
		}

		//template<typename T>
		//auto from(std::initializer_list<T> const & c) -> from_range<std::initializer_list<T> const>
		//{
		//	return from_range<std::initializer_list<T> const>(&c);
		//}

		//template<typename Iterable>
		//auto from(Iterable const & c) -> from_range<Iterable const>
		//{
		//	return from_range<Iterable const>(&c);
		//}

		//template<typename Iterable>
		//auto from(Iterable && c) -> from_range<Iterable>
		//{
		//	return from_range<Iterable>(&c);
		//}
		
		//generate (infinity)
		template<typename T, typename UnaryGenerator>
		auto generate(T start, UnaryGenerator generator) -> generate_range<details::functor<UnaryGenerator, T, T>>
		{
			return {start, details::functor<UnaryGenerator, T, T>(generator) };
		}
		
		//arithmetic_sequence (infinity)
		template<typename T, typename U>
		auto arithmetic_sequence(T start, U difference) -> decltype(generate(start, foam::composition::expression<U>() + difference ))
		{
			return generate(start, foam::composition::expression<U>() + difference );
		}

		//geometric_sequence (infinity)
		template<typename T>
		auto geometric_sequence(T start, T ratio) -> decltype(generate(start, foam::composition::expression<T>() * ratio ))
		{
			return generate(start, foam::composition::expression<T>() * ratio );
		}
	
		//prime_sequence
		template<typename T>
		auto prime_sequence(T start) -> decltype(generate(start, details::next_prime() ))
		{
			details::next_prime p{};
			return generate(p.first(start), p);
		}

		//prime_range 
		template<typename T>
		auto prime_range(T start, T end) -> decltype(prime_sequence(start) | take_while( foam::composition::expression<T>() < end ))
		{
			return prime_sequence(start) | take_while( foam::composition::expression<T>() < end );
		}

		//range [start, end)
		template<typename T, typename U>
		auto range(T start, U end) -> decltype(arithmetic_sequence(T(), 1) | take(0)) 
		{
			return arithmetic_sequence(start, end > start ? 1 : -1 ) | take( std::abs(end - start) );
		}
		
		//repeat
		template<typename T>
		auto repeat(T value, int count) -> decltype(arithmetic_sequence(0, 0) | take(0)) 
		{
			return arithmetic_sequence(value, 0) | take(count);
		}
	}
}
