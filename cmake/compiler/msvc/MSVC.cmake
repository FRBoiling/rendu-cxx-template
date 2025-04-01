# cmake/Compiler/MSVC.cmake
# ====================================================
# Microsoft Visual C++编译器配置
# 适用于Windows开发环境

# [函数] 应用MSVC专用配置
# 参数:
#   target - 要配置的目标名称
function(rendu_compiler_config_msvc target)
  # 诊断和代码分析选项
  target_compile_options(${target} PRIVATE
      /analyze                  # 启用静态分析
      /sdl                      # 安全开发生命周期检查
      /permissive-              # 严格标准符合模式
      /Zc:__cplusplus           # 正确报告C++版本
      /Zc:inline               # 清除未使用的COMDAT
  )

  # 安全特性配置
  target_compile_options(${target} PRIVATE
      /guard:cf                 # 启用控制流防护
      /Qspectre                 # Spectre缓解措施
  )

  # 调试配置
  target_compile_options(${target} PRIVATE
      /DEBUG:FASTLINK           # 快速调试链接
      /Zc:threadSafeInit-      # 禁用线程安全静态初始化
  )

  # 运行时库配置
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

  # UNICODE支持
  target_compile_definitions(${target} PRIVATE
      UNICODE
      _UNICODE
  )

  # 并行编译配置
  target_compile_options(${target} PRIVATE
      /MP            # 多进程编译
      /Gw            # 全局数据优化
  )

  # 异常处理配置
  target_compile_options(${target} PRIVATE
      $<$<CONFIG:Debug>:/EHa>  # Debug模式启用C++异常
      $<$<NOT:$<CONFIG:Debug>>:/EHsc> # Release模式基本异常
  )
endfunction()