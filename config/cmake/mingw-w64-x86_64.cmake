set(CMAKE_SYSTEM_NAME      Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compilers
set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER  x86_64-w64-mingw32-windres)

# Set target environment path
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set installation paths
set(CMAKE_INSTALL_PREFIX "C:/Program Files/SString" CACHE PATH "Installation directory")

# Configure Boost for cross-compilation
set(Boost_INCLUDE_DIR /usr/x86_64-w64-mingw32/include)
set(Boost_LIBRARY_DIR /usr/x86_64-w64-mingw32/lib)
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_NO_SYSTEM_PATHS    ON)

# Set Boost component paths
set(Boost_LOCALE_LIBRARY_DEBUG   ${Boost_LIBRARY_DIR}/libboost_locale-mt-x64.a)
set(Boost_LOCALE_LIBRARY_RELEASE ${Boost_LIBRARY_DIR}/libboost_locale-mt-x64.a)
set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER  x86_64-w64-mingw32-windres)

# Set target environment path
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set installation paths
set(CMAKE_INSTALL_PREFIX "C:/Program Files/SString" CACHE PATH "Installation directory")
