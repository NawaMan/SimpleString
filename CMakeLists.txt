cmake_minimum_required(VERSION 3.10)

# Set Clang as the compiler - must be before project()
set(CMAKE_CXX_COMPILER clang++)

project(unicode_boost_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Print compiler information
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_COMPILER full path: ${CMAKE_CXX_COMPILER}")

find_package(Boost REQUIRED COMPONENTS locale)

add_executable(unicode_example src/main.cpp)
target_link_libraries(unicode_example PRIVATE Boost::locale)

add_executable(simple_unicode src/simple_unicode.cpp)
target_link_libraries(simple_unicode PRIVATE Boost::locale)
