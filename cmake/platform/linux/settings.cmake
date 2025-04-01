#**********************************
#  Created by boil on 2025/02/21.
#**********************************

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

message(STATUS "  处理器架构 - ${RENDU_SYSTEM_PROCESSOR}(${CMAKE_SYSTEM_PROCESSOR})")

# 设置默认配置文件目录
if(NOT CONF_DIR)
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "配置文件目录")
  message(CHECK_START "LINUX: 配置文件目录检测")
  message(CHECK_PASS "LINUX: 使用默认配置路径 [${CONF_DIR}]")
endif()

# 配置卸载目标
configure_file("${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
message(CHECK_START "LINUX: 生成卸载脚本")
message(CHECK_PASS "LINUX: 生成路径 ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

# 创建卸载目标
add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
)
message(CHECK_START "LINUX: 创建卸载指令")
message(CHECK_PASS "LINUX: 支持 make uninstall 命令")

# 设置二进制输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/lib")
message(CHECK_START "LINUX: 配置输出目录")
message(CHECK_PASS "LINUX: 可执行文件 → ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(CHECK_PASS "LINUX: 共享库文件 → ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Gold 链接器配置
if(RENDU_USE_GOLD_LINKER)
  message(CHECK_START "LINUX: 检测GNU gold链接器")
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version
      ERROR_QUIET OUTPUT_VARIABLE LD_VERSION)
  if("${LD_VERSION}" MATCHES "GNU gold")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
    message(CHECK_PASS "LINUX: 已启用 (版本: ${LD_VERSION})")
  else()
    message(CHECK_FAIL "LINUX: 未找到 (当前链接器: ${CMAKE_LINKER})")
  endif()
endif()

message(CHECK_START "LINUX: 编译器信息检测")
message(CHECK_PASS "LINUX: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(CHECK_PASS "LINUX: 编译器路径 ${CMAKE_C_COMPILER}")

