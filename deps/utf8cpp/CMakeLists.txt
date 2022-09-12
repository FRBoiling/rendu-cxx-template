#**********************************
#  Created by boil on 2022/9/10.
#**********************************

set(target-name utf8cpp)
set(project-name deps)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    private_sources
    # Exclude
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    public_includes
)

add_library(${target-name} INTERFACE ${private_sources} ${public_includes})

target_link_libraries(${target-name}
    INTERFACE rendu-dependency-interface
    #    PUBLIC
    )

target_include_directories(${target-name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
#set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target-name} PROPERTIES FOLDER ${project-name})