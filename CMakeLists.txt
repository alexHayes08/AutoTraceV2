cmake_minimum_required(VERSION 3.1)

project(AutoTraceV3)
# Version number
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (CMAKE_CXX_STANDARD 14)

# Set build dir
set(CMAKE_BINARY_DIR build)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#subdirs(src/InputReaders src/OutputWriters)

#set(SOURCES "src/common.h")
file(GLOB SOURCES
    "src/*.h"s
    "src/*.cpp"
    "src/InputReaders/*.h"
    "src/InputReaders/*.cpp"
    "src/OutputWriters/*.h"
    "src/OutputWriters/*.cpp"
)

# Configure a header file to pass some of the CMake settings
# to the source
#configure_file (
#    "${PROJECT_SOURCE_DIR}/AutoTraceConfig.h.in"
#    "${PROJECT_BINARY_DIR}/AutoTraceConfig.h"
#    )

add_executable(${PROJECT_NAME} ${SOURCES})
add_definitions(-DDEBUG)
