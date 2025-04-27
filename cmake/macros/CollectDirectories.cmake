# CollectDirectories.cmake

# ====================================================================
# 智能包含目录收集器
#
# 函数: rendu_collect_include_directories
# 描述: 递归收集指定目录及其子目录作为包含路径，支持精确排除
#
# 参数:
#   output_var    - 输出变量名(存储结果的目录列表)
#   root_dir      - 搜索根目录(绝对路径)
#   [EXCLUDE_DIRS dir1 [dir2...]]  - 要排除的目录列表
#   [EXCLUDE_REGEX regex]          - 排除目录的正则表达式
#
# 用法:
#   rendu_collect_include_directories(MY_INCLUDES
#       ${CMAKE_CURRENT_SOURCE_DIR}
#       EXCLUDE_DIRS 
#           ${CMAKE_CURRENT_SOURCE_DIR}/tests
#           ${CMAKE_BINARY_DIR}
#       EXCLUDE_REGEX ".*/private")
# ====================================================================
# 智能递归收集包含目录，支持排除目录和正则
function(rendu_collect_include_directories output_var root_dir)
    set(options "")
    set(oneValueArgs EXCLUDE_REGEX)
    set(multiValueArgs EXCLUDE_DIRS)
    cmake_parse_arguments(PARSE_ARGV 2 ARG 
        "${options}" 
        "${oneValueArgs}" 
        "${multiValueArgs}")

    # 内部递归函数
    function(_rendu_collect_dirs current_dir result_var)
        set(include_dir TRUE)
        get_filename_component(current_dir_abs "${current_dir}" ABSOLUTE)

        # 检查显式排除目录
        foreach(excl_dir IN LISTS ARG_EXCLUDE_DIRS)
            get_filename_component(excl_dir_abs "${excl_dir}" ABSOLUTE)
            if(current_dir_abs STREQUAL excl_dir_abs)
                set(include_dir FALSE)
                break()
            endif()
        endforeach()

        # 检查正则排除
        if(include_dir AND ARG_EXCLUDE_REGEX)
            if(current_dir_abs MATCHES "${ARG_EXCLUDE_REGEX}")
                set(include_dir FALSE)
            endif()
        endif()

        # 添加当前目录
        if(include_dir)
            set(${result_var} "${current_dir_abs}" PARENT_SCOPE)
        else()
            set(${result_var} "" PARENT_SCOPE)
        endif()

        # 递归子目录
        file(GLOB sub_dirs LIST_DIRECTORIES true "${current_dir}/*")
        foreach(sub_dir IN LISTS sub_dirs)
            if(IS_DIRECTORY "${sub_dir}")
                _rendu_collect_dirs("${sub_dir}" sub_result)
                if(sub_result)
                    set(${result_var} "${${result_var}};${sub_result}" PARENT_SCOPE)
                endif()
            endif()
        endforeach()
    endfunction()

    # 调用递归
    _rendu_collect_dirs("${root_dir}" collected_dirs)
    # 去重
    list(REMOVE_DUPLICATES collected_dirs)
    set(${output_var} ${collected_dirs} PARENT_SCOPE)
endfunction()

