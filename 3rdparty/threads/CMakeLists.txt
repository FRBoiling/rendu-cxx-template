#**********************************
#  Created by boil on 2023/4/9.
#**********************************

find_package(Threads REQUIRED)

add_library(threads INTERFACE)

target_link_libraries(threads
  INTERFACE
    ${CMAKE_THREAD_LIBS_INIT})
