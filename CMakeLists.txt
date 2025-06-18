cmake_minimum_required(VERSION 3.24)

# 项目基本信息
project(rendu-cxx-template
   LANGUAGES CXX
   VERSION 1.2.3
   DESCRIPTION "RenduCore C++ Template"
   HOMEPAGE_URL "https://github.com/xxx/rendu"
)

# 设置 CMake 宏脚本目录
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")
include(RenduProject)
include(RenduExtensions)
include(CheckPlatform)

# 添加依赖与核心源码（排除 cmake/.git/build 目录）
rendu_add_subdirectories(
  DIR ${CMAKE_CURRENT_SOURCE_DIR}
  EXCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/.git
    ${CMAKE_CURRENT_SOURCE_DIR}/.vscode
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/build
)

# 可选：添加统一的 uninstall 目标（跨平台）
rendu_add_uninstall_target()