# Rendu - 一个免费开源的 MMORPG 服务器模拟器
# =========================
# 构建选项配置
# =========================

# 是否构建 launcher 可执行文件
option(RENDU_LAUCHER "构建 launcher" ON)
# 工具链相关选项
option(RENDU_TOOLS "构建工具" ON)
# 测试相关选项
option(BUILD_TESTING "构建测试套件" ON)
set(RENDU_BUILD_TESTING ${BUILD_TESTING})

# 脚本构建模式可选项
set(RENDU_SCRIPTS_AVAILABLE_OPTIONS none static dynamic minimal-static minimal-dynamic)
# 校验 RENDU_SCRIPTS 变量是否合法
if(RENDU_SCRIPTS)
  list(FIND RENDU_SCRIPTS_AVAILABLE_OPTIONS "${RENDU_SCRIPTS}" RENDU_SCRIPTS_INDEX)
  if(${RENDU_SCRIPTS_INDEX} EQUAL -1)
    message(FATAL_ERROR "RENDU_SCRIPTS 变量值 (${RENDU_SCRIPTS}) 非法！允许值为: ${RENDU_SCRIPTS_AVAILABLE_OPTIONS}，如有疑问请查阅论坛 TCE00019。")
  endif()
endif()

# 设置 RENDU_SCRIPTS 默认值及可选项
set(RENDU_SCRIPTS "static" CACHE STRING "选择核心脚本构建方式")
set_property(CACHE RENDU_SCRIPTS PROPERTY STRINGS ${RENDU_SCRIPTS_AVAILABLE_OPTIONS})

include(ConfigureScripts)
# 当 RENDU_SCRIPTS="custom" 时，构建所有脚本模块
rendu_get_script_module_list(RENDU_SCRIPT_MODULE_LIST)
foreach(SCRIPT_MODULE ${RENDU_SCRIPT_MODULE_LIST})
  rendu_script_module_name_to_variable(${SCRIPT_MODULE} RENDU_SCRIPT_MODULE_VARIABLE)
  set(${RENDU_SCRIPT_MODULE_VARIABLE} "default" CACHE STRING "${SCRIPT_MODULE} 模块的构建类型")
  set_property(CACHE ${RENDU_SCRIPT_MODULE_VARIABLE} PROPERTY STRINGS default disabled static dynamic)
endforeach()


option(RENDU_USE_SCRIPTPCH "编译脚本时使用预编译头" ON)
option(RENDU_USE_COREPCH "编译核心库时使用预编译头" ON)

# 动态链接依赖判断
option(RENDU_WITH_DYNAMIC_LINKING "启用动态库链接" OFF)
rendu_is_dynamic_linking_required(RENDU_WITH_DYNAMIC_LINKING_FORCED)
if(RENDU_WITH_DYNAMIC_LINKING AND RENDU_WITH_DYNAMIC_LINKING_FORCED)
  set(RENDU_WITH_DYNAMIC_LINKING_FORCED OFF)
endif()
if(RENDU_WITH_DYNAMIC_LINKING OR RENDU_WITH_DYNAMIC_LINKING_FORCED)
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF)
endif()

# 警告与调试相关选项
option(RENDU_WITH_WARNINGS "编译时显示所有警告" OFF)
option(RENDU_WITH_WARNINGS_AS_ERRORS "将警告视为错误" OFF)
option(RENDU_WITH_COREDEBUG "包含核心的额外调试代码" OFF)

# 指标与配置文件相关选项
option(RENDU_WITHOUT_METRICS "禁用指标上报（如 InfluxDB 和 Grafana）" OFF)
option(RENDU_WITH_DETAILED_METRICS "启用详细指标上报（如统计每个会话的更新时间）" OFF)
option(RENDU_COPY_CONF "将 authserver 和 worldserver 的 .conf.dist 文件复制到项目目录" ON)

# IDE 源码树分组模式
set(RENDU_WITH_SOURCE_TREE "hierarchical" CACHE STRING "为 IDE 构建源码树结构")
set_property(CACHE RENDU_WITH_SOURCE_TREE PROPERTY STRINGS no flat hierarchical hierarchical-folders)

# GIT 相关选项
option(RENDU_WITHOUT_GIT "禁用 GIT 测试流程" OFF)

# UNIX 平台专用选项
if(UNIX)
  option(RENDU_USE_LD_GOLD "使用 GNU gold 链接器" OFF)
endif()