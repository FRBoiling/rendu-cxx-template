include(CheckCXXSourceCompiles)

# 设置build-directive(在core中用来告诉我们使用了哪种build类型)
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -D_BUILD_DIRECTIVE="$<CONFIG>")

set(CLANG_EXPECTED_VERSION 7.0.0)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS CLANG_EXPECTED_VERSION)
  message(FATAL_ERROR "Clang: This project requires version ${CLANG_EXPECTED_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(STATUS "Clang: Minimum version required is ${CLANG_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
endif()

#测试clang-7中导致64位from_chars链接失败的bug(在某些配置中)
#如果clang需求被更新到>=clang-8，可以移除这个检查，以及src/common/utilities/StringConvert.h中相关的ifdef块
include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
#include <charconv>
#include <cstdint>

int main()
{
    uint64_t n;
    char const c[] = \"0\";
    std::from_chars(c, c+1, n);
    return static_cast<int>(n);
}
" CLANG_HAVE_PROPER_CHARCONV)

if (NOT CLANG_HAVE_PROPER_CHARCONV)
  message(STATUS "Clang: Detected from_chars bug for 64-bit integers, workaround enabled")
  target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -DRENDU_NEED_CHARCONV_WORKAROUND)
endif()

if(WITH_WARNINGS)
  target_compile_options(rendu-warning-interface
    INTERFACE
      -W
      -Wall
      -Wextra
      -Wimplicit-fallthrough
      -Winit-self
      -Wfatal-errors
      -Wno-mismatched-tags
      -Woverloaded-virtual)

  message(STATUS "Clang: All warnings enabled")
endif()

if(WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -g3)

  message(STATUS "Clang: Debug-flags set (-g3)")
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

  message(STATUS "Clang: Enabled Address Sanitizer ASan")
endif()

if(MSAN)
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

if(UBSAN)
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

if(TSAN)
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

if(BUILD_TIME_ANALYSIS)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -ftime-trace)

  message(STATUS "Clang: Enabled build time analysis (-ftime-trace)")
endif()

# -wno-narrow，需要在g3d中抑制警告。
# -wno-deprecdeprece-register，在Unix系统上，需要# 来抑制185个gsoap警告。
# -Wno-deprecated-copy，需要在g3d中抑制警告
target_compile_options(rendu-compile-option-interface
  INTERFACE
    -Wno-narrowing
    -Wno-deprecated-register)

if(BUILD_SHARED_LIBS)
  # -fPIC，需要允许在共享库中进行静态链接。
  # -fvisibility=hidden，将默认可见性设置为hidden，以防止导出所有的符号。
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fPIC)

  target_compile_options(rendu-hidden-symbols-interface
    INTERFACE
      -fvisibility=hidden)

  # --no-undefined 当有未定义符号时抛出错误
  # (由于缺少RENDU_*_API宏导致)。
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --no-undefined")
  message(STATUS "Clang: Disallow undefined symbols")
endif()

# 通过在PCH生成期间强制模板实例化来加速PCH构建
set(CMAKE_REQUIRED_FLAGS "-fpch-instantiate-templates")
check_cxx_source_compiles("int main() { return 0; }" CLANG_HAS_PCH_INSTANTIATE_TEMPLATES)
unset(CMAKE_REQUIRED_FLAGS)
if(CLANG_HAS_PCH_INSTANTIATE_TEMPLATES)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -fpch-instantiate-templates)
endif()
