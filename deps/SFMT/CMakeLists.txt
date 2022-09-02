#**********************************
#  Created by boil on 2022/8/14.
#**********************************

CollectSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SFMT_SOURCES
    # Exclude
)

add_library(sfmt STATIC ${SFMT_SOURCES})

target_include_directories(sfmt
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})

# using the standard Mersenne exponent 19937
target_compile_definitions(sfmt PUBLIC -DSFMT_MEXP=19937)

# enable SIMD instructions if available
include(CheckCCompilerFlag)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "(powerpc|ppc)64|(powerpc|ppc)64le")
  check_c_compiler_flag("-maltivec" HAVE_ALTIVEC)
  if (HAVE_ALTIVEC)
    target_compile_options(sfmt PRIVATE -mabi=altivec -maltivec)
    target_compile_definitions(sfmt PUBLIC -DHAVE_ALTIVEC)
  else ()
    message(WARNING "Altivec not available - performance will be poor!")
  endif ()
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
  if (APPLE)
    message("performance is apple m1 max")
  else ()
    check_c_compiler_flag(-mfpu=neon HAVE_NEON)
    if (HAVE_NEON)
      target_compile_options(sfmt PRIVATE -mfpu=neon -ftree-vectorize)
      target_compile_definitions(sfmt PUBLIC -DHAVE_NEON)
    else ()
      message(WARNING "Neon not available - performance will be poor!")
    endif ()
  endif ()
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|AARCH64")
  check_c_compiler_flag(-march=armv8-a+simd HAVE_NEON)
  if (HAVE_NEON)
    target_compile_options(sfmt PRIVATE -ftree-vectorize)
    target_compile_definitions(sfmt PUBLIC -DHAVE_NEON)
  else ()
    message(WARNING "Neon not available - performance will be poor!")
  endif ()
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i686|amd64|x86_64|AMD64")
  #SSE2 is always available
  set(HAVE_SSE2 1)

  if (NOT CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(sfmt PRIVATE -msse2)
  endif ()
  target_compile_definitions(sfmt PUBLIC -DHAVE_SSE2)
endif ()

set_target_properties(sfmt PROPERTIES LINKER_LANGUAGE CXX)

# inherit trinitycore generic build options (e.g. fPIC)
target_link_libraries(sfmt PRIVATE rendu-dependency-interface)

set_target_properties(sfmt
    PROPERTIES
    FOLDER
    "deps")