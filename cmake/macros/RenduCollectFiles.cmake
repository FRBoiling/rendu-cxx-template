# CollectFiles.cmake

# ====================================================================
# 智能源文件收集器
# 
# 函数: rendu_collect_source_files
# 描述: 递归收集指定目录下的C/C++源文件和头文件，支持精确排除
#
# 参数:
#   output_var    - 输出变量名(存储结果的文件列表)
#   root_dir      - 搜索根目录(绝对路径)
#   [EXCLUDE_DIRS dir1 [dir2...]]  - 要排除的目录列表
#   [EXTENSIONS ext1 [ext2...]]    - 自定义文件扩展名(默认: .c,.cc,.cpp,.h,.hh,.hpp)
#
# 用法:
#   rendu_collect_source_files(MY_SOURCES
#       ${CMAKE_CURRENT_SOURCE_DIR}
#       EXCLUDE_DIRS 
#           ${CMAKE_CURRENT_SOURCE_DIR}/tests
#           ${CMAKE_BINARY_DIR}
#       EXTENSIONS  .c .cc .cpp .inl)
# ====================================================================

function(rendu_collect_source_files output_var root_dir)
    # 参数解析
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs EXCLUDE_DIRS EXTENSIONS)
    cmake_parse_arguments(PARSE_ARGV 2 ARG
            "${options}"
            "${oneValueArgs}"
            "${multiValueArgs}")

    # 设置默认扩展名
    if (NOT ARG_EXTENSIONS)
        set(ARG_EXTENSIONS .c .cc .cpp .h .hh .hpp .inl .def)
    endif ()

    # 生成GLOB模式
    set(patterns "")
    foreach (ext IN LISTS ARG_EXTENSIONS)
        list(APPEND patterns "${root_dir}/*${ext}")
    endforeach ()

    # 执行递归搜索
    file(GLOB_RECURSE files
            LIST_DIRECTORIES false
            RELATIVE "${root_dir}"
            ${patterns})

    # 过滤排除目录
    set(filtered_files "")
    foreach (file IN LISTS files)
        set(include_file TRUE)

        # 获取文件的绝对路径
        get_filename_component(abs_path "${root_dir}/${file}" ABSOLUTE)

        # 检查是否在排除目录中
        foreach (excl_dir IN LISTS ARG_EXCLUDE_DIRS)
            get_filename_component(excl_dir_abs "${excl_dir}" ABSOLUTE)
            string(FIND "${abs_path}" "${excl_dir_abs}/" pos)
            if (pos EQUAL 0)
                set(include_file FALSE)
                break()
            endif ()
        endforeach ()

        # 添加到结果
        if (include_file)
            list(APPEND filtered_files "${abs_path}")
        endif ()
    endforeach ()

    # 返回结果
    set(${output_var} ${filtered_files} PARENT_SCOPE)
endfunction()


# =========================
# 智能收集头文件（仅头文件）
#
# 函数: rendu_collect_header_files
# 描述: 递归收集指定目录下的C/C++头文件，支持精确排除
#
# 参数:
#   output_var    - 输出变量名(存储结果的文件列表)
#   root_dir      - 搜索根目录(绝对路径)
#   [EXCLUDE_DIRS dir1 [dir2...]]  - 要排除的目录列表
#   [EXTENSIONS ext1 [ext2...]]    - 自定义文件扩展名(默认: .c,.cc,.cpp,.h,.hh,.hpp)
#
# 用法:
#   rendu_collect_header_files(MY_SOURCES
#       ${CMAKE_CURRENT_SOURCE_DIR}
#       EXCLUDE_DIRS 
#           ${CMAKE_CURRENT_SOURCE_DIR}/tests
#           ${CMAKE_BINARY_DIR}
#       EXTENSIONS .h .hh .hpp .hxx)
# =========================
function(rendu_collect_header_files output_var root_dir)
    # 参数解析
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs EXCLUDE_DIRS EXTENSIONS)
    cmake_parse_arguments(PARSE_ARGV 2 ARG
            "${options}"
            "${oneValueArgs}"
            "${multiValueArgs}")

    # 设置默认扩展名（仅头文件）
    if (NOT ARG_EXTENSIONS)
        set(ARG_EXTENSIONS .h .hh .hpp .hxx .inl .def)
    endif ()

    # 调用通用收集函数
    rendu_collect_source_files(${output_var}
            ${root_dir}
            EXCLUDE_DIRS ${ARG_EXCLUDE_DIRS}
            EXTENSIONS ${ARG_EXTENSIONS})
endfunction()