# CheckBuildDir.cmake
# =========================
# 强制要求 out-of-source 构建
# =========================

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" RENDU_BUILDING_IN_SOURCE)

if (RENDU_BUILDING_IN_SOURCE)
    rendu_log_fatal("
    本项目强制要求使用源码树外(out-of-source)构建。
    请先删除本目录下的 'CMakeCache.txt' 文件，
    然后新建一个独立的构建目录，并在该目录下运行：
      cmake path_to_project [options]
  ")
endif ()
