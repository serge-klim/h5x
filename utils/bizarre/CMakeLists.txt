cmake_minimum_required (VERSION 3.8)

# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost REQUIRED )
# find_package(HDF5 COMPONENTS C CXX REQUIRED )


add_library (bizarre 
             bizarre.hpp
             bizarre.cpp
)

target_include_directories(bizarre INTERFACE ..)


target_link_libraries(bizarre INTERFACE
                        # Boost::boost
                        # hdf5_cpp-static
                        )

target_compile_options(benchmark PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/Qvec-report:2>
  $<$<CXX_COMPILER_ID:CLANG>:-Rpass-missed=loop-vectorize>
)
