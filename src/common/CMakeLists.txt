#**********************************
#  Created by boil on 2022/8/14.
#**********************************
set(TARGET_NAME common)
CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES
  # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

list(APPEND PRIVATE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/debugging/errors.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/debugging/errors.h)


if(USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/common_pch.h)
endif(USE_COREPCH)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DTRINITY_API_EXPORT_COMMON)

add_library(${TARGET_NAME}
  ${PRIVATE_SOURCES}
)
