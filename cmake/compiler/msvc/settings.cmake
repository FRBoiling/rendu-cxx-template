#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 设置可执行二进制文件(支持DLL的平台上的.exe文件和.dll文件)的输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(MSVC_EXPECTED_VERSION 19.24)
set(MSVC_EXPECTED_VERSION_STRING "Microsoft Visual Studio 2019 16.4")

# 如果是MSVC这个文件也被编译器使用，但报告他们自己的版本号-不要版本检查他们
if(NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MSVC_EXPECTED_VERSION)
    message(FATAL_ERROR "MSVC: This project requires version ${MSVC_EXPECTED_VERSION} (${MSVC_EXPECTED_VERSION_STRING}) to build but found ${CMAKE_CXX_COMPILER_VERSION}")
  else()
    message(STATUS "MSVC: Minimum version required is ${MSVC_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
  endif()
endif()

# CMake默认设置警告标志，但是我们手动管理不同的核心和依赖目标
string(REGEX REPLACE "/W[0-4] " "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# 搜索两次
# 一次是 /W参数后面
# 一次是 行结尾
# 通过正则表达式确保没有 \b
string(REGEX REPLACE "/W[0-4]$" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/W[0-4] " "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REGEX REPLACE "/W[0-4]$" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

target_compile_options(rendu-warning-interface
  INTERFACE
    /W3)

# 禁用允许模式，使MSVC更急于拒绝其他编译器不接受的代码
target_compile_options(rendu-compile-option-interface
  INTERFACE
    /permissive-)

# 设置静态库等的输出路径(注释掉了-这里只是一个示例)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(PLATFORM EQUAL 64)
  # 这个定义对于解决这里描述的智能感知错误是必要的:http://tinyurl.com/2cb428。语法高亮显示对于正确的调试器功能非常重要。
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      -D_WIN64)

  message(STATUS "MSVC: 64-bit 系统, 执行-D_WIN64参数")

else()
  # 将32位可执行文件标记为大地址感知，这样它们就可以使用>2GB地址空间
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
  message(STATUS "MSVC: 启用 大地址感知")

  target_compile_options(rendu-compile-option-interface
    INTERFACE
      /arch:SSE2)
  message(STATUS "MSVC: 启用 SSE2 支持")

  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
  message(STATUS "MSVC: 禁用 用于调试构建的安全异常处理程序")
endif()

# 设置build-directive(在core中用来告诉我们使用了哪种build类型)
# msbuild/devev 不要设置CMAKE_MAKE_PROGRAM，你可以在下拉菜单中选择构建类型
if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      -D_BUILD_DIRECTIVE="$(ConfigurationName)")
else()
  # 所有类似于make的生成器都是这样(nmake, ninja)
  target_compile_definitions(rendu-compile-option-interface
    INTERFACE
      -D_BUILD_DIRECTIVE="$<CONFIG>")
endif()

# VS上的多线程编译
target_compile_options(rendu-compile-option-interface
  INTERFACE
    /MP)

if((PLATFORM EQUAL 64) OR (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.23026.0) OR BUILD_SHARED_LIBS)
  # 启用扩展对象支持
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      /bigobj)

  message(STATUS "MSVC: Enabled increased number of sections in object files")
endif()

# /Zc:throwingNew.
#当您在命令行上指定 Zc:throwingNew 时，它会指示编译器假定程序最终将与一致操作符new实现相链接，并且可以从程序中省略所有这些额外的空检查。
#http://blogs.msdn.com/b/vcblog/archive/2015/08/06/new-in-vs-2015-zc-throwingnew.aspx
if(NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.23026.0))
  # makes this flag a requirement to build RD at all
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      /Zc:throwingNew)
endif()

# 定义 _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES - 通过将strcpy调用更改为strcpy_s来消除警告，这可防止缓冲区溢出
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES)
message(STATUS "MSVC: 重载标准名称")

# 忽略有关较旧、安全性较低的函数的警告
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -D_CRT_SECURE_NO_WARNINGS)
message(STATUS "MSVC: 禁用 NON-SECURE 警告 ")

# 忽略有关POSIX弃用的警告
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -D_CRT_NONSTDC_NO_WARNINGS)

message(STATUS "MSVC: 禁用 POSIX 警告 ")

# 忽略特定警告
target_compile_options(rendu-compile-option-interface
  INTERFACE
    /wd4351  # C4351: new behavior: elements of array 'x' will be default initialized
    /wd4091) # C4091: 'typedef ': ignored on left of '' when no variable is declared

if(NOT WITH_WARNINGS)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      /wd4996  # C4996 deprecation
      /wd4985  # C4985 'symbol-name': attributes not present on previous declaration.
      /wd4244  # C4244 'argument' : conversion from 'type1' to 'type2', possible loss of data
      /wd4267  # C4267 'var' : conversion from 'size_t' to 'type', possible loss of data
      /wd4619  # C4619 #pragma warning : there is no warning number 'number'
      /wd4512) # C4512 'class' : assignment operator could not be generated

  message(STATUS "MSVC: 已禁用泛型编译时警告")
endif()

if(BUILD_SHARED_LIBS)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      /wd4251  # C4251: needs to have dll-interface to be used by clients of class '...'
      /wd4275) # C4275: non dll-interface class ...' used as base for dll-interface class '...'

  message(STATUS "MSVC: 启用 shared lib 连接")
endif()

# 将为其他编译器启用的一些警告从级别4移至级别3
target_compile_options(rendu-compile-option-interface
  INTERFACE
    /w34100  # C4100 'identifier' : unreferenced formal parameter
    /w34101  # C4101: 'identifier' : unreferenced local variable
    /w34189  # C4189: 'identifier' : local variable is initialized but not referenced
    /w34389) # C4189: 'equality-operator' : signed/unsigned mismatch

# 启用以下警告并将其视为错误，以便轻松检测虚拟函数签名故障：
# 'function' : 成员函数不重写任何基类虚拟成员函数
# 'virtual_function' : 没有可用于基类中的虚拟成员函数的重写；函数被隐藏
target_compile_options(rendu-compile-option-interface
  INTERFACE
    /we4263
    /we4264)

# 在调试版本中禁用增量链接。
# 以防止链接卡住(这可能会在较新的VS版本中修复)。
macro(DisableIncrementalLinking variable)
  string(REGEX REPLACE "/INCREMENTAL *" "" ${variable} "${${variable}}")
  set(${variable} "${${variable}} /INCREMENTAL:NO")
endmacro()

# 禁用Visual Studio 2022生成过程管理
# 同时编译 num_cpus*num_project 是必要的，因为当前实现中存在一个错误，使得脚本在游戏项目构建完成后同时只构建单个文件
if (NOT MSVC_TOOLSET_VERSION LESS 143)
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/Directory.Build.props" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

DisableIncrementalLinking(CMAKE_EXE_LINKER_FLAGS_DEBUG)
DisableIncrementalLinking(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)
DisableIncrementalLinking(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
DisableIncrementalLinking(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)
