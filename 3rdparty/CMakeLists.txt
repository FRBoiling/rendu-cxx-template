#**********************************
#  Created by boil on 2022/8/14.
#**********************************

get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${FOLDER_NAME})

add_subdirectory(threads)

if(SERVERS OR TOOLS)
  add_subdirectory(boost)
  add_subdirectory(zlib)
  add_subdirectory(g3dlite)
  add_subdirectory(recastnavigation)
  add_subdirectory(fmt)
  add_subdirectory(SFMT)
  add_subdirectory(utf8cpp)
  add_subdirectory(valgrind)
  add_subdirectory(openssl)
  add_subdirectory(jemalloc)
  add_subdirectory(argon2)
  add_subdirectory(short_alloc)
endif()

if(SERVERS)
  add_subdirectory(mysql)
  add_subdirectory(readline)
  add_subdirectory(gsoap)
  add_subdirectory(rapidjson)
  add_subdirectory(efsw)
  add_subdirectory(protobuf)
endif()

if(TOOLS)
  add_subdirectory(CascLib)
endif()

if(BUILD_TESTING)
  add_subdirectory(catch)
endif()