# =========================
# rendu_package_project
#
# 项目安装与打包辅助函数
# Project install & packaging helper function
#
# 参数说明 / Arguments:
#   NAME                - 项目名称 / Project name
#   VERSION             - 项目版本 / Project version
#   INCLUDE_DIR         - 头文件目录 / Include directory
#   INCLUDE_DESTINATION - 头文件安装目标目录 / Install destination for headers
#   BINARY_DIR          - 构建输出目录 / Build output directory
#   COMPATIBILITY       - 版本兼容性 / Version compatibility (default: AnyNewerVersion)
#   EXPORT_HEADER       - 导出头文件名 / Export header filename
#   VERSION_HEADER      - 版本头文件名 / Version header filename
#   NAMESPACE           - CMake target 命名空间 / CMake target namespace
#   DISABLE_VERSION_SUFFIX - 禁用版本后缀 / Disable version suffix
#   ARCH_INDEPENDENT    - 架构无关安装 / Architecture independent install
#   INCLUDE_HEADER_PATTERN - 头文件匹配模式 / Header file pattern
#   CPACK               - 是否启用 CPack 打包 / Enable CPack packaging
#   RUNTIME_DESTINATION - 运行时目标目录 / Runtime install destination
#   DEPENDENCIES        - 依赖列表 / Dependencies
# =========================

