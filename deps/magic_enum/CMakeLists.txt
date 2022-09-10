#**********************************
#  Created by boil on 2022/9/10.
#**********************************

set(target-name magic_enum)
set(project-name deps)

include(CheckSymbolExists)
CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    private_sources
    # Exclude
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    public_includes
)

add_library(${target-name} STATIC ${private_sources} ${public_includes})
add_library(${target-name}::${target-name} ALIAS ${target-name})
target_include_directories(${target-name}
    PUBLIC
    ${public_includes}
    )

add_library(${target-name}-header-only INTERFACE)
add_library(${target-name}::${target-name}-header-only ALIAS ${target-name}-header-only)
target_compile_definitions(${target-name}-header-only INTERFACE FMT_HEADER_ONLY=1)
target_include_directories(${target-name}-header-only
    INTERFACE
    ${public_includes}
    )

set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${target-name}
    PRIVATE rendu-dependency-interface
    )

set_target_properties(${target-name}
    PROPERTIES
    FOLDER
    ${project-name})