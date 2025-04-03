#**********************************
#  Created by boil on 2025/02/21.
#**********************************
#  cmake/Project/GlobalVariables.cmake
# ======================================================
# 全局构建配置
# ======================================================

print_section("CMake全局构建配置")
# [安全防护]
# 禁止构建时修改源码树（防止误操作污染源码）
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
# 强制要求外部构建（禁止在源码目录内构建）
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# [动态库路径控制]
set(CMAKE_SKIP_BUILD_RPATH OFF)          # 构建阶段生成RPATH（确保临时构建可运行）
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)  # 构建时不使用安装RPATH（隔离开发/生产环境）
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")  # 安装后库搜索路径
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)  # 自动追加链接器路径到RPATH

# [开发便利性]
# 自动添加当前二进制目录到头文件搜索路径（便于访问生成的头文件）
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 生成compile_commands.json（支持clangd/LSP等工具）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
  set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

print_option("构建类型" "${CMAKE_BUILD_TYPE}" ${Green})

#----------------------- 标准规范配置 -----------------------
# C++20标准配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
print_final_option("C++标准" "已启用C++${CMAKE_CXX_STANDARD}标准(禁用编译器扩展)" ${Green})









