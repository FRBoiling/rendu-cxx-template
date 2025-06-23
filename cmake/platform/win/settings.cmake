# platform/win/settings.cmake 平台相关配置 

# =========================
# Windows 平台相关设置
# =========================
function(rendu_setup_win_options)
    # 设置编译选项和宏定义
    target_compile_definitions(rendu-compile-option-interface
            INTERFACE
            _WIN32_WINNT=0x0A00                     # Windows 10
            NTDDI_VERSION=0x0A000007                # 19H1 (1903)
            WIN32_LEAN_AND_MEAN
            NOMINMAX
            RENDU_REQUIRED_WINDOWS_BUILD=18362
    )

    # 设置可执行文件和 DLL 的输出目录
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

    # 为所有可执行目标添加 WindowsSettings.manifest
    target_sources(rendu-core-interface
            INTERFACE
            $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${CMAKE_SOURCE_DIR}/cmake/platform/win/WindowsSettings.manifest>
    )
endfunction()

rendu_setup_win_options()