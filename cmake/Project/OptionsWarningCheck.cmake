include(ShowMessage)

# 全局警告计数器和消息列表
set(WARNING_COUNT 0)

# 定义 warning 函数
function(print_warning_message msg)
  # 更新全局计数器和消息列表
  math(EXPR new_count "${WARNING_COUNT} + 1")
  set(WARNING_COUNT ${new_count} PARENT_SCOPE)  # 使用 PARENT_SCOPE 传递回父作用域
  print_option(" 第${new_count}个  " ${msg} ${Yellow})
endfunction()

function(show_warnings)
  if (WARNING_COUNT GREATER 0)
    print_final_option(" 检查结束" "共发现 ${WARNING_COUNT} 个潜在风险配置" ${Red})
  endif ()
endfunction()


print_section("智能警告")
if (RENDU_STRICT_WARNINGS)
  print_warning_message("严格告警模式已启用 - 可能存在编译错误")
endif ()
if (RENDU_ENABLE_CORE_DEBUG AND NOT CMAKE_BUILD_TYPE MATCHES "Relase|RelWithDebInfo")
  print_warning_message("核心调试模式建议使用 Debug/RelWithDebInfo 构建类型")
endif ()
if (NOT BUILD_SHARED_LIBS AND (RENDU_USE_COREPCH OR RENDU_USE_SCRIPTPCH))
  print_warning_message("静态链接与预编译头可能存在兼容性问题")
endif ()
if (RENDU_SCRIPTS_MODE STREQUAL \"none\")
  print_warning_message("脚本系统已禁用 - 部分游戏功能不可用")
endif ()
if (RENDU_DETAILED_METRICS AND NOT RENDU_ENABLE_METRICS)
  print_warning_message("详细指标需要基础指标功能支持")
endif ()
if (RENDU_FORCE_DYNAMIC_LINKING)
  print_warning_message("动态链接已被强制启用（覆盖用户设置）")
endif ()

show_warnings()

message("")