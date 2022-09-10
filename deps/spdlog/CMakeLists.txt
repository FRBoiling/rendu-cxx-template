#**********************************
#  Created by boil on 2022/9/10.
#**********************************

set(target-name spdlog)
set(project-name deps)

include(CheckSymbolExists)
CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    private_sources
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    public_includes
)

add_library(${target-name} STATIC ${private_sources} ${public_includes})
add_library(${target-name}::${target-name} ALIAS ${target-name})
#target_compile_definitions(${target-name} PUBLIC SPDLOG_COMPILED_LIB SPDLOG_FMT_EXTERNAL)
target_compile_definitions(${target-name} PUBLIC SPDLOG_COMPILED_LIB)
target_include_directories(${target-name}
    PUBLIC
    ${public_includes}
    )

set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${target-name}
    PRIVATE rendu-dependency-interface
    PUBLIC fmt::fmt
    )

set_target_properties(${target-name}
    PROPERTIES
    FOLDER
    ${project-name})