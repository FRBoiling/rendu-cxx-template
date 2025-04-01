# cmake/Toolchains/Windows11.cmake
# ====================================================
# Windows 11专用工具链
# 支持x64/ARM64架构

include_guard(GLOBAL)

# [选项] 架构配置
set(WIN11_ARCH "x64" CACHE STRING "目标架构 (x64, ARM64)")
set(WIN_SDK_VERSION "10.0.22000.0" CACHE STRING "Windows SDK版本")

function(rendu_toolchain_init)
  # 架构配置
  if(WIN11_ARCH STREQUAL "ARM64")
    set(CMAKE_GENERATOR_PLATFORM ARM64)
    set(CMAKE_VS_PLATFORM_NAME "ARM64")
  else()
    set(CMAKE_GENERATOR_PLATFORM x64)
  endif()

  # MSVC配置
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

  # 安全特性
  add_compile_options(
      /guard:cf          # 控制流防护
      /Qspectre          # Spectre缓解
      /ZH:SHA_256        # 调试信息哈希
  )

  # ARM64特殊配置
  if(WIN11_ARCH STREQUAL "ARM64")
    add_compile_options(
        /arm64EC       # ARM64EC兼容模式
        /D_ARM64_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1
    )
  endif()

  # 最新Windows SDK
  set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION ${WIN_SDK_VERSION})
endfunction()