#**********************************
#  Created by boil on 2022/8/14.
#**********************************
set(target-name framework)
set(project-name cores)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${target-name}_pch.h)
endif ()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_FRAMEWORK)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    public_includes
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

# Provide an interface target for the game project to allow
# dependent projects to build meanwhile.
add_library(${target-name}-interface INTERFACE)

target_include_directories(${target-name}-interface
    INTERFACE
    ${public_includes})

target_link_libraries(${target-name}-interface
    INTERFACE
    )

add_library(${target-name}
    ${private_sources})

target_include_directories(${target-name}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${target-name}
    PRIVATE
    rendu-core-interface
    PUBLIC
    common
    shared
    ${target-name}-interface)

set_target_properties(${target-name}
    PROPERTIES
    FOLDER
    ${project-name})

if (BUILD_SHARED_LIBS)
  if (UNIX)
    install(TARGETS ${target-name}
        LIBRARY
        DESTINATION lib)
  elseif (WIN32)
    install(TARGETS ${target-name}
        RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${target-name} ${PRIVATE_PCH_HEADER})
endif ()
