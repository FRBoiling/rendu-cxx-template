#**********************************
#  Created by boil on 2023/02/16.
#**********************************
#|--------------------------------
#| rendu-core tests module
#|--------------------------------
if (RENDU_BUILD_TESTING)
  set(PROJECT_NAME test)
  rendu_add_subdirectory(
      ${CMAKE_CURRENT_SOURCE_DIR}
      # Exclude
  )
endif ()


