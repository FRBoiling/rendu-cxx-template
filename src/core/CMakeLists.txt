#**********************************
#  Created by boil on 2022/8/14.
#**********************************

if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
  add_definitions(-fno-delete-null-pointer-checks)
endif ()

if (WIN32)
  list(APPEND sources_windows
      ${CMAKE_SOURCE_DIR}/src/common/platform/ServiceWin32.cpp
      ${CMAKE_SOURCE_DIR}/src/common/platform/ServiceWin32.h
      )
endif (WIN32)

AddSubdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

