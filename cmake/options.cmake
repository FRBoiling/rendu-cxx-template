#**********************************
#  Created by boil on 2022/8/14.
#**********************************

option(CORES           "构建 cores 目录中所有可执行程序"                            1)

set(SCRIPTS_AVAILABLE_OPTIONS none static dynamic minimal-static minimal-dynamic)

# 当SCRIPTS变量的值不是一个有效选项时，记录一个致命错误。
if(SCRIPTS)
  list(FIND SCRIPTS_AVAILABLE_OPTIONS "${SCRIPTS}" SCRIPTS_INDEX)
  if(${SCRIPTS_INDEX} EQUAL -1)
    message(FATAL_ERROR "您的SCRIPTS变量的值(${SCRIPTS})无效! 允许的值是:${SCRIPTS_AVAILABLE_OPTIONS}在TCE00019论坛搜索有问题。")                        
  endif()
endif()

set(SCRIPTS "static" CACHE STRING "使用脚本构建核心")
set_property(CACHE SCRIPTS PROPERTY STRINGS ${SCRIPTS_AVAILABLE_OPTIONS})

# 当选择-DSCRIPT="custom"时，构建所有脚本模块的列表
GetScriptModuleList(SCRIPT_MODULE_LIST)
foreach(SCRIPT_MODULE ${SCRIPT_MODULE_LIST})
  ScriptModuleNameToVariable(${SCRIPT_MODULE} SCRIPT_MODULE_VARIABLE)
  set(${SCRIPT_MODULE_VARIABLE} "default" CACHE STRING "构建 ${SCRIPT_MODULE} 模块的类型。")
  set_property(CACHE ${SCRIPT_MODULE_VARIABLE} PROPERTY STRINGS default disabled static dynamic)
endforeach()

option(TOOLS            "构建 tools 目录中所有可执行程序"              1)
option(USE_SCRIPTPCH    "编译 script 时使用预编译头"              1)
option(USE_COREPCH      "编译 core 时使用预编译头"              1)
option(WITH_DYNAMIC_LINKING "启用动态库链接。"                         0)
IsDynamicLinkingRequired(WITH_DYNAMIC_LINKING_FORCED)
if(WITH_DYNAMIC_LINKING AND WITH_DYNAMIC_LINKING_FORCED)
  set(WITH_DYNAMIC_LINKING_FORCED OFF)
endif()
if(WITH_DYNAMIC_LINKING OR WITH_DYNAMIC_LINKING_FORCED)
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF)
endif()
option(WITH_WARNINGS    "显示编译期间的所有警告"                            0)
option(WITH_COREDEBUG   "在core中包含额外的debug-code"                       0)
option(WITH_STRICT_DATABASE_TYPE_CHECKS "启用严格检查数据库字段值访问器" 0)
option(WITHOUT_METRICS  "禁用指标报告(例如，InfluxDB和Grafana)"       0)
option(WITH_DETAILED_METRICS  "启用详细的度量报告(例如，每个会话更新所需的时间)" 0)
option(COPY_CONF        "拷贝 可执行程序 和 .conf.dist 文件到项目目录"      1)
set(WITH_SOURCE_TREE    "hierarchical" CACHE STRING "为IDE构建源代码树。")
set_property(CACHE WITH_SOURCE_TREE PROPERTY STRINGS no flat hierarchical hierarchical-folders)
option(WITHOUT_GIT      "禁用GIT测试例程"                            0)
option(BUILD_TESTING    "构建测试套件" 1)

if(UNIX)
  option(USE_LD_GOLD    "Use GNU gold 连接器"                                        0)
endif()
