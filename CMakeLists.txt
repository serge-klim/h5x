cmake_minimum_required(VERSION 3.24)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project(h5x VERSION 0.0.6 LANGUAGES CXX)

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
