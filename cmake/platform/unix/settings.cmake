#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 设置默认配置目录
if(NOT CONF_DIR)
  set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/etc CACHE PATH "Configuration directory")
  message(STATUS "UNIX: 使用默认配置目录")
endif()

# 配置卸载程序
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
message(STATUS "UNIX: 配置卸载目标")

# 创建卸载目标(允许使用“make uninstall”)
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
)
message(STATUS "UNIX: 创建卸载目标")

if(USE_LD_GOLD)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE LD_VERSION)
  if("${LD_VERSION}" MATCHES "GNU gold")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
    message(STATUS "UNIX: 使用 GNU gold 链接器 ")
  else()
    message(WARNING "UNIX: GNU gold 链接器不可用，使用默认的系统链接器 ")
  endif()
else()
  message(STATUS "UNIX: 使用默认系统连接器 ")
endif()

if(APPLE)
  find_program(HOMEBREW_EXECUTABLE brew)

  if (HOMEBREW_EXECUTABLE)
    # 设置homebrew路径
    message(STATUS "Homebrew 检测到 ${HOMEBREW_EXECUTABLE}")
    execute_process(COMMAND ${HOMEBREW_EXECUTABLE} config OUTPUT_VARIABLE HOMEBREW_STATUS_STR)
    string(REGEX MATCH "HOMEBREW_PREFIX: ([^\n]*)" HOMEBREW_STATUS_STR ${HOMEBREW_STATUS_STR})
    set(HOMEBREW_PREFIX ${CMAKE_MATCH_1})
    message(STATUS "Homebrew 安装在 ${HOMEBREW_PREFIX}")
    set(CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
  endif()
endif()

message(STATUS "UNIX: 编译器检测到: ${CMAKE_C_COMPILER}")
if(CMAKE_C_COMPILER MATCHES "gcc" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake)
elseif(CMAKE_C_COMPILER MATCHES "icc")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/icc/settings.cmake)
elseif(CMAKE_C_COMPILER MATCHES "clang" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
  include(${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake)
else()
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      -D_BUILD_DIRECTIVE="${CMAKE_BUILD_TYPE}")
endif()
