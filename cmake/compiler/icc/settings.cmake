#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= 架构优化配置 =======================#
if(PLATFORM EQUAL 32)
  # 32位架构自动派发SSE2指令集
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -axSSE2)
  message(STATUS "ICC: 32位架构已启用自动派发(-axSSE2)")
else()
  # 64位架构强制SSE2指令集
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -xSSE2)
  message(STATUS "ICC: 64位架构已启用强制SSE2(-xSSE2)")
endif()

#======================= 诊断系统配置 =======================#
if(WITH_WARNINGS)
  # 启用最高级别警告(-w1)
  target_compile_options(rendu-warning-interface
      INTERFACE
      -w1
      -Wcheck   # 添加语法严格检查
      -Wremarks) # 显示优化备注

  message(STATUS "ICC: 已启用全警告模式(-w1)")
endif()

#======================= 调试符号配置 =======================#
if(WITH_COREDEBUG)
  # 包含调试符号及行号信息
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -g
      -debug inline-debug-info)  # 增强调试信息

  message(STATUS "ICC: 已配置扩展调试符号(-g)")
endif()

#======================= 高级优化配置 =======================#
# 保留现有代码结构，添加以下新配置
if(NOT WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -ipo          # 跨文件优化
      -no-prec-div  # 快速除法
      -fp-model fast=2)

  message(STATUS "ICC: 已启用激进优化模式(-ipo -fp-model)")
endif()
