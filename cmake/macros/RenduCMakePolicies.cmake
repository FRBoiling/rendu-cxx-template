#-------------------------------------------------------------------------------
# 宏定义: rendu_set_policies
# 功能: 设置项目编译所需的 CMake 策略
# 描述:
#   此宏设置一系列 CMake 策略以确保项目编译环境的一致性和正确性。
#   这些策略主要影响依赖处理、选项缓存行为和版本控制等方面。
#-------------------------------------------------------------------------------
macro(rendu_set_policies)
    # CMP0144: 允许 find_dependency() 继承父项目的 REQUIRED/QUIET/NO_MODULE 等参数
    # 这确保子项目查找依赖时遵循父项目的查找规则
    if (POLICY CMP0144)
        cmake_policy(SET CMP0144 NEW)
    endif ()

    # CMP0153: 允许 find_dependency() 继承父项目的 NO_POLICY_SCOPE 参数
    # 确保策略作用域在依赖查找过程中正确传递
    if (POLICY CMP0153)
        cmake_policy(SET CMP0153 NEW)
    endif ()

    # CMP0077: 允许 option() 命令影响缓存变量，而不强制缓存
    # 这使得选项可以在父项目中被覆盖，增强配置灵活性
    cmake_policy(SET CMP0077 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

    # CMP0126: 允许 set(... CACHE ...) 命令不缓存变量
    # 确保变量设置行为符合现代 CMake 实践
    if (POLICY CMP0126)
        cmake_policy(SET CMP0126 NEW)
        set(CMAKE_POLICY_DEFAULT_CMP0126 NEW)
    endif ()

    # CMP0135: 使用下载时间作为时间戳，便于 URL 变更时正确重建
    # 确保外部依赖在 URL 或版本变更时能被正确重新下载
    if (POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
        set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
    endif ()

    # CMP0150: 相对 git 仓库路径以父项目 remote 为基准
    # 确保相对路径的 git 仓库引用能正确解析
    if (POLICY CMP0150)
        cmake_policy(SET CMP0150 NEW)
        set(CMAKE_POLICY_DEFAULT_CMP0150 NEW)
    endif ()
endmacro()