#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target_name protobuf)
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
    dep::protobuf
    rendu::test
    DEFINES
    LINKOPTS
)
