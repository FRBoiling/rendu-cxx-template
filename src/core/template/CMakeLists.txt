#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target_name template)
set(project-name cores)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

list(APPEND private_sources ${sources_windows})

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${target_name}_pch.h)
endif ()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${target_name}
    ${private_sources}
    )

if (NOT WIN32)
  set_target_properties(${target_name} PROPERTIES
      COMPILE_DEFINITIONS _RENDU_CORE_CONFIG="${CONF_DIR}/${target_name}.conf"
      )
endif ()

set_target_properties(${target_name} PROPERTIES LINK_FLAGS "${${target_name}_LINK_FLAGS}")

target_link_libraries(${target_name}
    PRIVATE
    rendu-core-interface
    PUBLIC
    framework
    #TODO:BOIL 添加依赖项
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    public_includes
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${target_name}
    PUBLIC
    ${public_includes}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(${target_name}
    PROPERTIES
    FOLDER
    ${project-name})

if (COPY_CONF AND WIN32)
  if ("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
    add_custom_command(TARGET ${target_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${target_name}.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
        )
  elseif (MINGW)
    add_custom_command(TARGET ${target_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${target_name}.conf.dist ${CMAKE_BINARY_DIR}/bin/
        )
  endif ()
endif ()

if (UNIX)
  install(TARGETS ${target_name} DESTINATION bin)
  if (COPY_CONF)
    install(FILES ${target_name}.conf.dist DESTINATION ${CONF_DIR})
  endif ()
elseif (WIN32)
  install(TARGETS ${target_name} DESTINATION "${CMAKE_INSTALL_PREFIX}")
  if (COPY_CONF)
    install(FILES ${target_name}.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${target_name} ${PRIVATE_PCH_HEADER})
endif ()
