#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(EXEC_NAME example_enum)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    private_sources
)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${EXEC_NAME}
    ${private_sources}
    )

target_link_libraries(${EXEC_NAME}
    PRIVATE
    rendu-core-interface
    PUBLIC
    common
    #TODO:BOIL 添加依赖项
    )

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    public_includes
)

target_include_directories(${EXEC_NAME}
    PUBLIC
    ${public_includes}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

