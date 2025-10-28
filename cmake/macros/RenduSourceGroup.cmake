# 源码组织与智能收集
include(RenduCollectDirectories)
include(RenduCollectFiles)

# =============================================
# 函数: rendu_source_group
# 描述: 跨平台增强版 source_group，自动处理分隔符、支持批量文件、空分组名归为根分组
#
# 参数:
#   GROUP_NAME - 分组名（自动处理 / -> \，支持空分组）
#   FILES      - 文件列表（可变参数）
#
# 用法示例:
# rendu_source_group("Core/Utils" ${SRC_LIST})
# rendu_source_group("" ${ROOT_FILES})
# =============================================
macro(rendu_source_group GROUP_NAME)
    set(files ${ARGN})
    # 兼容 Windows 分组分隔符
    if (WIN32)
        string(REPLACE "/" "\\" group "${GROUP_NAME}")
    else ()
        set(group "${GROUP_NAME}")
    endif ()
    if (group STREQUAL "")
        set(group "\\")
    endif ()
    source_group("${group}" FILES ${files})
endmacro()

# ====================================================================
# 增强版智能源文件分组器
#
# 宏: rendu_source_groups
# 描述: 根据目录结构组织源文件，支持多种IDE工程视图模式
#
# 参数:
#   dir - 源文件根目录(绝对或相对路径)
#   [EXCLUDE_DIRS dir1 [dir2...]] - 要排除的目录列表
#   [EXTENSIONS ext1 [ext2...]] - 自定义文件扩展名(默认包含常见C/C++扩展名)
#
# 控制变量:
#   RENDU_SOURCE_GROUPING_MODE - 分组模式 [FLAT|HIERARCHICAL|NONE|FOLDERS]
#       FLAT: 一级目录分组
#       HIERARCHICAL: 完整目录结构分组
#       FOLDERS: 启用VS文件夹视图的分层分组
#       NONE: 不分组(默认)
#
# 功能增强:
#   1. 支持排除特定目录
#   2. 支持自定义文件扩展名
#   3. 改进的路径处理
#   4. 性能优化

# # 基本用法
# rendu_source_groups(${CMAKE_CURRENT_SOURCE_DIR}/src)
#
# # 高级用法
# rendu_source_groups(
#     ${PROJECT_SOURCE_DIR}/libs
#     EXCLUDE_DIRS
#         ${PROJECT_SOURCE_DIR}/libs/thirdparty
#         ${PROJECT_SOURCE_DIR}/libs/tests
#     EXTENSIONS h hpp cpp inl
# )
# # 启用VS文件夹视图
# set(RENDU_SOURCE_GROUPING_MODE "FOLDERS")
# rendu_source_groups(${CMAKE_CURRENT_SOURCE_DIR}/core)
# ====================================================================
macro(rendu_source_groups dir)
    # 参数解析
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs EXCLUDE_DIRS EXTENSIONS)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 参数校验和路径处理
    if (NOT IS_ABSOLUTE "${dir}")
        get_filename_component(dir "${dir}" ABSOLUTE)
    endif ()

    if (NOT EXISTS "${dir}")
        rendu_log_warn("Directory does not exist: ${dir}")
        return()
    endif ()

    # 检查分组模式(兼容新旧变量名)
    if (DEFINED WITH_SOURCE_TREE AND NOT DEFINED RENDU_SOURCE_GROUPING_MODE)
        set(RENDU_SOURCE_GROUPING_MODE "${WITH_SOURCE_TREE}")
    elseif (NOT DEFINED RENDU_SOURCE_GROUPING_MODE)
        set(RENDU_SOURCE_GROUPING_MODE "NONE")
    endif ()

    string(TOUPPER "${RENDU_SOURCE_GROUPING_MODE}" grouping_mode)

    # 设置默认扩展名
    if (NOT ARG_EXTENSIONS)
        set(ARG_EXTENSIONS
                h hh hpp hxx
                c cc cpp cxx
                inl def)
    endif ()

    # 仅在有意义的分组模式下执行
    if (NOT grouping_mode STREQUAL "NONE")
        # 生成GLOB模式
        set(patterns "")
        foreach (ext IN LISTS ARG_EXTENSIONS)
            list(APPEND patterns "${dir}/*.${ext}")
        endforeach ()

        # 递归收集所有源文件
        file(GLOB_RECURSE elements
                LIST_DIRECTORIES false
                RELATIVE "${dir}"
                ${patterns})

        # 过滤排除目录
        set(filtered_elements "")
        foreach (element IN LISTS elements)
            set(include_file TRUE)

            # 检查是否在排除目录中
            get_filename_component(element_dir "${element}" DIRECTORY)
            foreach (excl_dir IN LISTS ARG_EXCLUDE_DIRS)
                string(FIND "${dir}/${element_dir}" "${excl_dir}" pos)
                if (pos EQUAL 0)
                    set(include_file FALSE)
                    break()
                endif ()
            endforeach ()

            if (include_file)
                list(APPEND filtered_elements "${element}")
            endif ()
        endforeach ()

        # 启用VS文件夹视图(仅Windows)
        if ((grouping_mode STREQUAL "FOLDERS" OR grouping_mode STREQUAL "HIERARCHICAL_FOLDERS") AND WIN32)
            set_property(GLOBAL PROPERTY USE_FOLDERS ON)
        endif ()

        # 分组处理
        foreach (element IN LISTS filtered_elements)
            get_filename_component(element_dir "${element}" DIRECTORY)

            # 确定分组名
            if ("${element_dir}" STREQUAL "")
                set(group_name "\\")  # 根分组
            else ()
                if (grouping_mode MATCHES "^FLAT")
                    # 扁平模式: 取首级目录
                    string(FIND "${element_dir}" "/" first_slash_pos)
                    if (NOT first_slash_pos EQUAL -1)
                        string(SUBSTRING "${element_dir}" 0 ${first_slash_pos} group_name)
                    else ()
                        set(group_name "${element_dir}")
                    endif ()
                else ()
                    # 分层模式: 完整路径转换
                    if (WIN32 AND (grouping_mode STREQUAL "FOLDERS" OR grouping_mode STREQUAL "HIERARCHICAL_FOLDERS"))
                        string(REPLACE "/" "\\" group_name "${element_dir}")
                    else ()
                        set(group_name "${element_dir}")
                    endif ()
                endif ()
            endif ()

            # 应用分组(处理路径特殊字符)
            file(TO_NATIVE_PATH "${dir}/${element}" native_file_path)
            rendu_source_group("${group_name}" "${native_file_path}")
        endforeach ()
    endif ()
endmacro()
