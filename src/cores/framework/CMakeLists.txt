set(LIB_NAME framework)
CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES
  # Exclude
  ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

if(USE_COREPCH)
  set(PRIVATE_PCH_HEADER precompiled_headers/${LIB_NAME}_pch.h)
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_SHARED)

add_library(${LIB_NAME}
  ${PRIVATE_SOURCES}
)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES
  # Exclude
  ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(${LIB_NAME}
  PUBLIC
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${LIB_NAME}
  PRIVATE
    rendu-interface
    common
  PUBLIC
    #TODO:添加reps库
    )

 set_target_properties(${LIB_NAME}
     PROPERTIES
       FOLDER
         "rendu")

 if(BUILD_SHARED_LIBS)
   if(UNIX)
     install(TARGETS shared
       LIBRARY
         DESTINATION lib)
   elseif(WIN32)
     install(TARGETS shared
       RUNTIME
         DESTINATION "${CMAKE_INSTALL_PREFIX}")
   endif()
 endif()

# Generate precompiled header
if(USE_COREPCH)
  add_cxx_pch(${LIB_NAME} ${PRIVATE_PCH_HEADER})
endif()