cmake_minimum_required(VERSION 3.1)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED )

add_executable (test
        test.hpp
        test.cpp
        iterator_test.cpp
)

target_link_libraries(test
        h5x
        Boost::boost
		Boost::unit_test_framework
)

