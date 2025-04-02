# Windows x86_64 MSVC toolchain configuration

# Include common configurations
include(${CMAKE_CURRENT_LIST_DIR}/common/base.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/windows.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common/compilers/msvc.cmake)

# Set architecture
set(CMAKE_SYSTEM_PROCESSOR   AMD64)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_GENERATOR_TOOLSET  "v143") # Visual Studio 2022
