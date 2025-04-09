# Linux-specific configuration
set(CMAKE_SYSTEM_NAME Linux)

# Linux-specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Linux library paths
list(APPEND CMAKE_FIND_ROOT_PATH
    /usr/lib
    /usr/include
)

# Linux Boost configuration
set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
