#**********************************
#  Created by boil on 2022/10/24.
#**********************************
#|--------------------------------
#| rendu-core tools module
#|--------------------------------
if (RENDU_BUILD_TOOLS)
  set(PROJECT_NAME tool)
  rendu_add_subdirectory(
      ${CMAKE_CURRENT_SOURCE_DIR}
      # Exclude
      ${CMAKE_CURRENT_SOURCE_DIR}/proto_generate
  )
endif ()
