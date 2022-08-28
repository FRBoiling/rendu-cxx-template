#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(TARGET_NAME helloworld)
CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE_SOURCES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

list(APPEND PRIVATE_SOURCES ${sources_windows})

if(WIN32)
  if(MSVC)
    list(APPEND PRIVATE_SOURCES ${TARGET_NAME}.rc)
  endif()
endif()

if(USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${TARGET_NAME}_pch.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${TARGET_NAME}
    ${PRIVATE_SOURCES}
    )

if(NOT WIN32)
  set_target_properties(${TARGET_NAME} PROPERTIES
      COMPILE_DEFINITIONS _RENDU_CORE_CONFIG="${CONF_DIR}/${TARGET_NAME}.conf"
      )
endif()

set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "${${TARGET_NAME}_LINK_FLAGS}")

target_link_libraries(${TARGET_NAME}
    PRIVATE
    rendu-interface
    PUBLIC
    framework
    #TODO:BOIL 添加依赖项
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${PUBLIC_INCLUDES}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(${TARGET_NAME}
    PROPERTIES
    FOLDER
    "rendu")

if(COPY_CONF AND WIN32)
  if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
    add_custom_command(TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
        )
  elseif(MINGW)
    add_custom_command(TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.conf.dist ${CMAKE_BINARY_DIR}/bin/
        )
  endif()
endif()

if(UNIX)
  install(TARGETS ${TARGET_NAME} DESTINATION bin)
  if(COPY_CONF)
    install(FILES ${TARGET_NAME}.conf.dist DESTINATION ${CONF_DIR})
  endif()
elseif(WIN32)
  install(TARGETS ${TARGET_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}")
  if(COPY_CONF)
    install(FILES ${TARGET_NAME}.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
  add_cxx_pch(${TARGET_NAME} ${PRIVATE_PCH_HEADER})
endif()
