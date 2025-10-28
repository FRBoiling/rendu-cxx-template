# 源码组织与智能收集
include(RenduCollectDirectories)
include(RenduCollectFiles)

# =============================================
# 函数: rendu_add_library
# 描述: 增强版 add_library，自动收集源文件并标准化目标属性，支持依赖与宏定义配置
#
# 参数:
#   DIR        - 源码目录（必填）
#   PROJECT    - 项目名（可选，用于IDE工程标签）
#   NAME       - 目标名（必填）
#   PRIVATE_LINK  - 需要 PRIVATE 链接的接口库（可选）
#   PUBLIC_LINK   - 需要 PUBLIC 链接的依赖库（可选）
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
#     PRIVATE_LINK  rendu-core-interface
#     PUBLIC_LINK   rendu::core
#     DEFINES   MYLIB_EXPORTS
#     STATIC
#     ALIAS     ON
# )
# =============================================

function(rendu_add_library)
    # 参数解析
    set(options STATIC SHARED INTERFACE ALIAS)
    set(oneValueArgs DIR PROJECT NAME)
    set(multiValueArgs PRIVATE_LINK PUBLIC_LINK DEFINES SOURCES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 检查必需参数
    if (NOT ARG_NAME)
        rendu_log_fatal("必须指定 NAME")
    endif ()
    if (NOT ARG_DIR)
        rendu_log_fatal("必须指定 DIR")
    endif ()

    # 收集源文件（只执行一次）
    if (NOT ARG_SOURCES)
        # 自动收集源文件
        rendu_collect_source_files(
                SRC_LIST ${ARG_DIR}
                EXTENSIONS .c .cc .cpp .inl
                EXCLUDE_DIRS
                "${ARG_DIR}/precompiled_headers"
        )
    else ()
        set(SRC_LIST ${ARG_SOURCES})
    endif ()

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
        rendu_log_debug("${target_name} ${lib_type} 库")
    else ()
        set(lib_type INTERFACE)
        add_library(${target_name} ${lib_type})
        rendu_log_debug("${target_name} 自动转为 header-only ${lib_type} 库（无源文件）")
    endif ()

    # 目标属性
    if (ARG_PROJECT)
        set_target_properties(${target_name} PROPERTIES PROJECT_LABEL "${ARG_PROJECT}")
        set_target_properties(${target_name} PROPERTIES FOLDER "${ARG_PROJECT}/${ARG_NAME}")
    endif ()

    # 自动收集 include 目录
    rendu_collect_include_directories(INCLUDE_DIRS "${ARG_DIR}"
            EXCLUDE_DIRS
            "${ARG_DIR}/tests"
            "${CMAKE_BINARY_DIR}"
            EXCLUDE_REGEX ".*/private"
    )

    # 根据库类型确定包含目录可见性
    if (lib_type STREQUAL "INTERFACE")
        set(visibility INTERFACE)
    else ()
        set(visibility PUBLIC)
    endif ()
    target_include_directories(${target_name}
            ${visibility}
            ${INCLUDE_DIRS}
    )

    # 链接接口库
    if (ARG_PRIVATE_LINK)
        if (lib_type STREQUAL "INTERFACE")
            target_link_libraries(${target_name} INTERFACE ${ARG_PRIVATE_LINK})
        else ()
            target_link_libraries(${target_name} PRIVATE ${ARG_PRIVATE_LINK})
        endif ()
    endif ()

    # 链接依赖
    if (ARG_PUBLIC_LINK)
        if (lib_type STREQUAL "INTERFACE")
            target_link_libraries(${target_name} INTERFACE ${ARG_PUBLIC_LINK})
        else ()
            target_link_libraries(${target_name} PUBLIC ${ARG_PUBLIC_LINK})
        endif ()
    endif ()

    # 添加预处理宏
    if (ARG_DEFINES)
        if (lib_type STREQUAL "INTERFACE")
            target_compile_definitions(${target_name} INTERFACE ${ARG_DEFINES})
        else ()
            target_compile_definitions(${target_name} PRIVATE ${ARG_DEFINES})
        endif ()
    endif ()

    # 支持 ALIAS，命名规范为 project::name
    if (ARG_ALIAS AND ARG_PROJECT)
        set(alias_name "${ARG_PROJECT}::${ARG_NAME}")
        rendu_log_debug("${alias_name} 作为 ALIAS 目标")
        add_library(${alias_name} ALIAS ${target_name})
    endif ()

endfunction()
