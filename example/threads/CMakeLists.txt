#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target_name example_threads)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${target_name}
    ${private_sources}
    )

target_link_libraries(${target_name}
    PRIVATE
    rendu-interface
    PUBLIC
    threads
    #TODO:BOIL 添加依赖项
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    public_includes
)

target_include_directories(${target_name}
    PUBLIC
    ${public_includes}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

