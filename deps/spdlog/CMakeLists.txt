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


 if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
 target_compile_definitions(${target-name}
    PUBLIC
    -DSPDLOG_COMPILED_LIB
    -DSPDLOG_USE_STD_FORMAT
    )
 else()
 target_compile_definitions(${target-name}
    PUBLIC
    -DSPDLOG_COMPILED_LIB
#    -DSPDLOG_USE_STD_FORMAT
    )
endif ()



target_link_libraries(${target-name}
    PRIVATE rendu-dependency-interface
    PUBLIC threads
    )

set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${target-name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${target-name} PROPERTIES FOLDER ${project-name})