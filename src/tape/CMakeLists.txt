set(SRCS
        tape.cpp
)

add_library(tape ${SRCS})
target_include_directories(tape PRIVATE ${CMAKE_BINARY_DIR})