#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target-name common)
set(project-name cores)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${target-name}_pch.h)
endif (USE_COREPCH)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_COMMON)

add_library(${target-name}
    ${private_sources}
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    public_includes
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${target-name}
    PUBLIC
    # Provide the binary dir for all child targets
    ${CMAKE_BINARY_DIR}
    ${public_includes}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${target-name}
    PRIVATE
    rendu-core-interface
    PUBLIC
    spdlog
    sfmt
    utf8cpp
    #TODO:添加reps库
    )

add_dependencies(${target-name} revision_data.h)

target_compile_definitions(${target-name}
    PUBLIC
    -DSPDLOG_COMPILED_LIB
    -DRENDU_
    )
set_target_properties(${target-name}
    PROPERTIES
    FOLDER
    ${project-name})

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${target-name} ${PRIVATE_PCH_HEADER})
endif ()