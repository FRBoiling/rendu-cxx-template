#**********************************
#  Created by boil on 2022/9/25.
#**********************************

set(project-name tests)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    TEST_SOURCES
)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${project-name} ${TEST_SOURCES})

target_link_libraries(${project-name}
    PRIVATE
    rendu-core-interface
    common
    Catch2::Catch2)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    TEST_INCLUDES)

target_include_directories(${project-name}
    PUBLIC
    ${TEST_INCLUDES}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

catch_discover_tests(${project-name})

set_target_properties(${project-name}
    PROPERTIES
    FOLDER
    ${project-name})
