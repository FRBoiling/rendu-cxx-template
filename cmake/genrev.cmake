# Rendu 
# =========================
# 版本与构建信息生成脚本
# =========================

# 处理构建目录变量
if (NOT RENDU_BUILDDIR)
    set(RENDU_BUILDDIR ${CMAKE_BINARY_DIR})
endif ()

# GIT 信息处理
if (RENDU_WITHOUT_GIT)
    set(RENDU_REV_DATE "1970-01-01 00:00:00 +0000")
    set(RENDU_REV_HASH "unknown")
    set(RENDU_REV_BRANCH "Archived")
    string(TIMESTAMP RENDU_REV_DATE_FALLBACK "%Y-%m-%d %H:%M:%S" UTC)
else ()
    # find_package(Git 1.7)
    find_package(Git REQUIRED)

    if (GIT_EXECUTABLE)
        execute_process(
                COMMAND "${GIT_EXECUTABLE}" rev-parse --short=12 HEAD
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                OUTPUT_VARIABLE RENDU_REV_HASH
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_QUIET
        )

        if (RENDU_REV_HASH)
            execute_process(
                    COMMAND "${GIT_EXECUTABLE}" diff-index --quiet HEAD --
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    RESULT_VARIABLE RENDU_IS_DIRTY
                    ERROR_QUIET
            )
            if (RENDU_IS_DIRTY)
                set(RENDU_REV_HASH "${RENDU_REV_HASH}+")
            endif ()

            execute_process(
                    COMMAND "${GIT_EXECUTABLE}" show -s --format=%ci
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE RENDU_REV_DATE
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET
            )

            execute_process(
                    COMMAND "${GIT_EXECUTABLE}" symbolic-ref -q --short HEAD
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE RENDU_REV_BRANCH
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET
            )

            if (NOT RENDU_REV_BRANCH)
                execute_process(
                        COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/heads "--format=%(refname:short)"
                        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                        OUTPUT_VARIABLE RENDU_REV_BRANCH
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET
                )
            endif ()

            if (NOT RENDU_REV_BRANCH)
                execute_process(
                        COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/remotes "--format=%(refname:short)"
                        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                        OUTPUT_VARIABLE RENDU_REV_BRANCH
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        ERROR_QUIET
                )
            endif ()

            if (NOT RENDU_REV_BRANCH)
                set(RENDU_REV_BRANCH ${RENDU_REV_HASH})
            endif ()

            string(REGEX MATCH "^[^ \t\r\n]+" RENDU_REV_BRANCH ${RENDU_REV_BRANCH})
        endif ()
    endif ()

    if (NOT RENDU_REV_HASH)
        message(STATUS "
    Could not find a proper repository signature (hash) - you may need to pull tags with git fetch -t
    Continuing anyway - note that the versionstring will be set to \"unknown 1970-01-01 00:00:00 (Archived)\"")
        set(RENDU_REV_DATE "1970-01-01 00:00:00 +0000")
        set(RENDU_REV_HASH "unknown")
        set(RENDU_REV_BRANCH "Archived")
        string(TIMESTAMP RENDU_REV_DATE_FALLBACK "%Y-%m-%d %H:%M:%S" UTC)
    else ()
        set(RENDU_REV_DATE_FALLBACK ${RENDU_REV_DATE})
    endif ()
endif ()

# 提取日期信息
string(REGEX MATCH "([0-9]+)-([0-9]+)-([0-9]+)" RENDU_REV_DATE_FALLBACK_MATCH ${RENDU_REV_DATE_FALLBACK})
set(RENDU_REV_YEAR ${CMAKE_MATCH_1})
set(RENDU_REV_MONTH ${CMAKE_MATCH_2})
set(RENDU_REV_DAY ${CMAKE_MATCH_3})

# 主机系统信息
cmake_host_system_information(RESULT RENDU_BUILD_HOST_SYSTEM QUERY OS_NAME)
cmake_host_system_information(RESULT RENDU_BUILD_HOST_DISTRO QUERY DISTRIB_INFO)
cmake_host_system_information(RESULT RENDU_BUILD_HOST_SYSTEM_RELEASE QUERY OS_RELEASE)
if (WIN32)
    execute_process(
            COMMAND powershell -NoProfile -Command "$v=(Get-CimInstance -ClassName Win32_OperatingSystem); '{0} ({1})' -f $v.Caption, $v.Version"
            OUTPUT_VARIABLE RENDU_BUILD_HOST_SYSTEM_RELEASE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX REPLACE "^.* Windows " "" RENDU_BUILD_HOST_SYSTEM_RELEASE ${RENDU_BUILD_HOST_SYSTEM_RELEASE})
endif ()

if (CMAKE_SCRIPT_MODE_FILE)
    set(CMAKE_PLATFORM_INFO_DIR ${RENDU_BUILDDIR}${CMAKE_FILES_DIRECTORY})
    include(${CMAKE_ROOT}/Modules/CMakeDetermineSystem.cmake)
endif ()

# 生成 revision_data.h
configure_file(
        "${CMAKE_SOURCE_DIR}/revision_data.h.in.cmake"
        "${RENDU_BUILDDIR}/revision_data.h"
        @ONLY
)