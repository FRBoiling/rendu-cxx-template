#**********************************
#  Created by boil on 2022/10/24.
#**********************************
#|--------------------------------
#| rendu-core tools module
#|--------------------------------
if (RENDU_BUILD_TOOLS)
  get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  project(${FOLDER_NAME})
  rendu_add_subdirectory(
      ${CMAKE_CURRENT_SOURCE_DIR}
      # Exclude
      ${CMAKE_CURRENT_SOURCE_DIR}/proto_generate
  )
endif ()
