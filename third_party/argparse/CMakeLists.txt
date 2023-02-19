#**********************************
#  Created by boil on 2023/02/19.
#**********************************

set(PROJECT_NAME argparse)
rendu_add_library(
    NAME
    argparse
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