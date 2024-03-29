set(SRCS
        inode.cpp
)

add_library(inode ${SRCS})
target_include_directories(inode PRIVATE ${CMAKE_BINARY_DIR})
