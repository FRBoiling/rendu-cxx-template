#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# Need to pass old ${CMAKE_BINARY_DIR} as param because its different at build stage
set(project-name cores)

add_custom_target(revision_data.h ALL
    COMMAND "${CMAKE_COMMAND}" -DBUILDDIR="${CMAKE_BINARY_DIR}" -P "${CMAKE_SOURCE_DIR}/cmake/genrev.cmake" "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )

set_target_properties(revision_data.h
    PROPERTIES
    FOLDER
    ${project-name})
