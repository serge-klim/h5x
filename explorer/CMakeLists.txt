cmake_minimum_required (VERSION 3.8)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options REQUIRED )
#find_package(HDF5 COMPONENTS C CXX REQUIRED )

#find_package(spdlog REQUIRED)


add_executable (explorer 
                main.cpp
                options.cpp
                config.hpp
                h5tool.cpp
                )

target_link_libraries(explorer
        PRIVATE
        Boost::boost
        Boost::program_options
#        hdf5_cpp-static
        h5x
)

target_compile_options(explorer PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

#target_compile_definitions(runner PUBLIC -DSPDLOG_ACTIVE_LEVEL=0)
