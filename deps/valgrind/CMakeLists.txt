#**********************************
#  Created by boil on 2022/8/14.
#**********************************

add_library(valgrind INTERFACE)

target_include_directories(valgrind
    INTERFACE
    "${VALGRIND_INCLUDE_DIR}")
