# 源码组织与智能收集
include(RenduCollectDirectories)
include(RenduCollectFiles)
include(RenduAddExecutable)
include(RenduAddLibrary)
include(RenduAddSubdirectories)
include(RenduSourceGroup)

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