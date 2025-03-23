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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${AARCH64_ROOT}/include -I/usr/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${AARCH64_ROOT}/include -I/usr/include")

# Boost configuration
set(BOOST_ROOT /usr)
set(BOOST_INCLUDEDIR /usr/include)
set(BOOST_LIBRARYDIR /usr/lib/aarch64-linux-gnu)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG ON)

# Add additional Boost paths
set(CMAKE_FIND_ROOT_PATH
    ${CMAKE_FIND_ROOT_PATH}
    /usr/lib/aarch64-linux-gnu
    /usr/include
)

# Set Boost component names
set(Boost_LOCALE_LIBRARY_RELEASE /usr/lib/aarch64-linux-gnu/libboost_locale.so.1.74.0)
set(Boost_LOCALE_LIBRARY_DEBUG ${Boost_LOCALE_LIBRARY_RELEASE})

# Force CMake to find Boost
set(Boost_INCLUDE_DIR /usr/include)
set(Boost_LIBRARY_DIRS /usr/lib/aarch64-linux-gnu)
set(Boost_LIBRARIES ${Boost_LOCALE_LIBRARY_RELEASE})
set(Boost_FOUND TRUE)
set(Boost_LOCALE_FOUND TRUE)
