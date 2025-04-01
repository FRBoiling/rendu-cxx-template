#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= MSVC生成器特殊处理 =======================#
# 第二阶段：通过处理cmake -A参数指定的平台覆盖
if (CMAKE_GENERATOR_PLATFORM)
  if (CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
    set(RENDU_SYSTEM_PROCESSOR "x86")
  elseif (CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set(RENDU_SYSTEM_PROCESSOR "amd64")
  elseif (CMAKE_GENERATOR_PLATFORM STREQUAL "ARM")
    set(RENDU_SYSTEM_PROCESSOR "arm")
  elseif (CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
    set(RENDU_SYSTEM_PROCESSOR "arm64")
  endif ()
else ()
  message(STATUS "  未指定平台覆盖")
endif ()

message(STATUS "  处理器架构 - ${RENDU_SYSTEM_PROCESSOR}(${CMAKE_SYSTEM_PROCESSOR})")

# Windows平台编译定义
target_compile_definitions(rendu-compile-option-interface
    INTERFACE
    _WIN32_WINNT=0x0A00                     # Windows 10最低版本
    NTDDI_VERSION=0x0A000007                # 19H1版本号(1903)
    WIN32_LEAN_AND_MEAN                     # 精简Windows头文件
    NOMINMAX                                # 禁用MIN/MAX宏定义
    RENDU_REQUIRED_WINDOWS_BUILD=18362)   # 最低构建版本号
message(STATUS "WINDOWS: 已配置平台编译选项")

# 设置统一输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/lib")
message(STATUS "WINDOWS: 运行时输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# 添加应用程序清单文件
target_sources(rendu-core-interface
    INTERFACE
    $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CMAKE_SOURCE_DIR}/cmake/platform/win/WindowsSettings.manifest>)
message(STATUS "WINDOWS: 已添加应用程序清单文件")

# 编译器检测
message(STATUS "WINDOWS: 检测到编译器 ${CMAKE_C_COMPILER}")
