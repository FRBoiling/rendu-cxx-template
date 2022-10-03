#**********************************
#  Created by boil on 2022/9/10.
#**********************************

set(target-name sfmt)
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

add_library(${target-name} STATIC ${private_sources} ${public_includes})

include(CheckCCompilerFlag)
target_compile_definitions(${target-name} PRIVATE -DSFMT_MEXP=19937)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)|(^i.86$)")
  #SSE2 is always available
  set(HAVE_SSE2 1)
  if (NOT CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${target-name} PRIVATE -msse2)
  endif ()
  target_compile_definitions(${target-name} PRIVATE -DHAVE_SSE2)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  check_c_compiler_flag(-mfpu=neon HAVE_NEON)
  if (HAVE_NEON)
    target_compile_options(${target-name} PRIVATE -mfpu=neon -ftree-vectorize)
    target_compile_definitions(${target-name} PRIVATE -DHAVE_NEON)
  else ()
    message(WARNING "Neon not available - performance will be poor!")
  endif ()
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
  check_c_compiler_flag(-march=armv8-a+simd HAVE_NEON)
  if (HAVE_NEON)
    target_compile_options(${target-name} PRIVATE -ftree-vectorize)
    target_compile_definitions(${target-name} PRIVATE -DHAVE_NEON)
  else ()
    message(WARNING "Neon not available - performance will be poor!")
  endif ()
endif ()

target_link_libraries(${target-name}
    PRIVATE rendu-dependency-interface
#    PUBLIC
    )

set_target_properties(${target-name} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${target-name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${target-name} PROPERTIES FOLDER ${project-name})
