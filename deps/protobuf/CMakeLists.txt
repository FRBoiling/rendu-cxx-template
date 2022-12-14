#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(target-name protobuf)
set(project-name deps)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    private_sources
    # Exclude
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    public_includes
    # Exclude
)
add_library(${target-name} INTERFACE ${private_sources} ${public_includes})


if (BUILD_SHARED_LIBS)
  target_compile_definitions(${target-name}
      PRIVATE
      -DLIBPROTOBUF_EXPORTS
      -DLIBPROTOC_EXPORTS
      PUBLIC
      -DPROTOBUF_USE_DLLS)
endif ()

if (MSVC)
  target_compile_options(${target-name}
      PRIVATE
      /wd4996)

  target_compile_definitions(${target-name}
      PRIVATE
      -D_SCL_SECURE_NO_WARNINGS)
endif ()

target_link_libraries(${target-name}
    INTERFACE rendu-dependency-interface
    #    PUBLIC
    )
set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${target-name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${target-name} PROPERTIES FOLDER ${project-name})
