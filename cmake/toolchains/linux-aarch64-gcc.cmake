set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Set paths
set(AARCH64_ROOT /usr/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH ${AARCH64_ROOT})

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Add cross-compilation include paths
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${AARCH64_ROOT}/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${AARCH64_ROOT}/include")

# Boost configuration
set(Boost_ARCHITECTURE "-aarch64")
set(Boost_COMPILER "-gcc")
set(BOOST_ROOT /usr/aarch64-linux-gnu)
set(BOOST_INCLUDEDIR ${BOOST_ROOT}/include)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib)
