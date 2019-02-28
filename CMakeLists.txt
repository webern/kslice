cmake_minimum_required(VERSION 3.13)
project(kslice)

set(CMAKE_CXX_STANDARD 11)
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package( OpenCV REQUIRED )
find_package( boost COMPONENTS system filesystem program_options REQUIRED )

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

# apparent macOS bug with find_package(boost)
# https://stackoverflow.com/questions/45648260/find-packageboost-returns-empty-boost-libraries
#if (NOT Boost_LIBRARIES)
#    list(APPEND Boost_LIBRARIES "/usr/local/lib/libboost_system.a" "/usr/local/lib/libboost_filesystem.a" "libboost_program_options.a")
#    #message(FATAL_ERROR "Boost_LIBRARIES not found")
#endif()

#include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
#link_directories(${Boost_LIBRARY_DIRS})

#include_directories(kslice ${Boost_INCLUDE_DIRS})
#message("OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

#add_subdirectory("lib/extern/ezxml")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_executable(kslice main.cpp)
target_link_libraries(kslice ${OpenCV_LIBS})
#target_link_libraries(kslice ${Boost_LIBS})

#message("Boost_LIBRARIES = ${Boost_LIBRARIES}")
#message("Boost_VERSION = ${Boost_VERSION}")

#target_link_libraries( kslice ${Boost_LIBRARIES} )
#target_link_libraries( kslice ezxml )
target_link_libraries( kslice kscore )
