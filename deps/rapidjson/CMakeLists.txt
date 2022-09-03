#**********************************
#  Created by boil on 2022/8/14.
#**********************************

add_library(rapidjson INTERFACE)

target_include_directories(rapidjson
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(rapidjson
    INTERFACE
    -DRAPIDJSON_HAS_STDSTRING
    -DRAPIDJSON_48BITPOINTER_OPTIMIZATION=0
    -DRAPIDJSON_ASSERT=WPAssert)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
  target_compile_definitions(rapidjson INTERFACE -DRAPIDJSON_SSE2)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  target_compile_definitions(rapidjson INTERFACE -DRAPIDJSON_NEON)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  target_compile_definitions(rapidjson INTERFACE -DRAPIDJSON_NEON)
endif ()
