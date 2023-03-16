#**********************************
#  Created by boil on 2022/8/14.
#**********************************
# 将所有源文件收集到给定变量中，用于包含子目录中的所有源。忽略可变参数中列出的完全限定目录。
# Use it like:
# CollectSourceFiles(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(CollectAllFiles current_dir variable)
  list(FIND ARGN ${current_dir} IS_EXCLUDED)
  if (IS_EXCLUDED EQUAL -1)
    file(GLOB COLLECTED_FILES
        ${current_dir}/*.c
        ${current_dir}/*.cc
        ${current_dir}/*.cpp
        ${current_dir}/*.pb.cc
        ${current_dir}/*.h
        ${current_dir}/*.hh
        ${current_dir}/*.hpp
        ${current_dir}/*.ipp
        ${current_dir}/*.pb.h
        ${current_dir}/*.inc
        )
    list(APPEND ${variable} ${COLLECTED_FILES})
    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach (SUB_DIRECTORY ${SUB_DIRECTORIES})
      if (IS_DIRECTORY ${SUB_DIRECTORY})
        CollectAllFiles(${SUB_DIRECTORY} ${variable} ${ARGN})
      endif ()
    endforeach ()
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction(CollectAllFiles)


# 将所有源文件收集到给定变量中，用于包含子目录中的所有源。忽略可变参数中列出的完全限定目录。
# Use it like:
# CollectSourceFiles(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(CollectSourceFiles current_dir variable)
  list(FIND ARGN ${current_dir} IS_EXCLUDED)
  if (IS_EXCLUDED EQUAL -1)
    file(GLOB COLLECTED_SOURCES
        ${current_dir}/*.c
        ${current_dir}/*.cc
        ${current_dir}/*.cpp
        ${current_dir}/*.pb.cc
        )
    list(APPEND ${variable} ${COLLECTED_SOURCES})
    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach (SUB_DIRECTORY ${SUB_DIRECTORIES})
      if (IS_DIRECTORY ${SUB_DIRECTORY})
        CollectSourceFiles(${SUB_DIRECTORY} ${variable} ${ARGN})
      endif ()
    endforeach ()
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction()

# 将所有源文件收集到给定变量中，用于包含子目录中的所有源。忽略可变参数中列出的完全限定目录。
# Use it like:
# CollectHeaderFiles(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(CollectHeaderFiles current_dir variable)
  list(FIND ARGN ${current_dir} IS_EXCLUDED)
  if (IS_EXCLUDED EQUAL -1)
    file(GLOB COLLECTED_HEADERS
        ${current_dir}/*.h
        ${current_dir}/*.hh
        ${current_dir}/*.hpp
        ${current_dir}/*.ipp
        ${current_dir}/*.pb.h
        ${current_dir}/*.inc
        )
    list(APPEND ${variable} ${COLLECTED_HEADERS})
    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach (SUB_DIRECTORY ${SUB_DIRECTORIES})
      if (IS_DIRECTORY ${SUB_DIRECTORY})
        CollectHeaderFiles(${SUB_DIRECTORY} ${variable} ${ARGN})
      endif ()
    endforeach ()
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction(CollectHeaderFiles)

# 收集所有子目录到给定的变量，用于包含所有子目录。忽略可变参数中列出的完全限定目录。
# Use it like:
# CollectIncludeDirectories(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PUBLIC_INCLUDES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(CollectIncludeDirectories current_dir variable)
  list(FIND ARGN "${current_dir}" IS_EXCLUDED)
  if (IS_EXCLUDED EQUAL -1)
    list(APPEND ${variable} ${current_dir})
    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach (SUB_DIRECTORY ${SUB_DIRECTORIES})
      if (IS_DIRECTORY ${SUB_DIRECTORY})
        list(APPEND ${variable} ${SUB_DIRECTORY})
#        CollectIncludeDirectories(${SUB_DIRECTORY} ${variable} ${ARGN})
      endif ()
    endforeach ()
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction(CollectIncludeDirectories)

# 将所有源文件收集到给定变量中，用于包含子目录中的所有源。忽略可变参数中列出的完全限定目录。
# Use it like:
# CollectProtoFiles(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers
#   ${CMAKE_CURRENT_SOURCE_DIR}/platform)
#
function(CollectProtoFiles current_dir variable)
  list(FIND ARGN ${current_dir} IS_EXCLUDED)
  if (IS_EXCLUDED EQUAL -1)
    file(GLOB COLLECTED_SOURCES
        ${current_dir}/*.proto
    )
    list(APPEND ${variable} ${COLLECTED_SOURCES})

    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach (SUB_DIRECTORY ${SUB_DIRECTORIES})
      if (IS_DIRECTORY ${SUB_DIRECTORY})
        CollectSourceFiles(${SUB_DIRECTORY} ${variable} ${ARGN})
      endif ()
    endforeach ()
    set(${variable} ${${variable}} PARENT_SCOPE)
  endif ()
endfunction(CollectProtoFiles)
