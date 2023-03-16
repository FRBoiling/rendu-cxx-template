#**********************************
#  Created by boil on 2023/02/19.
#**********************************

set(target_name spdlog)
list(APPEND other_deps "")
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  list(APPEND other_deps ws2_32)
  #list(APPEND win32libs  wsock32 )
endif ()

rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-dependency-interface
    DEPS
    dep::threads
    dep::fmt
    ${other_deps}
    DEFINES
    -DSPDLOG_COMPILED_LIB
    #    COPTS
    #   -lws2_32
    #   -lwsock32
)
