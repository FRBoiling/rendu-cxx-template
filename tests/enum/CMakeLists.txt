#**********************************
#  Created by boil on 2023/02/19.
#**********************************

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

rendu_add_executable(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${FOLDER_NAME}
    SETTING
    rendu-core-interface
    DEPS
    rendu::common
    DEFINES
    LINKOPTS
)
