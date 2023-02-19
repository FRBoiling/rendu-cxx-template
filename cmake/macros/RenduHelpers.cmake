#**********************************
#  Created by boil on 2022/10/19.
#**********************************

include(CMakeParseArguments)
function(_rendu_target_compile_features_if_available TARGET TYPE FEATURE)
  if (FEATURE IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    target_compile_features(${TARGET} ${TYPE} ${FEATURE})
  else ()
    message(WARNING "Feature ${FEATURE} is unknown for the CXX compiler")
  endif ()
endfunction(_rendu_target_compile_features_if_available)

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
    [==[
#ifdef _MSC_VER
#  if _MSVC_LANG < 201700L
#    error "The compiler defaults or is configured for C++ < 17"
#  endif
#elif __cplusplus < 201700L
#  error "The compiler defaults or is configured for C++ < 17"
#endif
int main() { return 0; }
]==]
    RENDU_INTERNAL_AT_LEAST_CXX17)

if (RENDU_INTERNAL_AT_LEAST_CXX17)
  set(RENDU_INTERNAL_CXX_STD_FEATURE cxx_std_17)
else ()
  set(RENDU_INTERNAL_CXX_STD_FEATURE cxx_std_14)
endif ()

# 将所有子目录添加。忽略可变参数中列出的完全限定目录。
# Use it like:
# rendu_add_subdirectory(
#   ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
#   ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/platform)
#
function(rendu_add_subdirectory current_dir)
  list(FIND ARGN "${current_dir}" IS_EXCLUDED)
  if (IS_EXCLUDED EQUAL -1)
    list(APPEND DIRS ${current_dir})
    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach (SUB_DIRECTORY ${SUB_DIRECTORIES})
      if (IS_DIRECTORY ${SUB_DIRECTORY})
        list(FIND ARGN "${SUB_DIRECTORY}" IS_EXCLUDED)
        if (IS_EXCLUDED EQUAL -1)
          get_filename_component(element_name ${SUB_DIRECTORY} NAME)
          add_subdirectory(${element_name})
        endif ()
      endif ()
    endforeach ()
  endif ()
endfunction(rendu_add_subdirectory)

