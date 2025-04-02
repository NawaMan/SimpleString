# macOS-specific configuration
set(CMAKE_SYSTEM_NAME Darwin)

# macOS-specific flags
set(CMAKE_MACOSX_RPATH ON)

# macOS Boost configuration
set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

# Set deployment target for macOS
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS deployment version")
