set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

# Specify the cross compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Set paths
set(MINGW_ROOT /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH ${MINGW_ROOT})

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Add MinGW include paths
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++ -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++/x86_64-w64-mingw32 -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++/backward -I/usr/x86_64-w64-mingw32/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/x86_64-w64-mingw32/include")

# Boost configuration
set(BOOST_ROOT /usr/x86_64-w64-mingw32)
set(BOOST_INCLUDEDIR /usr/x86_64-w64-mingw32/include)
set(BOOST_LIBRARYDIR /usr/x86_64-w64-mingw32/lib)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_DEBUG ON)

# Add additional Boost paths
set(CMAKE_FIND_ROOT_PATH
    ${CMAKE_FIND_ROOT_PATH}
    /usr/x86_64-w64-mingw32/lib
    /usr/x86_64-w64-mingw32/include
)

# Set Boost component names
set(Boost_LOCALE_LIBRARY_RELEASE /usr/x86_64-w64-mingw32/lib/libboost_locale.a)
set(Boost_LOCALE_LIBRARY_DEBUG ${Boost_LOCALE_LIBRARY_RELEASE})

# Force CMake to find Boost
set(Boost_INCLUDE_DIR /usr/x86_64-w64-mingw32/include)
set(Boost_LIBRARY_DIRS /usr/x86_64-w64-mingw32/lib)
set(Boost_LIBRARIES ${Boost_LOCALE_LIBRARY_RELEASE})
set(Boost_FOUND TRUE)
set(Boost_LOCALE_FOUND TRUE)