# Parameters:
# NAME: name of target (see Note)
# HDRS: List of public header files for the library
# SRCS: List of source files for the library
# PUBLIC_DEPS: List of other libraries to be linked in to the binary targets
# PRIVATE_DEPS: List of other libraries to be linked in to the binary targets
# COPTS: List of private compile options
# DEFINES: List of public defines
# LINKOPTS: List of link options
# PUBLIC: Add this so that this library will be exported under rendu::
#
# rendu_add_library(
#   NAME
#     awesome
#   HDRS
#     "a.h"
#   SRCS
#     "a.cc"
# )
# rendu_add_library(
#   NAME
#     fantastic_lib
#   SRCS
#     "b.cc"
#   DEPS
#     rendu::awesome # not "awesome" !
#   PUBLIC
# )
#
# rendu_add_library(
#   NAME
#     main_lib
#   ...
#   DEPS
#     rendu::fantastic_lib
# )
#
# rendu_add_executable(
#   NAME
#     awesome_main
#   SRCS
#     "awesome_main.cc"
#   DEPS
#     rendu::awesome
#     GTest::gmock
#     GTest::gtest_main
# )
function(rendu_add_library)
  cmake_parse_arguments(RENDU_LIB
      ""
      "NAME"
      "CMAKE_CUR_SOURCE_DIR;CMAKE_CUR_BINARY_DIR;CMAKE_BINARY_DIR;COPTS;DEFINES;LINKOPTS;PUBLIC_DEPS;PRIVATE_DEPS"
      ${ARGN}
      )
  set(_NAME "${PROJECT_NAME}_${RENDU_LIB_NAME}")

  message(STATUS "[lib ] " ${_NAME})
  CollectAllFiles(
      ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}
      PRIVATE_SOURCES
      # Exclude
      ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
  )

  list(APPEND PRIVATE_SOURCES
      #      ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/Debugging/Errors.cpp
      #      ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/Debugging/Errors.h
      )

  set(_SRCS "${PRIVATE_SOURCES}")
  foreach (src_file IN LISTS _SRCS)
    if (${src_file} MATCHES ".*\\.(h|hpp|inc)")
      list(REMOVE_ITEM _SRCS "${src_file}")
    endif ()
  endforeach ()

  if (_SRCS STREQUAL "")
    set(LIB_IS_INTERFACE 1)
  else ()
    set(LIB_IS_INTERFACE 0)
  endif ()

  if (USE_COREPCH)
    CollectHeaderFiles(
        ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
        PRIVATE_PCH_HEADER
    )
  endif (USE_COREPCH)

  GroupSources(${RENDU_LIB_CMAKE_CUR_SOURCE_DIR})
  #  add_definitions(-DTRINITY_API_EXPORT_COMMON)

  if (LIB_IS_INTERFACE) #TODO:BOIL head only
    add_library(${_NAME} INTERFACE)
    target_sources(${_NAME} INTERFACE ${PRIVATE_SOURCES})
    target_compile_features(${_NAME}
        INTERFACE
        cxx_std_11
        )

    target_include_directories(${_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}>
        # Provide the binary dir for all child targets
        ${RENDU_LIB_CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        ${RENDU_LIB_CMAKE_CUR_BINARY_DIR})

    target_link_libraries(${_NAME}
        INTERFACE
        #
        ${RENDU_LIB_PRIVATE_DEPS}
        ${RENDU_LIB_PUBLIC_DEPS}
        )

  else ()
    add_library(${_NAME})
    target_sources(${_NAME} PRIVATE ${PRIVATE_SOURCES})
    CollectIncludeDirectories(
        ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}
        PUBLIC_INCLUDES
        # Exclude
        ${RENDU_LIB_CMAKE_CUR_SOURCE_DIR}/precompiled_headers)

    target_include_directories(${_NAME}
        PUBLIC
        # Provide the binary dir for all child targets
        ${RENDU_LIB_CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        PRIVATE
        ${RENDU_LIB_CMAKE_CUR_BINARY_DIR})

    target_link_libraries(${_NAME}
        PRIVATE
        #
        ${RENDU_LIB_PRIVATE_DEPS}
        PUBLIC
        ${RENDU_LIB_PUBLIC_DEPS}
        )

  endif ()
  target_compile_definitions(${_NAME} PUBLIC ${RENDU_LIB_DEFINES})
  #  add_dependencies(${_NAME} ${RENDU_LIB_CMAKE_BINARY_DIR}/revision_data.h)
  set_target_properties(${_NAME}
      PROPERTIES
      FOLDER
      ${RENDU_LIB_NAME})

  if (BUILD_SHARED_LIBS)
    message(STATUS "build shared libs")
    if (UNIX)
      install(TARGETS ${_NAME}
          LIBRARY
          DESTINATION lib)
    elseif (WIN32)
      install(TARGETS ${_NAME}
          RUNTIME
          DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif ()
  endif ()

  # Generate precompiled header
  if (USE_COREPCH)
    message(STATUS "use core pch ")
    set(_header "${PRIVATE_PCH_HEADER}")
    if (_header STREQUAL "")
    else ()
      add_cxx_pch(${_NAME} ${PRIVATE_PCH_HEADER})
    endif ()
  endif ()
  add_library(${PROJECT_NAME}::${RENDU_LIB_NAME} ALIAS ${_NAME})
endfunction(rendu_add_library)

function(rendu_add_executable)
  cmake_parse_arguments(RENDU_EXEC
      ""
      "NAME"
      "CMAKE_CUR_SOURCE_DIR;CMAKE_CUR_BINARY_DIR;CMAKE_BINARY_DIR;COPTS;DEFINES;LINKOPTS;PUBLIC_DEPS;PRIVATE_DEPS"
      ${ARGN}
      )
  set(_NAME "${PROJECT_NAME}_${RENDU_EXEC_NAME}")
  message(STATUS "[exec] " ${_NAME})
  CollectAllFiles(
      ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}
      PRIVATE_SOURCES
      # Exclude
      ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
  )

  list(APPEND PRIVATE_SOURCES
      #      ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}/Debugging/Errors.cpp
      #      ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}/Debugging/Errors.h
      )

  if (USE_COREPCH)
    CollectHeaderFiles(
        ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
        PRIVATE_PCH_HEADER
    )
    #    set(PRIVATE_PCH_HEADER precompiled_headers/${RENDU_EXEC_NAME}_pch.h)
  endif (USE_COREPCH)

  GroupSources(${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR})
  add_executable(${_NAME} ${PRIVATE_SOURCES})
  target_link_libraries(${_NAME}
      PRIVATE
      ${RENDU_EXEC_PRIVATE_DEPS}
      PUBLIC
      ${RENDU_EXEC_PUBLIC_DEPS}
      )
  CollectIncludeDirectories(
      ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}
      PUBLIC_INCLUDES
      # Exclude
      ${RENDU_EXEC_CMAKE_CUR_SOURCE_DIR}/precompiled_headers)
  target_include_directories(${_NAME}
      PUBLIC
      # Provide the binary dir for all child targets
      ${RENDU_EXEC_CMAKE_BINARY_DIR}
      ${PUBLIC_INCLUDES}
      PRIVATE
      ${RENDU_EXEC_CMAKE_CUR_BINARY_DIR})
  set_target_properties(${_NAME}
      PROPERTIES
      FOLDER
      ${RENDU_EXEC_NAME})

  if (BUILD_SHARED_LIBS)
    if (UNIX)
      install(TARGETS ${_NAME}
          LIBRARY
          DESTINATION lib)
    elseif (WIN32)
      install(TARGETS ${_NAME}
          RUNTIME
          DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif ()
  endif ()
  # Generate precompiled header
  if (USE_COREPCH)
    set(_header "${PRIVATE_PCH_HEADER}")
    if (_header STREQUAL "")
    else ()
      add_cxx_pch(${_NAME} ${PRIVATE_PCH_HEADER})
    endif ()
  endif ()
endfunction(rendu_add_executable)

function(rendu_add_test)
  cmake_parse_arguments(RENDU_TEST
      ""
      "NAME"
      "CMAKE_CUR_SOURCE_DIR;CMAKE_CUR_BINARY_DIR;CMAKE_BINARY_DIR;COPTS;DEFINES;LINKOPTS;DEPS"
      ${ARGN}
      )
  set(_NAME "${PROJECT_NAME}_${RENDU_TEST_NAME}")
  message(STATUS "[test] " ${_NAME})
  CollectAllFiles(
      ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}
      PRIVATE_SOURCES
      # Exclude
      ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
  )

  list(APPEND PRIVATE_SOURCES
      #      ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}/Debugging/Errors.cpp
      #      ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}/Debugging/Errors.h
      )

  if (USE_COREPCH)
    CollectHeaderFiles(
        ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}/precompiled_headers
        PRIVATE_PCH_HEADER
    )
    #    set(PRIVATE_PCH_HEADER precompiled_headers/${RENDU_TEST_NAME}_pch.h)
  endif (USE_COREPCH)

  GroupSources(${RENDU_TEST_CMAKE_CUR_SOURCE_DIR})
  add_executable(${_NAME} "")
  target_sources(${_NAME} PRIVATE ${PRIVATE_SOURCES})
  target_link_libraries(${_NAME}
      PRIVATE
      rendu-core-interface
      PUBLIC
      ${RENDU_TEST_DEPS}
      )
  CollectIncludeDirectories(
      ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}
      PUBLIC_INCLUDES
      # Exclude
      ${RENDU_TEST_CMAKE_CUR_SOURCE_DIR}/precompiled_headers)
  target_include_directories(${_NAME}
      PUBLIC
      # Provide the binary dir for all child targets
      ${RENDU_TEST_CMAKE_BINARY_DIR}
      ${PUBLIC_INCLUDES}
      PRIVATE
      ${RENDU_TEST_CMAKE_CUR_BINARY_DIR})
  set_target_properties(${_NAME}
      PROPERTIES
      FOLDER
      ${RENDU_TEST_NAME})

  if (BUILD_SHARED_LIBS)
    if (UNIX)
      install(TARGETS ${_NAME}
          LIBRARY
          DESTINATION lib)
    elseif (WIN32)
      install(TARGETS ${_NAME}
          RUNTIME
          DESTINATION "${CMAKE_INSTALL_PREFIX}")
    endif ()
  endif ()
  # Generate precompiled header
  if (USE_COREPCH)
    set(_header "${PRIVATE_PCH_HEADER}")
    if (_header STREQUAL "")
    else ()
      add_cxx_pch(${_NAME} ${PRIVATE_PCH_HEADER})
    endif ()
  endif ()
  add_test(NAME ${_NAME} COMMAND ${_NAME})
endfunction(rendu_add_test)


