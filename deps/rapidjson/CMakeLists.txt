#**********************************
#  Created by boil on 2022/8/14.
#**********************************

if(POLICY CMP0025)
  # detect Apple's Clang
  cmake_policy(SET CMP0025 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

set(LIB_MAJOR_VERSION "1")
set(LIB_MINOR_VERSION "1")
set(LIB_PATCH_VERSION "0")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")

if (CMAKE_VERSION VERSION_LESS 3.0)
  PROJECT(RapidJSON CXX)
else()
  cmake_policy(SET CMP0048 NEW)
  PROJECT(RapidJSON VERSION "${LIB_VERSION_STRING}" LANGUAGES CXX)
endif()

# compile in release with debug info mode by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(RAPIDJSON_BUILD_ASAN "Build rapidjson with address sanitizer (gcc/clang)" OFF)
option(RAPIDJSON_BUILD_UBSAN "Build rapidjson with undefined behavior sanitizer (gcc/clang)" OFF)

option(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT "Build rapidjson with -march or -mcpu options" ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT AND NOT CMAKE_CROSSCOMPILING)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "powerpc" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc64le")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=native")
    else()
      #FIXME: x86 is -march=native, but doesn't mean every arch is this option. To keep original project's compatibility, I leave this except POWER.
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  set(EXTRA_CXX_FLAGS -Weffc++ -Wswitch-default -Wfloat-equal -Wconversion -Wsign-conversion)
  if (RAPIDJSON_BUILD_CXX11 AND CMAKE_VERSION VERSION_LESS 3.1)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.7.0")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
  elseif (RAPIDJSON_BUILD_CXX17 AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  endif()
  if (RAPIDJSON_BUILD_ASAN)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.0")
      message(FATAL_ERROR "GCC < 4.8 doesn't support the address sanitizer")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    endif()
  endif()
  if (RAPIDJSON_BUILD_UBSAN)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
      message(FATAL_ERROR "GCC < 4.9 doesn't support the undefined behavior sanitizer")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    endif()
  endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(NOT CMAKE_CROSSCOMPILING)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "powerpc" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "ppc64le")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=native")
    else()
      #FIXME: x86 is -march=native, but doesn't mean every arch is this option. To keep original project's compatibility, I leave this except POWER.
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-missing-field-initializers")
  set(EXTRA_CXX_FLAGS -Weffc++ -Wswitch-default -Wfloat-equal -Wconversion -Wimplicit-fallthrough)
  if (RAPIDJSON_BUILD_CXX11 AND CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif (RAPIDJSON_BUILD_CXX17 AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  endif()
  if (RAPIDJSON_BUILD_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
  if (RAPIDJSON_BUILD_UBSAN)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    endif()
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  # CMake >= 3.10 should handle the above CMAKE_CXX_STANDARD fine, otherwise use /std:c++XX with MSVC >= 19.10
  if (RAPIDJSON_BUILD_CXX11 AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.10")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++11")
  elseif (RAPIDJSON_BUILD_CXX17 AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.14")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
  endif()
  # Always compile with /WX
  if(CMAKE_CXX_FLAGS MATCHES "/WX-")
    string(REGEX REPLACE "/WX-" "/WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
  endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "XL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qarch=auto")
endif()

#add extra search paths for libraries and includes
SET(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Directory where lib will install")
SET(DOC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}" CACHE PATH "Path to the documentation")

IF(UNIX OR CYGWIN)
  SET(_CMAKE_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME}")
ELSEIF(WIN32)
  SET(_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake")
ENDIF()
SET(CMAKE_INSTALL_DIR "${_CMAKE_INSTALL_DIR}" CACHE PATH "The directory cmake files are installed in")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Provide config and version files to be used by other applications
# ===============================

################################################################################
# Export package for use from the build tree
EXPORT( PACKAGE ${PROJECT_NAME} )

# Create the RapidJSONConfig.cmake file for other cmake projects.
# ... for the build tree
SET( CONFIG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET( CONFIG_DIR ${CMAKE_CURRENT_BINARY_DIR})
SET( ${PROJECT_NAME}_INCLUDE_DIR "\${${PROJECT_NAME}_SOURCE_DIR}/include" )

# ... for the install tree
SET( CMAKECONFIG_INSTALL_DIR ${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME} )
FILE( RELATIVE_PATH REL_INCLUDE_DIR
    "${CMAKECONFIG_INSTALL_DIR}"
    "${CMAKE_INSTALL_PREFIX}/include" )

SET( ${PROJECT_NAME}_INCLUDE_DIR "\${${PROJECT_NAME}_CMAKE_DIR}/${REL_INCLUDE_DIR}" )
SET( CONFIG_SOURCE_DIR )
SET( CONFIG_DIR )

SET(target-name rapidjson)
set(project-name deps)

add_library(${target-name} INTERFACE)

target_include_directories(${target-name}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${target-name}
    PROPERTIES FOLDER
    ${project-name}
    )
set_target_properties(${target-name}
    PROPERTIES LINKER_LANGUAGE
    CXX
    )




