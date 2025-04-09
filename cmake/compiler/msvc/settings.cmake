# RenduCore - CMake settings for MSVC compiler
# =========================
# MSVC 编译器相关设置
# =========================
function(rendu_setup_msvc_options)
    set(RENDU_MSVC_EXPECTED_VERSION 19.32)
    set(RENDU_MSVC_EXPECTED_VERSION_STRING "Microsoft Visual Studio 2022 17.2")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS RENDU_MSVC_EXPECTED_VERSION)
            rendu_log_fatal("MSVC: RenduCore requires version ${RENDU_MSVC_EXPECTED_VERSION} (${RENDU_MSVC_EXPECTED_VERSION_STRING}) to build but found ${CMAKE_CXX_COMPILER_VERSION}")
        else ()
            rendu_log_info("MSVC: Minimum version required is ${RENDU_MSVC_EXPECTED_VERSION}, found ${CMAKE_CXX_COMPILER_VERSION} - ok!")
        endif ()
    endif ()

    # 移除 CMake 默认警告级别，统一由 interface target 控制
    string(REGEX REPLACE "/W[0-4] " "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/W[0-4]$" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/W[0-4] " "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W[0-4]$" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

    target_compile_options(rendu-warning-interface
            INTERFACE
            /W3)

    # 更严格的标准模式
    target_compile_options(rendu-compile-option-interface
            INTERFACE
            /permissive-)

    if (RENDU_PLATFORM EQUAL 32)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
        rendu_log_info("MSVC: Enabled large address awareness")

        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /arch:SSE2)
        rendu_log_info("MSVC: Enabled SSE2 support")

        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
        rendu_log_info("MSVC: Disabled Safe Exception Handlers for debug builds")
    endif ()

    # 多线程编译
    if ("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /MP)
    else ()
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                $<$<CONFIG:Debug,RelWithDebInfo>:/FS /Zf>)
    endif ()

    # 支持大对象文件
    if ((RENDU_PLATFORM EQUAL 64) OR (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.23026.0) OR BUILD_SHARED_LIBS)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /bigobj)
        rendu_log_info("MSVC: Enabled increased number of sections in object files")
    endif ()

    # 优化 new 行为
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /Zc:throwingNew)
    endif ()

    # 安全相关宏定义
    target_compile_definitions(rendu-compile-option-interface
            INTERFACE
            _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
            _CRT_SECURE_NO_WARNINGS
            _CRT_NONSTDC_NO_WARNINGS
            _USE_MATH_DEFINES)

    rendu_log_info("MSVC: 安全相关宏定义已设置")

    # 忽略部分警告
    target_compile_options(rendu-compile-option-interface
            INTERFACE
            /wd4351  # C4351: new behavior: elements of array 'x' will be default initialized
            /wd4091) # C4091: 'typedef ': ignored on left of '' when no variable is declared

    if (NOT RENDU_WITH_WARNINGS)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /wd4996  # C4996 deprecation
                /wd4985  # C4985 'symbol-name': attributes not present on previous declaration.
                /wd4244  # C4244 'argument' : conversion from 'type1' to 'type2', possible loss of data
                /wd4267  # C4267 'var' : conversion from 'size_t' to 'type', possible loss of data
                /wd4619  # C4619 #pragma warning : there is no warning number 'number'
                /wd4512) # C4512 'class' : assignment operator could not be generated

        rendu_log_info("MSVC: Disabled generic compiletime warnings")
    endif ()

    if (BUILD_SHARED_LIBS)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /wd4251  # C4251: needs to have dll-interface to be used by clients of class '...'
                /wd4275) # C4275: non dll-interface class ...' used as base for dll-interface class '...'
        rendu_log_info("MSVC: Enabled shared linking")
    endif ()

    # 额外警告和错误
    target_compile_options(rendu-compile-option-interface
            INTERFACE
            /w15038  # C5038: data member 'member1' will be initialized after data member 'member2'
            /w34100  # C4100: 'identifier' : unreferenced formal parameter
            /w34101  # C4101: 'identifier' : unreferenced local variable
            /w34189  # C4189: 'identifier' : local variable is initialized but not referenced
            /w34389  # C4389: 'equality-operator' : signed/unsigned mismatch
            /w35054) # C5054: 'operator 'operator-name': deprecated between enumerations of different types'

    target_compile_options(rendu-compile-option-interface
            INTERFACE
            /we4263  # treat as error: member function does not override any base class virtual member function
            /we4264) # treat as error: no override available for virtual member function from base

    # AddressSanitizer 支持
    if (RENDU_ASAN)
        target_compile_definitions(rendu-compile-option-interface
                INTERFACE
                _DISABLE_STRING_ANNOTATION
                _DISABLE_VECTOR_ANNOTATION)
        target_compile_options(rendu-compile-option-interface
                INTERFACE
                /fsanitize=address)
        rendu_log_info("MSVC: Enabled Address Sanitizer ASan")
    endif ()

    # 禁用增量链接，防止 debug 链接卡死
    macro(rendu_disable_incremental_linking variable)
        string(REGEX REPLACE "/INCREMENTAL *" "" ${variable} "${${variable}}")
        set(${variable} "${${variable}} /INCREMENTAL:NO")
    endmacro()

    rendu_disable_incremental_linking(CMAKE_EXE_LINKER_FLAGS_DEBUG)
    rendu_disable_incremental_linking(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)
    rendu_disable_incremental_linking(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
    rendu_disable_incremental_linking(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)

    # 兼容 VS2022 构建管理
    if (NOT MSVC_TOOLSET_VERSION LESS 143)
        file(COPY "${CMAKE_CURRENT_LIST_DIR}/Directory.Build.props" DESTINATION "${CMAKE_BINARY_DIR}")
    endif ()
endfunction()

rendu_setup_msvc_options()