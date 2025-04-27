#====================================================================
# RenduCore 核心构建配置脚本
# 
# 文件名：      ConfigureRenduTargets.cmake
# 描述：        定义项目的基础接口目标，用于统一配置编译选项、特性和符号可见性等
# 
# 架构概述：
#   1. 基础编译选项接口 (rendu-compile-option-interface)
#   2. 项目特性接口 (rendu-feature-interface)
#   3. 警告级别接口 (rendu-warning-interface)
#   4. 默认配置接口 (rendu-default-interface)
#   5. 静默警告接口 (rendu-no-warning-interface)
#   6. 符号隐藏接口 (rendu-hidden-symbols-interface)
#   7. 依赖项专用接口 (rendu-dependency-interface)
#   8. 核心模块接口 (rendu-core-interface)
# 
# 使用说明：
#   1. 项目目标应链接相应的接口库以继承配置：
#       - 内部模块：PUBLIC rendu-core-interface
#       - 第三方依赖：PUBLIC rendu-dependency-interface
#   2. 平台相关配置应在单独脚本中扩展 rendu-warning-interface
#====================================================================
#------------------------
# 1. 核心编译选项配置接口
#------------------------
# 创建接口库：定义所有目标共享的基础编译选项
add_library(rendu-compile-option-interface INTERFACE)
# 禁用编译器扩展（使用严格的标准模式）
set(CMAKE_CXX_EXTENSIONS OFF)      # 使用 -std=c++20 而非 -std=gnu++20
# 设置C++语言标准为C++20
set(CMAKE_CXX_STANDARD 20)
# 添加配置相关预定义宏（自动设置_BUILD_DIRECTIVE=Debug/Release等）
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    _BUILD_DIRECTIVE="$<CONFIG>")

#------------------------
# 2. 项目特性接口库
#------------------------
# 预留接口：用于集中管理项目级特性宏和配置开关
add_library(rendu-feature-interface INTERFACE)
# 示例使用方式（实际添加时取消注释）:
# target_compile_definitions(rendu-feature-interface INTERFACE
#   ENABLE_ADVANCED_FEATURES
#   USE_OPTIMIZED_ALGORITHM)

#------------------------
# 3. 警告级别接口库
#------------------------
# 平台相关配置：由具体平台的CMake脚本设置编译器特定警告选项
add_library(rendu-warning-interface INTERFACE)
# 示例（平台脚本中设置）:
# if(UNIX)
#   target_compile_options(rendu-warning-interface INTERFACE -Wall -Wextra)
# endif()

#------------------------
# 4. 默认接口库（核心聚合接口）
#------------------------
# 聚合常用基础接口，作为项目默认配置组合
add_library(rendu-default-interface INTERFACE)
# 继承编译选项接口和特性接口
target_link_libraries(rendu-default-interface
  INTERFACE
    rendu-compile-option-interface
    rendu-feature-interface)

#------------------------
# 5. 静默警告接口库
#------------------------
# 特殊用途：为需要禁用警告的目标提供统一配置
add_library(rendu-no-warning-interface INTERFACE)
# 编译器差异处理：MSVC使用/W0，其他编译器使用-w
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(rendu-no-warning-interface INTERFACE /W0)
else()
  target_compile_options(rendu-no-warning-interface INTERFACE -w)
endif()

#------------------------
# 6. 符号可见性控制接口
#------------------------
# 默认隐藏符号配置（避免暴露内部实现细节）
add_library(rendu-hidden-symbols-interface INTERFACE)
# 编译器特定实现（示例）:
# if(NOT WIN32)
#   target_compile_options(rendu-hidden-symbols-interface INTERFACE -fvisibility=hidden)
# endif()

#------------------------
# 7. 依赖项接口库
#------------------------
# 依赖库专用配置：聚合适合第三方库的保守配置
add_library(rendu-dependency-interface INTERFACE)
# 组合默认接口 + 禁用警告 + 符号隐藏
target_link_libraries(rendu-dependency-interface
  INTERFACE
    rendu-default-interface
    rendu-no-warning-interface
    rendu-hidden-symbols-interface)

#------------------------
# 8. 核心模块接口库
#------------------------
# 核心组件专用配置：聚合项目内部核心目标的配置
add_library(rendu-core-interface INTERFACE)
# 组合默认接口 + 项目标准警告级别
target_link_libraries(rendu-core-interface
  INTERFACE
    rendu-default-interface
    rendu-warning-interface)  # 应用项目预设的警告级别