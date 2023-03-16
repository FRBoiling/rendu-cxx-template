#**********************************
#  Created by boil on 2023/02/19.
#**********************************

set(target_name container)
rendu_add_test(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-core-interface
    DEPS
    rendu::common
    rendu::test
    DEFINES
    LINKOPTS
)
