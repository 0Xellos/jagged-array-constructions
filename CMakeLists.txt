cmake_minimum_required(VERSION 3.20)
project(jagged-array-constructions VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)

include(Catch)

add_executable(benchmark benchmark.cpp)
target_include_directories(benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(benchmark PRIVATE FILE_SET HEADERS FILES
        nested-vector.hpp
        flat-vector.hpp
        two-step-construction.hpp
        two-step-construction-split-inputs.hpp
        flat-two-step-construction.hpp
        flat-result-two-step-construction.hpp
)
target_link_libraries(benchmark PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(benchmark WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

