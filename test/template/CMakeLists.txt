#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target_name template)
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
    rendu::test
    DEFINES
    LINKOPTS
)

