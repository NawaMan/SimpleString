# Windows-specific configuration
set(CMAKE_SYSTEM_NAME Windows)

# Windows-specific definitions
add_definitions(-DWIN32 -D_WINDOWS)

# Windows Boost configuration
set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME OFF)
