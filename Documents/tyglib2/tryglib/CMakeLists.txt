cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
include_directories(${OPENGL_INCLUDE_DIR})
#include_directories("deps")
#include_directories("additional")
#include_directories("additional/glad/include")

message(WARNING  "OPENGL_INCLUDE_DIR}")
message(WARNING  ${OPENGL_INCLUDE_DIR})

##include_directories("${CMAKE_SOURCE_DIR}/src")
#file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/libobs/*.h")
#file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/libobs/*.cpp")

set(INPUT ${SOURCE_FILES} ${HEADER_FILES} )

#add_executable(${TARGET_NAME} WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_SOURCES} ${QM_FILES})
add_executable(main WIN32 ${HEADER_FILES} ${RESOURCE_SOURCES} main.cpp) #Win32


#include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/glad")
#include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs-opengl")

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")
include_directories("${CMAKE_BINARY_DIR}/")

#add_subdirectory(libobs-opengl)
#add_subdirectory(deps)

#include_directories("${CMAKE_SOURCE_DIR}/deps/glad/include")
#link_directories("${CMAKE_SOURCE_DIR}/libs")

#add_library(main STATIC obs)
target_link_libraries(main
        obs)

#target_link_libraries(main obs)


#include_directories("${CMAKE_SOURCE_DIR}/gl/")
#link_directories("${CMAKE_SOURCE_DIR}/gl")
