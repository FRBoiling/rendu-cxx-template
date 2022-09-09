#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(LIB_NAME fmt)
set(FOLDER_NAME deps)

include(CheckSymbolExists)

set(strtod_l_headers stdlib.h)

if (APPLE)
  set(strtod_l_headers ${strtod_l_headers} xlocale.h)
endif ()

if (WIN32)
  check_symbol_exists(_strtod_l "${strtod_l_headers}" HAVE_STRTOD_L)
else ()
  check_symbol_exists(strtod_l "${strtod_l_headers}" HAVE_STRTOD_L)
endif ()

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES
    # Exclude
)

add_library(${LIB_NAME} STATIC ${SOURCES})

if (HAVE_STRTOD_L)
  target_compile_definitions(${LIB_NAME}
      PUBLIC
      FMT_LOCALE)
endif ()

target_include_directories(${LIB_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIB_NAME}
    PRIVATE
    rendu-dependency-interface)

set_target_properties(${LIB_NAME}
    PROPERTIES
    FOLDER
    ${FOLDER_NAME})
