# [函数] cmake项目头图案
#function(rendu_project_icon)
#  message("
#  ================================================
#
#  ██████╗ ███████╗███╗   ██╗██████╗ ██╗   ██╗
#  ██╔══██╗██╔════╝████╗  ██║██╔══██╗██║   ██║
#  ██████╔╝█████╗  ██╔██╗ ██║██║  ██║██║   ██║
#  ██╔══██╗██╔══╝  ██║╚██╗██║██║  ██║██║   ██║
#  ██║  ██║███████╗██║ ╚████║██████╔╝╚██████╔╝
#  ╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝╚═════╝  ╚═════╝
#
#  Rendu Core
#  ================================================")

include(ShowMessage)

printf_header("╔══════════════════════════════════════════════════════════╗" ${Blue})
printf_header("║                                                          ║
║       ██████╗ ███████╗███╗   ██╗██████╗ ██╗   ██╗        ║
║       ██╔══██╗██╔════╝████╗  ██║██╔══██╗██║   ██║        ║
║       ██████╔╝█████╗  ██╔██╗ ██║██║  ██║██║   ██║        ║
║       ██╔══██╗██╔══╝  ██║╚██╗██║██║  ██║██║   ██║        ║
║       ██║  ██║███████╗██║ ╚████║██████╔╝╚██████╔╝        ║
║       ╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝╚═════╝  ╚═════╝         ║ " ${Blue})
printf_header("╚══════════════════════════════════════════════════════════╝" ${Blue})

#----------------------- cmake策略配置 -----------------------
print_section("基础配置")

if (CMAKE_VERSION VERSION_LESS 3.15)
  message(FATAL_ERROR "3.15或更高版本要求（当前版本：${CMAKE_VERSION}）")
  print_option("版本" "要求3.15或更高版本（当前版本:${CMAKE_VERSION})" ${Green})
else ()
  print_option("版本" "${CMAKE_VERSION}" ${Green})
endif ()

include(Policies)

print_final_option("策略" "✔ 已配置" ${Cyan})

set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# 自动包含当前二进制目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 生成编译数据库
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

#----------------------- 项目配置 -----------------------
include(ConfigureBaseTargets)







