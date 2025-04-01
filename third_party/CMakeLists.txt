#**********************************
#  Created by boil on 2022/8/14.
#**********************************

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
rendu_add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/argparse
    ${CMAKE_CURRENT_SOURCE_DIR}/date
    ${CMAKE_CURRENT_SOURCE_DIR}/fmt
    ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp
    ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum
    ${CMAKE_CURRENT_SOURCE_DIR}/protobuf
    ${CMAKE_CURRENT_SOURCE_DIR}/spdlog
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib
)
