# 源码组织与智能收集
include(RenduCollectDirectories)
include(RenduCollectFiles)

# =============================================
# 函数: rendu_add_subdirectories
# 描述: 增强版 add_subdirectory，自动添加指定目录下所有子文件夹为子项目，支持排除指定目录
#
# 参数:
#   DIR           - 根目录（必填）
#   EXCLUDE_DIRS  - 需要排除的子目录列表（可选，绝对或相对路径均可）
#
# 用法示例:
# rendu_add_subdirectories(
#     DIR
#           ${CMAKE_CURRENT_SOURCE_DIR}
#     EXCLUDE_DIRS
#           ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
#           ${CMAKE_CURRENT_SOURCE_DIR}/tests
# )
# =============================================

function(rendu_add_subdirectories)
    set(options RECURSIVE)
    set(oneValueArgs DIR)
    set(multiValueArgs EXCLUDE_DIRS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT ARG_DIR)
        rendu_log_fatal("rendu_add_subdirectories: 必须指定 DIR")
    endif ()
    if (NOT IS_DIRECTORY "${ARG_DIR}")
        rendu_log_warn("rendu_add_subdirectories: 目录不存在: ${ARG_DIR}")
        return()
    endif ()

    # 规范化排除目录为绝对路径并去除末尾斜杠
    set(EXCLUDE_ABS_DIRS "")
    foreach (excl IN LISTS ARG_EXCLUDE_DIRS)
        if (IS_ABSOLUTE "${excl}")
            file(TO_CMAKE_PATH "${excl}" excl_norm)
        else ()
            get_filename_component(excl_norm "${ARG_DIR}/${excl}" ABSOLUTE)
        endif ()
        string(REGEX REPLACE "/$" "" excl_norm "${excl_norm}")
        list(APPEND EXCLUDE_ABS_DIRS "${excl_norm}")
    endforeach ()

    # 查找所有一级子目录
    file(GLOB children RELATIVE "${ARG_DIR}" "${ARG_DIR}/*")
    foreach (child IN LISTS children)
        set(child_path "${ARG_DIR}/${child}")
        if (IS_DIRECTORY "${child_path}")
            get_filename_component(child_abs "${child_path}" ABSOLUTE)
            string(REGEX REPLACE "/$" "" child_abs "${child_abs}")
            list(FIND EXCLUDE_ABS_DIRS "${child_abs}" found)
            if (found EQUAL -1)
                add_subdirectory("${child_path}")
                if (ARG_RECURSIVE)
                    rendu_add_subdirectories(DIR "${child_path}" EXCLUDE_DIRS ${ARG_EXCLUDE_DIRS} RECURSIVE)
                endif ()
            endif ()
        endif ()
    endforeach ()
endfunction()
