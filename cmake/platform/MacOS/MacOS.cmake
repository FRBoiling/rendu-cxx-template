# cmake/Platform/MacOS.cmake
# ===============================================
# macOS平台专属配置（现代CMake优化版）
# 平台配置主函数
function(rendu_configure_macos_platform)
  # 平台标识定义
  set(RENDU_PLATFORM_DEFINES
      RENDU_PLATFORM_MACOS
      _DARWIN_C_SOURCE=1
      _APPLE_C_SOURCE=1
      PARENT_SCOPE  # 现在在函数内部正确使用PARENT_SCOPE
  )

  # 系统库配置（按需启用）
  if(RENDU_ENABLE_MACOS_FRAMEWORKS)
    find_library(SECURITY_LIB Security REQUIRED)
    find_library(SYSTEMCONFIGURATION_LIB SystemConfiguration REQUIRED)
    set(RENDU_PLATFORM_LIBS
        ${SECURITY_LIB}
        ${SYSTEMCONFIGURATION_LIB}
        "-framework Foundation"
        PARENT_SCOPE
    )
  endif()
endfunction()

# 编译器配置（兼容多版本）
function(rendu_platform_compiler_config target)
  target_compile_options(${target} PRIVATE
      $<$<COMPILE_LANGUAGE:CXX>:-fstack-protector-strong>
      $<$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},13.0>:-ftrivial-auto-var-init=pattern>
      $<$<VERSION_LESS:${CMAKE_CXX_COMPILER_VERSION},13.0>:-ftrivial-auto-var-init=zero>
  )
endfunction()

# 链接器配置（增强安全性）
function(rendu_platform_linker_config target)
  target_link_options(${target} PRIVATE
      -Wl,-dead_strip
      -Wl,-headerpad_max_install_names
      -Wl,-no_weak_imports
      $<$<BOOL:${RENDU_ENABLE_HARDENED_RUNTIME}>:-Wl,-hardened_runtime>
  )
endfunction()

# 安全配置（分级策略）
function(rendu_platform_security_config target)
  set(safety_level 2)
  if(RENDU_ENABLE_HARDENED_RUNTIME)
    set(safety_level 3)
  endif()

  target_compile_definitions(${target} PRIVATE
      _FORTIFY_SOURCE=${safety_level}
  )
  target_link_options(${target} PRIVATE
      -Wl,-bind_at_load
  )
endfunction()

# 安装配置（多架构支持）
function(rendu_platform_install_config target)
  install(TARGETS ${target}
      BUNDLE DESTINATION . COMPONENT Runtime
      RUNTIME DESTINATION bin COMPONENT Runtime
      LIBRARY DESTINATION lib COMPONENT Development
      ARCHIVE DESTINATION lib COMPONENT Development
      FRAMEWORK DESTINATION Frameworks COMPONENT Development
  )
endfunction()

# 平台扩展功能（与项目选项集成）
macro(rendu_platform_extensions)
  # 代码签名配置
  option(RENDU_ENABLE_CODE_SIGN "Enable macOS code signing" OFF)
  if(RENDU_ENABLE_CODE_SIGN)
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY
        $<IF:$<CONFIG:Release>,Apple Distribution,Apple Development>
    )
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual")
    set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "" CACHE STRING "开发者团队ID")
  endif()

  # 通用二进制配置
  if(NOT CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "目标架构")
    if(RENDU_ENABLE_UNIVERSAL_BUILD)
      set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
    endif()
  endif()

  # 部署目标版本管理
  if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "最低支持版本")
  endif()
endmacro()

rendu_configure_macos_platform()
