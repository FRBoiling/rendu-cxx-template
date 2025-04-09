
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


# =============================================
# 函数: rendu_add_executable
# 描述: 增强版 add_executable，自动收集源文件并标准化目标属性，支持依赖与宏定义配置
#
# 参数:
#   DIR        - 源码目录（必填）
#   PROJECT    - 项目名（可选，用于IDE工程标签）
#   NAME       - 目标名（必填）
#   LINKOPTS   - 需要 PUBLIC 链接的接口库（可选）
#   DEPS       - 需要 PRIVATE 链接的依赖库（可选）
#   DEFINES    - 需要添加的预处理宏（可选）
#   SOURCES    - 指定源文件列表（可选，未指定则自动收集）
#   ALIAS      - 是否创建 ALIAS 目标（可选，ON 时自动命名为 project::name）
#
# 用法示例:
# rendu_add_executable(
#     DIR       ${CMAKE_CURRENT_SOURCE_DIR}
#     PROJECT   ${PROJECT_NAME}
#     NAME      myexe
#     LINKOPTS  rendu-core-interface
#     DEPS      rendu::core
#     DEFINES   MYEXE_EXPORTS
#     ALIAS     ON
# )
# =============================================

function(rendu_add_executable)
    # 参数解析
    set(options ALIAS)
    set(oneValueArgs DIR PROJECT NAME)
    set(multiValueArgs LINKOPTS DEPS DEFINES SOURCES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 检查必需参数
    if (NOT ARG_NAME)
        rendu_log_fatal(" 必须指定 NAME")
    endif ()
    if (NOT ARG_DIR)
        rendu_log_fatal(" 必须指定 DIR")
    endif ()

    # 收集源文件
    if (NOT ARG_SOURCES)
        rendu_collect_source_files(SRC_LIST "${ARG_DIR}")
    else ()
        set(SRC_LIST ${ARG_SOURCES})
    endif ()

    set(target_name "${ARG_PROJECT}_${ARG_NAME}")
    add_executable(${target_name} ${SRC_LIST})

    # 自动收集 include 目录
    rendu_collect_include_directories(INCLUDE_DIRS "${ARG_DIR}"
            EXCLUDE_DIRS
            "${ARG_DIR}/tests"
            "${CMAKE_BINARY_DIR}"
            EXCLUDE_REGEX ".*/private"
    )

    target_include_directories(${target_name} PUBLIC ${INCLUDE_DIRS})

    # 目标属性
    if (ARG_PROJECT)
        set_target_properties(${target_name} PROPERTIES PROJECT_LABEL "${ARG_PROJECT}")
        set_target_properties(${target_name} PROPERTIES FOLDER "${ARG_PROJECT}/${ARG_NAME}")
    endif ()

    # 链接接口库
    if (ARG_LINKOPTS)
        target_link_libraries(${target_name} PUBLIC ${ARG_LINKOPTS})
    endif ()

    # 链接依赖
    if (ARG_DEPS)
        target_link_libraries(${target_name} PRIVATE ${ARG_DEPS})
    endif ()

    # 添加预处理宏
    if (ARG_DEFINES)
        target_compile_definitions(${target_name} PRIVATE ${ARG_DEFINES})
    endif ()

    # 支持 ALIAS，命名规范为 project::name
    if (ARG_ALIAS AND ARG_PROJECT)
        set(alias_name "${ARG_PROJECT}::${ARG_NAME}")
        add_executable(${alias_name} ALIAS ${target_name})
        rendu_log_debug(" ${alias_name} 作为 ALIAS 目标")
    endif ()
endfunction()


# =============================================
# 函数: rendu_add_library
# 描述: 增强版 add_library，自动收集源文件并标准化目标属性，支持依赖与宏定义配置
#
# 参数:
#   DIR        - 源码目录（必填）
#   PROJECT    - 项目名（可选，用于IDE工程标签）
#   NAME       - 目标名（必填）
#   LINKOPTS   - 需要 PUBLIC 链接的接口库（可选）
#   DEPS       - 需要 PRIVATE 链接的依赖库（可选）
#   DEFINES    - 需要添加的预处理宏（可选）
#   SOURCES    - 指定源文件列表（可选，未指定则自动收集）
#   STATIC/SHARED/INTERFACE - 库类型（可选，默认STATIC）
#   ALIAS      - 是否创建 ALIAS 目标（可选，ON 时自动命名为 project::name）
#
# 用法示例:
# rendu_add_library(
#     DIR       ${CMAKE_CURRENT_SOURCE_DIR}
#     PROJECT   ${PROJECT_NAME}
#     NAME      mylib
#     LINKOPTS  rendu-core-interface
#     DEPS      rendu::core
#     DEFINES   MYLIB_EXPORTS
#     STATIC    
#     ALIAS     ON
# )
# =============================================

function(rendu_add_library)
    # 参数解析
    set(options STATIC SHARED INTERFACE ALIAS)
    set(oneValueArgs DIR PROJECT NAME)
    set(multiValueArgs LINKOPTS DEPS DEFINES SOURCES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 检查必需参数
    if (NOT ARG_NAME)
        rendu_log_fatal("必须指定 NAME")
    endif ()
    if (NOT ARG_DIR)
        rendu_log_fatal("必须指定 DIR")
    endif ()

    # 收集源文件
    if (NOT ARG_SOURCES)
        # 自动收集源文件
        rendu_collect_source_files(
                SRC_LIST ${ARG_DIR}
                EXTENSIONS .c .cc .cpp .inl)
    else ()
        set(SRC_LIST ${ARG_SOURCES})
    endif ()

    # 自动收集 include 目录
    rendu_collect_include_directories(INCLUDE_DIRS "${ARG_DIR}"
            EXCLUDE_DIRS
            "${ARG_DIR}/tests"
            "${CMAKE_BINARY_DIR}"
            EXCLUDE_REGEX ".*/private"
    )

    # 创建库
    set(target_name "${ARG_PROJECT}_${ARG_NAME}")
    # 自动判断是否为 header-only
    if (SRC_LIST)
        if (ARG_STATIC)
            set(lib_type STATIC)
        elseif (ARG_SHARED)
            set(lib_type SHARED)
        elseif (ARG_INTERFACE)
            set(lib_type INTERFACE)
        else ()
            set(lib_type STATIC) # 默认静态库
        endif ()
        add_library(${target_name} ${lib_type} ${SRC_LIST})
        rendu_log_debug("${target_name} ${lib_type} 库（无源文件）")
    else ()
        set(lib_type INTERFACE)
        add_library(${target_name} ${lib_type})
        rendu_log_debug("${target_name} 自动转为 header-only ${lib_type} 库（无源文件）")
    endif ()
    # 根据库类型确定包含目录可见性
    if (lib_type STREQUAL "INTERFACE")
        set(visibility INTERFACE)
    else ()
        set(visibility PUBLIC)
    endif ()

    target_include_directories(${target_name} ${visibility} ${INCLUDE_DIRS})

    # 目标属性
    if (ARG_PROJECT)
        set_target_properties(${target_name} PROPERTIES PROJECT_LABEL "${ARG_PROJECT}")
        set_target_properties(${target_name} PROPERTIES FOLDER "${ARG_PROJECT}/${ARG_NAME}")
    endif ()

    # 链接接口库
    if (ARG_LINKOPTS)
        # 如果是 INTERFACE 库，使用 INTERFACE 链接
        # 否则使用 PRIVATE 链接
        if (lib_type STREQUAL "INTERFACE")
            target_link_libraries(${target_name} INTERFACE ${ARG_LINKOPTS})
        else ()
            # 对于非 INTERFACE 库，使用 PRIVATE 链接
            # 这样可以避免 INTERFACE 库的依赖被传播
            # 但仍然允许其他库链接到这个库
            # 并使用其依赖
            # 这对于静态库和共享库都适用
            # 但对于 INTERFACE 库，PUBLIC 链接是必须的
            # 因为 INTERFACE 库本身不包含源文件     
            target_link_libraries(${target_name} PRIVATE ${ARG_LINKOPTS})
        endif ()
    endif ()

    # 链接依赖
    if (ARG_DEPS)
        # 如果是 INTERFACE 库，使用 INTERFACE 链接 
        # 否则使用 PRIVATE 链接
        if (lib_type STREQUAL "INTERFACE")
            target_link_libraries(${target_name} INTERFACE ${ARG_DEPS})
        else ()
            # 对于非 INTERFACE 库，使用 PRIVATE 链接
            # 这样可以避免 INTERFACE 库的依赖被传播
            # 但仍然允许其他库链接到这个库
            # 并使用其依赖
            # 这对于静态库和共享库都适用                
            target_link_libraries(${target_name} PRIVATE ${ARG_DEPS})
        endif ()
    endif ()

    # 添加预处理宏
    if (ARG_DEFINES)
        # 对于 INTERFACE 库，使用 INTERFACE 定义
        # 否则使用 PRIVATE 定义
        if (lib_type STREQUAL "INTERFACE")
            target_compile_definitions(${target_name} INTERFACE ${ARG_DEFINES})
        else ()
            # 对于非 INTERFACE 库，使用 PRIVATE 定义
            # 这样可以避免 INTERFACE 库的定义被传播
            # 但仍然允许其他库链接到这个库
            # 并使用其定义
            # 这对于静态库和共享库都适用
            # 但对于 INTERFACE 库，PUBLIC 定义是必须的
            # 因为 INTERFACE 库本身不包含源文件
            # 并且需要在其他库中使用其定义
            # 这对于静态库和共享库都适用
            target_compile_definitions(${target_name} PRIVATE ${ARG_DEFINES})
        endif ()
    endif ()

    # 支持 ALIAS，命名规范为 project::name
    if (ARG_ALIAS AND ARG_PROJECT)
        set(alias_name "${ARG_PROJECT}::${ARG_NAME}")
        rendu_log_debug("${alias_name} 作为 ALIAS 目标")
        # 创建 ALIAS 目标
        # 这样可以在其他地方使用 project::name 来引用这个库
        # 例如在其他库或可执行文件中链接时
        # 可以使用 target_link_libraries(myexe PRIVATE project::name)
        # 这样可以避免直接引用库名，增强可移植性
        add_library(${alias_name} ALIAS ${target_name})
    endif ()
endfunction()


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


# =============================================
# 函数: rendu_install_targets
# 描述: 批量安装目标，支持指定安装类型、目录、组件等
#
# 参数:
#   TARGETS    - 目标列表（必填）
#   RUNTIME    - 可执行文件安装目录（可选）
#   LIBRARY    - 动态库安装目录（可选）
#   ARCHIVE    - 静态库安装目录（可选）
#   INCLUDES   - 头文件安装目录（可选）
#   COMPONENT  - 安装组件名（可选）
#
# 用法示例:
# rendu_install_targets(
#     TARGETS mylib myexe
#     RUNTIME bin
#     LIBRARY lib
#     ARCHIVE lib
#     COMPONENT core
# )
# =============================================
function(rendu_install_targets)
    set(options "")
    set(oneValueArgs RUNTIME LIBRARY ARCHIVE INCLUDES COMPONENT)
    set(multiValueArgs TARGETS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT ARG_TARGETS)
        rendu_log_fatal("rendu_install_targets: 必须指定 TARGETS")
    endif ()

    foreach (target IN LISTS ARG_TARGETS)
        install(TARGETS ${target}
                RUNTIME DESTINATION ${ARG_RUNTIME}
                LIBRARY DESTINATION ${ARG_LIBRARY}
                ARCHIVE DESTINATION ${ARG_ARCHIVE}
                INCLUDES DESTINATION ${ARG_INCLUDES}
                COMPONENT ${ARG_COMPONENT}
        )
    endforeach ()
endfunction()

# =============================================
# 函数: rendu_add_uninstall_target
# 描述: 添加标准卸载目标，自动生成 uninstall 脚本
#
# 用法示例:
# rendu_add_uninstall_target()
# =============================================
function(rendu_add_uninstall_target)
    if (NOT TARGET uninstall)
        configure_file(
                "${CMAKE_SOURCE_DIR}/cmake/platform/cmake_uninstall.in.cmake"
                "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                @ONLY
        )
        add_custom_target(uninstall
                COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                COMMENT "Uninstall all installed files"
        )
        rendu_log_info("Uninstall target added")
    endif ()
endfunction()

# rendu_add_library(
#     DIR       ${CMAKE_CURRENT_SOURCE_DIR}
#     NAME      mylib
#     # ...
# )
# rendu_install_targets(
#     TARGETS mylib
#     RUNTIME bin
#     LIBRARY lib
#     ARCHIVE lib
#     COMPONENT core
# )
# rendu_add_uninstall_target()