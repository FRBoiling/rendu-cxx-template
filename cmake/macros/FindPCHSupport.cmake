#FindPCHSupport.cmake

# =========================
# 预编译头支持工具函数
# =========================
#
# 用法示例:
# rendu_add_cxx_pch(TARGETS my_target1 my_target2 HEADER "${CMAKE_SOURCE_DIR}/src/precompiled.h")
# rendu_reuse_cxx_pch(TARGETS other_target1 other_target2 REUSE_FROM my_target1)
#
# rendu_add_library(
#     DIR       ${CMAKE_CURRENT_SOURCE_DIR}
#     NAME      mylib
#     # ... 其他参数 ...
# )
# rendu_add_cxx_pch(TARGETS mylib HEADER "${CMAKE_SOURCE_DIR}/src/precompiled.h")


function(rendu_add_cxx_pch)
  set(options)
  set(oneValueArgs HEADER)
  set(multiValueArgs TARGETS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT ARG_TARGETS OR NOT ARG_HEADER)
    message(FATAL_ERROR "rendu_add_cxx_pch: 必须指定 TARGETS 和 HEADER")
  endif()

  foreach(target IN LISTS ARG_TARGETS)
    target_precompile_headers(${target} PRIVATE ${ARG_HEADER})
  endforeach()
endfunction()

function(rendu_reuse_cxx_pch)
  set(options)
  set(oneValueArgs REUSE_FROM)
  set(multiValueArgs TARGETS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT ARG_TARGETS OR NOT ARG_REUSE_FROM)
    message(FATAL_ERROR "rendu_reuse_cxx_pch: 必须指定 TARGETS 和 REUSE_FROM")
  endif()

  foreach(target IN LISTS ARG_TARGETS)
    target_precompile_headers(${target} REUSE_FROM ${ARG_REUSE_FROM})
  endforeach()
endfunction()