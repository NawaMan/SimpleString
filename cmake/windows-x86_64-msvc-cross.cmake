# Cross-compilation toolchain file for Microsoft Visual C++ compiler
set(CMAKE_SYSTEM_NAME      Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compilers (using Wine for MSVC)
set(CMAKE_C_COMPILER   wine cl.exe)
set(CMAKE_CXX_COMPILER wine cl.exe)
set(CMAKE_RC_COMPILER  wine rc.exe)

# Set MSVC environment variables
set(ENV{INCLUDE}     "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.29.30133/include;C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/ucrt;C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/shared;C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um")
set(ENV{LIB}         "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.29.30133/lib/x64;C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/ucrt/x64;C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64")
set(ENV{LIBPATH}     "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.29.30133/lib/x64")
set(ENV{PATH}        "C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64;C:/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0/x64;${ENV{PATH}}")

# Set target environment path
set(CMAKE_FIND_ROOT_PATH "Z:/usr/x86_64-w64-mingw32")

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set installation paths
set(CMAKE_INSTALL_PREFIX "C:/Program Files/SString" CACHE PATH "Installation directory")

# Configure Boost for cross-compilation
set(Boost_INCLUDE_DIR     "Z:/usr/x86_64-w64-mingw32/include")
set(Boost_LIBRARY_DIR     "Z:/usr/x86_64-w64-mingw32/lib")
set(Boost_USE_STATIC_LIBS ON)

# MSVC specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP /W4")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /MP /W4")

# Set runtime library to Multi-threaded DLL
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   /MD")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   /MDd")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     /MDd")
