# Windows x86_64 MinGW-w64 toolchain configuration

# Include common configurations
include(${CMAKE_CURRENT_LIST_DIR}/common/base.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/windows.cmake)

# Set architecture and cross-compiler
set(CMAKE_SYSTEM_PROCESSOR AMD64)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Set paths
set(MINGW_ROOT /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH ${MINGW_ROOT})

# Cross-compilation settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Add MinGW include paths
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++ -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++/x86_64-w64-mingw32 -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++/backward -I/usr/x86_64-w64-mingw32/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/x86_64-w64-mingw32/include")

# MinGW-specific Boost configuration
set(BOOST_ROOT ${MINGW_ROOT})
set(BOOST_INCLUDEDIR ${MINGW_ROOT}/include)
set(BOOST_LIBRARYDIR ${MINGW_ROOT}/lib)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_DEBUG ON)
set(Boost_NO_SYSTEM_PATHS ON)

# Add MinGW-specific paths
list(APPEND CMAKE_FIND_ROOT_PATH
    ${MINGW_ROOT}/lib
)

# Set Boost paths for MinGW
set(Boost_LOCALE_LIBRARY_RELEASE ${MINGW_ROOT}/lib/libboost_locale.a)
set(Boost_REGEX_LIBRARY_RELEASE  ${MINGW_ROOT}/lib/libboost_regex.a)
set(Boost_LOCALE_LIBRARY_DEBUG   ${Boost_LOCALE_LIBRARY_RELEASE})
set(Boost_REGEX_LIBRARY_DEBUG    ${Boost_REGEX_LIBRARY_RELEASE})
set(Boost_LIBRARIES              ${Boost_LOCALE_LIBRARY_RELEASE} ${Boost_REGEX_LIBRARY_RELEASE})
set(Boost_FOUND                  TRUE)
set(Boost_LOCALE_FOUND           TRUE)
set(Boost_REGEX_FOUND            TRUE)

# Ensure static linking of standard libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -D_GLIBCXX_USE_CXX11_ABI=1")
