cmake_minimum_required(VERSION 3.13.4)
project(h5x)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
#set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#string(REPLACE "-O2" "-Ox" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

####################################################################
#find_path(Ittapi_INCLUDE_DIR
#  NAMES ittnotify.h
#  #PATHS ${Ittapi_INCLUDE_DIRS}
#)
#mark_as_advanced(Ittapi_INCLUDE_DIR)
#
#find_library(Ittapi_LIBRARY
#  NAMES ittnotify
#  #PATHS ${Ittapi_LIBRARY_DIRS}
#)
#mark_as_advanced(Ittapi_LIBRARY)
#
#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(Ittapi
#    REQUIRED_VARS Ittapi_INCLUDE_DIR Ittapi_LIBRARY)
#
#
#if(Ittapi_FOUND)
#    set(Ittapi_INCLUDE_DIRS ${Ittapi_INCLUDE_DIR})
#    set(Ittapi_LIBRARIES ${Ittapi_LIBRARY})
#    if(NOT TARGET Ittapi::Ittapi)
#        add_library(Ittapi::Ittapi UNKNOWN IMPORTED)
#        set_target_properties(Ittapi::Ittapi PROPERTIES
#            IMPORTED_LOCATION             "${Ittapi_LIBRARY}"
#            INTERFACE_INCLUDE_DIRECTORIES "${Ittapi_INCLUDE_DIR}")
#    endif()
#endif()
#
#####################################################################

add_subdirectory(h5x)
add_subdirectory(test)
add_subdirectory(explorer)
add_subdirectory(benchmark)
add_subdirectory(write)
#add_subdirectory(log)
#add_subdirectory(runner)
#add_subdirectory(concurent)
#add_subdirectory(qbenchmark)
#add_subdirectory(cpu_time)

