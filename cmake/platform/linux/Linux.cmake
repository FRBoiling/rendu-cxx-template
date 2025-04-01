# cmake/Platform/Linux.cmake
# ===============================================

# 平台标识
set(RENDU_PLATFORM_DEFINES
    RENDU_PLATFORM_LINUX
    _GNU_SOURCE
    CACHE INTERNAL "Linux平台预处理定义"
)

# 系统库配置
find_package(Threads REQUIRED)
list(APPEND RENDU_PLATFORM_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    dl
)

# 编译器配置
function(rendu_platform_compiler_config target)
  target_compile_options(${target} PRIVATE
      -fPIC
      -fstack-protector-strong
  )
endfunction()

# 链接器配置
function(rendu_platform_linker_config target)
  target_link_options(${target} PRIVATE
      -Wl,-z,now
      -Wl,-z,relro
      -Wl,--as-needed
  )
endfunction()

# 安全配置
function(rendu_platform_security_config target)
  target_compile_definitions(${target} PRIVATE
      _FORTIFY_SOURCE=2
  )
  target_link_options(${target} PRIVATE
      -Wl,-z,noexecstack
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