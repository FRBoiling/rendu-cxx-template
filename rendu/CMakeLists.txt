#**********************************
#  Created by boil on 2025/8/14.
#**********************************

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${FOLDER_NAME})

rendu_add_subdirectories(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    EXCLUDE_DIRS
)