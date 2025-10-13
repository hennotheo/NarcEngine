# REMOVING COVERAGE FLAGS (WE DONT TEST THOSE LIBS)
set(_CFLAGS   "${CMAKE_C_FLAGS}")
set(_CXXFLAGS "${CMAKE_CXX_FLAGS}")

# strip coverage
string(REPLACE "--coverage" "" CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}")
string(REPLACE "--coverage" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors)
set(FETCHCONTENT_QUIET ON)

set(CMAKE_POLICY_WARNING_CMP0000 OFF)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

include(FetchContent)
message(STATUS "Fetching dependencies...")

set(FETCHCONTENT_FULLY_DISCONNECTED OFF CACHE BOOL "Permet à FetchContent de réutiliser un dépôt local")

# --- PYTHON ---
find_package (Python COMPONENTS Interpreter)

# RE ENABLE ORIGINAL FLAGS
set(CMAKE_C_FLAGS   "${_CFLAGS}")
set(CMAKE_CXX_FLAGS "${_CXXFLAGS}")