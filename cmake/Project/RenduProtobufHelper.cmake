#**********************************
#  Created by boil on 2025/02/21.
#**********************************
#**********************************
# 协议缓冲区支持宏 (v3.0)
# 支持：C++/C# 协议代码生成
#**********************************

##例子： C++协议生成
#rendu_add_protobuf(
#    PROJECT MyApp
#    NAME messages
#    PROTOC $<TARGET_FILE:protobuf::protoc>
#    INPUT_DIRS "proto"
#    OUTPUT_DIR "${CMAKE_BINARY_DIR}/generated"
#)
#
## 使用生成文件
#add_library(proto_lib
#    ${messages_GENERATED_SRCS}
#)


function(rendu_add_protobuf)
  cmake_parse_arguments(RD
      "CSHARP"
      "PROJECT;NAME;PROTOC;OUTPUT_DIR"
      "INPUT_DIRS;LANG_OPTS"
      ${ARGN}
  )

  #====================== 参数校验 ======================#
  if(NOT RD_PROTOC)
    message(FATAL_ERROR "必须指定PROTOC参数（protoc编译器路径）")
  endif()
  if(NOT RD_INPUT_DIRS)
    message(FATAL_ERROR "必须指定INPUT_DIRS参数（proto文件搜索目录）")
  endif()

  #====================== 目录准备 ======================#
  get_filename_component(OUTPUT_DIR_ABS "${RD_OUTPUT_DIR}" ABSOLUTE)
  if(NOT IS_DIRECTORY "${OUTPUT_DIR_ABS}")
    file(MAKE_DIRECTORY "${OUTPUT_DIR_ABS}")
  endif()

  #====================== 文件收集 ======================#
  set(proto_files "")
  foreach(dir IN LISTS RD_INPUT_DIRS)
    get_filename_component(ABS_DIR "${dir}" ABSOLUTE)
    rendu_collect_proto_files("${ABS_DIR}" proto_files)
  endforeach()

  if(NOT proto_files)
    message(WARNING "在目录 ${RD_INPUT_DIRS} 中未找到任何.proto文件")
    return()
  endif()

  #====================== 生成配置 ======================#
  set(generated_files "")
  set(include_options "-I${CMAKE_CURRENT_SOURCE_DIR}")
  foreach(dir IN LISTS RD_INPUT_DIRS)
    list(APPEND include_options "-I${dir}")
  endforeach()

  #====================== 编译命令 ======================#
  foreach(proto IN LISTS proto_files)
    get_filename_component(file_we "${proto}" NAME_WE)

    # C++代码生成
    if(NOT RD_CSHARP)
      set(src "${OUTPUT_DIR_ABS}/${file_we}.pb.cc")
      set(hdr "${OUTPUT_DIR_ABS}/${file_we}.pb.h")
      set(lang_opt "--cpp_out=${OUTPUT_DIR_ABS}")
      list(APPEND generated_files "${src}" "${hdr}")

      # C#代码生成
    else()
      set(src "${OUTPUT_DIR_ABS}/${file_we}.cs")
      set(lang_opt "--csharp_out=${OUTPUT_DIR_ABS}")
      list(APPEND generated_files "${src}")
    endif()

    add_custom_command(
        OUTPUT "${src}" ${hdr}
        COMMAND "${RD_PROTOC}"
        ARGS ${lang_opt} ${include_options} "${proto}"
        DEPENDS "${proto}"
        COMMENT "生成协议代码: ${proto}"
    )
  endforeach()

  #====================== 生成目标 ======================#
  set(target "${RD_PROJECT}_proto_${RD_NAME}")
  add_custom_target(${target} ALL
      DEPENDS ${generated_files}
      COMMENT "协议缓冲区编译目标: ${target}"
  )

  # 标记生成文件属性
  set_source_files_properties(${generated_files} PROPERTIES GENERATED TRUE)

  #====================== 输出结果 ======================#
  set(${RD_NAME}_GENERATED_SRCS ${generated_files} PARENT_SCOPE)
  message(STATUS "生成协议文件: ${generated_files}")
endfunction()

