# Rendu - A free and open-source MMORPG server emulator

# 输出核心信息和所选构建类型
message("")
message("* RenduCore revision   : ${rev_hash} ${rev_date} (${rev_branch} branch)")
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT IS_MULTI_CONFIG)
  message("* RenduCore buildtype  : ${CMAKE_BUILD_TYPE}")
endif()
message("")

# 输出安装目录信息
message("* Install core to        : ${CMAKE_INSTALL_PREFIX}")
if(RENDU_COPY_CONF)
  if(UNIX)
    message("* Install configs to     : ${CONF_DIR}")
  else()
    message("* Install configs to     : ${CMAKE_INSTALL_PREFIX}")
  endif()
endif()
message("")

# 显示配置选项
if(RENDU_SERVERS)
  message("* Build world/auth       : Yes (default)")
else()
  message("* Build world/bnetserver : No")
endif()

if(RENDU_SCRIPTS AND (NOT RENDU_SCRIPTS STREQUAL "none"))
  message("* Build with scripts     : Yes (${RENDU_SCRIPTS})")
else()
  message("* Build with scripts     : No")
endif()

if(RENDU_TOOLS)
  message("* Build map/vmap tools   : Yes (default)")
else()
  message("* Build map/vmap tools   : No")
endif()

if(RENDU_BUILD_TESTING)
  message("* Build unit tests       : Yes")
else()
  message("* Build unit tests       : No (default)")
endif()

if(RENDU_USE_COREPCH)
  message("* Build core w/PCH       : Yes (default)")
else()
  message("* Build core w/PCH       : No")
endif()

if(RENDU_USE_SCRIPTPCH)
  message("* Build scripts w/PCH    : Yes (default)")
else()
  message("* Build scripts w/PCH    : No")
endif()

if(RENDU_WITH_WARNINGS)
  message("* Show all warnings      : Yes")
else()
  message("* Show all warnings      : No  (default)")
endif()

if(RENDU_WITH_WARNINGS_AS_ERRORS)
  message("* Stop build on warning  : Yes")
else()
  message("* Stop build on warning  : No  (default)")
endif()

if(RENDU_WITH_COREDEBUG)
  message("")
  message(" *** RENDU_WITH_COREDEBUG - WARNING!")
  message(" *** additional core debug logs have been enabled!")
  message(" *** this setting doesn't help to get better crash logs!")
  message(" *** in case you are searching for better crash logs use")
  message(" *** -DCMAKE_BUILD_TYPE=RelWithDebInfo")
  message(" *** DO NOT ENABLE IT UNLESS YOU KNOW WHAT YOU'RE DOING!")
  message("* Use coreside debug     : Yes")
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      RENDU_DEBUG)
else()
  message("* Use coreside debug     : No  (default)")
endif()

if(NOT RENDU_WITH_SOURCE_TREE STREQUAL "no")
  message("* Show source tree       : Yes (${RENDU_WITH_SOURCE_TREE})")
else()
  message("* Show source tree       : No")
endif()

if(RENDU_WITHOUT_GIT)
  message("* Use GIT revision hash  : No")
  message("")
  message(" *** RENDU_WITHOUT_GIT - WARNING!")
  message(" *** By choosing the RENDU_WITHOUT_GIT option you have waived all rights for support,")
  message(" *** and accept that or all requests for support or assistance sent to the core")
  message(" *** developers will be rejected. This due to that we will be unable to detect")
  message(" *** what revision of the codebase you are using in a proper way.")
  message(" *** We remind you that you need to use the repository codebase and a supported")
  message(" *** version of git for the revision-hash to work, and be allowede to ask for")
  message(" *** support if needed.")
else()
  message("* Use GIT revision hash  : Yes (default)")
endif()

if(RENDU_WITHOUT_METRICS)
  message("")
  message(" *** RENDU_WITHOUT_METRICS - WARNING!")
  message(" *** This will disable all metrics output (i.e. InfluxDB and Grafana)")
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      WITHOUT_METRICS)
elseif (RENDU_WITH_DETAILED_METRICS)
  message("")
  message(" *** RENDU_WITH_DETAILED_METRICS - WARNING!")
  message(" *** This will enable detailed metrics output (i.e. time each session takes to update)")
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      WITH_DETAILED_METRICS)
endif()

if(BUILD_SHARED_LIBS)
  message("")
  message(" *** RENDU_WITH_DYNAMIC_LINKING - INFO!")
  message(" *** Will link against shared libraries!")
  message(" *** Please note that this is an experimental feature!")
  if(RENDU_WITH_DYNAMIC_LINKING_FORCED)
    message("")
    message(" *** Dynamic linking was enforced through a dynamic script module!")
  endif()
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      RENDU_API_USE_DYNAMIC_LINKING)

  WarnAboutSpacesInBuildPath()
endif()

message("")