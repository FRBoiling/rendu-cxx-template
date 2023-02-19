#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target_name example_args)
rendu_add_executable(
    NAME
    ${target_name}
    CMAKE_CUR_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    CMAKE_CUR_BINARY_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_BINARY_DIR
    ${CMAKE_BINARY_DIR}
    PRIVATE_DEPS
    PUBLIC_DEPS
    rendu::common
    DEFINES
    LINKOPTS
)

