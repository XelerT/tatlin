set(SRCS
        sorter.cpp
)

add_library(sorter ${SRCS})
target_include_directories(sorter PRIVATE ${CMAKE_BINARY_DIR})