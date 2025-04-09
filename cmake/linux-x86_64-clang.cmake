# Linux x86_64 Clang toolchain configuration

# Include common configurations
include(${CMAKE_CURRENT_LIST_DIR}/common/base.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/linux.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/compilers/clang.cmake)

# Set architecture
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m64")
