#**********************************
#  Created by boil on 2023/4/9.
#**********************************
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${FOLDER_NAME}
    LINKOPTS
    rendu-dependency-interface
    DEPS
)
