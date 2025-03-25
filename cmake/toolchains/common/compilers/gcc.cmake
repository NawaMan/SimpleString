# GCC compiler configuration
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# GCC-specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
