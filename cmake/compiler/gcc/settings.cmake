#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= 编译器版本检测 =======================#
set(GCC_EXPECTED_VERSION 11.1.0)

message(STATUS "GCC: 编译器路径 = ${CMAKE_CXX_COMPILER}")
message(STATUS "GCC: 期望版本 >= ${GCC_EXPECTED_VERSION}")
message(STATUS "GCC: 实际版本 = ${CMAKE_CXX_COMPILER_VERSION}")

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_EXPECTED_VERSION)
  message(FATAL_ERROR "GCC: 不满足最低版本要求\n"
      "需求版本: ${GCC_EXPECTED_VERSION}\n"
      "当前版本: ${CMAKE_CXX_COMPILER_VERSION}\n"
      "编译器路径: ${CMAKE_CXX_COMPILER}")
else()
  message(STATUS "GCC: 版本检查通过")
endif()

#======================= 核心编译选项 =======================#
target_compile_options(rendu-compile-option-interface
    INTERFACE
    -fno-delete-null-pointer-checks
    -Wno-attributes
)

#======================= 架构优化 =======================#
if(PLATFORM EQUAL 32)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -msse2
      -mfpmath=sse)
  message(STATUS "GCC: 32位架构已启用SSE2优化")
endif()

#======================= 警告选项配置 =======================#
set(GCC_WARNING_OPTS
    -W
    -Wall
    -Wextra
    -Winit-self
    -Winvalid-pch
    -Wfatal-errors
    -Woverloaded-virtual
    -Wno-missing-field-initializers
    -Wno-maybe-uninitialized
)

if(WITH_WARNINGS)
  target_compile_options(rendu-warning-interface INTERFACE ${GCC_WARNING_OPTS})
  message(STATUS "GCC: 已启用严格警告模式")
endif()

#======================= Sanitizers配置 =======================#
function(configure_sanitizer name flags)
  target_compile_options(rendu-compile-option-interface INTERFACE -fno-omit-frame-pointer ${flags})
  target_link_options(rendu-compile-option-interface INTERFACE -fno-omit-frame-pointer ${flags})
  message(STATUS "GCC: 已启用${name}")
endfunction()

if(ASAN)
  configure_sanitizer("地址检测器(ASan)" "-fsanitize=address -fsanitize-recover=address")
endif()

#======================= 共享库配置 =======================#
if(BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface INTERFACE -fPIC)
  target_link_options(rendu-compile-option-interface INTERFACE --no-undefined)
  message(STATUS "GCC: 共享库模式已启用(PIC+符号检查)")
endif()
