#**********************************
#  Created by boil on 2023/4/9.
#**********************************

rendu_find_package(
        NAME Spdlog
        GITHUB_REPOSITORY gabime/spdlog
        GIT_TAG v1.16.0
)

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

rendu_add_library(
        DIR     ${CMAKE_CURRENT_SOURCE_DIR}
        PROJECT    ${PROJECT_NAME}
        NAME    ${FOLDER_NAME}
        PRIVATE_LINK    rendu-dependency-interface
        PUBLIC_LINK spdlog::spdlog
        DEFINES
        INTERFACE
        ALIAS     ON
)
