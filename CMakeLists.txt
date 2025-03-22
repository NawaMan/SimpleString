cmake_minimum_required(VERSION 3.10)

# Set policies
cmake_policy(SET CMP0048 NEW)  # project() command manages VERSION variables
cmake_policy(SET CMP0042 NEW)  # MACOSX_RPATH is enabled by default

project(SString VERSION 1.0.0 LANGUAGES CXX)

# Output architecture information
message(STATUS "Building for: ${CMAKE_SYSTEM_NAME} - ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally enable testing
option(BUILD_TESTING "Build tests" ON)

# Find Boost
find_package(Boost REQUIRED COMPONENTS locale)

# Print compiler information
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_COMPILER full path: ${CMAKE_CXX_COMPILER}")

# Set RPATH for executables
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/lib/x86_64-linux-gnu;/home/linuxbrew/.linuxbrew/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Add the library
add_library(sstring_lib
    src/compare_result.cpp
    src/sstring.cpp
)
target_include_directories(sstring_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(sstring_lib PUBLIC
    Boost::locale
)

# Set library output directory to dist
set_target_properties(sstring_lib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist
)

# Add tests if enabled
if(BUILD_TESTING)
    enable_testing()
    
    # Use pkg-config to find GTest
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTEST REQUIRED gtest)

    # Add tests
    add_executable(sstring_tests
        tests/sstring_test.cpp
        tests/compare_result_test.cpp
        tests/sstring_sharing_test.cpp
        tests/char_test.cpp
        tests/sstring_char_test.cpp
    )
    target_include_directories(sstring_tests PRIVATE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(sstring_tests PRIVATE
        sstring_lib
        ${GTEST_LIBRARIES}
        pthread
    )
    add_test(NAME sstring_tests COMMAND sstring_tests)
endif()
