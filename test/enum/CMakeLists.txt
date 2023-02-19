#**********************************
#  Created by boil on 2023/02/19.
#**********************************

set(PROJECT_NAME test)
rendu_add_test(
    NAME
    rendu_enum
    CMAKE_CUR_SOURCE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    CMAKE_CUR_BINARY_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_BINARY_DIR
    ${CMAKE_BINARY_DIR}
    DEPS
    rendu::common
    rendu::test
    DEFINES
    LINKOPTS
)

