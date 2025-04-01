#**********************************
#  Created by boil on 2025/02/21.
#**********************************

# 检查清单文件存在性
if(NOT EXISTS "@CMAKE_CURRENT_BINARY_DIR@/install_manifest.txt")
  message(FATAL_ERROR "无法找到安装清单文件: \"@CMAKE_CURRENT_BINARY_DIR@/install_manifest.txt\"")
endif()

# 读取并处理清单文件
file(READ "@CMAKE_CURRENT_BINARY_DIR@/install_manifest.txt" files)
string(REPLACE "\n" ";" files "${files}")

# 现代CMake循环语法
foreach(file IN LISTS files)
  message(STATUS "正在卸载：\"${file}\"")

  if(EXISTS "${file}")
    # 使用更现代的execute_process替换exec_program
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E remove "${file}"
        RESULT_VARIABLE rm_retval
    )

    # 简化条件判断
    if(NOT rm_retval EQUAL 0)
      message(FATAL_ERROR "删除文件失败：\"${file}\" (错误码: ${rm_retval})")
    endif()
  else()
    message(STATUS "文件不存在：\"${file}\"")
  endif()
endforeach()

# 可选空目录删除（使用现代语法）
# file(REMOVE_RECURSE "@CMAKE_INSTALL_PREFIX@/yourapp")
