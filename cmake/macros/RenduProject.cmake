# 设置作者和许可证等元信息（可选，便于后续扩展）
set(PROJECT_AUTHOR "boil" CACHE STRING "Project author")
set(PROJECT_LICENSE "MIT" CACHE STRING "Project license")

# 禁止源码目录变更和源码内构建
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

#  CMake 策略
include(RenduCMakePolicies)
rendu_set_policies()

# 默认构建类型为 Release（如未指定）
if (CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
    set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif ()
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build type" FORCE)
endif ()

# 平台与基础目标配置
include(RenduLogging)
include(RenduConfigureTargets)
include(RenduPackageManager)

# 常用模块与配置
include(cmake/options.cmake)

# 获取源码树的版本号与哈希
include(cmake/genrev.cmake)

# 显示构建选项
include(cmake/showoptions.cmake)
