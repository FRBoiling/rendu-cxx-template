#**********************************
#  Created by boil on 2025/02/21.
#**********************************

# [策略] 设置推荐的 CMake 策略
# 版本检查：最低要求 3.15（对应 VS2019）
include(ShowMessage)

print_section("CMake 策略配置")

# 检查 CMake 版本
if (CMAKE_VERSION VERSION_LESS 3.15)
  print_option("版本" "要求 3.15 或更高版本（当前版本: ${CMAKE_VERSION})" ${Red})
  message(FATAL_ERROR "CMake 版本过低，最低要求 3.15，当前版本为 ${CMAKE_VERSION}")
else()
  print_option("版本" "${CMAKE_VERSION}" ${Green})
endif()

#----------------------- 核心策略设置 -----------------------
# 引用未定义的宏时扩展为空字符串
cmake_policy(SET CMP0005 NEW)

# 根据 CMake 版本设置 CMP0043 策略
if (CMAKE_VERSION VERSION_LESS 3.16)
  cmake_policy(SET CMP0043 OLD) # 禁用 'Ignore COMPILE_DEFINITIONS_<Config> properties'
else()
  cmake_policy(SET CMP0043 NEW) # 未使用 Cotire，设置为 NEW
endif()

# 仅在未加引号时将 if() 参数解释为变量或关键字
cmake_policy(SET CMP0054 NEW)

# find_package() 使用 <PackageName>_ROOT 变量
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

# 打印最终策略配置状态
print_final_option("策略" "✔ 已配置" ${Cyan})