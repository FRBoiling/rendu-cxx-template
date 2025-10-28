# 源码组织与智能收集
include(RenduCollectDirectories)
include(RenduCollectFiles)

# =============================================
# 函数: rendu_add_executable
# 描述: 增强版 add_executable，自动收集源文件并标准化目标属性，支持依赖与宏定义配置
#
# 参数:
#   DIR        - 源码目录（必填）
#   PROJECT    - 项目名（可选，用于IDE工程标签）
#   NAME       - 目标名（必填）
#   PRIVATE_LINK   - 需要 PRIVATE 链接的接口库（可选）
#   PUBLIC_LINK    - 需要 PUBLIC 链接的依赖库（可选）
#   DEFINES    - 需要添加的预处理宏（可选）
#   SOURCES    - 指定源文件列表（可选，未指定则自动收集）
#   ALIAS      - 是否创建 ALIAS 目标（可选，ON 时自动命名为 project::name）
#
# 用法示例:
# rendu_add_executable(
#     DIR       ${CMAKE_CURRENT_SOURCE_DIR}
#     PROJECT   ${PROJECT_NAME}
#     NAME      myexe
#     PRIVATE_LINK  rendu-core-interface
#     PUBLIC_LINK      rendu::core
#     DEFINES   MYEXE_EXPORTS
#     ALIAS     ON
# )
# =============================================

function(rendu_add_executable)
    # 参数解析
    set(options ALIAS)
    set(oneValueArgs DIR PROJECT NAME)
    set(multiValueArgs PRIVATE_LINK PUBLIC_LINK DEFINES SOURCES)
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
    if (ARG_PRIVATE_LINK)
        target_link_libraries(${target_name} PRIVATE ${ARG_PRIVATE_LINK})
    endif ()

    # 链接依赖
    if (ARG_PUBLIC_LINK)
        target_link_libraries(${target_name} PUBLIC ${ARG_PUBLIC_LINK})
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
