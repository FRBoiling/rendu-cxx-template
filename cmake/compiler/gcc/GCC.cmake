# cmake/Compiler/GCC.cmake
# ====================================================
# GNU GCC编译器专用配置
# 适用于Linux/mingw环境

# [函数] 应用GCC专用配置
# 参数:
#   target - 要配置的目标名称
function(rendu_compiler_platform_config target)
  # 诊断和静态分析选项
  target_compile_options(${target} PRIVATE
      # 额外警告选项
      -Wdouble-promotion     # 隐式float到double转换警告
      -Wnull-dereference     # 空指针解引用警告
      -Wduplicated-branches  # 重复分支警告
      -Wshadow               # 变量遮蔽警告
      # 代码生成优化
      -fno-strict-aliasing   # 严格别名规则放宽
  )

  # LTO优化配置
  if(RENDU_ENABLE_LTO)
    target_compile_options(${target} PRIVATE
        -flto
        -fno-fat-lto-objects
    )
    target_link_options(${target} PRIVATE
        -flto
        -fuse-linker-plugin
    )
  endif()

  # 安全加固选项
  target_compile_options(${target} PRIVATE
      -fstack-clash-protection  # 堆栈冲突保护
      -fcf-protection=full      # 控制流完整性保护
  )

  # 内存初始化选项
  if(RENDU_SECURE_INIT)
    target_compile_options(${target} PRIVATE
        -ftrivial-auto-var-init=pattern  # 自动变量模式初始化
    )
  endif()

  # 特定架构优化
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    target_compile_options(${target} PRIVATE
        -march=native
        -mtune=native
    )
  endif()
endfunction()