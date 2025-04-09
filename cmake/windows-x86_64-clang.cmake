# Windows x86_64 Clang toolchain configuration for cross-compilation

# Specify the target system
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

# Include common configurations
include(${CMAKE_CURRENT_LIST_DIR}/common/base.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/windows.cmake)

# Use Clang with MinGW target
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set target triple for Windows
set(TARGET_TRIPLE x86_64-w64-mingw32)
set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})

# Set MinGW paths
set(MINGW_ROOT /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH ${MINGW_ROOT})

# Cross-compilation settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Enable LLVM IR generation by default for this toolchain
set(LLVM_IR_GENERATION ON CACHE BOOL "Enable LLVM IR generation" FORCE)

# Windows-specific Clang flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-compatibility -fms-extensions")

# Add MinGW include paths
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++ -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++/x86_64-w64-mingw32 -I/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/c++/backward -I/usr/x86_64-w64-mingw32/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/x86_64-w64-mingw32/include")

# Ensure static linking of standard libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

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
