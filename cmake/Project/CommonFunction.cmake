#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#**********************************
# 集成化构建目标宏 (v2.1)
# 包含：库/可执行文件/测试/子目录/安装 五大功能
#**********************************
include(GroupSources)
include(AutoCollect)
include(ShowMessage)

#============================== 基础函数 ==============================#
function(_rendu_setup_target target type)
  print_option("配置目标属性" "${target}(类型: ${type})" ${Green}) # 增加状态提示

  # 预编译头处理
  if (RD_USE_PCH)
    if (RD_PCH_HEADERS)
      print_option("为${target} 添加预编译头" "${RD_PCH_HEADERS}" ${Green})
      add_cxx_pch(${target} "${RD_PCH_HEADERS}")
    else ()
      print_option("${target} 添加预编译头" "RD_USE_PCH 已启用，但未指定 RD_PCH_HEADERS" ${Green})
      message(WARNING "RD_USE_PCH 已启用，但未指定 RD_PCH_HEADERS")  # 增加警告
    endif ()
  endif ()
endfunction()

#------------------------------
function(_rendu_collect_code_files dir hdr_var src_var)
  print_option("正在收集代码文件，目录" "${dir}" ${Green})

  # 空目录校验
  if (NOT IS_DIRECTORY "${dir}")
    print_option("无效源目录: ${dir} (路径不存在或不是目录)" ${Red})
  endif ()

  # 收集头文件和源文件
  rendu_collect_header_files("${dir}" RD_TARGET_HDRS)
  rendu_collect_source_files("${dir}" RD_TARGET_SRCS)

  # 合并去重头文件
  list(APPEND RD_TARGET_HDRS ${${hdr_var}})  # 将外部变量与收集的头文件合并
  list(REMOVE_DUPLICATES RD_TARGET_HDRS)  # 去重

  # 合并去重源文件
  list(APPEND RD_TARGET_SRCS ${${src_var}})  # 将外部变量与收集的源文件合并
  list(REMOVE_DUPLICATES RD_TARGET_SRCS)  # 去重

  list(LENGTH RD_TARGET_HDRS hdr_count)
  list(LENGTH RD_TARGET_SRCS src_count)
  print_option("收集结果" "${hdr_count} 个头文件, ${src_count} 个源文件" ${Green})

  set(${hdr_var} ${RD_TARGET_HDRS} PARENT_SCOPE)  # 将结果传递到父作用域
  set(${src_var} ${RD_TARGET_SRCS} PARENT_SCOPE)  # 将结果传递到父作用域

  # 分组源文件
  RenduGroupSources("${dir}")
endfunction()

