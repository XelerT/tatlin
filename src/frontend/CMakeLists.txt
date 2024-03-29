set(SRCS
        error_handl.cpp
        driver.cpp
)

add_library(frontend ${SRCS})
target_include_directories(frontend PRIVATE ${CMAKE_BINARY_DIR})
