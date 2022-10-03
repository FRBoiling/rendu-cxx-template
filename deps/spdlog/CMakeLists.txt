#**********************************
#  Created by boil on 2022/9/10.
#**********************************

set(target-name spdlog)
set(project-name deps)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    private_sources
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    public_includes
)

add_library(${target-name} ${private_sources} ${public_includes})

target_compile_definitions(${target-name}
    PUBLIC
    -DSPDLOG_COMPILED_LIB
#    -DSPDLOG_USE_STD_FORMAT
    )

#if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
#  target_compile_definitions(${target-name} INTERFACE -DRAPIDJSON_SSE2)
#elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
#  target_compile_definitions(${target-name} INTERFACE -DRAPIDJSON_NEON)
#elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
#  target_compile_definitions(${target-name} INTERFACE -DRAPIDJSON_NEON)
#endif ()

target_link_libraries(${target-name}
    PRIVATE rendu-dependency-interface
    PUBLIC threads
    )

set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${target-name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${target-name} PROPERTIES FOLDER ${project-name})