#**********************************
#  Created by boil on 2022/8/14.
#**********************************

include(CheckCXXSourceCompiles)

# 设置build-directive(在core中用来告诉我们使用了哪种build类型)
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -D_BUILD_DIRECTIVE="$<CONFIG>")

set(GCC_EXPECTED_VERSION 8.3.0)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_EXPECTED_VERSION)
  message(FATAL_ERROR "GCC: This project requires version ${GCC_EXPECTED_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(STATUS "GCC: Minimum version required is ${GCC_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
endif()

if(PLATFORM EQUAL 32)
  # 32位系统上需要启用SSE2 (x64标准)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -msse2
      -mfpmath=sse)
endif()
if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      -DHAVE_SSE2
      -D__SSE2__)
  message(STATUS "GCC: SFMT 启用, 强制启用 SSE2")
endif()

if(WITH_WARNINGS)
  target_compile_options(rendu-warning-interface
    INTERFACE
      -W
      -Wall
      -Wextra
      -Winit-self
      -Winvalid-pch
      -Wfatal-errors
      -Woverloaded-virtual)

  message(STATUS "GCC: 启用用所有警告")
endif()

if(WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -g3)

  message(STATUS "GCC: Debug-flags set (-g3)")
endif()

if(ASAN)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=address
      -fsanitize-recover=address
      -fsanitize-address-use-after-scope)

  target_link_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=address
      -fsanitize-recover=address
      -fsanitize-address-use-after-scope)

  message(STATUS "GCC: 启用 Address Sanitizer")
endif()

if(BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fPIC
      -Wno-attributes)

  target_compile_options(rendu-hidden-symbols-interface
    INTERFACE
      -fvisibility=hidden)

  # 当缺少  RENDU_*_API 宏时，应中断构建，但它会报告预编译头中缺少引用。
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--no-undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined")

  message(STATUS "GCC: 已启用 SHARED库 链接")
endif()
