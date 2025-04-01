#**********************************
#  Created by boil on 2025/02/21.
#**********************************

include(ShowMessage)

#======================= 处理器架构检测 =======================#
# 通过系统信息检测架构
if (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64|x86_64|AMD64")
  set(RENDU_SYSTEM_PROCESSOR "amd64")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM|aarch)64$")
  set(RENDU_SYSTEM_PROCESSOR "arm64")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM)$")
  set(RENDU_SYSTEM_PROCESSOR "arm")
else ()
  set(RENDU_SYSTEM_PROCESSOR "x86")
endif ()

print_option("处理器架构" "${RENDU_SYSTEM_PROCESSOR}(${CMAKE_SYSTEM_PROCESSOR}" ${Green})

# 设置默认配置文件目录
if (NOT CONF_DIR)
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "配置文件目录")
  print_option("使用默认配置目录" "${CONF_DIR}" ${Green})
endif ()

# 配置卸载目标
configure_file("${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)

print_option("配置卸载目标" "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" ${Green})

# 创建卸载目标（支持 make uninstall 命令）
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
)

print_option("创建卸载指令" " 支持 make uninstall 命令" ${Green})

# 设置二进制输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/bin")
print_option("可执行文件输出目录" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" ${Green})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/lib")
print_option("共享库文件输出目录" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" ${Green})

# Homebrew 路径配置
find_program(HOMEBREW_EXECUTABLE brew)
if (HOMEBREW_EXECUTABLE)
  execute_process(COMMAND ${HOMEBREW_EXECUTABLE} config OUTPUT_VARIABLE HOMEBREW_STATUS_STR)
  string(REGEX MATCH "HOMEBREW_PREFIX: ([^\n]*)" HOMEBREW_STATUS_STR ${HOMEBREW_STATUS_STR})
  set(HOMEBREW_PREFIX ${CMAKE_MATCH_1})
  set(CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")

  print_option("Homebrew路径" "[${HOMEBREW_PREFIX}]" ${Green})
else ()
  print_option("Homebrew路径" "未检测到Homebrew" ${Yellow})
endif ()

