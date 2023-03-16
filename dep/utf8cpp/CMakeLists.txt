#**********************************
#  Created by boil on 2023/02/19.
#**********************************

set(target_name utf8cpp)
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
