# MSVC compiler configuration
set(CMAKE_C_COMPILER cl)
set(CMAKE_CXX_COMPILER cl)

# MSVC-specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")

# Set runtime library to dynamic (MD/MDd)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
