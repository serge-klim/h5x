cmake_minimum_required(VERSION 3.13.4)
project(unrelated-benchmarks)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
#set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#string(REPLACE "-O2" "-Ox" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

add_subdirectory(benchmark)
add_subdirectory(utils/bizarre)
add_subdirectory(test)

