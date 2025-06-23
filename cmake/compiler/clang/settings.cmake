# 设置 Clang 编译器相关选项
function(rendu_setup_clang_options)
    set(RENDU_CLANG_EXPECTED_VERSION 11.0.0)
    if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
        set(RENDU_CLANG_EXPECTED_VERSION 12.0.5)
    endif ()

    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS RENDU_CLANG_EXPECTED_VERSION)
        rendu_log_fatal("Clang: RenduCore requires version ${RENDU_CLANG_EXPECTED_VERSION} to build but found ${CMAKE_CXX_COMPILER_VERSION}")
    else ()
        rendu_log_info("Clang: Minimum version required is ${RENDU_CLANG_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
    endif ()

    if (RENDU_WITH_WARNINGS)
        target_compile_options(rendu-warning-interface
                INTERFACE
                -W
                -Wall
                -Wextra
                -Wimplicit-fallthrough
                -Winit-self
                -Wfatal-errors
                -Wno-mismatched-tags
                -Woverloaded-virtual
                -Wno-missing-field-initializers)
        rendu_log_info("Clang: All warnings enabled")
    endif ()

    if (RENDU_WITH_COREDEBUG)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -g3)
        rendu_log_info("Clang: Debug-flags set (-g3)")
    endif ()

    if (RENDU_ASAN)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=address
                -fsanitize-recover=address
                -fsanitize-address-use-after-scope)
        target_link_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=address
                -fsanitize-recover=address
                -fsanitize-address-use-after-scope)
        rendu_log_info("Clang: Enabled Address Sanitizer ASan")
    endif ()

    if (RENDU_MSAN)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=memory
                -fsanitize-memory-track-origins
                -mllvm
                -msan-keep-going=1)
        target_link_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=memory
                -fsanitize-memory-track-origins)
        rendu_log_info("Clang: Enabled Memory Sanitizer MSan")
    endif ()

    if (RENDU_UBSAN)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=undefined)
        target_link_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=undefined)
        rendu_log_info("Clang: Enabled Undefined Behavior Sanitizer UBSan")
    endif ()

    if (RENDU_TSAN)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=thread)
        target_link_options(rendu-compile-option-interface
                INTERFACE
                -fno-omit-frame-pointer
                -fsanitize=thread)
        rendu_log_info("Clang: Enabled Thread Sanitizer TSan")
    endif ()

    if (RENDU_BUILD_TIME_ANALYSIS)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -ftime-trace)
        rendu_log_info("Clang: Enabled build time analysis (-ftime-trace)")
    endif ()

    target_compile_options(rendu-compile-option-interface
            INTERFACE
            -Wno-narrowing
            -Wno-deprecated-register
            -Wno-undefined-inline)

    if (BUILD_SHARED_LIBS)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                -fPIC)
        target_compile_options(rendu-hidden-symbols-interface
                INTERFACE
                -fvisibility=hidden)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --no-undefined")
        rendu_log_info("Clang: Disallow undefined symbols")
    endif ()
endfunction()

rendu_setup_clang_options()