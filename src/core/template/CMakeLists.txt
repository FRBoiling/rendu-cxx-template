#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(EXEC_NAME template)
set(project-name cores)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

list(APPEND private_sources ${sources_windows})

if (WIN32)
  if (MSVC)
    list(APPEND private_sources ${EXEC_NAME}.rc)
  endif ()
endif ()

if (USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${EXEC_NAME}_pch.h)
endif ()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXEC_NAME}
    ${private_sources}
    )

if (NOT WIN32)
  set_target_properties(${EXEC_NAME} PROPERTIES
      COMPILE_DEFINITIONS _RENDU_CORE_CONFIG="${CONF_DIR}/${EXEC_NAME}.conf"
      )
endif ()

set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS "${${EXEC_NAME}_LINK_FLAGS}")

target_link_libraries(${EXEC_NAME}
    PRIVATE
    rendu-interface
    PUBLIC
    framework
    #TODO:BOIL 添加依赖项
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    public_includes
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${EXEC_NAME}
    PUBLIC
    ${public_includes}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(${EXEC_NAME}
    PROPERTIES
    FOLDER
    ${project-name})

if (COPY_CONF AND WIN32)
  if ("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
    add_custom_command(TARGET ${EXEC_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${EXEC_NAME}.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
        )
  elseif (MINGW)
    add_custom_command(TARGET ${EXEC_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${EXEC_NAME}.conf.dist ${CMAKE_BINARY_DIR}/bin/
        )
  endif ()
endif ()

if (UNIX)
  install(TARGETS ${EXEC_NAME} DESTINATION bin)
  if (COPY_CONF)
    install(FILES ${EXEC_NAME}.conf.dist DESTINATION ${CONF_DIR})
  endif ()
elseif (WIN32)
  install(TARGETS ${EXEC_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}")
  if (COPY_CONF)
    install(FILES ${EXEC_NAME}.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(${EXEC_NAME} ${PRIVATE_PCH_HEADER})
endif ()
