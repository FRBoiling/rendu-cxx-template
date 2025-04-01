# cmake/Toolchains/MacOS-AppleSilicon.cmake
# ====================================================
# Apple Silicon原生编译配置
# 适用于M1/M2芯片的优化配置

include_guard(GLOBAL)

# [选项] 通用配置
set(APPLE_SILICON_ARCH "arm64" CACHE STRING "目标架构 (arm64)")
set(OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "最低macOS版本")

function(rendu_toolchain_init)
  # 架构配置
  set(CMAKE_OSX_ARCHITECTURES ${APPLE_SILICON_ARCH})
  set(CMAKE_OSX_DEPLOYMENT_TARGET ${OSX_DEPLOYMENT_TARGET})

  # 编译器优化
  add_compile_options(
      -mcpu=apple-m1  # M1专属优化
      -mtune=apple-m1
      -fforce-emit-vtables
  )

  # 链接器配置
  add_link_options(
      -ld_classic  # 兼容旧版链接器
      -Wl,-no_weak_imports
  )

  # 系统框架路径
  set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

  # 通用二进制支持
  if(RENDU_UNIVERSAL_BUILD)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
    add_compile_options(-arch arm64 -arch x86_64)
    add_link_options(-arch arm64 -arch x86_64)
  endif()
endfunction()