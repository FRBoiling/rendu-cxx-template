#**********************************
#  Created by boil on 2023/02/19.
#**********************************

set(PROJECT_NAME utf8cpp)
rendu_add_library(
    NAME
    utf8
    CMAKE_CUR_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    CMAKE_CUR_BINARY_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_BINARY_DIR
    ${CMAKE_BINARY_DIR}
    PRIVATE_DEPS
    rendu-dependency-interface
    PUBLIC_DEPS
    DEFINES
    LINKOPTS
)