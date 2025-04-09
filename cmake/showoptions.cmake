# Rendu - 一个免费开源的 MMORPG 服务器模拟器

# 输出核心信息和所选构建类型
rendu_log_info("*****************************************************")
rendu_log_info("* RenduCore 版本信息   : ${RENDU_REV_HASH} ${RENDU_REV_DATE} (${RENDU_REV_BRANCH} 分支)")
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (NOT IS_MULTI_CONFIG)
    rendu_log_info("* RenduCore 构建类型   : ${CMAKE_BUILD_TYPE}")
endif ()

# 输出安装目录信息
rendu_log_info("* 安装核心到           : ${CMAKE_INSTALL_PREFIX}")
if (RENDU_COPY_CONF)
    if (UNIX)
        rendu_log_info("* 配置文件安装到       : ${RENDU_CONF_DIR}")
    else ()
        rendu_log_info("* 配置文件安装到       : ${CMAKE_INSTALL_PREFIX}")
    endif ()
endif ()

# 显示配置选项
if (RENDU_LAUCHER)
    rendu_log_info("* 构建 launcher        : 是（默认）")
else ()
    rendu_log_info("* 构建 launcher        : 否")
endif ()

if (RENDU_TOOLS)
    rendu_log_info("* 构建工具             : 是（默认）")
else ()
    rendu_log_info("* 构建工具             : 否")
endif ()

if (RENDU_BUILD_TESTING)
    rendu_log_info("* 构建单元测试         : 是")
else ()
    rendu_log_info("* 构建单元测试         : 否（默认）")
endif ()

if (RENDU_SCRIPTS AND (NOT RENDU_SCRIPTS STREQUAL "none"))
    rendu_log_info("* 启用脚本支持         : 是（${RENDU_SCRIPTS}）")
else ()
    rendu_log_info("* 启用脚本支持         : 否")
endif ()

if (RENDU_USE_COREPCH)
    rendu_log_info("* Core 使用预编译头    : 是（默认）")
else ()
    rendu_log_info("* Core 使用预编译头    : 否")
endif ()

if (RENDU_USE_SCRIPTPCH)
    rendu_log_info("* 脚本使用预编译头     : 是（默认）")
else ()
    rendu_log_info("* 脚本使用预编译头     : 否")
endif ()

if (RENDU_WITH_WARNINGS)
    rendu_log_info("* 显示所有警告         : 是")
else ()
    rendu_log_info("* 显示所有警告         : 否（默认）")
endif ()

if (RENDU_WITH_WARNINGS_AS_ERRORS)
    rendu_log_info("* 警告视为错误         : 是")
else ()
    rendu_log_info("* 警告视为错误         : 否（默认）")
endif ()

if (RENDU_WITH_COREDEBUG)
    rendu_log_info("*****************************************************")
    rendu_log_info(" *** RENDU_WITH_COREDEBUG - 警告！")
    rendu_log_info(" *** 已启用额外的核心调试日志！")
    rendu_log_info(" *** 该设置不会提升崩溃日志质量！")
    rendu_log_info(" *** 如需更好的崩溃日志请使用 -DCMAKE_BUILD_TYPE=RelWithDebInfo")
    rendu_log_info(" *** 除非你明白用途，否则不要启用！")
    rendu_log_info("* 启用核心调试         : 是")
    target_compile_definitions(rendu-compile-option-interface
            INTERFACE
            RENDU_DEBUG)
else ()
    rendu_log_info("* 启用核心调试         : 否（默认）")
endif ()

if (NOT RENDU_WITH_SOURCE_TREE STREQUAL "no")
    rendu_log_info("* 显示源码树           : 是（${RENDU_WITH_SOURCE_TREE}）")
else ()
    rendu_log_info("* 显示源码树           : 否")
endif ()

if (RENDU_WITHOUT_GIT)
    rendu_log_info("* 使用 GIT 版本哈希    : 否")
    rendu_log_info("*****************************************************")
    rendu_log_info(" *** RENDU_WITHOUT_GIT - 警告！")
    rendu_log_info(" *** 选择该选项后你放弃了所有官方支持权利，")
    rendu_log_info(" *** 任何向核心开发者请求支持的行为都将被拒绝。")
    rendu_log_info(" *** 因为我们无法准确判断你所用代码的版本。")
    rendu_log_info(" *** 请确保你使用官方仓库代码和受支持的 git 版本，")
    rendu_log_info(" *** 否则 revision-hash 功能无法正常工作。")
else ()
    rendu_log_info("* 使用 GIT 版本哈希    : 是（默认）")
endif ()

if (RENDU_WITHOUT_METRICS)
    rendu_log_info("*****************************************************")
    rendu_log_info(" *** RENDU_WITHOUT_METRICS - 警告！")
    rendu_log_info(" *** 所有 metrics 输出（如 InfluxDB 和 Grafana）将被禁用")
    target_compile_definitions(rendu-compile-option-interface
            INTERFACE
            WITHOUT_METRICS)
elseif (RENDU_WITH_DETAILED_METRICS)
    rendu_log_info("*****************************************************")
    rendu_log_info(" *** RENDU_WITH_DETAILED_METRICS - 警告！")
    rendu_log_info(" *** 将启用详细 metrics 输出（如每个 session 的耗时）")
    target_compile_definitions(rendu-compile-option-interface
            INTERFACE
            WITH_DETAILED_METRICS)
endif ()

if (BUILD_SHARED_LIBS)
    rendu_log_info("*****************************************************")
    rendu_log_info(" *** RENDU_WITH_DYNAMIC_LINKING - 信息！")
    rendu_log_info(" *** 将链接到共享库！")
    rendu_log_info(" *** 注意：该功能为实验性！")
    if (RENDU_WITH_DYNAMIC_LINKING_FORCED)
        rendu_log_info("*****************************************************")
        rendu_log_info(" *** 动态链接由脚本模块强制启用！")
    endif ()
    target_compile_definitions(rendu-compile-option-interface
            INTERFACE
            RENDU_API_USE_DYNAMIC_LINKING)

    WarnAboutSpacesInBuildPath()
endif ()

rendu_log_info("*****************************************************")