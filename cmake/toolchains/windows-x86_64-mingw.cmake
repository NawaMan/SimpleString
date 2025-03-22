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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MINGW_ROOT}/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${MINGW_ROOT}/include")
