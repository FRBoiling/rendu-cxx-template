#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target-name protobuf)
set(project-name deps)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    protobuf_STAT_SRCS
    # Exclude
)

if (MSVC)
  set(protobuf_STAT_SRCS
      ${protobuf_STAT_SRCS}
      src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc
      )
else ()
  set(protobuf_STAT_SRCS
      ${protobuf_STAT_SRCS}
      src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc
      )
endif ()

add_library(protobuf ${protobuf_STAT_SRCS})

target_include_directories(protobuf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(protobuf
    PRIVATE
    rendu-default-interface
    rendu-no-warning-interface
    PUBLIC
    threads)

if (BUILD_SHARED_LIBS)
  target_compile_definitions(protobuf
      PRIVATE
      -DLIBPROTOBUF_EXPORTS
      -DLIBPROTOC_EXPORTS
      PUBLIC
      -DPROTOBUF_USE_DLLS)
endif ()

if (MSVC)
  target_compile_options(protobuf
      PRIVATE
      /wd4996)

  target_compile_definitions(protobuf
      PRIVATE
      -D_SCL_SECURE_NO_WARNINGS)
endif ()

set_target_properties(protobuf
    PROPERTIES
    FOLDER
    "deps")

if (BUILD_SHARED_LIBS)
  if (UNIX)
    install(TARGETS protobuf
        LIBRARY
        DESTINATION lib)
  elseif (WIN32)
    install(TARGETS protobuf
        RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif ()
endif ()
