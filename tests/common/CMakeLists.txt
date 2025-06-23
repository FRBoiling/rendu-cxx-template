#**********************************
#  Created by boil on 2022/10/24.
#**********************************

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

rendu_add_executable(
    DIR       ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT   ${PROJECT_NAME}
    NAME      ${FOLDER_NAME}
    LINKOPTS  rendu-core-interface
    DEPS      3rdparty::catch
    DEFINES   
    ALIAS     ON
)