function(rendu_package_project)
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    cmake_parse_arguments(
            ARG
            ""
            "NAME;VERSION;INCLUDE_DIR;INCLUDE_DESTINATION;BINARY_DIR;COMPATIBILITY;EXPORT_HEADER;VERSION_HEADER;NAMESPACE;DISABLE_VERSION_SUFFIX;ARCH_INDEPENDENT;INCLUDE_HEADER_PATTERN;CPACK;RUNTIME_DESTINATION"
            "DEPENDENCIES"
            ${ARGN}
    )

    # 版本后缀处理
    if (ARG_DISABLE_VERSION_SUFFIX)
        unset(ARG_VERSION_SUFFIX)
    else ()
        set(ARG_VERSION_SUFFIX -${ARG_VERSION})
    endif ()

    # 版本兼容性
    if (NOT DEFINED ARG_COMPATIBILITY)
        set(ARG_COMPATIBILITY AnyNewerVersion)
    endif ()

    # 命名空间处理
    if (DEFINED ARG_NAMESPACE)
        if (ARG_CPACK)
            set(CPACK_PACKAGE_NAMESPACE ${ARG_NAMESPACE})
        endif ()
        set(ARG_NAMESPACE ${ARG_NAMESPACE}::)
        add_library(${ARG_NAMESPACE}${ARG_NAME} ALIAS ${ARG_NAME})
    endif ()

    # 生成导出头文件和版本头文件
    if (DEFINED ARG_VERSION_HEADER OR DEFINED ARG_EXPORT_HEADER)
        set(ARG_VERSION_INCLUDE_DIR ${ARG_BINARY_DIR}/PackageProjectInclude)

        if (DEFINED ARG_EXPORT_HEADER)
            include(GenerateExportHeader)
            generate_export_header(
                    ${ARG_NAME} EXPORT_FILE_NAME ${ARG_VERSION_INCLUDE_DIR}/${ARG_EXPORT_HEADER}
            )
        endif ()

        if (DEFINED ARG_VERSION_HEADER)
            # 解析版本号
            unset(CMAKE_MATCH_1)
            unset(CMAKE_MATCH_3)
            unset(CMAKE_MATCH_5)
            unset(CMAKE_MATCH_7)
            string(REGEX MATCH "^([0-9]+)(\\.([0-9]+))?(\\.([0-9]+))?(\\.([0-9]+))?$" _
                    "${ARG_VERSION}"
            )
            set(ARG_VERSION_MAJOR ${CMAKE_MATCH_1})
            set(ARG_VERSION_MINOR ${CMAKE_MATCH_3})
            set(ARG_VERSION_PATCH ${CMAKE_MATCH_5})
            set(ARG_VERSION_TWEAK ${CMAKE_MATCH_7})

            if (NOT DEFINED ARG_VERSION_MAJOR)
                set(ARG_VERSION_MAJOR "0")
            endif ()
            if (NOT DEFINED ARG_VERSION_MINOR)
                set(ARG_VERSION_MINOR "0")
            endif ()
            if (NOT DEFINED ARG_VERSION_PATCH)
                set(ARG_VERSION_PATCH "0")
            endif ()
            if (NOT DEFINED ARG_VERSION_TWEAK)
                set(ARG_VERSION_TWEAK "0")
            endif ()

            string(TOUPPER ${ARG_NAME} UPPERCASE_ARG_NAME)
            string(REGEX REPLACE [^a-zA-Z0-9] _ UPPERCASE_ARG_NAME ${UPPERCASE_ARG_NAME})
            configure_file(
                    ${PACKAGE_PROJECT_ROOT_PATH}/version.h.in
                    ${ARG_VERSION_INCLUDE_DIR}/${ARG_VERSION_HEADER} @ONLY
            )
        endif ()

        # 设置 include 目录
        get_target_property(target_type ${ARG_NAME} TYPE)
        if (target_type STREQUAL "INTERFACE_LIBRARY")
            set(VISIBILITY INTERFACE)
        else ()
            set(VISIBILITY PUBLIC)
        endif ()
        target_include_directories(
                ${ARG_NAME} ${VISIBILITY} "$<BUILD_INTERFACE:${ARG_VERSION_INCLUDE_DIR}>"
        )
        install(
                DIRECTORY ${ARG_VERSION_INCLUDE_DIR}/
                DESTINATION ${ARG_INCLUDE_DESTINATION}
                COMPONENT "${ARG_NAME}_Development"
        )
    endif ()

    # 架构无关处理
    set(wbpvf_extra_args "")
    if (NOT DEFINED ARG_ARCH_INDEPENDENT)
        get_target_property(target_type "${ARG_NAME}" TYPE)
        if (target_type STREQUAL "INTERFACE_LIBRARY")
            set(ARG_ARCH_INDEPENDENT YES)
        endif ()
    endif ()

    if (ARG_ARCH_INDEPENDENT)
        set(wbpvf_extra_args ARCH_INDEPENDENT)
        set(INSTALL_DIR_FOR_CMAKE_CONFIGS ${CMAKE_INSTALL_DATADIR})
    else ()
        set(INSTALL_DIR_FOR_CMAKE_CONFIGS ${CMAKE_INSTALL_LIBDIR})
    endif ()

    # 生成版本文件
    write_basic_package_version_file(
            "${ARG_BINARY_DIR}/${ARG_NAME}ConfigVersion.cmake"
            VERSION ${ARG_VERSION}
            COMPATIBILITY ${ARG_COMPATIBILITY} ${wbpvf_extra_args}
    )

    # 设置默认运行时安装目录
    if (NOT DEFINED ARG_RUNTIME_DESTINATION)
        set(ARG_RUNTIME_DESTINATION ${ARG_NAME}${ARG_VERSION_SUFFIX})
    endif ()

    # 安装目标
    install(
            TARGETS ${ARG_NAME}
            EXPORT ${ARG_NAME}Targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${ARG_RUNTIME_DESTINATION}
            COMPONENT "${ARG_NAME}_Runtime"
            NAMELINK_COMPONENT "${ARG_NAME}_Development"
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${ARG_RUNTIME_DESTINATION}
            COMPONENT "${ARG_NAME}_Development"
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${ARG_RUNTIME_DESTINATION}
            COMPONENT "${ARG_NAME}_Runtime"
            BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}/${ARG_RUNTIME_DESTINATION}
            COMPONENT "${ARG_NAME}_Runtime"
            PUBLIC_HEADER DESTINATION ${ARG_INCLUDE_DESTINATION} COMPONENT "${ARG_NAME}_Development"
            INCLUDES
            DESTINATION "${ARG_INCLUDE_DESTINATION}"
    )

    # CMake 配置文件安装目录
    set("${ARG_NAME}_INSTALL_CMAKEDIR"
            "${INSTALL_DIR_FOR_CMAKE_CONFIGS}/cmake/${ARG_NAME}${ARG_VERSION_SUFFIX}"
            CACHE PATH "CMake package config location relative to the install prefix"
    )
    mark_as_advanced("${ARG_NAME}_INSTALL_CMAKEDIR")

    # 生成并安装 CMake 配置文件
    configure_file(
            ${PACKAGE_PROJECT_ROOT_PATH}/Config.cmake.in
            "${ARG_BINARY_DIR}/${ARG_NAME}Config.cmake" @ONLY
    )
    install(
            EXPORT ${ARG_NAME}Targets
            DESTINATION "${${ARG_NAME}_INSTALL_CMAKEDIR}"
            NAMESPACE ${ARG_NAMESPACE}
            COMPONENT "${ARG_NAME}_Development"
    )
    install(
            FILES "${ARG_BINARY_DIR}/${ARG_NAME}ConfigVersion.cmake"
            "${ARG_BINARY_DIR}/${ARG_NAME}Config.cmake"
            DESTINATION "${${ARG_NAME}_INSTALL_CMAKEDIR}"
            COMPONENT "${ARG_NAME}_Development"
    )

    # 安装头文件
    if (NOT DEFINED ARG_INCLUDE_HEADER_PATTERN)
        set(ARG_INCLUDE_HEADER_PATTERN "*")
    endif ()
    if (ARG_INCLUDE_DESTINATION AND ARG_INCLUDE_DIR)
        install(
                DIRECTORY ${ARG_INCLUDE_DIR}/
                DESTINATION ${ARG_INCLUDE_DESTINATION}
                COMPONENT "${ARG_NAME}_Development"
                FILES_MATCHING
                PATTERN "${ARG_INCLUDE_HEADER_PATTERN}"
        )
    endif ()

    # 设置版本变量
    set(${ARG_NAME}_VERSION
            ${ARG_VERSION}
            CACHE INTERNAL ""
    )

    # CPack 打包支持
    if (ARG_CPACK)
        if (CPACK_PACKAGE_NAMESPACE)
            set(CPACK_PACKAGE_NAME ${CPACK_PACKAGE_NAMESPACE}-${ARG_NAME})
        else ()
            set(CPACK_PACKAGE_NAME ${ARG_NAME})
        endif ()
        if (NOT CPACK_PACKAGE_DESCRIPTION_SUMMARY)
            set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
        endif ()
        if (NOT CPACK_PACKAGE_HOMEPAGE_URL)
            set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
        endif ()
        set(CPACK_VERBATIM_VARIABLES YES)
        set(CPACK_THREADS 0)
        set(CPACK_PACKAGE_VERSION_MAJOR ${ARG_VERSION_MAJOR})
        set(CPACK_PACKAGE_VERSION_MINOR ${ARG_VERSION_MINOR})
        set(CPACK_PACKAGE_VERSION_PATCH ${ARG_VERSION_PATCH})

        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
            set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
        endif ()

        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
        set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
        set(CPACK_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
                OWNER_READ
                OWNER_WRITE
                OWNER_EXECUTE
                GROUP_READ
                GROUP_EXECUTE
                WORLD_READ
                WORLD_EXECUTE
        )

        include(CPack)
    endif ()
endfunction()