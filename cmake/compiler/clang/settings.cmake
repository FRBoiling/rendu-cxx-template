#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= 编译器版本检测 =======================#
set(CLANG_EXPECTED_VERSION 11.0.0)

# 特殊处理AppleClang版本差异
if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  set(CLANG_EXPECTED_VERSION 12.0.5)
endif()

print_option("编译器路径" "${CMAKE_CXX_COMPILER}" ${Green})

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS CLANG_EXPECTED_VERSION)
  print_option("期望最低版本" "${CLANG_EXPECTED_VERSION}" ${Red})
  print_option("实际版本" "${CMAKE_CXX_COMPILER_VERSION}" ${Red})
else()
  print_option("期望最低版本" "${CLANG_EXPECTED_VERSION}" ${Green})
  print_option("实际版本" "${CMAKE_CXX_COMPILER_VERSION}" ${Green})
endif()

#======================= 警告系统配置 =======================#
set(CLANG_WARNING_OPTS
    -W                  # 启用所有警告类别
    -Wall               # 标准警告集合
    -Wextra             # 额外警告集合
    -Wimplicit-fallthrough  # 检查未标记的case穿透
    -Winit-self         # 初始化自身变量检测
    -Wno-mismatched-tags    # 忽略类/结构体标签不匹配
    -Woverloaded-virtual    # 虚函数重载检查
    -Wno-missing-field-initializers  # 允许缺失字段初始化
)

# 警告增强配置
target_compile_options(rendu-warning-interface
    INTERFACE
    ${CLANG_WARNING_OPTS}
)

# 警告抑制配置
target_compile_options(rendu-no-warning-interface
    INTERFACE
    -w  # 禁用所有警告
)
print_option("警告系统" "已配置" ${Green})


#======================= 调试分析配置 =======================#
if(WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -g3)
  print_option("调试分析" "已配置三级调试符号(-g3)" ${Green})
endif()

if(BUILD_TIME_ANALYSIS)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -ftime-trace)
  print_option("调试分析" "已启用编译耗时分析(-ftime-trace)" ${Green})
endif()


#======================= 兼容性配置 =======================#
target_compile_options(rendu-compile-option-interface
    INTERFACE
    -Wno-narrowing
    -Wno-deprecated-register
    -Wno-undefined-inline)

#======================= 共享库配置 =======================#
if(BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -fPIC)
  target_link_options(rendu-compile-option-interface
      INTERFACE
      --no-undefined)
  print_option("共享库模式" "已启用(PIC+符号检查)" ${Green})
endif()

#======================= 预编译头优化 =======================#
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-fpch-instantiate-templates CLANG_HAS_PCH_INSTANTIATE_TEMPLATES)
if(CLANG_HAS_PCH_INSTANTIATE_TEMPLATES)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -fpch-instantiate-templates)
  print_option("预编译头文件" "已启用模板预实例化加速PCH" ${Green})
endif()


#======================= 编译功能检测 =======================#
#unset(CLANG_HAVE_PROPER_CHARCONV CACHE)

#check_cxx_source_compiles("
##include <charconv>
#int main() {
#    uint64_t n;
#    return std::from_chars(\"0\", \"0\"+1, n).ec == std::errc();
#}" CLANG_HAVE_PROPER_CHARCONV)
#
#if(NOT CLANG_HAVE_PROPER_CHARCONV)
#  target_compile_definitions(rendu-compile-option-interface
#      INTERFACE
#      RENDU_NEED_CHARCONV_WORKAROUND)
#  message(STATUS "Clang: 已启用64位from_chars兼容方案")
#endif()


#======================= 运行时检测配置 =======================#
function(configure_sanitizer name flags)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      -fno-omit-frame-pointer
      ${flags})
  target_link_options(rendu-compile-option-interface
      INTERFACE
      -fno-omit-frame-pointer
      ${flags})
  message(STATUS "Clang: 已启用${name}")
endfunction()

if(ASAN)
  configure_sanitizer("地址检测器(ASan)" "-fsanitize=address -fsanitize-recover=address")
endif()

if(MSAN)
  configure_sanitizer("内存检测器(MSan)" "-fsanitize=memory -fsanitize-memory-track-origins -mllvm -msan-keep-going=1")
endif()

if(UBSAN)
  configure_sanitizer("未定义行为检测器(UBSan)" "-fsanitize=undefined")
endif()

if(TSAN)
  configure_sanitizer("线程检测器(TSan)" "-fsanitize=thread")
endif()
