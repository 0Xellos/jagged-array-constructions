cmake_minimum_required(VERSION 3.20)
project(jagged-array-constructions VERSION 0.1.0 LANGUAGES CXX)

find_package(Catch2 3 REQUIRED)

include(Catch)

add_executable(benchmark benchmark.cpp)
target_include_directories(benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(benchmark PRIVATE FILE_SET HEADERS FILES nested-vector.hpp flat-vector.hpp)
target_link_libraries(benchmark PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(benchmark WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

