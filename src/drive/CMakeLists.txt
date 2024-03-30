set(SRCS
        drive.cpp
)

add_library(drive ${SRCS})
target_include_directories(drive PRIVATE ${CMAKE_BINARY_DIR})