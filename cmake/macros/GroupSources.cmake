#**********************************
#  Created by boil on 2022/8/14.
#**********************************

macro(GroupSources dir)
  # 如果没有设置WITH_SOURCE_TREE(空字符串)，则跳过此步骤。
  if(NOT ${WITH_SOURCE_TREE} STREQUAL "")
    # 包括所有头文件和c文件
    file(GLOB_RECURSE elements RELATIVE ${dir} *.h *.hpp *.c *.cpp *.cc)

    foreach(element ${elements})
      # 提取文件名和目录
      get_filename_component(element_name ${element} NAME)
      get_filename_component(element_dir ${element} DIRECTORY)

      if(NOT ${element_dir} STREQUAL "")
        # 如果文件在子目录中，则将其用作源组。
        if(${WITH_SOURCE_TREE} STREQUAL "flat")
          # 只使用第一个子目录构建平面结构
          string(FIND ${element_dir} "/" delemiter_pos)
          if(NOT ${delemiter_pos} EQUAL -1)
            string(SUBSTRING ${element_dir} 0 ${delemiter_pos} group_name)
            source_group("${group_name}" FILES ${dir}/${element})
          else()
            # 建立层次结构
            # 文件在根目录下
            source_group("${element_dir}" FILES ${dir}/${element})
          endif()
        else()
          # 使用完整的层次结构来构建source_groups.
          string(REPLACE "/" "\\" group_name ${element_dir})
          source_group("${group_name}" FILES ${dir}/${element})
        endif()
      else()
        # 如果文件在根目录中，请将其放在根source_group中
        source_group("\\" FILES ${dir}/${element})
      endif()
    endforeach()
  endif()
endmacro()

if(WITH_SOURCE_TREE STREQUAL "hierarchical-folders")
  # 使用文件夹
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()
