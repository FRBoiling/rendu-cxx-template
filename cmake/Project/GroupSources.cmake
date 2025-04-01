#**********************************
#  Created by boil on 2025/02/21.
#**********************************

#[[
  组织源代码分组结构
  参数:
    dir - 需要分组的源代码根目录
  模式:
    flat              - 扁平结构(仅显示一级子目录)
    hierarchical      - 层次结构(保留完整目录树)
    hierarchical-folders - 启用IDE文件夹视图
]]
macro(RenduGroupSources dir)
  if(NOT ${RENDU_SOURCE_LAYOUT} STREQUAL "")
    # 获取所有C/C++源文件（包含子目录）
    file(GLOB_RECURSE elements RELATIVE "${dir}"
        "*.h" "*.hpp" "*.c" "*.cpp" "*.cc")

    set(_group_mode "${RENDU_SOURCE_LAYOUT}")

    foreach(element IN LISTS elements)
      get_filename_component(relative_dir "${element}" DIRECTORY)

      # 处理根目录下的文件
      if("${relative_dir}" STREQUAL "")
        source_group("\\" FILES "${dir}/${element}")
        continue()
      endif()

      # 根据分组模式路由处理逻辑
      if(_group_mode STREQUAL "flat")
        _process_flat_group("${relative_dir}" "${dir}/${element}")
      else()
        _process_hierarchical_group("${relative_dir}" "${dir}/${element}" "${_group_mode}")
      endif()
    endforeach()
  endif()
endmacro()

# 扁平化分组处理器
# 将路径截取到第一个子目录级别
# 示例: src/core/system -> src
function(_process_flat_group relative_dir full_path)
  # 查找第一个目录分隔符
  string(FIND "${relative_dir}" "/" first_slash)
  if(NOT first_slash EQUAL -1)
    # 截取首层目录名
    string(SUBSTRING "${relative_dir}" 0 ${first_slash} group_name)
    source_group("${group_name}" FILES "${full_path}")
  else()
    # 无子目录时直接使用当前目录名
    source_group("${relative_dir}" FILES "${full_path}")
  endif()
endfunction()

# 层次结构分组处理器
# 保留完整目录结构，适配Visual Studio解决方案视图
function(_process_hierarchical_group relative_dir full_path mode)
  if(mode MATCHES "hierarchical")
    # 转换路径分隔符为Windows风格
    string(REPLACE "/" "\\" group_name "${relative_dir}")
    source_group("${group_name}" FILES "${full_path}")
  endif()
endfunction()

# 启用IDE文件夹视图支持 (Visual Studio/Xcode)
if(RENDU_SOURCE_LAYOUT STREQUAL "hierarchical-folders")
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()
