cmake_minimum_required(VERSION 3.11)
project(mutated-copy-vs-in-place-exe VERSION 1.0.0 DESCRIPTION "The executable of the mutated-copy-vs-in-place project" LANGUAGES CXX)

add_executable(mutated-copy-vs-in-place)
target_compile_features(mutated-copy-vs-in-place PRIVATE cxx_std_17)
target_include_directories(mutated-copy-vs-in-place PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(mutated-copy-vs-in-place
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/mutated-copy-vs-in-place/statistics.hpp"
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
)
