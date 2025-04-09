# 设置 UNIX 平台相关选项
function(rendu_setup_unix_options)
    # 设置默认配置目录
    if (NOT RENDU_CONF_DIR)
        set(RENDU_CONF_DIR "${CMAKE_INSTALL_PREFIX}/etc" CACHE PATH "Configuration directory")
        rendu_log_info("UNIX: Using default configuration directory: ${RENDU_CONF_DIR}")
    endif ()

    # RPATH 配置
    set(CMAKE_SKIP_BUILD_RPATH OFF)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

    # 链接器选择
    if (RENDU_USE_LD_GOLD)
        execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE LD_VERSION)
        if ("${LD_VERSION}" MATCHES "GNU gold")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
            rendu_log_info("UNIX: Using GNU gold linker")
        else ()
            message(WARNING "UNIX: GNU gold linker isn't available, using the default system linker")
        endif ()
    else ()
        rendu_log_info("UNIX: Using default system linker")
    endif ()

    # macOS 下 Homebrew 支持
    if (APPLE)
        find_program(HOMEBREW_EXECUTABLE brew)
        if (HOMEBREW_EXECUTABLE)
            rendu_log_info("Homebrew found at ${HOMEBREW_EXECUTABLE}")
            execute_process(COMMAND ${HOMEBREW_EXECUTABLE} config OUTPUT_VARIABLE HOMEBREW_STATUS_STR)
            string(REGEX MATCH "HOMEBREW_PREFIX: ([^\n]*)" _ "${HOMEBREW_STATUS_STR}")
            set(HOMEBREW_PREFIX "${CMAKE_MATCH_1}")
            rendu_log_info("Homebrew installation found at ${HOMEBREW_PREFIX}")
            set(CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
        endif ()
    endif ()

    rendu_log_info("UNIX: Detected compiler: ${CMAKE_C_COMPILER}")

    # 设置输出目录
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/lib")
endfunction()

rendu_setup_unix_options()