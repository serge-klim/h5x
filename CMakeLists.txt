cmake_minimum_required(VERSION 3.24)

project(h5x VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option (HDF5_LINK_STATICALLY "link HDF5 statically" OFF)
option (BUILD_TESTS "build tests" ON)
option (BUILD_TOOLS "build tools" ON)
option (BUILD_BENCHMARK "build benchmark" ON)

add_subdirectory(h5x)
if(BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif ()
