# cmake/Platform/Windows.cmake
# ===============================================

# 平台标识
set(RENDU_PLATFORM_DEFINES
    RENDU_PLATFORM_WINDOWS
    WIN32_LEAN_AND_MEAN
    _WIN32_WINNT=0x0A00
    NOMINMAX
    CACHE INTERNAL "Windows平台预处理定义"
)

# 系统库配置
list(APPEND RENDU_PLATFORM_LIBS
    ws2_32
    advapi32
    shell32
)

# 编译器配置
function(rendu_platform_compiler_config target)
  target_compile_options(${target} PRIVATE
      /MP           # 多进程编译
      /guard:cf     # 控制流防护
      /sdl          # 安全开发生命周期检查
  )
endfunction()

# 链接器配置
function(rendu_platform_linker_config target)
  target_link_options(${target} PRIVATE
      /DYNAMICBASE  # ASLR支持
      /NXCOMPAT     # DEP支持
  )
endfunction()

# 安全配置
function(rendu_platform_security_config target)
  target_compile_definitions(${target} PRIVATE
      _SCL_SECURE_NO_WARNINGS
  )
endfunction()

# 安装配置
function(rendu_platform_install_config target)
  install(TARGETS ${target}
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
  )
endfunction()