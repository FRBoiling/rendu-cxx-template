# RenduCore - CMake settings for GCC compiler

# =========================
# GCC 编译器相关设置
# =========================

set(RENDU_GCC_EXPECTED_VERSION 11.1.0)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS RENDU_GCC_EXPECTED_VERSION)
  message(FATAL_ERROR "GCC: RenduCore requires version ${RENDU_GCC_EXPECTED_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(STATUS "GCC: Minimum version required is ${RENDU_GCC_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
endif()

target_compile_options(rendu-compile-option-interface
  INTERFACE
    -fno-delete-null-pointer-checks)

if(RENDU_PLATFORM EQUAL 32)
  # 32位系统需要手动开启 SSE2（x64 默认支持）
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -msse2
      -mfpmath=sse)
endif()

if(RENDU_SYSTEM_PROCESSOR MATCHES "x86|amd64")
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      HAVE_SSE2
      __SSE2__)
  message(STATUS "GCC: SFMT enabled, SSE2 flags forced")
endif()

if(RENDU_WITH_WARNINGS)
  target_compile_options(rendu-warning-interface
    INTERFACE
      -W
      -Wall
      -Wextra
      -Winit-self
      -Winvalid-pch
      -Wfatal-errors
      -Woverloaded-virtual
      -Wno-missing-field-initializers # 结构体成员有默认值时该警告无意义
      -Wno-maybe-uninitialized)       # std::optional 场景下该警告易误报
  message(STATUS "GCC: All warnings enabled")
endif()

if(RENDU_WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -g3)
  message(STATUS "GCC: Debug-flags set (-g3)")
endif()

if(RENDU_ASAN)
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
  message(STATUS "GCC: Enabled Address Sanitizer")
endif()

if(BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fPIC
      -Wno-attributes)
  target_compile_options(rendu-hidden-symbols-interface
    INTERFACE
      -fvisibility=hidden)
  # -Wl,--no-undefined 可用于链接阶段强制符号完整，但会影响 PCH
  message(STATUS "GCC: Enabled shared linking")
endif()