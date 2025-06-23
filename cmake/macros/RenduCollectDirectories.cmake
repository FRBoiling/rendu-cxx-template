# CollectDirectories.cmake
# 智能包含目录收集器（仅收集第一层子目录，支持排除）
#
# 用法示例：
# rendu_collect_include_directories(MY_INCLUDES
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     EXCLUDE_DIRS 
#         ${CMAKE_CURRENT_SOURCE_DIR}/tests
#         ${CMAKE_BINARY_DIR}
#     EXCLUDE_REGEX ".*/private"
# )
#
# 说明：
# - MY_INCLUDES：输出变量名，存储收集到的第一层子目录列表
# - ${CMAKE_CURRENT_SOURCE_DIR}：要收集的根目录
# - EXCLUDE_DIRS：可选，指定要排除的目录（第一层子目录中）
# - EXCLUDE_REGEX：可选，指定要排除的目录正则表达式

function(rendu_collect_include_directories output_var root_dir)
    set(options)
    set(oneValueArgs EXCLUDE_REGEX)
    set(multiValueArgs EXCLUDE_DIRS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(GLOB children RELATIVE "${root_dir}" "${root_dir}/*")
    set(result "")
    foreach (child ${children})
        set(child_path "${root_dir}/${child}")
        if (IS_DIRECTORY "${child_path}")
            # 检查是否在 EXCLUDE_DIRS
            set(excluded_dir FALSE)
            foreach (ex_dir ${ARG_EXCLUDE_DIRS})
                if ("${child_path}" STREQUAL "${ex_dir}")
                    set(excluded_dir TRUE)
                endif ()
            endforeach ()
            # 检查是否匹配 EXCLUDE_REGEX
            set(excluded_regex FALSE)
            if (ARG_EXCLUDE_REGEX)
                if ("${child_path}" MATCHES "${ARG_EXCLUDE_REGEX}")
                    set(excluded_regex TRUE)
                endif ()
            endif ()
            # 满足条件才加入
            if (NOT excluded_dir AND NOT excluded_regex)
                list(APPEND result "${child_path}")
            endif ()
        endif ()
    endforeach ()
    set(${output_var} ${result} PARENT_SCOPE)
endfunction()
