#**********************************
#  Created by boil on 2022/8/14.
#**********************************

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE_SOURCES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

list(APPEND PRIVATE_SOURCES ${sources_windows})

if(WIN32)
  if(MSVC)
    list(APPEND PRIVATE_SOURCES helloworld.rc)
  endif()
endif()

if(USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/helloworld_pch.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(helloworld
    ${PRIVATE_SOURCES}
    )

if(NOT WIN32)
  set_target_properties(helloworld PROPERTIES
      COMPILE_DEFINITIONS _RENDU_CORE_CONFIG="${CONF_DIR}/helloworld.conf"
      )
endif()

if(UNIX AND NOT NOJEM AND NOT APPLE)
  set(helloworld_LINK_FLAGS "-pthread -lncurses ${helloworld_LINK_FLAGS}")
endif()

set_target_properties(helloworld PROPERTIES LINK_FLAGS "${helloworld_LINK_FLAGS}")

target_link_libraries(helloworld
    PRIVATE
    rendu-core-interface
    PUBLIC
    framework
    #TODO:BOIL 添加依赖项
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC_INCLUDES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(helloworld
    PUBLIC
    ${PUBLIC_INCLUDES}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(helloworld
    PROPERTIES
    FOLDER
    "helloworld")

# Add all dynamic projects as dependency to the helloworld
if(HELLOWORLD_DYNAMIC_SCRIPT_MODULES_DEPENDENCIES)
  add_dependencies(helloworld ${HELLOWORLD_DYNAMIC_SCRIPT_MODULES_DEPENDENCIES})
endif()

if(COPY_CONF AND WIN32)
  if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
    add_custom_command(TARGET helloworld
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/helloworld.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
        )
  elseif(MINGW)
    add_custom_command(TARGET helloworld
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/helloworld.conf.dist ${CMAKE_BINARY_DIR}/bin/
        )
  endif()
endif()

if(UNIX)
  install(TARGETS helloworld DESTINATION bin)
  if(COPY_CONF)
    install(FILES helloworld.conf.dist DESTINATION ${CONF_DIR})
  endif()
elseif(WIN32)
  install(TARGETS helloworld DESTINATION "${CMAKE_INSTALL_PREFIX}")
  if(COPY_CONF)
    install(FILES helloworld.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
  add_cxx_pch(helloworld ${PRIVATE_PCH_HEADER})
endif()
