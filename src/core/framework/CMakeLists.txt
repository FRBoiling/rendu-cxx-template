#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target_name framework)
rendu_add_library(
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
    rendu::shared
    DEFINES
    LINKOPTS
)
