find_package(Boost COMPONENTS program_options CONFIG REQUIRED)

add_executable (write 
                main.cpp
                options.cpp
                config.hpp
                h5tool.cpp
                )

target_link_libraries(write
        PRIVATE
        Boost::boost
        Boost::program_options
        h5x::h5x
)

target_compile_options(write PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

