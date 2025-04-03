#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#======================= 公共工具函数 =======================#
function(_collect_files variable patterns)
#  message(STATUS "正在收集文件，模式: ${patterns}")  # 提示当前操作

  # 使用 GLOB_RECURSE 收集文件
  file(GLOB_RECURSE TEMP_FILES
      CONFIGURE_DEPENDS  # 支持增量构建
      ${patterns}
  )

  # 检查是否找到文件
  if(NOT TEMP_FILES)
#    message(WARNING "未找到匹配的文件，模式: ${patterns}")  # 警告提示
    return()  # 如果没有文件，直接返回
  endif()

  # 去重
  list(REMOVE_DUPLICATES TEMP_FILES)

  # 追加到变量
  list(APPEND ${variable} ${TEMP_FILES})

  # 将变量传递到父作用域
  set(${variable} ${${variable}} PARENT_SCOPE)

  # 提示收集结果
  list(LENGTH TEMP_FILES file_count)
#  message(STATUS "收集到 ${file_count} 个文件，模式: ${patterns}")
endfunction()

function(_check_exclusion current_dir excluded result)
  # 修复list FIND参数：原代码错误展开变量
  list(FIND excluded "${current_dir}" index)
  if (index EQUAL -1)
    set(${result} TRUE PARENT_SCOPE)
  else ()
    set(${result} FALSE PARENT_SCOPE)
  endif ()
endfunction()


#======================= 主收集函数 =======================#
# 将所有源文件收集到给定变量中，用于包含子目录中的所有源。忽略可变参数中列出的完全限定目录。
# Use it like:
# rendu_collect_all_files(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(rendu_collect_all_files current_dir variable)
  _check_exclusion(${current_dir} "${ARGN}" should_include)
  if (should_include)
    _collect_files(${variable}
        "${current_dir}/*.c"
        "${current_dir}/*.cc"
        "${current_dir}/*.cpp"
        "${current_dir}/*.pb.[ch]*"
        "${current_dir}/*.h"
        "${current_dir}/*.hh"
        "${current_dir}/*.hpp"
        "${current_dir}/*.ipp"
        "${current_dir}/*.inc"
    )
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction()

#======================= 其他收集函数 =======================#
# 将所有源文件收集到给定变量中，用于包含子目录中的所有源。忽略可变参数中列出的完全限定目录。
# Use it like:
# rendu_collect_source_files(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(rendu_collect_source_files current_dir variable)
#  message(STATUS "正在收集源文件，目录: ${current_dir}")  # 提示当前操作

  # 将当前目录转换为绝对路径
  get_filename_component(current_dir_abs "${current_dir}" ABSOLUTE)
#  message(STATUS "绝对路径: ${current_dir_abs}")  # 显示绝对路径

  # 检查是否排除当前目录
  _check_exclusion(${current_dir_abs} "${ARGN}" should_include)

  if (should_include)
    # 定义源文件模式
    set(source_file_patterns
        "${current_dir_abs}/*.c"
        "${current_dir_abs}/*.cc"
        "${current_dir_abs}/*.cpp"
        "${current_dir_abs}/*.pb.cc"
    )
#    message(STATUS "源文件模式: ${source_file_patterns}")  # 显示文件模式
    # 收集文件
    _collect_files(${variable} "${source_file_patterns}")

    # 将结果传递到父作用域
    set(${variable} ${${variable}} PARENT_SCOPE)
#    message(STATUS "源文件收集完成，变量: ${variable}")  # 提示完成
  else ()
    message(STATUS "目录被排除: ${current_dir_abs}")  # 提示排除
  endif ()
endfunction()

#------------------------------
function(rendu_collect_header_files current_dir variable)
#  message(STATUS "正在收集头文件，目录: ${current_dir}")  # 提示当前操作

  # 将当前目录转换为绝对路径
  get_filename_component(current_dir_abs "${current_dir}" ABSOLUTE)
#  message(STATUS "绝对路径: ${current_dir_abs}")  # 显示绝对路径

  # 检查是否排除当前目录
  _check_exclusion(${current_dir_abs} "${ARGN}" should_include)

  if (should_include)
    # 定义文件模式
    set(file_patterns
        "${current_dir_abs}/*.h"
        "${current_dir_abs}/*.hh"
        "${current_dir_abs}/*.hpp"
        "${current_dir_abs}/*.ipp"
        "${current_dir_abs}/*.pb.h"
        "${current_dir_abs}/*.inc"
    )
#    message(STATUS "头文件模式: ${file_patterns}")  # 显示文件模式

    # 收集文件
    _collect_files(${variable} "${file_patterns}")

    # 将结果传递到父作用域
    set(${variable} ${${variable}} PARENT_SCOPE)
#    message(STATUS "头文件收集完成，变量: ${variable}")  # 提示完成
  else ()
    message(STATUS "目录被排除: ${current_dir_abs}")  # 提示排除
  endif ()
endfunction()


#======================= Proto文件收集 =======================#
function(rendu_collect_proto_files current_dir variable)
  _check_exclusion(${current_dir} "${ARGN}" should_include)
  if (should_include)
    _collect_files(${variable}
        "${current_dir}/*.proto"
    )
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction()

#======================= 包含目录收集 =======================#
# 收集所有子目录到给定的变量，用于包含所有子目录。忽略可变参数中列出的完全限定目录。
# Use it like:
# rendu_collect_include_directories(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PUBLIC_INCLUDES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#

function(rendu_collect_include_directories current_dir variable)
  _check_exclusion(${current_dir} "${ARGN}" should_include)
  if (should_include)
    # 获取所有直接子目录
    file(GLOB subdirs
        CONFIGURE_DEPENDS
        LIST_DIRECTORIES true
        "${current_dir}/*"
    )

    # 过滤排除目录
    set(filtered_dirs)
    foreach (dir IN LISTS subdirs)
      _check_exclusion(${dir} "${ARGN}" should_include)
      if (should_include)
        list(APPEND filtered_dirs ${dir})
      endif ()
    endforeach ()

    list(APPEND ${variable} ${filtered_dirs})
    set(${variable} ${${variable}} PARENT_SCOPE)

    #    list(LENGTH filtered_dirs dir_count)
    #    message(STATUS "已过滤包含目录: ${current_dir} (保留${dir_count}个)")
  endif ()
endfunction()





