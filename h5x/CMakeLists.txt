find_package(Boost CONFIG REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C CXX CONFIG)

add_library (h5x INTERFACE)

target_sources(h5x PUBLIC FILE_SET HEADERS
				FILES
                 h5x.hpp
                 h5_info.hpp
                 iterator.hpp
                 h5_types.hpp
				 h5_fused.hpp
				 h5_describe.hpp
                 read_cache.hpp
                 detail/view.hpp
                 detail/read_cache.hpp
			   )

add_library (h5x::h5x ALIAS h5x)

target_include_directories(h5x INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:include/h5x>
		)

target_link_libraries(h5x INTERFACE
                        Boost::boost
#                       hdf5::hdf5
                        hdf5::hdf5_cpp$<IF:$<BOOL:${HDF5_LINK_STATICALLY}>,-static,-shared>
                        )

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
							${CMAKE_CURRENT_BINARY_DIR}/h5x-config.cmake
							INSTALL_DESTINATION lib/cmake/h5x
							NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/h5x-config-version.cmake
		VERSION ${CMAKE_PROJECT_VERSION}
		COMPATIBILITY AnyNewerVersion
)

install(TARGETS h5x
        EXPORT h5x-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        FILE_SET HEADERS DESTINATION include/h5x
)

install(FILES 
			${CMAKE_CURRENT_BINARY_DIR}/h5x-config.cmake
			${CMAKE_CURRENT_BINARY_DIR}/h5x-config-version.cmake
		DESTINATION 
		    ${CMAKE_INSTALL_DATADIR}/h5x
)

install(EXPORT h5x-targets
		FILE h5x-targets.cmake
		NAMESPACE h5x::
		DESTINATION ${CMAKE_INSTALL_DATADIR}/h5x
)
