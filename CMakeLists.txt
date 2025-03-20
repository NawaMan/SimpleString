cmake_minimum_required(VERSION 3.10)
project(unicode_boost_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS locale)

add_executable(unicode_example src/main.cpp)
target_link_libraries(unicode_example PRIVATE Boost::locale)
