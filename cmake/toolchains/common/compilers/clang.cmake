# Clang compiler configuration
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Clang-specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
