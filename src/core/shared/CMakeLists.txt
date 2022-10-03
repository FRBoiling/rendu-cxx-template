#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target-name shared)
set(project-name cores)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers/${target-name}_pch.h)
endif ()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_SHARED)

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
    ${public_includes}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${target-name}
    PRIVATE
    rendu-core-interface
    common
    PUBLIC
    protobuf
    rapidjson
    #TODO:添加reps库
    )

set_target_properties(${target-name}
    PROPERTIES
    FOLDER
    ${project-name})

if (BUILD_SHARED_LIBS)
  if (UNIX)
    install(TARGETS shared
        LIBRARY
        DESTINATION lib)
  elseif (WIN32)
    install(TARGETS shared
        RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${target-name} ${PRIVATE_PCH_HEADER})
endif ()