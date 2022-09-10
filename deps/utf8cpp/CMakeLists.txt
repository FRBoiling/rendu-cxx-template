#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target-name utf8cpp)
set(project-name deps)

add_library(${target-name} INTERFACE)

target_include_directories(${target-name}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})
