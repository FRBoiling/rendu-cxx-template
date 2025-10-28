#**********************************
#  Created by boil on 2022/8/14.
#**********************************

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

rendu_add_executable(
        DIR
        ${CMAKE_CURRENT_SOURCE_DIR}
        PROJECT
        ${PROJECT_NAME}
        NAME
        ${FOLDER_NAME}
        PRIVATE_LINK
        rendu-core-interface rendu::common
        PUBLIC_LINK
        DEFINES
)