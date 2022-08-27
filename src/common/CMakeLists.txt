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

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/common_pch.h)
endif (USE_COREPCH)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_COMMON)

add_library(${TARGET_NAME}
    ${PRIVATE_SOURCES}
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${TARGET_NAME}
    PUBLIC
    # Provide the binary dir for all child targets
      ${CMAKE_BINARY_DIR}
      ${PUBLIC_INCLUDES}
    PRIVATE
      ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${TARGET_NAME}
    PRIVATE
      rendu-core-interface
    PUBLIC
      boost
      fmt
      utf8cpp
      sfmt
    )

add_dependencies(${TARGET_NAME} revision_data.h)

set_target_properties(${TARGET_NAME}
    PROPERTIES
    FOLDER
    "rendu")

if (BUILD_SHARED_LIBS)
  if (UNIX)
    install(TARGETS ${TARGET_NAME}
        LIBRARY
        DESTINATION lib)
  elseif (WIN32)
    install(TARGETS ${TARGET_NAME}
        RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${TARGET_NAME} ${PRIVATE_PCH_HEADER})
endif ()