# CMakeLists.txt
# 顶层CMake配置文件

# 最低CMake版本要求（必须第一个声明）
cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Utilities")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Project")

include(Policies)
include(GlobalVariables)

# 项目定义
project(rendu
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Modern C++ Project"
    HOMEPAGE_URL "https://github.com/yourusername/rendu"
)

include(BaseHead)

#----------------------- 项目配置 -----------------------
include(BaseTargets)

# 设置项目全局选项
include(Options)

include(CheckPlatform)
include(CheckCompiler)

include(GenRevisionData)

include(OptionsShow)

# 创建排除目录列表（跨平台兼容写法）
set(EXCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/.git"
    "${CMAKE_CURRENT_SOURCE_DIR}/.cache"
    "${CMAKE_CURRENT_SOURCE_DIR}/.vscode"
    "${CMAKE_CURRENT_SOURCE_DIR}/.idea"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-*"  # 增强模式匹配
    "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    "${CMAKE_CURRENT_SOURCE_DIR}/build*"
)

include(CheckBuildDir)

#-------------------------------------------------------------------------------
# 子目录添加
#-------------------------------------------------------------------------------
include(CommonFunction)

rendu_add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    RENDU_CORE_DIRS  # 明确接收变量
    # 排除目录
    ${EXCLUDE_DIRS}
)