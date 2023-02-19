#**********************************
#  Created by boil on 2022/8/14.
#**********************************

rendu_add_library(
    NAME
    common
    CMAKE_CUR_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    CMAKE_CUR_BINARY_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_BINARY_DIR
    ${CMAKE_BINARY_DIR}
    PRIVATE_DEPS
    rendu-core-interface
    PUBLIC_DEPS
    spdlog::spdlog
    sfmt::sfmt
    utf8cpp::utf8
    DEFINES
    LINKOPTS
)