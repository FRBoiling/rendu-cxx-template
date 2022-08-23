#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 为可执行二进制文件设置输出路径(在支持dll的平台上.exe文件和.dll文件)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置build-directive(在core中用来告诉我们使用了哪种build类型)
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -D_BUILD_DIRECTIVE="$<CONFIG>")

if(PLATFORM EQUAL 32)
  # 32位系统上需要启用SSE2 (x64标准)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -msse2
      -mfpmath=sse)
endif()
target_compile_definitions(rendu-compile-option-interface
  INTERFACE
    -DHAVE_SSE2
    -D__SSE2__)
message(STATUS "GCC: SFMT 开启, 强制启用 SSE2 ")

if(WITH_WARNINGS)
  target_compile_options(rendu-warning-interface
    INTERFACE
      -W
      -Wall
      -Wextra
      -Winit-self
      -Winvalid-pch
      -Wfatal-errors
      -Woverloaded-virtual)

  message(STATUS "GCC: 启用所有警告")
endif()

if(WITH_COREDEBUG)
  target_compile_options(rendu-compile-option-interface
    INTERFACE
      -g3)

  message(STATUS "GCC: Debug-flags set (-g3)")
endif()
