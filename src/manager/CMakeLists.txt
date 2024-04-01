set(SRCS
        manager.cpp
)

add_library(manager ${SRCS})
target_include_directories(manager PRIVATE ${CMAKE_BINARY_DIR})