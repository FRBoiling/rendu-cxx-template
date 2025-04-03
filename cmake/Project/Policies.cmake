#**********************************
#  Created by boil on 2025/02/21.
#**********************************

# [策略] 设置推荐的CMake策略
# 版本检查：最低要求3.15（对应VS2019）
include(ShowMessage)

print_section("CMake策略配置")

if (CMAKE_VERSION VERSION_LESS 3.15)
  message(FATAL_ERROR "3.15或更高版本要求（当前版本：${CMAKE_VERSION}）")
  print_option("版本" "要求3.15或更高版本（当前版本:${CMAKE_VERSION})" ${Green})
else ()
  print_option("版本" "${CMAKE_VERSION}" ${Green})
endif ()

#----------------------- 核心策略设置 -----------------------
cmake_policy(SET CMP0005 NEW)
if(CMAKE_VERSION VERSION_LESS "3.16.0")
  cmake_policy(SET CMP0043 OLD) # Disable 'Ignore COMPILE_DEFINITIONS_<Config> properties'
else()
  cmake_policy(SET CMP0043 NEW) # Cotire isn't used so set to NEW
endif()
cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if(SOME_STRING_VARIABLE MATCHES "MSVC") as if(SOME_STRING_VARIABLE MATCHES "1")

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW) # find_package() uses <PackageName>_ROOT variables
endif()

print_final_option("策略" "✔ 已配置" ${Cyan})