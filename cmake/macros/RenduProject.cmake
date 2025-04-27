# 设置作者和许可证等元信息（可选，便于后续扩展）
set(PROJECT_AUTHOR "boil" CACHE STRING "Project author")
set(PROJECT_LICENSE "MIT" CACHE STRING "Project license")

# 禁止源码目录变更和源码内构建
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# CMake 策略设置
if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW)
endif()
if(POLICY CMP0153)
  cmake_policy(SET CMP0153 NEW)
endif()

# 默认构建类型为 Release（如未指定）
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
  set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build type" FORCE)
endif()   


# 平台与基础目标配置
include(ConfigureRenduTargets)

# 常用模块与配置
include(cmake/options.cmake)

# 关闭预编译头（仅开发者调试用）
if(NOPCH)
  set(RENDU_USE_COREPCH OFF)
  set(RENDU_USE_SCRIPTPCH OFF)
endif()


# 预编译头支持
find_package(PCHSupport)


# Git 支持
if(NOT RENDU_WITHOUT_GIT)
  find_package(Git 1.7)
endif()


# 获取源码树的版本号与哈希
include(cmake/genrev.cmake)

# 显示构建选项
include(cmake/showoptions.cmake)
