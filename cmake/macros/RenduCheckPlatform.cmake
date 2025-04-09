# RenduCheckPlatform.cmake
# =========================
# 平台与编译器检测脚本
# =========================

# 检测平台相关信息（位数和架构）
function(rendu_check_platform)
    # 检测 64 位或 32 位平台
    if (CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(RENDU_PLATFORM 64)
    else ()
        set(RENDU_PLATFORM 32)
    endif ()
    set(RENDU_PLATFORM ${RENDU_PLATFORM} PARENT_SCOPE)
    rendu_log_info("Detected ${RENDU_PLATFORM}-bit platform")

    # 检测处理器架构
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64|x86_64|AMD64")
        set(RENDU_SYSTEM_PROCESSOR "amd64")
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM|aarch)64$")
        set(RENDU_SYSTEM_PROCESSOR "arm64")
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|ARM)$")
        set(RENDU_SYSTEM_PROCESSOR "arm")
    else ()
        set(RENDU_SYSTEM_PROCESSOR "x86")
    endif ()

    # 兼容 MSVC 的 -A 平台参数
    if (CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
        set(RENDU_SYSTEM_PROCESSOR "x86")
    elseif (CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
        set(RENDU_SYSTEM_PROCESSOR "amd64")
    elseif (CMAKE_GENERATOR_PLATFORM STREQUAL "ARM")
        set(RENDU_SYSTEM_PROCESSOR "arm")
    elseif (CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
        set(RENDU_SYSTEM_PROCESSOR "arm64")
    endif ()
    set(RENDU_SYSTEM_PROCESSOR ${RENDU_SYSTEM_PROCESSOR} PARENT_SCOPE)
    rendu_log_info("Detected ${RENDU_SYSTEM_PROCESSOR} processor architecture")

    # 平台相关设置
    if (WIN32)
        include("${CMAKE_SOURCE_DIR}/cmake/platform/win/settings.cmake")
    elseif (UNIX)
        include("${CMAKE_SOURCE_DIR}/cmake/platform/unix/settings.cmake")
    endif ()
endfunction()

# 检测编译器相关信息
function(rendu_check_compiler)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        include("${CMAKE_SOURCE_DIR}/cmake/compiler/msvc/settings.cmake")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        include("${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        include("${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        include("${CMAKE_SOURCE_DIR}/cmake/compiler/icc/settings.cmake")
    endif ()
endfunction()

# 使用示例
rendu_check_platform()
rendu_check_compiler()