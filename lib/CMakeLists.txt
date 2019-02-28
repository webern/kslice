cmake_minimum_required(VERSION 3.13)
project(kscore)

set(CMAKE_CXX_STANDARD 11)
set(PUBLIC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PRIVATE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
message("PUBLIC_INCLUDE = ${PUBLIC_INCLUDE}")
message("PRIVATE_INCLUDE = ${PRIVATE_INCLUDE}")

find_package( OpenCV REQUIRED )

set(Boost_USE_STATIC_LIBS   ON)
find_package( boost COMPONENTS system filesystem program_options REQUIRED )

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

# apparent macOS bug with find_package(boost)
# https://stackoverflow.com/questions/45648260/find-packageboost-returns-empty-boost-libraries
if (NOT Boost_LIBRARIES)
    list(APPEND Boost_LIBRARIES "/usr/local/lib/libboost_system.a" "/usr/local/lib/libboost_filesystem.a" "libboost_program_options.a")
endif()

if (NOT Boost_LIBRARIES)
    message(FATAL_ERROR "Boost_LIBRARIES not found")
endif()

#include_directories(PUBLIC ${PUBLIC_INCLUDE})
#include_directories(PRIVATE ${PRIVATE_INCLUDE})
include_directories(PRIVATE ${OpenCV_INCLUDE_DIRS})
link_directories(PRIVATE ${Boost_LIBRARY_DIRS})

include_directories(kslice ${Boost_INCLUDE_DIRS})
message("OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/ezxml")

add_library(kscore kslice.cpp Slice.cpp)
target_include_directories(kscore PUBLIC include)
target_include_directories(kscore PRIVATE .)
target_link_libraries(kscore ${OpenCV_LIBS})
target_link_libraries(kscore ${Boost_LIBS})

message("Boost_LIBRARIES = ${Boost_LIBRARIES}")
message("Boost_VERSION = ${Boost_VERSION}")

target_link_libraries( kscore ${Boost_LIBRARIES} )
target_link_libraries( kscore ezxml )
