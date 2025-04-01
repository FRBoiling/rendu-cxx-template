#**********************************
#  Created by boil on 2022/9/10.
#**********************************
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${CMAKE_PROJECT_NAME}
    NAME
    ${FOLDER_NAME}
    LINKOPTS
    rendu-core-interface
    DEPS
)
