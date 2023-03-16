#**********************************
#  Created by boil on 2022/9/10.
#**********************************
set(target_name sfmt)
rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-dependency-interface
    DEPS
)

