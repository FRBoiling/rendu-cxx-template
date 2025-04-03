#**********************************
#  Created by boil on 2022/8/14.
#**********************************
set(target_name launcher)
rendu_add_executable(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    LINKOPTS
    rendu-core-interface
    DEPS
    rendu::core
    DEFINES
)