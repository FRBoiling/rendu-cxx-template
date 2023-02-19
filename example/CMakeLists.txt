#**********************************
#  Created by boil on 2022/8/14.
#**********************************

if (RENDU_FLAG_EXAMPLES)
  set(PROJECT_NAME example)
  rendu_add_subdirectory(
      ${CMAKE_CURRENT_SOURCE_DIR}
      # Exclude
  )
endif ()


