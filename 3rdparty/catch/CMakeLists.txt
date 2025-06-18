if(RENDU_BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1
    GIT_SHALLOW    1)
  FetchContent_MakeAvailable(Catch2)

# 在 include(Catch) 之前加上：
    if(EXISTS "${Catch2_SOURCE_DIR}/extras/Catch.cmake")
        list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
    elseif(EXISTS "${Catch2_SOURCE_DIR}/contrib/Catch.cmake")
        list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/contrib")
    endif()

    include(Catch)

#     add_library(catch INTERFACE)
# target_link_libraries(catch
#   INTERFACE
#    Catch2::Catch2)

  get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${FOLDER_NAME}
    LINKOPTS
    rendu-dependency-interface
    DEPS
    Catch2::Catch2
  )

endif()