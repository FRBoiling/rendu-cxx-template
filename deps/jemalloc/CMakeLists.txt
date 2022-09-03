#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(LIB_NAME jemalloc)
set(FOLDER_NAME deps)

if (CMAKE_SYSTEM_NAME MATCHES "Linux" AND NOT NOJEM)
  # 我们需要根据平台特定的设置生成jemalloc_def.h头文件
  CHECK_SYMBOL_EXISTS(MADV_FREE "sys/mman.h" HAVE_MADV_FREE)

  if (PLATFORM EQUAL 32)
    set(JEM_SIZEDEF 2)
    set(JEM_TLSMODEL)
    set(JEM_VADDRBITS 32)
  else ()
    set(JEM_SIZEDEF 3)
    set(JEM_TLSMODEL "__attribute__\(\(tls_model\(\"initial-exec\"\)\)\)")
    set(JEM_VADDRBITS 48)
  endif ()

  if (HAVE_MADV_FREE)
    set(JEM_MADFREE_DEF "#define")
  else ()
    set(JEM_MADFREE_DEF "#undef")
  endif ()

  if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(JEM_CPU_SPINWAIT "")
    set(JEM_HAVE_CPU_SPINWAIT 0)
  else ()
    set(JEM_CPU_SPINWAIT "__asm__ volatile\(\"pause\"\)")
    set(JEM_HAVE_CPU_SPINWAIT 1)
  endif ()

  # Create the header, so we can use it
  configure_file(
      "${CMAKE_SOURCE_DIR}/deps/jemalloc/jemalloc_internal_defs.h.in.cmake"
      "${BUILDDIR}/jemalloc_internal_defs.h"
      @ONLY
  )

  # Done, let's continue
  CollectSourceFiles(
      ${CMAKE_CURRENT_SOURCE_DIR}
      jemalloc_STAT_SRC
      # Exclude
  )

  add_library(${LIB_NAME} STATIC ${jemalloc_STAT_SRC})

  target_include_directories(${LIB_NAME}
      PRIVATE
      ${BUILDDIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

  target_compile_definitions(${LIB_NAME}
      PUBLIC
      -DNO_BUFFERPOOL
      PRIVATE
      -D_GNU_SOURCE
      -D_REENTRAN)

  target_link_libraries(${LIB_NAME}
      PRIVATE
      rendu-dependency-interface
      PUBLIC
      threads
      valgrind
      ${CMAKE_DL_LIBS})

  set_target_properties(${LIB_NAME}
      PROPERTIES
      FOLDER
      ${FOLDER_NAME})

else ()
  # 为jemalloc提供一个虚拟目标，在禁用或不支持jemalloc时使用它。
  add_library(${LIB_NAME} INTERFACE)
  target_link_libraries(${LIB_NAME}
      INTERFACE
      valgrind)

endif ()
