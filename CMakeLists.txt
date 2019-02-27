cmake_minimum_required(VERSION 3.13)
project(kslice)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_MODULE_PATH, "./cmake")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#message("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

find_package(Ffmpeg) #add here the list of ffmpeg components required
message("FFMPEG_INCLUDE_DIRS = ${FFMPEG_INCLUDE_DIRS}")
message("FFMPEG_LIBRARY_DIRS = ${FFMPEG_LIBRARY_DIRS}")
message("FFMPEG_LIBRARIES = ${FFMPEG_LIBRARIES}")

# I am surprised that FFMPEG requires all of these, maybe I configured FFMPEG incorrectly
# CoreServices.framework
# CoreFoundation.framework
# AudioUnit.framework
# AudioToolbox.framework
# CoreAudio.framework

find_package( OpenCV REQUIRED )
find_package( boost COMPONENTS system filesystem REQUIRED )

#find_library(CORE_MEDIA CoreMedia)
#if (NOT CORE_MEDIA)
#    message(FATAL_ERROR "CoreMedia not found")
#endif()
#
#find_library(CORE_SERVICES CoreServices)
#if (NOT CORE_SERVICES)
#    message(FATAL_ERROR "CoreServices not found")
#endif()
#
#find_library(AUDIO_TOOLBOX AudioToolbox)
#if (NOT AUDIO_TOOLBOX)
#    message(FATAL_ERROR "AudioToolbox not found")
#endif()
#
#find_library(CORE_VIDEO CoreVideo)
#if (NOT CORE_VIDEO)
#    message(FATAL_ERROR "CoreVideo not found")
#endif()
#
#find_library(CORE_FOUNDATION CoreFoundation)
#if (NOT CORE_FOUNDATION)
#    message(FATAL_ERROR "CoreFoundation not found")
#endif()
#
#find_library(VIDEO_TOOLBOX VideoToolbox)
#if (NOT VIDEO_TOOLBOX)
#    message(FATAL_ERROR "VideoToolbox not found")
#endif()
#
#find_library(CORE_AUDIO CoreAudio)
#if (NOT CORE_AUDIO)
#    message(FATAL_ERROR "CoreAudio not found")
#endif()
#
#
#find_library(ICONV Iconv)
#if (NOT ICONV)
#    message(FATAL_ERROR "ICONV not found")
#endif()

#find_library(LIBZ libz.a)
#if (NOT LIBZ)
#    message(FATAL_ERROR "LIBZ not found")
#endif()

#find_package(zlib)
#find_library(LZMA_LIB lzma)
#if (NOT LZMA_LIB)
#    message(FATAL_ERROR "lzma not found")
#endif()
#
#find_library(SWR_LIB swresample)
#if (NOT SWR_LIB)
#    message(FATAL_ERROR "swresample not found")
#endif()
#
#find_library(X264_LIB x264)
#if (NOT X264_LIB)
#    message(FATAL_ERROR "x264 not found")
#endif()
#
##2\. Security.framework
#find_library(SECURITY_FRAMEWORK Security)
#if (NOT SECURITY_FRAMEWORK)
#    message(FATAL_ERROR "Security not found")
#endif()

#find_library(IMAGE_IO ImageIO)
#if (NOT IMAGE_IO)
#    message(FATAL_ERROR "ImageIO not found")
#endif()


if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif()

# apparent macOS bug
# https://stackoverflow.com/questions/45648260/find-packageboost-returns-empty-boost-libraries
if (NOT Boost_LIBRARIES)
    list(APPEND Boost_LIBRARIES "/usr/local/lib/libboost_system.a" "/usr/local/lib/libboost_filesystem.a")
    #message(FATAL_ERROR "Boost_LIBRARIES not found")
endif()

#include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(kslice ${Boost_INCLUDE_DIRS})
message("OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/ezxml")

#link_directories(${FFMPEG_LIBRARY_DIRS})
add_executable(kslice main.cpp Slice.cpp Slice.h)
#target_link_libraries(kslice ${FFMPEG_LIBRARIES})
#target_link_libraries(kslice ${CORE_VIDEO})
##target_link_libraries(kslice ${CORE_AUDIO})
#target_link_libraries(kslice ${CORE_FOUNDATION})
#target_link_libraries(kslice ${VIDEO_TOOLBOX})
#target_link_libraries(kslice ${ICONV})
#target_link_libraries(kslice ${CORE_SERVICES})
#target_link_libraries(kslice ${AUDIO_TOOLBOX})
#target_link_libraries(kslice ${CORE_MEDIA})
#target_link_libraries(kslice ${ZLIB_LIBRARIES})
#target_link_libraries(kslice ${LZMA_LIB})
#target_link_libraries(kslice ${SWR_LIB})
#target_link_libraries(kslice ${X264_LIB})
#target_link_libraries(kslice ${SECURITY_FRAMEWORK})
target_link_libraries(kslice ${OpenCV_LIBS})
target_link_libraries(kslice ${Boost_LIBS})

message("Boost_LIBRARIES = ${Boost_LIBRARIES}")
message("Boost_VERSION = ${Boost_VERSION}")


target_link_libraries( kslice ${Boost_LIBRARIES} )
target_link_libraries( kslice ezxml )
#target_link_libraries(kslice ${LIBZ})
#CoreServices
#AUDIO_TOOLBOX