# cmake/Toolchains/Ubuntu22.cmake
# ====================================================
# Ubuntu 22.04专用配置
# 支持x64/ARM64架构

include_guard(GLOBAL)

# [选项] 架构配置
set(UBUNTU_ARCH "x86_64" CACHE STRING "目标架构 (x86_64, aarch64)")
set(UBUNTU_GLIBC_VERSION "2.35" CACHE STRING "GLIBC版本")

function(rendu_toolchain_init)
  # 架构配置
  if(UBUNTU_ARCH STREQUAL "aarch64")
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
  else()
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
  endif()

  # 安全加固选项
  add_compile_options(
      -fstack-protector-strong
      -D_FORTIFY_SOURCE=3
      -Wformat -Wformat-security
  )

  # 链接器配置
  add_link_options(
      -Wl,-z,now
      -Wl,-z,relro
      -Wl,--as-needed
  )

  # 多架构支持
  if(UBUNTU_ARCH STREQUAL "aarch64")
    add_compile_options(-march=armv8-a+crc+crypto)
    add_link_options(-march=armv8-a+crc+crypto)
  else()
    add_compile_options(-march=x86-64-v3)
  endif()

  # GLIBC版本控制
  add_compile_options(-D__GLIBC_MINOR__=${UBUNTU_GLIBC_VERSION})
endfunction()