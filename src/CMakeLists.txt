set(SRC_LIST
        manager
        tape
        utils
        ui
        sorter
)

foreach(SRC ${SRC_LIST})
        add_subdirectory(${SRC})
endforeach()

set(SRC_LIST "${SRC_LIST}" PARENT_SCOPE)
