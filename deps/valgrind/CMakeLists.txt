#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target-name valgrind)
add_library(${target-name} INTERFACE)

target_include_directories(${target-name}
    INTERFACE
    "${VALGRIND_INCLUDE_DIR}")

target_link_libraries(${target-name}
    INTERFACE rendu-dependency-interface
    #    PUBLIC
    )
