cmake_minimum_required (VERSION 3.8)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED filesystem iostreams)
find_package(benchmark REQUIRED)

add_executable (benchmark
        benchmark.cpp
        data.hpp
        )

target_link_libraries(benchmark
        h5x
        Boost::boost
        Boost::filesystem
        Boost::iostreams
        benchmark::benchmark
        )

#target_compile_options(benchmark PRIVATE
#  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /Ox>
#  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
#)
