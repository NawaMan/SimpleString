# macOS x86_64 toolchain configuration

# Include common configurations
include(${CMAKE_CURRENT_LIST_DIR}/common/base.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/macos.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/compilers/clang.cmake)

# Set architecture
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -arch x86_64")
