#**********************************
#  Created by boil on 2025/02/21.
#**********************************

function(add_cxx_pch target_list pch_header)
  # 参数校验
  if(NOT pch_header)
    message(FATAL_ERROR "必须指定PCH头文件")
  endif()

  if(NOT EXISTS "${pch_header}")
    message(FATAL_ERROR "PCH头文件不存在: ${pch_header}")
  endif()

  # 遍历目标列表
  foreach(target IN LISTS target_list)
    if(NOT TARGET ${target})
      message(FATAL_ERROR "目标不存在: ${target}")
    endif()

    # 设置编译器特定选项
    if(MSVC)
      target_compile_options(${target} PRIVATE "/Yu${pch_header}")
    endif()

    # 现代CMake预编译头命令
    target_precompile_headers(${target} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${pch_header}>"
    )

    message(STATUS "已为 ${target} 添加PCH支持: ${pch_header}")
  endforeach()
endfunction()

function(reuse_cxx_pch target_list reuse_from_target)
  # 校验源目标
  if(NOT TARGET ${reuse_from_target})
    message(FATAL_ERROR "PCH源目标不存在: ${reuse_from_target}")
  endif()

  # 获取源目标的PCH配置
  get_target_property(pch_header ${reuse_from_target} PRECOMPILE_HEADERS)
  if(NOT pch_header)
    message(FATAL_ERROR "目标 ${reuse_from_target} 未配置PCH")
  endif()

  foreach(target IN LISTS target_list)
    if(NOT TARGET ${target})
      message(FATAL_ERROR "目标不存在: ${target}")
    endif()

    # 跨编译器兼容性处理
    if(MSVC)
      target_compile_options(${target} PRIVATE "/Yu${pch_header}")
    endif()

    target_precompile_headers(${target} REUSE_FROM ${reuse_from_target})
    message(STATUS "目标 ${target} 复用 ${reuse_from_target} 的PCH配置")
  endforeach()
endfunction()
