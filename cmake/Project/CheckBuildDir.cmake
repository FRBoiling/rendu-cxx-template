#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= 构建目录检查 =======================#
# 检查是否在源码目录构建
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  # 检测终端颜色支持
  if(NOT WIN32 AND "$ENV{TERM}" MATCHES "color")
    string(ASCII 27 Esc)
    set(RedF "${Esc}[31m")
    set(ResetColor "${Esc}[0m")
  endif()

  message(FATAL_ERROR "${RedF}
  ██████  ██████  ██████  ███████
 ██      ██    ██ ██   ██ ██
 ██      ██    ██ ██   ██ █████
 ██      ██    ██ ██   ██ ██
  ██████  ██████  ██████  ██

  禁止在源码目录直接构建！
  ================================================
  推荐构建步骤：
  1. 创建独立构建目录：
     mkdir build && cd build
  2. 配置项目：
     cmake ${CMAKE_SOURCE_DIR} ${CMAKE_BUILD_TYPE_OPTION}
  3. 编译项目：
     cmake --build . ${CMAKE_BUILD_FLAGS_OPTION}

  快速命令：
  cmake -B build ${CMAKE_SOURCE_DIR} && cmake --build build

  ${ResetColor}")
endif()

# 二次检查构建残留文件
if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeCache.txt")
  message(FATAL_ERROR "检测到源码目录中存在CMakeCache.txt，请先删除该文件再执行构建")
endif()
