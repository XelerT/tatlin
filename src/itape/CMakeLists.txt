set(SRCS
        itape.cpp
)

add_library(itape ${SRCS})
target_include_directories(itape PRIVATE ${CMAKE_BINARY_DIR})