#============================== 核心宏 ==============================#
function(rendu_add_library)
  cmake_parse_arguments(RD_LIB
      ""
      "PROJECT;NAME;DIR"
      "HDRS;SRCS;DEPS;LINKOPTS;DEFINES;COPTS"
      ${ARGN}
  )

  # 参数校验
  if (NOT RD_LIB_NAME)
    message(FATAL_ERROR "rendu_add_library 必须指定 NAME 参数")
  endif ()
  if (NOT RD_LIB_DIR)
    message(FATAL_ERROR "rendu_add_library 必须指定 DIR 参数")
  endif ()
  print_section("正在添加库目标: ${RD_LIB_NAME} (目录: ${RD_LIB_DIR})")

  # 收集源文件和头文件
  _rendu_collect_code_files("${RD_LIB_DIR}" RD_LIB_HDRS RD_LIB_SRCS)
  list(LENGTH RD_LIB_HDRS hdr_count)
  list(LENGTH RD_LIB_SRCS src_count)
  #  message(STATUS "收集结果: ${hdr_count} 个头文件, ${src_count} 个源文件")
  # 添加库目标
  set(include_dirs
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # 关键修改点
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  )

  print_option("设置包含目录" "${include_dirs}" ${Green})

  if (src_count EQUAL 0)
    print_option("创建接口库" "${RD_LIB_NAME}" ${Green})
    add_library(${RD_LIB_NAME} INTERFACE)
    # 包含目录设置
    target_include_directories(${RD_LIB_NAME} INTERFACE ${include_dirs})

    # 依赖和链接选项
    if (RD_LIB_DEPS)
      print_option("链接依赖库" "${RD_LIB_DEPS}" ${Green})
      target_link_libraries(${RD_LIB_NAME} INTERFACE ${RD_LIB_DEPS})
    endif ()
    if (RD_LIB_LINKOPTS)
      print_option("添加链接选项" "${RD_LIB_LINKOPTS}" ${Green})
      target_link_libraries(${RD_LIB_NAME} INTERFACE ${RD_LIB_LINKOPTS})
    endif ()

  else ()
    print_option("创建静态/动态库" "${RD_LIB_NAME}" ${Green})
    add_library(${RD_LIB_NAME} ${RD_LIB_SRCS} ${RD_LIB_HDRS})
    # 包含目录设置
    target_include_directories(${RD_LIB_NAME} PUBLIC ${include_dirs})

    # 依赖和链接选项
    if (RD_LIB_DEPS)
      print_option("链接依赖库" "${RD_LIB_DEPS}" ${Green})
      target_link_libraries(${RD_LIB_NAME} PUBLIC ${RD_LIB_DEPS})
    endif ()
    if (RD_LIB_LINKOPTS)
      print_option("添加链接选项" "${RD_LIB_LINKOPTS}" ${Green})
      target_link_libraries(${RD_LIB_NAME} PRIVATE ${RD_LIB_LINKOPTS})
    endif ()

  endif ()



  add_library(${RD_LIB_PROJECT}::${RD_LIB_NAME} ALIAS ${RD_LIB_NAME})

  # 设置目标属性
  _rendu_setup_target(${RD_LIB_NAME} "LIBRARY")

  print_final_option("库目标" "${RD_LIB_PROJECT}::${RD_LIB_NAME} 配置完成" ${Cyan})
  message("")
endfunction()

#------------------------------
function(rendu_add_executable)
  cmake_parse_arguments(RD_EXE
      ""
      "NAME;DIR"
      "HDRS;SRCS;DEPS;LINKOPTS;DEFINES;COPTS"
      ${ARGN}
  )

  # 参数校验
  if (NOT RD_EXE_NAME)
    message(FATAL_ERROR "rendu_add_executable 必须指定 NAME 参数")
  endif ()
  if (NOT RD_EXE_DIR)
    message(FATAL_ERROR "rendu_add_executable 必须指定 DIR 参数")
  endif ()
  print_section("正在添加可执行目标: ${RD_EXE_NAME} (目录: ${RD_EXE_DIR})")

  # 收集源文件和头文件
  _rendu_collect_code_files("${RD_EXE_DIR}" RD_EXE_HDRS RD_EXE_SRCS)

  if (NOT RD_EXE_SRCS)
    print_option("未找到任何源文件" "目录: ${RD_EXE_DIR}" ${Yellow})
  endif ()

  # 添加可执行目标
  add_executable(${RD_EXE_NAME} ${RD_EXE_SRCS} ${RD_EXE_HDRS})
  print_option("创建可执行目标" "${RD_EXE_NAME}" ${Green})

  # 编译定义
  if (RD_EXE_DEFINES)
    print_option("添加编译定义" "${RD_EXE_DEFINES}" ${Green})
    target_compile_definitions(${RD_EXE_NAME} PRIVATE ${RD_EXE_DEFINES})
  endif ()

  # 依赖和链接选项
  if (RD_EXE_DEPS)
    print_option("链接依赖库" "${RD_EXE_DEPS}" ${Green})
    target_link_libraries(${RD_EXE_NAME} PUBLIC ${RD_EXE_DEPS})
  endif ()
  if (RD_EXE_LINKOPTS)
    print_option("添加链接选项" "${RD_EXE_LINKOPTS}" ${Green})
    target_link_libraries(${RD_EXE_NAME} PRIVATE ${RD_EXE_LINKOPTS})
  endif ()

  # 设置目标属性
  _rendu_setup_target(${RD_EXE_NAME} "EXECUTABLE")
  print_final_option("可执行目标" "${RD_EXE_NAME}配置完成" ${Cyan})
  message("")
