#**********************************
#  Created by boil on 2022/8/14.
#**********************************
set(PROJECT_NAME rendu-core)
cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)
# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the Project Name and Description
# Set projectname (must be done AFTER setting configurationtypes)
project(r${PROJECT_NAME} LANGUAGES CXX DESCRIPTION "moderncpp-project is a starter project for Modern C++")
# project(rendu-core)

# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 NEW)
cmake_policy(SET CMP0043 NEW) # Cotire isn't used so set to NEW
cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if(SOME_STRING_VARIABLE MATCHES "MSVC") as if(SOME_STRING_VARIABLE MATCHES "1")
cmake_policy(SET CMP0074 NEW) # find_package() uses <PackageName>_ROOT variables

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
  set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(ConfigureScripts)

# set default buildoptions and print them
include(cmake/options.cmake)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if(NOPCH)
  set(USE_COREPCH 0)
  set(USE_SCRIPTPCH 0)
endif()

include(ConfigureBaseTargets)
include(CheckPlatform)

include(GroupSources)
include(AutoCollect)

find_package(PCHSupport)
find_package(Git 1.7)

# find mysql client binary (needed by genrev)
find_package(MySQL OPTIONAL_COMPONENTS binary)

# Find revision ID and hash of the sourcetree
include(cmake/genrev.cmake)

# print out the results before continuing
include(cmake/showoptions.cmake)

# add dependencies
add_subdirectory(deps)
# add core sources
add_subdirectory(src)

if(BUILD_TESTING)
  include(CTest)
  list(APPEND CMAKE_MODULE_PATH  "${Catch2_SOURCE_DIR}/contrib")
  include(Catch)
  add_subdirectory(tests)
  # Catch cmakefile messes with our settings we explicitly leave up to the user restore user preference
  if (NOT WITH_SOURCE_TREE STREQUAL "hierarchical-folders")
    set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
  endif()
endif()
