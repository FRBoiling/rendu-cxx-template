# cmake/Compiler/Clang.cmake
# ====================================================
# Clang/LLVM编译器专用配置
# 适用于macOS/Linux/Windows(Clang-cl)

# [函数] 应用Clang专用配置
# 参数:
#   target - 要配置的目标名称
function(rendu_compiler_config_clang target)
  # 诊断和静态分析选项
  target_compile_options(${target} PRIVATE
      # 启用所有警告
      -Weverything
      # 排除过于严格的警告
      -Wno-c++98-compat         # C++98兼容性警告
      -Wno-exit-time-destructors # 退出时析构函数警告
      -Wno-global-constructors  # 全局构造函数警告
      # 内存安全选项
      -fsanitize=safe-stack     # 安全堆栈保护
  )

  # LTO优化配置
  if(RENDU_ENABLE_LTO)
    target_compile_options(${target} PRIVATE
        -flto=thin
    )
    target_link_options(${target} PRIVATE
        -flto=thin
        -fuse-ld=lld          # 使用LLD链接器
    )
  endif()

  # Objective-C兼容性配置（macOS专用）
  if(APPLE)
    target_compile_options(${target} PRIVATE
        -Wno-objc-interface-ivars  # Objective-C接口变量警告
        -Wno-deprecated-declarations # 废弃声明警告
    )
  endif()

  # 内存初始化安全选项
  if(RENDU_SECURE_INIT)
    target_compile_options(${target} PRIVATE
        -ftrivial-auto-var-init=pattern
    )
  endif()

  # 控制流完整性配置
  if(RENDU_ENABLE_CFI)
    target_compile_options(${target} PRIVATE
        -fsanitize=cfi
        -fvisibility=hidden
    )
  endif()
endfunction()