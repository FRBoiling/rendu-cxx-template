#**********************************
#  Created by boil on 2022/8/14.
#**********************************

find_package(Threads REQUIRED)

add_library(threads INTERFACE)
target_link_libraries(threads
    INTERFACE
    ${CMAKE_THREAD_LIBS_INIT})
