find_package(Boost COMPONENTS program_options log log_setup CONFIG REQUIRED)

add_executable (chunker 
                main.cpp
                chunker.cpp
                loggers.hpp
                loggers.cpp
                )

target_link_libraries(chunker
        PRIVATE
        Boost::boost
        Boost::program_options
        Boost::log
        Boost::log_setup
        h5x::h5x
)

target_compile_options(chunker PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

