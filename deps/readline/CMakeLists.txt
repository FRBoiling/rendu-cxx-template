#**********************************
#  Created by boil on 2022/8/14.
#**********************************

add_library(readline INTERFACE)

if (UNIX)
  find_package(Readline REQUIRED)

  target_link_libraries(readline
      INTERFACE
      Readline::Readline)
endif ()
