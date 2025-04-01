#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#[[
  将语义版本号转换为可比较的整数值
  参数:
    requested_version - 输入版本号 (格式: x.y.z 或 x.y)
    normalized_version - 输出整数 (格式: 10000*x + 100*y + z)
  异常:
    版本组件大于100时触发FATAL_ERROR
]]
macro(normalize_version requested_version normalized_version)
  # 参数空值校验
  if(NOT requested_version)
    message(FATAL_ERROR "必须提供请求版本号")
  endif()

  # 正则匹配版本号组件
  string(REGEX MATCH "([0-9]+)\\.([0-9]+)(\\.([0-9]+))?" _ "${requested_version}")
  if(NOT CMAKE_MATCH_0)
    message(FATAL_ERROR "无效版本格式: ${requested_version}")
  endif()

  # 提取版本三组件
  set(_major ${CMAKE_MATCH_1})  # 主版本号
  set(_minor ${CMAKE_MATCH_2})  # 次版本号
  set(_patch ${CMAKE_MATCH_4})  # 修订号 (可选)

  # 处理缺失的修订号
  if(NOT _patch)
    set(_patch 0)
  endif()

  # 校验版本号组件范围
  foreach(comp IN ITEMS _major _minor _patch)
    if(${${comp}} GREATER 100)
      message(FATAL_ERROR "版本号组件超过最大值100: ${requested_version}")
    endif()
  endforeach()

  # 生成标准化版本值
  math(EXPR ${normalized_version} "${_major}*10000 + ${_minor}*100 + ${_patch}")
endmacro()

#[[
  检查数值范围 [lower, upper)
  参数:
    lower - 范围下限 (包含)
    value - 待检查值
    upper - 范围上限 (不包含)
    result - 检查结果 (TRUE/FALSE)
]]
macro(check_range_inclusive_lower lower value upper result)
  if(${value} LESS ${lower} OR ${value} GREATER_EQUAL ${upper})
    set(${result} FALSE)
  else()
    set(${result} TRUE)
  endif()
endmacro()

#[[
  验证发现版本是否满足最低要求
  参数:
    min_version   - 最低要求版本
    found_version - 实际发现版本
    result_var    - 结果变量 (TRUE/FALSE)
]]
macro(ensure_version min_version found_version result_var)
  # 输入校验
  foreach(arg IN ITEMS min_version found_version)
    if(NOT ${arg})
      message(FATAL_ERROR "${arg} 不能为空")
    endif()
  endforeach()

  # 版本标准化处理
  normalize_version(${min_version} req_num)
  normalize_version(${found_version} found_num)

  # 版本比较逻辑
  set(${result_var} FALSE)
  if(found_num GREATER_EQUAL req_num)
    set(${result_var} TRUE)
  endif()
endmacro()

#[[
  验证版本是否在指定范围内 [min, max)
  参数:
    min_version   - 最低版本 (包含)
    found_version - 实际版本
    max_version   - 最高版本 (不包含)
    result_var    - 结果变量 (TRUE/FALSE)
]]
macro(ensure_version_range min_version found_version max_version result_var)
  # 三重参数校验
  foreach(arg IN ITEMS min_version found_version max_version)
    if(NOT ${arg})
      message(FATAL_ERROR "${arg} 不能为空")
    endif()
  endforeach()

  # 标准化所有版本号
  normalize_version(${min_version} min_num)
  normalize_version(${found_version} found_num)
  normalize_version(${max_version} max_num)

  # 范围检查执行
  check_range_inclusive_lower(${min_num} ${found_num} ${max_num} ${result_var})
endmacro()
