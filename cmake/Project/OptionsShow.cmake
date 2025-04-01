# cmake/Project/OptionsShow.cmake
# ====================================================

#**********************************
#  Created by boil on 2025/02/21.
#**********************************

# Rendu 构建配置全景视图
# 设计原则:
#   1. 展示Options.cmake中所有配置开关


# ======================= 配置摘要生成 ======================= #
message("")
include(ShowMessage)

##===================== 项目功能模块 =====================#
print_section("核心功能")
print_switch("服务器模块构建" RENDU_ENABLE_SERVERS ${Green} "✔ 已启用" ${Red} "已禁用")
print_switch("工具模块构建 " RENDU_ENABLE_TOOLS ${Green} "✔ 已启用" ${Red} "已禁用")

#===================== 脚本系统配置 =====================#
if (RENDU_SCRIPTS_MODE STREQUAL "none")
  print_final_option("脚本模块构建" "✖ 已禁用" ${Red})
else ()
  print_final_option("脚本模块构建" "${RENDU_SCRIPTS_MODE} 模式" ${Green})
endif ()

#===================== 编译优化配置 =====================#
print_section("编译策略")
print_switch("核心头文件预编译" RENDU_USE_CORE_PCH ${Green} "✔ 已启用" ${Red} "✖ 已禁用")
print_switch("脚本头文件预编译" RENDU_USE_SCRIPT_PCH ${Green} "✔ 已启用" ${Red} "✖ 已禁用")

#===================== 链接配置 =====================#
if (UNIX AND NOT APPLE)
  print_switch("Gold链接器" RENDU_USE_GOLD_LINKER ${Green} "✔ 已启用" ${Magenta} "✖ 已禁用")
endif ()
print_switch("动态链接开关  " RENDU_DYNAMIC_LINKING ${Yellow} "✔ 已启用" ${Bold} "未启用")
print_switch("强制动态开关  " RENDU_FORCE_DYNAMIC_LINKING ${Yellow} "✔ 已启用" ${Bold} "未启用")
print_final_switch("链接方式  " BUILD_SHARED_LIBS ${Green} "动态开关开启" ${Magenta} "动态开关关闭")


#===================== 调试与监控 =====================#
print_section("调试监控")
print_switch("核心调试  " RENDU_ENABLE_CORE_DEBUG ${Yellow} "✖ 已启用" ${Green} "✔ 已禁用")
print_switch("严格告警  " RENDU_STRICT_WARNINGS ${Green} "✔ 已启用" ${Magenta} "✖ 已禁用")
print_switch("基础指标  " RENDU_ENABLE_METRICS ${Green} "✔ 已启用" ${Red} "✖ 已禁用")
print_final_switch("详细指标  " RENDU_DETAILED_METRICS ${Yellow} "✔ 已启用" ${Magenta} "✖ 已禁用")

#===================== 项目配置 =====================#
print_section("项目配置")
print_switch("自动配置  " RENDU_AUTO_CONFIG ${Green} "✔ 已启用" ${Red} "✖ 已禁用")
print_final_option("源码布局  " "${RENDU_SOURCE_LAYOUT}" ${Magenta})

#print_final_switch("严格数据库" RENDU_STRICT_DB_TYPES ${Yellow} "✔ 已启用" ${Bold} "未启用")

#===================== 开发者选项 =====================#
print_section("开发者选项")
print_switch("详细输出  " RENDU_VERBOSE_OUTPUT ${Yellow} "✔ 已启用" ${Magenta} "✖ 已禁用")
print_switch("测试套件  " RENDU_ENABLE_TESTING ${Yellow} "✔ 已启用" ${Magenta} "✖ 已禁用")
print_final_switch("Git检查  " RENDU_DISABLE_GIT_HOOKS ${Yellow} "✔ 已启用" ${Green} "✖ 已禁用")


# ======================= 智能警告系统 ======================= #
include(OptionsWarningCheck)
