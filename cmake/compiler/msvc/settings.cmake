#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= 编译器版本检测 =======================#
set(MSVC_EXPECTED_VERSION 19.32)
set(MSVC_EXPECTED_VERSION_STRING "Microsoft Visual Studio 2022 17.2")

message(STATUS "MSVC: 编译器路径 = ${CMAKE_CXX_COMPILER}")
message(STATUS "MSVC: 期望版本 >= ${MSVC_EXPECTED_VERSION}")

# 仅检查纯MSVC编译器（排除Clang-cl等兼容编译器）
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MSVC_EXPECTED_VERSION)
    message(FATAL_ERROR "MSVC: 不满足最低版本要求\n"
        "需求版本: ${MSVC_EXPECTED_VERSION} (${MSVC_EXPECTED_VERSION_STRING})\n"
        "当前版本: ${CMAKE_CXX_COMPILER_VERSION}\n"
        "编译器路径: ${CMAKE_CXX_COMPILER}")
  else()
    message(STATUS "MSVC: 版本检查通过 (${CMAKE_CXX_COMPILER_VERSION})")
  endif()
endif()


#======================= 核心编译选项 =======================#
target_compile_options(rendu-compile-option-interface
    INTERFACE
    /permissive-  # 严格标准合规模式
    /Zc:throwingNew)  # 优化new运算符

#======================= 架构优化配置 =======================#
if(PLATFORM EQUAL 32)
  # 32位架构特殊优化
  target_compile_options(rendu-compile-option-interface INTERFACE /arch:SSE2)
  target_link_options(rendu-compile-option-interface INTERFACE /LARGEADDRESSAWARE)

  # 调试模式配置
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
  message(STATUS "MSVC: 32位架构已配置SSE2和大地址支持")
endif()

#======================= 并行编译配置 =======================#
if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
  target_compile_options(rendu-compile-option-interface INTERFACE /MP)
  message(STATUS "MSVC: 已启用多线程编译(/MP)")
else()
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      $<$<CONFIG:Debug,RelWithDebInfo>:/FS /Zf>)
  message(STATUS "MSVC: 已启用PDB加速选项(/FS /Zf)")
endif()

#======================= 对象文件优化 =======================#
if((PLATFORM EQUAL 64) OR (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.23026.0) OR BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface INTERFACE /bigobj)
  message(STATUS "MSVC: 已扩展对象文件段限制(/bigobj)")
endif()

#======================= 安全编译定义 =======================#
target_compile_definitions(rendu-compile-option-interface
    INTERFACE
    _CRT_SECURE_CPP_OVERLOAD_STANDALONE_NAMES=1
    _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_WARNINGS
    _USE_MATH_DEFINES)
message(STATUS "MSVC: 安全运行时库配置完成")


#======================= 警告系统配置 =======================#

# 基础警告配置（始终生效的核心警告设置）
set(MSVC_W3_WARNING_OPTS
    /W3             # 警告级别3（建议维护级别）
    /wd4351         # C4351: 数组成员默认初始化行为变更警告
    /wd4091         # C4091: 无变量typedef警告（常见于Windows头文件）
    /we4263         # C4263: 虚函数签名不匹配（强制错误级别）
    /we4264         # C4264: 虚函数隐藏问题（强制错误级别）
)

set(MSVC_W4_WARNING_OPTS
    /W4             # 提升至最高警告级别
    /w44265         # C4265: 类有虚函数但无虚析构函数
    /w44388         # C4388: 有符号/无符号不匹配
    /we4127         # C4127: 条件表达式恒成立
    /we4834         # C4834: 丢弃具有[[nodiscard]]属性的返回值
)

# 扩展警告抑制（仅在开启警告时生效的可选抑制）
set(MSVC_EXTRA_WARNING_OPTS
    /wd4996   # C4996: 函数/变量已弃用警告
    /wd4985   # C4985: 数学符号不一致警告
    /wd4244   # C4244: 浮点/整型隐式转换警告
    /wd4267   # C4267: size_t到较小类型转换警告
    /wd4619   # C4619: 不支持的#pragma指令警告
    /wd4512   # C4512: 无法生成赋值运算符警告
)

#======================= 警告抑制配置 =======================#
target_compile_options(rendu-warning-interface
    INTERFACE
    # 合并警告级别配置（W3和W4互斥）
    $<$<BOOL:${WITH_WARNINGS}>:${MSVC_W4_WARNING_OPTS}>
    $<$<NOT:$<BOOL:${WITH_WARNINGS}>>:${MSVC_W3_WARNING_OPTS}>
)

target_compile_options(rendu-no-warning-interface
    INTERFACE
    $<$<NOT:$<BOOL:${WITH_WARNINGS}>>:
    /W0
    ${MSVC_EXTRA_WARNING_OPTS}
    >
)

# 消息输出需要同步调整
if(WITH_WARNINGS)
  message(STATUS "MSVC: 已启用增强警告级别/W4（包含C4265/C4388/C4127/C4834）")
else()
  message(STATUS "MSVC: 使用基础警告级别/W3")
endif()

#======================= 共享库配置 =======================#
if(BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface
      INTERFACE
      /wd4251   # C4251: DLL接口警告
      /wd4275)  # C4275: 基类DLL接口警告
  message(STATUS "MSVC: 共享库模式特殊配置完成")
endif()

#======================= 高级警告配置 =======================#
target_compile_options(rendu-compile-option-interface
    INTERFACE
    /w15038  # C5038: 成员初始化顺序
    /w34100  # C4100: 未引用形参
    /w34101  # C4101: 未引用局部变量
    /w34189  # C4189: 初始化但未引用变量
    /w34389  # C4389: 符号类型不匹配
    /w35054) # C5054: 枚举类型不匹配

#======================= 地址检测器 =======================#
if(ASAN)
  target_compile_definitions(rendu-compile-option-interface
      INTERFACE
      _DISABLE_STRING_ANNOTATION
      _DISABLE_VECTOR_ANNOTATION)
  target_compile_options(rendu-compile-option-interface INTERFACE /fsanitize=address)
  message(STATUS "MSVC: 地址检测器(ASan)已激活")
endif()

#======================= 链接器优化 =======================#
function(disable_incremental_linking variable)
  string(REGEX REPLACE "/INCREMENTAL( |$)" "" ${variable} "${${variable}}")
  set(${variable} "${${variable}} /INCREMENTAL:NO" PARENT_SCOPE)
  message(STATUS "MSVC: 已禁用增量链接: ${variable}")
endfunction()

# 应用链接优化
disable_incremental_linking(CMAKE_EXE_LINKER_FLAGS_DEBUG)
disable_incremental_linking(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)
disable_incremental_linking(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
disable_incremental_linking(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)

#======================= 构建系统兼容 =======================#
if (NOT MSVC_TOOLSET_VERSION LESS 143)
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/Directory.Build.props" DESTINATION "${CMAKE_BINARY_DIR}")
  message(STATUS "MSVC: 已应用VS2022构建系统兼容配置")
endif()
