#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(LIB_NAME magic_enum)
set(FOLDER_NAME deps)

option(MAGIC_ENUM_OPT_ENABLE_NONASCII "Enable support for non-ASCII enumeration identifier" ON)
option(MAGIC_ENUM_OPT_BUILD_EXAMPLES "Build magic_enum examples" ON)

if (MAGIC_ENUM_OPT_BUILD_EXAMPLES)
  add_subdirectory(example)
endif ()

if (MAGIC_ENUM_OPT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif ()

include(CMakePackageConfigHelpers)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES
    # Exclude
)

add_subdirectory(example)

add_library(${LIB_NAME} STATIC ${SOURCES})
target_include_directories(${LIB_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
# inherit RenduCore generic build options (e.g. fPIC)
target_link_libraries(${LIB_NAME} PRIVATE rendu-dependency-interface)

set_target_properties(${LIB_NAME}
    PROPERTIES
    FOLDER
    ${FOLDER_NAME})

