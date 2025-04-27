#**********************************
#  Created by boil on 2023/4/9.
#**********************************

rendu_add_subdirectories(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    EXCLUDE_DIRS
)

# 设置 CMake 模块路径
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(RD_${FOLDER_NAME} INTERFACE)
target_link_libraries(RD_${FOLDER_NAME} INTERFACE spdlog::spdlog)

add_library(${PROJECT_NAME}::${FOLDER_NAME} ALIAS RD_${FOLDER_NAME})