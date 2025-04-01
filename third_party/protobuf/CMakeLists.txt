#**********************************
#  Created by boil on 2023/4/9.
#**********************************

rendu_add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Exclude
)
get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(RD_${FOLDER_NAME} INTERFACE)
target_link_libraries(RD_${FOLDER_NAME} INTERFACE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

add_library(${PROJECT_NAME}::${FOLDER_NAME} ALIAS RD_${FOLDER_NAME})