endfunction()

#------------------------------
function(rendu_add_test)
  cmake_parse_arguments(RD_TEST
      ""
      "NAME;DIR"
      "HDRS;SRCS;DEPS;LINKOPTS;DEFINES;COPTS"
      ${ARGN}
  )

  # 参数校验
  if (NOT RD_TEST_NAME)
    message(FATAL_ERROR "rendu_add_test 必须指定 NAME 参数")
  endif ()
  if (NOT RD_TEST_DIR)
    message(FATAL_ERROR "rendu_add_test 必须指定 DIR 参数")
  endif ()
  message(STATUS "正在添加测试目标: ${RD_TEST_NAME} (目录: ${RD_TEST_DIR})")

  # 添加 GTest 依赖
  list(APPEND RD_TEST_DEPS GTest::gmock GTest::gtest_main)
  message(STATUS "添加测试依赖: GTest::gmock, GTest::gtest_main")

  # 创建可执行目标
  rendu_add_executable(NAME ${RD_TEST_NAME} DIR ${RD_TEST_DIR} DEPS ${RD_TEST_DEPS})

  # 添加测试
  add_test(NAME ${RD_TEST_NAME}
      COMMAND ${RD_TEST_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  message(STATUS "添加测试: ${RD_TEST_NAME} (工作目录: ${CMAKE_CURRENT_BINARY_DIR})")

  # 设置目标属性
  _rendu_setup_target(${RD_TEST_NAME} "TEST")
  message(STATUS "测试目标 ${RD_TEST_NAME} 配置完成\n")
endfunction()

#============================== 目录管理 ==============================#
# 将所有子目录添加。忽略可变参数中列出的完全限定目录。
# Use it like:
# rendu_add_subdirectory(
#   ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
#   ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/platform)
#
function(rendu_add_subdirectory current_dir)
  get_filename_component(current_dir "${current_dir}" ABSOLUTE)
#  message(STATUS "正在处理子目录: ${current_dir}")  # 目录处理入口提示

  # 参数规范化处理
  set(processed_args)
  foreach (arg IN LISTS ARGN)
    get_filename_component(abs_arg "${arg}" ABSOLUTE)
    list(APPEND processed_args "${abs_arg}")
  endforeach ()

  # 目录存在性校验
  if (NOT IS_DIRECTORY "${current_dir}")
    message(FATAL_ERROR "无效工作目录: ${current_dir} (路径不存在或不可访问)")
  endif ()

  # 参数分离
  set(exclude_dirs ${processed_args})

  # 检查排除列表
  list(FIND exclude_dirs "${current_dir}" is_excluded)
  if (is_excluded EQUAL -1)
    file(GLOB sub_dirs
        CONFIGURE_DEPENDS
        LIST_DIRECTORIES true
        "${current_dir}/*"
    )

    foreach (sub_dir IN LISTS sub_dirs)
      if (IS_DIRECTORY "${sub_dir}")
        list(FIND exclude_dirs "${sub_dir}" is_sub_excluded)
        if (is_sub_excluded EQUAL -1)
          get_filename_component(dir_name "${sub_dir}" NAME)
          if (EXISTS "${sub_dir}/CMakeLists.txt")
            #            message(STATUS "添加子目录: ${dir_name}")  # 子目录添加提示
            add_subdirectory("${sub_dir}")
          else ()
#            message(STATUS "跳过目录（无 CMakeLists.txt）: ${sub_dir}")  # 明确跳过原因
          endif ()
        else ()
          #          message(STATUS "排除目录: ${sub_dir}")  # 显式提示排除操作
        endif ()
      endif ()
    endforeach ()

  else ()
    message(STATUS "排除根目录: ${current_dir}")  # 显式提示排除根目录
  endif ()
endfunction()

#============================== 安装配置 ==============================#

function(_rendu_common_install target component)
  # 公共安装逻辑
  include(GNUInstallDirs)
  install(TARGETS ${target} EXPORT RenduTargets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${component}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${component}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${component}
  )

  # 统一版本管理
  if(NOT DEFINED RD_VERSION)
    set(RD_VERSION ${PROJECT_VERSION})
  endif()
  set_target_properties(${target} PROPERTIES
      VERSION ${RD_VERSION}
      SOVERSION ${RD_VERSION_MAJOR}
  )
endfunction()

function(rendu_install_library)
  cmake_parse_arguments(RD_INSTALL
      ""
      "TARGET;EXPORT_HEADER_DIR"
      "COMPONENTS"
      ${ARGN}
  )

  # 参数校验
  if(NOT RD_INSTALL_TARGET)
    message(FATAL_ERROR "必须指定 TARGET 参数")
  endif()
  if(NOT TARGET ${RD_INSTALL_TARGET})
    message(FATAL_ERROR "目标不存在: ${RD_INSTALL_TARGET}")
  endif()

  # 版本继承
  if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION 1.0.0)
  endif()
  set(RD_INSTALL_VERSION ${PROJECT_VERSION})
  set(RD_INSTALL_LIB_VERSION ${PROJECT_VERSION_MAJOR})

  # 生成导出头文件
  string(TOUPPER "${RD_INSTALL_TARGET}_EXPORT" EXPORT_MACRO)
  set(export_header "${RD_INSTALL_EXPORT_HEADER_DIR}/${RD_INSTALL_TARGET}_export.h")
  generate_export_header(${RD_INSTALL_TARGET}
      EXPORT_MACRO_NAME ${EXPORT_MACRO}
      EXPORT_FILE_NAME ${export_header}
  )

  # 核心安装逻辑
  include(GNUInstallDirs)
  install(TARGETS ${RD_INSTALL_TARGET} EXPORT RenduTargets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      COMPONENT ${RD_INSTALL_TARGET}
  )

  # 头文件安装
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      COMPONENT ${RD_INSTALL_TARGET}
      FILES_MATCHING PATTERN "*.h*"
  )

  # 导出头文件安装
  install(FILES ${export_header}
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${RD_INSTALL_TARGET}"
  )

  # 导出配置
  install(EXPORT RenduTargets
      FILE "${PROJECT_NAME}Targets.cmake"
      NAMESPACE Rendu::
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

  # 生成配置文件
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
      "${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
      INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

  message(STATUS "安装配置完成：${RD_INSTALL_TARGET} -> ${CMAKE_INSTALL_PREFIX}")
endfunction()


function(rendu_export_and_install_lib component)
  # 参数校验
  if(NOT component)
    message(FATAL_ERROR "必须指定组件名称参数")
  endif()

  # 生成导出宏定义
  string(TOUPPER "${component}_EXPORT" export_macro_name)

  # 包含目录配置
  target_include_directories(${component} INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
      $<INSTALL_INTERFACE:include>)

  # 生成导出头文件
  generate_export_header(
      ${component}
      EXPORT_MACRO_NAME ${export_macro_name}
      EXPORT_FILE_NAME "caf/detail/${component}_export.hpp"
  )

  # 目标属性配置
  set_target_properties(${component} PROPERTIES
      EXPORT_NAME ${component}
      SOVERSION ${RD_VERSION}
      VERSION ${RD_LIB_VERSION}
      OUTPUT_NAME caf_${component}
  )

  # 调用公共安装函数
  _rendu_common_install(${component} ${component})

  # 安装头文件目录
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/caf"
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
      COMPONENT ${component}
      FILES_MATCHING PATTERN "*.hpp"
  )

  # 安装导出头文件
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/caf/detail/${component}_export.hpp"
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/caf/detail/"
  )
endfunction()