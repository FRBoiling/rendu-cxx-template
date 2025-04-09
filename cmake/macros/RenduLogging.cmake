# =============================================
# 模块: RenduLogging.cmake
# 描述: 提供CMake项目中的日志功能
# 支持多级别日志记录并输出到控制台和文件
# =============================================

# 日志级别常量定义
set(RENDU_LOG_LEVEL_NONE 0)
set(RENDU_LOG_LEVEL_FATAL 1)
set(RENDU_LOG_LEVEL_ERROR 2)
set(RENDU_LOG_LEVEL_WARN 3)
set(RENDU_LOG_LEVEL_INFO 4)
set(RENDU_LOG_LEVEL_DEBUG 5)
set(RENDU_LOG_LEVEL_TRACE 6)

# 默认日志级别
if (NOT DEFINED RENDU_CMAKE_LOG_LEVEL)
    set(RENDU_CMAKE_LOG_LEVEL ${RENDU_LOG_LEVEL_INFO})
endif ()

# 日志文件位置
if (NOT DEFINED RENDU_CMAKE_LOG_FILE)
    set(RENDU_CMAKE_LOG_FILE "${CMAKE_BINARY_DIR}/cmake.log")
endif ()

# 验证日志目录是否可写
get_filename_component(log_dir "${RENDU_CMAKE_LOG_FILE}" DIRECTORY)
file(MAKE_DIRECTORY "${log_dir}")

# 检查目录是否存在且可写
if (NOT EXISTS "${log_dir}" OR NOT IS_DIRECTORY "${log_dir}")
    message(WARNING "日志目录不存在或无法创建: ${log_dir}")
    set(RENDU_CMAKE_LOG_FILE "${CMAKE_BINARY_DIR}/cmake.log")
    get_filename_component(log_dir "${RENDU_CMAKE_LOG_FILE}" DIRECTORY)
    file(MAKE_DIRECTORY "${log_dir}")
endif ()

if (NOT EXISTS "${log_dir}" OR NOT IS_DIRECTORY "${log_dir}")
    message(WARNING "无法创建日志目录，日志将不会输出到文件")
    set(RENDU_CMAKE_LOG_FILE "")
else ()
    # 尝试写入测试
    file(WRITE "${RENDU_CMAKE_LOG_FILE}.test" "Test\n")
    if (EXISTS "${RENDU_CMAKE_LOG_FILE}.test")
        file(REMOVE "${RENDU_CMAKE_LOG_FILE}.test")
    else ()
        message(WARNING "日志文件无法写入: ${RENDU_CMAKE_LOG_FILE}，日志将不会输出到文件")
        set(RENDU_CMAKE_LOG_FILE "")
    endif ()
endif ()

# =============================================
# 函数: rendu_set_log_level
# 描述: 设置当前日志级别
# 参数:
#   level - 日志级别 (NONE,FATAL, ERROR, WARN, INFO, DEBUG, TRACE)
# =============================================
function(rendu_set_log_level level)
    string(TOUPPER "${level}" level_upper)

    # 直接映射字符串到级别常量
    if (level_upper STREQUAL "NONE")
        set(level_value ${RENDU_LOG_LEVEL_NONE})
    elseif (level_upper STREQUAL "FATAL")
        set(level_value ${RENDU_LOG_LEVEL_FATAL})
    elseif (level_upper STREQUAL "ERROR")
        set(level_value ${RENDU_LOG_LEVEL_ERROR})
    elseif (level_upper STREQUAL "WARN")
        set(level_value ${RENDU_LOG_LEVEL_WARN})
    elseif (level_upper STREQUAL "INFO")
        set(level_value ${RENDU_LOG_LEVEL_INFO})
    elseif (level_upper STREQUAL "DEBUG")
        set(level_value ${RENDU_LOG_LEVEL_DEBUG})
    elseif (level_upper STREQUAL "TRACE")
        set(level_value ${RENDU_LOG_LEVEL_TRACE})
    else ()
        message(WARNING "无效的日志级别 '${level}'，使用默认级别 INFO")
        set(level_value ${RENDU_LOG_LEVEL_INFO})
        set(level_upper "INFO")
    endif ()

    # 设置日志级别
    set(RENDU_CMAKE_LOG_LEVEL ${level_value} PARENT_SCOPE)
endfunction()

