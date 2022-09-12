#**********************************
#  Created by boil on 2022/9/10.
#**********************************

set(target-name rapidjson)
set(project-name deps)

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    private_sources
)

CollectIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    public_includes
)

add_library(${target-name} INTERFACE ${private_sources} ${public_includes})


target_compile_definitions(${target-name}
    INTERFACE
    -DRAPIDJSON_HAS_STDSTRING
    -DRAPIDJSON_48BITPOINTER_OPTIMIZATION=0
    -DRAPIDJSON_ASSERT=WPAssert
    )

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
  target_compile_definitions(${target-name} INTERFACE -DRAPIDJSON_SSE2)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  target_compile_definitions(${target-name} INTERFACE -DRAPIDJSON_NEON)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  target_compile_definitions(${target-name} INTERFACE -DRAPIDJSON_NEON)
endif ()

target_link_libraries(${target-name}
    INTERFACE rendu-dependency-interface
    #    PUBLIC
    )

target_include_directories(${target-name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
#set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${target-name} PROPERTIES FOLDER ${project-name})
