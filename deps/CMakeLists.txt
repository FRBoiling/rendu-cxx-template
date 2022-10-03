#**********************************
#  Created by boil on 2022/8/14.
#**********************************

AddSubdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Exclude
)

include(FetchContent)
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
    GIT_SHALLOW 1)
FetchContent_MakeAvailable(Catch2)
