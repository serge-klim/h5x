cmake_minimum_required(VERSION 3.8)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED )

add_executable (test
        test.hpp
        test.cpp
		bizarre_test.cpp
)

target_link_libraries(test
        bizarre
        Boost::boost
		Boost::unit_test_framework
)

