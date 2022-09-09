#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(LIB_NAME common)
set(FOLDER_NAME cores)

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
  set(PRIVATE_PCH_HEADER precompiled_headers/${LIB_NAME}_pch.h)
endif (USE_COREPCH)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_COMMON)

add_library(${LIB_NAME}
    ${PRIVATE_SOURCES}
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${LIB_NAME}
    PUBLIC
    # Provide the binary dir for all child targets
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${LIB_NAME}
    PRIVATE
    rendu-interface
    PUBLIC
    boost
    fmt
    utf8cpp
    sfmt
    readline
    spdlog
    magic_enum
    #TODO:添加reps库
    )

add_dependencies(${LIB_NAME} revision_data.h)

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