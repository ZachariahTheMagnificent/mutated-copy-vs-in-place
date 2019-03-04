//#define MUTATE_IN_PLACE

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <mutated-copy-vs-in-place/statistics.hpp>
using zachariahs_world::math::Statistics;
using zachariahs_world::math::get_statistics;

template<typename iterator>
void quick_sort(const iterator begin, const iterator end)
{
	if (begin == end)
	{
		return;
	}

	auto middle = *begin;
	auto lower_end = begin + 1;

	for ( auto it = begin + 1; it != end; ++it )
	{
		if ( *it < middle )
		{
			std::swap ( *lower_end, *it );
			++lower_end;
		}
	}

	std::swap ( *begin, *( lower_end - 1 ) );

	quick_sort ( begin, lower_end - 1 );
	quick_sort ( lower_end, end );
}

#if defined MUTATE_IN_PLACE
template<typename type>
void quick_sort ( std::vector<type>& vector )
{
	quick_sort ( vector.begin ( ), vector.end ( ) );
}
#else
template<typename type>
std::vector<type> quick_sort ( std::vector<type> vector )
{
	quick_sort ( vector.begin ( ), vector.end ( ) );
	return vector;
}
#endif

int main ( )
{
	constexpr auto num_tests = 1000;
	constexpr auto num_ints = 1'000;
	constexpr auto num_frames = 1000;
	constexpr auto lowest_int = int { 12 };
	constexpr auto highest_int = int { 758 };

	using clock = std::chrono::steady_clock;

	std::cout << "Mutate copy vs mutate in place benchmark test\n";
	std::cout << '\n';

	auto test = [ lowest_int, highest_int, num_ints, num_frames ]
	{
		auto rng_machine = std::mt19937 { };
		const auto int_generator = std::uniform_int_distribution<int> { lowest_int, highest_int };
		auto original_ints = std::vector<int> { };
		original_ints.reserve ( num_ints );

		for ( auto index = std::size_t { }; index < num_ints; ++index ) 
		{
			original_ints.push_back ( int_generator ( rng_machine ) );
		}
		std::vector<int> results;

		for ( auto index = std::size_t { }; index < num_frames; ++index )
		{

#if defined MUTATE_IN_PLACE
			quick_sort ( original_ints );
#else
			auto results = quick_sort ( original_ints );
#endif
		}
	};

	auto data_points = std::vector<unsigned long long> { };

	// Warmup
	test ( );

	for ( auto index = std::size_t { }; index < num_tests; ++index )
	{
		// Beginning of benchmark
		auto start = clock::now ( );
		test ( );
		auto end = clock::now ( );
		auto duration = end - start;
		auto duration_in_nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds> ( duration );
		auto count = duration_in_nanoseconds.count ( );
		data_points.push_back ( count );

		std::cout << "Test " << index + 1 << '/' << num_tests << " done!\n";
	}

	const auto statistics = get_statistics ( data_points );
	std::cout << "Programmed with: ";
	if constexpr (sizeof(void*) == 8)
	{
		std::cout << "[x64]";
	}
	std::cout
#if defined MUTATE_IN_PLACE
		<< "[MUTATE_IN_PLACE]"
#endif
		<< '\n';
	std::cout << "Average: " << statistics.mean << "ns\n";
	std::cout << "Standard deviation: " << statistics.standard_deviation << "ns\n";
	std::cout << "Highest: " << statistics.highest << "ns\n";
	std::cout << "Lowest: " << statistics.lowest << "ns\n";
	std::cout << "Median: " << statistics.median << "ns\n";
	return 0;
}