# =============================================
# 函数: rendu_log_set_prefix
# 描述: 设置日志前缀，并将当前前缀压入栈中
# 参数:
#   prefix - 日志前缀字符串
# =============================================
function(rendu_log_set_prefix prefix)
    # 将当前前缀压入栈
    if (DEFINED RENDU_LOG_PREFIX)
        list(APPEND RENDU_LOG_PREFIX_STACK "${RENDU_LOG_PREFIX}")
        set(RENDU_LOG_PREFIX_STACK "${RENDU_LOG_PREFIX_STACK}" CACHE INTERNAL "日志前缀栈")
    endif ()

    # 设置新前缀
    set(RENDU_LOG_PREFIX "${prefix}" CACHE INTERNAL "当前日志前缀")

    # 记录日志（使用新前缀）
    rendu_log_info("日志前缀设置为: ${prefix}")
endfunction()

# =============================================
# 函数: rendu_log_message
# 描述: 记录日志信息（内部使用）
# 参数:
#   level     - 日志级别 (数字)
#   level_str - 日志级别 (字符串)
#   message   - 日志消息
# =============================================
function(rendu_log_message level level_str message)
    if (level GREATER ${RENDU_CMAKE_LOG_LEVEL})
        return()
    endif ()

    # 获取当前时间
    string(TIMESTAMP current_time "%Y-%m-%d %H:%M:%S")

    # 构建带前缀的日志消息
    set(prefixed_message "${RENDU_LOG_PREFIX} ${message}")
    # 构建日志行
    set(log_line "[${current_time}] [${level_str}] ${prefixed_message}")
    # 根据日志级别设置颜色
    if (level_str STREQUAL "FATAL")
        message(FATAL_ERROR "${log_line}")
    elseif (level_str STREQUAL "ERROR")
        message(SEND_ERROR "${log_line}")
    elseif (level_str STREQUAL "WARNING")
        message(WARNING "${log_line}")
    elseif (level_str STREQUAL "INFO " OR level_str STREQUAL "STATUS")
        message(STATUS "${log_line}")
    elseif (level_str STREQUAL "DEBUG")
        message(STATUS "${log_line}")
    elseif (level_str STREQUAL "TRACE")
        message(STATUS "${log_line}")
    else ()
        message("${log_line}")
    endif ()

    # 追加到日志文件（如果日志文件有效）
    if (RENDU_CMAKE_LOG_FILE)
        file(APPEND "${RENDU_CMAKE_LOG_FILE}" "${log_line}\n")
    endif ()
endfunction()

# =============================================
# 函数: rendu_log_fatal
# 描述: 记录错误级日志
# 参数:
#   message - 错误消息
# =============================================
macro(rendu_log_fatal message)
    rendu_log_message(${RENDU_LOG_LEVEL_FATAL} "FATAL" "${message}")
endmacro()


# =============================================
# 函数: rendu_log_error
# 描述: 记录错误级日志
# 参数:
#   message - 错误消息
# =============================================
macro(rendu_log_error message)
    rendu_log_message(${RENDU_LOG_LEVEL_ERROR} "ERROR" "${message}")
endmacro()

# =============================================
# 函数: rendu_log_warn
# 描述: 记录警告级日志
# 参数:
#   message - 警告消息
# =============================================
macro(rendu_log_warn message)
    rendu_log_message(${RENDU_LOG_LEVEL_WARN} "WARN " "(${CMAKE_CURRENT_FUNCTION})${message}")
endmacro()

# =============================================
# 函数: rendu_log_info
# 描述: 记录信息级日志
# 参数:
#   message - 信息消息
# =============================================
macro(rendu_log_info message)
    rendu_log_message(${RENDU_LOG_LEVEL_INFO} "INFO " "${message}")
endmacro()

# =============================================
# 函数: rendu_log_debug
# 描述: 记录调试级日志
# 参数:
#   message - 调试消息
# =============================================
macro(rendu_log_debug message)
    rendu_log_message(${RENDU_LOG_LEVEL_DEBUG} "DEBUG" "${message}")
endmacro()

# =============================================
# 函数: rendu_log_trace
# 描述: 记录追踪级日志
# 参数:
#   message - 追踪消息
# =============================================
macro(rendu_log_trace message)
    rendu_log_message(${RENDU_LOG_LEVEL_TRACE} "TRACE" "${message}")
endmacro()


# 日志系统初始化函数
rendu_set_log_level(TRACE)
rendu_log_set_prefix("RD")


rendu_log_info("日志文件位置: ${RENDU_CMAKE_LOG_FILE}")
rendu_log_info("日志系统初始化完成，日志级别: ${RENDU_CMAKE_LOG_LEVEL}")