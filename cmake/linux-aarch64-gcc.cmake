# Linux AArch64 GCC toolchain configuration

# Include common configurations
include(${CMAKE_CURRENT_LIST_DIR}/common/base.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/linux.cmake)

# Override compiler settings for cross-compilation
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER       aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER     aarch64-linux-gnu-g++)

# Set paths
set(AARCH64_ROOT         /usr/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH ${AARCH64_ROOT})

# Cross-compilation settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Add cross-compilation include paths
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${AARCH64_ROOT}/include")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -I${AARCH64_ROOT}/include")

# Boost configuration
set(BOOST_ROOT       /usr)
set(BOOST_INCLUDEDIR /usr/include)
set(BOOST_LIBRARYDIR /usr/lib/aarch64-linux-gnu)
set(Boost_DEBUG      ON)

# Add AArch64-specific paths
list(APPEND CMAKE_FIND_ROOT_PATH
    /usr/lib/aarch64-linux-gnu
)

# Set Boost paths for AArch64
set(Boost_LOCALE_LIBRARY_RELEASE /usr/lib/aarch64-linux-gnu/libboost_locale.so.1.74.0)
set(Boost_LOCALE_LIBRARY_DEBUG   ${Boost_LOCALE_LIBRARY_RELEASE})
set(Boost_LIBRARIES              ${Boost_LOCALE_LIBRARY_RELEASE})
set(Boost_FOUND                  TRUE)
set(Boost_LOCALE_FOUND           TRUE)
