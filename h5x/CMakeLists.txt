cmake_minimum_required (VERSION 3.8)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED )
find_package(HDF5 COMPONENTS C CXX REQUIRED )


add_library (h5x INTERFACE
             h5x.hpp
             iterator.hpp
             h5_types.hpp
             detail/view.hpp
)

target_include_directories(h5x INTERFACE ..)


target_link_libraries(h5x INTERFACE
                        Boost::boost
                        hdf5_cpp-static
                        )

