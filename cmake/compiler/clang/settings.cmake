# RenduCore - CMake settings for Clang compiler

# =========================
# Clang 编译器相关设置
# =========================

# 检查 Clang 版本（AppleClang 特殊处理）
set(RENDU_CLANG_EXPECTED_VERSION 11.0.0)
if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  # AppleClang 版本号与 LLVM 不一致，12.0.5 对应 LLVM 11
  set(RENDU_CLANG_EXPECTED_VERSION 12.0.5)
endif()

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS RENDU_CLANG_EXPECTED_VERSION)
  message(FATAL_ERROR "Clang: RenduCore requires version ${RENDU_CLANG_EXPECTED_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(STATUS "Clang: Minimum version required is ${RENDU_CLANG_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
endif()

# 警告选项
if(RENDU_WITH_WARNINGS)
  target_compile_options(rendu-warning-interface
    INTERFACE
      -W
      -Wall
      -Wextra
      -Wimplicit-fallthrough
      -Winit-self
      -Wfatal-errors
      -Wno-mismatched-tags
      -Woverloaded-virtual
      -Wno-missing-field-initializers)
  message(STATUS "Clang: All warnings enabled")
endif()

# 调试选项
if(RENDU_WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -g3)
  message(STATUS "Clang: Debug-flags set (-g3)")
endif()

# AddressSanitizer
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
  message(STATUS "Clang: Enabled Address Sanitizer ASan")
endif()

# MemorySanitizer
if(RENDU_MSAN)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=memory
      -fsanitize-memory-track-origins
      -mllvm
      -msan-keep-going=1)
  target_link_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=memory
      -fsanitize-memory-track-origins)
  message(STATUS "Clang: Enabled Memory Sanitizer MSan")
endif()

# UndefinedBehaviorSanitizer
if(RENDU_UBSAN)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=undefined)
  target_link_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=undefined)
  message(STATUS "Clang: Enabled Undefined Behavior Sanitizer UBSan")
endif()

# ThreadSanitizer
if(RENDU_TSAN)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=thread)
  target_link_options(rendu-compile-option-interface
    INTERFACE
      -fno-omit-frame-pointer
      -fsanitize=thread)
  message(STATUS "Clang: Enabled Thread Sanitizer TSan")
endif()

# 构建时间分析
if(RENDU_BUILD_TIME_ANALYSIS)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -ftime-trace)
  message(STATUS "Clang: Enabled build time analysis (-ftime-trace)")
endif()

# 兼容性警告抑制
target_compile_options(rendu-compile-option-interface
  INTERFACE
    -Wno-narrowing
    -Wno-deprecated-register
    -Wno-undefined-inline)

# 共享库相关设置
if(BUILD_SHARED_LIBS)
  # -fPIC 允许静态库用于动态库链接
  # -fvisibility=hidden 防止导出所有符号
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fPIC)
  target_compile_options(rendu-hidden-symbols-interface
    INTERFACE
      -fvisibility=hidden)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --no-undefined")
  message(STATUS "Clang: Disallow undefined symbols")
endif()