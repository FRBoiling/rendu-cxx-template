# cmake/Utils/Colors.cmake
#[[
终端颜色管理模块
版本: 2.1
优化点:
  - 增强Windows终端检测
  - 统一变量命名规范
  - 改进颜色禁用逻辑
]]

# 检测有效的颜色支持环境
if(DISABLE_COLOR)
  set(_color_supported FALSE)
elseif(CMAKE_COLOR_MAKEFILE OR NOT CMAKE_VERSION VERSION_LESS 3.24)
  # 现代CMake版本或强制启用颜色
  set(_color_supported TRUE)
elseif(CMAKE_TERMINAL_IS_COLOR_CAPABLE)
  # 终端明确声明支持颜色
  set(_color_supported TRUE)
elseif(WIN32 AND (CMAKE_GENERATOR MATCHES "Ninja" OR CMAKE_GENERATOR MATCHES "Visual Studio"))
  # Windows现代终端(PowerShell/新版CMD)
  set(_color_supported TRUE)
else()
  set(_color_supported FALSE)
endif()

if(_color_supported)
  string(ASCII 27 Esc)
  set(ColorReset  "${Esc}[0m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(Bold        "${Esc}[1m")
else()
  set(ColorReset  "")
  set(Red         "")
  set(Green       "")
  set(Yellow      "")
  set(Blue        "")
  set(Magenta     "")
  set(Cyan        "")
  set(Bold        "")
endif()