cmake_minimum_required(VERSION 3.10)

# Set Clang as the compiler
set(CMAKE_CXX_COMPILER clang++)

project(simple_string)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Print compiler information
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_COMPILER full path: ${CMAKE_CXX_COMPILER}")

# Use pkg-config to find GTest
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest)

# Find Boost
find_package(Boost REQUIRED COMPONENTS locale)

# Set RPATH for executables
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/lib/x86_64-linux-gnu;/home/linuxbrew/.linuxbrew/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add the library
add_library(simple_string_lib
    src/sstring.cpp
    src/compare_result.cpp
)
target_include_directories(simple_string_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(simple_string_lib PUBLIC
    Boost::locale
)

# Add tests
add_executable(simple_string_tests
    tests/sstring_test.cpp
    tests/compare_result_test.cpp
    tests/sstring_sharing_test.cpp
)
target_include_directories(simple_string_tests PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(simple_string_tests PRIVATE
    simple_string_lib
    ${GTEST_LIBRARIES}
    pthread
)
add_test(NAME sstring_tests COMMAND sstring_tests)

# Keep existing examples
add_executable(unicode_example src/main.cpp)
target_link_libraries(unicode_example PRIVATE Boost::locale)

add_executable(simple_unicode src/simple_unicode.cpp)
target_link_libraries(simple_unicode PRIVATE Boost::locale)
