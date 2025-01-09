find_package(Boost COMPONENTS unit_test_framework CONFIG REQUIRED )

add_executable (unit_test
        test.hpp
        test.cpp
        cache_test.cpp
        h5x_cache_test.cpp
        iterator_test.cpp
        data_sink_test.cpp
)

target_link_libraries(unit_test
        h5x::h5x
        Boost::boost
		Boost::unit_test_framework
)

add_test(NAME test COMMAND unit_test)
