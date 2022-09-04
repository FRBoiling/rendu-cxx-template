#**********************************
#  Created by boil on 2022/8/14.
#**********************************
set(LIB_NAME framework)
set(FOLDER_NAME cores)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE_SOURCES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${LIB_NAME}_pch.h)
endif ()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_FRAMEWORK)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

# Provide an interface target for the game project to allow
# dependent projects to build meanwhile.
add_library(${LIB_NAME}-interface INTERFACE)

target_include_directories(${LIB_NAME}-interface
    INTERFACE
    ${PUBLIC_INCLUDES})

target_link_libraries(${LIB_NAME}-interface
    INTERFACE
    )

add_library(${LIB_NAME}
    ${PRIVATE_SOURCES})

target_include_directories(${LIB_NAME}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${LIB_NAME}
    PRIVATE
    rendu-interface
    PUBLIC
    common
    shared
    ${LIB_NAME}-interface)

set_target_properties(${LIB_NAME}
    PROPERTIES
    FOLDER
    ${FOLDER_NAME})

if (BUILD_SHARED_LIBS)
  if (UNIX)
    install(TARGETS ${LIB_NAME}
        LIBRARY
        DESTINATION lib)
  elseif (WIN32)
    install(TARGETS ${LIB_NAME}
        RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${LIB_NAME} ${PRIVATE_PCH_HEADER})
endif ()
