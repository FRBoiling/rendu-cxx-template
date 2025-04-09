if (RENDU_BUILD_TESTING)

    rendu_find_package(
            NAME Catch2
            GITHUB_REPOSITORY catchorg/Catch2
            VERSION 3.6.0
    )

    get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    rendu_add_library(
            DIR ${CMAKE_CURRENT_SOURCE_DIR}
            PROJECT ${PROJECT_NAME}
            NAME ${FOLDER_NAME}
            LINKOPTS rendu-dependency-interface
            DEPS Catch2::Catch2 Catch2::Catch2WithMain
            DEFINES
            INTERFACE
            ALIAS ON
    )

endif ()