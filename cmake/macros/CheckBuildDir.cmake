#**********************************
#  Created by boil on 2022/8/14.
#**********************************

#
# Force out-of-source build
#
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" BUILDING_IN_SOURCE)

if(BUILDING_IN_SOURCE)
  message(FATAL_ERROR "
  此项目需要使用非源代码构建。
  删除文件'CMakeCache.txt'。
  在继续之前，创建一个单独的构建目录，
  然后从那里运行'cmake path_to_project [options]'。")
endif()
# This project requires an out of source build. Remove the file 'CMakeCache.txt'
# found in this directory before continuing, create a separate build directory
# and run 'cmake path_to_project [options]' from there.