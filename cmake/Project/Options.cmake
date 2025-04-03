# cmake/Project/Options.cmake
# ====================================================
# Rendu 构建配置
# 设计原则:
#   1. 声明式配置，无业务逻辑

#===================== 项目功能模块 =====================#
option(RENDU_ENABLE_SERVERS "服务器模块构建" ON)
option(RENDU_ENABLE_TOOLS "工具模块构建）" ON)

#===================== 脚本系统配置 =====================#
set(RENDU_SCRIPTS_MODES none static dynamic minimal-static minimal-dynamic)
# 脚本模块配置
if(RENDU_SCRIPTS_MODE)
  list(FIND RENDU_SCRIPTS_MODES "${RENDU_SCRIPTS_MODE}" _index)
  if(${_index} EQUAL -1)
    message(FATAL_ERROR "无效的脚本模式: (${RENDU_SCRIPTS_MODE})！\n"
        "可用选项: ${RENDU_SCRIPTS_MODES}\n"
        "如果仍有问题，请在论坛搜索 TCE00019 错误代码。\n"
    )
  endif()
endif()

set(RENDU_SCRIPTS_MODE "static" CACHE STRING "脚本模式")
set_property(CACHE RENDU_SCRIPTS_MODE PROPERTY STRINGS ${RENDU_SCRIPTS_MODES})

include(Scripts)
# Build a list of all script modules when -DSCRIPT="custom" is selected
GetScriptModuleList(SCRIPT_MODULE_LIST)
foreach(module ${SCRIPT_MODULE_LIST})
  ScriptModuleNameToVariable(${module} module_variable)
  set(${module_variable} "default" CACHE STRING "Build type of the ${module} module.")
  set_property(CACHE ${module_variable} PROPERTY STRINGS default disabled static dynamic)
endforeach()

#===================== 编译优化配置 =====================#
option(RENDU_USE_CORE_PCH "核心头文件预编译" ON)
option(RENDU_USE_SCRIPT_PCH "脚本头文件预编译" ON)

#===================== 链接配置 =====================#
option(RENDU_DYNAMIC_LINKING "启用动态链接" OFF)
include(DynamicLinking)
IsDynamicLinkingRequired(RENDU_FORCE_DYNAMIC_LINKING)

#option(RENDU_FORCE_DYNAMIC_LINKING "强制动态链接（覆盖用户设置）" ON)
if(RENDU_DYNAMIC_LINKING AND RENDU_FORCE_DYNAMIC_LINKING)
  set(RENDU_FORCE_DYNAMIC_LINKING OFF)  # 强制动态链接（覆盖用户设置）
endif()

if(RENDU_DYNAMIC_LINKING OR RENDU_FORCE_DYNAMIC_LINKING)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "全局共享库开关" FORCE)
else()
  set(BUILD_SHARED_LIBS ${RENDU_DYNAMIC_LINKING} CACHE BOOL "全局共享库开关" FORCE)
endif()

option(RENDU_USE_GOLD_LINKER "使用GNU gold链接器" OFF)

#===================== 调试与监控 =====================#
option(RENDU_ENABLE_CORE_DEBUG "核心调试模式" OFF)
option(RENDU_STRICT_WARNINGS "将警告视为错误" OFF)
option(RENDU_ENABLE_METRICS "基础指标收集" ON)
option(RENDU_DETAILED_METRICS "详细性能指标" OFF)

#===================== 项目配置 =====================#
option(RENDU_AUTO_CONFIG "自动生成配置文件" ON)
set(RENDU_SOURCE_LAYOUT "hierarchical" CACHE STRING "源码组织结构")
set_property(CACHE RENDU_SOURCE_LAYOUT PROPERTY STRINGS
    no flat hierarchical hierarchical-folders)

#option(RENDU_STRICT_DB_TYPES "严格数据库类型检查" OFF)


#===================== 开发者选项 =====================#

option(RENDU_VERBOSE_OUTPUT "详细构建输出" OFF)
option(RENDU_ENABLE_TESTING "构建测试套件" OFF)
option(RENDU_DISABLE_GIT_HOOKS "禁用Git钩子检查" ON)

#===================== 平台特定配置 =====================#

#===================== 输入验证 =====================#

