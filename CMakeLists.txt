#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# **********************************
cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
set(PROJECT_NAME rendu)
# Define the Project Name and Description
# Set projectname (must be done AFTER setting configurationtypes)
#project(${PROJECT_NAME} LANGUAGES CXX DESCRIPTION "moderncpp-project is a starter project for Modern C++")
project(${PROJECT_NAME})

# **********************************
# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 NEW)
cmake_policy(SET CMP0043 NEW) # Cotire isn't used so set to NEW
cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if(SOME_STRING_VARIABLE MATCHES "MSVC") as if(SOME_STRING_VARIABLE MATCHES "1")
cmake_policy(SET CMP0074 NEW) # find_package() uses <PackageName>_ROOT variables

# set macro-directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")
include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(ConfigureScripts)
include(ConfigureBaseTargets)
include(CheckPlatform)
include(AutoAddSubdirectory)
include(GroupSources)
include(AutoCollect)

# set default buildoptions and print them
include(cmake/options.cmake)
# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)
# print out the results before continuing
include(cmake/showoptions.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
## find git binary
find_package(Git 1.7)
## find mysql client binary (needed by genrev)
find_package(MySQL OPTIONAL_COMPONENTS binary)
## find pch binary
find_package(PCHSupport)
if (NOPCH)
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif ()

# build in Release-mode by default if not explicitly set
if (CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
  set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif ()
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()
# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

AddSubdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/.git
    ${CMAKE_CURRENT_SOURCE_DIR}/.idea
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/build
)

# 测试模块构建
# **********************************
if (BUILD_TESTING)
  include(CTest)
  list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/contrib")
  include(Catch)
  add_subdirectory(tests)
  # Catch cmakefile messes with our settings we explicitly leave up to the user restore user preference
  if (NOT WITH_SOURCE_TREE STREQUAL "hierarchical-folders")
    set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
  endif ()
endif ()
