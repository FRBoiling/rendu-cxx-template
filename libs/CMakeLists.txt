#**********************************
#  Created by boil on 2022/10/24.
#**********************************
#|--------------------------------
#| rendu-core rendu module
#|--------------------------------

message(STATUS "正在编译 ${CMAKE_PROJECT_NAME} ")

rendu_add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Exclude
)
