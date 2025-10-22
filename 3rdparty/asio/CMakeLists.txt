#**********************************
#  Created by boil on 2023/4/9.
#**********************************

rendu_find_package(
        NAME asio
        GITHUB_REPOSITORY chriskohlhoff/asio
        VERSION 1.36.0
        GIT_TAG asio-1-36-0
)

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

rendu_add_library(
        DIR     ${CMAKE_CURRENT_SOURCE_DIR}
        PROJECT    ${PROJECT_NAME}
        NAME    ${FOLDER_NAME}
        LINKOPTS    rendu-dependency-interface
        DEPS asio::asio
        DEFINES
        INTERFACE
        ALIAS     ON
)


