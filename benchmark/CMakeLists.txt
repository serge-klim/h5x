cmake_minimum_required(VERSION 3.13.4)
set(CMAKE_CXX_STANDARD 23)

set(Boost_USE_STATIC_LIBS ON)
#find_package(Boost REQUIRED filesystem iostreams)
find_package(benchmark REQUIRED)

add_executable (benchmark
        benchmark.cpp
        )

target_link_libraries(benchmark
        bizarre
#       Boost::boost
#       Boost::filesystem
#       Boost::iostreams
        benchmark::benchmark
        )

target_compile_options(benchmark PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/Qvec-report:2>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
